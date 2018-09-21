class World:
    def render_perspective(self):
        # No extra sampling
        for r in range(0, self.view_plane.vres):
            for c in range(0, self.view_plane.hres):
                x = self.view_plane.pixel_size * (c - 0.5 * (self.view_plane.hres - 1.0))
                y = self.view_plane.pixel_size * (r - 0.5 * (self.view_plane.vres - 1.0))
                ray.origin = np.array([x, y, zw])
                pixel_color = self.tracer.trace_ray(ray)
                png[r, c] = pixel_color
        
        spm.imsave('out_img.png', png)

    def build(self):
        # Single Sphere
        self.view_plane = ViewPlane(500, 500, 1.0, 1.0)
        self.background_color = RGBColor.black
        self.objects = Sphere(np.array([0,0,0]), 160)
        self.tracer = SingleSphereTracer(self)
        
        # For posterity
        # Multiple Spheres
        #sampler = Jittered(9)
        #sampler = NRooks(9)
        sampler = MultiJittered(9)
        self.view_plane = ViewPlane(60, 60, 2.0, 1.0, sampler)
        self.eye = np.array(500)
        self.view_distance = 200
        self.background_color = RGBColor.black
        self.objects = [Sphere(np.array([-45, 45, 40]), 50, RGBColor.yellow), 
                        Sphere(np.array([0,30,0]), 15, RGBColor.red),
                        Triangle(np.array([-110, -85, 80]), np.array([120, 10, 20]), np.array([-40, 50, -30]), RGBColor.blue)]
                        #Plane(np.array([0,0,0]), np.array([0,5,1]), RGBColor.green),
        self.tracer = MultipleObjectsTracer(self)
        

    def render_scene(self):
        zw = 100
        ray = Ray(np.array([0,0,0]), np.array([0,0,-1]))
        png = np.array([[RGBColor.clear for i in range(0,self.view_plane.vres)] for j in range(0,self.view_plane.hres)])
        num_samples = self.view_plane.num_samples

        # For each pixel
        for r in range(0, self.view_plane.vres):
            for c in range(0, self.view_plane.hres):

                # Sample num_samples times
                pixel_color = RGBColor.clear
                for j in range(0, num_samples):
                    sp = self.view_plane.sampler.sample_unit_square()
                    x = self.view_plane.pixel_size * (c - 0.5 * self.view_plane.hres + sp[0])
                    y = self.view_plane.pixel_size * (r - 0.5 * self.view_plane.vres + sp[1])
                    ray.origin = np.array([x, y, zw])
                    pixel_color = pixel_color + self.tracer.trace_ray(ray)
                png[r, c] = pixel_color / num_samples
        
        # Save to png
        spm.imsave('out_img.png', png)

    def render_perspective(self):
        ray = Ray(np.array([0,0,self.eye]), np.array([0,0,-1]))
        png = np.array([[RGBColor.clear for i in range(0,self.view_plane.vres)] for j in range(0,self.view_plane.hres)])
        num_samples = self.view_plane.num_samples

        # For each pixel
        for r in range(0, self.view_plane.vres):
            for c in range(0, self.view_plane.hres):

                # Sample num_samples times
                pixel_color = RGBColor.clear
                for j in range(0, num_samples):
                    sp = self.view_plane.sampler.sample_unit_square()
                    x = self.view_plane.pixel_size * (c - 0.5 * self.view_plane.hres + sp[0])
                    y = self.view_plane.pixel_size * (r - 0.5 * self.view_plane.vres + sp[1])
                    ray.direction = np.array([x, y, -self.camera.view_distance])
                    ray.direction = normalize(ray.direction)
                    pixel_color = pixel_color + self.tracer.trace_ray(ray)
                png[r, c] = pixel_color / num_samples
        
        # Save to png
        spm.imsave('out_img.png', png)


class ViewPlane:
    # TODO: Not needed
    def set_sampler(self, sampler):
        self.num_samples = sampler.num_samples
        self.sampler = sampler

    # TODO: Not needed
    def set_samples(self, num_samples):
        self.num_samples = num_samples
        if (num_samples <= 1):
            self.sampler = SingleSample(1)
        else:
            self.sampler = MultiJittered(num_samples)

class SingleSphereTracer(Tracer):
    def trace_ray(self, ray):
        sr = ShadeRec(self.world)
        if(self.world.objects.hit(ray, sr)):
            return RGBColor.red
        else:
            return RGBColor.black
import numpy as np
import scipy.misc as spm
import time
from abc import abstractmethod

class Ray:
    def __init__(self, origin, direction):
        self.origin = origin
        self.direction = direction

class GeometricObject:
    @abstractmethod
    def hit(self, ray, sr):
        pass

class Plane(GeometricObject):
    def __init__(self, point, normal, color):
        self.point = point
        self.normal = normal
        self.color = color
        self.k_epsilon = 10**-6

    def hit(self, ray, sr):
        """Alters sr
        Returns (whether hit, tmin)"""
        t = np.dot(self.point - ray.origin, self.normal) / np.dot(ray.direction, self.normal)
        if (t > self.k_epsilon):
            sr.normal = self.normal
            sr.local_hit_point = ray.origin + t*ray.direction
            return True, t
        else:
            return False, t

class Sphere(GeometricObject):
    def __init__(self, center, radius, color):
        self.center = center
        self.radius = radius
        self.color = color
        self.k_epsilon = 10**-6

    def hit(self, ray, sr):
        """Alters sr
        Returns (whether hit, tmin)"""
        temp = ray.origin - self.center
        a = np.dot(ray.direction, ray.direction)
        b = 2.0 * np.dot(temp, ray.direction)
        c = np.dot(temp, temp) - self.radius * self.radius
        disc = b * b - 4.0 * a * c

        if(disc < 0.0):
            return False, None
        else: 
            e = disc**0.5
            denom = 2.0 * a
            # Smaller root
            t = (-b - e) / denom
            if (t > self.k_epsilon):
                sr.normal = (temp + t * ray.direction) / self.radius
                sr.local_hit_point = ray.origin + t * ray.direction
                return True, t
            # Larger root
            t = (-b + e) / denom
            if (t > self.k_epsilon):
                sr.normal = (temp + t * ray.direction) / self.radius
                sr.local_hit_point = ray.origin + t * ray.direction
                return True, t
        return False, None

class ShadeRec:
    def __init__(self, world):
        self.hit_an_object = False
        self.local_hit_point = None
        self.normal = None
        self.color = RGBColor.black
        self.world = world

class RGBColor:
    clear = np.array([0,0,0,0])
    black = np.array([0,0,0,255])
    white = np.array([255,255,255,255])
    red = np.array([255,0,0,255])
    green = np.array([0,255,0,255])
    blue = np.array([0,0,255,255])
    cyan = np.array([0,255,255,255])
    magenta = np.array([255,0,255,255])
    yellow = np.array([255,255,0,255])
    def __init__(self, r, b, g, a):
        self.rgba = np.array([r,g,b,a])

class World:
    def __init__(self):
        self.view_plane = None
        self.background_color = None
        self.objects = None
        self.tracer = None

    def build(self):
        """
        # Green Plane
        self.view_plane = ViewPlane(200, 200, 1.0, 1.0)
        self.background_color = RGBColor.black
        self.objects = Plane(np.array([0,0,0]), np.array([0,1,1]))
        self.tracer = GreenScreenTracer()
        """

        """
        # Single Sphere
        self.view_plane = ViewPlane(500, 500, 1.0, 1.0)
        self.background_color = RGBColor.black
        self.objects = Sphere(np.array([0,0,0]), 160)
        self.tracer = SingleSphereTracer(self)
        """

        # Multiple Spheres
        self.view_plane = ViewPlane(300, 300, 1.0, 1.0)
        self.background_color = RGBColor.black
        self.objects = [Sphere(np.array([0,-25,0]), 80, RGBColor.red), 
                        Sphere(np.array([0,30,0]), 60, RGBColor.yellow),
                        Plane(np.array([0,0,0]), np.array([0,1,1]), RGBColor.green)]
        self.tracer = MultipleObjectsTracer(self)

    def render_scene(self):
        zw = 100
        ray = Ray(np.array([0,0,0]), np.array([0,0,-1]))
        png = np.array([[RGBColor.clear for i in range(0,self.view_plane.vres)] for j in range(0,self.view_plane.hres)])

        for r in range(0, self.view_plane.vres):
            for c in range(0, self.view_plane.hres):
                x = self.view_plane.pixel_size * (c - 0.5 * (self.view_plane.hres - 1.0))
                y = self.view_plane.pixel_size * (r - 0.5 * (self.view_plane.vres - 1.0))
                ray.origin = np.array([x, y, zw])
                pixel_color = self.tracer.trace_ray(ray)
                png[r, c] = pixel_color
        
        spm.imsave('out_img.png', png)

    def hit_bare_bones_objects(self, ray):
        sr = ShadeRec(self)
        tmin = 10.0**10
        num_objects = len(self.objects)

        for i in range(0, num_objects):
            hit, t = self.objects[i].hit(ray, sr)

            if(hit and t < tmin):
                sr.hit_an_object = True
                tmin = t
                sr.color = self.objects[i].color

        return sr

class ViewPlane:
    def __init__(self, hres, vres, pixel_size, gamma):
        self.hres = hres
        self.vres = vres
        self.pixel_size = pixel_size
        self.gamma = gamma

class Tracer:
    def trace_ray(self, ray):
        return RGBColor.black

class GreenScreenTracer(Tracer):
    def trace_ray(self, ray):
        return RGBColor.green

class SingleSphereTracer(Tracer):
    def __init__(self, world):
        self.world = world

    def trace_ray(self, ray):
        sr = ShadeRec(self.world)
        if(self.world.objects.hit(ray, sr)):
            return RGBColor.red
        else:
            return RGBColor.black

class MultipleObjectsTracer(Tracer):
    def __init__(self, world):
        self.world = world

    def trace_ray(self, ray):
        sr = self.world.hit_bare_bones_objects(ray)
        if(sr.hit_an_object):
            return sr.color
        else:
            return self.world.background_color


#================= Run ray tracer ======================
start_time = time.time()
w = World()
w.build()
w.render_scene()
end_time = time.time()
print(end_time - start_time)

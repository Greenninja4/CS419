import numpy as np
import scipy.misc as spm
import time
import random
import math
import sys
import copy
from abc import abstractmethod

# Taken from stack overflow (https://stackoverflow.com/questions/21030391/how-to-normalize-an-array-in-numpy)
def normalize(v):
    norm = np.linalg.norm(v, ord=1)
    if norm == 0:
        norm = np.finfo(v.dtype).eps
    return v / norm

class Ray:
    def __init__(self, origin, direction):
        self.origin = origin
        self.direction = direction

class GeometricObject:
    @abstractmethod
    def hit(self, ray, sr):
        # SIDE EFFECT: updates sr (hit_an_object, normal, local_hit_point)
        # returns (bool a_hit, float t_at_hit)"""
        pass

    @abstractmethod
    def get_normal(self, point):
        pass

class Plane(GeometricObject):
    def __init__(self, point, normal, color):
        self.point = point
        self.normal = normal
        self.color = color

        self.k_epsilon = 10**-6

    def hit(self, ray, sr):
        t = np.dot(self.point - ray.origin, self.normal) / np.dot(ray.direction, self.normal)
        if (t > self.k_epsilon):
            sr.normal = self.normal
            sr.local_hit_point = ray.origin + t*ray.direction
            return True, t
        else:
            return False, t

    def get_normal(self, point):
        return self.normal

class Triangle(GeometricObject):
    def __init__(self, p1, p2, p3, color):
        self.p1 = p1
        self.p2 = p2
        self.p3 = p3
        self.normal = np.cross(p1-p2, p1-p3)
        self.color = color
        self.k_epsilon = 10**-6

    def hit(self, ray, sr):
        """Alters sr
        Returns (whether hit, tmin)"""
        t = np.dot(self.p1 - ray.origin, self.normal) / np.dot(ray.direction, self.normal)
        # Hits plane
        if (t > self.k_epsilon):
            point = ray.origin + t * ray.direction
            temp = np.dot(np.cross(self.p2-self.p1, self.p3-self.p1), self.normal)
            b1 = np.dot(np.cross(self.p3-self.p2, point-self.p2), self.normal) / temp
            b2 = np.dot(np.cross(self.p1-self.p3, point-self.p3), self.normal) / temp
            b3 = np.dot(np.cross(self.p2-self.p1, point-self.p1), self.normal) / temp
            # Hits triangle
            if (0 < b1 and b1 < 1 and 0 < b2 and b2 < 1 and 0 < b3 and b3 < 1):
                sr.normal = self.normal
                sr.local_hit_point = ray.origin + t*ray.direction
                return True, t
        return False, t

    def get_normal(self, point):
        return self.normal

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

    def get_normal(self, point):
        normal = point - self.center
        normal = normalize(normal)
        return normal

class ShadeRec:
    def __init__(self, world):
        self.world = world

        self.hit_an_object = False
        self.local_hit_point = None
        self.normal = None
        self.color = RGBColor.black
        

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
        self.camera = None
        self.light = None
        

    def build(self):
        sampler = MultiJittered(9)
        self.view_plane = ViewPlane(60, 60, 2.0, 1.0, sampler)
        self.background_color = RGBColor.black
        self.objects = [Sphere(np.array([-45, 45, 40]), 50, RGBColor.yellow), 
                        Sphere(np.array([0,30,0]), 15, RGBColor.red),
                        Triangle(np.array([-110, -85, 80]), np.array([120, 10, 20]), np.array([-40, 50, -30]), RGBColor.blue)]
                        #Plane(np.array([0,0,0]), np.array([0,5,1]), RGBColor.green),
        self.tracer = MultipleObjectsTracer(self)
        
        eye = np.array([-1100, 400, 500])
        up = np.array([0,1,0])
        lookat = np.array([0, 0, -50])
        exposure_time = 1.0
        zoom = 1
        self.view_distance = 400
        self.camera = Orthographic(eye, up, lookat, exposure_time, zoom)
        self.camera = Pinhole(eye, up, lookat, exposure_time, zoom)
        self.light = np.array([-100,-100,100])
        
        
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
                    ray.direction = np.array([x, y, -self.view_distance])
                    ray.direction = normalize(ray.direction)
                    pixel_color = pixel_color + self.tracer.trace_ray(ray)
                png[r, c] = pixel_color / num_samples
        
        # Save to png
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
                               
                shader = NoShader()
                point = ray.origin + t * ray.direction
                light = self.light
                k = 1
                L = normalize(light - point)
                N = normalize(self.objects[i].get_normal(point))
                i = self.objects[i].color
                sr.color = shader.shade(k, L, N, i)

        return sr

class ViewPlane:
    def __init__(self, hres, vres, pixel_size, gamma, sampler):
        self.hres = hres
        self.vres = vres
        self.pixel_size = pixel_size
        self.gamma = gamma
        self.num_samples = sampler.num_samples
        self.sampler = sampler

    def set_sampler(self, sampler):
        self.num_samples = sampler.num_samples
        self.sampler = sampler

    def set_samples(self, num_samples):
        self.num_samples = num_samples
        if (num_samples <= 1):
            self.sampler = SingleSample(1)
        else:
            self.sampler = MultiJittered(num_samples)

class Tracer:
    def trace_ray(self, ray):
        return RGBColor.clear

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

class Sampler:
    def __init__(self, num_samples):
        self.num_samples = num_samples
        self.num_sets = 83 # Magic boi prime number
        self.samples = np.ndarray((self.num_samples * self.num_sets, 2))
        self.generate_samples()
        self.shuffled_indices = None
        self.count = 0
        self.jump = (random.randint(0, sys.maxsize) % self.num_sets) * self.num_samples

    @abstractmethod
    def generate_samples(self):
        pass

    def setup_shuffled_indices(self):
        pass

    def shuffle_samples(self):
        pass

    def sample_unit_square(self):
        if (self.count % self.num_samples == 0): # Start of a new pixel
            self.jump = (random.randint(0, sys.maxsize) % self.num_sets) * self.num_samples

        sample = self.samples[self.jump + self.count % self.num_samples]
        self.count += 1
        return sample

class SingleSample(Sampler):
    def generate_samples(self):
        pass

class Regular(Sampler):
    def generate_samples(self):
        pass        
        
class Jittered(Sampler):
    def generate_samples(self):
        # Sub pixel number in each direction of square
        n = int(self.num_samples**0.5)
        # Run through sampler sets
        for p in range(0, self.num_sets):
            # Run through (x,y) in subpixels
            for j in range(0, n):
                for k in range(0, n):
                    x = (k + random.random()) / n
                    y = (j + random.random()) / n
                    sp = np.array([x, y])
                    self.samples[p*self.num_samples + j*n + k] = sp

class NRooks(Sampler):
    def generate_samples(self):
        # Run through sampler sets
        for p in range(0, self.num_sets):
            # Run through num_samples to generate diagonal rooks
            for j in range(0, self.num_samples):
                x = (j + random.random()) / self.num_samples
                y = (j + random.random()) / self.num_samples
                sp = np.array([x, y])
                self.samples[p*self.num_samples + j] = sp
        self.shuffle_x_coordinates()
        self.shuffle_y_coordinates()

    def shuffle_x_coordinates(self):
        # Run through each sample set
        for p in range(0, self.num_sets):
            # Run through each sample in set
            for i in range(0, self.num_samples - 1):
                target = random.randint(0, sys.maxsize) % self.num_samples + p*self.num_samples
                temp = self.samples[i + p*self.num_samples + 1][0]
                self.samples[i + p*self.num_samples + 1][0] = self.samples[target][0]
                self.samples[target][0] = temp

    def shuffle_y_coordinates(self):
        # Run through each sample set
        for p in range(0, self.num_sets):
            # Run through each sample in set
            for i in range(0, self.num_samples - 1):
                target = random.randint(0, sys.maxsize) % self.num_samples + p*self.num_samples
                temp = self.samples[i + p*self.num_samples + 1][1]
                self.samples[i + p*self.num_samples + 1][1] = self.samples[target][1]
                self.samples[target][1] = temp


# CORRELATED Multi-jittered (x and y coors are mixed using the same randomization) -> mix shuffling coordinates together
# Regular Multi-jittered
# Algorithm from https://graphics.pixar.com/library/MultiJitteredSampling/paper.pdf
class MultiJittered(Sampler):
    def generate_samples(self):
        n = int(self.num_samples**0.5)

        # Run through each sample set
        for p in range(0, self.num_sets):
            # Run through each psuedo-diagonal sub pixel
            for j in range(0, n):
                for k in range(0, n):
                    x = (j + (k + random.random()) / n) / n
                    y = (k + (j + random.random()) / n) / n
                    sp = np.array([x, y])
                    self.samples[p*self.num_samples + j*n + k] = sp
        self.shuffle_x_coordinates()
        self.shuffle_y_coordinates()

    def shuffle_x_coordinates(self):
        n = int(self.num_samples**0.5)
        # Run through each sample set
        for p in range(0, self.num_sets):
            # Run through each psuedo-diagonal sub pixel
            for j in range(0, n):
                for i in range(0, n):
                    k = j + (random.randint(0, sys.maxsize) % self.num_samples) * (n-j)
                    temp = self.samples[j*n + i][0]
                    self.samples[j*n + i][0] = self.samples[k*n + i][0]
                    self.samples[k*n + i][0] = temp
                
    def shuffle_y_coordinates(self):
        n = int(self.num_samples**0.5)
        # Run through each sample set
        for p in range(0, self.num_sets):
            # Run through each psuedo-diagonal sub pixel
            for i in range(0, n):
                for j in range(0, n):
                    k = i + (random.randint(0, sys.maxsize) % self.num_samples) * (n-i)
                    temp = self.samples[j*n + i][0]
                    self.samples[j*n + i][0] = self.samples[j*n + k][0]
                    self.samples[j*n + k][0] = temp


class Camera:
    def __init__(self, eye, up, lookat, exposure_time, zoom):
        self.eye = eye
        self.lookat = lookat
        self.up = up
        self.u = None
        self.v = None
        self.w = None
        self.compute_uvw()
        self.exposure_time = exposure_time
        self.zoom = zoom

    def compute_uvw(self):
        w = self.eye - self.lookat
        w = normalize(w)
        u = np.cross(self.up, w)
        u = normalize(u)
        v = np.cross(w, u)
        self.u = u
        self.v = v
        self.w = w

    @abstractmethod
    def render_scene(self):
        pass

class Orthographic(Camera):
    def render_scene(self, world):
        pixel_color = RGBColor.clear
        view_plane = copy.deepcopy(world.view_plane)
        zw = 100
        ray = Ray(np.array([0,0,0]), np.array([0,0,-1]))
        num_samples = view_plane.num_samples
        view_plane.pixel_size = view_plane.pixel_size / self.zoom
        png = np.array([[RGBColor.clear for i in range(0,view_plane.vres)] for j in range(0,world.view_plane.hres)])
        
        for r in range(0, view_plane.vres):
            for c in range(0, view_plane.hres):

                pixel_color = RGBColor.clear
                for j in range(0, num_samples):
                    sp = view_plane.sampler.sample_unit_square()
                    x = view_plane.pixel_size * (c - 0.5 * view_plane.hres + sp[0])
                    y = view_plane.pixel_size * (r - 0.5 * view_plane.vres + sp[1])
                    pp = np.array([x,y])
                    ray.origin = np.array([x,y,zw])
                    pixel_color = pixel_color + world.tracer.trace_ray(ray)
                pixel_color = pixel_color / num_samples
                pixel_color = pixel_color * self.exposure_time
                png[r, c] = pixel_color
        spm.imsave('out_img.png', png)

class Pinhole(Camera):
    def render_scene(self, world):
        pixel_color = RGBColor.clear
        view_plane = copy.deepcopy(world.view_plane)
        ray = Ray(self.eye, np.array([0,0,-1]))
        num_samples = world.view_plane.num_samples
        view_plane.pixel_size = view_plane.pixel_size / self.zoom
        png = np.array([[RGBColor.clear for i in range(0,world.view_plane.vres)] for j in range(0,world.view_plane.hres)])
        
        for r in range(0, view_plane.vres):
            for c in range(0, view_plane.hres):
                pixel_color = RGBColor.clear
                for j in range(0, num_samples):
                    sp = view_plane.sampler.sample_unit_square()
                    x = view_plane.pixel_size * (c - 0.5 * view_plane.hres + sp[0])
                    y = view_plane.pixel_size * (r - 0.5 * view_plane.vres + sp[1])
                    pp = np.array([x,y])
                    ray.direction = self.ray_direction(pp, world.view_distance)
                    pixel_color = pixel_color + world.tracer.trace_ray(ray)
                pixel_color = pixel_color / num_samples
                pixel_color = pixel_color * self.exposure_time
                png[r, c] = pixel_color
        spm.imsave('out_img.png', png)

    def ray_direction(self, p, d):
        direction = p[0] * self.u + p[1] * self.v - d * self.w
        direction = normalize(direction)
        return direction


class Shader:
    @abstractmethod
    def shade(self, k, L, N, i):
        pass

class NoShader(Shader):
    def shade(self, k, L, N, i):
        return i

class DiffusePhongShader:
    def shade(self, k, L, N, i):
        diffuse = i * k * np.max([0, np.dot(L, N)])
        # Make alpha same
        diffuse[3] = i[3]
        # print("i: ", i, "diffuse: ", diffuse)
        return diffuse

#================= Run ray tracer ======================
start_time = time.time()
w = World()
w.build()
#w.render_scene()
#w.render_perspective()
w.camera.render_scene(w)
end_time = time.time()
print(end_time - start_time)

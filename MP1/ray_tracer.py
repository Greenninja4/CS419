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
        # SIDE EFFECT: updates sr (normal, local_hit_point)
        # returns (bool a_hit, float t_at_hit)"""
        pass

    @abstractmethod
    def get_normal(self, point):
        pass

class Plane(GeometricObject):
    def __init__(self, point, normal, color):
        self.point = point
        self.normal = normalize(normal)
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
        self.color = color

        self.normal = normalize(np.cross(p1-p2, p1-p3))
        self.k_epsilon = 10**-6

    def hit(self, ray, sr):
        t = np.dot(self.p1 - ray.origin, self.normal) / np.dot(ray.direction, self.normal)
        # Hits plane
        if (t > self.k_epsilon):
            # Calculate barycentric coordinates
            hit_point = ray.origin + t * ray.direction
            temp = np.dot(np.cross(self.p2-self.p1, self.p3-self.p1), self.normal)
            b1 = np.dot(np.cross(self.p3-self.p2, hit_point-self.p2), self.normal) / temp
            b2 = np.dot(np.cross(self.p1-self.p3, hit_point-self.p3), self.normal) / temp
            #b3 = np.dot(np.cross(self.p2-self.p1, hit_point-self.p1), self.normal) / temp
            b3 = 1 - b1 - b2
            # Hits triangle
            if (0 < b1 and b1 < 1 and 0 < b2 and b2 < 1 and 0 < b3 and b3 < 1):
                sr.normal = self.normal
                sr.local_hit_point = hit_point
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
        temp = ray.origin - self.center
        a = np.dot(ray.direction, ray.direction)
        b = 2.0 * np.dot(temp, ray.direction)
        c = np.dot(temp, temp) - self.radius * self.radius
        disc = b * b - 4.0 * a * c

        if(disc < 0.0):
            # TODO: Should None be -1?
            return False, None
        else: 
            e = np.sqrt(disc)
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
        # TODO: Should None be -1?
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
        self.color = RGBColor.clear

class RGBColor:
    # Common colors
    clear = np.array([0,0,0,0])
    black = np.array([0,0,0,255])
    white = np.array([255,255,255,255])
    red = np.array([255,0,0,255])
    green = np.array([0,255,0,255])
    blue = np.array([0,0,255,255])
    cyan = np.array([0,255,255,255])
    magenta = np.array([255,0,255,255])
    yellow = np.array([255,255,0,255])

    # TODO: standardize interface with common colors
    def __init__(self, r, b, g, a):
        self.rgba = np.array([r,g,b,a])

class World:
    def __init__(self):
        self.view_plane = None
        self.background_color = None
        self.objects = None
        self.tracer = None
        self.camera = None
        # TODO: Make multiple lights
        self.light = None
        self.shader = None
        
    def build(self):
        sampler = SingleSample(1)
        #sampler = MultiJittered(9)
        self.view_plane = ViewPlane(100, 100, 1.0, 1.0, sampler)
        self.background_color = RGBColor.black
        self.objects = [Sphere(np.array([-45, 45, 40]), 50, RGBColor.yellow), 
                        Sphere(np.array([45, 45, 20]), 30, RGBColor.red),
                        Triangle(np.array([-110, -85, 80]), np.array([120, 10, 20]), np.array([-40, 50, -30]), RGBColor.blue),
                        Triangle(np.array([-10, -8, 80]), np.array([-120, 130, 20]), np.array([-400, 50, 30]), RGBColor.green)]
        self.tracer = MultipleObjectsTracer(self)
        
        # Camera
        eye = np.array([0, 0, 500])
        #eye = np.array([-300, -300, 800])
        up = np.array([0,1,0])
        lookat = np.array([0, 0, 0])
        exposure_time = 1.0
        zoom = .333
        view_distance = 500
        #self.camera = Orthographic(eye, up, lookat, exposure_time, zoom, view_distance)
        self.camera = Pinhole(eye, up, lookat, exposure_time, zoom, view_distance)

        self.light = np.array([50, 150, 200])
        self.shader = DiffusePhongShader()
        #self.shader = NoShader()

    def hit_bare_bones_objects(self, ray):
        sr = ShadeRec(self)
        tmin = sys.maxsize
        num_objects = len(self.objects)

        # Run through each object
        for i in range(0, num_objects):
            hit, t = self.objects[i].hit(ray, sr)
            # If there is a hit and it is the closest object
            if(hit and t < tmin):
                sr.hit_an_object = True
                tmin = t
                point = ray.origin + t * ray.direction
                light = self.light
                # TODO: Make a property of the object
                k = 1
                L = normalize(light - point)
                N = normalize(self.objects[i].get_normal(point))
                ill = self.objects[i].color
                sr.color = self.shader.shade(k, L, N, ill)
        return sr

class ViewPlane:
    def __init__(self, hres, vres, pixel_size, gamma, sampler):
        self.hres = hres
        self.vres = vres
        self.pixel_size = pixel_size
        self.gamma = gamma
        self.sampler = sampler

        self.num_samples = sampler.num_samples
        
class Tracer:
    def __init__(self, world):
        self.world = world

    @abstractmethod
    def trace_ray(self, ray):
        pass

class MultipleObjectsTracer(Tracer):
    def trace_ray(self, ray):
        sr = self.world.hit_bare_bones_objects(ray)
        if(sr.hit_an_object):
            return sr.color
        else:
            return self.world.background_color

class Sampler:
    def __init__(self, num_samples):
        self.num_samples = num_samples

        # TODO: Magic boi prime number
        self.num_sets = 83 
        self.samples = np.ndarray((self.num_samples * self.num_sets, 2))
        # One time generation of randomized samples
        self.generate_samples()
        self.count = 0
        self.jump = (random.randint(0, sys.maxsize) % self.num_sets) * self.num_samples

    @abstractmethod
    def generate_samples(self):
        pass

    def sample_unit_square(self):
        # Start of a new pixel
        if (self.count % self.num_samples == 0):
            self.jump = (random.randint(0, sys.maxsize) % self.num_sets) * self.num_samples
        sample = self.samples[self.jump + self.count % self.num_samples]
        self.count += 1
        return sample

class SingleSample(Sampler):
    def generate_samples(self):
        # Sub pixel number in each direction of square
        n = int(self.num_samples**0.5)
        # Run through sampler sets
        for p in range(0, self.num_sets):
            # Run through (x,y) in subpixels
            for j in range(0, n):
                for k in range(0, n):
                    x = (k) / n
                    y = (j) / n
                    sp = np.array([x, y])
                    self.samples[p*self.num_samples + j*n + k] = sp
        
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

# Algorithm from https://graphics.pixar.com/library/MultiJitteredSampling/paper.pdf
# TODO: Correlated multi-jittered samples are slightly more efficient when shuffling indexes
# This is regular (non-correlated) multi-jittered
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
    def __init__(self, eye, up, lookat, exposure_time, zoom, view_distance):
        self.eye = eye
        self.up = up
        self.lookat = lookat
        self.exposure_time = exposure_time
        self.zoom = zoom
        self.view_distance = view_distance

        self.u = None
        self.v = None
        self.w = None
        self.compute_uvw()

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
        ray = Ray(self.eye, self.lookat)
        view_plane.pixel_size = view_plane.pixel_size / self.zoom
        png = np.array([[RGBColor.clear for i in range(0,view_plane.vres)] for j in range(0,world.view_plane.hres)])
        
        for r in range(0, view_plane.vres):
            for c in range(0, view_plane.hres):
                pixel_color = RGBColor.clear
                for j in range(0, view_plane.num_samples):
                    sp = view_plane.sampler.sample_unit_square()
                    x = view_plane.pixel_size * (c - 0.5 * view_plane.hres + sp[0])
                    y = view_plane.pixel_size * (r - 0.5 * view_plane.vres + sp[1])
                    pp = np.array([x,y])
                    ray.origin = np.array([x,y,ray.origin[2]])
                    ray.direction = self.ray_direction(pp, self.view_distance)
                    pixel_color = pixel_color + world.tracer.trace_ray(ray)
                pixel_color = pixel_color / view_plane.num_samples
                pixel_color = pixel_color * self.exposure_time
                png[r, c] = pixel_color
        spm.imsave('out_img.png', png)

    def ray_direction(self, p, d):
        direction = p[0] * self.u + p[1] * self.v - d * self.w
        direction = normalize(direction)
        return direction

class Pinhole(Camera):
    def render_scene(self, world):
        # Deep copy the viewplane so rendering doesn't modify it
        view_plane = copy.deepcopy(world.view_plane)
        # Only the eyepoint matters... the direction gets changed in for loops
        ray = Ray(self.eye, self.lookat)
        view_plane.pixel_size = view_plane.pixel_size / self.zoom
        png = np.array([[RGBColor.clear for i in range(0,world.view_plane.vres)] for j in range(0,world.view_plane.hres)])
        
        for r in range(0, view_plane.vres):
            for c in range(0, view_plane.hres):
                pixel_color = RGBColor.clear
                for j in range(0, view_plane.num_samples):
                    sp = view_plane.sampler.sample_unit_square()
                    x = view_plane.pixel_size * (c - 0.5 * view_plane.hres + sp[0])
                    y = view_plane.pixel_size * (r - 0.5 * view_plane.vres + sp[1])
                    pp = np.array([x,y])
                    ray.direction = self.ray_direction(pp, self.view_distance)
                    pixel_color = pixel_color + world.tracer.trace_ray(ray)
                pixel_color = pixel_color / view_plane.num_samples
                # TODO: Does this affect alpha?
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
        # Keep alpha-value the same
        diffuse[3] = i[3]
        return diffuse

#================= Run ray tracer ======================
start_time = time.time()
w = World()
w.build()
w.camera.render_scene(w)
end_time = time.time()
print("Render time (s): ", end_time - start_time)

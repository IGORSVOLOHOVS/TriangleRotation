import bpy
import os
import ctypes

epsilon = 1e-5

# Определение структуры Vector
class Vector(ctypes.Structure):
    _fields_ = [("x", ctypes.c_float),
                ("y", ctypes.c_float),
                ("z", ctypes.c_float)]

# Определение структуры Triangle
class Triangle(ctypes.Structure):
    _fields_ = [("v1", Vector),
                ("v2", Vector),
                ("v3", Vector)]

# C:\Users\User\Projects\ObjectRotationC\python\rotate.dll    
dll_path = os.path.abspath('/python/rotate.dll')
rotate = ctypes.CDLL(dll_path)


# Определение сигнатур функций
rotate.set_vector.argtypes = [ctypes.POINTER(Vector), ctypes.c_float, ctypes.c_float, ctypes.c_float]
rotate.rotate_point_around_x.argtypes = [ctypes.c_float, ctypes.POINTER(ctypes.c_float), ctypes.POINTER(ctypes.c_float)]
rotate.rotate_point_around_y.argtypes = [ctypes.c_float, ctypes.POINTER(ctypes.c_float), ctypes.POINTER(ctypes.c_float)]
rotate.rotate_point_around_z.argtypes = [ctypes.c_float, ctypes.POINTER(ctypes.c_float), ctypes.POINTER(ctypes.c_float)]
rotate.rotate_vector_around_x.argtypes = [ctypes.c_float, ctypes.POINTER(Vector)]
rotate.rotate_vector_around_y.argtypes = [ctypes.c_float, ctypes.POINTER(Vector)]
rotate.rotate_vector_around_z.argtypes = [ctypes.c_float, ctypes.POINTER(Vector)]
rotate.rotate_triabgle_around_x.argtypes = [ctypes.c_float, ctypes.POINTER(Triangle)]
rotate.rotate_triabgle_around_y.argtypes = [ctypes.c_float, ctypes.POINTER(Triangle)]
rotate.rotate_triabgle_around_z.argtypes = [ctypes.c_float, ctypes.POINTER(Triangle)]
rotate.rotate_by_diraction.argtypes = [ctypes.c_float, ctypes.c_float, ctypes.POINTER(ctypes.c_float), ctypes.POINTER(ctypes.c_float), ctypes.POINTER(ctypes.c_float)]
rotate.rotate_triangle_by_direction.argtypes = [ctypes.c_float, ctypes.c_float, ctypes.POINTER(Triangle)]

rotate.calculate_triangle_points_by_side.restype = Triangle
rotate.calculate_triangle_points_by_side.argtypes = [ctypes.c_uint8]

rotate.get_heights.argtypes = [ctypes.POINTER(Triangle), ctypes.POINTER(ctypes.c_float), ctypes.POINTER(ctypes.c_float), ctypes.POINTER(ctypes.c_float)]

def set_vector(x, y, z):
    vector = Vector()
    rotate.set_vector(ctypes.byref(vector), x, y, z)
    return vector

def set_triangle(v1, v2, v3):
    triangle = Triangle()
    triangle.v1 = v1
    triangle.v2 = v2
    triangle.v3 = v3
    return triangle

def rotate_point_around_x(angle, point):
    x = ctypes.c_float(point[0])
    y = ctypes.c_float(point[1])
    z = ctypes.c_float(point[2])
    rotate.rotate_point_around_x(angle, ctypes.byref(x), ctypes.byref(y), ctypes.byref(z))
    return [x.value, y.value, z.value]

def rotate_point_around_y(angle, point):
    x = ctypes.c_float(point[0])
    y = ctypes.c_float(point[1])
    z = ctypes.c_float(point[2])
    rotate.rotate_point_around_y(angle, ctypes.byref(x), ctypes.byref(y), ctypes.byref(z))
    return [x.value, y.value, z.value]

def rotate_point_around_z(angle, point):
    x = ctypes.c_float(point[0])
    y = ctypes.c_float(point[1])
    z = ctypes.c_float(point[2])
    rotate.rotate_point_around_z(angle, ctypes.byref(x), ctypes.byref(y), ctypes.byref(z))
    return [x.value, y.value, z.value]

def rotate_vector_around_x(angle, vector):
    rotate.rotate_vector_around_x(angle, vector)

def rotate_vector_around_y(angle, vector):
    rotate.rotate_vector_around_y(angle, vector)

def rotate_vector_around_z(angle, vector):
    rotate.rotate_vector_around_z(angle, vector)

def rotate_triangle_around_x(angle, triangle):
    rotate.rotate_triabgle_around_x(angle, triangle)

def rotate_triangle_around_y(angle, triangle):
    rotate.rotate_triabgle_around_y(angle, triangle)

def rotate_triangle_around_z(angle, triangle):
    rotate.rotate_triabgle_around_z(angle, triangle)

def rotate_by_diraction(angle_x, angle_y, point, vector, triangle):
    x = ctypes.c_float(point[0])
    y = ctypes.c_float(point[1])
    z = ctypes.c_float(point[2])
    rotate.rotate_by_diraction(angle_x, angle_y, ctypes.byref(x), ctypes.byref(y), ctypes.byref(z), vector, triangle)
    return [x.value, y.value, z.value]

def rotate_triangle_by_direction(angle_x, angle_y, triangle):
    rotate.rotate_triangle_by_direction(angle_x, angle_y, triangle)

def calculate_triangle_points_by_side(side):
    triangle = rotate.calculate_triangle_points_by_side(ctypes.c_uint8(side))
    return triangle



def get_heights(triangle):
    h1 = ctypes.c_float(0.0)
    h2 = ctypes.c_float(0.0)
    h3 = ctypes.c_float(0.0)
    
    rotate.get_heights(ctypes.byref(triangle), ctypes.byref(h1), ctypes.byref(h2), ctypes.byref(h3))
    
    return h1.value, h2.value, h3.value

def number(value):
    if abs(value) < epsilon:
        return 0.0
    return value

direction = -45
tilt = -15

new_triangle = rotate.calculate_triangle_points_by_side(3)

rotate.rotate_triangle_by_direction(direction, tilt, new_triangle)

#vertices of triangle
A = [new_triangle.v1.x, new_triangle.v1.y, new_triangle.v1.z]
B = [new_triangle.v2.x, new_triangle.v2.y, new_triangle.v2.z]
C = [new_triangle.v3.x, new_triangle.v3.y, new_triangle.v3.z]

print('New triangle:')
print(number(A[0]), number(A[1]), number(A[2]))
print(number(B[0]), number(B[1]), number(B[2]))
print(number(C[0]), number(C[1]), number(C[2]))

h1, h2, h3 = rotate.get_heights(new_triangle)

print('Heights:')
print(rotate.number(h1), rotate.number(h2), rotate.number(h3))

verts = [A, B, C,]
faces = [ (0, 1, 2),]
edges = []

# create a mesh from the vert, edge, and face data
mesh_data = bpy.data.meshes.new("triangle_data")
mesh_data.from_pydata(verts, edges, faces)

# create a object using the mesh data
mesh_obj = bpy.data.objects.new("triangle_object", mesh_data)

bpy.context.collection.objects.link(mesh_obj)

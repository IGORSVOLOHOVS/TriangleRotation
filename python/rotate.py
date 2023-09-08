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
    
dll_path = os.path.abspath('rotate.dll')
rotate = ctypes.CDLL(dll_path)

# Загрузка библиотеки
rotate = ctypes.CDLL('rotate.dll')

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

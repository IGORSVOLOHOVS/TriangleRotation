import rotate
import ctypes

print('New triangle:')

new_triangle = rotate.calculate_triangle_points_by_side(3)

print('New triangle:')
print(rotate.number(new_triangle.v1.x), rotate.number(new_triangle.v1.y), rotate.number(new_triangle.v1.z))
print(rotate.number(new_triangle.v2.x), rotate.number(new_triangle.v2.y), rotate.number(new_triangle.v2.z))
print(rotate.number(new_triangle.v3.x), rotate.number(new_triangle.v3.y), rotate.number(new_triangle.v3.z))

rotate.rotate_triangle_by_direction(90, 90, new_triangle)

print('After rotation:')
print(rotate.number(new_triangle.v1.x), rotate.number(new_triangle.v1.y), rotate.number(new_triangle.v1.z))
print(rotate.number(new_triangle.v2.x), rotate.number(new_triangle.v2.y), rotate.number(new_triangle.v2.z))
print(rotate.number(new_triangle.v3.x), rotate.number(new_triangle.v3.y), rotate.number(new_triangle.v3.z))

h1, h2, h3 = rotate.get_heights(new_triangle)

print('Heights:')
print(rotate.number(h1), rotate.number(h2), rotate.number(h3))



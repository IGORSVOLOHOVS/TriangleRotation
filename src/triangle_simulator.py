import bpy
import math
from mathutils import Vector, Matrix

# Triangle coordinates
vertex_coords = [
    (0.0, 1.0, 0.0),
    (-1.0, -1.0, 0.0),
    (1.0, -1.0, 0.0)
]

# Triangle faces
faces = [(0, 1, 2)]

triangle_mesh = bpy.data.meshes.new("Triangle")
triangle_object = bpy.data.objects.new("Triangle", triangle_mesh)

triangle_location = bpy.context.scene.cursor.location
triangle_object.location = triangle_location

bpy.context.scene.collection.objects.link(triangle_object)

triangle_mesh.from_pydata(vertex_coords, [], faces)
triangle_mesh.update(calc_edges=True)

# Print triangle vertices before rotation
print("Before rotation:")
for vertex in triangle_mesh.vertices:
    print(vertex.co)

# Rotate the vertices manually
angle = math.radians(43)
rotation_matrix = Matrix.Rotation(angle, 3, 'Z')
rotated_vertices = [rotation_matrix @ Vector(coord) for coord in vertex_coords]

# Set the rotated vertices to the mesh
for i, vertex in enumerate(triangle_mesh.vertices):
    vertex.co = rotated_vertices[i]

# Print triangle vertices after rotation
print("After rotation:")
for vertex in triangle_mesh.vertices:
    print(vertex.co)

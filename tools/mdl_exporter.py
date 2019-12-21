
import sys
import xml.etree.ElementTree as et

if len(sys.argv) != 2:
    print("invalid arguments")
    sys.exit()

class vec2:
    def __init__(self, x = 0, y = 0):
        self.x = x
        self.y = y
    
class vec3:
    def __init__(self, x = 0, y = 0, z = 0):
        self.x = x
        self.y = y
        self.z = z

class vec4:
    def __init__(self, x = 0, y = 0, z = 0, w = 0):
        self.x = x
        self.y = y
        self.z = z
        self.w = w

class Effect:
    def __init__(self, id):
        self.id = id

class Image:
    def __init__(self, id, name, file):
        self.id = id
        self.name = name
        self.file = file

class Mesh:
    def __init__(self, id, name):
        self.id = id
        self.name = name
        self.sources = {}
        self.vertex_array = None

class Skin:
    def __init__(self):
        self.sources = {}
        self.joints = []
        self.bind_poses = []

class Node:
    def __init__(self):
        self.id = None
        self.scale = None
        self.rotation_x = None
        self.rotation_y = None
        self.rotation_z = None
        self.translation = None

class Collada:

    ns = "{http://www.collada.org/2005/11/COLLADASchema}"

    class Node:
        def __init__(self, element):
            self.element = element
        
        def find(self, name):
            e = self.element.find("{}{}".format(Collada.ns, name))
            return None if e == None else self.__class__(e)

        def find_all(self, name):
            return [self.__class__(e) for e in self.element.findall("{}{}".format(Collada.ns, name))]
        
        def attrib(self, name):
            return self.element.attrib[name]

        def text(self):
            return self.element.text

        def children(self):
            return [self.__class__(e) for e in self.element]
    
    def __init__(self, path):
        tree = et.parse(path)
        root = tree.getroot()

        self.library_effects = self.Node(root.find("{}library_effects".format(self.ns)))
        self.library_images = self.Node(root.find("{}library_images".format(self.ns)))
        self.library_geometries = self.Node(root.find("{}library_geometries".format(self.ns)))
        self.library_controllers = self.Node(root.find("{}library_controllers".format(self.ns)))
        self.library_visual_scenes = self.Node(root.find("{}library_visual_scenes".format(ns)))

def parse_float_array(array, stride, count):
    f = None
    if stride == 1:    f = lambda i: float(array[i])
    elif stride == 2:  f = lambda i: vec2(float(array[i + 0]), float(array[i + 1]))
    elif stride == 3:  f = lambda i: vec3(float(array[i + 0]), float(array[i + 1]), float(array[i + 2]))
    elif stride == 4:  f = lambda i: vec4(float(array[i + 0]), float(array[i + 1]), float(array[i + 2]), float(array[i + 3]))
    elif stride == 16: f = lambda i: [float(array[i + j]) for j in range(0, 16)]
    return [f(i) for i in range(0, count, stride)]

file = Collada(sys.argv[1])

library_effects = {}
library_images = {}
library_geometries = {}
library_controllers = {}

for effect in file.library_effects.children():
    id = effect.attrib("id")
    library_effects[id] = Effect(id)

for image in file.library_images.children():
    id = image.attrib("id")
    name = image.attrib("name")
    source = image.find("init_from").text()
    library_images[id] = Image(id, name, source)

for geometry in file.library_geometries.children():
    mesh_id = geometry.attrib("id")
    data = Mesh(id, name)

    mesh = geometry.find("mesh")
    for source in mesh.find_all("source"):
        float_array = source.find("float_array")

        src_id = source.attrib("id")
        count = int(float_array.attrib("count"))
        stride = int(source.find("technique_common").find("accessor").attrib("stride"))

        data.sources[src_id] = parse_float_array(float_array.text().split(), stride, count)

    vertex_array = mesh.find("vertices")
    data.vertex_array = data.sources[vertex_array.find("input").attrib("source")[1:]]
    data.sources[vertex_array.attrib("id")] = data.vertex_array

    library_geometries[mesh_id] = data

for controller in file.library_controllers.children():
    controller_id = controller.attrib("id")
    skin = controller.find("skin")
    skin_source = library_geometries[skin.attrib("source")[1:]]
    data = Skin()

    bind_shape_matrix = parse_float_array(skin.find("bind_shape_matrix").text().split(), 16, 1)
    for source in skin.find_all("source"):
        id = source.attrib("id")
        
        print(id)
        array = source.find("Name_array")
        if array != None:
            data.sources[id] = array.text().split()
        else:
            array = source.find("float_array")
            count = int(array.attrib("count"))
            stride = int(source.find("technique_common").find("accessor").attrib("stride"))
            data.sources[id] = parse_float_array(array.text().split(), stride, count)
        
    for input in skin.find("joints").children():
        semantic = input.attrib("semantic")
        source = input.attrib("source")[1:]

        if semantic == "JOINT":
            data.joints = source
        elif semantic == "INV_BIND_MATRIX":
            data.bind_poses = source

    vertex_weights = skin.find("vertex_weights")
    v_count = vertex_weights.find("vcount")
    v_array = vertex_weights.find("v")

    print("size of source vertex_array = {}".format(len(skin_source.vertex_array)))
    print("v_count = {}".format(len(v_count.text().split())))
    print("v_array size = {}".format(len(v_array.text().split())))

    library_controllers[controller_id] = data

''''
for geometry in file.library_geometries.children():
    for triangle_array in mesh.find_all("triangles"):
'''

def process_node(node, parent = None):
    scale = node.find("scale")

for visual_scene in file.library_visual_scenes:
    for node in visual_scene.find_all("node"):
        process_node(node)


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

class Collada:

    ns = "{http://www.collada.org/2005/11/COLLADASchema}"

    class Node:
        def __init__(self, element):
            self.element = element
        
        def find(self, name):
            return self.__class__(self.element.find("{}{}".format(Collada.ns, name)))

        def find_all(self, name):
            return [self.__class__(e) for e in self.findall("{}{}".format(Collada.ns, name))]
        
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

def parse_array(array, stride, count):
    f = None
    if stride == 2:   f = lambda i: vec2(float(array[i + 0]), float(array[i + 1]))
    elif stride == 3: f = lambda i: vec3(float(array[i + 0]), float(array[i + 1]), float(array[i + 2]))
    elif stride == 4: f = lambda i: vec4(float(array[i + 0]), float(array[i + 1]), float(array[i + 2]), float(array[i + 3]))
    return [f(i) for i in range(0, count, stride)]

file = Collada(sys.argv[1])

library_effects = {}
library_images = {}
library_geometries = {}

for effect in file.library_effects.children():
    id = effect.attrib("id")
    library_effects[id] = Effect(id)

for image in file.library_images.children():
    id = image.attrib("id")
    name = image.attrib("name")
    source = image.find("init_from").text()
    library_images[id] = Image(id, name, source)

for geometry in file.library_geometries.children():
    id = geometry.attrib("id")
    name = geometry.attrib("name")
    data = Mesh(id, name)

    mesh = geometry.find("mesh")
    for source in mesh.find_all("source"):
        float_array = source.find("float_array")

        id = float_array.attrib("id")
        count = int(float_array.attrib("count"))
        stride = int(source.find("technique_common").find("accessor").attrib("stride"))

        data.sources[id] = parse_array(float_array.text().split(' '), stride, count)

    vertex_array = mesh.find("vertices")
    mesh.vertex_array = mesh.sources[vertex_array.find("input").attrib("source")[1:]]
    mesh.sources[vertex_array.attrib("id")] = mesh.vertex_array

    for triangle_array in mesh.find_all("triangles"):
        

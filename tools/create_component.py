# -*-coding:Utf-8 -*

import sys


def create_component(namespace, name, parent="zn::Behaviour"):

	headerFilePath = name + ".hpp"
	implemFilePath = name + ".cpp"
	headerGuard = "HEADER_" + namespace.upper() + "_" + name.upper() + "_HPP_INCLUDED"
	indent = "\t"

	f = open(name + ".hpp", "w")
	f.write("#ifndef " + headerGuard + "\n")
	f.write("#define " + headerGuard + "\n\n")

	f.write("#include <fm/scene/behaviour/Behaviour.hpp>\n\n")

	f.write("namespace " + namespace + "\n{\n\n")

	f.write("class " + name + " : public " + parent + "\n{\n")
	f.write(indent + "ZN_OBJECT(" + namespace + "::" + name + ", " + parent + ")\n")
	f.write("};\n\n")

	f.write("} // namespace " + namespace + "\n\n")

	f.write("#endif // " + headerGuard + "\n\n")
	f.close()

	f = open(name + ".cpp", "w")
	f.close()


if __name__ == "__main__":
	create_component(sys.argv[1], sys.argv[2], sys.argv[3] if len(sys.argv)==4 else "zn::Behaviour")



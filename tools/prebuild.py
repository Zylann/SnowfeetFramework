# -*-coding:Utf-8 -*
# ------------------------------------------------------------------------------
# This file can be executed just before the compilation process.
# Its purpose is mainly to generate C++ code from certain declarations
# made in source files. It bases its input from a Code::Blocks project file.
# ------------------------------------------------------------------------------

import os
import re
import sys
from datetime import datetime
import xml.etree.ElementTree as ET
import json


# Prebuild configuration data
class Config:
	
	def __init__(self):
		self.root = ""
		self.projectFormat = ""
		self.projects = []

	def load(filePath):
		configFile = open(filePath, "r")
		configData = json.load(configFile)
		configFile.close()

		cfg = Config()

		cfg.root = configData["root"]
		cfg.projectFormat = configData["projectFormat"]

		projects = configData["projects"]

		for projectData in projects.items():
			proj = ConfigProject()
			proj.name = projectData[0]
			odbData = projectData[1]["objectRegistration"]
			proj.odbConfig = ODBConfig(odbData["namespace"], odbData["files"][0], odbData["files"][1])
			cfg.projects.append(proj)

		return cfg


# Configuration of the code generation for one project
class ConfigProject:

	def __init__(self):
		self.name = ""
		self.odbConfig = None


# Configuration of the code generation for custom RTTI
class ODBConfig:

	def __init__(self, pNamespace, pHeaderFilePath, pImplemFilePath):
		self.headerFilePath = pHeaderFilePath
		self.implemFilePath = pImplemFilePath
		self.namespace = pNamespace


# RTTI information gathered from C++ source files (ZN_OBJECT classes)
class ObjectClassInfo:

	def __init__(self, pName, pInheritedName, pHeaderFile):
		self.name = pName
		self.inheritedName = pInheritedName
		self.headerFile = pHeaderFile


def is_cpp_header(filename):
	name,ext = os.path.splitext(filename)
	ext = ext.lower()
	return ext == ".hpp" or ext == ".h" or ext == ".hxx"


def get_codeblocks_project_files(codeblocksFilePath):
	files = []
	xmlTree = ET.parse(codeblocksFilePath)
	root = xmlTree.getroot()
	project = root.find("./Project")

	for unit in project.findall("Unit"):
		filePath = unit.attrib["filename"]
		#print(filePath)
		files.append(filePath)

	return files


def get_object_classes(files):
	classes = []
	reg = re.compile("\s*ZN_OBJECT\\(\s*(\\S+)\s*,\s*(\\S+)\s*\\).*")
	for filePath in files:
		if is_cpp_header(filePath):
			#print(filename)
			for line in open(filePath, "r"):
				line = line.strip()
				if line and line[0] != '#' and line[0] != '/':
					m = reg.match(line)
					if m:
						#print(str(i) + ": " + line)
						res = m.groups(0)
						#print(res)
						klass = ObjectClassInfo(res[0], res[1], filePath)
						classes.append(klass)
	return classes


def generate_registration_files(headerFilePath, implFilePath, namespace, classes):

	headerGuardName = "GENERATED_HEADER_"+namespace.upper()+"_CLASS_REGISTRATION_INCLUDED" 
	regFunctionName = "registerClasses"
	generatedCodeInfo = "// /!\ This source file has been generated. Do not edit. /!\\\n\n"
	indent = "\t"

	# Header

	f = open(headerFilePath, "w")
	f.write(generatedCodeInfo)
	f.write("#ifndef " + headerGuardName + "\n")
	f.write("#define " + headerGuardName + "\n\n")
	f.write("#include <fm/reflect/ObjectTypeDatabase.hpp>\n\n")
	f.write("namespace " + namespace + "\n{\n\n")
	f.write("void " + regFunctionName + "(ObjectTypeDatabase & odb);\n\n")
	f.write("} // namespace " + namespace + "\n\n")
	f.write("#endif // " + headerGuardName + "\n\n")
	f.close()

	# Implementation

	f = open(implFilePath, "w")
	f.write(generatedCodeInfo)
	f.write("#include <fm/util/macros.hpp>\n\n")
	f.write("#include \"" + headerFilePath + "\"\n\n")

	for classDef in classes:
		f.write("#include \"" + classDef.headerFile.replace("\\", "/") + "\"\n")

	f.write("\nnamespace " + namespace + "\n{\n\n")
	f.write("void " + regFunctionName + "(ObjectTypeDatabase & odb)\n{\n")

	f.write(indent + "ZN_CALL_ONCE;\n\n")

	for classDef in classes:
		f.write(indent + "odb.registerType<" + classDef.name + ">();\n")

	f.write("}\n\n")
	f.write("} // namespace " + namespace + "\n\n")
	f.close()


def prebuild_codeblocks_project(projectFilePath, projectConfig):
	timeBefore = datetime.now()

	print("Parsing Code::Blocks project...")
	files = get_codeblocks_project_files(projectFilePath)

	# Parse source files and retrieve ZN_OBJECT declarations
	print("Parsing source files...")
	classes = get_object_classes(files)

	# Generate object registrations
	print("Generating code...")
	odbConf = projectConfig.odbConfig
	generate_registration_files(odbConf.headerFilePath, odbConf.implemFilePath, odbConf.namespace, classes)

	timeDelta = datetime.now() - timeBefore
	print("Done (time taken: " + str(timeDelta))


def prebuild_project(project, config):
	if config.projectFormat == "cbp":
		projectFilePath = project.name + "." + config.projectFormat
		prebuild_codeblocks_project(projectFilePath, project)
	else:
		print("[Error] the " + ext + " project file format is not supported.")


def prebuild(projectsFolderPath):
	print("Reading prebuild.json config file...")
	config = Config.load("prebuild.json")
	for project in config.projects:
		prebuild_project(project, config)


if __name__ == "__main__":	
	prebuild(".")


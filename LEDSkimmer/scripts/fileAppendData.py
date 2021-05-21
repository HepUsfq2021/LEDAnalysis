import numpy as np

f = open("parallelSkimData.yaml","a+")
f1 = open("CMS_Run2011A_DoubleElectron_AOD_12Oct2013-v1_20000_file_index.txt", "r")
f2 = open("CMS_Run2011A_DoubleElectron_AOD_12Oct2013-v1_20001_file_index.txt", "r")
f3 = open("CMS_Run2011B_DoubleElectron_AOD_12Oct2013-v1_00000_file_index.txt", "r")
f4 = open("CMS_Run2011B_DoubleElectron_AOD_12Oct2013-v1_20000_file_index.txt", "r")

i = 0
for linea in f1:
	i = i+1
	f.write("      - name: "+"a"+str(i)+"\n")
	f.write("        template: dataskim-argo\n")
	f.write("        arguments:\n")
	f.write("          parameters: [{name: files, value: "+linea+"}, {name: it, value: "+str(i)+"}]\n")

f1.close()

for linea in f2:
	i = i+1
	f.write("      - name: "+"a"+str(i)+"\n")
	f.write("        template: dataskim-argo\n")
	f.write("        arguments:\n")
	f.write("          parameters: [{name: files, value: "+linea+"}, {name: it, value: "+str(i)+"}]\n")

f2.close()

for linea in f3:
	i = i+1
	f.write("      - name: "+"a"+str(i)+"\n")
	f.write("        template: dataskim-argo\n")
	f.write("        arguments:\n")
	f.write("          parameters: [{name: files, value: "+linea+"}, {name: it, value: "+str(i)+"}]\n")

f3.close()

for linea in f4:
	i = i+1
	f.write("      - name: "+"a"+str(i)+"\n")
	f.write("        template: dataskim-argo\n")
	f.write("        arguments:\n")
	f.write("          parameters: [{name: files, value: "+linea+"}, {name: it, value: "+str(i)+"}]\n")


f.close()
f4.close()

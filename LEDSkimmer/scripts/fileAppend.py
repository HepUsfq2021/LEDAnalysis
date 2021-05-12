import numpy as np

f1 = open("CMS_Run2011B_DoubleElectron_AOD_12Oct2013-v1_00000_file_index.txt", "r")
f2 = open("parallel-Electron_example.yaml","a+")
i = 0
for linea in f1:
	i = i+1
	f2.write("		- name: "+"a"+str(i)+"\n")
	f2.write("		template: nanoaod-argo arguments: \n")
	f2.write("		parameters: [{name: files, value: "+linea+"}, {name: it, value:"+str(i)+"}]\n\n")
		
f1.close()
f2.close()

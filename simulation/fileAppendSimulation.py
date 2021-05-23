import numpy as np

f = open("parallel-simulation.yaml","a+")
i = 0
for i in range(2000):
	i = i+1
	f.write("      - name: a"+str(i)+"\n")
	f.write("        template: simulation-argo\n")
	f.write("        arguments:\n")
	f.write("          parameters: [{name: it, value: "+str(i)+"}]\n")
		
f.close()

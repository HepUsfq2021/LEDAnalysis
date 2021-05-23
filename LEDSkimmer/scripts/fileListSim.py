import glob

fileList = []
for file in glob.glob("simData/*.root"):
	fileList.append(file.rstrip())

f = open("fileNamesSim.txt",'a+')

i = 0
for i in range(len(fileList)):
	f.write("file:data/"+fileList[i].rstrip()+"\n")
f.close()





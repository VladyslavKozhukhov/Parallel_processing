import numpy as np
import matplotlib.mlab as mlab
import matplotlib.pyplot as plt
total =[]
for i in range(0,50):
        lst= []
        f  = open("./run"+str(i)+"/test.out","r")

        for line in f:
                lst = line.split(" ")
        f.close()
#       lst = [float(i[:-3]) for i in lst]
#       total= total+lst
        for v in lst[:-1]:
                total.append(float(v))
        #print([i[:-5] for i in lst])
        #total = total[:-1]
#print(total)
print(len(total))


#total[0]=float((total[0].split("["))[1])
#total[-1]=float((total[-1].split("]"))[0])
num_bins = 20
total=[float(i) for i in total]
n, bins, patches = plt.hist(total, num_bins, facecolor='blue', alpha=0.5)
plt.show()


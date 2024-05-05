import subprocess 
import time
import pandas as pd

with open('runspeedchecks.out', 'r') as results:
   #lines = f.read().split('\n')
    outputs = results.read().split('Command')[1:]

data = []
for i in range(len(outputs)):
    o = outputs[i]
    lines = o.split('\n')

    #print(lines[0])
    
    keypair_loc = lines.index("kyber_keypair: ")
    keypair_results = lines[keypair_loc:keypair_loc+3]
    #print("keypair_results: \n", keypair_results)

    encaps_loc = lines.index("kyber_encaps: ")
    encaps_results = lines[encaps_loc:encaps_loc+3]
    #print("encaps_results: \n", encaps_results)
   
    decaps_loc = lines.index("kyber_decaps: ")
    decaps_results = lines[decaps_loc:decaps_loc+3]
    #print("decaps_results: \n", decaps_results)

    #print("line: ", [lines[0].split("/")[2], keypair_results[1].split(" ")[1], keypair_results[2].split(" ")[1], 
    #        encaps_results[1].split(" ")[1], encaps_results[2].split(" ")[1], 
    #        decaps_results[1].split(" ")[1], decaps_results[2].split(" ")[1] ])

    data.append( [lines[0].split("/")[1]=="avx2", lines[0].split("/")[2], keypair_results[1].split(" ")[1], keypair_results[2].split(" ")[1], 
            encaps_results[1].split(" ")[1], encaps_results[2].split(" ")[1], 
            decaps_results[1].split(" ")[1], decaps_results[2].split(" ")[1] ] )
    #data.append(keypair_results[1].split(" ")[1]+","+keypair_results[2].split(" ")[1]+","+
    #        encaps_results[1].split(" ")[1]+","+encaps_results[2].split(" ")[1]+","+
    #        decaps_results[1].split(" ")[1]+","+decaps_results[2].split(" ")[1]) 

df = pd.DataFrame(data, columns=["avx2", "speedtest", "keypair_median", "keypair_average", "encaps_median", "encaps_average", "decaps_meian", "decaps_average"])
print(df.shape)
print(df.head())
print(df.columns)

df.to_csv("speed.csv", index=False)

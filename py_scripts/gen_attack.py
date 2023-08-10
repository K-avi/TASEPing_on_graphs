import numpy as np 
import random as r 

def gen_random_attack(total_nb_lines, nb_attacks, mean_attack_size, variation=0):
    """
    int,int,int -> random_attack arrays
    """
    l = []
    
   
    
    for i in range(0,nb_attacks) : 
         var = r.randint(0,variation)
         narr = np.random.randint(0,total_nb_lines, mean_attack_size-var, dtype="uint32")
         l.append(narr)
    return l
    

def write_attack(raa,fname):
    
    with open(fname,"w") as file :
        file.write(f"{len(raa)},{sum([len(i) for i in raa])}\n")
        for i in raa: 
            file.write(f'{len(i)},{";".join( [str(j) for j in i])}\n')
        
   
    
import numpy as np
import matplotlib.pyplot as plt
import argparse as args
import os


def loaddir(dirpath, sim_name): 
    """
    """
    res = []
    dir = os.listdir(dirpath)
    
    for i in dir : 
        if(sim_name in i ):
            res.append([i,np.loadtxt(dirpath+"/"+i)])
    return res 

def plotdir(dirpath, sim_name): 
    """ 
    """
    
    res = loaddir(dirpath, sim_name)
    color = iter(['r', 'g', 'b', 'y','pink', 'black', 'brown', 'grey', 'purple', 'orange'])

    dtplot = [] 
    with open (dirpath+"/index.csv") as index : 
        
        dict_index = dict()
        for i in index.readlines() : 
            s = i.split(",")
            
            dict_index[s[0].split("/")[-1]] = [int(s[2])/int(s[1]), int(s[1])]
        
        
        for i in res : 
            
            dtplot.append( np.array( [dict_index[i[0]][0] , i[1][-1]/dict_index[i[0]][1]] ) )
    
   
    plt.scatter(*zip(*dtplot))  
    plt.xlabel("number of entities / number of graph nodes")
    plt.ylabel("number of entities moved during last iteration / number of graph nodes")
    plt.title("Scatterplot of a TASEP model on a graph")
    plt.show()
    
    return 0 


if __name__ == '__main__': 
    
    
    print("parsing arguments")
    parser = args.ArgumentParser(
        prog="plot",
        description="plots an already ran simulation"
    )
    
    parser.add_argument(
        "res_dir", metavar="res_dir", type=str,
        nargs=1, help="string to specify the result directory to load",
    )
    
    parser.add_argument(
        "sim_name", metavar="sim_name", type=str,
        nargs=1, help="string to specify the name of the simul",
    )


    arg = parser.parse_args()

    #res = loaddir(arg.res_dir[0], arg.sim_name[0])
    plotdir(arg.res_dir[0], arg.sim_name[0])

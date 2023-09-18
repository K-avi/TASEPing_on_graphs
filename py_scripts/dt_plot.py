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
            res.append(np.loadtxt(i))
    return res 

def plotdir(dirpath, sim_name): 
    """ 
    """
    
    res = loaddir(dirpath, sim_name)
    color = iter(['r', 'g', 'b', 'y','pink', 'black', 'brown', 'grey', 'purple', 'orange'])

    dtplot = [] 
    
    for i in res : 
        dtplot.append(i[-1])
    
    plt.scatter(dtplot)
    plt.show()
    
    return 0 


if name == '__main__': 
    
    
    res = loaddir("path")
    plotdir(res)
import numpy as np
import matplotlib.pyplot as plt
import argparse as args
import os


def loaddir(dirpath, sim_name): 
    """
    string , string -> list(string, np1d array)
    loads a formatted directory (producer by the launch.sh script)
    and returns a 
    """
    res = []
    dir = os.listdir(dirpath)
    
    for i in dir : 
        if(sim_name in i ):
            res.append([i,np.loadtxt(dirpath+"/"+i)])
    return res 


def plotdir(dirpath, sim_name, col="blue",lab=None): 
    """ 
    string, sring -> None
    
    loads the simulation of name 'sim_name' stored at the 
    'dir_path' path 
    (use the launch.sh script to create the simulations) and 
    plots it on a plt.scatterplot
    
    the X coordinates of the scatterplot is 
    [nb total entities]/[nb nodes of the_graphs] 
    
    the Y coordiantes of the scatterplot is
    [nb entities moved in the final iteration]/[nb nodes of the graph]
    """
  
    res = loaddir(dirpath, sim_name)

    dtplot = [] 
    with open (dirpath+"/index.csv") as index : 
        
        dict_index = dict()
        for i in index.readlines() : 
            s = i.split(",")
            
            dict_index[s[0].split("/")[-1]] = [int(s[2])/int(s[1]), int(s[1])]
        
        
        for i in res : 
            
            dtplot.append( np.array( [dict_index[i[0]][0] , i[1][-1]/dict_index[i[0]][1]] ) )
    
   
    plt.scatter(*zip(*dtplot),color=col,label=lab)  
    
    
    
    return 

def plot_mult_dirs(dirplot_list):
    """
    list[[directory path, simulation name]] -> None 
    create a scatterplot with the data 
    in a list of directories. 
    See documentation of plotdir for 
    more informations
    """
    color = iter(['r', 'g', 'b', 'y','pink', 'black', 'brown', 'grey', 'purple', 'orange'])

    for i in dirplot_list :
       
        plotdir(i[0], i[1],next(color))
    
    plt.xlabel("number of entities / number of graph nodes")
    plt.ylabel("number of entities moved during last iteration / number of graph nodes")
    plt.title("Scatterplot of a TASEP model on a graph")
    plt.legend()
    plt.show()

def plot_sims(sim_list, graph_size):
    """
    
    list[strings] -> none
    
    placeholder function 
    to like 
    -> load a couple of sims 
    -> plot the evolution of their %wk moved at each iteration 
    """
    
    ite = np.linspace(0,999,1000)
    color = iter(['r', 'g', 'b', 'y','pink', 'black', 'brown', 'grey', 'purple', 'orange'])

    for i in sim_list : 
        c = next(color)
        plt.plot(ite,np.loadtxt(i)/graph_size,label=i,color=c)
        
    plt.xlabel("iteration")
    plt.ylabel("number of entities moved during current iteration / number of graph nodes")
    plt.title("Plot of multiple result of TASEP models on a graph")
    plt.legend()
    plt.show()
 

def main():
    """
    parses a directory path and simulation name 
    to plot a simulation
    """
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

    plot_mult_dirs([ [arg.res_dir[0], arg.sim_name[0]] ])


if __name__ == '__main__': 
    
   main()
   """
   testing area : 
   
   test_plot_sims = []
   for i in range(1,10): 
       test_plot_sims.append("res/res0."+str(i))
     
   test_plot_sims.append("res/res1.0")
   plot_sims(test_plot_sims,280951)
   """
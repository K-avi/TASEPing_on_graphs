import numpy as np 
import networkx as nx 
import scipy.sparse.csgraph as cg


def wog_to_nx(path): 
    """
    str -> networkx.graph 
    loads the walking on graphs rep
    of a graph from the csv file at path 
    into a networkx graph
    """
    g = nx.Graph()
    
    with open(path,"r") as f : 
        lines = f.readlines()
        lines = lines[1::] #skips the gsize thing 
        
        for i in lines : 
            if(i.strip()):
               
                a = int(i.split(",")[0])
                t = i.split(',')[2].split(';')  
      
                edges_to = [ int( b.split(':')[0]) for b in t]                
                g.add_edges_from([ (a,b) for b in edges_to if (a,b) not in g])           
    return g

def wog_to_sparse(path):
    """
    str -> scipy.sparce.csgraph
    loads the walking on graphs rep
    of a graph from the csv file at path 
    into a sparce csgraph
    """
    with open(path,"r") as f : 
        lines = f.readlines()
        n = np.zeros( (int(lines[0].split(",")[0]),int(lines[0].split(",")[0])), dtype="uint8")
        lines = lines[1::] #skips the gsize thing 
        
        for i in lines : 
            if(i.strip()):
               
                a = int(i.split(",")[0])
                t = i.split(',')[2].split(';')  
                edges_to = [ int( b.split(':')[0]) for b in t]                
                               
                n[a][edges_to] = 1       
   
    return cg.csgraph_from_dense(n)


def wog_to_dict(path): 
    """
    str -> dict
    loads the walking on graphs rep
    of a graph from the csv file at path 
    into a python dict 
    """
    g = dict()
    
    with open(path,"r") as f : 
        lines = f.readlines()
        lines = lines[1::] #skips the gsize thing 
        
        for i in lines : 
            if(i.strip()):
               
                a = int(i.split(",")[0])
                t = i.split(',')[2].split(';')  
      
                edges_to = [ int( b.split(':')[0]) for b in t]                
                 
                g[a] = edges_to
    return g


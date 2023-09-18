import networkx as nx 


def discretise_tog(g,step): 
    """
    nx.Graph w the lil length attribute on the lines -> beeg boy nx.DiGraph
    the way graphs are discretised for the TOG program 
    """
    
    gret = nx.DiGraph()
    nb_nodes = g.number_of_nodes()
    nb_edges = g.number_of_edges()
    
    
    for a,b,length in g.edges(data="length") : 
            
            ns = int(float(length)/step)
            
            new_ed = [ a, *list(range( nb_nodes, nb_nodes + ns )), b]
            nx.add_path(gret, new_ed)
            nb_nodes += ns
            
            new_ed = [b , *list(range(nb_nodes , nb_nodes+ns)),a]
            nx.add_path(gret, new_ed)
            nb_nodes += ns
            
            nb_edges += 2*ns
    return gret

def discretise_wog(g,step): 
    """
    nx.Graph w the lil length attribute on the lines -> beeg boy nx.graph
    the way graphs are discretised for the WOG program 
    """
    
    gret = nx.Graph()
    nb_nodes = g.number_of_nodes()
    nb_edges = g.number_of_edges()
    
    for a,b,length in g.edges(data="length") : 
            
            ns = int(float(length)/step)
            
            new_ed = [ a, *list(range( nb_nodes, nb_nodes + ns )), b]
            nx.add_path(gret, new_ed)
            nb_nodes += ns
            
        
            nb_edges += ns
    return gret
    


# I'll see about the huuuuh matching lines thingy later 
# maybe I can do it in a clever way, even if I can 
# and I prolly can , I won't atm 

def makeCSVTOG(Graph, path):
    """
    nxDiGraph , path -> custom csv graph file 
    writes the custom csv corresponding to the graph passed
    as argument in the file at path
    """ 
    with open (path, "w") as file: 
     
        file.write(f"{Graph.number_of_nodes()},{Graph.number_of_edges()}\n") #2 times nb of edges cuz need (a,b) and (b,a)
        for i in Graph:
         
            file.write(f'{i},{Graph.out_degree(i)},{":0;".join( str(i) for i in Graph.neighbors(i))+":0"}\n')
    file.close

def makeCSVWOG(Graph, path):
    """
    nxDiGraph , path -> custom csv graph file 
    writes the custom csv corresponding to the graph passed
    as argument in the file at path
    """ 
    with open (path, "w") as file: 
     
        file.write(f"{Graph.number_of_nodes()},{Graph.number_of_edges()*2}\n") #2 times nb of edges cuz need (a,b) and (b,a)
        for i in Graph:
         
            file.write(f'{i},{Graph.degree(i)},{":0;".join( str(i) for i in Graph.neighbors(i))+":0"}\n')
    file.close


#test area 
import graph_convert as gc 
import osmnx as ox 
g = ox.graph_from_place("paris", network_type="drive")
g = nx.Graph(g)
g.remove_edges_from(nx.selfloop_edges(g) )

g = nx.k_core(g,2)
s = max(nx.connected_components(g))

g.remove_nodes_from([ i for i in g.nodes() if not i in s ])
g = nx.convert_node_labels_to_integers(g,0)
dg_tog = discretise_tog(g,10)
#dg = nx.convert_node_labels_to_integers(dg, 0)
dg_wog = discretise_wog(g,10)
#makeCSVTOG(dg_tog,"test.csv")

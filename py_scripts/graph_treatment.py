import networkx as nx 




def get_pathes(g): 
    
    """
    nx.DiGrapg -> list[ [path list] ]
    like a DFS 
    where ir "builds" the list 
    by adding the path list when I see one and shit 

    reference : networkx dfs_edges()
    https://networkx.org/documentation/stable/_modules/networkx/algorithms/traversal/depth_first_search.html
    """
    pathes = []
    visited_nodes = set()
    visited_paths = set()
    visited_edges = set()
    stack = []
    cur = None
    cpt =  0
   
    for parent in g.nodes():
       
       
        if parent not in visited_nodes: 
            
            visited_nodes.add(parent)
            stack.append(  (parent, iter(g[parent])))
            
            while stack :
                
                parent, children = stack[-1]
                
                
                try:
                    child = next(children)
                    
                    if cur is not None and  g.degree(parent) == 2 and g.degree(child) == 2 : #case when in a "discretised path"
                        sup,inf = parent,child 
                        if sup < inf : 
                            sup,inf=inf,sup 
                        if (sup,inf) not in visited_edges :
                    #     print(cpt ,sup,inf)
                            visited_edges.add((sup,inf))
                            cur.append(child)
                             
                        sup,inf = parent,child 
                    # print("check0", sup, inf)
                        if sup < inf : 
                            sup,inf=inf,sup 
                        if (sup,inf) not in visited_edges :
                        #  print(cpt ,sup,inf, visited_edges)
                            visited_edges.add((sup,inf))
                            cur.append(child)
                            
                            sup,inf = cur[0], cur[-1] #checks the "direction"
                    #        print("c",cur, sup,inf)
                            if inf > sup :
                                sup,inf = inf,sup
                            if (sup,inf) not in visited_paths: 
                                visited_paths.add( (sup,inf) ) #adds the path if it's not already seen
                                pathes.append(cur)
                            
                            cur = None
                            
                    elif cur is not None and  g.degree(parent) > 2 and g.degree(child) == 2 : #entering a disc path
                        
                        sup,inf = parent,child 
                        if sup < inf : 
                            sup,inf=inf,sup 
                        if (sup,inf) not in visited_edges :
                        #  print(f"yo {sup} {inf} cpt {cpt}")
                            visited_edges.add((sup,inf))
                            cur.append(child)
                        
                        
                    elif cur is not None and  g.degree(parent) > 2 and g.degree(child) > 2 : #no discr path but a direct one, why not ! 
                        
                        sup,inf = parent,child 
                        if sup < inf : 
                            sup,inf=inf,sup 
                            
                        if (sup,inf) not in visited_edges :
                            
                            visited_edges.add((sup,inf))
                            cur.append(child)
                            
                            sup,inf = cur[0], cur[-1] #checks the "direction"
                            
                            if inf > sup :
                                sup,inf = inf,sup
                            if (sup,inf) not in visited_paths: 
                                visited_paths.add( (sup,inf) ) #adds the path if it's not already seen
                                pathes.append(cur)
                            
                            cur = None
                       
                    elif cur is None and  g.degree(parent) == 2 and g.degree(child) == 2 : #looking 
                       
                       # print( len(pathes), cur, parent, child, len(visited_paths), len(visited_edges)) 
                        print( "wtf1")
                        return pathes,cur,parent,child, visited_paths, visited_edges
                        
                        #sup,inf = parent,child 
                        #if sup < inf : 
                        #    sup,inf=inf,sup 
                            
                       # if (sup,inf) not in visited_edges :
                            
                      #      visited_edges.add((sup,inf))
                     #   
                    #
                    elif cur is None and  g.degree(parent) == 2 and g.degree(child) > 2 : #wtf 
                       
                        print( len(pathes), cur, parent, child, len(visited_paths), len(visited_edges)) 
                        raise Exception("wtf 2")
                      #  sup,inf = parent,child 
                       #  if sup < inf : 
                        #    sup,inf=inf,sup 
                            
                        #if (sup,inf) not in visited_edges :
                            
                         #   visited_edges.add((sup,inf))
                        
                        
                    elif cur is None and  g.degree(parent) > 2 and g.degree(child) == 2 : #normal thing 
                    # print(f"{cpt} {parent} {child}\n")
                        cur = []
                        cur.append(parent)
                        cur.append(child)
                        
                        sup,inf = cur[0], cur[-1] #checks the "direction"
                        
                        if inf > sup :
                            sup,inf = inf,sup
                        if (sup,inf) not in visited_edges: 
                            visited_edges.add( (sup,inf) ) #adds the path if it's not already seen
                        
                            
                    elif cur is None and  g.degree(parent) > 2 and g.degree(child) > 2 : #no discr path
                        
                        pathes.append([parent,child])
                        
                        
                        sup,inf = parent,child 
                        
                        if inf > sup :
                            sup,inf = inf,sup
                        if (sup,inf) not in visited_paths: 
                            visited_paths.add( (sup,inf) ) #adds the path if it's not already seen
                            visited_edges.add((sup,inf))
                
                    
                    if child not in visited_nodes :
                            
                        visited_nodes.add(child)      
                        stack.append((child, iter(g[child])))
                   
                except StopIteration:
                    stack.pop()  
            cpt+=1 
    print(cpt)  
    return pathes
#completely wrong ; what a waste of time and kilobytes 
#"naive" algorithm prolly better way to do it 
#implementation of the alg might also be awful 




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

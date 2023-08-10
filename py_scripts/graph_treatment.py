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
#not tested ; prolly slow af 
#"naive" algorithm prolly better way to do it 
#implementation of the alg might also be awful 

def convert_graph(g): 
    """
    nx.Graph -> treated nx.DiGraph 
    converts and turns g into a DiGraph, 
    changes the topology of the graph to match 
    the graph I want to use in the C 
    TOG program
    """
    
    g = nx.DiGraph(g)#what a conversion ! 
    
#not done

import graph_convert as gc 

g = gc.wog_to_nx("paris_final_gendown.csv")


l = get_pathes(g)

pathes,cur,parent,child, visited_path, visited_edges = l   
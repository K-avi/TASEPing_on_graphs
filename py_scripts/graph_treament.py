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
    stack = []
    cur = None
    
    for parent in g.nodes():
        
        visited.add(parent)
        stack.append(  (parent, iter(g[parent])))
        
        while stack :
             
            parent, children = stack[-1]
            stack.pop()
            
            if cur is not None and  l.degree(parent) == 2 and l.degree(child) == 2 : #case when in a "discretised path"
                cur.append(child)
            elif cur is not None and  l.degree(parent) == 2 and l.degree(child) > 2 : #case at the end of a "discretised path"
                cur.append(child)
                
                sup,inf = cur[0], cur[-1] #checks the "direction"
                
                if inf > sup :
                    sup,inf = inf,sup
                if (sup,inf) not in visited_paths: 
                    visited_paths.add(sup,inf) #adds the path if it's not already seen
                    pathes.add(cur)
                
                cur = None
            elif cur is not None and  l.degree(parent) > 2 and l.degree(child) == 2 : #entering a disc path
                cur.append(child)
            elif cur is not None and  l.degree(parent) > 2 and l.degree(child) > 2 : #no discr path but a direct one, why not ! 
                cur.append(child)
                
                sup,inf = cur[0], cur[-1] #checks the "direction"
                
                if inf > sup :
                    sup,inf = inf,sup
                if (sup,inf) not in visited_paths: 
                    visited_paths.add(sup,inf) #adds the path if it's not already seen
                    pathes.add(cur)
                
                cur = None
                
            elif cur is None and  l.degree(parent) == 2 and l.degree(child) == 2 : #looking 
                raise Exception("wtf")
            elif cur is None and  l.degree(parent) == 2 and l.degree(child) > 2 : #wtf 
                raise Exception("wtf")
            elif cur is None and  l.degree(parent) > 2 and l.degree(child) == 2 : #normal thing 
                cur.append(child)
            elif cur is None and  l.degree(parent) > 2 and l.degree(child) > 2 : #no discr path
                cur.append(child)    
                
            try:
                child = next(children)
                if child not in visited:
                    
                    visited.add(child)
                    stack.append((child, iter(G[child])))
            except StopIteration:
                stack.pop()   
                 
    return l
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
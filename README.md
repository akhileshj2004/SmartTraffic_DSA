# SmartTraffic_DSA
## Data Structures and Algorithms  In C Programming Language 


## 1. Introduction
  Smart Traffic Management, this project exemplifies a sophisticated integration of cuttingedge algorithms. The utilization of Dijkstra's algorithm for meticulous route planning and 
  Breadth-First Search (BFS) to establish city connectivity underscores our commitment to 
  optimizing urban mobility. Beyond efficient navigation, the inclusion of parking slot 
  reservation functionality further enhances the comprehensive nature of our solution. This 
  project not only addresses the complexities of traffic management but sets a benchmark 
  for the seamless integration of advanced algorithms in urban planning and mobility 
  enhancement.


## 2. Problem Statement:
### 2.1 Domain: 
  The urban landscape grapples with persistent inefficiencies in traffic management, marked 
  by congestion and suboptimal routes. The pressing need for an adaptive solution is evident 
  as conventional systems struggle to meet the demands of modern cities. Our project 
  addresses this challenge, utilizing advanced algorithms to create a responsive traffic 
  management system, aiming to alleviate congestion and enhance urban mobility.

### 3.2 Module Description:
  The `bfsReachableCities` function employs a Breadth-First Search (BFS) algorithm to 
  determine and display cities reachable from a given source city within a specified road 
  limit. The function takes two parameters: `src` representing the source city's index and 
  `limit` indicating the maximum road distance permissible. The function initializes an array 
  `visited` to keep track of visited cities and a queue for BFS traversal. Starting from the 
  source city, the algorithm iteratively explores neighbouring cities within the road limit and 
  enqueues them for further exploration. The process continues until either the road limit is 
  exhausted or all reachable cities are visited. The function then prints the names of the 
  cities within the specified road limit from the source city.

  

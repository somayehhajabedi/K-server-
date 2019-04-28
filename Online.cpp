#include <fstream>
#include <algorithm> 
#include <iostream>
#include <sstream>
#include <string>
#include <list>
#include <queue>
#include <vector>
#include <stdlib.h>
using namespace std;


int reqLength;  //request length
int comCount = 0; //combination count 
int OPTcosts[230300]; 
int nodes = 0;
int reqs[500];

//static int count = 0;


struct serverCFGs {
  int cfg[4];
  int cost; 
} srvcfg[230300];


vector<int> points;//all nodes
vector< vector<int> > combinations; //combination of configuration
vector<int> combination;



void adjacencyMatrix(int M,int N,string fileName);
int** computeDistancesFromFile(string fileName);
//void computeOPT(string D,string  r,string C0);
//void computeWFA(int** D,string  r,string  C0);
//void computeGreedy(int** D,string r,string C0);



void addEdge(vector <int> edges[], int u, int v)//Add edges to graph 
{ 
   edges[u].push_back(v); 
   edges[v].push_back(u); 
} 

//**********************************Produce combination of configuration****************************************************//
int pretty_func(const vector<int>& v) 
{
      static int count = 0;
      //cout << "combination no :" 
      (++count);
      //for (int i = 0; i < v.size(); ++i) { cout << v[i] << " "; }
      //cout << "] " << endl;
      combinations.push_back(v);
      return (count);	
}

  void combination_calc(int offset, int k) 
{
	if (k == 0) 
	{
        	comCount = pretty_func(combination);
        	return;
      	} 
      	for (int i = offset; i <= points.size() - k; ++i) 
	{
        	combination.push_back(points[i]);
        	combination_calc(i+1, k-1);
        	combination.pop_back();
        }
}


void calCombinationCFG() {
    int n = 6, k = 4;
          combination_calc(0, k);
  
//    for(int i =0; i<=combinations.size(); i++){
//          for(int j =0; j<combinations[i].size();j++)
//                  cout<<combinations[i][j]<<" ";
//          cout<<endl;
          //cout<<"test size"<<combinations[i].size()<<endl;
//          }
          //cout<<"combinations size"<<combinations.size()<<endl;

}


//**************************************************************************************************************//
int MinimumDisTwoConfig(int** D,int arr1[],int arr2[]){  //Calculate distance between two configurations of servers
         int totDis = 0;
         for (int i=0;i<4; i++){
                 if (arr1[i] == -1)
                         continue;
                 if (arr2[i] == -1)
                         continue;
                 int xpoint;
                 int ypoint;
                 xpoint = arr1[i];
                 ypoint = arr2[i];
                 totDis= D[xpoint][ypoint]+ totDis;
 
         }
         return totDis;
 
 }


//***************************************************************************************************************//
int disStrings(int** Distance,int arr1[4], int arr2[4]){  //Calculate distance between two points

int k=0;
int diffArr1[4] = {-1,-1,-1,-1};
for (int i=0;i<4;i++){
	int diff = 0;
	for (int j=0;j<4;j++){
	if (arr1[i] != arr2[j]){
		diff++;
	}
	}
	if (diff == 4){
		diffArr1[k] = arr1[i];
		k++;
		}
}
	//for (int i=0;i< 4;i++)
	//cout<< "DiffArr1:   "   <<diffArr1[i] << " \n";
	//cout<< "\n";

//for diff2
 k=0;
 int diffArr2[4] = {-1,-1,-1,-1};
  for (int i=0;i<4;i++){
          int diff = 0;
          for (int j=0;j<4;j++){
          if (arr2[i] != arr1[j]){
                  diff++;
          }
          }
          if (diff == 4){
                  diffArr2[k] = arr2[i];
                  k++;
                  }
  }
          //for (int i=0;i< 4;i++)
          //cout<< "DiffArr2:   "   <<diffArr2[i] << " \n";
int dis;
dis = MinimumDisTwoConfig(Distance,diffArr1,diffArr2);
return dis;

}

//*****************************************************************************************//
int  computeWFA(int** D,int r[],int C0[])
{ // Compute Work function algorithm cost

int finalCost = 0;
int s=0;
int index0,index1,index2,index3,dis0,dis1,dis2,dis3,dis;
int WFACFGs[500]; 
int initialConfig = 0; 
//int len =  sizeof(r)/sizeof(r[0]);
int rLength = reqLength;
int WFACost = 0;
for (int i=0;i<rLength;i++){

	WFACFGs[0] = 0;
	//cout << "i: "<< "request:  " << i <<  "  " << r[i]<<  "  \n";
	for (int j=0;j<comCount;j++){
	//for (int j=0;j<5;j++){
		

		initialConfig = 0;

		if ((srvcfg[j].cfg[0] == C0[0]) 
			&&  (srvcfg[j].cfg[1] == C0[1]) 
				&& (srvcfg[j].cfg[2] == C0[2]) 
					&& (srvcfg[j].cfg[3] == C0[3]))
		{
			//cout << "Initial state" << "\n";
			WFACFGs[s] = j;
			s++;
			initialConfig = 1;	
			
                } 


/*
		cout << "config[j]\n";
		for (int c=0;c<4;c++)
		cout << srvcfg[j].cfg[c] << "  ";
		cout << "\n";

*/
		int found =0;
 		for(int k =0;k<4;k++){
         	if (r[i] ==  srvcfg[j].cfg[k]){
			found = 1;
			//cout << "foundddd" << "\n";
			break;
         	}//if
				
		}//k: array of each config

		
		if (found == 0){//cal cost for processing r[i]
			//cout<< "found == 0" << "\n";
			int origcfg0[4];
			int origcfg1[4];
			int origcfg2[4];
			int origcfg3[4];

			for(int l =0;l<4;l++){

			origcfg0[l] = srvcfg[j].cfg[l];
			origcfg1[l] = srvcfg[j].cfg[l];
			origcfg2[l] = srvcfg[j].cfg[l];
			origcfg3[l] = srvcfg[j].cfg[l];
			
			}

			origcfg0[0] = r[i];
			origcfg1[1] = r[i];
			origcfg2[2] = r[i];
			origcfg3[3] = r[i];
			/*
			for(int k =0;k<4;k++)
			cout << "cfg0: " << origcfg0[k] << " ";
			cout << "\n";
			
			
			for(int k =0;k<4;k++)
			cout <<  "cfg1: "<<  origcfg1[k] << " ";
			cout << "\n";
			
			for(int k =0;k<4;k++)
			cout <<  "cfg2: " << origcfg2[k] << " ";
			cout << "\n";
			

			for(int k =0;k<4;k++)
			cout <<  "cfg3: " << origcfg3[k] << " ";
			cout << "\n";
			*/


			int cost0,cost1,cost2,cost3,cost;

			//config0

			//cost0= disStrings(D,C0,origcfg0); //+ D[r[i],origcfg0[0]];
			int index;
			int m = sizeof(origcfg0)/sizeof(origcfg0[0]); 
  			sort(origcfg0, origcfg0+m); 
			for (int z=0;z<comCount;z++){
				if (origcfg0[0] == srvcfg[z].cfg[0] &&
					origcfg0[1] == srvcfg[z].cfg[1] &&
						origcfg0[2] == srvcfg[z].cfg[2] &&
							origcfg0[3] == srvcfg[z].cfg[3]) {
					
				index0 = z;
				//cout << "index0 : " << index0 <<"\n";
				}
				
			}
			//cout << "XXX: " << srvcfg[index0].cost << "\n";
			//cout << "DDDD: " << D[r[i]][srvcfg[j].cfg[0]] << "\n";
			cost0= srvcfg[index0].cost + D[r[i]][srvcfg[j].cfg[0]];
			dis0 = D[r[i]][srvcfg[j].cfg[0]];
			//config1
			 //int index;
                         m = sizeof(origcfg1)/sizeof(origcfg1[0]);
                         sort(origcfg1, origcfg1+m);
                         for (int z=0;z<comCount;z++){
                                 if (origcfg1[0] == srvcfg[z].cfg[0] &&
                                         origcfg1[1] == srvcfg[z].cfg[1] &&
                                                 origcfg1[2] == srvcfg[z].cfg[2] &&
                                                         origcfg1[3] == srvcfg[z].cfg[3]) {
                                         
                                 index1 = z;
                                 //cout << "index1 : " << index1 <<"\n";
                                 }
                          
                         }
			dis1 = D[r[i]][srvcfg[j].cfg[1]];
			cost1= srvcfg[index1].cost + D[r[i]][srvcfg[j].cfg[1]];

			//config2
			  //int index;
			  m = sizeof(origcfg2)/sizeof(origcfg2[0]);
                          sort(origcfg2, origcfg2+m);
                          for (int z=0;z<comCount;z++){
                                  if (origcfg2[0] == srvcfg[z].cfg[0] &&
                                          origcfg2[1] == srvcfg[z].cfg[1] &&
                                                  origcfg2[2] == srvcfg[z].cfg[2] &&
                                                          origcfg2[3] == srvcfg[z].cfg[3]) {
                                          
                                  index2 = z;
                                  //cout << "index2 : " << index2 <<"\n";
                                  }
                           
                          }
			dis2 = D[r[i]][srvcfg[j].cfg[2]]; 
                        cost2= srvcfg[index2].cost + D[r[i]][srvcfg[j].cfg[2]];
			//config3
			   m = sizeof(origcfg3)/sizeof(origcfg3[0]);
                           sort(origcfg3, origcfg3+m);
                           for (int z=0;z<comCount;z++){
                                   if (origcfg3[0] == srvcfg[z].cfg[0] &&
                                           origcfg3[1] == srvcfg[z].cfg[1] &&
                                                   origcfg3[2] == srvcfg[z].cfg[2] &&
                                                           origcfg3[3] == srvcfg[z].cfg[3]) {
                                           
                                   index3 = z;
                                   //cout << "index3 : " << index3 <<"\n";
                                   }
                            
                           }
			  dis3 =  D[r[i]][srvcfg[j].cfg[3]];
                          cost3= srvcfg[index3].cost + D[r[i]][srvcfg[j].cfg[3]];
							
			//cost1= disStrings(D,C0,origcfg1); //+ D[r[i],origcfg1[1]];
		
/*	
			cout << "cost0: "<< cost0 << "\n";
			cout << "cost1: "<< cost1 << "\n"; 
			cout << "cost2: "<< cost2 << "\n";
			cout << "cost3: "<< cost3 << "\n";
*/				
			cost = min (cost0,cost1);


			//cout << "min cost1: "<< cost << "\n";
			cost = min (cost,cost2);
			//cout << "min cost2: "<< cost << "\n";	
			cost = min (cost,cost3);


    			if (cost == cost0) 
				index = index0;
                       	else if (cost == cost1)
				index = index1;
			else if (cost == cost2)
				index = index2;
			else if (cost == cost3)
				index = index3;  

			//cout << "min cost: "<< cost << "\n";
				
			srvcfg[j].cost= cost;
			//ssscout<< cost <<"  ";
			if (initialConfig == 1)//Update initial configuration  
			{
				//cout << "This is initial configuration" << "\n";
				WFACFGs[s] = index ;
				s++;	
				for (int i=0;i<4;i++)
				{
					C0[i]= srvcfg[index].cfg[i];		
				}

			if (cost == cost0)
                                 dis  = dis0;
                         else if (cost == cost1)
                                 dis = dis1;
                         else if (cost == cost2)
                                 dis = dis2;
                         else if (cost == cost3)
                                 dis = dis3;
 
				//cout << "finalCost1 : "<< finalCost << "\n";
				//cout << "disCost : "<< dis << "\n";
				finalCost = finalCost + dis; 
				//cout << "finalCost2 : "<< finalCost << "\n";
					
			}
	

                 }
                else if (found == 1){ //cost is the same as previous step
                         //cout << "Found == 1" << " \n";
                        srvcfg[j].cost = srvcfg[j].cost;
			if (initialConfig == 1)//Update initial configuration
			{
				for (int q=0;q<4;q++)
				{
					C0[q]= srvcfg[j].cfg[q];
				}
			}			
 
                 }
         }//j :Number of  configs
 
         }//i: Number of requests
 
	return finalCost;		

}
//********************************************************************************************************//
int computeOPT(int** D,int r[],int C0[]){


//int len =  sizeof(r)/sizeof(r[0]);
int len = reqLength;
for (int i=0;i<len;i++){
	//cout << "i: "<< "request:  " << i <<  "  " << r[i]<<  "  \n";
	for (int j=0;j<comCount;j++){
	
/*s
		cout << "config[j]\n";
		for (int a=0;a<4;a++)
		cout << srvcfg[j].cfg[a] << "  ";
		cout << "\n";

*/

		int found =0;
 		for(int k =0;k<4;k++){
         	if (r[i] ==  srvcfg[j].cfg[k]){
			found = 1;
			//cout << "Found config" << "\n";
			break;
         	}//if
				
		}//k: array of each config

		
		if (found == 0){//cal cost for processing r[i]
			//cout<< "found == 0" << "\n";
			int origcfg0[4];
			int origcfg1[4];
			int origcfg2[4];
			int origcfg3[4];

			for(int l =0;l<4;l++){

			origcfg0[l] = srvcfg[j].cfg[l];
			origcfg1[l] = srvcfg[j].cfg[l];
			origcfg2[l] = srvcfg[j].cfg[l];
			origcfg3[l] = srvcfg[j].cfg[l];
			
			}

			origcfg0[0] = r[i];
			origcfg1[1] = r[i];
			origcfg2[2] = r[i];
			origcfg3[3] = r[i];
			/*
			for(int k =0;k<4;k++)
			cout << "cfg0: " << origcfg0[k] << " ";
			cout << "\n";
			
			
			for(int k =0;k<4;k++)
			cout <<  "cfg1: "<<  origcfg1[k] << " ";
			cout << "\n";
			
			for(int k =0;k<4;k++)
			cout <<  "cfg2: " << origcfg2[k] << " ";
			cout << "\n";
			

			for(int k =0;k<4;k++)
			cout <<  "cfg3: " << origcfg3[k] << " ";
			cout << "\n";
			*/


			int cost0,cost1,cost2,cost3,cost;

			//config0

			//cost0= disStrings(D,C0,origcfg0); //+ D[r[i],origcfg0[0]];
			int index;
			int m = sizeof(origcfg0)/sizeof(origcfg0[0]); 
  			sort(origcfg0, origcfg0+m); 
			for (int z=0;z<comCount;z++){
				if (origcfg0[0] == srvcfg[z].cfg[0] &&
					origcfg0[1] == srvcfg[z].cfg[1] &&
						origcfg0[2] == srvcfg[z].cfg[2] &&
							origcfg0[3] == srvcfg[z].cfg[3]) {
					
				index = z;
				//cout << "index : " << index <<"\n";
				}
				
			}
			/*
			cout << "XXX: " << srvcfg[index].cost << "\n";
			cout << "DDDD: " << D[r[i]][srvcfg[j].cfg[0]] << "\n";
			*/
			cost0= srvcfg[index].cost + D[r[i]][srvcfg[j].cfg[0]];

			//config1
			 //int index;
                         m = sizeof(origcfg1)/sizeof(origcfg1[0]);
                         sort(origcfg1, origcfg1+m);
                         for (int z=0;z<comCount;z++){
                                 if (origcfg1[0] == srvcfg[z].cfg[0] &&
                                         origcfg1[1] == srvcfg[z].cfg[1] &&
                                                 origcfg1[2] == srvcfg[z].cfg[2] &&
                                                         origcfg1[3] == srvcfg[z].cfg[3]) {
                                         
                                 index = z;
                                 //cout << "index : " << index <<"\n";
                                 }
                          
                         }
			cost1= srvcfg[index].cost + D[r[i]][srvcfg[j].cfg[1]];

			//config2
			  //int index;
			  m = sizeof(origcfg2)/sizeof(origcfg2[0]);
                          sort(origcfg2, origcfg2+m);
                          for (int z=0;z<comCount;z++){
                                  if (origcfg2[0] == srvcfg[z].cfg[0] &&
                                          origcfg2[1] == srvcfg[z].cfg[1] &&
                                                  origcfg2[2] == srvcfg[z].cfg[2] &&
                                                          origcfg2[3] == srvcfg[z].cfg[3]) {
                                          
                                  index = z;
                                  //cout << "index : " << index <<"\n";
                                  }
                           
                          }
                         cost2= srvcfg[index].cost + D[r[i]][srvcfg[j].cfg[2]];
			//config3
			   m = sizeof(origcfg3)/sizeof(origcfg3[0]);
                           sort(origcfg3, origcfg3+m);
                           for (int z=0;z<comCount;z++){
                                   if (origcfg3[0] == srvcfg[z].cfg[0] &&
                                           origcfg3[1] == srvcfg[z].cfg[1] &&
                                                   origcfg3[2] == srvcfg[z].cfg[2] &&
                                                           origcfg3[3] == srvcfg[z].cfg[3]) {
                                           
                                   index = z;
                                   //cout << "index : " << index <<"\n";
                                   }
                            
                           }
                          cost3= srvcfg[index].cost + D[r[i]][srvcfg[j].cfg[3]];


			//cost1= disStrings(D,C0,origcfg1); //+ D[r[i],origcfg1[1]];
		/*	
			cout << "cost0: "<< cost0 << "\n";
			cout << "cost1: "<< cost1 << "\n"; 
			cout << "cost2: "<< cost2 << "\n";
			cout << "cost3: "<< cost3 << "\n";
		*/		
			cost = min (cost0,cost1);
			//cout << "min cost1: "<< cost << "\n";
			cost = min (cost,cost2);
			//cout << "min cost2: "<< cost << "\n";	
			cost = min (cost,cost3);

			//ssscout << "min cost: "<< cost << "\n";
				
					

			srvcfg[j].cost= cost;
			//ssscout<< cost <<"  ";
		
		}
		else if (found == 1){ //cost is the same as previous step 
			//cout << "Found == 1" << " \n";
			srvcfg[j].cost = srvcfg[j].cost;
			
		}
	}//j :Number of  configs
	
	}//i: Number of requests
	int mini = srvcfg[0].cost;
	for (int s=0;s < comCount; s++){
		if (srvcfg[s].cost < mini)
			mini = srvcfg[s].cost;
	}
		
	return mini;
}

//*****************************************************************************************************//

int minEdgeBFS(vector <int> edges[], int u, int v, int n) 
{

    // visited[n] for keeping track of visited 
    // node in BFS 
    vector<bool> visited(n, 0); 
     	
    // Initialize distances as 0 
    vector<int> distance(n, 0); 
  
    // queue to do BFS. 
    queue <int> Q; 
    distance[u] = 0; 
 	//std::cout << "u: source:  " << u << "\n"; 
    Q.push(u); 
    visited[u] = true; 
    while (!Q.empty()) 
    {
        int x = Q.front();
	Q.pop(); 
        //std::cout << "front: " << x << "\n" ;
 
      for (int i=0; i<edges[x].size(); i++) 
	//for (int i=0; i<5; i++)
        {
	//std::cout << "visited: " << visited[edges[x][i]]  << "\n" ;	 
            if (visited[edges[x][i]]) 
                continue; 
  
            // update distance for i 
            distance[edges[x][i]] = distance[x] + 1; 
            Q.push(edges[x][i]); 
            visited[edges[x][i]] = 1; 
        } 
    } 
    return distance[v];
} 

//************************************************************************************************//
int** computeDistancesFromFile(string fileName)
{
	//int nodes = 0;
	std::string N; 
	vector <int> edges[5000];
  	std::ifstream file(fileName);
  	std::string line;
  	while (std::getline(file, line))
  	{
  		// Ignore headers and comments
  		if ( (line.length() >= 6) && (line[0] != '%'))
		{
			N = line[0];
			N.append(1,line[1]);
			cout << "NNNNNNN: " << N << "\n";
			nodes = std::stoi(N);	
			cout << " Number of nodes: " <<  nodes << "\n";	
			continue;	
		}	
		else if (line[0] == '%')
		{
          		continue;
		}
      		std::istringstream iss(line);
      		int m, n;
      		if (!(iss >> m >> n)) { break; } // error
  
      		// process pair (m,n)
          	std::cout << m << "  " <<  n << "\n";
	 	addEdge(edges, m, n);	  
  	}
	
  	std::cout << "\n";
  	//fin.close();

 	int** matrix;                      // Creates a pointer to the array
 	matrix = new int*[nodes];            // Creates the array of M*M size
 	//std::fill(matrix, &matrix[0][0], &matrix[M][0], 0); // From <algorithm>, zeros all entries.
 	for (int i = 0; i < nodes; i++)
     	{
		matrix[i]= new int[nodes];
         	for (int j = 0; j < nodes; j++)
         	{
			matrix[i][j] = minEdgeBFS(edges, i,j , nodes);
         	}
         	//std::cout << std::endl;
     	}
	for (int i = 0; i < nodes; i++)
       	{
        	for (int j = 0; j < nodes; j++)
          	{
			cout << matrix[i][j] << "  ";
          	}
          	std::cout << std::endl;
      	}

return matrix;
  }//


//********************************************************************************************************************//

int computeGreedy(int** D,int r[],int C0[]){
	int totalCost = 0;
	int ymin =0;
	int xmin =0;
	//int len =  sizeof(r)/sizeof(r[0]);  //request sequence length	
	int length = reqLength;

	for (int n = 0; n < length; n++)
	{
/*
		cout <<  "original C0 "<< "\n" ;
	
		for (int k = 0; k<4; k++)// k : number of servers
	{
                 cout << C0[k]<< "  ";
	}
	cout <<   "\n" ;
*/
		int x = r[n];
		int x0 = r[n];  //nth request
		int y0 = C0[0]; //first server
		int min = D[x0][y0];	//Distance between request and first server
		ymin = y0;
		//cout << x << "min dis between request and  first server : \n"  << min  << "\n";
	for (int j = 0; j < 4; j++) //finding nearest server to current request
	{
		int y = C0[j];
		//cout << "nth request: " << x << "  kth server: " << y << "\n";
		//cout << x << "min dis between request and  kth  server : \n"  << D[x][y]  << "\n";
		if (D[x][y] < min)
		{
			min = D[x][y];
			xmin = x;
			//ymin = y;
			ymin = j;
		}
		
		//min = std::min(min, D[x][y]);
		
                 //ssscout << D[x][y] << ' ';
	}
	//cout << "MIN:  " <<  min << "\n";	
	totalCost = totalCost + min;
	//cout << "min: " << "xmin: " << " ymin: " << min << "  " << xmin << "  " << ymin << "\n";
	//update C0 (current position of servers)
	C0[ymin] = r[n];
	sort(C0, C0+4);
/*
	std::cout << "updated C0 " << "\n" ;
	for (int k = 0; k<4; k++)
	{
		cout << C0[k]<< "  ";
	}
	cout <<   "\n" ;	
*/
       	}
	//std::cout << "totalcost: " << totalCost ;
	//std::cout << std::endl;	
	return totalCost;
}	
/*
int MinimumDisTwoConfig(int** D,int arr1[],int arr2[]){
	int totDis = 0;
	for (int i=0;i<4; i++){
		if (arr1[i] == -1)
			continue;
		if (arr2[i] == -1)
			continue;
		int xpoint;
		int ypoint;
		xpoint = arr1[i];
		ypoint = arr2[i];
		totDis= D[xpoint][ypoint]+ totDis; 
			
	}
	return totDis;

}
*/

int main()
{

	cout << "Enter the number of requests:  " << "\n";
	cin >> reqLength;


//*****************************Input Graphs from website *******************************// 	 
	//string fileName = "ENZYMES_g120.edges";//correct
	//string fileName = "ENZYMES_g74.edges";
	//string fileName = "ENZYMES_g327.edges";
	//string fileName = "ENZYMES_g193.edges";//correct
	//string fileName = "ENZYMES_g226.edges";//correct


//****************************Our Graphs***********************************************//

	string fileName = "test_25_007.txt";
	//string fileName = "test_33_008.txt";
	//string fileName = "test_38_0075.txt";
	//string fileName = "test_40_008.txt";
	//string fileName = "test_46_0065.txt";


//***************************Generate sequence Randomly*********************************//


	//int requests[] = {0,40,40,16,28,36,28,18,35,3,27,30,15,19,19,8,43,22,10,3};//f
	//int requests[] = {23,22,21,20,19,18,17,16,15,14,13,14,15,16,15,16,17,16,17,18};//sec
	//int requests[] = {20,22,15,28,18,19,8,17,26,11,28,37,16,19,10,32,6,22,18,30};//third
	int requests[] = {23,22,21,20,19,18,19,20,19,20,19,20,19,18,17,16,15,14,15,14};//forth
	
	
	//comCount = calComCount(nodes,4);

	int** Distance; //Minimum distance between nodes

	Distance = computeDistancesFromFile(fileName);//Compute minimum distance 

	//int C0[] = {0,1,2,3};
	int C0[] = {4,5,6,7};
	//int requests[] = {7,8,3,4,5,4,6,6,8,3,7,8,4,5,4,6,6,8,7,8,4,5,4,6,6,8,3};
	//int requests[] = {0,1,2,3,0,0,1,2,3,0,0,1,2,3,0,0,1,2,3,0,0,1,2,3,0,0};

//********************************************************************************//
	int  minGreedy = computeGreedy( Distance, requests, C0);
	cout << "Minimum cost in Greedy: " << minGreedy << "\n";
//********************************************************************************//

	

	//Intialize all points
	for(int k=0; k < nodes; k++) 
	points.push_back(k);

	calCombinationCFG();//Produce all combination 4 of n (number of nodes)
	cout << "Number of combinations : " << comCount << "\n";


	for(int i=0;i<combinations.size(); i++)
	{
		for(int j =0; j<combinations[i].size();j++)
		{
			srvcfg[i].cfg[j] = combinations[i][j];
			//cout<<combinations[i][j]<<" ";
			//cout<< srvcfg[i].cfg[j] <<" ";
		}

		//cout<<endl;
	}

//*********************Initial Configuration ***********************************//
	//int C00[4] = {0,1,2,3};
	int C00[4] = {4,5,6,7};



//********************Distance between different configuration*******************//
	int C1[4]= {-1,-1,-1,-1};
	//disStrings(C00,C1);
	int dis;
	for (int i =0; i< comCount; i++){

	for (int j =0;j<4;j++){
	C1[j] = srvcfg[i].cfg[j];
	//cout<< "C1: "<<C1[j] <<"  ";
	}
	//cout << "\n";

        int m = sizeof(C1)/sizeof(C1[0]);
        sort(C1, C1+m);


	dis = disStrings(Distance,C00,C1);
	//dis = MinimumDisTwoConfig(Distance,C00,C1);
	//cout << "dis: " << dis << "\n";
	srvcfg[i].cost = dis;

	}


	/*
	int C11[4]= {7,1,2,3};
	dis = disStrings(Distance,C00,C11);
	cout << "xxxxxxxxxxdis: "<< dis << "\n";
	*/

	//Fill struct of configurations

	//for (int i;i<4;i++)
	//cout << C00[i]<< " ";
	//for (int i =0; i< comCount; i++){
	//	cout<< srvcfg[i].cost <<" \n";
	//}


	int Minimum = computeOPT(Distance,requests,C00);
	cout << "Minimum cost for OPT: " <<  Minimum <<  "\n";
	//cout << "OPTCosts: " << "\n";

	/*
	for (int t=0;t<comCount;t++)
	{
		cout << srvcfg[t].cost << "  ";

	}
	*/
	int MinimumWFA = computeWFA(Distance,requests,C00);
	cout << "Minimum cost for WFA: " <<  MinimumWFA <<  "\n";


	return 0;

}


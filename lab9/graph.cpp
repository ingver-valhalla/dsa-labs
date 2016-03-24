#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <cfloat>

using namespace std;

//---------------------------------------------------------------------------
struct Item {
	int D;
	Item * Next;
	Item():D(-1),Next(0){}
	Item(int V): D(V),Next(0){}
};
//---------------------------------------------------------------------------
class Cont {
protected:
	Item * _Head;
	Item * _Tail;
public:
	Cont():_Head(0),_Tail(0){}
	bool Empty(void) {return _Head?false:true;}
	virtual void Push(int V) = 0;
	virtual int  Pop(void);
	~Cont();
};
//---------------------------------------------------------------------------
class CStack: public Cont {
public:
	CStack():Cont(){}
	virtual void Push(int V);
};
//---------------------------------------------------------------------------
class CQueue: public Cont {
public:
	CQueue():Cont(){}
	virtual void Push(int V);
};
//---------------------------------------------------------------------------
class Matrix {
protected:
	double ** _m;
	int _Size;
	virtual  void   Init(double Value);
public:
	Matrix(int Size);
	Matrix(const Matrix & M);
	double operator () (int row, int col);
	int    Size(void) {return _Size;}
	void   Set(int row, int col, double Value);
	virtual  void   Print(void);
	void   PrintFloydPath(int From, int To);
	void   PrintFloydPaths();
	~Matrix();
};
//---------------------------------------------------------------------------
class Graph: public Matrix {
	virtual void    Init(double Value);
public:
	Graph(int Size):Matrix(Size){Init(DBL_MAX);}
	Graph(const Graph & G):Matrix(G){}
	virtual void    Random(double Density, double MaxWeight = 1);
	virtual void    Print(void);
	virtual void    AddEdge(int V0, int V1, double Weight = 1);
	virtual void    DeleteEdge(int V0, int V1);
	virtual  int    EdgeCount(void);
	int    VertexCount(void) {return _Size;}
	virtual Graph   ShortestPath(int From, int To);
	void    Visit(int From, Cont & C);
	void    FindMinEdge(int & V0, int & V1);
	Graph   Floyd(Matrix & M);
	Graph   Kruskal(void);
	int     Hamiltonian(int v, int w, int Length, bool * Labelled, Graph & G);
	Graph   HamiltonianPath(int From, int To);
	void    Eulerian();
	void    HamiltonianGraph();
};
//---------------------------------------------------------------------------
class SGraph: public Graph {
public:
	SGraph(int Size):Graph(Size){}
	SGraph(const SGraph & G): Graph(G){}
	virtual void    Random(double Density, double MaxWeight = 1);
	virtual  int    EdgeCount(void) {return Graph::EdgeCount()/2;};
	virtual void    AddEdge(int V0, int V1, double Weight = 1);
};
//---------------------------------------------------------------------------
class WGraph: public Graph {
public:
	WGraph(int Size):Graph(Size){}
	WGraph(const WGraph & G): Graph(G){}
	virtual  int    EdgeCount(void) {return Graph::EdgeCount()/2;};
	virtual void    AddEdge(int V0, int V1, double Weight = 1);
};
//---------------------------------------------------------------------------
class OrGraph: public Graph {
public:
	OrGraph(int Size):Graph(Size){}
	OrGraph(const OrGraph & G): Graph(G){}
	virtual void    Random(double Density, double MaxWeight = 1);
	virtual void    AddEdge(int V0, int V1, double Weight = 1);
	OrGraph TransClosure(bool PathOrder);
	
};
//---------------------------------------------------------------------------
class OrWGraph: public Graph {
public:
	OrWGraph(int Size):Graph(Size){}
	OrWGraph(const OrWGraph & G): Graph(G){}
	virtual void    Random(double Density, double MaxWeight = 1);
};
//---------------------------------------------------------------------------
struct Deikstra {
	bool   Label;
	double Path;
	int    Vertex;
	Deikstra():Label(false),Path(DBL_MAX),Vertex(-1){}
};
//---------------------------------------------------------------------------
int Cont::Pop(void)
{
	if(_Head)
	{
		Item * I = _Head;
		_Head = _Head->Next;
		int V = I->D;
		delete I;
		if (!_Head) _Tail = 0;
		return V;
	}
	return -1;
}
//---------------------------------------------------------------------------
Cont::~Cont()
{
	while(_Head)
	{
		Item * I = _Head->Next;
		delete _Head;
		_Head = I;
	}
}
//---------------------------------------------------------------------------
void CStack::Push(int V)
{
	if(_Head)
	{
		Item * I = new Item(V);
		I->Next = _Head;
		_Head = I;
	}
	else
		_Tail = _Head = new Item(V);
}
//---------------------------------------------------------------------------
void CQueue::Push(int V)
{
	if(_Head)
	{
		_Tail->Next = new Item(V);
		_Tail = _Tail->Next;
	}
	else
		_Tail = _Head = new Item(V);
}
//---------------------------------------------------------------------------
Matrix::Matrix(int Size):_m(0),_Size(0)
{
	if(Size > 0)
	{
		_Size = Size;
		_m = new double*[_Size];
		for(int i = 0; i < _Size; i++)
			_m[i] = new double [_Size];
		Init(0);
	}
}
//---------------------------------------------------------------------------
Matrix::Matrix(const Matrix & M)
{
	if (&M != this)
	{
		_Size = 0;
		_m = 0;
		if(M._Size > 0)
		{
			_Size = M._Size;
			_m = new double*[_Size];
			for(int i = 0; i < _Size; i++)
				_m[i] = new double [_Size];
		}
		for(int i = 0; i < _Size; i++)
			for(int j = 0; j < _Size; j++)
				_m[i][j] = M._m[i][j];
	}
}
//---------------------------------------------------------------------------
void   Matrix::Init(double Value)
{
	for(int i = 0; i < _Size; i++)
		for(int j = 0; j < _Size; j++)
			_m[i][j] = Value;
}
//---------------------------------------------------------------------------
double Matrix::operator () (int row, int col)
{
	if(row < _Size && col < _Size && row >= 0 && col >= 0)
		return _m[row][col];
	return DBL_MAX;
}
//---------------------------------------------------------------------------
void   Matrix::Set(int row, int col, double Value)
{
	if(row < _Size && col < _Size && row >= 0 && col >= 0)
		_m[row][col] = Value;
}
//---------------------------------------------------------------------------
void   Matrix::Print(void)
{
	for(int i = 0; i < _Size; i++)
	{
		for(int j = 0; j < _Size; j++)
			printf("%10.2lf",_m[i][j]);
		printf("\n");
	}
}
//---------------------------------------------------------------------------
void   Matrix::PrintFloydPath(int From, int To)
{
	if (From != To)
	{
		printf("Path from %d to %d :  ",From,To);
		int v = From;
		printf("%d ",v);
		do
		{
			v = _m[v][To];
			if (v == -1)
			{
				printf("--> |x|");
				break;
			}
			printf("--> %d ",v);
		}
		while(v != To);
		printf("\n");
	}
}
//---------------------------------------------------------------------------
void   Matrix::PrintFloydPaths()
{
	for (int v = 0; v < _Size; v++)
		for (int w = 0; w < _Size; w++)
			PrintFloydPath(v,w);
	printf("\n");
}
//---------------------------------------------------------------------------
Matrix::~Matrix()
{
	for(int i = 0; i < _Size; i++)
		delete [] _m[i];
	delete [] _m;
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
void   Graph::Init(double Value)
{
	for(int i = 0; i < _Size; i++)
		for(int j = 0; j < _Size; j++)
			if(i != j) _m[i][j] = Value;
}
//---------------------------------------------------------------------------
void    Graph::AddEdge(int V0, int V1, double Weight)
{
	if(V0 >= 0 && V1 >= 0 && V0 < _Size && V1 < _Size && V0 != V1 && Weight >= 0)
	{
		_m[V0][V1] = Weight;
	}
}
//---------------------------------------------------------------------------
void    Graph::DeleteEdge(int V0, int V1)
{
	if(V0 >= 0 && V1 >= 0 && V0 < _Size && V1 < _Size && V0 != V1)
	{
		_m[V0][V1] = DBL_MAX;
	}
}
//---------------------------------------------------------------------------
void   Graph::Random(double Density, double MaxWeight)
{
	if (Density >= 0 && Density <= 1 && MaxWeight >= 0)
	{
		for(int i = 0; i < _Size; i++)
			for(int j = 0; j < _Size; j++)
				if(i > j)
				{
					if(Density >= (double)rand()/RAND_MAX)
						_m[i][j] = MaxWeight*rand()/RAND_MAX;
					else
						_m[i][j] = DBL_MAX;
					_m[j][i] = _m[i][j];
				}
				else
					continue;
	}
}
//---------------------------------------------------------------------------
void   Graph::Print(void)
{
	for(int i = 0; i < _Size; i++)
	{
		for(int j = 0; j < _Size; j++)
			if(_m[i][j] < DBL_MAX)
				printf("%7.2lf",_m[i][j]);
			else
				printf("     --");
			printf("\n");
	}
	printf("\n");
}
//---------------------------------------------------------------------------
Graph Graph::ShortestPath(int From, int To)
{
	if (From >= 0 && From < _Size && To < _Size && To >= 0)
	{
		if(From == To) return Graph(0);
		Deikstra * D = new Deikstra[_Size];
		D[From].Path   = 0;
		D[From].Vertex = -1;
		int U, V = From;
		double SPath;
		do
		{
			D[V].Label = true;
			for(U = 0; U < _Size; U++)
			{
				if(D[U].Label || _m[V][U] == DBL_MAX) continue;
				if(D[U].Path > D[V].Path + _m[V][U])
				{
					D[U].Path   = D[V].Path + _m[V][U];
					D[U].Vertex = V;
				}
			}
			SPath = DBL_MAX;
			V = -1;
			for(U = 0; U < _Size; U++)
				if(!(D[U].Label) && D[U].Path < SPath)
				{
					SPath = D[U].Path;
					V = U;
				}
		}
		while(V != -1 && V != To);
		Graph G(_Size);
		V = To;
		while(D[V].Vertex != -1)
		{
			G.AddEdge(V,D[V].Vertex,_m[V][D[V].Vertex]);
			V = D[V].Vertex;
		}
		delete [] D;
		return G;
	}
	return Graph(0);
}
//---------------------------------------------------------------------------
int    Graph::EdgeCount(void)
{
	if(_Size)
	{
		int Count = 0;
		for(int i = 0; i < _Size; i++)
			for(int j = 0; j < _Size; j++)
			{
				if(i == j) continue;
				if(_m[i][j] < DBL_MAX) Count++;
			}
			return Count;
	}
	return 0;
}
//---------------------------------------------------------------------------
Graph   Graph::Floyd(Matrix & M)
{
	Graph G(*this);
	if(M.Size() == _Size)
	{
		for(int i = 0; i < _Size; i++)
			for(int s = 0; s < _Size; s++)
				M.Set(i, s, G._m[i][s] == DBL_MAX ? -1 : s);
			for(int i = 0; i < _Size; i++)
				for(int s = 0; s < _Size; s++)
					if (G._m[s][i] < DBL_MAX)
						for(int t = 0; t < _Size; t++)
							if (G._m[i][t] < DBL_MAX)
								if(G._m[s][t] > G._m[s][i] + G._m[i][t])
								{
									G._m[s][t] = G._m[s][i] + G._m[i][t];
									M.Set(s, t, M(s, i));
								}
	}
	return G;
}
//---------------------------------------------------------------------------
void    Graph::FindMinEdge(int & V0, int & V1)
{
	double MinWeight = DBL_MAX;
	V0 = V1 = -1;
	for(int i = 0; i < _Size; i++)
		for(int s = 0; s < _Size; s++)
		{
			if (i == s)  continue;
			//if (_m[i][s] < DBL_MAX)
			if( _m[i][s] < MinWeight )
			{
				MinWeight = _m[i][s];
				V0        = i;
				V1        = s;
			}
		}
	if (MinWeight == DBL_MAX)
		V0 = V1 = -1;
}
//---------------------------------------------------------------------------
Graph   Graph::Kruskal(void)
{
	Graph MST(_Size);
	Graph G(*this);
	bool * Labelled = new bool[_Size];
	for(int i = 0; i < _Size; i++) Labelled[i] = false;
	int V0 = -1, V1 = -1;
	while (MST.EdgeCount() < (_Size - 1)*2)
	{
		G.FindMinEdge(V0,V1);
		if(V0 == -1 && V1 == -1)
			break;
		if(!Labelled[V0] || !Labelled[V1])
		{
			MST.AddEdge(V0,V1,_m[V0][V1]);
			MST.AddEdge(V1,V0,_m[V0][V1]);
			Labelled[V0] = true;
			Labelled[V1] = true;
		}
		G.DeleteEdge(V0,V1);
		G.DeleteEdge(V1,V0);
	}
	delete [] Labelled;
	return MST;
}
//---------------------------------------------------------------------------
void    Graph::Visit(int From, Cont & C)
{
	if (From >= 0 && From < _Size)
	{
		int V = From;
		bool * Labelled = new bool[_Size];
		for(int i = 0; i < _Size; i++)
			Labelled[i] = false;
		C.Push(V);
		Labelled[V] = true;
		do
		{
			V = C.Pop();
			printf("%d ", V);
			for(int U = 0; U < _Size; U++)
			{
				if(!Labelled[U] && _m[V][U] != DBL_MAX)
				{
					C.Push(U);
					Labelled[U] = true;
				}
			}
		}
		while(!C.Empty());
		delete [] Labelled;
		printf("\n");
	}
}
//---------------------------------------------------------------------------
int  Graph::Hamiltonian(int v, int w, int Length, bool * Labelled, Graph & G)
{
	if (v == w)
		return Length == 0 ? 1 : 0;
	Labelled[v] = true;
	for (int t = 0; t < _Size; t++)
		if( _m[v][t] < DBL_MAX && _m[v][t] != 0 && !Labelled[t])
			if ( Hamiltonian(t, w, Length - 1, Labelled, G))
			{
				G.AddEdge(t,v,_m[t][v]);
				G.AddEdge(v,t,_m[v][t]);
				return 1;
			}
			Labelled[v] = false;
		return 0;
}
//---------------------------------------------------------------------------
void Graph::Eulerian()
{
	// iterating by rows
	for( int i = 0; i < _Size-1; ++i ) {
		int curDegree = 0;
		int val;

		// Count current degree
		for( int j = 0; j < i; ++j ) {
			if( _m[i][j] != DBL_MAX ) {
				++curDegree;
			}
		}

		// Generate remaining
		for( int j = i+1; j < _Size-2; ++j ) {
			val = rand() % 2;
			if( val ) {
				++curDegree;
				_m[i][j] = _m[j][i] = val;
			}
		}

		// Special case for two last values in each row

		if( i < _Size-2 ) {
			// If not generated any edges yet make an edge.
			// Current degree will be odd (will be useful after)
			if( !curDegree ) {
				_m[i][_Size-2] = _m[_Size-2][i] = 1;
				++curDegree;
			}
			// Else let the random decide
			else if( (val = rand()%2) ) {
				++curDegree;
				_m[i][_Size-2] = _m[_Size-2][i] = 1;
			}

		}

		// Check if degree is odd on last iteration and insert necessary edge
		if(curDegree%2)
			_m[i][_Size-1] = _m[_Size-1][i] = 1;
	}
}
//---------------------------------------------------------------------------
void Graph::HamiltonianGraph()
{
	// Dirac's condition
	int half = (int)(_Size / 2.0 + 0.5);
	cout << "Degree must be >= " << half << endl;
	int i, j;
	int curDegree;
	int val;

	// Iterate by rows
	for( i = 0; i < _Size; ++i ) {
		curDegree = 0;
		// Count current degree
		for( j = 0; j < i && (_Size-j-1 > half-curDegree); ++j ) {
			if( _m[i][j] != DBL_MAX ) {
				++curDegree;
			}
		}
		// generate edges, excluding last column, and checking if Dirac's
		// condition can be satisfied
		for( ; j < _Size && (_Size-j-1 > half-curDegree); ++j ) {
			if( i == j ) {
				continue;
			}
			if( (val = rand() % 2) ) {
				++curDegree;
				_m[i][j] = _m[j][i] = val;
			}
		}
		// Insert edges to satisfy Dirac's condition
		if( i > 0 && j == i+1 ) {
			_m[i][i-1] = _m[i-1][i] = 1;
		}
		for( ; j < _Size-1; ++j ) {
			_m[i][j] = _m[j][i] = 1;
		}
		//cout << "i = " << i << endl;
		//Print();
	}
}
//---------------------------------------------------------------------------
Graph   Graph::HamiltonianPath(int From, int To)
{
	bool * Labelled = new bool[_Size];
	for (int i = 0; i < _Size; i++) Labelled[i] = false;
	Graph G(_Size);
	int f = Hamiltonian(From,To, _Size - 1 ,Labelled, G);
	delete [] Labelled;
	return G;
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
void    SGraph::AddEdge(int V0, int V1, double Weight)
{
	Graph::AddEdge(V0,V1,1);
	Graph::AddEdge(V1,V0,1);
}
//---------------------------------------------------------------------------
void   SGraph::Random(double Density, double MaxWeight)
{
	if (Density >= 0 && Density <= 1 && MaxWeight >= 0)
	{
		for(int i = 0; i < _Size; i++)
			for(int j = 0; j < _Size; j++)
				if(i > j)
				{
					if(Density >= (double)rand()/RAND_MAX)
						_m[i][j] = 1.0;
					else
						_m[i][j] = DBL_MAX;
					_m[j][i] = _m[i][j];
				}
				else
					continue;
	}
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
void    WGraph::AddEdge(int V0, int V1, double Weight)
{
	Graph::AddEdge(V0,V1,Weight);
	Graph::AddEdge(V1,V0,Weight);
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
void    OrGraph::AddEdge(int V0, int V1, double Weight)
{
	Graph::AddEdge(V0,V1,1);
}
//---------------------------------------------------------------------------
OrGraph OrGraph::TransClosure(bool PathOrder)
{
	OrGraph G(*this);
	for(int i = 0; i < _Size; i++)
		for(int s = 0; s < _Size; s++)
			if (G._m[s][i] == 1)
				for(int t = 0; t < _Size; t++)
					G._m[s][t] = G._m[i][t] == 1 ? 1 : G._m[s][t];
				if(!PathOrder)
					for(int i = 0; i < _Size; i++)
						G._m[i][i] = 0;
					return G;
}
//---------------------------------------------------------------------------
void   OrGraph::Random(double Density, double MaxWeight)
{
	if (Density >= 0 && Density <= 1 && MaxWeight >= 0)
	{
		for(int i = 0; i < _Size; i++)
			for(int j = 0; j < _Size; j++)
			{
				if(i == j) continue;
				if(Density >= (double)rand()/RAND_MAX)
					_m[i][j] = 1.0;
				else
					_m[i][j] = DBL_MAX;
			}
	}
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
void   OrWGraph::Random(double Density, double MaxWeight)
{
	if (Density >= 0 && Density <= 1 && MaxWeight >= 0)
	{
		for(int i = 0; i < _Size; i++)
			for(int j = 0; j < _Size; j++)
			{
				if(i == j) continue;
				if(Density >= (double)rand()/RAND_MAX)
					_m[i][j] = MaxWeight*rand()/RAND_MAX;
				else
					_m[i][j] = DBL_MAX;
			}
	}
}

int main(int argc, char* argv[])
{
	srand(time(NULL));
	//cout << "DBL_MAX = " << DBL_MAX << endl;
	//Graph A(5);
	//A.Random(1, 1);
	////Matrix M(A.VertexCount());
	////Graph F(A.Floyd(M));
	//Graph G(A.Kruskal());
	//Graph H(A.HamiltonianPath(0,1));
	//cout << "A :" << endl;
	//A.Print();
	//cout << "Kruskal of A : " << endl;
	//G.Print();
	//cout << "Hamiltonian path in A : " << endl;
	//H.Print();
	////F.Print();
	////M.PrintFloydPaths();
	////M.Print();
	////G.Print();

	//Graph E(6);
	//E.Eulerian();
	//cout << "E :" << endl;
	//E.Print();

	Graph H(10);
	H.HamiltonianGraph();
	cout << "H :" << endl;
	H.Print();

	return 0;
}

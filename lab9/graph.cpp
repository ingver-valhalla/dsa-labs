#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <cfloat>
#include <ctime>
#include <fstream>

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
	virtual void Print();
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

	void    EulerianGraph();
	bool    isEulerian();
	bool    isConnected();
	bool    Hierholzer(int v, Cont& cont);
	bool    FindEulerianCycle(Cont& answer);
	void    PrintEulerianCycle();

	void    HamiltonianGraph();
	bool    isHamiltonian();
	void    PrintHamiltonianCycle();
	bool    FindHamiltonianCycle(int from, CStack& answer);
private:
	bool    FindHamiltonianCycleRecur(int from, int to, int len, CStack& answer, bool* marks);
	



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
		for(int j = 0; j < _Size; ++j)
			printf("-------");
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
Graph   Graph::HamiltonianPath(int From, int To)
{
	bool * Labelled = new bool[_Size];
	for (int i = 0; i < _Size; i++) Labelled[i] = false;
	Graph G(_Size);
	Hamiltonian(From,To, _Size - 1 ,Labelled, G);
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


//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
// Метод генерирации эйлерова графа
// 
int times; // для подсчета перегенераций
void Graph::EulerianGraph()
{
	times = 0;
	do {
		Init(DBL_MAX);
		// проход по строкам матрицы
		for( int i = 0; i < _Size-1; ++i ) {
			int curDegree = 0;
			int val;

			// подсчет текущей степени
			for( int j = 0; j < i; ++j ) {
				if( _m[i][j] == 1 ) {
					++curDegree;
				}
			}

			// генерация оставшихся ребер
			for( int j = i+1; j < _Size-2; ++j ) {
				val = rand() % 2;
				if( val ) {
					++curDegree;
					_m[i][j] = _m[j][i] = val;
				}
			}

			// обработка двух последних столбцов

			if( i != _Size-2 ) {
				// Если ни одного ребра еще не было сгенеровано, добавить одно
				// ребро. Текущая степень вершнины будет нечентной (что будет
				// учтено позже)
				if( !curDegree ) {
					_m[i][_Size-2] = _m[_Size-2][i] = 1;
					++curDegree;
				}
				// Иначе снова сгенерировать случайно
				else if( (val = rand()%2) ) {
					++curDegree;
					_m[i][_Size-2] = _m[_Size-2][i] = 1;
				}

			}
			// Проверка четности текущей вершины и вставка ребра при
			// необходимости
			if(curDegree%2 != 0) {
				_m[i][_Size-1] = _m[_Size-1][i] = 1;
			}
		}
		++times;
	} while (!isEulerian());

	//cout << "times = " << times << "\n";
}
//---------------------------------------------------------------------------
// Проверка эйлеровости
bool Graph::isEulerian()
{
	if(!isConnected())
		return false;

	bool has_edges = false;
	for(int i = 0; i < _Size; ++i) {
		int degree = 0;
		for(int j = 0; j < _Size; ++j) {
			if(_m[i][j] == 1) {
				++degree;
				has_edges = true;
			}
		}
		if(degree%2 != 0) {
				return false;
		}
	}
	return has_edges;
}
//---------------------------------------------------------------------------
// Проверка связности
bool Graph::isConnected()
{
	int* marks = new int[_Size]();
	marks[0] = 1; // достижимость вершин

	int i = 0, j = 0;
	for(int v = 0; v < _Size; ++v) {
		bool has_reachable = false;
		for(i = 0; i < _Size; ++i) {
			if(marks[i] == 1) {
				has_reachable = true;
				break;
			}
		}
		if(!has_reachable)
			break;

		for(j = 0; j < _Size; ++j) {
			if(_m[i][j] == 1 && marks[j] == 0) {
				marks[j] = 1; // пометить всех соседей i как достижимые
			}
		}
		marks[i] = 2; // вершина i обработана
	}
	// проверить, не осталось ли необработанных вершин
	for(i = 0; i < _Size; ++i) {
		if(marks[i] == 0) {
			delete[] marks;
			return false;
		}
	}

	delete[] marks;
	return true;
}
//---------------------------------------------------------------------------
// Генерация гамильтонова графа
void Graph::HamiltonianGraph()
{
	// Условик Дирака
	int half = (int)(_Size / 2.0 + 0.5);
	//cout << "Degree must be >= " << half << endl;
	int i, j;
	int curDegree;
	int val;

	// Проход по строкам
	for( i = 0; i < _Size; ++i ) {
		curDegree = 0;
		// Подсчет текущей степени
		for( j = 0; j < i && (_Size-j-1 > half-curDegree); ++j ) {
			if( _m[i][j] != DBL_MAX ) {
				++curDegree;
			}
		}
		// генерация ребер и проверка на возможность соблюдения условия Дирака 
		for( ; j < _Size && (_Size-j-2 > half-curDegree); ++j ) {
			if( i == j ) {
				continue;
			}
			if( (val = rand() % 2) ) {
				++curDegree;
				_m[i][j] = _m[j][i] = val;
			}
		}
		// Вставка ребер для последних столбцов для соблюдения условия Дирака
		for( ; j < _Size-1; ++j ) {
			if(i == j)
				continue;
			_m[i][j] = _m[j][i] = 1;
		}
	}
}
//---------------------------------------------------------------------------
// Проверка на гамильтоновость
bool Graph::isHamiltonian()
{
	double degree = 0;
	for(int i = 0; i < _Size; ++i) {
		for(int j = 0; j < _Size; ++j) {
			if(i == j && _m[i][j] == 1) {
				cout << "vertex can't be connected with itself\n";
				return false;
			}
			if(_m[i][j] == 1)
				++degree;
		}
		double half = _Size/2.0;
		if(degree < half) {
			cout << "degree must be not lesser than " << half << endl;
			return false;
		}
	}
	return true;
}
//---------------------------------------------------------------------------
// Вывод эйлерова цикла
void Graph::PrintEulerianCycle()
{
	CQueue eulerian_cycle;
	if(!FindEulerianCycle(eulerian_cycle)) {
		cout << "Can't find eulerian cycle\n";
		return;
	}
	
	// printing
	cout << "Eulerian cycle:\n";
	eulerian_cycle.Print();
	cout << endl;
	cout << "--------------" << endl;
}
//---------------------------------------------------------------------------
// Поиск эйлерова цикла
bool Graph::FindEulerianCycle(Cont& answer)
{
	if(!isEulerian()) {
		cout << "Graph is not eulerian\n";
		return false;
	}

	Graph g(*this);

	answer.Push(0);
	if (!g.Hierholzer(0, answer))
		return false;

	for(int i = 0; i < g._Size; ++i) {
		for(int j = 0; j < g._Size; ++j) {
			if(g._m[i][j] == 1) {
				g.Print();
				cout << "Remained edges\n";
				return false;
			}
		}
	}

	return true;
}
//---------------------------------------------------------------------------
// Алгоритм Хиерхолцера
// (эйлеров цикл помещается в контейнер answer, за исключением начальной
// вершины)
bool Graph::Hierholzer(int v, Cont& answer)
{
	if(v < 0 || v > _Size) {
		cout << "invalid vertex";
		return false;
	}

	CQueue cycles = CQueue();
	int p = v;

	do {
		for(int i = 0; i < _Size; ++i)
			if(_m[p][i] == 1) {
				cycles.Push(i);
				DeleteEdge(p, i);
				DeleteEdge(i, p);
				p = i;
				break;
			}
	} while(v != p);

	while((p = cycles.Pop()) != -1) {
		answer.Push(p);
		if (!Hierholzer(p, answer))
			return false;
	}

	return true;
}
//---------------------------------------------------------------------------
// Вывод гамильтонова цикла
void Graph::PrintHamiltonianCycle()
{
	if(!isHamiltonian()) {
		cout << "Graph is not hamiltonian\n";
		return;
	}

	CStack answer = CStack();
	if(!FindHamiltonianCycle(0, answer)) {
		cout << "There is no hamiltonian cycle\n";
		return;
	}

	// printing
	cout << "Hamiltonian cycle\n";
	int p = -1;
	while((p = answer.Pop()) != -1) {
		cout << p << " ";
	}
	cout << endl;
	cout << "--------------" << endl;
}
//---------------------------------------------------------------------------
// Поиск гамильтонова цикла
bool Graph::FindHamiltonianCycle(int from, CStack& answer)
{
	if(from < 0 || from > _Size) {
		cout << "FindHamiltonianCycle: Invalid vertex\n";
		return false;
	}

	bool *marks = new bool[_Size];
	for (int i = 0; i < _Size; ++i) {
		marks[i] = false;
	}
	
	answer.Push(from);
	marks[from] = true;

	for(int v = 0; v < _Size; ++v) {
		if(_m[from][v] == 1 && !marks[v]
		   && FindHamiltonianCycleRecur(v, from, _Size-1, answer, marks))
		{
			answer.Push(from);
			return true;
		}
	}

	answer.Pop();

	delete [] marks;

	return false;
}
//---------------------------------------------------------------------------
// Рекурсивная часть процедуры поиска гамильтонова цикла
bool Graph::FindHamiltonianCycleRecur(int from, int to, int len, CStack& answer, bool* marks)
{
	if(from < 0 || from > _Size || to < 0 || to > _Size) {
		cout <<  "FindHamiltonianCycleRecur: Invalid vertex\n";
		return false;
	}

	answer.Push(from);
	marks[from] = true;
	for(int v = 0; v < _Size; ++v) {
		if(_m[from][v] == 1 && !marks[v]
		   && FindHamiltonianCycleRecur(v, to, len-1, answer, marks))
		{
			return true;
		}
	}
	if(len == 1 && _m[from][to] == 1) {
		return true;
	}
	answer.Pop();
	marks[from] = false;
	return false;

}
//---------------------------------------------------------------------------
// Метод для вывода содержимого контейнера
void Cont::Print()
{
	Item *p = _Head;
	while(p != NULL) {
		cout << p->D << " ";
		p = p->Next;
	}
}
//---------------------------------------------------------------------------
// Тест времени работы поиска эйлерова цикла
void test_eulerian_time(fstream& f)
{
	cout << "Testing eulerian cycle search time...\n";
	f << "Eulerian cycle\n";

	int minEdges = 1e8, maxEdges = 0;
	const int cgraphs = 5000,
	      cverteces = 100,
	      maxEdgesPossible = cverteces * (cverteces - 1);

	struct {
		int count;
		double time;
	} stats[maxEdgesPossible];

	for (int i = 0; i < maxEdgesPossible; ++i) {
		stats[i] = { 0, 0 };
	}

	for (int i = 0; i < cgraphs; ++i) {
		Graph G(cverteces);
		G.EulerianGraph();
		clock_t start = 0;
		clock_t end = 0;
		int edges = G.EdgeCount();
		CQueue q;
		++stats[edges].count;


		start = clock();
		G.FindEulerianCycle(q);
		end = clock();
		stats[edges].time += 1000.0 * (end - start) / CLOCKS_PER_SEC;
		//cout << "time for " << edges << " edges: " << stats[edges].time;
		//cin.get();

		if (minEdges > edges)
			minEdges = edges;
		if (maxEdges < edges)
			maxEdges = edges;
		//cout << G.EdgeCount() << endl;
	}

	cout << "graphs generated: " << cgraphs << endl;
	cout << "verteces: " << cverteces << endl;
	cout << "max edges possible: " << cverteces * (cverteces - 1) << endl;
	cout << "minEdges = " << minEdges << endl;
	cout << "maxEdges = " << maxEdges << endl;
	cout << "diff = " << maxEdges - minEdges << endl;
	cout << "stats:\n";

	for (int i = 0, j = 0; i < maxEdgesPossible; ++i) {
		if (stats[i].count > 10) {
			stats[i].time = stats[i].time / stats[i].count;
			cout << "[" << i << ", " << stats[i].count << ", " << stats[i].time << "] ";
			f << "(" << i << ";" << stats[i].time << ")\n";
			f.flush();
			++j;
		}
		if (j == 5) {
			cout << endl;
			j = 0;
		}
	}
	cout << endl;
}
//---------------------------------------------------------------------------
// Тест времени поиска гамильтонова цикла
void test_hamiltonian_time(fstream& f)
{
	const int NVERTS = 10;
	const int NTESTS = 100;

	clock_t test_start = 0;
	clock_t test_stop = 0;
	double total = 0;

	total = 0;
	cout << "Testing hamiltonian cycle search time...\n";
	f << "Hamiltonian cycle\n";
	for (int v = 50; v <= 1000; v += 10) {
		//cout << v << "verteces:\n";
		//double min = DBL_MAX;
		//double max = 0;
		Graph g(v);
		for (int i = 0; i < NTESTS; ++i) {
			CStack stack;

			g.HamiltonianGraph();

			test_start = clock();
			if (!g.FindHamiltonianCycle(0, stack)) {
				cout << "Failed to find Hamiltonian cycle\n";
				return;
			}

			test_stop = clock();
			double diff = 1000.0 * (test_stop - test_start) / CLOCKS_PER_SEC; 
			total += diff;

			//if (min > diff)
				//min = diff;
			//if (max < diff)
				//max = diff;
			//cout << "diff: " << diff << endl;
		}

		//cout << "max diff: " << max - min << endl;
		
		double average = total / NTESTS;
		cout << "(" << v << ";" << average << ")\n";
		f << "(" << v << ";" << average << ")\n";
		//cin.get();
	}
}
//---------------------------------------------------------------------------
// Тест времени работы алгоритмов
void test_time()
{
	fstream f("out.txt", ios::out);
	if(!f) {
		cerr << "Can't open file out.txt\n";
		return;
	}
	cout << "test_time:\n";

	test_eulerian_time(f);
	test_hamiltonian_time(f);
}

int main(int argc, char* argv[])
{
	srand(time(NULL));

	//int max = 0;
	//for (int i = 0; i < 10; ++i) {
		//Graph G(7);
		//G.EulerianGraph();
		//G.Print();
		//if (times > max)
			//max = times;
		//G.PrintEulerianCycle();
		//cin.get();
	//}

	//cout << "max = " << max << endl;

	
	double gen = 0;
	double srch = 0;
	double elapsed = 0;
	clock_t test_start = 0;
	clock_t test_stop = 0;
	for (int i = 0; i < 10; ++i) {
		Graph G(10000);
		test_start = clock();
		G.HamiltonianGraph();
		test_stop = clock();
		elapsed = 1000.0*(test_stop - test_start) / CLOCKS_PER_SEC;
		cout << "gen elapsed " << elapsed  << "\n";
		gen += elapsed;
		//G.Print();
		//G.PrintHamiltonianCycle();

		CStack stack;
		test_start = clock();
		G.FindHamiltonianCycle(0,stack);
		test_stop = clock();
		//cin.get();

		elapsed = 1000.0*(test_stop - test_start) / CLOCKS_PER_SEC;
		cout << "srch elapsed " << elapsed  << "\n";
		srch += elapsed;
	}
	cout << "gen: " << (gen / 10) << endl;
	cout << "srch: " << (srch / 10) << endl;
	
	//cin.get();
	//test_time();

	return 0;
}

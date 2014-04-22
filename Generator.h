#pragma once
/*
	Project: Tree Diagram
	Author : Yangff
	Date   : 2014-4-20
*/
#ifndef _TreeDiagram_GENERATOR_H_
#define _TreeDiagram_GENERATOR_H_
#include "Common.h"
#include "Graph.h"
#include "Tree.h"
#include <map>
namespace TreeDiagram{
	template <class T>
	class Generator{
	public:
		T Generate();
	};

	template <class T>
	class RandomTreeGenerator : Generator<T>{
	protected:
		int n; int _gerrter;
		std::vector<int> prufer;
		int& Get(int x){
			if (x >= 1 && x <= prufer.size())
				return prufer[x - 1];
			SetLastError("Bad Requesr %d for [%d, %d]", x, 1, prufer.size());
			return _gerrter;
		}
		void GeneratePrufer(){
			if (prufer.size())
				return ;
			for (int i = 1; i <= n - 2; i++)
				prufer.push_back(rand()%n + 1);
		}
		static int getSet(std::vector<int> &d, int s, int t){
			for (int i = s; i <= t; i++){
				if (d[i] == 1)
					return i;
			}
			SetLastError("Not Any d[i] = 1?");
			return -1;
		}
	public:
		RandomTreeGenerator(int n):n(n),Generator<T>(){
			if (n < 3){
				SetLastError("RandomTreeGenerator require atleast %d points, but %d.", 3, n);
				n = 3;
			};
		}
		RandomTreeGenerator(std::vector<int> prufer):prufer(prufer),Generator<T>(){
			if (prufer.size() == 0){
				SetLastError("RandomTreeGenerator require prufer seq.");
				n = 3;
			}
		}
		// Mr. Kong, ni ke shi de ci zheng?

		auto Generate() -> typename std::enable_if<is_undirectedTree<T>::value, T>::type{
			// setp 1: Generate prufer seq.
			GeneratePrufer();
			// setp 2: Build Tree;

			std::vector<int> d; d.resize(n + 1);
			std::vector<bool> used; used.resize(n + 1);
			for (int i = 1; i <= n; i++)
				d[i] = 1, used[i] = false;
			T Tree;Times(n, [&]{ Tree.AddPoint(typename T::PointDataType(rand())); });
			for (int i = 1; i <= n - 2; i++)
				d[Get(i)] += 1;
			int idx, xx; idx = xx = getSet(d, 1, n);
			for (int j = 1; j <= n - 2; j++){
				used[xx] = true;
				Tree.AddEdge(xx, Get(j), typename T::EdgeDataType(rand()));
				d[Get(j)] --;
				if (Get(j) < idx && d[Get(j)] == 1)
					xx = Get(j);
				else
					idx = xx = getSet(d, idx + 1, n);
			}
			std::vector<int> last;
			for (int i = 1; i <= n; i++){
				if (used[i] == false)
					last.push_back(i);
			}
			Tree.AddEdge(last[0], last[1], typename T::EdgeDataType(rand()));
			return Tree;
		};
	}; // class

	template<class T>
	class RandomGraphGenerator : Generator<T> {
	protected:
		int n, m;
		std::pair<int, int> GetEdge(int id){
			int l = id / (n - 1);
			int r = id % (n - 1);
			if (r >= l)
				r++;
			return std::make_pair(l + 1, r + 1);
		}
		int GetId(int u, int v){
			u--; v--;
			if (v > u - 1)
				v++;
			return u * (n - 1) + v;
		}
		static void nextCur(int &cur, int lmt, std::map<int, bool> &used){
			cur += rand(); cur %= lmt;
			while (used[cur]){
				cur++;
				if (cur >= lmt)
					cur = 0;
			}
		}
	public:
		RandomGraphGenerator(int n, int m):n(n), m(m), Generator<T>(){
			if (m > n * (n - 1))
				SetLastError("For %d points, only %d edges can be generated, but %d given.", n, n * (n - 1), m);
		}
		auto Generate() -> typename std::enable_if<is_graph<T>::value, T>::type{
			std::map<int, bool> used;
			int cur = 0, lmt = n * (n - 1);
			T G; Times(n, [&]{G.AddPoint(typename T::PointDataType(rand()));});
			for (int i = 0; i < m; i++){
				nextCur(cur, lmt, used);
				G.AddEdge(GetEdge(cur).first, GetEdge(cur).second, typename T::EdgeDataType(rand()));
				used[cur] = true;
			}
			return G;
		};
	}; // class

	template<class T0, class T1>
		class MSTGraphGeneratorByTree : public RandomGraphGenerator<T0> {};
	template<class PointData, class EdgeData>
	class MSTGraphGeneratorByTree<Graph<PointData, EdgeData>, UndirectedTree<PointData, EdgeData> >
	: public RandomGraphGenerator<Graph<PointData, EdgeData> >
	{
	protected:
		typedef Graph<PointData, EdgeData> GType;
		typedef UndirectedTree<PointData, EdgeData> TType;
		TType T;
	public:

		MSTGraphGeneratorByTree(TType T, int m):T(T),RandomGraphGenerator<GType>(T.GetPointSize(), m){
			int n = T.GetPointSize();
			if (m < n - 1){
				SetLastError("A tree must have %d edges, but %d", n - 1, m);
				return ;
			}

		}

		GType Generate() {
			std::map<int, bool> used;
			int cur = 0, lmt = RandomGraphGenerator<GType>::n * (RandomGraphGenerator<GType>::n - 1);
			GType G;
			// InitTarjan
			std::vector<std::vector<std::pair<int,EdgeData> > > Fa;
			std::vector<int> Deep;
			Fa.resize(RandomGraphGenerator<GType>::n + 1);
			Deep.resize(RandomGraphGenerator<GType>::n + 1);
			for (int i = 1; i <= RandomGraphGenerator<GType>::n; i++)
				Fa[i].resize(21);
			T.Traversal(1, [&](int cur, int fa, int deep){
				Deep[cur] = deep;
				Fa[cur][0].first = fa;
				if (fa != 0)
					Fa[cur][1].second = T.GetEdge(fa, cur);
			});

			for (int p = 1; p <= 20; p++)
				for (int i = 1; i <= RandomGraphGenerator<GType>::n; i++){
					if (Fa[i][p-1].first != 0){
						Fa[i][p].first = Fa[Fa[i][p-1].first][p-1].first;
						Fa[i][p].second = std::max(Fa[i][p].second, Fa[Fa[i][p-1].first][p-1].second);
					};
				}

			T.EachPoint([&](int i, PointData p){ G.AddPoint(p); });
			int _unsafe_n = RandomGraphGenerator<GType>::n;
			auto _UNSAFE_GetId = [=](int u, int v){
				u--; v--;
				if (v > u - 1)
					v++;
				return u * (_unsafe_n - 1) + v;
			};
			T.EachEdge([&](int u, int v, EdgeData e){
				G.AddEdge(u,v,e);
				used[_UNSAFE_GetId(u, v)] = true;
			});
			for (int i = 0; i < RandomGraphGenerator<GType>::m - (RandomGraphGenerator<GType>::n - 1); i++){
				RandomGraphGenerator<GType>::nextCur(cur, lmt, used);
				int u = RandomGraphGenerator<GType>::GetEdge(cur).first, v = RandomGraphGenerator<GType>::GetEdge(cur).second;
			//	fprintf(stderr, "%d %d\n", u, v);
				int ou = u, ov = v;
				used[cur] = true;
				used[RandomGraphGenerator<GType>::GetId(v, u)] = true;
				if (Deep[u] > Deep[v])
					std::swap(u, v);
				EdgeData MaxV = 0;
				for (int c = 20; Deep[u] != Deep[v]; c--){
					if (Fa[v][c].first != 0 && Deep[Fa[v][c].first] >= Deep[u]){
						MaxV = std::max(MaxV, Fa[v][c].second);
						v = Fa[v][c].first;
					}
				}
				if (u != v){
					//fprintf(stderr, "cur %d %d\n", u, v);
					for (int c = 20; Fa[u][0].first != Fa[v][0].first; c--){
						if (Fa[u][c].first != Fa[v][c].first){
							MaxV = std::max(MaxV, Fa[u][c].second);
							MaxV = std::max(MaxV, Fa[v][c].second);
							u = Fa[u][c].first, v = Fa[v][c].first;
						}
					}
					if (u) MaxV = std::max(MaxV, Fa[u][0].second);
					if (v) MaxV = std::max(MaxV, Fa[v][0].second);
				}
				G.AddEdge(ou, ov, MaxV + 1);
				G.AddEdge(ov, ou, MaxV + 1);
			}
			return G;
		}

	};
}; // namspace
#endif

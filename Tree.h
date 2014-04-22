#pragma once
/*
	Project: Tree Diagram
	Author : Yangff
	Date   : 2014-4-20
*/
#ifndef _TreeDiagram_TREE_H_
#define _TreeDiagram_TREE_H_
#include "Graph.h"
#include <vector>
#include <functional>
namespace TreeDiagram{
	template<class PointData = Nothing, class EdgeData = Nothing, class PointDataI = PointData, class EdgeDataI = EdgeData>
	class UndirectedTree : public Graph<PointData, EdgeData, PointDataI, EdgeDataI> {
	protected:
		std::vector<int> mFa;
		int _getFather(int u){
			if (mFa[u] == u)
				return u;
			return _getFather(mFa[u]);
		}
		void _merge(int u, int v){
			mFa[_getFather(u)] = _getFather(v);
		}
		bool isTreeStruct = true;
	public:
		bool AddEdge(int u, int v, EdgeData d = EdgeDataI()){
			CleanLastError();
			if (u >= 1 && u <= Graph<PointData, EdgeData, PointDataI, EdgeDataI>::GetPointSize() && v >= 1 && v <= Graph<PointData, EdgeData, PointDataI, EdgeDataI>::GetPointSize()){
				if (isTreeStruct){
					if (_getFather(u - 1) != _getFather(v - 1)){
						_merge(u - 1, v - 1);
					} else {
						SetLastError("Bad Edge %d to %d (not a tree).", u, v);
						return false;
					}
				}
				return Graph<PointData, EdgeData, PointDataI, EdgeDataI>::AddEdge(v, u, d) &&
						Graph<PointData, EdgeData, PointDataI, EdgeDataI>::AddEdge(u, v, d);
			}
			SetLastError("Bad Edge %d to %d (for only %d points).", u, v, Graph<PointData, EdgeData, PointDataI, EdgeDataI>::GetPointSize());
			return false;
		}
		bool EraseEdge(int u, int v){
			if (isTreeStruct){
				SetLastError("Unsupport Function");
				return false;
			}
			Graph<PointData, EdgeData, PointDataI, EdgeDataI>::EraseEdge(u, v);
			Graph<PointData, EdgeData, PointDataI, EdgeDataI>::EraseEdge(v, u);
		}
		bool IsTreeStruct(){
			return isTreeStruct;
		}
		bool MarkAsGraph(){
			isTreeStruct = false;
		}
		int AddPoint(PointData p = PointDataI()){
			mFa.push_back(mFa.size());
			Graph<PointData, EdgeData, PointDataI, EdgeDataI>::AddPoint(p);
		}
		bool Union(int u, int v){
			CleanLastError();
			if (!isTreeStruct){
				SetLastError("Only for struct tree!");
				return false;
			}
			if (u >= 1 && u <= Graph<PointData, EdgeData, PointDataI, EdgeDataI>::GetPointSize() && v >= 1 && v <= Graph<PointData, EdgeData, PointDataI, EdgeDataI>::GetPointSize()){
				return _getFather(u) == _getFather(v);
			}
			SetLastError("Bad Edge %d to %d (for only %d points).", u, v, Graph<PointData, EdgeData, PointDataI, EdgeDataI>::GetPointSize());
			return false;
		}
		void SetEdge(int u, int v, EdgeData e){
			if (u >= 1 && u <= Graph<PointData, EdgeData, PointDataI, EdgeDataI>::GetPointSize() && v >= 1 && v <= Graph<PointData, EdgeData, PointDataI, EdgeDataI>::GetPointSize()){
				if (Graph<PointData, EdgeData, PointDataI, EdgeDataI>::P[u].edges.find(Graph<PointData, EdgeData, PointDataI, EdgeDataI>::edge(u,v,e))){
					Graph<PointData, EdgeData, PointDataI, EdgeDataI>::P[u].edges.erase(Graph<PointData, EdgeData, PointDataI, EdgeDataI>::P[u].edges.find(Graph<PointData, EdgeData, PointDataI, EdgeDataI>::edge(u,v,e)));
					Graph<PointData, EdgeData, PointDataI, EdgeDataI>::P[u].edges.erase(Graph<PointData, EdgeData, PointDataI, EdgeDataI>::P[v].edges.find(Graph<PointData, EdgeData, PointDataI, EdgeDataI>::edge(v,u,e)));
					Graph<PointData, EdgeData, PointDataI, EdgeDataI>::AddEdge(u,v,e);
					Graph<PointData, EdgeData, PointDataI, EdgeDataI>::AddEdge(v,u,e);
				}
			}
			SetLastError("Bad Edge %d to %d (for only %d points).", u, v, Graph<PointData, EdgeData, PointDataI, EdgeDataI>::GetPointSize());
			return false;
		}
		class DFS{
		protected:
			std::vector<bool> vst;
		public:
			DFS(size_t size){vst.resize(size);}
			template<class Each, class Func>
			void Traversal(Each eacher, Func func, int cur, int fa = 0, int deep = 0){
				vst[cur] = true;
				func(cur, fa, deep);
				eacher(cur, [&](int next){
					if (!vst[next]) {
						Traversal(eacher, func, next, cur, deep + 1);
					}
				});
			}
		};
		template<class Func, class Type = DFS>
		void Traversal(int S, Func func){
			Type Traversaler(Graph<PointData, EdgeData, PointDataI, EdgeDataI>::P.size());
			Traversaler.Traversal([&](int cur, std::function<void(int)> nxt){
				Graph<PointData, EdgeData, PointDataI, EdgeDataI>::EachEdge(cur, [&](int u, int v){
					nxt(v);
				});
			}, func, S);
		}
	};// class
	template<class T>
	struct is_undirectedTree{ static const bool value = false; };
	template<class A, class B, class C, class D>
	struct is_undirectedTree<UndirectedTree<A,B,C,D> >{
		static const bool value = true;
	};
}; // namespace
#endif

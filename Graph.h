#pragma once
/*
	Project: Tree Diagram
	Author : Yangff
	Date   : 2014-4-20

	TODO   :
				* Support Erase Point :(
*/
#ifndef _TreeDiagram_GRAPH_H_
#define _TreeDiagram_GRAPH_H_
#include "Common.h"
#include <set>
#include <vector>
namespace TreeDiagram{
	template<class PointData = Nothing, class EdgeData = Nothing, class PointDataI = PointData, class EdgeDataI = EdgeData>
	class Graph {
	protected:
		struct edge {
			int u, v; EdgeData edge_data;
			bool operator < (edge x) const{
				return v < x.v;
			}
			edge(int u = 0, int v = 0, EdgeData edge_data = EdgeDataI()):u(u),v(v),edge_data(edge_data){;}
		};
		struct point {
			int p; std::set<edge> edges;
			PointData point_data;
			point(int p = 0, PointData point_data = PointDataI(), std::set<edge> edges = std::set<edge>()):p(p), edges(edges), point_data(point_data){};
		};
		std::vector<point> P;
		int edge_cnt;
	public:

		typedef PointData PointDataType;
		typedef EdgeData EdgeDataType;

		int GetPointSize() {
			return P.size();
		}
		bool AddEdge(int u, int v, EdgeData d = EdgeDataI()){
			CleanLastError();
			if (u >= 1 && u <= GetPointSize() && v >= 1 && v <= GetPointSize()){
				edge e(u,v,d);
				if (P[u - 1].edges.find(e) != P[u - 1].edges.end()){
					SetLastError("Already have Edge %d to %d.", u, v);
					return false;
				}
				P[u - 1].edges.insert(e);
				edge_cnt++;
				return true;
			}
			SetLastError("Bad Edge %d to %d (for only %d points).", u, v, GetPointSize());
			return false;
		}
		bool EraseEdge(int u, int v){
			CleanLastError();
			if (u >= 1 && u <= GetPointSize() && v >= 1 && v <= GetPointSize()){
				edge e(u, v);
				if (P[u - 1].edges.find(e) == P[u - 1].edges.end()){
					SetLastError("Not found Edge %d to %d.", u, v);
					return false;
				}
				P[u - 1].edges.erase(e);
				edge_cnt--;
				return true;
			}
			SetLastError("Bad Edge %d to %d (for only %d points).", u, v, GetPointSize());
			return false;
		}

		EdgeData GetEdge(int u, int v){
			CleanLastError();
			if (u >= 1 && u <= GetPointSize() && v >= 1 && v <= GetPointSize()){
				edge e(u, v);
				if (P[u - 1].edges.find(e) == P[u - 1].edges.end()){
					SetLastError("Not found Edge %d to %d.", u, v);
					return false;
				}
				return P[u - 1].edges.find(e)->edge_data;
			}
			SetLastError("Bad Edge %d to %d (for only %d points).", u, v, GetPointSize());
			return EdgeDataI();
		}

		int AddPoint(PointData p = PointDataI()){
			P.push_back(point(GetPointSize() + 1, PointData(p)));
			return GetPointSize();
		}

		PointData GetPoint(int p){
			CleanLastError();
			if (p >= 1 && p <= GetPointSize())
				return P[p - 1].point_data;
			SetLastError("Bad Point %d (for only %d points).", p, GetPointSize());
		}

		template<class Func>
		auto EachPoint(Func func)
		-> typename std::enable_if<CheckArgc::function_argc<Func>::value == 1, void>::type
		{
			for (int i = 0; i < int(P.size()); i++)
				func(i + 1);
		}

		template<class Func>
		auto EachPoint(Func func)
		-> typename std::enable_if<CheckArgc::function_argc<Func>::value == 2, void>::type
		{
			for (int i = 0; i < int(P.size()); i++)
				func(i + 1, P[i].point_data);
		}

		template<class Func>
		auto EachEdge(Func func)
		-> typename std::enable_if<CheckArgc::function_argc<Func>::value == 1, void>::type
		{
			for (int i = 0; i < int(P.size()); i++)
				for (auto it = P[i].edges.begin(); it != P[i].edges.end(); it++){
					func(it->edge_data);
				}
		}

		template<class Func>
		auto EachEdge(Func func)
		-> typename std::enable_if<CheckArgc::function_argc<Func>::value == 2, void>::type
		{
			for (int i = 0; i < int(P.size()); i++)
				for (auto it = P[i].edges.begin(); it != P[i].edges.end(); it++){
					func(it->u, it->v);
				}
		}

		template<class Func>
		auto EachEdge(Func func)
		-> typename std::enable_if<CheckArgc::function_argc<Func>::value == 3, void>::type
		{
			for (int i = 0; i < int(P.size()); i++)
				for (auto it = P[i].edges.begin(); it != P[i].edges.end(); it++){
					func(it->u, it->v, it->edge_data);
				}
		}


		template<class Func>
		auto EachEdge(int i, Func func)
		-> typename std::enable_if<CheckArgc::function_argc<Func>::value == 1, void>::type
		{
			CleanLastError();
			if (i >= 1 && i <= GetPointSize()){
				i--;
				for (auto it = P[i].edges.begin(); it != P[i].edges.end(); it++){
					func(it->edge_data);
				}
				return ;
			}
			SetLastError("Bad Point %d (for only %d points).", i, GetPointSize());
		}

		template<class Func>
		auto EachEdge(int i, Func func)
		-> typename std::enable_if<CheckArgc::function_argc<Func>::value == 2, void>::type
		{
			CleanLastError();
			if (i >= 1 && i <= GetPointSize()){
				i--;
				for (auto it = P[i].edges.begin(); it != P[i].edges.end(); it++){
					func(it->u, it->v);
				}
				return ;
			}
			SetLastError("Bad Point %d (for only %d points).", i, GetPointSize());
		}

		template<class Func>
		auto EachEdge(int i, Func func)
		-> typename std::enable_if<CheckArgc::function_argc<Func>::value == 3, void>::type
		{
			CleanLastError();
			if (i >= 1 && i <= GetPointSize()){
				i--;
				for (auto it = P[i].edges.begin(); it != P[i].edges.end(); it++){
					func(it->u, it->v, it->edge_data);
				}
				return ;
			}
			SetLastError("Bad Point %d (for only %d points).", i, GetPointSize());
		}

	}; // class Graph

	template<class T>
	struct is_graph{ static const bool value = false; };
	template<class A, class B, class C, class D>
	struct is_graph<Graph<A,B,C,D> >{
		static const bool value = true;
	};

}; // namespace
#endif

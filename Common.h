#pragma once
#ifndef _TreeDiagram_COMMON_H_
#define _TreeDiagram_COMMON_H_
#define _DEBUG 1
/*
	Project: Tree Diagram
	Author : Yangff
	Date   : 2014-4-20
*/
#include <string>
#include <cstdio>
#include <type_traits>
#include <cstdarg>
#include <functional>

namespace TreeDiagram{
	struct Nothing { int unused; Nothing(int x = 0){unused = 0;}};
#ifdef _DEBUG
	std::string LastError;
	bool ErrorHappened;
	void SetLastError(const char *fmt, ...){
		va_list args;
		va_start(args, fmt);
		int n = vsnprintf(NULL, 0, fmt, args);
		//~ printf("%d\n", n);
		char * ch = new char[n + 1];
		va_end(args);
		va_start(args, fmt);
		vsnprintf(ch, n, fmt, args);
		LastError = ch;
		va_end(args);
		delete [] ch;
		ErrorHappened = true;
	}

	void CleanLastError(){
		LastError = "";
		ErrorHappened = false;
	}

	std::string GetLastError(){
		return LastError;
	}

	bool CheckError(){return ErrorHappened;}
#else
	void SetLastError(std::string Error){}
	void CleanLastError(){}
	std::string GetLastError(){return "";}
	bool CheckError(){return false;}
#endif
	namespace CheckArgc{
		template<class F>
		struct deduce_function{};

		template<class Ret, class C, class... Args>
		struct deduce_function<Ret (C::*)(Args...) const>
		{
			static const int value = sizeof...(Args);
		};

		template<class Ret, class C, class... Args>
		struct deduce_function<Ret (C::*)(Args...)>
		{
			static const int value = sizeof...(Args);
		};

		template<class Ret, class... Args>
		struct deduce_function<Ret (*)(Args...)>
		{
			static const int value = sizeof...(Args);
		};


		template<class F, bool f>
		struct _inline_function_argc;

		template<class F>
		struct _inline_function_argc<F, true>
		{
			static const int value = deduce_function<decltype(&std::remove_reference<F>::type::operator())>::value;
		};

		template<class F>
		struct _inline_function_argc<F, false>
		{
			static const int value = deduce_function<F >::value;
		};

		template<class F>
		struct function_argc : _inline_function_argc<F, std::is_class<F>::value >
		{};

	};
	template<class Func>
	auto Times(int x, Func func)
	-> typename std::enable_if<CheckArgc::function_argc<Func>::value == 0, void>::type
	{
		for (int i = 0; i < x; i++)
			func();
	}
	template<class Func>
	auto Times(int x, Func func)
	-> typename std::enable_if<CheckArgc::function_argc<Func>::value == 1, void>::type
	{
		for (int i = 0; i < x; i++)
			func(i);
	}

	struct Int{
		int x;
		#define Ints(xx) xx(Int y):Int(y.x){;}
		int& operator () (){return x;}
		int operator () () const{return x;}
		Int(int x=0):x(x){;}
		#define MakeOperator1(op) \
		Int operator op (Int y){  \
			Int r; 					\
			r.x = x op y.x; 			\
			return r;  				\
		};                        \
		Int operator op (int y){  \
			Int r; 					\
			r.x = x op y; 			\
			return r;  				\
		};

		MakeOperator1(+);
		MakeOperator1(-);
		MakeOperator1(*);
		MakeOperator1(/);
		MakeOperator1(^);
		MakeOperator1(%);
		MakeOperator1(&);
		MakeOperator1(|);

		bool operator == (Int y) const{
			return x == y.x;
		}

		bool operator < (Int y) const{
			return x < y.x;
		}

		bool operator > (Int y) const{
			return x > y.x;
		}

		bool operator == (int y) const{
			return x == y;
		}

		bool operator < (int y) const{
			return x < y;
		}

		bool operator > (int y) const{
			return x > y;
		}

		void operator = (int y){
			x = y;
		}

	};

}; // namespace
#endif

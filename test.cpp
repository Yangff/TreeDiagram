	namespace CheckFunctionArgs{
		template<class Ret, int need, class... _Elements>
		struct deduce_function;
		template<class Ret, int need>
		struct deduce_function<Ret (), need>{
			static const bool invoke = need == 0;
		};

		template<class Ret, int need, class Head, class... Tail>
		struct deduce_function<Ret (Head, Tail...), need> : deduce_function<Ret (Tail...), need - 1>{
		};
	};
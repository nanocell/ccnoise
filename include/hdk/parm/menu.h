
#ifndef __PARM_MENU_H__
#define __PARM_MENU_H__

namespace cchdk
{

	/****************************************************************************************************/
	
	template<std::size_t N>
	struct Menu
	{
		static const std::size_t elements = N;
		typedef Menu<N> type;

		Menu() : choice_list(PRM_CHOICELIST_SINGLE, (PRM_Name*)0) {}

		PRM_ChoiceList* get() { rebuild(); return &(this->choice_list); }

		PRM_Name& operator[](std::size_t i) { return menu_values[i]; }

		void rebuild() 
		{
			for (unsigned int i = 0; i < N; ++i)
			{
				this->menu_values[i] = PRM_Name( this->names[i].c_str(), this->labels[i].c_str() );
			}

			this->choice_list = PRM_ChoiceList(PRM_CHOICELIST_SINGLE, &this->menu_values[0]);
		}
		
		boost::array<std::string, N> names;
		boost::array<std::string, N> labels;
		boost::array<PRM_Name, N+1> menu_values;
		PRM_ChoiceList choice_list;
	};

	/****************************************************************************************************/

	template<>
	struct Menu<0> 
	{
		static const std::size_t elements = 0;
		typedef Menu<0> type;
		
		PRM_ChoiceList* get() { return 0; }
	};

	/****************************************************************************************************/

	template<std::size_t N = 0>
	struct NoMenuT : Menu<0> {};

	typedef NoMenuT<0> NoMenu;

	/****************************************************************************************************/

	//Generate menu type definitions, and MakeMenu functions:
	// typedef Menu<0> Menu0;
	// typedef Menu<1> Menu1;
	// typedef Menu<2> Menu2;
	// ...etc.
	//
	// Menu0 MakeMenu() { Menu0 m; return m; }
	// Menu1 MakeMenu(std::string name0, std::string label0) { Menu1 m; m.menu_values[0] = PRM_Name(name0, label0); }
	// Menu2 MakeMenu(std::string name0, std::string label0, std::string name1, std::string label1) { Menu2 m; m.menu_values[0] = PRM_Name(name0, label0);  m.menu_values[1] = PRM_Name(name1, label1); m.menu_values[2] = PRM_Name(name2, label2); return m; }
	// ...etc.

#include <boost/preprocessor/repetition.hpp>
#include <boost/preprocessor/arithmetic/sub.hpp>
#include <boost/preprocessor/punctuation/comma_if.hpp>
#include <boost/preprocessor/iteration/local.hpp>

#ifndef MAX_MENU_SIZE
	#define MAX_MENU_SIZE 15
#endif

#define MENU_PARM(z, n, unused) std::string name ## n, std::string label ## n

#define MENU_ASSIGN(z, n, unused) m.names[n] = name ## n; m.labels[n] = label ## n; 

#define MAKE_Menu(z, n, unused)  \
typedef Menu<n> Menu ## n; \
Menu<n> MakeMenu(BOOST_PP_ENUM(n,MENU_PARM,~))  \
{  \
	Menu<n> m;  \
	BOOST_PP_REPEAT(n,MENU_ASSIGN,~) \
	BOOST_PP_IF(n, m.menu_values[n] = PRM_Name(0);, ; ) \
	return m; \
}

//Vertical repetition of the MAKE_Menu macro
#define BOOST_PP_LOCAL_MACRO(n) MAKE_Menu(~, n, ~)
#define BOOST_PP_LOCAL_LIMITS (0, MAX_MENU_SIZE - 1)
#include BOOST_PP_LOCAL_ITERATE()

#undef MAKE_Menu
#undef MENU_PARM
#undef MENU_ASSIGN

} // ns ccnoise

#endif


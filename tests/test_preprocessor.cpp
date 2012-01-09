
//#include <sysexits.h>


#include <boost/preprocessor/repetition/enum_params.hpp>

#include <boost/preprocessor/repetition.hpp>
#include <boost/preprocessor/arithmetic/sub.hpp>
#include <boost/preprocessor/punctuation/comma_if.hpp>
#include <boost/preprocessor/iteration/local.hpp>

#ifndef MAX_MENU_SIZE
	#define MAX_MENU_SIZE 4
#endif

#define MENU_PARM(z, n, unused) std::string name ## n, std::string label ## n

#define MENU_ASSIGN(z, n, unused) m.menu_values[n] = PRM_Name(name ## n.c_str(), label ## n.c_str()); 

#define MAKE_Menu(z, n, unused)  \
typedef Menu<n> Menu ## n; \
Menu<n> MakeMenu(BOOST_PP_ENUM(n,MENU_PARM,~))  \
{  \
	Menu<n> m;  \
	BOOST_PP_REPEAT(n,MENU_ASSIGN,~) \
	MENU_ASSIGN(z, n, ~) \
	BOOST_PP_IF(n, m.choice_list = PRM_ChoiceList(PRM_CHOICELIST_SINGLE, &(m.menu_values[0]));, ; ) \
	return m; \
}

//Vertical repetition of the MAKE_Menu macro
#define BOOST_PP_LOCAL_MACRO(n) MAKE_Menu(~, n, ~)
#define BOOST_PP_LOCAL_LIMITS (0, MAX_MENU_SIZE - 1)
#include BOOST_PP_LOCAL_ITERATE()

#undef MAKE_Menu
#undef MENU_PARM
#undef MENU_ASSIGN

int main(int argc, char* argv[])
{

	return EX_OK;
}

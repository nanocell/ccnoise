
#ifndef __PARMTOOLS_H__
#define __PARMTOOLS_H__

#include "hdk/parm/defaults.h"
#include "hdk/parm/range.h"
#include "hdk/parm/menu.h"
namespace cchdk
{

	/****************************************************************************************************/

	template<std::size_t N, class DefaultsT=NoDefaults, class MenuT=NoMenu, class RangeT=NoRange>
	struct Parm
	{
		static const std::size_t elements = N;
		Parm(PRM_Type prm_type, std::string aname, std::string alabel, DefaultsT adefaults=DefaultsT(), MenuT amenu=MenuT(), RangeT arange=RangeT()) :
			name(aname), label(alabel),
			prm_name(aname.c_str(), alabel.c_str()), 
			prm_defaults(adefaults),
			prm_menu(amenu),
			prm_range(arange)
		{
			BOOST_STATIC_ASSERT(( DefaultsT::elements == N || DefaultsT::elements == 0 ) );
			tpl = PRM_Template(prm_type, this->elements, &(this->prm_name), this->prm_defaults.get(), this->prm_menu.get(), this->prm_range.get() );
		}

		PRM_Template get() { return tpl; }

		std::string name;
		std::string label;
		
		PRM_Name prm_name;
		DefaultsT prm_defaults;
		RangeT prm_range;
		MenuT prm_menu;

		PRM_Template tpl;
	};

}

#endif


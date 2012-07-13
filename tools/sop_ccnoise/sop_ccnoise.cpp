/*
 * Copyright (c) 2012 Van Aarde Krynauw
 */

//This is a SOP that generates noise using the ccnoise library

#include <limits.h>
#include <UT/UT_DSOVersion.h>
#include <UT/UT_Math.h>
#include <UT/UT_Interrupt.h>
#include <GU/GU_Detail.h>
#include <GU/GU_PrimPoly.h>
#include <CH/CH_LocalVariable.h>
#include <PRM/PRM_Include.h>
#include <OP/OP_Operator.h>
#include <OP/OP_OperatorTable.h>

#include <boost/array.hpp>
#include <boost/tuple/tuple.hpp>
#include <hdk/parm/parm.h>
#include <vector>

#include <ccnoise/value.h>
#include <ccnoise/gradient_utils.h>

#include <ccmath/traits/imath_type_traits.h>

#include "sop_ccnoise.h"

/****************************************************************************************************/


//
// Help is stored in a "wiki" style text file.  This text file should be copied
// to $HOUDINI_PATH/help/nodes/sop/ccnoise.txt
//
// See the sample_install.sh file for an example.
//


void newSopOperator(OP_OperatorTable *table)
{
	table->addOperator( new OP_Operator(
				"ccnoise",			// Internal name
				"CCNoise",			// UI name
				cchdk::SOP_CCNoise::myConstructor,	// constructor
				cchdk::SOP_CCNoise::myTemplateList,	// ui parameters
				1,				// Min # of sources
				1,				// Max # of sources
				cchdk::SOP_CCNoise::myVariables,	// Local variables
				0) );
}

namespace cchdk
{

static Parm<1, Defaults1> PrmNoiseDimension(PRM_ORD, "dim", "Noise Dimension", MakeDefaults(1.f));
static Parm<3, Defaults3> PrmFrequency(PRM_XYZ, "freq", "Frequency", MakeDefaults(1.f, 1.f, 1.f));
static Parm<3, Defaults3> PrmOffset(PRM_XYZ, "offs", "Offset", MakeDefaults(0.f, 0.f, 0.f));
static Parm<1, Defaults1> PrmTimeOffset(PRM_FLT, "timeoffs", "Time Offset", MakeDefaults(0));
static Parm<1, Defaults1, NoMenu, Range> PrmAmplitude(PRM_FLT, "amp", "Amplitude", MakeDefaults(1.f), NoMenu(), Range(0.f, 10.f));

static Menu3 noisetypes_menu = MakeMenu("value", "Value", "gradient", "Gradient (Perlin)", "vgradient", "Value-Gradient");
static Parm<1, Defaults1, Menu3> PrmNoise(PRM_ORD, "noise", "Noise", MakeDefaults(1), noisetypes_menu);

static Menu4 attributes_menu = MakeMenu("position", "Position", "color", "Color", "normals", "Normals", "velocity", "Velocity");
static Parm<1, Defaults1, Menu4> PrmApplyTo(PRM_ORD, "applyto", "Apply To", MakeDefaults(0), attributes_menu);

static Menu2 displacement_menu = MakeMenu("along_normal", "Along Normal", "position", "Position"); 
static Parm<1, Defaults1, Menu2> PrmDisplacement(PRM_ORD, "displacement", "Displacement", MakeDefaults(0), displacement_menu);

static Parm<1, Defaults1> PrmComputeNormals(PRM_TOGGLE, "compute_normals", "Compute Normals", MakeDefaults(1.f));

PRM_Template SOP_CCNoise::myTemplateList[] = {
		PrmNoise.get(),
		PrmApplyTo.get(),
		PrmNoiseDimension.get(),
		PrmFrequency.get(),
		PrmOffset.get(),
		PrmTimeOffset.get(),
		PrmAmplitude.get(),
		PRM_Template(PRM_SEPARATOR),
		PrmDisplacement.get(),
		PrmComputeNormals.get(),
    PRM_Template()
};

struct attribute_pos_tag {};
struct attribute_color_tag {};

//Tags for different methods to apply a noise value to the outgoing attribute
struct apply_displace_along_vector_tag {}; 
struct apply_displace_pos_tag {};
struct apply_set_attribute_tag {};

struct noise_input_3d_tag{};
struct noise_input_4d_tag{};
struct noise_output_1d_tag{};
struct noise_output_3d_tag{};

/****************************************************************************************************/
template<>
////inline void SOP_CCNoise::applyNoiseToResult<apply_displace_along_vector_tag>(UT_Vector4& pos, GEO_AttributeHandle& attr, GEO_AttributeHandle& attr_vec, float noise_value)
inline void SOP_CCNoise::applyNoiseToResult<apply_displace_along_vector_tag>(GA_Offset ptoffs, GA_ROAttributeRef attr, GA_ROAttributeRef attr_vec, float noise_value)
{
	//Apply displacement along vector
	//pos += noise_value * UT_Vector4(attr_vec.getV3());
	//gdp->setPos(ptoffs,  gdp->getPos3(ptoffs) + noise_value * attr_vec.getV3());
}

/****************************************************************************************************/
template<>
inline void SOP_CCNoise::applyNoiseToResult<apply_displace_pos_tag>(GA_Offset ptoffs, GA_ROAttributeRef attr, GA_ROAttributeRef attr_vec, float noise_value)
{
	//Displace the position with a 3D noise
	//pos += noise_value * UT_Vector4(attr_vec.getV3());
}

/****************************************************************************************************/

template<>
inline void SOP_CCNoise::applyNoiseToResult<apply_set_attribute_tag>(GA_Offset pos, GA_RWAttributeRef attr, GA_RWAttributeRef attr_aux, float noise_value)
{
	//Set the given attribute directly from the noise value
	attr.setV3(UT_Vector3(noise_value, noise_value, noise_value));
}

/****************************************************************************************************/

template<class NoiseT>
void SOP_CCNoise::applyPositionDisplacementAlongVector(GA_ROAttributeRef attr, std::string vector_name, fpreal t)
{
	//Apply displacement along the normal
	//GEO_AttributeHandle nml = gdp->getPointAttribute( vector_name.c_str() );
	GA_ROAttributeRef nml = gdp->findFloatTuple(GA_ATTRIB_POINT, vector_name.c_str(), 3);

	applyNoise<NoiseT, apply_displace_along_vector_tag, true>(attr, nml, t);
}

/****************************************************************************************************/

template<class NoiseT>
void SOP_CCNoise::applyPositionDisplacementOnPosition(GA_ROAttributeRef attr, fpreal t)
{
	//Perform a displacement on the position
	//GEO_AttributeHandle unused;

	//applyNoise<NoiseT, apply_displace_pos_tag, false>(attr, unused, t);
}

/****************************************************************************************************/

template<class NoiseT>
void SOP_CCNoise::applyNoiseOnPosition(GA_ROAttributeRef attr, fpreal t)
{
	//Apply noise on position
	
	int displacement_type = evalInt(PrmDisplacement.name.c_str(), 0, t);

	switch (displacement_type)
	{
		case 0: // displacement along normal (this will invoke a noise with a 1D result)
			std::cout << "Displacing along normal..." << std::endl;
			applyPositionDisplacementAlongVector<NoiseT>(attr, "N", t);
			break;
		case 1: // displace position (this will invoke a noise function with a 3D result
			std::cout << "Displacing position..." << std::endl;
			applyPositionDisplacementOnPosition<NoiseT>(attr, t);
			break;
		default:
			break;
	};

}

/****************************************************************************************************/

template<class NoiseT>
void SOP_CCNoise::applyNoiseOnColor(GEO_AttributeHandle& attr, fpreal t)
{
	//Apply noise on color
	GEO_AttributeHandle unused;
	applyNoise<NoiseT, apply_set_attribute_tag, false>(attr, unused, t);
}

/****************************************************************************************************/

template<class NoiseT>
void SOP_CCNoise::applyNoiseFunction(fpreal t)
{
	//Evaluate the attribute on which noise values should be applied.
	int attribute_type = evalInt(PrmApplyTo.name.c_str(), 0, t);

	GEO_AttributeHandle attr;
	switch (attribute_type)
	{
		case 0:
			attr = gdp->getPointAttribute("P");

			applyNoiseOnPosition<NoiseT>(attr, t);
			break;
		case 1:
			attr = gdp->getPointAttribute("Cd");
			if (!attr.isAttributeValid())
			{
				float default_color[3] = {1.0f, 1.0f, 1.0f};
				//gdp->addPointAttrib("Cd", sizeof(float)*3, GB_ATTRIB_FLOAT, default_color);
				gdp->addFloatTuple(GA_ATTRIB_POINT, "Cd", 3);
				attr = gdp->getPointAttribute("Cd");
			}
	
			applyNoiseOnColor<NoiseT>(attr, t);
			break;
		default:
			break;
	}
}

/****************************************************************************************************/

// Here's how we define local variables for the SOP.
enum {
	VAR_PT,		// Point number of the star
	VAR_NPT		// Number of points in the star
};

CH_LocalVariable
SOP_CCNoise::myVariables[] = {
    { "PT",	VAR_PT, 0 },		// The table provides a mapping
    { "NPT",	VAR_NPT, 0 },		// from text string to integer token
    { 0, 0, 0 },
};

float SOP_CCNoise::getVariableValue(int index, int thread)
{
	return SOP_Node::getVariableValue(index, thread);

	fpreal val;
	// myCurrPoint will be negative when we're not cooking so only try to
	// handle the local variables when we have a valid myCurrPoint index.
	if (myCurrPoint >= 0)
	{
		// Note that "gdp" may be null here, so we do the safe thing
		// and cache values we are interested in.
		switch (index)
		{
			case VAR_PT:
				return (fpreal) myCurrPoint;
				break;
			case VAR_NPT:
				return (fpreal) myTotalPoints;
				break;
			default:
			/* do nothing */;
		}
	}
	// Not one of our variables, must delegate to the base class.
	return SOP_Node::getVariableValue(index, thread);
}

OP_Node *
SOP_CCNoise::myConstructor(OP_Network *net, const char *name, OP_Operator *op)
{
    return new SOP_CCNoise(net, name, op);
}

SOP_CCNoise::SOP_CCNoise(OP_Network *net, const char *name, OP_Operator *op)
	: SOP_Node(net, name, op)
{
    myCurrPoint = -1;	// To prevent garbage values from being returned
}

SOP_CCNoise::~SOP_CCNoise() {}

/****************************************************************************************************/

unsigned SOP_CCNoise::disableParms()
{
    return 0;
}

/****************************************************************************************************/

OP_ERROR SOP_CCNoise::cookMySop(OP_Context &context)
{

	if (lockInputs(context) >= UT_ERROR_ABORT)
	{
		return error();
	}
	
	fpreal t = context.getTime();
	UT_Interrupt *boss;

	boss = UTgetInterrupt();
	duplicatePointSource(0, context);

	int noise_function = evalInt(PrmNoise.name.c_str(), 0, t);

	switch (noise_function)
	{
		case 0: //value noise
			//Iterate over all the points and apply a noise to the given attribute
			applyNoiseFunction< ccnoise::value<float> >(t);
			break;
		case 1: //gradient noise
			//applyNoiseFunction< ccnoise::gradient<float> >(t);
			break;
		default:
			break;
	}

	if ( evalInt(PrmComputeNormals.name.c_str(), 0, t) )
		gdp->normal(); //recompute point normals

	boss->opEnd();

	unlockInputs();

  gdp->notifyCache(GU_CACHE_ALL);

  return error();
}

/****************************************************************************************************/

template<class NoiseT, class ApplyT, bool UseAuxAttr>
//void SOP_CCNoise::applyNoise(GEO_AttributeHandle& attr, GEO_AttributeHandle& attr_aux, fpreal time)
//Apply noise on 'attr'
void SOP_CCNoise::applyNoise(GA_ROAttributeRef attr, GA_ROAttributeRef& attr_aux, fpreal time)
{
	UT_Vector3T<fpreal> freq;
	UT_Vector3T<fpreal> offs;
	float amp;
	float time_offs;
	GEO_Point* ppt;

	evalVectorProperty(PrmFrequency.name.c_str(), time, &freq[0], PrmFrequency.elements);
	evalVectorProperty(PrmOffset.name.c_str(), time, &offs[0], PrmOffset.elements);
	amp = evalFloat(PrmAmplitude.name.c_str(), 0, time);	
	time_offs = evalFloat(PrmTimeOffset.name.c_str(), 0, time);	

	//NOTE: The noise initialisation should be moved to the node constructor
	NoiseT noise;
	noise.init();

	float f; // noise output

	//Iterate over all points using GA_Offsets
	GA_Iterator it;
	GA_Offset block_start, block_end;
	GA_Offset pt_offs;

	for (it = gdp->getPointRange(); it.blockAdvance(block_start, block_end); )
	{
		for (pt_offs = block_start; pt_offs < block_end; ++pt_offs)
		{
			//Currently, we always treat the Position as the source for the noise
			UT_Vector3 pos = gdp->getPos3(); // noise input
			
			//Apply noise to color, based on position
			noise.get(f, pos[0] * freq[0] + offs[0], pos[1] * freq[1] + offs[1], pos[2] * freq[2] + offs[2], time_offs);
			f *= amp;
			
			attr.setElement(ppt);
			if (UseAuxAttr)
			{
				attr_aux.setElement(ppt);
			}

			//applyNoiseToResult<ApplyT>(pos, attr, attr_aux, f);
		}
	}
	
//	GEO_PointList ptlist = gdp->points();
//
//	for (ppt = ptlist.head(); ppt; ppt = ptlist.next(ppt))
//	{
//		//Currently, we always treat the Position as the source for the noise
//		UT_Vector4 pos = ppt->getPos(); // noise input
//		
//		//Apply noise to color, based on position
//		noise.get(f, pos[0] * freq[0] + offs[0], pos[1] * freq[1] + offs[1], pos[2] * freq[2] + offs[2], time_offs);
//		f *= amp;
//		
//		attr.setElement(ppt);
//		if (UseAuxAttr)
//		{
//			attr_aux.setElement(ppt);
//		}
//
//		applyNoiseToResult<ApplyT>(pos, attr, attr_aux, f);
//	}
}

/****************************************************************************************************/

} // ns cchdk


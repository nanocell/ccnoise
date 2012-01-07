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

#include <ccnoise/value_noise.h>
#include <ccnoise/gradient_noise.h>

#include <ccmath/traits/imath_type_traits.h>

#include "sop_ccnoise.h"

using namespace hdk_ccnoise;
using namespace cchdk;

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
				SOP_CCNoise::myConstructor,	// constructor
				SOP_CCNoise::myTemplateList,	// ui parameters
				1,				// Min # of sources
				1,				// Max # of sources
				SOP_CCNoise::myVariables,	// Local variables
				0) );
}

static Parm<1, Defaults1> PrmNoiseDimension(PRM_ORD, "dim", "Noise Dimension", MakeDefaults(1.f));
static Parm<3, Defaults3> PrmFrequency(PRM_XYZ, "freq", "Frequency", MakeDefaults(1.f, 1.f, 1.f));
static Parm<3, Defaults3> PrmOffset(PRM_XYZ, "offs", "Offset", MakeDefaults(0.f, 0.f, 0.f));
static Parm<1, Defaults1, Range> PrmAmplitude(PRM_FLT, "amp", "Amplitude", MakeDefaults(0.f), Range(0.f, 10.f));

PRM_Template SOP_CCNoise::myTemplateList[] = {
		PrmNoiseDimension.get(),
		PrmFrequency.get(),
		PrmOffset.get(),
		PrmAmplitude.get(),
    PRM_Template()
};


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

fpreal SOP_CCNoise::getVariableValue(int index, int thread)
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

	GEO_AttributeHandle attr;

	int dim = evalInt(PrmNoiseDimension.name.c_str(), 0, t);	

	attr = gdp->getPointAttribute("Cd");

	if (attr.isAttributeValid())
	{
		//Iterate over all the points and apply a noise to the given attribute
		//applyNoise< ccnoise::value_noise<float> >(attr, t);
	}
	
	attr = gdp->getPointAttribute("Cd");

	if (attr.isAttributeValid())
	{
		switch (dim)
		{
			case 0:
				//Iterate over all the points and apply a noise to the given attribute
				applyNoise< ccnoise::gradient<float> >(attr, t);
				break;
			case 1:
				applyNoise< ccnoise::value<float> >(attr, t);
				break;
			default:
				break;
		}
	}


	boss->opEnd();

	unlockInputs();

  gdp->notifyCache(GU_CACHE_ALL);

  return error();
}

/****************************************************************************************************/

template<class NoiseT>
void SOP_CCNoise::applyNoise(GEO_AttributeHandle& attr, fpreal time)
{
	UT_Vector3 freq;
	UT_Vector3 offs;
	float amp;
	GEO_Point* ppt;

	evalVectorProperty(PrmFrequency.name.c_str(), time, &freq[0], PrmFrequency.elements);
	evalVectorProperty(PrmOffset.name.c_str(), time, &offs[0], PrmOffset.elements);
	amp = evalFloat(PrmAmplitude.name.c_str(), 0, time);	

	NoiseT noise;
	noise.init();

	float f; // noise output
	
	GEO_PointList& ptlist = gdp->points();

	for (ppt = ptlist.head(); ppt; ppt = ptlist.next(ppt))
	{
		UT_Vector4& pos = ppt->getPos(); // noise input
		
		attr.setElement(ppt);
		//Apply noise to color, based on position
		noise.get(f, pos[0] * freq[0] + offs[0], pos[1] * freq[1] + offs[1], pos[2] * freq[2] + offs[2]);
		f *= amp;
		attr.setV3(UT_Vector3(f,f,f));
	}
}

/****************************************************************************************************/



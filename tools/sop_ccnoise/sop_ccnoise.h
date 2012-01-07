/*
 * Copyright (c) 2012 Van Aarde Krynauw
 */

//This is a SOP that generates noise using the ccnoise library

#ifndef __SOP_CCNOISE_H__
#define __SOP_CCNOISE_H__

#include <SOP/SOP_Node.h>

namespace hdk_ccnoise
{
	class SOP_CCNoise : public SOP_Node
	{
		public:
			static OP_Node	*myConstructor(OP_Network*, const char *,	OP_Operator *);

			/// Stores the description of the interface of the SOP in Houdini.
			/// Each parm template refers to a parameter.
			static PRM_Template		 myTemplateList[];

			/// This optional data stores the list of local variables.
			static CH_LocalVariable	 myVariables[];

		protected:
			SOP_CCNoise(OP_Network *net, const char *name, OP_Operator *op);
			virtual ~SOP_CCNoise();

			/// Disable parameters according to other parameters.
			virtual unsigned	disableParms();


			/// cookMySop does the actual work of the SOP computing, in this
			/// case, a star shape.
			virtual OP_ERROR		 cookMySop(OP_Context &context);

			/// This function is used to lookup local variables that you have
			/// defined specific to your SOP.
			//
			fpreal getVariableValue(int index, int thread);
			//virtual bool evalVariableValue(fpreal &val, int index, int thread);

			// Add virtual overload that delegates to the super class to avoid
			// shadow warnings.
			//virtual bool	evalVariableValue(UT_String &v, int i, int thread)
			//	{
			//		return evalVariableValue(v, i, thread);
			//	}
		
			//Apply noise to an attribute
			template<class NoiseT>
			void applyNoise(GEO_AttributeHandle& attr, fpreal time);

		private:

			/// Member variables are stored in the actual SOP, not with the geometry
			/// In this case these are just used to transfer data to the local 
			/// variable callback.
			/// Another use for local data is a cache to store expensive calculations.
			int		myCurrPoint;
			int		myTotalPoints;
	};
} // End HDK_Sample namespace

#endif

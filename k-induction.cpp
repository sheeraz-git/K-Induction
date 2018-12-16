
#include "k-induction.h"

int main()
{
//---------------------------------------------------------------------------------------------------------------------------
//------------------------------- CONVENTIONS--------------------------------------------------------------------------
//-------| means lor-------& means land------^ means lxor----% means limplies---~ means lnot---------------------------
//---------------------------------------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------------------------------------
//-------------------------------Three bit Ring Counter------------------------------------------------------------------------------
//	std::string IFText = "((((x0)=(f))&((x1)=(f))&((x2)=(t))))";

//	std::string TFText = "((((y0)=(x1))&((y1)=(x2))&((y2)=(x0))))";

//	std::string PFText = "((((x0)^(x1))^(x2))&(~(((x0)&(x1))&(x2))))";
//---------------------------------------------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------------
//Now create an object of the type k_induction to start the k-induction algorithm.
    k_induction KInduction;
//Call the root method that does the rest.
    bool result=KInduction.init();
//--------------------------------------------------------------------------------------------------------------  	
	return 0;
}
//-----------------------------------------------------------------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------------------------------------------------------------

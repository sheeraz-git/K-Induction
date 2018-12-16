#include "k-induction.h"
//----------------------------------------------------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------------------------------------------------------------
bool k_induction::init()
{
    std::cout << "\n\nEnter the no. of state variables:->";
    std::cin >> N;
    std::cout << "\n\n";    


//-------------------------------------------------------------------------------------------------------------
	// loop for creating the current state variables [x0, x1, x2]
	for(unsigned i=0; i<N; ++i){
		std::stringstream xName;
		xName << "x" << i;
		x.push_back(c.bool_const(xName.str().c_str()));
	}
	// loop for creating the next state variables [y0, y1, y2]
	for(unsigned i=0; i<N; ++i){
		std::stringstream yName;
		yName << "y" << i;
		y.push_back(c.bool_const(yName.str().c_str()));
	}

    std::cout << x << "\n\n";
    std::cout << y << "\n\n";

 	IF=(x[0] && !x[1] && !x[2]); // initial state formula

    std::cout << "I is :" << IF << "\n\n";

	TF=(y[0]==x[2] && y[1]==x[0] && y[2]==x[1]); // transition relation formula

    std::cout << "T is :" << TF << "\n\n";

//    PF= ite(x[0],x[1],x[2]) && !(x[0] && x[1] && x[2]);//safety property to be checked

    PF=!(x[0] && x[1] && x[2]);//safety property to be checked

    std::cout << "P is :" << TF << "\n\n";


//-------------------------------------------------------------------------------------------------------------------------------
	translate(0);
//-------------------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------------------
//Now compute the actual formula to be sent to SAT solver
//-------------------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------------------

	S.add(alpha[0] && !lambda[0]);


	bool result=S.check();
	//std:://getchar();

	if(result==sat){
//Print the trace and exit	
	    	std::cout <<"\nThe trace of length 0";
	    	std::cout << "\n";
        	print_trace(0);
		return 0;
	}    
	else{
	  	std::cout <<"\nNo trace of length 0";
	   	std::cout <<"\n----------------------------------------------------------------\n";     	
			
	}
    S.reset();
//-------------------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------------------------------------
//Run a loop to inductively instantiate the rest.
//-------------------------------------------------------------------------------------------------------------------------------
	int k=1;
	while(1){
		std::cout <<"\nLooking at traces of length " << k;
        //getchar();
//
//----------------------------------------------------------------------------------------------------------------------------------
		translate(k);
//------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------------
//Construct the fw_bound formula for k and send to SAT Solver
//-------------------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------------ 
        S.add(alpha[0] && alpha[k] && beta[k]);
        result=S.check();
       	if(result==unsat){
       		std::cout << "\nForward bound reached with k=" << k << ".\n\n\n\n";
       		return 0;
       	}
       	std::cout << "\nForward bound not reached for k=" << k << ".\n\n\n\n\n";
        S.reset();
//-------------------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------------
//Construct the bw_found formula for k and send to SAT Solver
//-------------------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------------

 
/*    
        S.add(alpha[k] && beta[k] && !lambda[k]);
    	result=S.check();
       	if(result==unsat){
       		std::cout << "\nBackward bound reached with k=" << k << ".\n\n\n\n";
       		return 0;
       	}
       	std::cout << "\nBackward bound not reached for k=" << k << ".\n\n\n\n";
        S.reset();
*/
//-------------------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------------
//Construct the safety_check formula for k and send to SAT Solver
//-------------------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------------
     
        S.add(alpha[0] && alpha[k] && !lambda[k]);
	   	result=S.check();
	    if(result==sat){
//-------------------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------------
//Print the trace and exit	
		    std::cout <<"\nThe trace of length "<< k << "\n";fflush(stdout);
		//std:://getchar();
           	print_trace(k);
		    return 0;
	    }
	    else{
//-------------------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------------
//Continue the loop
		    std::cout <<"\nNo trace of length "<< k << "\n";
		    std::cout <<"\n----------------------------------------------------------------\n";
//-------------------------------------------------------------------------------------------------------------
            S.reset();
		    k++;
           	continue;
	    }
//-------------------------------------------------------------------------------------------------------------
	}//end of while loop
//-------------------------------------------------------------------------------------------------------------
    	return 1;
}
//----------------------------------------------------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------------------------------------------------------------
void k_induction::translate(int k)
{
	switch(k){
	
		case 0:	
			alpha.push_back(IF);//for trace of length 0
			std::cout << "\nComputed alpha[0]\n";
			
			beta.push_back(c.bool_const("false"));
			std::cout << "\nComputed beta[0]\n";

			lambda.push_back(PF);
			std::cout << "\nComputed alpha[0]\n";

			break;

		case 1:	
			alpha.push_back(instantiate_T_at_k(1));//for trace of length 0
			std::cout << "\nComputed alpha[1]\n";
			
			beta.push_back(loopFree_at_k(1));//for trace of length k
			std::cout << "\nComputed beta[1]\n";

			lambda.push_back(instantiate_P_at_k(1));
			std::cout << "\nComputed lambda[1]\n";

			break;



		default:
			alpha.push_back(alpha[k-1] && instantiate_T_at_k(k));//for trace of length k
			std::cout << "\nComputed alpha[" << k << "]\n";
			
			beta.push_back(beta[k-1] && loopFree_at_k(k));
			std::cout << "\nComputed beta[" << k << "]\n";

			lambda.push_back(instantiate_P_at_k(k));
			std::cout << "\nComputed lambda[" << k << "]\n";

			break;
	}
	
}
//-----------------------------------------------------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------------------------------------
void k_induction::print_trace(int k)
{
                    model m = S.get_model();
 //                   std::cout << m << "\n\n";
    // traversing the model
                    for (unsigned i = 0; i < m.size(); i++) {
                 //       std::cout << m[i] << "\n\n";
                        func_decl v = m[i];
                //        std::cout << v << "\n\n";
                    // this problem contains only constants
                 //       assert(v.arity() == 0); 
                        std::cout << v.name() << " = " << m.get_const_interp(v) << " ";
                    }
                    std::cout << "\n\n";


}
//-----------------------------------------------------------------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------------------------------------------------------------------
expr k_induction::loopFree_at_k(int k)
{

                expr_vector	z(c);
				for(unsigned i=0; i<k*N; ++i){
					std::stringstream xName;
					xName << "x" << i;
					z.push_back(c.bool_const(xName.str().c_str()));
				}

//                std::cout << "\n\ncontents of z are" << z << "\n";
//                //getchar();
/*                
                expr_vector	z0(c);
				for(unsigned i=0; i<N; ++i){
					std::stringstream xName;
					xName << "x" << i+N*(k-1);
					z0.push_back(c.bool_const(xName.str().c_str()));
				}
*/
                expr_vector	z1(c);
				for(unsigned i=0; i<N; ++i){
					std::stringstream xName;
					xName << "x" << i+N*k;
					z1.push_back(c.bool_const(xName.str().c_str()));
				}

//                std::cout << "\n\ncontents of z1 are" << z1 << "\n";
//                //getchar();

//Now, compute loopFree expression
                std::cout << "\n\nNow wo compute loopFree expression for k=" << k << "\n";

                expr temp0(c);
                expr temp1(c);

                temp0=(z1[0]!=z[0]);
                for(unsigned i=1; i<N; ++i){
                    temp0=temp0 || (z1[i]!=z[i]);
                }
                temp1=temp0;
                for(unsigned j=1; j<k; ++j){
                    temp0=(z1[0]!=z[N*j]);
                    for(unsigned i=1; i<N; ++i){
                        temp0=temp0 || (z1[i]!=z[N*j+i]);
                    }
                    temp1=temp1 && temp0;
                }

//               expr loopFree(c);

//                if(k==1){
//                    loopFree=temp1;
//                }
//                else{
//                    loopFree=loopFree && temp1;
//                }
                std::cout << "\n\nLoopFree formula is-->"<< temp1 <<"\n\n";
                return temp1;

}
//-----------------------------------------------------------------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------------------------------------------------------------

//-----------------------------------------------------------------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------------------------------------------------------------
expr k_induction::instantiate_P_at_k(int k)
{

                expr_vector	z1(c);
				for(unsigned i=0; i<N; ++i){
					std::stringstream xName;
					xName << "x" << i+N*k;
					z1.push_back(c.bool_const(xName.str().c_str()));
				}


                expr subP(c);
                subP=PF.substitute(x,z1);
                std::cout << "P substituted : " << subP << "\n\n"; 
                //getchar();
                return subP;

}
//------------------------------------------------------------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------------------------------------------------------------
expr k_induction::instantiate_T_at_k(int k)
{
               
               expr_vector	z(c);
				for(unsigned i=0; i<k*N; ++i){
					std::stringstream xName;
					xName << "x" << i;
					z.push_back(c.bool_const(xName.str().c_str()));
				}

                
                expr_vector	z0(c);
				for(unsigned i=0; i<N; ++i){
					std::stringstream xName;
					xName << "x" << i+N*(k-1);
					z0.push_back(c.bool_const(xName.str().c_str()));
				}

                expr_vector	z1(c);
				for(unsigned i=0; i<N; ++i){
					std::stringstream xName;
					xName << "x" << i+N*k;
					z1.push_back(c.bool_const(xName.str().c_str()));
				}

//subT is T(x,y) with x=(x0,x1,x2) replaced current state z0 and y=(y0,y1,y2) replaced by next state z1.
                expr subT(c);
                subT=TF.substitute(y,z1);
                subT=subT.substitute(x,z0);
                std::cout << "T substituted : " << subT << "\n\n"; 
                //getchar();
                return subT;

}
//------------------------------------------------------------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------------------------------------------------------------
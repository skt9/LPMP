/*
 * ldp_triangle_factor.hxx
 *
 *  Created on: Apr 6, 2020
 *      Author: fuksova
 */

#ifndef INCLUDE_LIFTED_DISJOINT_PATHS_LDP_TRIANGLE_FACTOR_HXX_
#define INCLUDE_LIFTED_DISJOINT_PATHS_LDP_TRIANGLE_FACTOR_HXX_

#include <cstdlib>
#include <string>         // std::string
#include <bitset>
#include <vector>
#include <assert.h>
namespace LPMP {

class ldp_triangle_factor
{
public:

	//By default, all edges are lifted. However vu or uw can be base too.
	ldp_triangle_factor(size_t vu,size_t uw,size_t vw,bool vuBase=0,bool uwBase=0): //Maybe remember vertex indices instead of edge indices?
		vuInd(vu),
		uwInd(uw),
		vwInd(vw)
{

		//Feasible labelings of edge (vu,uw,vw)
		labelings={std::bitset<3>("000"),std::bitset<3>("111"),std::bitset<3>("100"),std::bitset<3>("010"),std::bitset<3>("001")};
		if(vuBase){
			labelings.push_back(std::bitset<3>("011"));
		}
		if(uwBase){
			labelings.push_back(std::bitset<3>("101"));
		}
		primal_=0;
}

	double LowerBound() const{
		double minValue=0;
		short minIndex=0;
		for (short i = 0; i < labelings.size(); ++i) {
			std::bitset<3> label=labelings[i];
			double value=label[0]*edgeCosts[0]+label[1]*edgeCosts[1]+label[2]*edgeCosts[2];
			if(value<minValue){
				minValue=value;
				minIndex=i;
			}
		}
		primal_=minIndex;
		return minValue;
	}

	double EvaluatePrimal() const{
		std::bitset<3> label=labelings[primal_];
		double value=label[0]*edgeCosts[0]+label[1]*edgeCosts[1]+label[2]*edgeCosts[2];
		return value;
	}

	template<class ARCHIVE> void serialize_primal(ARCHIVE& ar) { ar(primal_); }
	template<class ARCHIVE> void serialize_dual(ARCHIVE& ar) { ar(); }

	auto export_variables() { return std::tie(*static_cast<std::size_t>(this)); }

	void init_primal() { primal_ = 0; }

	double delta(short edgeId){  //difference: label[edgeId]=1-label[edgeId]=0
		assert(edgeId<=2);
		double min1=std::numeric_limits<double>::infinity();
		double min0=std::numeric_limits<double>::infinity();
		for (int i = 0; i < labelings.size(); ++i) {
			double value=labelings[i][0]*edgeCosts[0]+labelings[i][1]*edgeCosts[1]+labelings[i][2]*edgeCosts[2];
			if(labelings[i][edgeId]){
				min1=std::min(min1,value);
			}
			else{
				min0=std::min(min0,value);
			}
		}
		return min1-min0;
	}

	void updateCost(size_t edgeId,double update){  //update cost of one edge, assumed indices 0-2
		assert(edgeId<=2);
		edgeCosts[edgeId]+=update;
	}

private:
	std::size_t primal_; // index of feasible labeling
	size_t vuInd,uwInd,vwInd;
	double edgeCosts[3];
	std::vector<std::bitset<3>> labelings;
};





}





#endif /* INCLUDE_LIFTED_DISJOINT_PATHS_LDP_TRIANGLE_FACTOR_HXX_ */

/*
 * Genome.cpp
 *
 * The genetic representation of this morphology is a directed graph of
 * nodes and connections. Each graph contains the developmental instructions
 * for growing a creature, and provides a way of reusing instructions to
 * make similar or recursive components within the creature. A phenotype
 * hierarchy of parts is made from a graph by starting at a defined
 * root-node and synthesizing parts from the node information while tracing
 * through the connections of the graph. The graph can be recurrent. Nodes
 * can connect to themselves or in cycles to form recursive or fractal like
 * structures. They can also connect to the same child multiple times to
 * make duplicate instances of the same appendage.
 *
 *  Created on: Nov 17, 2014
 *      Author: leviathan
 */

//# corresponding header
#include <model/evolution/population/creature/genome/MixedGenome.h>
#include "configuration/PopulationConfiguration.h"
#include "configuration/MorphologyConfiguration.h"

//## controller headers
//## model headers
//## view headers
//## utils headers
#include "utils/Randomness.h"

MixedGenome::MixedGenome() :
		mSegmentsDepthLimit(0), mTotalSegmentQtyLimit(0) {

}

MixedGenome::~MixedGenome() {
	while (!mGenes.empty()) {
		Morphogene* f = mGenes.back();
		mGenes.pop_back();
		delete f;
	}
}

void MixedGenome::createRandomGenome(double branchiness) {
	Randomness randomness;

	int geneQty =
			1
					+ abs(
							randomness.nextNormalInt(
									PopulationConfiguration::POPULATION_GENES_INITIAL_MEAN,
									PopulationConfiguration::POPULATION_GENES_INITIAL_VAR));
	for (int i = 0; i < geneQty; i++) {
		Morphogene* gene = new Morphogene();
		gene->initialize(branchiness);
		mGenes.push_back(gene);
	}

	mSegmentsDepthLimit = abs(
			randomness.nextNormalInt(
					MorphologyConfiguration::LIMB_DEPTH_INITIAL_MEAN,
					MorphologyConfiguration::LIMB_DEPTH_INITIAL_VAR));

	mTotalSegmentQtyLimit = abs(
			randomness.nextNormalInt(
					MorphologyConfiguration::LIMB_TOTAL_INITIAL_MEAN,
					MorphologyConfiguration::LIMB_TOTAL_INITIAL_VAR));
}

void MixedGenome::addGene(Morphogene* gene) {
	mGenes.push_back(gene);
}

void MixedGenome::linkRandomGenes() {
	Randomness randomness;
	for (int geneIt = 0; geneIt < mGenes.size(); geneIt++) {

		// randomly choose a follow up gene until you get one different from its own type
		do {
			mGenes[geneIt]->setFollowUpGene(
					randomness.nextPosInt(0, mGenes.size() - 1));
		} while (mGenes[geneIt]->getFollowUpGene() != geneIt);

		std::vector<MorphogeneBranch*>::iterator branchIt =
				mGenes[geneIt]->getGeneBranches().begin();
		for (; branchIt != mGenes[geneIt]->getGeneBranches().end(); branchIt++) {

			//randomly choose a branch gene type until you get one distinct from the follow up gene
			do {
				(*branchIt)->setBranchGeneType(
						randomness.nextPosInt(0, mGenes.size() - 1));
			} while (mGenes.size() > 1
					&& (*branchIt)->getBranchGeneType()
							== mGenes[geneIt]->getFollowUpGene());

		}
	}
}

bool MixedGenome::equals(const MixedGenome & genome) const {

	std::vector<Morphogene*>::const_iterator it = mGenes.begin();
	std::vector<Morphogene*>::const_iterator it2 = genome.mGenes.begin();
	for (; it != mGenes.end(), it2 != genome.mGenes.end(); it++, it2++) {
		if (!(*it)->equals(**it2)) {
			return false;
		}
	}

	return true;
}

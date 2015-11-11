#ifndef MODEL_UNIVERSE_EVOLUTION_POPULATION_CREATURE_MODELORGANISM_MODELLEG_SnakeBuilder_HPP_
#define MODEL_UNIVERSE_EVOLUTION_POPULATION_CREATURE_MODELORGANISM_MODELLEG_SnakeBuilder_HPP_

//# corresponding headers
//# forward declarations
class MixedGenome;

//# system headers
//## controller headers
//## model headers
//## view headers
//# custom headers
//## base headers
//## configuration headers
//## controller headers
//## model headers
//## view headers
//## utils headers

/**
 * @brief		The model leg builder creates a simple model leg in the genome.
 * @details		Details
 * @date		2015-11-05
 * @author		Benjamin Ellenberger
 */
class SnakeBuilder {
public:
	SnakeBuilder();
	virtual ~SnakeBuilder();

	static void build(MixedGenome* genome);
};

#endif /* MODEL_UNIVERSE_EVOLUTION_POPULATION_CREATURE_MODELORGANISM_MODELLEG_SnakeBuilder_HPP_ */
/*
 * EvolutionConfiguration.h
 *
 *  Created on: Nov 17, 2014
 *      Author: leviathan
 */

#ifndef EVOLUTIONCONFIGURATION_H_
#define EVOLUTIONCONFIGURATION_H_

class EvolutionConfiguration {
public:
	//
	// constants
	//
	/**
	 * The size of the population. This is one more than the number of new individuals created in each generation, since
	 * the first individual in a generation is not new, but a copy of the best individual from
	 * the previous generation.
	 */
	static const unsigned POPULATIONSIZE = 20;
	static const unsigned DIMENSIONS = 200;

	/**
	 * The number of generations before the evolution terminates
	 */
	static const unsigned EVOLUTION_GENERATIONS = 1000;

	/**
	 * Number of elitists (the ones that get carried over to the next generation because they are so successful)
	 */
	static const unsigned EVOLUTION_SELECTION_ELITISTS_QTY = 1;

	/**
	 * This is the number of times that a creature will be evaluated during a fitness
	 measurement. This value is set to one in all of the experimental
	 setups described in this work although it can be set higher to reduce noise.
	 The trade-off is one of noise reduction vs. computation time.
	 */
	static const unsigned EVOLUTION_EVALUATION_RUNS = 1;
	static const unsigned Omega = 5;
	static const unsigned CrossPoints = 2;
	static const double CrossProb = 0.6;
	static const double FlipProb = 1. / DIMENSIONS;

	/**
	 * Whenever a parent is needed, this setting dictates the number of individuals to choose
	 * at random from the previous generation. These individuals constitute a tournament.
	 * The creature with the highest fitness wins and becomes the selected parent. The
	 * higher this value is in relation to the population size, the higher the selection pressure
	 * will be and the more rapid the loss of diversity will be.
	 */
	static const unsigned EVOLUTION_TOURNAMENT_SIZE = 10;

	/**
	 * This is the probability that a new creature, in any generation other than the first, will
	 be produced by the crossing of two parents from the previous generation.
	 */
	static const double EVOLUTION_CREATURE_CROSSOVER_RATE = 0.1;

	/**
	 * This is the mutation rate used in mutation operators affecting the genome.
	 */
	static const double EVOLUTION_CREATURE_MUTATION_RATE = 0.1;
};

#endif /* EVOLUTIONCONFIGURATION_H_ */

/*! \file ran.h
    \brief Wrapper to Boost random number facilities.
    Copyright 2009 Tuomas Tonteri. This file is licensed as public domain.

    Usage example: 

	Ran01<float> gen; // Call constructor
        gen(); // Generate a random number

        Ran<double> gen(seed,min,max) // Call constructor
        gen(); // Generate a random number
*/

#ifndef RAN_H
#define RAN_H

#include <boost/random.hpp>
#include <fstream>

#ifdef _MSC_VER
typedef unsigned __int64 uint64_t;
typedef __int64 int64_t;
#endif

typedef boost::mt19937 base_generator_type;
//typedef boost::minstd_rand base_generator_type;
//typedef boost::rand48 base_generator_type;
//typedef boost::lagged_fibonacci607 base_generator_type;

#ifdef unix
uint64_t GetSeed()
{
        uint64_t seed;
        std::ifstream urandom;
        urandom.open("/dev/urandom");
        urandom.read(reinterpret_cast<char*> (&seed), sizeof (seed));
        urandom.close();
	return seed;
}
#elif _WIN32
// TODO: Implement GetSeed() on Windows platform
uint64_t GetSeed()
{
	return 1;
}
#endif

/** Uniform distribution random numbers between 0 .. 1 using Boost. A wrapper over Boost::random. */
template <class realtype>
class Ran01 {
        base_generator_type generator;
        boost::uniform_01<base_generator_type,realtype> gen;
public:

        Ran01(uint64_t seed) : generator((uint64_t)seed), gen(generator) { }
        Ran01(void) : generator((uint64_t)GetSeed()), gen(generator) { }

        realtype operator () (void) { return gen(); } ///< Generate a random number
};

/** Uniform distribution integer random numbers. */
template <class inttype>
class IRan {
        base_generator_type generator;
        boost::uniform_int<inttype> uni_dist;
        boost::variate_generator<base_generator_type, boost::uniform_int<inttype> > gen;
public:
        /** Initialize generator with seed, min and max */
	IRan(long seed, int min, int max) : generator((uint64_t)seed), uni_dist(min,max), gen(generator,uni_dist) { }
	IRan(int min, int max) : generator((uint64_t)GetSeed()), uni_dist(min,max), gen(generator,uni_dist) { }

        inttype operator () (void) { return gen(); } ///< Generate a random number
};

/** Uniform distribution random numbers. */
template <class realtype>
class Ran {
	base_generator_type generator;
	boost::uniform_real<realtype> uni_dist;
	boost::variate_generator<base_generator_type, boost::uniform_real<realtype> > gen;
public:
	/** Initialize generator with seed, min and max */
	Ran(long seed, int min, int max) : generator((uint64_t)seed), uni_dist(min,max), gen(generator,uni_dist) { }
	Ran(int min, int max) : generator((uint64_t)GetSeed()), uni_dist(min,max), gen(generator,uni_dist) { }

	realtype operator () (void) { return gen(); } ///< Generate a random number
};


/** Normal distribution random numbers */
template <class realtype>
class Nran {
	base_generator_type generator;
	boost::normal_distribution<realtype> norm_dist;
	boost::variate_generator<base_generator_type, boost::normal_distribution<realtype> > gen;
public:
	/** Initialize generator with seed, mean and sigma */
	Nran(long seed, int mean, int sigma) : generator((uint64_t)seed), norm_dist(mean,sigma), gen(generator,norm_dist) { }
	Nran(int mean, int sigma) : generator((uint64_t)GetSeed()), norm_dist(mean,sigma), gen(generator,norm_dist) { }

	realtype operator () (void) { return gen(); } ///< Generate a random number
};

#endif

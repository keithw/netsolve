#include <cmath>
#include <tuple>
#include <limits>
#include <stdexcept>
#include <iostream>

#include "optimizer.hh"

/* PCC utility function as described in NSDI paper */
/* (rewards throughput, penalizes loss) */

/* alpha constant from PCC source code */
/* https://github.com/modong/pcc/blob/1afc958f/README.md */
/* https://github.com/modong/pcc/blob/1afc958f/sender/app/cc.h#L302 */

inline double utility_function( const double sending_rate,
				const double throughput )

{
  //  const double loss_rate = 1 - throughput / sending_rate;
  return .001 * log( sending_rate ) - sending_rate + throughput;
}

/* perturb one component of the rates */
template <unsigned int component>
template <class RateType>
RateType Optimizer<component>::adjust_rate( RateType rates,
					    const double delta )
{
  std::get<component>( rates ) = std::max( 0.0, std::get<component>( rates ) + delta );
  return rates;
}

/* calculate individual sender's utility for this configuration of rates */
template <unsigned int component>
template <class NetworkType>
double Optimizer<component>::evaluate( NetworkType & network,
				       const typename NetworkType::Rates & rates )
{
  const auto throughputs = network.throughputs_fast( rates );
  const double score = utility_function( std::get<component>( rates ), std::get<component>( throughputs ) );
  return score;
}

/* improve one component */
template <unsigned int component>
template <class NetworkType>
typename NetworkType::Rates Optimizer<component>::search_one( NetworkType & network,
							      typename NetworkType::Rates guess )
{
  /* don't allow flooding */
  if ( std::get<component>( guess ) > network.max_reasonable_rate() ) {
    throw std::runtime_error( "flooding detected" );
  }

  const auto sending_rates = guess;
  const auto throughputs = network.throughputs_fast( sending_rates );
  const auto our_sending_rate = std::get<component>( sending_rates );
  const auto our_throughput =   std::get<component>( throughputs );
  const auto our_loss_rate = our_sending_rate - our_throughput;

  const double adjustment = .01 * (.01 - our_loss_rate);

  //  std::cerr << "flow " << component << " adjusting rate from " << our_sending_rate << " by " << adjustment << " => ";

  guess = adjust_rate( guess, adjustment );

  //  std::cerr << "now " << std::get<component>( guess ) << "\n";
  return guess;
}

#include <cstdlib>
#include <iostream>
#include <limits>
#include <cmath>
#include <iomanip>

#include "parking_lot.hh"

using namespace std;

void print( const tuple<double, double, double> & best_rates )
{
  cout << "** Rates: " << get<0>( best_rates )
       << " " << get<1>( best_rates )
       << " " << get<2>( best_rates ) << "\n";  
}

double PCC_utility( const double sending_rate,
		   const double throughput )
{
  const double loss_rate = 1 - throughput / sending_rate;
  return throughput * ( 1.0f - 1.0f / ( 1.0f + exp( -100.0f * ( loss_rate - 0.05f ) ) ) ) - sending_rate * loss_rate;
}

double utility( const double sending_rate __attribute((unused)),
		const double throughput )
{
  return log( throughput );
}

template <unsigned int i>
tuple<double, double, double> adjust_guess( tuple<double, double, double> guess,
					    const double epsilon )
{
  get<i>( guess ) += epsilon;
  get<i>( guess ) = max( 0.0, get<i>( guess ) );
  return guess;
}

template <unsigned int i>
double evaluate( ParkingLot & network,
		 const tuple<double, double, double> guess )
{
  const auto throughputs = network.throughputs_fast( get<0>( guess ),
						     get<1>( guess ),
						     get<2>( guess ) );

  const double score = PCC_utility( get<i>( guess ), get<i>( throughputs ) );

  return score;
}

template <unsigned int i>
double partial( ParkingLot & network,
		const tuple<double, double, double> guess,
		const double epsilon )
{
  /* reference point */
  const double score = evaluate<i>( network, guess );

  /* new point */
  const double new_score = evaluate<i>( network, adjust_guess<i>( guess, epsilon ) );

  /* partial derivative */
  return (new_score - score) / (epsilon);
}

template <unsigned int i>
tuple<double, double, double> search_one( ParkingLot & network,
					  tuple<double, double, double> guess )
{
  //  cout << "Optimizing sender " << i << "\n";

  double delta = 32768 * numeric_limits<double>::epsilon();

  while ( true ) {
    if ( delta >= 0.1 ) {
      return guess;
    }

    const auto low_guess = adjust_guess<i>( guess, -delta );
    const auto high_guess = adjust_guess<i>( guess, delta );

    const double low_score = evaluate<i>( network, low_guess );
    const double mid_score = evaluate<i>( network, guess );
    const double high_score = evaluate<i>( network, high_guess );

    if ( get<i>( guess ) >= 100 ) {
      get<i>( guess ) *= 0.5;
      cout << "Terminating search because flooding found.\n";
      return guess;
    }

    if ( mid_score > low_score and mid_score > high_score ) {
      //      cout << "Terminating search with multiplier = " << multiplier << "\n";
      return guess;
    } else if ( low_score > mid_score ) {
      guess = low_guess;
      delta *= 1.1;
    } else {
      guess = high_guess;
      delta *= 1.1;
    }
  }
}

tuple<double, double, double> search( ParkingLot & network,
				      const tuple<double, double, double> guess,
				      const double search_radius )
{
  bool edge = true;
  tuple<double, double, double> best_throughputs { -1, -1, -1 };
  double best_score = numeric_limits<double>::lowest();

  const double A_min = max( 0.0, get<0>( guess ) - search_radius );
  const double A_max = get<0>( guess ) + search_radius;
  const double B_min = max( 0.0, get<1>( guess ) - search_radius );
  const double B_max = get<1>( guess ) + search_radius;
  const double C_min = max( 0.0, get<2>( guess ) - search_radius );
  const double C_max = get<2>( guess ) + search_radius;

  cerr << "Searching A in [" << A_min << ".." << A_max << "]"
       << ", B in [" << B_min << ".." << B_max << "]"
       << ", C in [" << C_min << ".." << C_max << "].\n";

  for ( double A = A_min; A < A_max; A += (A_max - A_min) / 500.0 ) {
    for ( double B = B_min; B < B_max; B += (B_max - B_min) / 500.0 ) {
      for ( double C = C_min; C < C_max; C += (C_max - C_min) / 500.0 ) {
	const auto throughputs = network.throughputs_fast( A, B, C );
	const double score =
	  PCC_utility( A, get<0>( throughputs ) )
	  + PCC_utility( B, get<1>( throughputs ) )
	  + PCC_utility( C, get<2>( throughputs ) );	  
	if ( score > best_score ) {
	  best_score = score;
	  best_throughputs = throughputs;

	  edge = ( A == A_min or A == A_max
		   or B == B_min or B == B_max
		   or C == C_min or C == C_max );
	}
      }
    }
  }

  if ( edge ) {
    throw runtime_error( "solution only found on edge of search space" );
  }

  return best_throughputs;
}

int main()
{
  ParkingLot network;

  for ( double A = 0; A < 25; A += 0.5 ) {
    for ( double B = 0; B < 25; B += 0.5 ) {
      for ( double C = 0; C < 25; C += 0.5 ) {
	tuple<double, double, double> best_rates { A, B, C };

	for ( unsigned int i = 0; i < 100000; i++ ) {
	  auto new_rates_A = search_one<0>( network, best_rates );
	  auto new_rates_B = search_one<1>( network, best_rates );
	  auto new_rates_C = search_one<2>( network, best_rates );

	  auto new_rates = make_tuple( get<0>( new_rates_A ),
				       get<1>( new_rates_B ),
				       get<2>( new_rates_C ) );

	  if ( new_rates == best_rates ) {
	    break;
	  }

	  best_rates = new_rates;
	}

	cout << A << " " << B << " " << C << " -> ";
	print( best_rates );
	cout << flush;
      }
    }
  }

  return EXIT_SUCCESS;
}

#include <cstdlib>
#include <iostream>
#include <limits>
#include <cmath>

#include "parking_lot.hh"

using namespace std;

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
tuple<double, double, double> search_one( ParkingLot & network,
				       tuple<double, double, double> guess,
				       const double search_radius )
{
  tuple<double, double, double> best_rates { -1, -1, -1 };
  double best_score = numeric_limits<double>::lowest();

  const double the_min = max( 0.0, get<i>( guess ) - search_radius );
  const double the_max = get<i>( guess ) + search_radius;

  cout << "Optimizing sender " << i << " on [" << the_min << ".." << the_max << "].\n";

  for ( double val = the_min; val < the_max; val += (the_max - the_min) / 1000000.0 ) {
    get<i>( guess ) = val;
    const auto throughputs = network.throughputs_fast( get<0>( guess ),
						       get<1>( guess ),
						       get<2>( guess ) );
    const double score = PCC_utility( get<i>( guess ), get<i>( throughputs ) );

    if ( score > best_score ) {
      best_score = score;
      best_rates = guess;
    }
  }

  return best_rates;  
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

void print( const tuple<double, double, double> & best_rates )
{
  cout << "** Best rates: " << get<0>( best_rates )
       << " " << get<1>( best_rates )
       << " " << get<2>( best_rates ) << "\n";  
}

int main()
{
  ParkingLot network;
  tuple<double, double, double> best_rates { 17, 7, 3 };

  double scale = 20;

  while ( true ) {
    print( best_rates );
    cout << "Optimizing A.\n";
    best_rates = search_one<0>( network, best_rates, scale );

    print( best_rates );
    cout << "Optimizing B.\n";
    best_rates = search_one<1>( network, best_rates, scale );

    print( best_rates );
    cout << "Optimizing C.\n";
    best_rates = search_one<2>( network, best_rates, scale );

    scale *= 0.99;
  }

  /*
  best_throughputs = search( network, best_throughputs, 15 );
  cout << "Best throughputs after round 1: " << get<0>( best_throughputs )
       << " " << get<1>( best_throughputs )
       << " " << get<2>( best_throughputs ) << "\n";

  best_throughputs = search( network, best_throughputs, 1.5f );
  cout << "Best throughputs after round 2: " << get<0>( best_throughputs )
       << " " << get<1>( best_throughputs )
       << " " << get<2>( best_throughputs ) << "\n";

  best_throughputs = search( network, best_throughputs, .15f );
  cout << "Best throughputs after round 3: " << get<0>( best_throughputs )
       << " " << get<1>( best_throughputs )
       << " " << get<2>( best_throughputs ) << "\n";

  best_throughputs = search( network, best_throughputs, .015f );
  cout << "Best throughputs after round 4: " << get<0>( best_throughputs )
       << " " << get<1>( best_throughputs )
       << " " << get<2>( best_throughputs ) << "\n";
  */

  cout << "Successful audits of the fast calculation: " << network.audit_count() << "\n";

  return EXIT_SUCCESS;
}

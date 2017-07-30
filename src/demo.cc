#include <cstdlib>
#include <iostream>
#include <limits>
#include <cmath>

#include "parking_lot.hh"

using namespace std;

float PCC_utility( const float sending_rate,
		   const float throughput )
{
  const float loss_rate = 1 - throughput / sending_rate;
  return throughput * ( 1.0f - 1.0f / ( 1.0f + exp( -100.0f * ( loss_rate - 0.05f ) ) ) ) - sending_rate * loss_rate;
}

float utility( const float sending_rate __attribute((unused)),
	       const float throughput )
{
  return log( throughput );
}

tuple<float, float, float> search( ParkingLot & network,
				   const tuple<float, float, float> guess,
				   const float search_radius )
{
  bool edge = true;
  tuple<float, float, float> best_throughputs { -1, -1, -1 };
  float best_score = numeric_limits<float>::lowest();

  const float A_min = max( 0.0f, get<0>( guess ) - search_radius );
  const float A_max = get<0>( guess ) + search_radius;
  const float B_min = max( 0.0f, get<1>( guess ) - search_radius );
  const float B_max = get<1>( guess ) + search_radius;
  const float C_min = max( 0.0f, get<2>( guess ) - search_radius );
  const float C_max = get<2>( guess ) + search_radius;

  cerr << "Searching A in [" << A_min << ".." << A_max << "]"
       << ", B in [" << B_min << ".." << B_max << "]"
       << ", C in [" << C_min << ".." << C_max << "].\n";

  for ( float A = A_min; A < A_max; A += (A_max - A_min) / 500.0f ) {
    for ( float B = B_min; B < B_max; B += (B_max - B_min) / 500.0f ) {
      for ( float C = C_min; C < C_max; C += (C_max - C_min) / 500.0f ) {
	const auto throughputs = network.throughputs_fast( A, B, C );
	const float score =
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
  tuple<float, float, float> best_throughputs { 15, 15, 15 };

  cout << PCC_utility( 13.7f, 18.2f ) << "\n";

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

  cout << "Successful audits of the fast calculation: " << network.audit_count() << "\n";

  return EXIT_SUCCESS;
}

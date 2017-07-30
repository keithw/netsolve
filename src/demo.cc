#include <cstdlib>
#include <iostream>
#include <limits>
#include <cmath>

#include "parking_lot.hh"

using namespace std;

float PCC_utility( const float sending_rate,
		   const float throughput )
{
  const float loss_rate = 1 - sending_rate / throughput;
  return throughput * ( 1.0f - 1.0f / ( 1.0f + exp( -100.0f * ( loss_rate - 0.05f ) ) ) ) - sending_rate * loss_rate;
}

float utility( const float throughput )
{
  return log( throughput );
}

int main()
{
  ParkingLot network;

  tuple<float, float, float> best_throughputs;
  float best_score = numeric_limits<float>::lowest();
  unsigned int calculation_count = 0;
  int last_percent = -1;

  for ( float A = 0; A < 30; A += .05f ) {
    const int percent = int( 100 * A / 30 );
    if ( percent != last_percent ) {
      cerr << percent << "%\n";
      last_percent = percent;
    }
    for ( float B = 0; B < 30; B += .05f ) {
      for ( float C = 0; C < 30; C += .05f ) {
	const auto throughputs = network.throughputs_fast( A, B, C, calculation_count++ );
	const float score =
	  PCC_utility( A, get<0>( throughputs ) )
	  + PCC_utility( B, get<1>( throughputs ) )
	  + PCC_utility( C, get<2>( throughputs ) );
	if ( score > best_score ) {
	  best_score = score;
	  best_throughputs = throughputs;
	}
      }
    }
  }

  cout << "Best throughputs: " << get<0>( best_throughputs )
       << " " << get<1>( best_throughputs )
       << " " << get<2>( best_throughputs ) << "\n";

  cout << "Successful audits of the fast calculation: " << network.audit_count() << "\n";

  return EXIT_SUCCESS;
}

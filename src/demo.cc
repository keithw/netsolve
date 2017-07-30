#include <cstdlib>
#include <iostream>
#include <limits>
#include <cmath>

#include "parking_lot.hh"

using namespace std;

double utility( const double throughput )
{
  return log( throughput );
}

int main()
{
  ParkingLot network;

  tuple<double, double, double> best_throughputs;
  double best_score = numeric_limits<double>::lowest();
  unsigned int calculation_count = 0;

  for ( double A = 0; A < 20; A += .025 ) {
    cerr << A << "/20\n";
    for ( double B = 0; B < 20; B += .025 ) {
      for ( double C = 0; C < 20; C += .025 ) {
	const auto throughputs = network.throughputs_fast( A, B, C, calculation_count++ );
	const double score =
	  utility( get<0>( throughputs ) )
	  + utility( get<1>( throughputs ) )
	  + utility( get<2>( throughputs ) );
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

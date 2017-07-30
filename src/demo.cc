#include <cstdlib>
#include <iostream>
#include <iomanip>

#include "parking_lot.hh"
#include "optimizer.cc"

using namespace std;

int main()
{
  ParkingLot network;

  cout << setprecision( 20 );

  for ( double A = 0; A < 25; A += 0.5 ) {
    for ( double B = 0; B < 25; B += 0.5 ) {
      for ( double C = 0; C < 25; C += 0.5 ) {
	tuple<double, double, double> best_rates { A, B, C };

	while ( true ) {
	  auto new_rates = Optimizer<0>::search_one( network, best_rates );
	  new_rates = Optimizer<1>::search_one( network, new_rates );
	  new_rates = Optimizer<2>::search_one( network, new_rates );

	  const double diff_A = abs( get<0>( new_rates ) - get<0>( best_rates ) );
	  const double diff_B = abs( get<1>( new_rates ) - get<1>( best_rates ) );
	  const double diff_C = abs( get<2>( new_rates ) - get<2>( best_rates ) );

	  const double max_diff = max( max( diff_A, diff_B ), diff_C );

	  best_rates = new_rates;

	  if ( max_diff < 1e-7 ) {
	    break;
	  }
	}

	cout << A << " " << B << " " << C << " -> " << to_string( best_rates ) << "\n";
	cout << flush;
      }
    }
  }

  return EXIT_SUCCESS;
}

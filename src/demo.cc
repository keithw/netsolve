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
	  cout << "Trying to improve " << to_string( best_rates );

	  auto new_rates_A = Optimizer<0>::search_one( network, best_rates );
	  auto new_rates_B = Optimizer<1>::search_one( network, best_rates );
	  auto new_rates_C = Optimizer<2>::search_one( network, best_rates );

	  auto new_rates = make_tuple( get<0>( new_rates_A ),
				       get<1>( new_rates_B ),
				       get<2>( new_rates_C ) );

	  const double diff_A = abs( get<0>( new_rates ) - get<0>( best_rates ) );
	  const double diff_B = abs( get<1>( new_rates ) - get<1>( best_rates ) );
	  const double diff_C = abs( get<2>( new_rates ) - get<2>( best_rates ) );

	  const double max_diff = max( max( diff_A, diff_B ), diff_C );

	  cout << "max diff: " << max_diff << "\n";

	  best_rates = new_rates;

	  if ( max_diff < 1e-4 ) {
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

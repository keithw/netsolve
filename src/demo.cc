#include <cstdlib>
#include <iostream>

#include "parking_lot.hh"

using namespace std;

int main()
{
  ParkingLot network;

  const auto ret = network.throughputs( 15, 5, 5 );
  cout << get<0>( ret ) << "\n";

  const auto ret2 = network.throughputs( 15, 5, 6 );
  cout << get<0>( ret2 ) << "\n";

  return EXIT_SUCCESS;
}

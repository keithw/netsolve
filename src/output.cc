#include <algorithm>
#include <cassert>

#include "output.hh"
#include "multiplex.hh"

using namespace std;

double Output::delivery_rate( const Multiplex & multiplex ) const
{
  const double input = multiplex.total();
  assert( input >= 0 );

  if ( input < capacity_ ) {
    return 1.0;
  } else {
    return capacity_ / input;
  }
}

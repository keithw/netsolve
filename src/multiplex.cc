#include <stdexcept>
#include <numeric>

#include "multiplex.hh"

using namespace std;

void Multiplex::set_rate( const unsigned int index, const double rate )
{
  if ( rate < 0 ) {
    throw domain_error( "rate must be non-negative" );
  }

  inputs_.at( index ) = rate;
}

double Multiplex::total() const
{
  return accumulate( inputs_.begin(), inputs_.end(), 0.0 );
}

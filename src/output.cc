#include <algorithm>
#include <cassert>

#include "output.hh"
#include "multiplex.hh"

using namespace std;

double Output::delivery_proportion( const Multiplex & multiplex ) const
{
  return output_rate( multiplex ) / multiplex.total();
}

double Output::output_rate( const Multiplex & multiplex ) const
{
  return min( multiplex.total(), capacity_ );
}

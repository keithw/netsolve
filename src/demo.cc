#include <cstdlib>
#include <iostream>

#include "node.hh"
#include "flow.hh"

using namespace std;

int main()
{
  Node x { "x", 20 };
  Node y { "y", 20 };
  Node z { "z", 0 };

  Flow A { "A", "y" };
  Flow B { "B", "z" };
  Flow C { "C", "z" };

  A.set_rate( 15 );
  B.set_rate( 15 );
  C.set_rate( 15 );

  x.add_flow( A );
  y.add_flow( B );
  x.add_flow( C );

  x.connect( y );
  y.connect( z );

  cout << y.terminal_flow( "A" ).str() << "\n";
  cout << z.terminal_flow( "B" ).str() << "\n";
  cout << z.terminal_flow( "C" ).str() << "\n";

  return EXIT_SUCCESS;
}

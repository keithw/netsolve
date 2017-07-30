#pragma once

#include <tuple>

#include "node.hh"
#include "flow.hh"

class ParkingLot
{
private:
  Node x { "x", 20 };
  Node y { "y", 20 };
  Node z { "z", 0 };

  Flow A { "A", "y" };
  Flow B { "B", "z" };
  Flow C { "C", "z" };

public:
  std::tuple<double, double, double> throughputs( const double A_rate,
						  const double B_rate,
						  const double C_rate );
};

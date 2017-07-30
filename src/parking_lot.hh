#pragma once

#include <tuple>

#include "node.hh"
#include "flow.hh"

class ParkingLot
{
private:
  Node x { "x", 20 };
  Node y { "y", 10 };
  Node z { "z", 0 };

  Flow A { "A", "y" };
  Flow B { "B", "z" };
  Flow C { "C", "z" };

  std::tuple<double, double, double> throughputs_shortcut( const double A_rate,
							const double B_rate,
							const double C_rate ) const;

  unsigned int calculation_count_ {};
  unsigned int audit_count_ {};

public:
  std::tuple<double, double, double> throughputs( const double A_rate,
					       const double B_rate,
					       const double C_rate );

  std::tuple<double, double, double> throughputs_fast( const double A_rate,
						    const double B_rate,
						    const double C_rate );

  unsigned int audit_count() const { return audit_count_; }
};

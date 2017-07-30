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

  std::tuple<float, float, float> throughputs_shortcut( const float A_rate,
							const float B_rate,
							const float C_rate ) const;

  unsigned int calculation_count_ {};
  unsigned int audit_count_ {};

public:
  std::tuple<float, float, float> throughputs( const float A_rate,
					       const float B_rate,
					       const float C_rate );

  std::tuple<float, float, float> throughputs_fast( const float A_rate,
						    const float B_rate,
						    const float C_rate );

  unsigned int audit_count() const { return audit_count_; }
};

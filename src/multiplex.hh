#pragma once

#include <vector>

class Multiplex
{
private:
  std::vector<double> inputs_;

public:
  Multiplex( const unsigned int num )
    : inputs_( num, 0 )
  {}

  void set_rate( const unsigned int index, const double rate );
  double total() const;
};

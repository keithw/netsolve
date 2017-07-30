#pragma once

class Flow
{
private:
  std::string id_;
  std::string destination_;
  double rate_;

public:
  Flow( const std::string & id,
	const std::string & destination,
	const double rate )
    : id_( id ),
      destination_( destination ),
      rate_( rate )
  {}

  double rate() const { return rate_; }
  void set_rate( const double rate ) { rate_ = rate; }
  const std::string & destination() const { return destination_; }
};

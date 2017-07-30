#pragma once

#include <string>

class Flow
{
private:
  std::string source_;
  std::string destination_;
  float rate_;
  std::string route_;

public:
  Flow( const std::string & source,
	const std::string & destination,
	const float rate = 0 )
    : source_( source ),
      destination_( destination ),
      rate_( rate ),
      route_( source )
  {}

  void set_rate( const float rate ) { rate_ = rate; }

  const std::string & source() const { return source_; }
  const std::string & destination() const { return destination_; }
  float rate() const { return rate_; }
  const std::string & route() const { return route_; }

  Flow add_hop( const std::string & hop ) const;
  Flow scale( const float delivery_proportion ) const;

  std::string str() const;
};

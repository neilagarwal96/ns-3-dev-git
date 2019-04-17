/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */

#ifndef NS3_SACKHELPER_NUM_H
#define NS3_SACKHELPER_NUM_H

#include "ns3/sequence-number.h"

namespace ns3 {

  typedef std::pair<SequenceNumber32, SequenceNumber32> SackBlock; //!< SACK block definition

  inline std::ostream &
  operator<< (std::ostream & os, SackBlock const & sackBlock)
  {
    std::stringstream ss;
    ss << "[" << sackBlock.first << ";" << sackBlock.second << "]";
    os << ss.str ();
    return os;
  }
}

#endif /* NS3_SACKHELPER_NUM_H */

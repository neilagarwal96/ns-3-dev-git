/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2015 Natale Patriciello <natale.patriciello@gmail.com>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation;
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 */
#include "tcp-decongest.h"
#include "ns3/log.h"

namespace ns3 {

NS_LOG_COMPONENT_DEFINE ("TcpDecongest");
NS_OBJECT_ENSURE_REGISTERED (TcpDecongest);

TypeId
TcpDecongest::GetTypeId (void)
{
  static TypeId tid = TypeId ("ns3::TcpDecongest")
    .SetParent<TcpCongestionOps> ()
    .SetGroupName ("Internet")
    .AddConstructor<TcpDecongest> ()
  ;
  return tid;
}

TcpDecongest::TcpDecongest (void) : TcpCongestionOps ()
{
  NS_LOG_FUNCTION (this);
}

TcpDecongest::TcpDecongest (const TcpDecongest& sock)
  : TcpCongestionOps (sock)
{
  NS_LOG_FUNCTION (this);
}

TcpDecongest::~TcpDecongest (void)
{
}

uint32_t
TcpDecongest::SlowStart (Ptr<TcpSocketState> tcb, uint32_t segmentsAcked)
{
  NS_LOG_FUNCTION (this << tcb << segmentsAcked);
  tcb->m_cWnd = 10000000;
  if (segmentsAcked >= 1)
    {
      // tcb->m_cWnd += tcb->m_segmentSize;
      // NS_LOG_INFO ("In SlowStart, updated to cwnd " << tcb->m_cWnd << " ssthresh " << tcb->m_ssThresh);
      return segmentsAcked - 1;
    }

  return 0;
}

/**
 * \brief NewReno congestion avoidance
 *
 * During congestion avoidance, cwnd is incremented by roughly 1 full-sized
 * segment per round-trip time (RTT).
 *
 * \param tcb internal congestion state
 * \param segmentsAcked count of segments acked
 */
void
TcpDecongest::CongestionAvoidance (Ptr<TcpSocketState> tcb, uint32_t segmentsAcked)
{
  NS_LOG_FUNCTION (this << tcb << segmentsAcked);

  // if (segmentsAcked > 0)
    // {
      // double adder = static_cast<double> (tcb->m_segmentSize * tcb->m_segmentSize) / tcb->m_cWnd.Get ();
      // adder = std::max (1.0, adder);
      // tcb->m_cWnd += static_cast<uint32_t> (adder);
      // NS_LOG_INFO ("In CongAvoid, updated to cwnd " << tcb->m_cWnd <<
                   // " ssthresh " << tcb->m_ssThresh);
    // }
}

/**
 * \brief Try to increase the cWnd following the NewReno specification
 *
 * \see SlowStart
 * \see CongestionAvoidance
 *
 * \param tcb internal congestion state
 * \param segmentsAcked count of segments acked
 */
void
TcpDecongest::IncreaseWindow (Ptr<TcpSocketState> tcb, uint32_t segmentsAcked)
{
  NS_LOG_FUNCTION (this << tcb << segmentsAcked);

  tcb->m_cWnd = 100000;
  // if (tcb->m_cWnd < tcb->m_ssThresh)
  //   {
  //     segmentsAcked = SlowStart (tcb, segmentsAcked);
  //   }
  //
  // if (tcb->m_cWnd >= tcb->m_ssThresh)
  //   {
  //     CongestionAvoidance (tcb, segmentsAcked);
  //   }

  /* At this point, we could have segmentsAcked != 0. This because RFC says
   * that in slow start, we should increase cWnd by min (N, SMSS); if in
   * slow start we receive a cumulative ACK, it counts only for 1 SMSS of
   * increase, wasting the others.
   *
   * // Incorrect assert, I am sorry
   * NS_ASSERT (segmentsAcked == 0);
   */
}

std::string
TcpDecongest::GetName () const
{
  return "TcpDecongest";
}

uint32_t
TcpDecongest::GetSsThresh (Ptr<const TcpSocketState> state,
                         uint32_t bytesInFlight)
{
  NS_LOG_FUNCTION (this << state << bytesInFlight);

  return std::max (2 * state->m_segmentSize, bytesInFlight / 2);
}

Ptr<TcpCongestionOps>
TcpDecongest::Fork ()
{
  return CopyObject<TcpDecongest> (this);
}

} // namespace ns3

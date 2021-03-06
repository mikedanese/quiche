// Copyright (c) 2016 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "net/third_party/quiche/src/quic/core/quic_tag.h"

#include "net/third_party/quiche/src/quic/core/crypto/crypto_protocol.h"
#include "net/third_party/quiche/src/quic/platform/api/quic_flags.h"
#include "net/third_party/quiche/src/quic/platform/api/quic_test.h"

namespace quic {
namespace test {
namespace {

class QuicTagTest : public QuicTest {};

TEST_F(QuicTagTest, TagToString) {
  EXPECT_EQ("SCFG", QuicTagToString(kSCFG));
  EXPECT_EQ("SNO ", QuicTagToString(kServerNonceTag));
  EXPECT_EQ("CRT ", QuicTagToString(kCertificateTag));
  EXPECT_EQ("CHLO", QuicTagToString(MakeQuicTag('C', 'H', 'L', 'O')));
  // A tag that contains a non-printing character will be printed as hex.
  EXPECT_EQ("43484c1f", QuicTagToString(MakeQuicTag('C', 'H', 'L', '\x1f')));
}

TEST_F(QuicTagTest, MakeQuicTag) {
  QuicTag tag = MakeQuicTag('A', 'B', 'C', 'D');
  char bytes[4];
  memcpy(bytes, &tag, 4);
  EXPECT_EQ('A', bytes[0]);
  EXPECT_EQ('B', bytes[1]);
  EXPECT_EQ('C', bytes[2]);
  EXPECT_EQ('D', bytes[3]);
}

TEST_F(QuicTagTest, ParseQuicTag) {
  QuicTag tag_abcd = MakeQuicTag('A', 'B', 'C', 'D');
  EXPECT_EQ(ParseQuicTag("ABCD"), tag_abcd);
  QuicTag tag_efgh = MakeQuicTag('E', 'F', 'G', 'H');
  EXPECT_EQ(ParseQuicTag("EFGH"), tag_efgh);
  QuicTag tag_ijk = MakeQuicTag('I', 'J', 'K', 0);
  EXPECT_EQ(ParseQuicTag("IJK"), tag_ijk);
  QuicTag tag_l = MakeQuicTag('L', 0, 0, 0);
  EXPECT_EQ(ParseQuicTag("L"), tag_l);
  QuicTag tag_hex = MakeQuicTag('M', 'N', 'O', 255);
  EXPECT_EQ(ParseQuicTag("4d4e4fff"), tag_hex);
  EXPECT_EQ(ParseQuicTag("4D4E4FFF"), tag_hex);
  QuicTag tag_zero = 0;
  EXPECT_EQ(ParseQuicTag(""), tag_zero);
  QuicTagVector tag_vector;
  tag_vector.push_back(tag_abcd);
  EXPECT_EQ(ParseQuicTagVector("ABCD"), tag_vector);
  tag_vector.push_back(tag_efgh);
  EXPECT_EQ(ParseQuicTagVector("ABCD,EFGH"), tag_vector);
  tag_vector.push_back(tag_ijk);
  EXPECT_EQ(ParseQuicTagVector("ABCD,EFGH,IJK"), tag_vector);
  tag_vector.push_back(tag_l);
  EXPECT_EQ(ParseQuicTagVector("ABCD,EFGH,IJK,L"), tag_vector);
  tag_vector.push_back(tag_hex);
  EXPECT_EQ(ParseQuicTagVector("ABCD,EFGH,IJK,L,4d4e4fff"), tag_vector);
  tag_vector.push_back(tag_zero);
  EXPECT_EQ(ParseQuicTagVector("ABCD,EFGH,IJK,L,4d4e4fff,"), tag_vector);
}

}  // namespace
}  // namespace test
}  // namespace quic

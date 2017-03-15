/*
 * Ceph - scalable distributed file system
 *
 * Copyright (C) 2015 Mirantis, Inc.
 *
 * Author: Alyona Kiseleva <akiselyova@mirantis.com>
 *
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU Lesser General Public
 *  License as published by the Free Software Foundation; either
 *  version 2.1 of the License, or (at your option) any later version.
 *
 */

#ifndef CEPH_COMPRESSION_PLUGIN_ZSTD_H
#define CEPH_COMPRESSION_PLUGIN_ZSTD_H

// -----------------------------------------------------------------------------
#include "ceph_ver.h"
#include "compressor/CompressionPlugin.h"
#include "ZstdCompressor.h"
// -----------------------------------------------------------------------------

class CompressionPluginZstd : public CompressionPlugin {

public:

  explicit CompressionPluginZstd(CephContext* cct) : CompressionPlugin(cct)
  {}

  int factory(CompressorRef *cs,
	      std::ostream *ss) override
  {
    static const map<string, string> none_config;
    return factory(cs, ss, none_config);
  }

  int factory(CompressorRef *cs,
	      std::ostream *ss,
	      map<std::string, std::string> &config) override
  {
    if (compressor == 0) {
      ZstdCompressor *interface = new ZstdCompressor(config);
      compressor = CompressorRef(interface);
    }
    *cs = compressor;
    return 0;
  }
};

#endif

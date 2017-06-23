// -*- mode:C++; tab-width:8; c-basic-offset:2; indent-tabs-mode:t -*-
// vim: ts=8 sw=2 smarttab
/*
 * Ceph - scalable distributed file system
 *
 * Copyright (C) 2017 UMCloud <jiaying.ren@umcloud.com>
 *
 * This is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License version 2.1, as published by the Free Software
 * Foundation.  See file COPYING.
 *
 */

#ifndef RGW_BL_S3_H_
#define RGW_BL_S3_H_


#include <map>
#include <string>
#include <iostream>
#include <include/types.h>

#include <expat.h>

#include "include/str_list.h"
#include "rgw_bl.h"
#include "rgw_xml.h"

using namespace std;

class BLTargetBucket_S3 : public XMLObj
{
public:
  BLTargetBucket_S3() {}
  ~BLTargetBucket_S3() override {}
  string& to_str() { return data; }
};

class BLTargetPrefix_S3 : public XMLObj
{
public:
  BLTargetPrefix_S3() {}
  ~BLTargetPrefix_S3() override {}
  string& to_str() { return data; }
};

class BLTargetGrants_S3 : public BLTargetGrants, public XMLObj
{
public:
  BLTargetGrants_S3(CephContext *_cct) : BLTargetGrants(_cct) {}
  BLTargetGrants_S3() {}
  ~BLTargetGrants_S3() override {}
  string& to_str() { return data; }

  bool xml_end(const char *el) override;
};

class BLGrant_S3 : public BLGrant, public XMLObj
{
public:
  BLGrant_S3(CephContext *_cct) : BLGrant(_cct) {}
  BLGrant_S3() {}
  ~BLGrant_S3() override {}
  string& to_str() { return data; }

  bool xml_end(const char *el) override;
};

class BLGrantee_S3 : public XMLObj
{
public:
  BLGrantee_S3() {}
  ~BLGrantee_S3() override {}
  string& to_str() { return data; }
};

class BLID_S3 : public XMLObj
{
public:
  BLID_S3() {}
  ~BLID_S3() override {}
  string& to_str() { return data; }
};

class BLDisplayName_S3 : public XMLObj
{
public:
  BLDisplayName_S3() {}
  ~BLDisplayName_S3() override {}
  string& to_str() { return data; }
};

class BLEmailAddress_S3 : public XMLObj
{
public:
  BLEmailAddress_S3() {}
  ~BLEmailAddress_S3() override {}
  string& to_str() { return data; }
};

class BLURI_S3 : public XMLObj
{
public:
  BLURI_S3() {}
  ~BLURI_S3() override {}
  string& to_str() { return data; }
};

class BLPermission_S3 : public XMLObj
{
public:
  BLPermission_S3() {}
  ~BLPermission_S3() override {}
  string& to_str() { return data; }
};

class BLLoggingEnabled_S3 : public BLLoggingEnabled, public XMLObj
{
public:
  BLLoggingEnabled_S3(CephContext *_cct) : BLLoggingEnabled(_cct) {}
  BLLoggingEnabled_S3() {}
  ~BLLoggingEnabled_S3() override {}
  string& to_str() { return data; }

  bool xml_end(const char *el) override;
};

class RGWBLXMLParser_S3 : public RGWXMLParser
{
  CephContext *cct;

  XMLObj *alloc_obj(const char *el) override;
public:
  RGWBLXMLParser_S3(CephContext *_cct) : cct(_cct) {}
};

class RGWBucketLoggingStatus_S3 : public RGWBucketLoggingStatus, public XMLObj
{
public:
  RGWBucketLoggingStatus_S3(CephContext *_cct) : RGWBucketLoggingStatus(_cct) {}
  RGWBucketLoggingStatus_S3() : RGWBucketLoggingStatus(nullptr) {}
  ~RGWBucketLoggingStatus_S3() override {}

  bool xml_end(const char *el) override;
  void to_xml(ostream& out)
  {
    out << "<BucketLoggingStatus xmlns=\"http://s3.amazonaws.com/doc/2006-03-01/\">";
    if (is_enabled()) {
      out << "<LoggingEnabled>";

      string _bucket = this->get_target_bucket();
      out << "<TargetBucket>" << _bucket << "</TargetBucket>";

      string _prefix = this->get_target_prefix();
      out << "<TargetPrefix>" << _prefix << "</TargetPrefix>";

      if (enabled.target_grants_specified) {
        out << "<TargetGrants>";
        const std::vector<BLGrant> &bl_grants = this->get_target_grants();
        for (auto grant_iter = bl_grants.begin(); grant_iter != bl_grants.end(); grant_iter++) {
          out << "<Grant>";
          std::string _type = grant_iter->get_type();
          out << "<Grantee xmlns:xsi=\"http://www.w3.org/2001/XMLSchema-instance\" xsi:type=\"" 
              << _type << "\">";
          if (_type == grantee_type_map[BL_TYPE_CANON_USER]) {
            out << "<ID>" << grant_iter->get_id() << "</ID>";
            std::string _display_name = grant_iter->get_display_name();
            if (!_display_name.empty()) {
              out << "<DisplayName>" << _display_name << "</DisplayName>";
            }
          } else if (_type == grantee_type_map[BL_TYPE_EMAIL_USER]) {
            out << "<EmailAddress>" << grant_iter->get_email_address() << "</EmailAddress>";
          } else if (_type == grantee_type_map[BL_TYPE_GROUP]) {
            out << "<URI>" << grant_iter->get_uri() << "</URI>";
          }
          out << "</Grantee>";
          out << "<Permission>" << grant_iter->get_permission() << "</Permission>";
          out << "</Grant>";
        }
        out << "</TargetGrants>";
      }
      out << "</LoggingEnabled>";
    }
    out << "</BucketLoggingStatus>";
  }

  int rebuild(RGWRados *store, RGWBucketLoggingStatus& dest);
  void dump_xml(Formatter *f) const
  {
    f->open_object_section_in_ns("BucketLoggingStatus", XMLNS_AWS_S3);

    if (is_enabled()) {
      f->open_object_section("LoggingEnabled");

      string _bucket = this->get_target_bucket();
      encode_xml("TargetBucket", _bucket, f);

      string _prefix = this->get_target_prefix();
      encode_xml("TargetPrefix", _prefix, f);

      if (enabled.target_grants_specified) {
        f->open_object_section("TargetGrants");
        const std::vector<BLGrant> &bl_grants = this->get_target_grants();
        for (auto grant_iter = bl_grants.begin(); grant_iter != bl_grants.end(); grant_iter++) {
          f->open_object_section("Grant");
          std::string _type = grant_iter->get_type();
          std::string _grantee = "<Grantee xmlns:xsi=\"http://www.w3.org/2001/XMLSchema-instance\" xsi:type=\""
                                 + _type + "\">";
          f->write_raw_data(_grantee.c_str());
          if (_type == grantee_type_map[BL_TYPE_CANON_USER]) {
            encode_xml("ID", grant_iter->get_id(), f);
            std::string _display_name = grant_iter->get_display_name();
            if (!_display_name.empty()) {
              encode_xml("DisplayName", _display_name, f);
            }
          } else if (_type == grantee_type_map[BL_TYPE_EMAIL_USER]) {
            encode_xml("EmailAddress", grant_iter->get_email_address(), f);
          } else if (_type == grantee_type_map[BL_TYPE_GROUP]) {
            encode_xml("URI", grant_iter->get_uri(), f);
          }
          f->write_raw_data("</Grantee>");
          encode_xml("Permission", grant_iter->get_permission(), f);
          f->close_section();  // Grant
        }
        f->close_section();  // TargetGrants
      }

      f->close_section(); // LoggingEnabled
    }

    f->close_section(); // BucketLoggingStatus
  }

};

#endif

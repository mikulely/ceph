======================================
Server Access Logging / Bucket Logging
======================================

.. versionadded:: Luminous

The Ceph Object Gateway supports AWS S3 feature ``Server Access
Logging``, or named ``Bucket Logging``.


Configuration
=============

Enable Ops Log
--------------

First we need to enable radosgw to record ops log by adding following config to ``ceph.conf`` ::

  rgw enable ops log = true

Create bucket logging delivery user
-----------------------------------

For example::

  $ radosgw-admin user create --uid=bl_deliver --display-name="Bucket Logging Delivery" --bl_deliver
  {
  ...
      "user_id": "bl_deliver",
      "display_name": "Bucket Logging Delivery",
      "keys": [
          {
              "user": "bl_deliver",
              "access_key": "TGBG51N4F7V6DAKO5Y5D",
              "secret_key": "9OYA8DW4CaxykOaWbUaEwIi8C5QpwPOU1pmCP6CV"
          }
      ],
      "bl_deliver": "true",
  ...
  }

Add bucket logging delivery user to zone config
-----------------------------------------------

For example::

  $ radosgw-admin zone modify --access-key=TGBG51N4F7V6DAKO5Y5D \
                              --secret=9OYA8DW4CaxykOaWbUaEwIi8C5QpwPOU1pmCP6CV \
                              --bl_deliver --rgw-zonegroup=default --rgw-zone=default

  $ radosgw-admin -c ceph.conf zone get --rgw-zone=default
  {
      "id": "12d51989-9652-4921-8d6d-c4a4d356cc85",
      "name": "default",
     "bl_pool": "default.rgw.log:bl",
     ...
     "bl_deliver_key": {
         "access_key": "TGBG51N4F7V6DAKO5Y5D",
         "secret_key": "9OYA8DW4CaxykOaWbUaEwIi8C5QpwPOU1pmCP6CV"
     },
      ...
  }

Check whether logging delivery user info has been saved or not
--------------------------------------------------------------

For example::

  restart RGW, then

  $ radosgw-admin -c ceph.conf zone get --rgw-zone=default
  {
      "id": "12d51989-9652-4921-8d6d-c4a4d356cc85",
      "name": "default",
      "bl_pool": "default.rgw.log:bl",
      ...
      "bl_deliver_key": {
          "access_key": "TGBG51N4F7V6DAKO5Y5D",
          "secret_key": "9OYA8DW4CaxykOaWbUaEwIi8C5QpwPOU1pmCP6CV"
      },
      ...
  }


 Enable bucket logging deliver thread
------------------------------------

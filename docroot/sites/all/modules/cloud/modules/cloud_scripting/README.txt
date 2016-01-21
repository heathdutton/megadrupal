BASIC INFO
==========

- Provides scripting feature such as bash, Perl and etc (Mainly bash scripts)
  on running an instance (or a virtual machine).
- Works with Cloud and Server Templates module.

* Windows is NOT supported.


HOW TO USE
==========

To create a scripting item for a template

1) Enable Scripting module
2) Go to the menu: Design | Scripting
3) Click 'New Script' button
4) Enter Name, Description, Script Type, Packages, Inputs, and Script
5) Click 'Add' button
6) Create a template
7) Go to the menu: Design | Template
8) Select a scripting item

* Scripts are executed by drupal's cron.
  Please set up and adjust cron job time for script execution.

  If you set up one minute for cron job, the scripting module
  try to execute scripts every one minutes or re-try those.


DIRECTORY STRUCTURE
===================

cloud
  +-modules (depends on Cloud module) (Cloud is a core module for Cloud package)
    +-cloud_activity_audit
    +-cloud_alerts
    x-cloud_auto_scaling
    x-cloud_billing
    x-cloud_cluster
    x-cloud_dashboard
    x-cloud_failover
    +-cloud_inputs
    x-cloud_metering
    +-cloud_pricing
    x-cloud_resource_allocator
    x-cloud_scaling_manager
    o-cloud_scripting
    +-cloud_server_templates


    x... Not released yet.


CHANGE HISTORY
==============
2011/12/21 7.x-1.x-dev released


Copyright
=========
Copyright (c) 2011-2012 DOCOMO Innovations, Inc.


End of README.txt
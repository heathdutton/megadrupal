BASIC INFO
==========

- Provides server template functionality for multiple cloud of
  each cloud sub-system.
- Works with Cloud module.


HOW TO USE
==========

1) Enable Server Template module
2) Go to the menu: Design | Template
3) Click 'New Template' button
4) Input server template information
5) Click 'Add' button


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
    +-cloud_scripting
    o-cloud_server_templates


    x... Not released yet.


CHANGE HISTORY
==============
2011/12/21 7.x-1.x-dev released


Copyright
=========
Copyright (c) 2011-2012 DOCOMO Innovations, Inc.


End of README.txt
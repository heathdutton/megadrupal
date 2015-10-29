BASIC INFO
==========
- THIS MODULES DOES NOT DO ANYTHING AT THIS TIME.
- Provides a set of trigger and action for cloud management
- Works with Cloud. Failover and auto-scaling module.

* This module will do nothing; not implemented for sending e-mail, etc.
* This module should be implemented as based on trigger/action on Drupal.

HOW TO USE
==========

1) Enable Alert module
2) Go to the menu: Design | Alerts | New Alert Button
3) Create a server template
4) Select alert(s) in a server template


TODO
====

- This module started with D5, so it needs to match D6's trigger and action APIs. 


DIRECTORY STRUCTURE
===================

cloud
  +-modules (depends on Cloud module) (Cloud is a core module for Cloud package)
    +-cloud_activity_audit
    o-cloud_alerts
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
    +-cloud_server_templates


    x... Not released yet.


CHANGE HISTORY
==============
2011/12/21 7.x-1.x-dev released


Copyright
=========
Copyright (c) 2011-2012 DOCOMO Innovations, Inc.


End of README.txt
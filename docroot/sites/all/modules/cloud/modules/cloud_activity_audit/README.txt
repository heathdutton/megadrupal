BASIC INFO
==========

- Provides unified audit log for user's activities.
- Not using watchdog functionality of Drupal.
- Works with Cloud module.


HOW TO USE
==========

1) Enable Activity Audit module
2) Update cloud information (by clicking '- Refresh Page -' link) or
   take any action (e.g. launch an instance, create a server template, etc.)  
3) Go to the menu: Report | Activity Audit


DIRECTORY STRUCTURE
===================

cloud
  +-modules (depends on Cloud module) (Cloud is a core module for Cloud package)
    o-cloud_activity_audit
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
    +-cloud_server_templates

    x... Not released yet.
   

CHANGE HISTORY
==============
2011/12/21 7.x-1.x-dev released


Copyright
=========
Copyright (c) 2011-2012 DOCOMO Innovations, Inc.


End of README.txt
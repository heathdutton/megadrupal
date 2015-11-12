BASIC INFO
==========

- Cloud Metering module for configuring Instance, User Budget and User Group budgets and thresholds.
- Set the Instance Threshold limit and Initial Budget for Instance.
- Implements Drupal queue for the following:
  -- Sends a mail to the Site Admin, Group User about when the Instance current budget crosses the defined Instance Threshold precentage limit. 
  -- Creates backup of the Instances on crossing 100% of Instance Initial Budget
  -- Detaches the Volume (Storage) from the Instance
  -- Terminates the Instance.
  -- Sends a notification mail to the Group Admin or Owner of the Instance whenever the group or instance cost crosses budget/threshold.
- Works with Cloud module.


KNOWN ISSUES
============

- No known issue exists


HOW TO USE
==========

1) Enable Metering module
2) Update cloud information (by clicking '- Refresh Page -' link) or
   take any action (e.g. launch an instance, create a server template, etc.)  
3) To configure the User Budget, Threshold and Initial Budget value, Go to the menu: Site Administration | Cloud Metering


DIRECTORY STRUCTURE
===================

cloud
  +-modules (depends on Cloud module) (Cloud is a core module for Cloud package)
    +-cloud_activity_audit
    +-cloud_alerts
    x-cloud_auto_scaling
    +-cloud_billing
    +-cloud_cluster
    +-cloud_dashboard
    +-cloud_failover
    +-cloud_inputs
    o-cloud_metering
    +-cloud_monitoring
    +-cloud_pricing
    +-cloud_resource_allocator
    x-cloud_scaling_manager
    +-cloud_scripting
    +-cloud_server_templates


    x... Not released yet.


CHANGE HISTORY
==============
2011/12/21 ver.1.2  released 6.x-1.2


Copyright
=========

Copyright (c) 2012 DOCOMO Innovations, Inc.

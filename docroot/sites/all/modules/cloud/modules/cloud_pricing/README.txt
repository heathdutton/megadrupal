BASIC INFO
==========

- Provides pricing for hourly rate configuration for Billing module.
- You can set up pricing 
- Works with Cloud and Billing module.


HOW TO USE
==========

1) Enable Pricing module
2) Go to the menu: Administer | Site configuration | Amazon EC2, OpenStack, Eucalyptus, or XCP
3) Click 'Pricing Info tab'
4) Click 'New Pricing' to add a new pricing item
5( or edit the existing predefined pricing item(s)


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
    o-cloud_pricing
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
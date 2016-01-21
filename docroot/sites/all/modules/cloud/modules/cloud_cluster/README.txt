BASIC INFO
==========

- Provides cluster feature which enables to bundle server templates.
- Works with Cloud and Server Templates module.


HOW TO USE
==========

1) Enable Cluster module
2) Create server template(s)
3) Go to the menu: Design | Cluster | Create
4) Input Nickname and Description (and click 'Save' button)
5) Select a cluster item
6) Click 'Add Server' button
7) Select a server template
8) Enter nickname, select SSH Key, Security Group and Availability Zone
9) Click 'Add' button


DIRECTORY STRUCTURE
===================

cloud
  +-modules (depends on Cloud module) (Cloud is a core module for Cloud package)
    +-cloud_activity_audit
    +-cloud_alerts
    x-cloud_auto_scaling
    +-cloud_billing
    o-cloud_cluster
    +-cloud_dashboard
    +-cloud_failover
    +-cloud_inputs
    +-cloud_metering
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
2011/07/02 ver.1.1  released 6.x-1.1
2011/06/13 ver.1.01 released 6.x-1.01
2011/06/10 ver.1.0  released 6.x-1.0
2011/06/02 ver.0.92 released 6.x-1.x-dev
2011/04/05 ver.0.91 released to reviewing process of drupal.org
2011/03/24 ver.0.9  released to reviewing process of drupal.org
2011/01/29 ver.0.82 released to reviewing process of drupal.org
2010/12/26 ver.0.81 released to reviewing process of drupal.org
2010/12/15 ver.0.8  released to reviewing process of drupal.org


Copyright
=========

Copyright (c) 2010-2012 DOCOMO Innovations, Inc.

End of README.txt
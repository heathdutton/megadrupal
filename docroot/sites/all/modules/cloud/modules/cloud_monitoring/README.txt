BASIC INFO
==========
This module provides monitoring capabilities through an external monitoring system.
Out of the box, this module supports Collectd.  Additional monitoring systems can be
implemented using the pluggable architecture and hooks.

HOW TO USE
==========
- Enable cloud_monitoring module
- Go to Design > Monitoring Systems
  On this page, there are two options, "Add Existing Monitor" and "Launch a new Monitor".  Choose an option
  and fill out the configuration form.
  - Choose "Launch a new Monitor" to automatically launch a new cloud instance and install Collectd on it.  
    Clanavi will launch a new cloud instance using System Templates and then install Collectd using the cloud_scripting module.
  - Choose "Add Existing Monitor" to register a monitoring server that already has monitoring software on it.  Use this option
    if the monitoring software is manually installed.
- After setting up a monitoring server, associate a cloud instance to the monitoring server.
  - On any cloud instance, click the "Monitor" tab.  If the instance is already associated, the page will 
    display graphs with the monitoring data.
  - If the cloud instance is not associated, fill out the form and click "Submit".  
    On the form, there is an option "Check this box to install the monitoring software".  By selecting this, 
    cloud_monitoring will install the monitoring software and configure it to send data to the main monitoring 
    server.
  
DEVELOPER INFORMATION
=====================
cloud_monitoring was designed to allow multiple monitoring systems to be configured in Clanavi.

To implement a new monitoring system, there are a few steps.
- Implement hook_cloud_monitor().  This wil register your monitoring system's information, and class file locations to cloud_monitoring
- Extend the monitor_system class and implement all the abstract methods.  Look at the cloud_monitoring_system.inc to understand the
  abstract methods that needs to be implemented.
- When you clear your site cache, the new monitoring system will be registered and be available to use.

TODO
====
Future version will use CTool's plugin architecture, which supports recursive class loading and parent class definitions.


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
    +-cloud_metering
    o-cloud_monitoring
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

End of README.txt 

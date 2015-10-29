Version
=======
Cloud 7.x-1.x

BASIC INFO
==========

Cloud (a.k.a Clanavi) is a set of modules that lets users administer public and 
private clouds from within Drupal. Clanavi provide users the ability to manage public 
clouds such as AWS EC2 clouds as well as private clouds like OpenStack, Eucalyptus and XCP.

FEATURES
========
- Clanavi provides functionalites for cloud management, such as:
  * starting/stopping instances
  * managing volumes and snapshot
  * firewall management with security groups
  * SSH Key management
- Clanavi features a pluggable architecture and can be extended to support other 
public/private clouds.  
- Clanavi provides hooks for external modules to interact with instances when they
are stopped and started.  See cloud.api.php for more details.  

INSTALLATION: FOR USAGE WITH AMAZON EC2
=======================================
1. Download Libraries Module.
2. Download the AWS SDK for PHP from Amazon: http://pear.amazonwebservices.com/get/sdk-latest.zip
3. Copy the AWS SDK for PHP into "sites/all/libraries" as awssdk
  (Please create a specific path "sites/all/libraries" under your Drupal installation path).
   If the SDK library is not installed correctly, the module will not function properly.
   * NOTE: If you encounter a library-related error, please check the awssdk path at
           'sites/all/libraries', clear Drupal's cache (from Performance settings) and
            restart apache2.
3. Turn on the Cloud module
4. Turn on the following modules for EC2 management
  - Server Templates
  - AWS Cloud (Found under Iaas package)
  - Pricing
5. In order for the system to automatically update the database with EC2 data, cron must be running.  If cron is not running,
   data can still be imported using the "Refresh page" link at the bottom of listing pages

BASIC USAGE: AMAZON EC2
=======================
1. Download, install and enable all modules as described in the installation instructions
2. Go to admin/config/clouds to add an Amazon EC2 Region.  Click Add Cloud
3. Enter properly EC2 credentials and save
4. Upon successfully saving the credentials, click the link in the success message to download all EC2 data

Additional documentation on Clanavi can be found at: http://drupal.org/node/1855518

PERMISSONS
==========
Clanavi provides a very granular permission set for managing instances.  For example, certain user roles
can be configured to administer instances by giving them view, edit and delete permissions.  Other user roles
can be configured for read only.

e.g. (For an administrator)
- Turn on
  'administer cloud' and
  'access dashboard' permissions 
 
(For a generic users)
- Turn on
  'access dashboard' permission only

* Please see also cloud/modules/iaas/modules/aws_cloud/README.txt for AWS compatible cloud.  

SYSTEM REQUIREMENTS
===================
- PHP    5.2 or Higher
- MySQL  5.1 or Higher
- Drupal 7.x
- 512MB Memory: If you use AWS Cloud module, the running host of this
  system requires more than 512MB memory to download a list of images
 (because it's huge amount of data for listing).

DIRECTORY STRUCTURE
===================

cloud
  +-modules (depends on Cloud module) (Cloud is a core module for Cloud package)
    +-cloud_activity_audit
    +-cloud_alerts
    +-cloud_inputs
    +-cloud_pricing
    +-cloud_scripting
    +-cloud_server_templates
    +-modules
     -iaas
      - aws_cloud

hook_* FOR SUBSIDIARY MODULES
=============================
See cloud.api.php for more details on api functions and hooks

Copyright
=========
Copyright (c) 2011-2012 DOCOMO Innovations, Inc.

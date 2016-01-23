-------------------------------------------------------------------------------
Backup and Migrate OpenCloud
  by arpieb
-------------------------------------------------------------------------------

DESCRIPTION:
This module provides a destination for the Backup and Migrate module to utilize 
the OpenCloud API to talk to Rackspace and OpenStack storage systems.

-------------------------------------------------------------------------------

INSTALLATION:
* Put the module in your drupal modules directory and enable it

* Due to the fact that Rackspace has released their PHP library wrapper for 
  the OpenCloud API under the Apache License, Version 2.0, you will have to 
  install their code separately in order for this module to function.  The 
  code currently lives here:

  https://github.com/rackspace/php-opencloud

  Install it into sites/all/libraries/php-opencloud or another location that 
  the Libraries module is configured to search.

* For Rackspace Cloud Files, use the following configuration settings as of 
  Jun 23, 2013:

  Connection class: Rackspace
  Endpoint URL: https://identity.api.rackspacecloud.com/v2.0/ 
    (If a non-US account, check with support)
  Username: <Your account username>
  Password: <Your account password>
  Tenant Name: <If applicable, client on your account, otherwise leave blank>

  Container: <Cloud Files container name; will be created if it doesn't exist>
  Service name: cloudFiles
  Region identifier: <The three-letter ID shown for your service, e.g. Chicago 
    is ORD, Dallas is DFW, etc>
  URL type: publicURL (if running within a private cloud network, consult with 
    support for the proper value to use here)

* For another OpenStack-based provider, consult with your provider for the 
  proper values, noting that you are using the php-opencloud library.

-------------------------------------------------------------------------------


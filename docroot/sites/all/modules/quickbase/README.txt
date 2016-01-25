QuickBase API

This module does nothing by itself, but provides a wrapper for the QuickBase
PHP SDK to integrate Intuit QuickBase datastores into a Drupal site. There is
no need to install this module unless another module requires it or you are
interested in calling the QuickBase API from within your custom module(s).

It has the following dependencies:

* Libraries (https://drupal.org/project/libraries)
* Intuit QuickBase API - PHP Class Library (http://git.octobang.com/quickbase-api)
* PHP curl library

The QuickBase HTTP API reference is located here:

http://www.quickbase.com/api-guide/index.html

======================================================================

INSTALLATION

1. Install as you would a normal module, there are no external dependencies
   other than the PHP curl library must be installed (enforced by the module).
2. Configure the API calling information via admin/config/system/quickbase

======================================================================

PUBLIC API

There are also abstracted API calls that allows developers to query the service:

function quickbase()
  Returns a fully initialized instance of the QuickBase class from the SDK
  with credentials already set, or NULL if unable to intialize

======================================================================

PERMISSIONS

In order to prevent someone accidentally wiping the API credentials, the
following permissions are available and are disabled for all roles by
default, except user 1:

administer quickbase
  Allows users to configure the QuickBase API module's system settings
  located at admin/config/system/quickbase

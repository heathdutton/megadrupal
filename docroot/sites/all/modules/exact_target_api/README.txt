UMMARY --

The ExactTarget API module provides abstraction of the ExactTarget XML API for 
use by other modules. By itself, this module provides no functionality and 
should only be installed if another module requires it as a dependency.

For a full description of the module, visit the project page:
  http://drupal.org/project/exact_target_api

To submit bug reports and feature suggestions, or to track changes:
  http://drupal.org/project/issues/exact_target_api


-- REQUIREMENTS --

Existing ExactTarget account with XML API access.


-- INSTALLATION --

* Install as usual, see http://drupal.org/node/70151 for further information.


-- CONFIGURATION --

* Configure integration with ExactTarget API in Administration > Configuration >
  System > ExactTarget API:

  - Select ExactTarget API endpoint for the XML API calls. Possible options are:
      1. Normal Endpoint for customers using the S1 instance 
         (https://api.exacttarget.com/integrate.aspx)

      2. S4 Instance Endpoint for customers using the S4 instance 
         (https://api.s4.exacttarget.com/integrate.aspx)

      3. S6 Instance Endpoint for customers using the S6 instance 
         (https://api.s6.exacttarget.com/integrate.aspx)

  - ExactTarget API Username

    Username associated with ExactTarget account

  - ExactTarget API Password

    Password associated with ExactTarget account


-- CUSTOMIZATION --

Under the hood ExactTarget API uses cURL to make requests to the appropriate API 
endpoint.

* Additional cURL options such as proxy or cert can be set using 
  hook_exact_target_api_curl_alter(). See http://drupal.org/node/1830656 for more info.




-- CONTACT --

Current maintainers:
* Katrin Silvius (nonsie) - http://drupal.org/user/29899
* Robert Bates (arpieb) - http://drupal.org/user/533416


This project has been sponsored by:
* Cool Blue Interactive
  Visit http://www.coolbluei.com/ for more information.

* Live Axle Interactive
  We are makers of interactive digital experiences.
  Visit http://www.liveaxle.com for more information.

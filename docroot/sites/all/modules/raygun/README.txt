Raygun.io
=========

Allows all errors and exceptions to be sent to Raygun.io so they are tracked on
their dashboard. For the full feature list see the feature list page on
http://raygun.io/features.

Module features
---------------

* Simple one stop configuration screen
* No coding required
* Can take over the PHP error and exception handler to catch all errors
* hook_requirements() integration to ensure you have downloaded the Raygun4Php
  library

Module requirements
-------------------

* You have an API key for your application at Raygun.io
* PHP 5.3+
* You have downloaded the raygun4php library (found at
  https://github.com/MindscapeHQ/raygun4php) to your `sites/all/libraries`
  directory under a sub folder `raygun`.

  The directory should look like:

  sites/all/libraries
  -- raygun
  ---- RaygunClient.php
  ---- RaygunEnvironmentMessage.php
  ...

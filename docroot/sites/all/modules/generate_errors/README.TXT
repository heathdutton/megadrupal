INTRODUCTION
------------

Current Maintainer: Jon Peck <jpeck@fluxsauce.com>

Generate Errors is a module that provides an user interface for developers to
manually generate errors. There are four options:

* Select or manually specify HTTP status code in the response header and
  optionally exit
* Throw an uncaught PHP Exception
* Exhaust memory in an infinite loop
* Trigger a PHP error level, including E_ERROR, E_WARNING, E_PARSE, E_NOTICE,
  E_USER_ERROR, E_USER_WARNING, E_USER_NOTICE, E_STRICT, and E_RECOVERABLE_ERROR

The selected error will occur only once, allowing a developer or systems
administrator to test system behaviors, such as custom error messages from the
server and so forth. Activity is logged in watchdog.

This module should not be deployed in any public-facing environment and is
intended for development and testing purposes only.

INSTALLATION
------------

Install the module like you would other Drupal modules. This module contains
no permissions or roles and allows any user - anonymous or authenticated - to
generate errors.

USAGE
-----

Generate Errors adds a normal menu item to /generate_errors - use the new link
in the menu or navigate directly to the path.

Choose the error that you wish to generate and click the associated submit
button.

A status message will be shown describing the generated error, and a log message
will be recorded in watchdog prior to the triggered error to document who did
what and when. Additional watchdog log entries may be created by the
generated error itself, depending on your system configuration.

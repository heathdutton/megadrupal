JSerror module provides easy way for logging an tracking JavaScript errors.

Tracking JavaScript errors can be painful. JS code that works on your browser 
might not necessarily work on every users browsers. This module records error 
as an when the appear on any browser and sends the data back to the server.

JSerror sends a small JavaScript snippet to the browser before any other 
JavaScript file, which collects all the errors and sends the data back to
the server, where the errors are logged in the database.

Requirements
------------

JSerror requires Browscap to extrack browser and platform information from the 
UserAgent string.

Installation
------------

JSerror can be installed via the standard Drupal installation process.
http://drupal.org/node/895232

Note
----

In case anonymous user has the permission 'access site reports',
anonymous page caching will be disabled. This might bring some performance
overhead. Hence, it is advisable not to grant 'access site reports' permission
to anonymous users.

Configurations
--------------

* Goto admin/config/development/jserror

* Check "Subpress warnings in console" if the errors are to be suppressed from 
  debugger console. Errors will no longer appear in a browser's JavaScript 
  console. Useful on production websites.
  
* Page request limit is the number of error requests per page a client can send
  to the server. Default is unlimited.
  
* Client request limit is the number of overall requests a client can send to
  the server in one hour. Default is unlimited.

* JavaScript log messages to keep, is the number of database entries to 
  maintain. Entries beyond this number will be truncated on cron run.

* Errors can be viewed at admin/reports/jserror

Credits / Contact
-----------------
Currently maintained by Abhishek Anand.

The best way to contact the authors is to submit an issue, be it a support
request, a feature request or a bug report, in the project issue queue:
  http://drupal.org/project/issues/jserror

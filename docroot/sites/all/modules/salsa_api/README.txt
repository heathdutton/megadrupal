
CONTENTS OF THIS FILE
---------------------

 * Introduction
 * Requirements
 * Installation
 * Configuration
 * Developers
 * For More Information


INTRODUCTION
------------

Salsa API is a module that allows Drupal to communicate with the Salsa CRM from
Salsa Labs (http://www.salsalabs.com). Note that Salsa API does not provide any
useful Salsa/Drupal integration on its own. It merely provides the API
functionality required by other modules, such as Salsa Entity
(http://drupal.org/project/salsa_entity).


REQUIREMENTS
------------

Salsa API requires login credentials for a Salsa campaign manager, and a web
server that supports cURL.


INSTALLATION
------------

Install Salsa API as you would any other Drupal module. If you need help, see
http://drupal.org/documentation/install/modules-themes/modules-7 (Drupal 7) or
http://drupal.org/documentation/install/modules-themes/modules-5-6 (Drupal 6).


CONFIGURATION
-------------

Configure Salsa API at admin/config/services/salsa (Configuration >> Web
Services >> Salsa).

In order to connect to your Salsa account, you must enter the Salsa API URL,
username, and password for your Salsa campaign manager.

For the "URL to Salsa API" field, enter the Salsa hostname that your
organization's Salsa HQ is on. For example, if you log in to Salsa with the URL
https://hq-org2.democracyinaction.org/dia/hq/login.jsp, enter
"https://hq-org2.democracyinaction.org" (no trailing slash) in this field.

In the "Campaign Manager Username" and "Campaign Manager Password" fields, enter
the username and password of the campaign manager you wish Drupal to use to
connect to your Salsa account. It is recommended that you create a new campaign
manager in your Salsa HQ for the exclusive use of Drupal.

Optionally, you may also enter a query timeout. Salsa API queries that run for
longer than this value will fail. The default setting is 10 seconds.



DEVELOPERS
----------

For information about using Salsa API with your own module, see DEVELOPERS.txt.
Note that the salsa_api_query() function is in the process of being phased out
in favor of the new SalsaAPI class, and will soon be marked as deprecated. See
the comments in salsa_api.inc and the issue queue for more details, or if you'd
like to help converting salsa_api_query() into methods in the new class.



FOR MORE INFORMATION
--------------------

 * Project Page: http://drupal.org/project/salsa_api
 * Issue Queue: http://drupal.org/project/issues/salsa_api
 * Salsa Labs External API documentation: http://www.salsalabs.com/devs/docs/api

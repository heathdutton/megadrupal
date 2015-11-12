README.txt - netFORUM

Thanks to everyone at Avectra for helping out with my questions on this, 
including but not limited to Kerim Guc, Darryl Hopkins, Devin Dasbach, and Ford Parsons

The netFORUM module is desiged to provide a common platform for pushing and pulling data via xWeb.

xWeb is a part of the netFORUM product created by Avectra for asociation management, and exists as an XML
over SOAP based service.  Requests are cached to the Drupal database for later retrieval and in the event that the
xWeb service is not available.

The module was developed against netFORUM build 2006.02.  If any problems are found using different versions 
please submit a ticket

The netFORUM only works with xWeb Secure and does not implement HTTP Auth security.  To use this module you must have:

xWeb Secure running without directory security on IIS (at least not passwords)
PHP 5
Drupal 5
Write access for the xWeb user is recommended.
SimpleXML enabled on your Drupal server
SOAP enabled on your Drupal server

Check for SimpleXML and SOAP by examining the output from phpinfo();

Create the xWeb user like adding a regular user, but using SQL manager set the usr_pwd.  For example:
UPDATE fw_user SET usr_pwd = 'testXwebPassword' WHERE usr_code = 'DrupalxWebUser'

Read more about how to set up and configure netforum client at
admin/help/netforum

WARNINGS:

The netforum_object_cache table is configured with longtext columns to store the results which should be PLENTY of room,
but by default MySQL will only allow packets up to 1MB in size.  If you use queries that return large amounts of data
be sure to increase the max_allowed_packet size.  You may need to add a line to your my.cnf file, something like :

max_allowed_packet = 10M


Sometimes PHP does not get along nicely with Microsofts IIS when using HTTPS for communication, 
this is described at http://us2.php.net/manual/en/wrappers.http.php .  The netFORUM module hides all 
errors and warnings when making requests in favor of throwing it's own.

The default timeout for a request to xWeb is set by the default_socket_timeout setting in php.ini, which is usually
60 seconds.  That's a long time to make a user wait, so there is functionality for verifying that xWeb is avaiable before requests.
If you wish to disable that, changing the default_socket_timeout is recommended.

PHP caches the SOAP WSDL, which for us is a good thing.  The WSDL describes all of the functions available for us.  
This is unlikely to change so a high value is recommended.  If xWeb is unavailable then best attempts are made to return 
values, but if the WSDL is unavailable then no attempt can be made.  That means that if xWeb is unavailable for longer 
than the WSDL is cached for, you will run into problems.  Set this in the php.ini file, look for the soap.wsdl_cache_ttl option.

When retreiving a list of object names and details for those objects, there can be a mismatch between what you can view, 
and what you can issue a GetQuery request for.  The netFORUM module parses GetFacadeXMLSchema for a more complete
list of data available for viewing.  Read more at:
http://wiki.avectra.com/index.php?title=XWeb:GetQuery#GetQuery_Does_Not_Recognize_Columns_from_GetFacadeXMLSchema

DEVELOPING:
The code is all documented using the Drupal standards and the Drupal API module can provide them for you.  There are some
helper functions that will create XML suitable for Insert and Update operations from arrays.  If you are having problems
make sure that the case of your request is correct.  GetFacadeXMLSchema is correct, GetFacadeXmlSchema will return nothing.
Check the logs to see if you are getting no response to your queries, and if so test it at admin/settings/netforum/xwebtest

The key function and most likely to be used is netforum_xweb_request(), and netforum_is_empty_guid() is pretty handy too.

Note that only some requests are cached, for many operations caching doesn't make sense.  The cached functions are declared
in xwebSecureClient.class.inc and is set with the $cachedFunctions variable.  Currently the following function responses are cached:
GetDynamicQuery,GetFacadeObject,GetIndividualInformation,GetOrganizationInformation,GetQuery


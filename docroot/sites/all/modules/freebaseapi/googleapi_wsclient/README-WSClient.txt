Manual configuration
====================

Freebase is a REST service and you can configure it yourself without even using 
this module to provide the presets. Just use wsclient on its own instead.

drush install wsclient http_client
drush enable wsclient wsclient_rest http_client

* Visit /admin/config/services/wsclient/add
* Create 
  Label: "Freebase"
  URL: "https://www.googleapis.com/freebase/v1/"
  Type: REST
* Add operation 
  Label: "Topic Lookup"
  Name: "topic"
  HTTP Method: GET
  Path: "topic@id" (no /, as freebase IDs include a leading /)
  Parameter: 
    Data type: text
    Multiple: no
    Name: id
    Required: yes
  The other parameters are not required. However they are documented at
  https://developers.google.com/freebase/v1/topic 
  and you can add them as needed.
  Result type: (text) for now.

Test
----

* Enable wsclient tester
* Visit 
  /admin/config/services/wsclient/manage/freebase_topic_lookup/operation/topic/test
  and add '/en/bob_dylan' as the id
* You should see a bunch of info come back.
* As the response was json, that big string will also have been parsed into a 
  PHP struct for you. If you have devel.module on, you can expand the structure
  also. This data is now available to use in your own application.

Troubleshooting
---------------

If you get the error 

  Error invoking the REST service Freebase, 
  operation topic: 
  Curl Error: SSL certificate problem, verify that the CA cert is OK. 
  Details:
  error:14090086:SSL routines:SSL3_GET_SERVER_CERTIFICATE:certificate verify failed
  
  There are two options:
  * Disable SSL verification (bad)
  * Download and use the right certificate (tricky)
  
Option B:
http://snippets.webaware.com.au/howto/stop-turning-off-curlopt_ssl_verifypeer-and-fix-your-php-config/

On clean OSX, find the file inside the Google API library
  google-api-php-client/src/io/cacerts.pem
  Open it - that should launch "keychain access"
  Trust it.

On OSX with Acquia desktop 
  cp sites/all/libraries/google-api-php-client/src/io/cacerts.pem \
   /Applications/acquia-drupal/common/
  Edit your php.ini file to add
  [Curl]
    curl.cainfo=/Applications/acquia-drupal/common/cacerts.pem
  Restart the Apache server. Done!

or Mamp, do something similar.

On a Linux server with Apache2 
  You should find that enough signed certificates are already in
  /etc/ssl/certs/
  and things should be up to date. If not, 
  Edit your php.ini file to add
  curl.cainfo=/path/to/ca-bundle.crt 
  eg
  /var/www/sites/all/libraries/google-api-php-client/src/io/cacerts.pem

  
  
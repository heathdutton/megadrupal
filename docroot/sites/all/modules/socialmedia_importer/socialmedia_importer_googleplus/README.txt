CONTENTS OF THIS FILE
---------------------

  * Summary
  * Requirements
  * Google Configuration

SUMMARY
-------

Googleplus Importer is a developer only module that integrate Googleplus´s 
API in Drupal. This module allows you to authenticate with this API and
use this authentication to Import Googleplus Data in JSON.

REQUIREMENTS
------------

1. Social Media Importer Module - This modules depends on Social Media Importer
   Module. 


GOOGLE CONFIGURATION
--------------------
1. Visit https://code.google.com/apis/console

2. Create a new project with appropriate details,
   if you don't have a project created.
   
3. Under "Services" tab enable googleplus service

4. Open "API Access" tab.

5. If you have not created a oauth2.0 client id then create it
   with appropriate details i.e. name, etc
   
6. Then on next screen select "Application type" web Application.

7. Provide your hostname.

8. Edit the Client settings and change the redirect uris to
   "http://example.com/socialmedia_importer/response_handler" and update.
   
9. Copy the client id, client secret
   to add Application form of the module.

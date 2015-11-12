Installation and Configuration

- Download and install the Libraries (7.x-2.x branch) module
  http://drupal.org/project/libraries

- Download google-api-php-client
  (http://code.google.com/p/google-api-php-client/) and place it in
  sites/all/libraries/google-api-php-client/ so that the path to apiClient.php is
  sites/all/libraries/google-api-php-client/src/apiClient.php

  google-api-php-client requires OpenSSL and fileinfo.

  NOTE: The zip package released on March 6, 2012 does not have the correct authentication
  functions to work with this module. It is recommended to get the latest version
  of google-api-php-client by checking out the SVN repository.

- Configure your setttings at /admin/config/media/google-cloud-storage

- Visit admin/config/media/file-system and set the Default download method to
  Google Cloud Storage

- Add a field of type File or Image etc and set the Upload destination to
  Google Cloud Storage in the Field Settings tab.


Google Cloud Storage Setup

- Create a Project (or edit existing project)
  https://code.google.com/apis/console

- (Services) Enable the Google Cloud Storage service

- (API Access) Create a Client ID (Type = Service)
  Download the private key, and upload to a safe and private location on the Drupal server.

For more information about obtaining client id or private key, visit:
https://developers.google.com/console/help/#service_accounts
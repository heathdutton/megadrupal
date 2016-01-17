The Facebook Photo Sync module enables the ability to have Image fields
automatically uploaded to a dynamically generated Album upon Entity creation.

Facebook Photo Sync was written and is maintained by Stuart Clark (deciphered)
of Realityloop Pty Ltd.
- http://www.realityloop.com


Features
--------------------------------------------------------------------------------

- Full entity support.
- Support for standard facebook accounts and facebook Pages.
- Dynamically generate new albums with token based name/description.
- Token based description for upload images.
- Ability to set maximum amount of images uploaded per ImageField.
- Features exportable.


Required Modules
--------------------------------------------------------------------------------

* Libraries API - http://drupal.org/project/libraries
* Token         - http://drupal.org/project/token


Setup and installation
--------------------------------------------------------------------------------

1. Create a facebook app: https://developers.facebook.com/apps
   Be sure to set the App Domain and Website to the same value, if you're
   testing locally (http://something.localhost) you must use your local
   domain (http://something.localhost).

2. Download and extract the Facebook PHP SDK (https://github.com/facebook/facebook-php-sdk)
   to your libraries directory as facebook-php-sdk.
     e.g., sites/all/libraries/facebook-php-sdk

3. Install and enable the module as per usual, or follow the instructions at
   http://drupal.org/node/895232

4. Enter your Facebook app ID and secret at the modules configuration page
   (Administration > Configuration > Web services > Facebook Photo Sync).

5. Once you've saved the Facebook app ID and secret you will be prompted to
   login to facebook (if you aren't already) and authorize your application.

6. Image field settings for Facebook Photo Sync can be found on the specific
   Image field settings forms.


Todo
--------------------------------------------------------------------------------

- Add more documentation.

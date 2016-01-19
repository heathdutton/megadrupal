Introduction
============
The module right now only allows to see your listings of your Wunderlist tasks

What to expect in the near future:
- update tasks
- delete tasks

Requirements
============
This module requires the following modules:
 * OAuth2 Client (https://www.drupal.org/project/oauth2_client)
 * Views module (https://www.drupal.org/project/views)

Installation
============
OAuth2 Client module is required for all requests to the Wunderlist API.
You can download the OAuth2 Client module at
http://drupal.org/project/oauth2_client
Copy the OAuth2 Client files and the Wunderlist files in your Drupal sites/all/
modules/ directory.

Configuration
=============
Once OAuth2 Client and Wunderlist have been enabled, go to
admin/config/services/wunderlist and:
- Click on the link "register your application" to get your OAuth Client id and
OAuth Client secret.
- Fill in your OAuth Client id and OAuth Client secret in the corresponding
form fields and press the "Save configuration" button
- A warning will appear with the link you need to click to allow
your application to access your tasks.
- Now you need to run the cron and visit /wunderlist to see your
Wunderlist tasks.

You can configure user permissions in Administration » People » Permissions:
- Administer Wunderlist: Allow access to the configuration page
- Access Wunderlist content: Allow access to the /wunderlist page

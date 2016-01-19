INTRODUCTION
------------
This module provides integration with Nimble CRM. Each time someone submits a
contact form response on your website, a new Contact will be added to your
Nimble CRM account. It has integration with the core Contact module and Webform
module.

 * For a full description of the module, visit the project page:
   https://www.drupal.org/sandbox/ribel/2417261

 * To submit bug reports and feature suggestions, or to track changes:
   https://www.drupal.org/project/issues/2417261


REQUIREMENTS
------------
This module requires the following modules:
 * OAuth2 Client (https://www.drupal.org/project/oauth2_client)

This module requires Nimble API access:
 * http://support.nimble.com/customer/portal/articles/1194074-nimble-api-access


RECOMMENDED MODULES
-------------------
 * Webform (https://www.drupal.org/project/webform)


INSTALLATION
 ------------
 1. You need to get the API keys from Nimble.
 For Nimble API Access, please email api-support@nimble.com and include the
 following:

 * App Name: Contact exporting tool for [your site name]
 * App Description: Creates contact on Nimble CRM account when contact form
   is submitted.
 * Redirect URL for oAuth callback: [your site full path]/oauth2/authorized

 2. After you get the Client ID and Client Secret keys, enter them on the
 configuration page. You will be redirected to the Nimble login page.
 Log in to your account and confirm API access to this app. After that, you will
 be redirected back to your site.

 3. Now your can enable integration with contact form or any webform on your
 site.

 4. If you use webform, don’t forget to map your components with Nimble fields.

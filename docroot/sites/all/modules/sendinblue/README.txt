INTRODUCTION
-------------
This module provides integration with the SendinBlue email
delivery service. This module strive to be simple, management
user-friendly, in order to help developers quickly and easily
combine with SendinBlue email delivery service.

The Main Features are followings:
  * API integration with the API Key of SendinBlue API.
  * Send transaction email by using the SMTP server of Sendinblue.
  * Support the unlimited management of Lists, Campaigns,
    Statistics of SendinBlue.
  * Have anonymous sign up forms to subscribe site visitors
    to any combination of SendinBlue lists.
  * Compatibility with Views Bulk Operations.
  * Standalone subscribe and unsubscribe forms.
  * Subscription forms can be created as pages or as blocks,
    with one or more list subscriptions on a single form.
  * Include merge fields on anonymous subscription forms.

REQUIREMENTS
-------------
This module requires the following modules:
   * Libraries (https://www.drupal.org/project/libraries)
   * Entity (https://www.drupal.org/project/entity)

INSTALLATION
-------------
  * You need to install & enable Libraries, Entity modules
  * Go to "Administer" -> "Modules" and enable the module.
  * You need to have the API Key of SendinBlue API.
  * You need to login with the API Key on Home page
    in order to use all functionality of module.

CONFIGURATION
-------------
  * Direct your browser to admin/config/system/sendinblue to configure the
     module.
  * You will need to put in your API Key of SendinBlue API
     for your SendinBlue account.
     If you do not have a SendinBlue account, go to
     [https://www.sendinblue.com/]([https://www.sendinblue.com/) and sign up
     for a new account. Once you have set up your account and are logged in,
     visit: [https://my.sendinblue.com/advanced/apikey/]
            ([https://my.sendinblue.com/advanced/apikey/)
     to generate a key.
  * Copy your newly created API key and go to the
     [SendinBlue config](http://example.com/admin/config/system/sendinblue)
     page in your Drupal site and paste it into the SendinBlue Access &
     Secret Key field.
  * You can manage lists, campaigns, statistics of SendinBlue on following
    pages:
     [Lists](http://example.com/admin/config/system/sendinblue/lists)
     [Campaigns](http://example.com/admin/config/system/sendinblue/campaigns)
     [Statistics](http://example.com/admin/config/system/sendinblue/statistics)
  * To add subscription form, you will need to visit:
     [Forms](http://example.com/admin/config/system/sendinblue/forms)
     You can add & delete and edit the forms easily.

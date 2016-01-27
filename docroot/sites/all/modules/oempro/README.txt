This module provides integration with the Oempro email delivery service.
While tools for sending email from your own server from within Drupal, like
SimpleNews, are great, they lack the sophistication and ease of use of dedicated
email services like Oempro.
Other players in the field are SimpleNews, Constant Contact, Campaign Monitor
and MailChimp.

oempro.module provides provides basic configuration and API integration. 
Specific functionality is provided by a submodules that depend upon 
oempro.module. See its README's for more details.

## Features
  * API integration
  * Support for an unlimited number of mailing lists
  * Having an anonymous sign up form to enroll users in a general newsletter.
  * Each Oempro list can be assigned to one or more roles
  * Editing of user list subscriptions on the user's edit page
  * Allow users to subscribe during registration
  * Map token and profile values to your Oempro merge fields
  * Required, optional, and free form list types.
  * Standalone subscribe and unsubscribe forms
  * Subscriptions can be maintained via cron or in real time
  * Individual blocks for each newsletter
  * Create campaigns containing any Drupal entity as content, send them, and
    view statisitcs.

## Installation Notes
  * You need to have a Oempro Installation configured and online.
  * You need to have at least one list created in Oempro to use the
    oempro_list module.

## Configuration
  1. Direct your browser to http://example.com/admin/config/services/oempro 
  to configure the module.

  2. You will need to put in your Oempro user details for your Oempro account.
  If you do not have a Oempro installation, go to 
  [http://http://octeth.com/]([http://octeth.com/) to purchase the software.

  3. Copy your API URL and go to the 
  [Oempro config](http://example.com/admin/config/services/oempro) page in 
  your Drupal site and paste it into the Oempro API URL field.
  Oempro username and password - Enter the Oempro user login details of the 
  user you wish to use to connect to the Oempro instance.

## Submodules
  * oempro_lists: Synchronise Drupal users with Oempro lists and allow
    users to subscribe, unsubscribe, and update member information.

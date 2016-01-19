Webform Google Exporter
=======================

This module adds a webform content type to your Drupal site.
A webform can be a questionnaire, contact or request form. These can be used 
by visitor to make contact or to enable a more complex survey than polls
provide. Submissions from a webform are saved in a database table and 
can optionally be mailed to e-mail addresses upon submission.

Requirements
------------
Webform 4.x-beta1 or higher

Installation
------------
1. Copy the entire webform_google_exporter directory the Drupal
   sites/all/modules directory.

2. Login as an administrator. Enable the module in the "Administer" -> "Modules"

3. You'll need to create an "API Project" on Google servers to allow users to
   authenticate. Follow the steps provided by Google on how to do this:
   https://developers.google.com/drive/auth/web-server#create_a_client_id_and_client_secret

   Under the "Services" tab, be sure to enable the following services:

   - Drive API

   Under the "API Access" tab, create a "Client ID" for a Web Application. Click
   the "More options" link when setting up a Client ID. Under the "Authorized
   Redirect URIs" field, enter a value such as:

   http://example.com/webform/google-exporter

   CAVEAT:

   Google requires your site have a publicly accessible URL in order to use the
   API, however if you want to test on a localhost, you can get around this
   problem by editing your HOSTS file on your computer to have a URL that seems
   public. For example you could make an entry like this:

   127.0.0.1 example.com

   Then use "mysite.com" when registering the Client ID in the "Authorized
   Redirect URIs" field.

3. After creating the necessary API Project, copy the values for "Client ID" and
   "Client secret" and save these values in the Webform Google Exporter settings
   on your Drupal site.

   This module adds new options to the normal Webform settings page. Edit the
   Webform Google Exporter settings under "Administer" -> "Configuration" ->
   "Content authoring" -> "Webform settings" inside the fieldset for
   "Webform Google Exporter".

5. When exporting a webform's results (under the "Results" -> "Download" tab on
   a webform) a new option for "Google Drive" will be shown. Select this option
   when creating the export. After the export is generated, you will be asked
   to login and then grant permissions to create the file. After allowing
   access, the file will be opened in your browser in Google Drive.

Support
-------
Please use the issue queue for filing bugs with this module at
http://drupal.org/project/issues/webform_google_exporter


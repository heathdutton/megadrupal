------------------------------
Ubercart Webform Checkout Pane
------------------------------

Originally provided by Jonah Ellison
Maintained by Martin B. - martin@webscio.net
Supported by JoyGroup - http://www.joygroup.nl


Introduction
------------
This module allows you to define Webform nodes as checkout panes in Ubercart.

This is useful if you want to collect additional information during the checkout process. For example, you may
want a quick survey or require additional fields that apply to the entire order.

Using the Webform module allows for many advantages: the forms/fields can easily be modified by an end-user; less
development time is required to create database tables and code to store/retrieve the data; and the form will
remember the user's previous answers.

Furthermore, this module now provides tokens for every field in each of your enabled webforms, making it easy for
you to insert the submitted values into confirmation emails, invoices, etc.


Installation
------------
 * Copy the module's directory to your modules directory and activate the module.


Usage
-----
 * Every Webform node on your site can be activated as a Ubercart checkout pane. In order to do so, go to the
 'Webform' tab of your Webform node, then select the 'Form Settings' secondary tab and then enable the 'Generate a
 checkout pane' setting in the 'Advanced settings' fieldset which usually appears in collapsed mode at the bottom of
 the page. (See also the 'Permissions' section below for more details on the accessibility of the mentioned setting).

 * The selected webforms will now be listed under admin/store/settings/checkout/panes where you can enable,
 disable and move around checkout panes. These act as normal Ubercart panes from here on in.

 * You can view/edit webform submissions relevant to the orders in your store directly through the orders, no need
 to go looking for the correct webform submission anywhere else.

 * The module provides full token support for the webform nodes acting as Ubercart checkout panes. To
 see a full list of available tokens, go to admin/help/token (requires Token module) and look for your webform
 node titles under the 'Orders' section. Additionally, an example template file (uc_order-uc_webform_pane.tpl.php) is
 also provided by the module which shows how to dynamically add all tokens from all webforms to your invoice template.


Permissions
------------
 * If you want to enable a webform to be a checkout pane, you need the 'administer store' permission.

 * Once a webform is acting as a checkout pane, everything in the admin interface provided by the Webform module
 becomes restricted to the users who don't have the 'administer store' permission, i.e. users without that permission
 will not be able to edit/delete the webform, add/edit/delete webform components, or modify webform submissions.

 * Note that users with the corresponding Webform module permissions will still be able to view the pages for the
 above actions, but they will not be able to submit any changes. (This is not ideal, but due to Drupal's inability
 to restrict permissions once they have been granted, this is the only feasible solution for now.)


Translations
------------
 * It is recommended to use the Locale module to translate all parts of your webform, including title, description
 and field names.

 * You can also create a copy node in your target language using the "translate" interface of a webform, but note
 that this will pretty much mean building a completely new form. You will have to re-add all fields, enable it as
 a checkout pane, and the submissions will be stored with a separate form id. If all of this doesn't bother you,
 there are no real technical issues, so you can safely go ahead with this approach.


Notes
-----
 * For Views integration see http://drupal.org/node/680386.

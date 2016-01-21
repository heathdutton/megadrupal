Eloqua Subscription Center
===========================

This Drupal extension adds a page to Drupal where Eloqua contacts can manage
their e-mail group subscriptions. The primary use-case is linking to this
"subscription center" page from the footer of all outbound e-mails from Eloqua.

## Features
* Configure the path at which the subscription center is accessed,
* Configure a subset of e-mail groups available to Contacts for subscription
  management,
* Optionally provide a confirmation message or redirect to a configurable
  location after a Contact manages his or her subscription preferences,
* Override (or keep) the default e-mail group names and descriptions as they
  exist in Eloqua,
* Link authenticated users directly to their subscription center page (based on
  their user e-mail address). No need to link directly from an e-mail.
* Full, optional integration with [the i18n module]() for localization needs.

## Installation
1. Install this module and its dependencies, [Eloqua REST API]() and
   [Eloqua API](), via drush:
  `drush dl eloqua_subscription_center eloqua_rest_api eloqua_api`
2. Enable Eloqua REST API: `drush en eloqua_rest_api`. Note, this module
   requires special attention during initial install/enable. Refer to
   [its installation instructions]() for details.
3. Then enable this module: `drush en eloqua_subscription_center`

## General Configuration
1. Ensure your user has the permission `manage eloqua subscription center`.
2. Navigate to `admin/config/system/eloqua-subscription-center`.
3. Optionally, configure your desired "subscription center path."
   * Defaulting setting is `subscription-center/[#email_domain]/[#contact_id]`,
     but you can provide your own path, as long as the path doesn't already
     exist, and the path you choose includes both tokens. An example might be:
     `preferences/manage/[#email_domain]/[#contact_id]/edit`
   * For more details, see the "Usage" section below.
4. Optionally, configure your desired "subscription center title." This is the
   title of the page where Contacts manage their subscription preferences. This
   defaults to `Subscriptions`.
5. This module assumes subscription management is handled in processing steps on
   a specific form in Eloqua. You most provide the HTML form name of that Eloqua
   form in the "Eloqua form name" field. For more details, see the "Usage"
   section below.
6. Optionally, configure what the "Default subscription status" of a contact is.
   This value is used when no subscription status (neither "subscribed" nor
   "unsubscribed") is found for a given contact in a given e-mail group. By
   default, "Subscribed" is checked, meaning Contacts are assumed to be
   subscribed to groups, even if this value isn't explicitly returned by Eloqua.
7. Optionally, check the "Hide e-mail" checkbox to visually hide the e-mail
   address from the Contact on the subscription management page. This can be
   useful in cases where you do not wish to show e-mail addresses to users for
   privacy reasons.
8. Optionally, configure form actions. Form actions are events that happen after
   a Contact successfully fills out the subscription management form. You can
   trigger a confirmation message, or redirect to a specific page.
   * Check "Display a confirmation message" to specify a confirmation message
     to be shown to Contacts. This will be shown the same way any Drupal message
     is shown / styled.
   * Check "Redirect to a confirmation page" to move Contacts to a special
     "thank you" page. For example, you could specify `node/123` to redirect to
     Node 123.
   * Note, you can use both at the same time (to force a confirmation message to
     show on a confirmation page), or neither (which results in the Contact
     seeing their preferences changed immediately).

## E-mail Group Configuration
The first time you install / configure this module, you won't see any e-mail
groups from Eloqua. To load them in, click the "Pull latest groups from Eloqua"
button at the bottom of the module configuration page.

Use this same button any time you add e-mail groups to Eloqua that you want to
manage in Drupal.

1. Once you've loaded in e-mail groups from Eloqua, you should see a vertical
   tab set of e-mail groups at the bottom of the module configuration page.
2. To allow a Contact to manage their subscription to an e-mail group, click in
   to the group and check the "allow subscription management" checkbox.
3. The names and descriptions from Eloqua are used to pre-populate those values
   within Drupal, but you can optionally override those values by editing the
   "Display title" and "Display description" fields for each e-mail group.
4. The "form element name" field defaults to the ID of the e-mail group in
   Eloqua. This field is used as the HTML form input name when sending the
   Contact's updated subscription preferences. It should match the field
   attached to the form in Eloqua used to process a "subscribe" or "unsubscribe"
   action. For more details, see the "Usage" section below.
5. You can also specify the order in which e-mail groups are displayed to
   Contacts on the subscription management page by editing the "Order" field for
   each e-mail group.

## Usage

The higher-level architecture and workflow for this module and how it integrates
with Eloqua are as follows:

#### From Eloqua's perspective
* E-mail groups are defined and managed in Eloqua to manage contact
  subscriptions to designated types of e-mail communication.
* A form exists, of any name (e.g. "drupal_subscriptions"), that's used
  exclusively to update contact e-mail group subscription values. Provide this
  form name to the Drupal administrator.
* This form has several fields:
  * "email": Used to associate subscription management with a contact.
  * And any number of checkbox fields of any name, each one corresponding to an
    e-mail group. For example, you might have a "Monthly newsletter" e-mail
    group for which you want to allow contacts to manage their preferences. For
    this group, you'd add a checkbox field whose HTML name was something like
    "newsletter."
  * You can have as many or as few of these e-mail group checkbox fields as
    needed. Provide these HTML field names (and their associated groups) to the
    Drupal administrator.
* This form has processing steps that, based on the values passed in with a form
  submission, either subscribe or unsubscribe the contact (specified by the
  "email" field) to the group (codified in the form names specified above).
* In the footer of every (or just relevant) e-mail(s) sent from Eloqua, you link
  to a page in Drupal. This link will need to include tokens for the Contact's
  e-mail domain, as well as the Contact's ID. Link text might be something like
  "manage my subscriptions." Talk to the Drupal administrator for the exact
  path that should be linked to here.

#### From Drupal's perspective
* E-mail groups are pulled from Eloqua into Drupal using Eloqua's REST API.
* An administrator in Drupal configures which e-mail groups are relevant for
  contacts to manage, marking each with "allow subscription management."
* For each relevant e-mail group, provide a "form element name." This is the
  HTML field name associated with the e-mail group on the subscription form in
  Eloqua. Talk to the Eloqua administrator for details.
* An "eloqua form name" must also be configured; this should be the name of the
  Eloqua form used to manage subscriptions. Talk to the Eloqua administrator for
  details.
* An administrator in Drupal also configures the path at which a subscription
  management page is available, defaulting to
  `example.com/subscription-center/[#email_domain]/[#contact_id]`. Provide this
  path to the Eloqua administrator to include in e-mail footers.

Note, both an e-mail domain and contact ID must be used in the subscription
center path to reduce as much as possible the ability for prying eyes to iterate
through all of your contacts and pull down their e-mail addresses.

Authenticated Drupal users can access their subscription center preferences at
any time by visiting, for instance, http://example.com/my-subscription-center.
This special path will automatically detect the Eloqua contact ID and e-mail
domain based on the authenticated user's e-mail address and redirect the user
to his/her subscriptions.

[the i18n module]: https://drupal.org/project/i18n
[Eloqua REST API]: https://drupal.org/project/eloqua_rest_api
[Eloqua API]: https://drupal.org/project/eloqua_api
[its installation instructions]: https://drupal.org/project/eloqua_rest_api#installation

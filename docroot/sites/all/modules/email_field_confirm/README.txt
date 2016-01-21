Introduction
============

The Email Field Confirm module will generate confirmation emails when a new
email address is added to an Email field (http://www.drupal.org/project/email).

An email address is considered confirmed for a given address/user combination.
This means that if user (uid:5) confirms email address user5@example.com and
then user (uid:10) adds user5@example.com, then user (uid:10) must also confirm
the email address.  If user (uid:5) adds user5@example.com to another email
field then user (uid:5) does *not* have to confirm the email address again.

*The inspiration for this module grew from the need to verify email addresses in
an email field and the Email Confirm
(http://www.drupal.org/project/email_confirm) module, however it has grown much
larger than originally anticipated.*

Email Field Confirm settings should work with any email field on any entity.
The settings are stored at the field instance (the field on a bundle).

Main Features
-------------

* Works on single-value and multi-value Email fields.
* Several hooks (see email_field_confirm.api.php) are available.
* Rules (http://www.drupal.org/project/rules) Integration is available.
* Custom Permissions
  * Bypass email field confirmation
    Email fields that require email confirmation will automatically be confirmed and not generate an email with confirmation link.
  * View ANY pending email field addresses
    View the pending (and confirmed) email addresses when viewing an add/edit form containing an email field.
  * Edit ANY pending email field addresses
    Can edit an email address value that is in a pending confirmation state. Note: Any user that may confirm the email address may automatically edit it.
  * Resend ANY pending confirmation email
    Generate an email with confirmation link to the user that is responsible for confirming. Note: any user that is responsible for confirming an email address may generate another confirmation email.
  * Confirm ANY pending confirmation email
    Manually confirm a pending email address without being the user responsible for confirming.
* For single-value fields
  * Option to keep the original email address on the entity until the new email
    address is validated.
  * Option to send a notification email to the original email address (if any).
  * Upon confirmation, the entity field is updated with the appropriate value.

*Note: So far not able to accurately track an original email address for multi-
value fields. An email could be removed while a new on added to as a new value
instead of overwriting. Values may also be re-ordered on multi-value fields.*


Configuration
=============

When adding or editing an existing email field instance configuration, the
following options are available.

* Send email to confirm this address?
  Check this option to enable confirmation emails for this instance of this
  field.
* Save the new email address?
  If checked, the new email address will be saved on the entity. However if this
  is unchecked, then the original value is retained until the new address is
  confirmed.
  (Note: multi-value fields must always save the new email address on the
  entity.)
* Notify original email address (if applicable)?
  When checked, if an email address is changed, the original email address will
  be notified that a change has taken place.
  (Note: multi-value fields do not retain the original email address.)
* Who may confirm the email addresses?
  Defines what user is actually responsible for confirming the email address.
  - Acting User: The user who is actively editing the entity adding the email
    address.
  - Entity Owner: The user that is attached to the entity as the author/owner.


Recommended Modules
===================

* Email Confirm (http://www.drupal.org/project/email_confirm)


Maintainers
===========

* Craig Aschbrenner <https://www.drupal.org/user/246322>

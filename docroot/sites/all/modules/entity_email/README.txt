Entity Email
============

Entity Email allows sending emails based on your own templates but can also record in the database any sent emails.

Main features:
- Built on the entity module and provide 2 entities:
  - Entity Email Type (your own email template)
  - Entity Email (an instance of the email sent)

- Supports the Rules module, so email actions can be triggered by any Rule.

- Supports the MimeMail module for HTML emails.

- Build with the fields API.


Required Modules
================

- Entity API


Optional Modules
================

- Rules
- MimeMail


Installation
============
* Go to the Modules page (/admin/modules) and enable it.


Entity Email Type
===================
Manage (import, add, edit, clone, delete, export) the different entity email type for your
site from the Structure page (/admin/structure/entity_email_type). Users must have 
"administer entity email types" permission for this.
 
* Label (required) - A descriptive label for the template.

* Name (required) - Machine name for the template.
 
* Subject (required) - The email subject. May contain tokens for substitution.
 
* Mail plain body  (optional) - The email plain body. May contain tokens for substitution.

* Mail Body (require the MimeMail module - optional) - The email HTML body. May contain tokens for substitution.

* Attachments (require the MimeMail module - optional) - The email attachments.

* Tags  (optional) -

* CC  (optional) -

* BCC  (optional) -


Using the Templates
===================
1) programmatically
2) via Rules

Triggering Emails From Rules
================================


Triggering Programmatically
===========================
global $user;
$params = array(
  'cuid' => $user,
  'fuid' => $user,
  'tuid' => $user,
  'mail_from' => '',
  'mail_to'
  'mail_replyto'
  'dynamic' => array('project' => 'projectttttt', 'id' => 'iddddd'),
);
$email = entity_email_send('new_quality_mentor', $params);


MimeMail Integration
====================

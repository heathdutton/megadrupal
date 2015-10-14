CONTENTS OF THIS FILE
---------------------

* Introduction
* Requirements
* Installation
* Configuration
* Troubleshooting
* Maintainers

INTRODUCTION
======================
Send to Kindle lets your site's users send documents to their Kindle devices.
This only works on field type 'file'. It creates a custom field formatter,
which must be specified for the file field in question, when creating or
editing  the content type.

After installation each user profile will have a new field called
'Send-to-Kindle E-Mail Address'. This is the email address of the Kindle device
to which the user wants documents sent. The user *MUST* go into their Amazon
account and configure their Kindle device to receive emails from
kindlemail@<domain_name>, where <domain_name> is the domain of your Drupal site.
The Send-to-Kindle user profile field has a description that instructs the user
to do this.

When you, or some other user, creates a content type with the field 'file', go
to the 'Manage Display' admin page for that field, located at:

    admin/structure/types/manage/<content_type>/display

Under the FORMAT column, there is a pull-down wherein you select the type of
formatter for the file. Pull it down and select 'Link with Send to Kindle
Button', then save the content type.

When the file is displayed, that display will be identical to the 'Generic file'
formatter, except there will be a button directly after it that reads 'Send to
Kindle'. Click the button, and the document is sent to the user's Kindle.

For customization purposes, the module includes a template file called
'sendtokindle_kindlebutton.tpl.php'. Copy this file to your theme directory,
and you can theme and modify the the file link as you wish. The template has
variables in it that allow a themer to create his/her own file link, or to
change the various fields in the email. These are described in more detail
in the template itself.

REQUIREMENTS
============
This module requires the mail system and mime mail modules:

    Mail system: https://www.drupal.org/project/mailsystem
    Mime mail: https://www.drupal.org/project/mimemail

INSTALLATION
============
To use the module, upload and install it per Drupal's standard module
installation instructions, along with the required modules. For more
instructions on how to upload and enable a module, see the official
instructions on how to do so:

    https://www.drupal.org/documentation/install/modules-themes/modules-7


CONFIGURATION
=============
There are no configuration options for this module. It does not show up
in the admin interface.

TROUBLESHOOTING
===============
If the 'Send to Kindle' button does not display, there may be several reasons.

1. Check to make sure the user is logged in. The modules does not work with
anonymous accounts.

2. Check to make sure the user has entered a Kindle email address in their
user profile. The module does not display a button if there is nowhere to
send the file.

3. Check to make sure the file link in question is a supported Kindle file
type. No button is displayed if a Kindle cannot display the file. The supported
types are:

    'doc',
    'docx',
    'html',
    'htm',
    'rtf',
    'txt',
    'jpeg',
    'jpg',
    'mobi',
    'azw',
    'gif',
    'png',
    'bmp',
    'pdf',

4. If a user's Kindle email address disappears when the module is disabled
and then re-enabled, that is is 'as designed'. There was no way I could find
to preserve the Kindle email in the user profile without displaying it and
making it available for edit -- which would be leaving part of the module
behind and visible when it is disabled. Which is bad.

I could solve this issue by requring other contributed module dependencies,
but I think creating a dependency just so the module can be disabled is too
much to ask, and inelegant.

On the other hand, if when the module is disabled and then re-enabled, all the
user has to do is enter his/her Kindle email in the profile again, well that's
not a big burden. So unless someone can offer me a solution, I'm going to leave
this as it is.


MAINTAINERS
===========
Author/Maintainer: Marshall Moseley, mosewrite AT earthlink DOT net

Enjoy!

--Marshall

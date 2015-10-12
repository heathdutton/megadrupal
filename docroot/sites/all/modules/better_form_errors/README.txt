################################################################################
#                                                                              #
#          Better Form Errors                                                  #
#          Version 7.x-1.0 (2015-06-21)                                        #
#                                                                              #
#          A module for Drupal 7.x                                             #
#                                                                              #
################################################################################

Web page
========

The project homepage is
  https://www.drupal.org/project/better_form_errors

Author
======

- Robert Allerstorfer (roball)
    https://www.drupal.org/user/405360

Overview
========

This module changes Drupal form validation error messages to a better readable
format with a stylable field name.

Drupal core or contributed modules such as "Webform" may print error messages
on server-side form validation for certain fields. For example, if a form
containing a required field called REQUIRED FIELD is submitted without a value
for that field, server-side validation will lead to the following HTML error
message:

  REQUIRED FIELD field is required.

The field name may not be easily distinguished from the rest of the message.
This module changes the HTML code of the message to:

  The field <span class="message-field" lang="LANG_CODE">REQUIRED FIELD</span> is required.

Since the field name is spanned within a class ("message-field"), it can be
styled to be easily distinguished from the rest of the message. LANG_CODE is the
language code representing the active interface language - such as "en", "de",
"fr", "fr-CA", or "zh-Hans". This allows you to further style the field name per
language. The module comes with a style sheet that styles the field name in
italic font. Of course, you can change the style to what you like in your own
style sheet.

Details
=======

The "Better Form Errors" module captures all error messages that Drupal would
output on unsuccessful server-side validation on submitted forms. These messages
will be scanned for known strings beside field names, and re-assembled from

  FIELD_NAME (field) EXPLANATION

to

  The field <span class="message-field" lang="LANG_CODE">FIELD_NAME_PLAIN</span> EXPLANATION

where FIELD_NAME_PLAIN is a sanitized version of FIELD_NAME, having HTML tags
and the following characters removed: leading quotes; trailing quotes, dots and
colons.

LANG_CODE is the language code of Drupal's active interface language.

Features
========

- Clean source code: 
  - Does not trigger any error/warning/notice message by PHP 5.4 on all possible
    error levels.
  - Does not trigger any warning by the Coder Review module on the "minor"
    severity warning level.
  - Does not trigger any Interface text translatability warning.
- Interface is fully translatable. As of version 7.x-1.0, the complete German
  translation is available on localize.drupal.org.
- Comes with complete documentation and integrated help.
- Zero configuration required.
- The module provided replacement rules can be overridden by other modules.

Requirements
============

Drupal 7.x core. No modules required.

Installation or Upgrade
=======================

This module follows Drupal's standard module installation procedure
( see https://www.drupal.org/documentation/install/modules-themes/modules-7 ).

If Drupal is running on a Unix server, the most convenient way to install the
module is doing it directly on the sh shell (via SSH). The commands to enter as
the root user may be something like:

[root@server ~]# cd /etc/drupal7/all/modules
[root@server modules]# rm -rf better_form_errors better_form_errors-*
[root@server modules]# wget \
> http://ftp.drupal.org/files/projects/better_form_errors-7.x-1.0.tar.gz
[root@server modules]# chmod 600 better_form_errors-7.x-1.0.tar.gz
[root@server modules]# tar -zxf better_form_errors-7.x-1.0.tar.gz
[root@server modules]# chown -R apache:apache better_form_errors

After installation, load the "Administration » Modules" (admin/modules)
page, tick the "Better Form Errors" checkbox within the "User interface" package
group and press the [ Save configuration ] button. You should see a message that
the "Better Form Errors" module has been enabled.

Configuration
=============

This module has zero configuration - just enable it and you are done.

Overriding default replacement rules
====================================

The default rules to find and replace error messages provided by the "Better
Form Errors" module can be overridden by other modules by implementing the hook
"better_form_errors_catch_message". This allows you to add translations for
languages or updated strings not covered by the current version of the module.

See inside the file "better_form_errors.api.php" how to implement that hook into
your own custom module.

Uninstallation
==============

1. Go to "Administration » Modules" (admin/modules), untick the
   "Better Form Errors" checkbox within the "Fields" group and press the
   [ Save configuration ] button. You should see a message that the
   "Better Form Errors" module has been disabled.

2. Click the [ Uninstall ] tab (admin/modules/uninstall), tick the
   "Better Form Errors" checkbox and press the [ Uninstall ] button. On the next
   screen you must confirm uninstall by again pressing an [ Uninstall ] button.

3. Entirely remove the module's directory from the modules directory:

[root@server modules]# rm -rf better_form_errors

Description
-----------
This module provides simple Server-Side Include (SSI)-like functionality in
a custom field type, allowing the inclusion of content from local files or
via HTTP anywhere a field can be attached.

Requirements
------------
Drupal 7.x

Installation
------------
1. Copy the entire include_field directory into Drupal's sites/all/modules
   (or sites/default/modules) directory.

2. Login as an administrator. Enable the module in "Administer" -> "Modules"

Overview
--------
After it is downloaded and enabled, this module provides a new field type.
Instances of this field type are edited to contain file locations or, optionally,
HTTP URLs, and when the fields are displayed the module replaces them inline with
the contents found at the specified locations.

Configuration
-------------
Two new permissions are provided: permission to edit the configuration of this
module, and permission to edit include field instances themselves.

File inclusion is only allowed from within a folder specified in configuration
(the default is sites/default/files/).

The ability to include content via HTTP is optional, and disabled by default.

HTTP inclusion may optionally be restricted to source domains which match a
regular expression specified in configuration.

Failure to retrieve included content can be handled in three ways: "loud"
failure displays a visible error message in HTML; "quiet" failure displays an
error message as an HTML comment; and "silent" failure displays no error message
at all.

Field instances default to displaying file contents, but can also be set to
display file locations.

Field instances can, optionally, pass the current query string through with the
request for an included HTTP resource. This can be useful for including dynamic
resources.

Security
--------

Because of the nature of this module it is particularly important to be mindful
of security concerns. In particular:

1. Remember that any included content will be served from your site, and
   will be treated by browsers with the same degree of trust that would be
   allowed if the content lived solely in your site.

2. Limit the ability to edit include fields as much as possible, only
   providing it to roles/accounts which are thoroughly trusted.

3. Lock down the include folder as much as you can to prevent include field
   editors from including files you don't want them to. In the absence of
   another setting the module restricts includes to sites/default/files/.

4. Do not enable HTTP includes unless you need them.

5. Never include remote content you do not the right to use in this manner,
   and avoid using this module to include content from any sources that you
   do not control.

Support
-------
Please use the issue queue for filing bugs with this module at
http://drupal.org/project/issues/1508964

Author
------
This module was originally created by Sven Aas for Mount Holyoke College.

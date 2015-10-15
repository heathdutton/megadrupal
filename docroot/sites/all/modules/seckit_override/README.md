CONTENTS OF THIS FILE
=====================
 * Introduction
 * Requirements
 * Installation
 * Configuration
 * Usage
 * Maintainers

INTRODUCTION
============
The SecKit Override module integrates with the
[Security Kit](https://drupal.org/project/seckit) module (seckit)
to allow the site as a whole to have a "default" level of level of protection,
while allowing specific pages within the site to change those protection
levels. For instance, SecKit might be configured to preven pages on the site
from being displayed in an <iframe> tag to prevent clickjacking. However,
there may be some specific pages within the site which are designed to be used
as widgets on another site which **should** be available within an <iframe>.
This module allows this level of granular control.

REQUIREMENTS
============
This module requires the following modules:
 * [Security Kit](https://drupal.org/project/seckit) version 1.9+

INSTALLATION
============
 * Install as you would normally install a contributed Drupal module. See:
   [Installing Modules](https://drupal.org/documentation/install/modules-themes/modules-7)
   for further information.

CONFIGURATION
=============
 * Configure base security settings in Administration » System » Security Kit
   * These settings will apply to all pages not overridden below, and also
     provide the basis for allowing settings to be inherited.
 * Configure base security settings in Administration » System »
   Security Kit » Overrides:
   * Paths are specified **WITHOUT** the leading '/' character. The path may
     include one or more '*' characters ti indicate a wildcard. For instance,
     the path 'widgets/*' would match any path which started with 'widgets/'.
   * Paths are processed in the order shown in the admin interface. If a given
     path matches multiple rules, then each rule will be applied in turn. As
     a best practice, place the most general rules at the top of the list,
     and the more specific rules further down.
   * The edit form for a given override should look almost identical to the
     original Security Kit base form. The only exceptions are that:
     * Any checkboxes have been changed to drop down lists.
     * All drop down lists should have an 'Inherit' option as the first choice.
   * And drop down left as 'Inherit' will use whatever settings it would have
     used if this override was not in place. If this is the first (or only)
     rule that matches this path, then the setting will be the same as the
     global SecKit setting. If this is **NOT** the first rule to match, then
     'Inherit' will leave whatever setting was put in place by the last rule
     to run.
   * Any text field or text area left blank will inherit the prior value, in
     a manner similar to that described for drop down lists. Any value placed
     in such a field will override the prior setting.

USAGE
=====
The inheritance feature of this module means that it is possible to set up
a fairly complex pattern of interactions with relatively little configuration
work. Simply set the baseline options using the existing Security Kit form,
and then only override the specific settings needed for a given path, leaving
all of the others to inherit from the base settings. Further fine tuning can
be set by matching a given path against multiple rules. For instance, the site
as a whole might be configured to block XSS attacks, and to block clickjacking
using the 'X-Frame-Options' settings, but pages found under 'widgets/' need to 
allow such an inclusion. Furthermore, pages under 'widgets/development' are
considered experimental, and should only be available from a specific location.
By using inheritance, this is possible. The rules should be ordered as
 * 'widgets/*'
 * 'widgets/development/*'
The 'widgets/*' rule should override the 'X-Frame-Options' setting to be
'Disabled'. The 'widgets/development/*' rule can leave 'X-Frame-Options' as
'Inherit' because, since it is processed second, it will inherit the disabled
'X-Frame-Options' setting from 'widgets/*'. It, however, override the
'From-Origin' and 'Allow loading content to' settings.

MAINTAINERS
===========
Current maintainers:
 * Dwayne Bailey [dbcollies](https://drupal.org/u/dbcollies)
 * Mark Meytin [mmeytin](https://www.drupal.org/user/1246898)

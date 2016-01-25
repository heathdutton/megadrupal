Description
===========

This module extends Formatted Field [https://drupal.org/project/formatter_field]
module by adding a user-friendly widget for the 'formatter' field. Site builders
are able now to maintain a per-field list of presets. For each preset a nice
title can be entered (e.g. 'Left-top photo with wrapping text') so that is easy
now for non-technical users to decide how to display the field when adding
content.

Important note!
===============

This module needs the patch from https://drupal.org/node/2048131#comment-7673095
to be appied to Formatted Field [https://drupal.org/project/formatter_field]
module.

Dependencies
============

The module requires:

- Formatted Field [https://drupal.org/project/formatter_field]
- Chaos tools suite (ctools) [https://drupal.org/project/ctools]

Install and configure
=====================

1. Install dependency modules.
2. Apply patch from https://drupal.org/node/2048131#comment-7673095.
3. Configure some presets for desired field(s) using the presets UI at
   admin/config/content/formatter-field-preset.
4. Follow Formatted Field [https://drupal.org/project/formatter_field] module
   instructions in order to create and configure 'formatter' field.
5. Use 'Formatter preset select' [formatter_preset_select] as widget for
   'formatter' field.

Credits
=======

- Author: Claudiu Cristea <clau.cristea@gmail.com>
- Sponsored by: Webikon.com

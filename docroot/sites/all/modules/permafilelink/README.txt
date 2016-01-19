
INTRODUCTION
------------

Permafilelink is a solution to the problem of stale inline file links in user
content, especially in formatted (possibly wysiwyg) textareas such as on
entity create/edit forms and block configuration forms, where the user can
pick images or documents through a built-in file browser, e.g. imce, elfinder
and such.

When saving an entity create/edit form or a block configuration form, the form
submit handler will scan the submitted text, and replace any occurrence of
links to managed files with a generic link based on the Drupal file id (fid).

When viewing the entity or the block, the permafilelink filter will translate
these permafilelinks back into the canonical form, optionally adding a bit of
further processing.


INSTALLATION
------------
Install as you would normally install a contributed Drupal module. See:
https://drupal.org/documentation/install/modules-themes/modules-7
for further information.


CONFIGURATION
-------------

Configure text formats:

Administration » Configuration » Content authoring » Text formats.

Go to the desired text formats and enable the "Permanent File Link" filter.

Optionally click "Enable further link shortening" if needed.


General configuration:

Administration » Configuration » Media » Permanent Filelink.

For extended reporting during content editing, click "Enable verbose reporting".
This setting will report various issues encountered during file link contraction
and expansion.



MAINTAINERS
-----------
Current maintainers:
 * Jesper Vingborg Andersen (vingborg) - https://www.drupal.org/u/vingborg


This project has been sponsored by:
 * Operate IT A/S (http://operateit.dk)

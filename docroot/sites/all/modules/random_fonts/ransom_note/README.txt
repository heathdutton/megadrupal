Ransom Note (ransom_note) module

SUMMARY

This module makes text look like a ransom note by applying a mix of webfonts.

PREREQUISITES

Ransom Note requires @font-your-face (http://drupal.org/project/fontyourface).

HOW TO USE IT

Enable some web fonts. Navigate to Configuration > @font-your-face settings >
Ransom Note, choose which fonts to use and target selectors (see 'Known issues'
below).

For a good looking ransom note, some monocase (all caps or no caps) fonts are
essential. Inverted (white on black background) fonts are good, too. Font
Squirrel has lots of both kinds.

KNOWN ISSUES

Ransom Note breaks markup of children elements, if there any. Hence, target
elements must be configured to be at the bottom level or no more than one level
up from it. For example, '#block-block-1' will work, while '.sidebar' will
garble all the blocks in the sidebar.

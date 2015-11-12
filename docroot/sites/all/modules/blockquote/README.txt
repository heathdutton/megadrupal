$Id: README.txt,v 1.3 2010/12/06 00:54:21 zeta Exp $

The Blockquote module is used to create styled blockquotes in your pages.

These blocks are intended for one or more verses or paragraphs that are
quoted verbatim (word for word).  Separating this text into a block shows that
the whole block is quoted, therefore no punctuation marks are needed to
indicate the beginning or end of the quotation.

Nevertheless, we do want the block to be styled appropriately,
and Garland in particular, is lacking this feature.

The Blockquote module provides this styling, especially tailored to
harmonise with Garland, but can be used with any theme.

RTL support is included: Make sure you are using an RTL or BIDI aware theme.

INSTALLATION INSTRUCTIONS
---
1. Extract this module to sites/[ all | {domain} ]/modules folder.
2. Login as the user who has administrator permissions (user/1).
3. Activate blockquote in the "Other" category,
   on the Administer Â» Site building Â» Modules page at admin/build/modules.

CONFIGURATION
---
1. To use the blockquotes filter in a particular input format, go to
   Administer Â» Site configuration Â» Input formats
   at admin/settings/filters

2. Select configure for your choice of input format,
   and activate one of the blockquote filters.

3. Make sure that the HTML filter allows <blockquote> tags,
   or that the filter you have chosen is arranged after the HTML filter,
   depending on your choice of blockquote filter.

Note:
If you are using the Garland theme, it is prefered to allow the <blockquote> tag
in the HTML filter, and use the first blockquote filter.
Either blockquote filter will try to make sure the <blockquote> tag appears in
your text so it should be explicitly allowed.
If you are using a theme other than Garland, you might find the two filters
produce slightly different results.  Experiment to find your preference.

KNOWN ISSUES
---
Please use the issue queue to report any issues you think need to be fixed.

COPYRIGHT AND ACKNOWLEDGEMENTS
---
The module Blockquote, excepting the image copied from the Garland theme,
is copyright 2008 by Nick Baxter-Jones.  It is licensed under the GPL, a copy
of which, is included.

The image from the Garland theme, is copied so that the module can be placed
in different modules/ folders on your site.

The module Blockquote was originally created by "zeta Î¶" in July 2008.

This module was motivated and inspired by a thread to add this feature to
the Garland theme.  Thanks to all who contributed, especially flobruit, who
pointed out my implementation uses extra markup that is not needed semantically,
so wouldn't be added to a core theme.

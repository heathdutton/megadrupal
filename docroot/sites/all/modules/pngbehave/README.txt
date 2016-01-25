The pngbehave module was developed using:

Pieces of the pngFix module (http://drupal.org/project/pngfix),

as well as the behavior and tiling scripts from:

// IE5.5+ PNG Alpha Fix v2.0 Alpha
// (c) 2004-2008 Angus Turnbull http://www.twinhelix.com
// licensed under the GNU LGPL, version 2.1 or later.
// For details, see: http://creativecommons.org/licenses/LGPL/2.1/

// IE5.5+ PNG Alpha Fix v2.0 Alpha: Background Tiling Support
// (c) 2008 Angus Turnbull http://www.twinhelix.com
// licensed under the GNU LGPL, version 2.1 or later.
// For details, see: http://creativecommons.org/licenses/LGPL/2.1/

This module is maintained by the Tomadoh Project (www.tomadoh.com and http://drupal.tomadoh.com)
and
Chris Paul 'mrjeeves' (jeeves at tomadoh dot com)

FEATURES:
- Works with any elements
- Works with backgrounds tiled, repeated, and streched WITHOUT need for parent element confusion
- Works with dynamically created elements that match the css selector string or directly call the behavior
- Works on style AND class changes in runtime WITHOUT needing to call the script again
- Never worry about re-fixing a png again
- Apply the behavior "myclass {behavior:url('/sites/all/modules/pngbehave/iepngfix.htc')}" to any style without worry


INSTALLATION AND USAGE:

1. Upload to your modules directory (likely /sites/all/modules)

2. Activate module (admin/build/modules)

3. Set elements and classes to process (admin/settings/pngbehave)

Comma seperated list of CSS classes/selectors to make BEHAVE!.

To include all img, ul, and li elements
-- img, ul, li 
or a specific class:
-- .star
or combination
-- img, .star, ul, li, ul ul, div.star etc...

RECOMMENDED SELECTORS: img, ul, li, span .

NOTE: USING 'div' IS HIGHLY DISCOURAGED AS IT DRAMATICALLY INCREASES PAGE LOAD TIMES. Use more specific selectors if needed.
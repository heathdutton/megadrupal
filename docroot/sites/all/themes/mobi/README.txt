OVERVIEW
--------
This theme creates a way sites who have .mobi aliases of their website
to have a specially formatted mobile format for their visitors who use
the .mobi domain, and allow the visitors of your .com/.net/.org/etc to
still see a fancy or complex theme.

This theme is intended to be used with the mobi loader module, though
it will also work fine without it as long as your intention is for all
your users to see the mobi format. When paired with the .mobi module,
the module overrides the default theme to display a mobile, portable
format from the .mobi theme.

INSTALLATION
------------
1. Copy the theme to sites/all/themes/mobi on your server.

2. Navigate to Administer -> Site building -> Themes
   and enable the .mobi theme.

3. Install the .mobi loader module.
   http://drupal.org/project/mobi_loader

   The .mobi loader module will
   check for visitors who requested your site with a .mobi
   domain alias and switch your output to use the mobi theme by
   default on the fly.

3. Configure your blocks at Administer -> Site building -> Blocks.
   As of Drupal 5, you can set different block options for each theme.
   The .mobi theme displays the left sidebar after the header, but
   before the main content, and the right sidebar below the main
   content, but before the footer.

RESOURCES
---------
ready.mobi page analysis: http://ready.mobi/
Opera Mini live demo: http://www.operamini.com/demo/

AUTHOR
------
David Kent Norman
http://deekayen.net/
http://deekayen.mobi/
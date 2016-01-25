KandS
----------------------

KandS is a Zen sub-theme with a heavy 19th-century influence.
Features include:
- Webfont support through the theme settings page
- Custom splash image, also through the theme settings page
- Responsive layout with SASS and Zen-Grids
- Off-canvas menu for mobile

There are two layouts: regular and mobile.  Mobile is designed to be viewed on
anything the size of an iPad or smaller and will resize to fit the viewing area.
It uses Zen-Grids which is built into Zen 5.  The regular layout goes up to 
1280 px.

In the mobile layout, the sidebar turns into an off-canvas menu.  It will
automatically resize to fit the viewing area, and also to fit the contents
of the region.

See below for more info on how to use the features.

Installation
----------------------

1. Install Zen 5.x
   * NOTE: * You must use the latest version of Zen.  KandS will not work with
             Zen 3.x or earlier.
2. Install KandS like a regular theme
3. Configure theme-specific settings at admin/appearance/settings/kands
   (See below for more information)
   
Customization
----------------------

If you want to customize this theme, you should use SASS.  For more
information: http://drupal.org/node/1548946

Webfont support
----------------------

Webfonts is a relatively new technology that lets you use any font on a web
page.  Letter includes Garamond, which is a webfont hosted by Google Webfonts.
You can easily override with your own webfont provider.  Here's how:

1. Go to the theme settings page: admin/appearance/settings/letter
2. Under "Webfont Settings," paste in the link to your webfont provider.  Letter
  supports either css (like Google Webfonts) or javascript (like Fonts.com).
3. If you use the css method, you also need to edit the theme css files.  Font
  family is set in css/html-reset.css.  
  (If you use javascript, you don't need to edit the css files.)

For more information about webfonts: http://koplowicz.com/node/706
For more information about Garamond: http://koplowicz.com/node/708

Splash Image
----------------------
Most Drupal themes have two images in the theme settings: favicon and logo.
KandS adds a third.  The splash image is a large image that only appears on the
front page.  It can be set manually in admin/appearance/settings/kands or via
css.  There is no upload option like there is with the favicon or logo; if you
want to upload a new splash image you will have to upload it directly to the
server via ftp or sftp.  (This feature will be coming in a future release.)

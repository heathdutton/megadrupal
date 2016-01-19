CONTENTS OF THIS FILE
----------------------
  * Introduction
  * Features
  * Installation
  * Quick Start
  * Useful Information
  * Authors


INTRODUCTION
----------------------

Sasson is a powerful tool-kit intended for Advanced Drupal Theming, aiming at
bringing the fun back to theming.
It is a set of advanced tools combined together in a modular structure,
everything is optional and we keep it so that what you don't use won't leave any
trace in your output code.

Clean and simple code, lightweight structure, latest technologies, 100% open-
source and the best DX (developer experience) we could think of is what we
hope you will find in this new Drupal base theme.


FEATURES
----------------------

* Made with Sass & Compass - just change a few Sass variables and it's yours.
* Mobile friendly - with responsive, content-first layout, out of the box.
  optional mobile-first responsive layout, media queries break-points are
  configurable.
* HTML5 for all core template - <header>, <footer>,
  <article> for nodes, <section> for blocks, <aside> for sidebars, <nav> for
  menus etc. (thanks Boron - http://drupal.org/project/boron)
* It includes Susy - a powerful, semantic layout system (http://susy.oddbird.net/).
* It includes an HTML5-friendly CSS Reset or Normalize (you choose), cross-
  browser compatibility improvements and other tweaks & best practices from
  HTML5Boilerplate (http://html5boilerplate.com).
* File watcher - See changes happen in real time. Sasson will auto-
  magically update your browser whenever you modify your style sheets (or any
  other file) without refreshing the browser.
* It *doesn't* give you a pile of CSS rules you will have to override.


MORE FEATURES
----------------------

* Ready made sub-theme starter kit. start fresh with a single Drush command,
  or just copy, rename, and start theming.
* Google web-fonts support (http://www.google.com/webfonts).
* HTML5 support in oldIEs via HTML5Shiv v3 (http://code.google.com/p/html5shiv/).
* HTML5 doctype and meta content-type.
* Search form uses the new input type="search" attribute.
* WAI-ARIA accessibility roles added to primary elements.
* Many extra body and node classes for easy theming.
* Responsive menus (via https://github.com/joggink/jquerymobiledropdown).
* Grid background "image", for easy element aligning, made with CSS3 and SASS
  to fit every grid you can imagine.
* Draggable overlay image you can lay over your HTML for easy visual
  comparison. you may also set different overlay opacity values.
* Bi-directionality support for RTL and LTR (Right-To-Left and Left-To-Right).


INSTALLATION
----------------------

* OK way -

  * Extract the theme in your sites/all/themes/ directory, enable it and
    start hacking.

* Better way -

  * Extract the theme in your sites/all/themes/ directory.
  * Move SUBTHEME into its own folder in your themes directory.
  * Optional but recommended - Rename at least your folder and .info file.
  * Enable your sub-theme and start hacking.

* Even better -

  * Use drush to create and enable your sub-theme(s) -

    # drush sns "My theme"


QUICK START
----------------------

Create your own layout in a few simple steps:

  1. Install Sasson and create a fresh sub-theme (see Installation section).
  2. Copy /sass/layout directory and templates from Sasson into your new sub-theme.
  3. Call desktop-first.css or mobile-first.css from your .info file -

      styles[stylesheets/layout/desktop-first.css][options][media] = all

  4. Modify the layout directory and templates to your needs.
  5. Compile your Sass stylesheets.
  6. Clean cache.

  * See Full On (https://www.drupal.org/project/fullon) for an example to theme
    layout overrides.


USEFUL INFORMATION
----------------------

* Out of the box Sasson will give you a 960 pixel wide responsive layout, you
  may change grid & layout properties using Sass variables.

* Sasson gives you a responsive layout - that means your site adapts itself
  to the browser it is displayed on. you may disable this behavior via theme settings.

* The default responsive layout takes a desktop-first approach, you can go
  mobile-first with a click in your theme settings.

* When loading style-sheets in your .info file Sasson allows you to specify
  settings like media queries, browsers, weight and any option available to
  drupal_add_css(), for example :

    styles[stylesheets/sasson.css][options][weight] = 1
    styles[stylesheets/sasson.css][options][media] = screen and (max-width:400px)
    styles[stylesheets/sasson.css][options][browsers][IE] = lte IE 7
    styles[stylesheets/sasson.css][options][browsers][!IE] = FALSE

  This will load sasson.scss with an extra weight for screen only (not print)
  on browsers wider then 400px and on IE7 or older only, you get the point.

* Sasson can apply classes to the <html> tag to allow easier styling for
  Internet Exporer, enable this feature under "HTML5 IE support" :

    - html.ie9 #selector { ... } /* IE9 only rules */
    - htm.lte-ie8 #selector { ... } /* IE8 and below rules */

* Bi-directionality - Many sites need to have both LTR (left-to-right e.g.
  English) and RTL (right-to-left e.g. Hebrew, Arabic) support for different
  sections of the site. Drupal allows you to add '-rtl' to your CSS filenames
  for style sheets that will be loaded for RTL pages only. Sasson allows you to
  use '-ltr' too (e.g. mytheme-ltr.scss) for files that will load on LTR pages
  only.


AUTHORS
----------------------

* Tsachi Shlidor (tsi) - http://drupal.org/user/322980
* Raz konforti (konforti) - http://drupal.org/user/99548
* Many others...

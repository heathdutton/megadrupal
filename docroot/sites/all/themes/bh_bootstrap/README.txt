BOOTSTRAP THEME FOR DRUPAL 7
----------------------------------------------------------------------

PLEASE CHECKOUT THE 7.x-2.x BRANCH TO CONTINUE!

This theme is a basic port of Twitter's "Bootstrap" [1] development
framework to Drupal 7. It probably can be used as a theme out-of-the-
box, but is intended to be used as a parent theme. The theme itself is
currently at an early stage of development, but appears reliable and
stable. Implemented features should simply work with Drupal 7.

However, please note:

  1.  this theme requires the Bootstrap library. [2] Ideally, the
      libraries module should be installed and the Bootstrap framework
      directory should be placed so that the main bootstrap css file
      is reachable at 
      
      sites/all/libraries/bootstrap/bootstrap.css
  
      Alternatively, download the Bootstrap framework, copy it into
      the theme directory so that the main bootstrap css file is
      reachable at
      
      sites/all/themes/bh_bootstrap/bootstrap/css/bootstrap.css

  2.  this theme requires jQuery 1.7.
      
  3.  this theme works well as an admin theme (surprisingly well in
      fact--it reflects well on both Drupal and Boostrap!), but please
      note that since it uses jQuery 1.7, adding fields to content
      types will NOT work currently (though the issue may be fixed in
      Drupal core). [3]
      
      Note also that though the theme is generally attractive and
      functional when used as an admin theme, there are definite
      problems with certain Drupal core pages and with commonly-used
      contrib modules. Some of these are easily fixed, others not so
      much. Your mileage may vary.
            
      If it doesn't work with your favourite contrib module, I'll
      gratefully accept and merge your patches :)
      
  4.  the excellent Webform module duplicates many of Drupal's core
      form theme functions. This theme does not yet implement these.


STRATEGY
----------------------------------------------------------------------
Bootstrap's markup and Drupal's are often similar, but can be quite
different. This theme employs two strategies to overcome this differ-
ence:

  1.  Remove Drupal (and module) stylesheets where they do more harm
      than good; in other words, rather than duplicate styles (such as
      Drupal's menu styles) that are amply handled by Bootstrap, simp-
      ly use the theme info file to remove them altogether, and
  
  2.  use theming functions or template files to modify Drupal's html
      (as minimally as is practical) so that Bootstrap's CSS will work
      directly; in these cases, I have tried to keep the theming
      functions as close as possible to the original functions con-
      tained in core or in the modules that provide the originals.
      
  3.  as a last resort, (since Bootstrap and Drupal often do not have
      sufficiently similar markup) "map" Bootstrap styles onto Drupal
      markup using Less--in fact I haven't needed to do any of this
      since restarting development on the 7.x-2.x branch.


Building subthemes
--------------------
@todo


BOOTSTRAP FEATURE IMPLEMENTATIONS
----------------------------------------------------------------------
So far, the following Bootstrap features have been positively imple-
mented (see [1] for the authoritative list of features):

  1.  the basic fixed, 16-column grid system
  2.  typography and lists
  3.  basic form styles and buttons (not including the Webform
      module's forms--yet)
  4.  topbar
  5.  tabs
  6.  breadcrumbs
  7.  block messages
  
The BH Bootstrap Extras module package also provides modules to enable
the following features for use with this theme:

  -   dropdowns--though these have only been tested with the Bootstrap
      navbar, but may also work with tabs, pills etc
  -   Bootstrap-style tabs for use with the teriffic Quicktabs module


"Hero" unit
--------------------
Bootstrap includes a "Main hero unit for a primary marketing message
or call to action". This is essentially a big block, styled dif-
ferently from the main content. This theme implements that feature
by providing a region-specific block template, 
block--highlighted.tpl.php that provides the necessary class, and
changes the block's heading from h2 to h1. In page.tpl.php, when
$highlighted is set, the page's heading changes from h1 to h2 (i.e. so
that there is never more than a single h1 per page).

Use Drupal's Blocks interface or the Context module to display a 'hero
unit' on any (or every) page on your site. As it's a standard Drupal
region, it may contain any kind of block content whatsoever.


NOTES
----------------------------------------------------------------------
[1]   http://twitter.github.com/bootstrap/
[2]   http://twitter.github.com/bootstrap/assets/bootstrap.zip
[3]   http://drupal.org/node/1448494
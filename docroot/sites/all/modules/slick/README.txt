
Slick Carousel
================================================================================

Slick is a powerful and performant slideshow/carousel solution leveraging Ken
Wheeler's Slick carousel.
See http://kenwheeler.github.io/slick

Powerful: Slick is one of the sliders [1], as of 9/15, the only one [2], which
supports a mix of responsive and lazy-loaded image, and 3rd party video and
audio in a single slideshow/carousel with image to iframe or multimedia lightbox
switchers. See below for the supported media.

Performant: Slick is stored as plain HTML the first time it is requested, and
then reused on subsequent requests. Carousels with cacheability and lazyload
are lighter and faster than those without.

As a CTools plugin, which is future-proof D8 CMI, Slick is easy to customize
either via UI or code, and can be stored at database, or codebase.

Slick has a gazillion of options, please start with the very basic working
samples from slick_example [3] only if trouble to build slicks. Be sure to read
its README.txt. Spending 5 minutes or so will save you hours in building more
complex slideshows.

[1] https://groups.drupal.org/node/20384
[2] https://www.drupal.org/node/418616
[3] http://dgo.to/slick_extras


FEATURES
--------------------------------------------------------------------------------
o Fully responsive. Scales with its container.
o Uses CSS3 when available. Fully functional when not.
o Swipe enabled. Or disabled, if you prefer.
o Desktop mouse dragging.
o Fully accessible with arrow key navigation.
o Built-in lazyLoad, and multiple breakpoint options.
o Random, autoplay, pagers, arrows, dots/text/tabs/thumbnail pagers etc...
o Supports pure text, responsive image, iframe, video, and audio carousels with
  aspect ratio. No extra jQuery plugin FitVids is required. Just CSS.
o Exportable via CTools.
o Works with Views, core and contrib fields: Image, Media or Field collection.
o Optional and modular skins, e.g.: Carousel, Classic, Fullscreen, Fullwidth,
  Grid, Split. Nothing loaded unless configured so.
o Various slide layouts are built with pure CSS goodness.
o Nested slicks, image/video/audio slide carousels/overlay or multiple slicks
  within a single Slick using Field collection, or Views.
o Some useful hooks and drupal_alters for advanced works.
o Modular integration with various contribs via optional sub-modules to build
  carousels with multimedia lightboxes or inline multimedia.
o Cacheability + lazyload = light + fast.



VERSIONS
--------------------------------------------------------------------------------
7.x-2.x supports exportable optionsets via CTools.
Be sure to run update, when upgrading from 7.x-1.x to 7.x-2.x to allow creating
database table to store/ manage option sets.

7.x-2.x supports Slick 1.5 above, and dropped supporting Slick 1.4.x and below.
Be sure to read the project home page for more info before updating your module.


INSTALLATION
--------------------------------------------------------------------------------
Install the module as usual, more info can be found on:
http://drupal.org/documentation/install/modules-themes/modules-7

The Slick module has several sub-modules:
- slick_ui, included, to manage optionsets, can be uninstalled at production.
- slick_fields, included, supports Image, Media, and Field collection fields.
- slick_views, a separate project as of 2015-5-29, > beta1:
  http://dgo.to/slick_views
- slick_devel, if you want to help testing and developing the Slick.
- slick_example, if you want to get up and running quickly.
  The last two are separate projects as of 2015-5-31, > beta1:
  http://dgo.to/slick_extras


REQUIREMENTS
--------------------------------------------------------------------------------
- Slick library:
  o Download Slick archive >= 1.5 from https://github.com/kenwheeler/slick/
  o Extract it as is, rename "slick-master" to "slick", so the needed assets are
    available at:
    sites/../libraries/slick/slick/slick.css
    sites/../libraries/slick/slick/slick-theme.css (optional if a skin chosen)
    sites/../libraries/slick/slick/slick.min.js

- CTools, for exportable optionsets -- only the main "Chaos tools" is needed.
  If you have Views installed, CTools is already enabled.
  D8 in core: CMI.

- libraries (>=2.x)
  D8: dropped.

- jquery_update with jQuery > 1.7, perhaps 1.8 if trouble with the latest Slick.
  D8: dropped.

- Download jqeasing from http://gsgd.co.uk/sandbox/jquery/easing
  Rename jquery.easing.1.3.js to jquery.easing.min.js, so available at:
  sites/../libraries/easing/jquery.easing.min.js
  This is a fallback for non-supporting browsers.

- A basic knowledge of Drupal site building is required.
  Please refer to the provided README on each sub-module, and each form item
  description for more info.
  Also refer to the supported modules guidelines to be useful for Slick.



OPTIONAL INTEGRATION
--------------------------------------------------------------------------------
Slick supports enhancements and more complex layouts.

- Colorbox, to have grids/slides that open up image/video/audio in overlay.
- Photobox, idem ditto.
- Picture, to get truly responsive image.
  D8 in core: Responsive image.
- Media, including media_youtube, media_vimeo, and media_soundcloud, to have
  fairly variant slides: image, video, audio, or a mix of em.
  D8: Media entity, or isfield.
- Field Collection, to add Overlay image/audio/video over the main image stage,
  with additional basic Scald integration for the image/video/audio overlay.
  D8: ?
- Color field module within Field Collection to colorize the slide individually.
  D8 in core: Color field.
- Mousewheel, download from https://github.com/brandonaaron/jquery-mousewheel,
  so it is available at:
  sites/.../libraries/mousewheel/jquery.mousewheel.min.js


RECOMMENDED MODULES
--------------------------------------------------------------------------------
- Block reference to have more complex slide content for Fullscreen/width skins.
- Field formatter settings, to modify field formatter settings and summaries.


OPTIONSETS
--------------------------------------------------------------------------------
To create your optionsets, go to:
"admin/config/media/slick"
These will be available at field formatter "Manage display", and Views UI.


VIEWS AND FIELDS
--------------------------------------------------------------------------------
Slick works with Views and as field display formatters.
Slick Views is available as a style plugin included at slick_views.module.
Slick fields is available as a display formatter included at slick_fields.module
which supports core and contrib fields: Image, Media, Field collection.


PROGRAMATICALLY
--------------------------------------------------------------------------------
See slick_fields.module for advanced sample, or slick.api.php for a simple one.


NESTED SLICKS
--------------------------------------------------------------------------------
Nested slick is a parent Slick containing slides which contain individual child
slick per slide. The child slicks are basically regular slide overlays like
a single video over the large background image, only with nested slicks it can
be many videos displayed as a slideshow as well.
Use Field collection, or Views to build one.
Supported multi-value fields for nested slicks: Image, Media, Atom reference.


SKINS
--------------------------------------------------------------------------------
The main purpose of skins are to demonstrate that often times some CSS lines are
enough to build fairly variant layouts. No JS needed. Unless, of course, when
you want more sophisticated slider like spiral 3D carousel which is beyond what
CSS can do. But more often CSS will do.

Skins allow swappable layouts like next/prev links, split image or caption, etc.
Be sure to enable slick_fields.module and provide a dedicated slide layout
per field to get more control over caption placements. However a combination of
skins and options may lead to unpredictable layouts, get dirty yourself.

Some default complex layout skins applied to desktop only, adjust for the mobile
accordingly. The provided skins are very basic to support the necessary layouts.
It is not the module job to match your awesome design requirements.

Optional skins:
--------------
- None
  It is all about DIY.
  Doesn't load any extra CSS other than the basic styles required by slick.
  Skins defined by sub-modules fallback to those defined at the optionset.
  Be sure to empty the Optionset skin to disable the skin at all.
  If you are using individual slide layout, do the layouts yourself.

- 3d back
  Adds 3d view with focal point at back, works best with 3 slidesToShow,
  centerMode, and caption below the slide.

- Classic
  Adds dark background color over white caption, only good for slider (single
  slide visible), not carousel (multiple slides visible), where small captions
  are placed over images, and animated based on their placement.

- Full screen
  Works best with 1 slidesToShow. Use z-index layering > 8 to position elements
  over the slides, and place it at large regions. Currently only works with
  Slick fields, use Views to make it a block. Use block_reference inside FC to
  have more complex contents inside individual slide, and assign it to Slide
  caption fields.

- Full width
  Adds additional wrapper to wrap overlay audio/video and captions properly.
  This is designated for large slider in the header or spanning width to window
  edges at least 1170px width for large monitor.

- Boxed
  Added a 0 60px margin to slick-list container and hide neighboring slides.
  An alternative to centerPadding which still reveals neighboring slides.

- Split
  Caption and image/media are split half, and placed side by side.

- Box carousel
  Added box-shadow to the carousel slides, multiple visible slides. Use
  slidesToShow option > 2.

- Boxed split
  Caption and image/media are split half, and have edge margin 0 60px.

- Grid, to create the last grid carousel. Use slidesToShow > 1 to have more grid
  combination, only if you have considerable amount of grids, otherwise 1.
  Avoid variableWidth and adaptiveHeight. Use consistent dimensions.
  Choose skin "Grid" for starter.
  Uses the Foundation 5.5 block-grid, and disabled if you choose your own skin
  not name Grid. Otherwise overrides skin Grid accordingly.

- Rounded, should be named circle
  This will circle the main image display, reasonable for small carousels, maybe
  with a small caption below to make it nice. Use slidesToShow option > 2.
  Expecting square images.

If you want to attach extra 3rd libraries, e.g.: image reflection, image zoomer,
more advanced 3d carousels, etc, simply put them into js array of the target
skin. Be sure to add proper weight, if you are acting on existing slick events,
normally < 0 (slick.load.min.js) is the one.

See slick.slick.inc for more info on skins.


HTML STRUCTURE
--------------------------------------------------------------------------------
Note, non-BEM classes are added by JS.
Before Slick 1.4:
-----------------
<div class="slick slick-processed slick-initialized slick-slider">
  <div class="slick__slide"></div>
  <nav class="slick__arrow"></nav>
</div>


After Slick 1.4:
-----------------
<div class="slick slick-processed">
  <div class="slick__slider slick-initialized slick-slider">
    <div class="slick__slide"></div>
  </div>
  <nav class="slick__arrow"></nav>
</div>

At both cases, asNavFor should target slick-initialized class/ID attributes.


HOW CAN YOU HELP?
--------------------------------------------------------------------------------
Please consider helping in the issue queue, provide improvement, or helping with
documentation.


BUG REPORTS OR SUPPORT REQUESTS
--------------------------------------------------------------------------------
A basic knowledge of Drupal site building is required, and Slick can't help you.
When you don't know how to build things, you may be tempted to think it is bug.

Please refer to the provided README, including descriptions on each form item,
also the relevant guidelines from the supported modules.

If you do have bug reports, we love bugs, please be sure to:
 o provide steps to reproduce it,
 o more detailed info such as screenshots of the output and Slick form, or words
   to identify it any better,
 o make sure that the bug is caused by the module.

For the Slick library bug, please report it to the actual library:
  https://github.com/kenwheeler/slick

You can create a fiddle to isolate the bug if reproduceable outside the module:
  http://jsfiddle.net/

For the support requests, detailed info on the problem is helpful.
Shortly, you should kindly help me with detailed info to help you. Thanks.


TROUBLESHOOTING
--------------------------------------------------------------------------------
- When upgrading from Slick v1.3.6 to later version, try to resave options at:
  o admin/config/media/slick
  o admin/structure/types/manage/CONTENT_TYPE/display
  o admin/structure/views/view/VIEW
  only if trouble to see the new options, or when options don't apply properly.
  This is most likely true when the library adds/changes options, or the module
  does something new.

- Always clear the cache, and re-generate JS (if aggregation is on) when
  updating the module to ensure things are picked up:
  o admin/config/development/performance

- For (pre-)alpha users only: Frontend type juggling is removed [#2497945]:
  - Please re-save and re-export optionsets if you are a pre-alpha user who
    stored optionsets in codes before alpha release on 2015-3-31, or precisely
    before 2015-3-2 commit:
    http://cgit.drupalcode.org/slick/commit/?id=f08c3b4

  - Please ignore if you:
    o are a (pre-)alpha user who stored optionsets in codes after alpha.
    o never stored/exported optionsets in codes.

- If switching from beta1 to the latest via Drush fails, try the good old UI.
  Be sure to clear cache first, then run /update.php, if broken slick.

- If you are customizing templates, or theme funtions be sure to re-check
  against the latest.

- A Slick instance may be cached by its ID, and will only take place if you
  set the "Cache" to some value than None. Having two different slicks with the
  same ID will cause the first one cached override the second.
  IDs are guaranteed unique if using sub-modules. However if you do custom works,
  or input one at Slick Views UI, be sure to have unique IDs as they should be.
  Be sure no useless/ sensitive data such as "Edit link" as they may be rendered
  as is regardless permissions.

- Current slide previously has a workaround class "slide--current". Core added
  "slick-current" later (a year or so) at v1.5.6. Now (8/25/15) "slide--current"
  is dropped for "slick-current", but the workaround is still kept due to core
  known issue with asNavFor and nested slicks not having proper "slick-current".
  If you use "slide--current" before, be sure to update it to "slick-current".


KNOWN ISSUES
--------------------------------------------------------------------------------
- The Slick lazyLoad is not supported with picture-enabled images. Slick only
  facilitates Picture to get in. The image formatting is taken over by Picture.
  If you want advanced lazyload, but not willing to use Picture, do preprocess
  with theme_image_lazy() and use lazy 'advanced' to override it and DIY.
  Please see theme_image_lazy() for more info.
- Photobox is best for:
  - infinite true + slidesToShow 1
  - infinite false + slidesToShow N
  If "infinite true + slidesToShow > 1" is a must, but you don't want dup
  thumbnails, simply override the JS to disable 'thumbs' option.
- The following is not module related, but worth a note:
  o lazyLoad ondemand has issue with dummy image excessive height.
    Added fixes to suppress it via CSS.
  o If the total < slidesToShow, Slick behaves. Previously added a workaround to
    fix this, but later dropped and handed over to the core instead.
  o Fade option with slideToShow > 1 will screw up.
  o variableWidth ignores slidesToShow.
  o Too much centerPadding at small device affects slidesToShow.


UNKNOWN ISSUES
--------------------------------------------------------------------------------
- Anything I am not aware of.
  Please report if you find one. Your report and help is any module QA. Thanks.


PERFORMANCE
--------------------------------------------------------------------------------
Any module, even the most innocent one, that provides settings in the UI needs
to store them in a table. The good thing is we can store them in codes.

With Bulk exporter, or Features, optionsets may be stored in codes to avoid
database lookup. Be sure to revert to Default via UI to avoid database lookup.
It is analog to Drupal 8 CMI, so it is the decent choice today.

See slick_example for the stored-in-code samples.

Store large array of skins at my_module.slick.inc to get advantage of Drupal
autoloading while short ones should be left in the main module file so that
they are always available.

Most heavy logic were already moved to backend, however slick can be optimized
more by configuring the "Cache" value per slick instance.

Ditch all the slick logic to cached bare HTML:
1. Persistent: the cached content will persist (be displayed) till the next
   cron runs, best for static contents where freshness is no use, such as logo,
   team, profile video, more permanent home slideshows, etc.
2. Any number: slick is expired (detached from cached contents) by the selected
   expiration time, and fetches fresh contents till the next cache rebuilt.
   If stale cache is not cleared, slick will keep fetching fresh contents.

Be sure to have a working cron job to clear stale cache, so that slick is loaded
from the correct cached version. At any rate, cached contents will be refreshed
regardless of the expiration time after the cron hits due to the nature of cron.
Leave it empty to disable caching.


CURRENT DEVELOPMENT STATUS
--------------------------------------------------------------------------------
A full release should be reasonable after proper feedbacks from the community,
some code cleanup, and optimization where needed. Patches are very much welcome.


ROADMAP
--------------------------------------------------------------------------------
- Bug fixes, code cleanup, optimization, and full release.
- Get 1.x out of dev.
- Drupal 8 port.



AUTHOR/MAINTAINER/CREDITS
--------------------------------------------------------------------------------
Slick 7.x-2.x-dev by gausarts, inspired by Flexslider with CTools integration.
Slick 7.x-1.x-dev by arshadcn, the original author.

With the help from the community:
- https://www.drupal.org/node/2232779/committers
- CHANGELOG.txt for helpful souls with their patches, suggestions and reports.


READ MORE
--------------------------------------------------------------------------------
See the project page on drupal.org: http://drupal.org/project/slick.

More info relevant to each option is available at their form display by hovering
over them, and click a dark question mark.

See the Slick docs at:
- http://kenwheeler.github.io/slick/
- https://github.com/kenwheeler/slick/

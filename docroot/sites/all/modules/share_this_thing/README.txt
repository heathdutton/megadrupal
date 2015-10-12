Share This Thing

Features
========
* "Share This Thing" is a simple module for exposing sharing options for the
  selected content-types.

* Creates a new "view_mode" for content to be embedded. So you can really choose
  what content should be displayed in those IFRAMEs.

* IFRAME width and height values are configurable separately for each
  content-type.

* Supports "Shorten" and "Yourls" modules for URL shortening options. In absence
  of these modules, exposes non-aliased full URL.
  i.e. http://example.com/node/123

* Provides an option for you to include your favorite sharing and/or bookmarking
  services below the Short URL and Embed code fields.


Dependencies
============
* Chaos tool suite (ctools) <http://drupal.org/project/ctools>


Recommended
===========
To provide short URLs, either install and configure one of these modules, or
implement your own solution using theme_share_this_thing_link().

* Shorten URLs <http://drupal.org/project/shorten>
* Yourls <http://drupal.org/project/yourls>


Known Issues
============
The widgets added in "Additional HTML" field may require you to manually include
a method to re-initialize the widgets.

The reason for this is the Ctools modal window uses AJAX to load its content.
When the modal window is closed, the referenced JavaScript libraries would stay
attached to the DOM. However, those widgets may need to be re-initialized to
function properly.

See API documentation for those widgets.

AddThis
http://support.addthis.com/customer/portal/articles/381263-addthis-client-api

ShareThis
http://support.sharethis.com/customer/portal/articles/439323-api-overview


Solution for AddThis:
---------------------
Include following JavaScript below the AddThis snippet:

// Re-initialize toolbox and/or counter.
if (window.addthis) {
  addthis.toolbox(".addthis_toolbox");
  addthis.counter(".addthis_counter");
}


Configuration and integration examples
======================================
AddThis integration example:

To make sure AddThis buttons inherit to correct content title and URL, include
addthis:title and addthis:url attributes respectively to the .addthis_toolbox
element.

See "Configuration Inheritance" at Addthis Client API documentation:
http://support.addthis.com/customer/portal/articles/381263-addthis-client-api#attribute-inherit

Include following JavaScript before AddThis (re-)initialized:

(function ($) {
  $("#share-this-thing-form").each(function() {
    $(".addthis_toolbox", this).attr({
      "addthis:url": $(".stt-share-link", this).val(),
      "addthis:title": $(".stt-share-link", this).attr("title")
    });
  })
})(jQuery);


API
===
Following theming function are available for you to customize "Share This Thing"
in your theme:

theme_share_this_thing_title()
theme_share_this_thing_embed()
theme_share_this_thing_link()
template_preprocess_share_this_thing_html()


Change modal window title:

theme('share_this_thing_title', array('title' => 'Your custom window title'));


Sponsorship
===========
This project is developed by Osman Gormus and sponsored by Project6 Design, Inc.,
a leading Drupal design firm in the San Francisco Bay Area.
Visit us at www.project6.com or contact us at drupal@project6.com

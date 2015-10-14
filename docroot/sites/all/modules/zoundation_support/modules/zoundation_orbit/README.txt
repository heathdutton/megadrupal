Zoundation Orbit Integration

Currently this module assumes that the theme will include the necessary
jquery.foundation.orbit.js file. We are considering if this approach makes sense
and whether or not to integrate with libraries API and avoid the theme
dependency. Given that this module is designed to work with the zoundation theme
this is a fair initial assumption.

This adds a field display formatter that can be applied to image and text fields
to create foundation oribit sliders. Currently this does not support captions as
it is our opinion that short of views integration there is not a good way to
associate images with captions.

Most of the orbit settings are exposed and can be edited via a content types
manage display opttions (admin/structure/types/manage/CONTENT-TYPE/display).

Note that if you choose to create content slides you must pick an aspect ratio
for the slides to render properly. For best results orbit image sliders should
have a uniform size this can be done by configuring an appropriate image style
(admin/config/media/image-styles) such that all slides are the exact same size.

Zurb Foundation Orbit page;
http://foundation.zurb.com/docs/orbit.php

== Views Configuration ==

Note that there is a field formatter and a views display style. You do not want
to use both of these in your views configuration.

=== Style formatter ===

The style formatter is intended to be used when you want full control over what
is displayed in slides. Note that more advanced formatting will be deferred to
the Field configuration. Of particular interest will be the field
"Style Settings" and "Rewrite Results".

We have deferred making any assumptions here so that the site maintainer will
have full control over all slide details via the views field configuration
settings.

=== Field Formatter ===

The field formatter will be used when you have a multi-valued field that you may
want to display as slideshow content. Generally this will be on single entity
displays, but you may want to use this for multiple listings with individual
slideshows per view display row. In this case you can have multiple slideshows
via the field display.

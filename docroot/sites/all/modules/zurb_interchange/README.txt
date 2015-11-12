ZURB Interchange
================

Interchange uses media queries to load the images that are appropriate for a user's browsers, making responsive images a snap.

Official documentation: http://foundation.zurb.com/docs/components/interchange.html
More info: http://zurb.com/article/1211/say-goodbye-to-painful-image-loads-on-sma

Configuration
=============

ZURB Interchange adds settings to Image field types in the Field UI display settings under Manage Display.

For images you want to enable this for, simply check off 'Enabled' next to each image style you want to use with Interchange, and set the order of the styles for this field.
The ordering is important, because the smallest sized image should be loaded first, in order of smallest to largest. Interchange will determine what image is referenced in
'src' based on the display resolution / device width being used, and swap accordingly for responsive design.

If you want to use the images as a background image, check off the setting "Set as a background image with Zurb Interchange". Drupal will
format the image as a div tag instead of img, and Interchange will handle the rest. You will need to add your own background CSS properties aside from
background-image, such as position and repeat. More information on this can be found in the Zurb Interchange documentation.

If you want to use your own custom media queries, you can, by declaring them with Javascript as outlined in the documentation. This is the easiest way
to achieve more complex configuration.
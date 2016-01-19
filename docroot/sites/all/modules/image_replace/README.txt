Image Replace
=============

Image Replace provides a way to supply optional alternative source images
mapped to image styles. This is useful when building responsive sites with
"art directed"[1] images where cropping and resizing is not enough but images
need to be swapped out completely. For example when delivering graphics
containing rendered text.

This module works well with responsive image modules leveraging image styles,
for example the Picture[2] module.

A simple configuration based on standard profile
------------------------------------------------

1. Download and configure Ctools, Breakpoints, Picture.
    * Add some breakpoints and create a breakpoint group.

2. Assign the existing image styles Thumbnail, Medium, Large to the
    breakpoints in the picture settings.

3. Modify image styles:
    * Large (480x480): Add the "Replace image" as the first effect.
    * Thumbnail: Thumbnail (100x100) -> Field: Image Thumbnail

4. Add two additional image fields onto the article content type:
    * field_image_small
    * field_image_large
    * Remove both from all view modes.

5. Specify image replace settings on the existing image field (field_image)
    and specify the following style -> field mapping:
        * Style: Thumbnail (100x100) -> Field: Image Thumbnail
        * Style: Large (480x480) -> Field: Image Large

6. Choose the Picture formatter for the image field on the article full view
    mode.

Please report issues to the bug tracker linked on the project homepage:
https://drupal.org/project/image_replace

1) Use Cases and Requirements for Standardizing Responsive Images - Art direction
   http://usecases.responsiveimages.org/#art-direction
2) Picture module:
   https://drupal.org/project/picture

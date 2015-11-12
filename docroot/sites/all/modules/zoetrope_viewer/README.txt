Zoetrope Viewer Module

SUMMARY
-------
The Zoetrope viewer module provides fields, field formatters and tokens for the
zoetrope viewer. It's made specifically for use with the engage photography
service from http://zoetrope.io.

REQUIREMENTS
------------
1. Entity Tokens, if tokens are required

INSTALLATION
------------
 * Install as you would normally install a contributed drupal module. See:
  https://drupal.org/documentation/install/modules-themes/modules-7
  for further information.

CONFIGURATION
-------------
Add a `Zoetrope ID` field to an entity, using the `Zoetrope Engage Viewer` as
the display type.

DISPLAY FORMATTERS
------------------
Zoetrope images are always square, though the size is configurable, using
standard image styles. Image styles are not actually processed for Zoetrope
images - only the sizes are honored. Any changes to the image styling need to
be done with CSS or by using an alternate trigger image.

If you wish to display a Zoetrope image if it exists, or fall back to a static
shot if it doesn't, this should be done using a template file.
e.g. if your engage image field is called `field_zoetrope_engage_image` and your
legacy/backup image field is called `field_image`.

```
  <?php if (empty($content['field_zoetrope_engage_image'])) {?>
    <div class="product-image flat-shot">
      <?php render($content['field_image']); ?>
    </div>
  <?php }
    else {?>
    <div class="product-image engage">
      <?php render($content['field_zoetrope_engage_image']); ?>
    </div>
  <?php } ?>
```

If you want to combine a list of regular images with Zoetrope images for
display, an image formatter `Image + Zoetrope Images` is provided to merge a
Zoetrope ID field into an image field for display.

TOKENS
------
The main use case for the tokens in this module is to use them in conjunction
with the metatags module.

Provided Tokens (replace node with any entity type, e.g. commerce_product,
if you have an image on a commerce_product):
*. `[node:field_name:preveiw_uri]` - The 1000px preview image url
*. `[node:field_name:zoetrope_uuid]` - The Zoetrope UUID of the image
*. `[node:field_name:starting_position]` - The index of the starting position

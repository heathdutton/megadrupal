This module will resize uploaded images to be below the set dimensions. It is
not an image style or other presentation layer module. If you want to maintain
the original image you should use
[Image Resize Filter](https://www.drupal.org/project/image_resize_filter) or an
image style. The use case for this module is sites where users may upload very
large images, but you do not want/need to keep the original.

This is different from the core image field size restrictions in that it will
work on already uploaded files and imported content.

## Related

These modules will allow you to adjust the site of the displayed image while
leaving the original untouched:

 * [Image Resize Filter](https://www.drupal.org/project/image_resize_filter)
 * [Imagecache](https://www.drupal.org/project/imagecache)

## Usage

Simply enable the module and it will begin resizing images when they are
uploaded. It will scan for images above the configured dimensions once a day and
queue them to be resized.

By default this module will resize images to be under 2560x1600. If you want to
change the dimensions it uses then you will need to update the module's
configuration. You can do this either through the interface, or by editing your
settings.php and setting the following:

```
<?php
  $conf['max_image_size_width'] = 2560;
  $conf['max_image_size_height'] = 1600;
?>
```

### Drush

There are two drush commands available:

`drush misq`: Queue images to process.
`drush misp`: Process images.

You can get more help and information by typing `drush help <command>`.

## This project has been sponsored by:

**McMurry/TMG**
  McMurry/TMG is a world-leading, results-focused content marketing firm. We
  leverage the power of world-class content — in the form of the broad
  categories of video, websites, print and mobile — to keep our clients’ brands
  top of mind with their customers.  Visit http://www.mcmurrytmg.com for more
  information.

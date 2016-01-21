# Drupal Module: Auto Retina
**Author:** Aaron Klump  <sourcecode@intheloftstudios.com>

##Summary
**Extends core image styles/effects by providing retina versions of any defined style, by simply adding `@2x` before the file extension, e.g. `some-great-file@2x.png`.  The resulting image is double as wide as the width defined in the image style effect.**

_This module can't do magic, so be aware that all source image widths should be at least double the width of your image style effect.  The upsampling rules are taken from the effect setting, so if upsampling is disabled, the @2x derivative image will not be upsampled either.  And visa versa.  You can check Reports > recent log messages to look for retina images of poor quality; filter by module = auto_retina._

You may also visit the [project page](http://www.drupal.org/project/auto_retina) on Drupal.org.

## What this module is not
* This module will not detect retina devices.
* This module will not output html tags for your images.

## What this module is
It simply provides the retina version of every image style you define, with not extra work on your part.

## Requirements
1. Depends on the Drupal 7 core image module.

## Installation
1. Install as usual, see [http://drupal.org/node/70151](http://drupal.org/node/70151) for further information.

## Configuration
1. This module leverages the `administer image styles` permission for making configuration changes.
1. The suffix can be changed from the default `@2x` by visiting the configuration page.
1. You can make these settings available to Javascript files by enabling the option in the advanced settings.  This will provide a `Drupal.settings.autoRetina` object, and can be handy for exposing configuration to your js files that deal with Retina images.

## Suggested Use
Once enabled, visit the image url of any derivate image, modify the url by prepending the extension with '@2x', and you should see the image double in width.

As an example, if the following produced a derivate image at 100px wide:

    sites/default/files/styles/thumbnail/public/images/my-great-photo.jpg?itok=hpMKPMBm

You would change the url to this, and see an image at 200px wide:

    sites/default/files/styles/thumbnail/public/images/my-great-photo@2x.jpg?itok=hpMKPMBm

**Please note that you must include the `itok` param when visiting a derivate url for the first time.  This is a requirement of the image module.  See `image_style_deliver()` for more info, reading about `image_allow_insecure_derivatives`.**

## Poor Quality (and Recent log messages)
When a retina image is generated and the quality could be better with a larger source image, an entry will be made in the message log.  In this way you can identify which images need larger originals to provide the best retina quality.  To disable this feature add the following to `settings.php`.

    $conf['auto_retina_log'] = FALSE;


## Uninstall
Be aware that when you uninstall this module, it will NOT delete the retina derivatives that it has created.

## Developers
1. `auto_retina_image_style_create_derivative()` is meant to replace `image_style_create_derivative()` in your code, if you wish to take advantage of the functionality of this module at a programattic level.

## Design Decisions/Rationale
We needed a way to have @2x versions of the images on the server without extra work.

##Contact
* **In the Loft Studios**
* Aaron Klump - Developer
* PO Box 29294 Bellingham, WA 98228-1294
* _skype_: intheloftstudios
* _d.o_: aklump
* <http://www.InTheLoftStudios.com>
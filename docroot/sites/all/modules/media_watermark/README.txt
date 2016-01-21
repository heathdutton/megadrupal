CONTENTS OF THIS FILE
---------------------

 * Introduction
 * Installation



INTRODUCTION
------------

Current Maintainer: Bogdan Tur <bogdan.tur1988@gmail.com>

Media Watermark module give possibility to add watermark image to files uploaded
through media module. This module support batch operations while we upload
multiple images with watermarking using plupload widget. If we want to add
watermark while upload we just need to check "Add watermark" checkbox and choose
appropriate watermark. I should notice that watermark could be added only to
.png,.gif,.jpg,.jpeg image files, and watermarking file should be .png or .gif
with transparency.


INSTALLATION
------------

1. This module REQUIRES media module and file_entity (not older then
versions 7.x-2.x).

2. Copy this media_watermark/ directory to sites/SITENAME/modules directory.

3. Enable the media_watermark as any other drupal module.

4. After installation you can find default watermarks if you will follow this
path file/add and check "Add watermark" checkbox. It is made to make easier
working with this module. You can edit any watermark if you will click on its
image.

5. To add watermark follow this path admin/config/media/watermark/add.

6. To configure all watermarks follow this path admin/config/media/watermark.

# Gallery Archive

The Gallery Archive module allows users with the appropriate permissions to
download an archive of a particular image gallery, with the original-size
images in a .tar.gz (or, in the future, .zip) file.

The module works only with image galleries set up using the node-as-an-image-
gallery format, where a node has an image field with multiple images.

More information about this module, including the latest release, can be found
at http://drupal.org/project/gallery_archive.

## Installation

  1. Visit the settings page, at admin/config/media/gallery-archive, and select
     your gallery content type, and the image field you're using for images.

  2. Give users the 'Download gallery archive' permission to allow them to view
     the 'Download Gallery' tab on image gallery pages.

## Important Notes

  - Gallery archives are never removed by this module by default. You can change
    this setting on the settings page, under the 'Deletion Frequency' option. If
    you need to change the frequency of the deletion (for instance, if galleries
    often have new pictures added, and you'd like the archive to be rebuilt
    every hour), you can use a module like Elysia Cron to set a different
    interval for the Gallery Archive module's cron run.

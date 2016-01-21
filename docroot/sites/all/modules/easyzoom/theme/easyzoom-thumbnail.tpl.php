<?php
/**
 * @file
 * Template for an Image Zoom thumbnail image.
 *
 * Available variables:
 * - $image_url: The image url.
 * - $zoom_image_url: The path to the zoom image.
 */
?>

<div class="easyzoom-thumbnails">
  <a href="<?php print $zoom_image_url; ?>" data-standard="<?php print $image_url; ?>">
    <?php print $thumbnail; ?>
  </a>
</div>

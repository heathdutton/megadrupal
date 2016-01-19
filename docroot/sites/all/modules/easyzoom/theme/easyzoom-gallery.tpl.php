<?php

/**
 * @file
 * Template for an Image Zoom gallery image.
 *
 * Available variables:
 * - $image: The image tag.
 * - $zoom_image_url: The path to the zoom image.
 * - $class: The wrapper div classes.
 */
?>

<div class="<?php print $class; ?>">
  <a href="<?php print $zoom_image_url; ?>">
    <?php print $image; ?>
  </a>
</div>

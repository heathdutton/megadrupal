<?php

/**
 * @file
 * Template for an Image Zoom gallery.
 *
 * Available variables:
 * - $image: The display image.
 * - $thumbs: An array of thumbnail images.
 */
?>

<?php print $image; ?>

<div id="imagezoom-thumb-wrapper">
  <ul class="imagezoom-thumbs">
    <?php foreach ($thumbs as $thumb): ?>
      <li class="imagezoom-thumb"><?php print $thumb; ?></li>
    <?php endforeach; ?>
  </ul>
</div>

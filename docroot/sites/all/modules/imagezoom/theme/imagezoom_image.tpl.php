<?php

/**
 * @file
 * Template for an Image Zoom image.
 *
 * Available variables:
 * - $image: The path to the display image.
 * - $zoom: The path to the zoom image.
 * - $width: The width of the thumbnail.
 * - $height: The height of the thumbnail.
 * - $alt: The alt text of the thumbnail.
 * - $title: The title of the thumbnail.
 */
?>

<img src="<?php print $image; ?>" data-zoom-image="<?php print $zoom; ?>" class="imagezoom-image" <?php if ($width): ?>width="<?php print $width; ?>" <?php endif; ?> <?php if ($height): ?>height="<?php print $height; ?>" <?php endif; ?> <?php if ($alt): ?>alt="<?php print $alt; ?>" <?php endif; ?> <?php if ($title): ?>title="<?php print $title; ?>" <?php endif; ?> />
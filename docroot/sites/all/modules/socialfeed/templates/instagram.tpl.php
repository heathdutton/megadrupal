<?php

/**
 * @file
 * This template handles the layout of Social Feed instagram block.
 *
 * Variables available:
 * - $instagram_images: An array of the Instagram pictures.
 * - $instagram_image: A single picture fetched from $instagram_image.
 */
?>
<ul>
<?php foreach ($instagram_images as $instagram_image) : ?>
	<li>
		<img src="<?php echo $instagram_image; ?>">
	</li>
<?php endforeach; ?>
</ul>

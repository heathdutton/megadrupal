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
    <?php if (isset($instagram_image['post_url']) && !empty($instagram_image['post_url'])) : ?>
    <a href="<?php echo $instagram_image['post_url']?>"><img src="<?php echo $instagram_image['image_url']; ?>"></a>
    <?php else: ?>
    <img src="<?php echo $instagram_image['image_url']; ?>">
    <?php endif; ?>
  </li>
<?php endforeach; ?>
</ul>

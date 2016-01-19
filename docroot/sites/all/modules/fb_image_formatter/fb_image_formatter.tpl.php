<?php
/**
 * @file
 * Template file for Facebook-like image format.
 *
 * @var $images []
 */

/**
 * Created by PhpStorm.
 * User: alan
 * Date: 11/4/14
 * Time: 8:30 PM
 */

?>
<div class='fb_image_formatter'>
  <?php if (isset($images) and is_array($images)): ?>
    <?php foreach ($images as $image): ?>
      <a class='colorbox'
         rel='lightshow[<?php echo $image['album']; ?>]'
         href='<?php echo $image['original']; ?>'>
        <div class='fb_image_formatter_item'
             style='background-image:url(<?php echo $image['thumbnail']; ?>)'>
          <div class='fb_image_formatter_item_title'>
            <?php echo $image['album']; ?>
          </div>
        </div>
      </a>
    <?php endforeach; ?>
  <?php endif; ?>
</div>

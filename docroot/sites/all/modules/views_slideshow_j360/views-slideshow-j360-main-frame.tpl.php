<?php
/**
 * @file
 * Displays a single j360.
 *
 * Available variables:
 * - $view: The view object.
 * - $options: Style options. See below.
 * - $rows: The output for the rows.
 * - $title: The title of this group of rows. May be empty.
 * - $id: The unique counter for this view.
 */
?>

<div id="views-slideshow-j360-<?php echo $id; ?>" class="views-slideshow-j360">

  <?php if (!empty($title)) : ?>
    <h3><?php echo $title; ?></h3>
  <?php endif; ?>

  <div id="views-slideshow-j360-images-<?php echo $id; ?>" class="views-slideshow-j360-images <?php echo $classes; ?>">

    <?php if (isset($images)) : ?>
      <?php foreach ($images as $image) : ?>
        <?php echo $image; ?>
      <?php endforeach; ?>
    <?php endif; ?>

    <?php if (isset($width)) : ?>
      <input type="hidden" id="j360-img-width" name="j360-img-width" value="<?php echo $width; ?>" />
      <input type="hidden" id="j360-img-height" name="j360-img-height" value="<?php echo $height; ?>" />
    <?php endif; ?>

  </div><!-- // end views-slideshow-j360-images -->
</div><!-- // end views-slideshow-j360 -->

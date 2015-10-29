<?php
/**
 * @file
 * Template file for field_slideshow.
 * @see restaurant_radix_preprocess_field_slideshow().
 */
?>
<div class="field-slideshow-wrapper carousel">
  <?php if ($controls_position == "before")  print(render($controls)); ?>
  <?php if ($pager_position == "before")  print(render($pager)); ?>

  <div class="<?php print $classes; ?>">
    <?php foreach ($items as $item) : ?>
      <div class="<?php print $item['classes']; ?>" style="background-image: url('<?php print $item['image_url']; ?>'); width: 400px; height: 300px;">
        <?php if (!empty($item['caption'])) : ?>
          <div class="container">
            <div class="caption">
              <?php print $item['caption']; ?>
            </div>
          </div>
        <?php endif; ?>
      </div>
    <?php endforeach; ?>
  </div>

  <?php if ($controls_position != "before") print(render($controls)); ?>
  <?php if ($pager_position != "before") print(render($pager)); ?>
</div>

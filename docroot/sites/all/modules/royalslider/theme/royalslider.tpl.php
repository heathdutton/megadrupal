<?php
/**
 * @file
 * RoyalSlider template.
 *
 * Available variables:
 *
 * These are the main variables used to output this template. They aren't
 * defined in royalslider_theme() and are instead added in a preprocess:
 * royalslider_preprocess_royalslider().
 *
 *  - $attributes_array array An array of attributes to apply to the wrapper.
 *  - $items_processed array An array of item render arrays.
 *
 * These are "internal" variables and shouldn't really be used directly.
 * However, they're here in case you want to go nuts.
 *
 *  - $optionset object A RoyalSlider Option Set object.
 *  - $skin string The machine_name of the active skin.
 *  - $items array The raw array of items that make up the slideshow.
 */
$i = 1;
?>
<div<?php print drupal_attributes($attributes_array)?>>
  <?php foreach ($items_processed as $item): ?>
    <div class="royalslider-item rsContent item-<?php print $i++; ?>">
      <?php print drupal_render($item); ?>
    </div>
  <?php endforeach; ?>
</div>

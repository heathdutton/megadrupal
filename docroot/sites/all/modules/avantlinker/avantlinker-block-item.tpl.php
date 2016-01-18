<?php
/**
 * @file
 * HTML for items in the related products block listing.
 *
 * Available variables:
 * - $classes: String of classes that can be used to style contextually through
 *   CSS. It can be manipulated through the variable $classes_array from
 *   preprocess functions. The default values can be one or more of the
 *   following:
 *   - avantlinker-block-item: The current template type, i.e., "theming hook".
 *   - $percent_off: Derived from retail price and sale price.
 *   - $product_name: From AvantLink API and unchanged.
 *   - $buy_url: From AvantLink API and unchanged.
 *   - $thumbnail_image: From AvantLink API and unchanged.
 *   - $sale_price: From AvantLink API and unchanged.
 *
 * - $classes_array: Array of html class attribute values. It is flattened
 *   into a string within the variable $classes.
 *
 *
 * @see template_preprocess_avantlinker_block_item()
 */

?>
<div class="<?php print $classes; ?>">
    <div class="rp_image">
      <a href="<?php print $buy_url; ?>" <?php print $target; ?>>
      <img src="<?php print $thumbnail_image; ?>" /></a>
    </div>
    <div class="rp_name">
      <span><a href="<?php print $buy_url; ?>" <?php print $target; ?>> <?php print $product_name; ?> </a></span>
    </div>
    <div class="prices"><a href="<?php print $buy_url; ?>" <?php print $target; ?>>
      <span class="sale_price"><?php print  $sale_price; ?></span>
      <?php
      if ($percent_off != 0) {
      ?>
      <span class="percent_off">&nbsp;<?php print $percent_off; ?>% Off</span>
      <?php
      }
      ?>
      </a>
  </div>
</div>

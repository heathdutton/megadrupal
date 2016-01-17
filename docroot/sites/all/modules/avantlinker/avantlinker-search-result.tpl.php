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
 *   - $brand_name: From AvantLink API unchanged.
 *   - $retail_price: From AvantLink API unchanged.
 *
 * - $classes_array: Array of html class attribute values. It is flattened
 *   into a string within the variable $classes.
 *
 *
 * @see template_preprocess_avantlinker_search_result()
 */

?>
<div class="<?php print $classes; ?>">
  <div class="psr_image"><a href="<?php print $buy_url; ?>" <?php print $target; ?>>
    <img src="<?php print $thumbnail_image; ?>" class="psr_product_image" /></a>
  </div>
  <div class="psr_product_info">
    <div class="psr_product_name"><a href="<?php print $buy_url; ?>" <?php print $target; ?>><?php print $product_name; ?></a>
    </div>
    <div class="psr_brand_name"><?php print $brand_name; ?>
    </div>
    <div class="psr_prices"><span class="psr_retail_price"><?php print $retail_price; ?></span>
<?php if ($sale_price != $retail_price) { ?>
          on sale for: <span class="psr_sale_price"><?php  print $sale_price; ?></span>
<?php
	}
?>
</div>
<div class="psr_description"><?php print $short_description; ?></div>
<div class="psr_merchant_name"><small><em><?php print $merchant_name; ?></em></small></div>
  </div>

</div>
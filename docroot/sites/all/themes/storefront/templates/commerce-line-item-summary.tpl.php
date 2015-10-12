<?php
/**
 * @file
 * Default implementation of a line item summary template.
 *
 * @param $quantity_raw
 *  The number of items in the cart.
 * @param $quantity_label
 *  The quantity appropriate label to use for the number of
 *  items in the shopping cart; "item" or "items" by default.
 * @param $quantity
 *  A single string containing the number and label.
 * @param $total_raw
 *  The raw numeric value of the total value of items in the cart.
 * @param $total_label
 *  A text label for the total value; "Total:" by default.
 * @param $total
 *  The currency formatted total value of items in the cart.
 * @param $links
 *  A rendered links array with cart and checkout links.
 * @param $view
 *  The View the line item summary is attached to.
 *
 * @see template_preprocess()
 * @see template_process()
 */

?>
<div class="line-item-summary">
  <?php if ($quantity_raw): ?>
  <div class="line-item-quantity">
    <span class="line-item-quantity-raw"><?php print $quantity_raw; ?></span> <span class="line-item-quantity-label"><?php print $quantity_label; ?></span>
  </div>
  <?php endif; ?>
  <?php if ($total): ?>
  <div class="line-item-total">
    <span class="line-item-total-label"><?php print $total_label; ?></span> <span class="line-item-total-raw"><?php print $total; ?></span>
  </div>
  <?php endif; ?>
</div>

  <div class="cart-block-links">
  <?php print $links; ?>
  </div>

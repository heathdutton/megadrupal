<?php

/**
 * @file
 */
?>
<div id="ec-cart-block-item-<?php echo $nid; ?>" <?php echo $attributes; ?>>
  <div class="ec-cart-item">
    <?php echo $link . $qty_multiplier . $qty; ?>
  </div>
  <div class="ec-cart-line-total">
    <?php echo $price; ?>
  </div>
</div>

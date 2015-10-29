<?php

/**
 * @file
 */
?>
<div class="ec-item-count">
  <?php echo $item_count; ?>
</div>
<div class="ec-cart-items">
  <div class="items">
    <?php echo implode("\n", $items); ?>
  </div>
  <div class="total-wrapper clearfix">
    <span class="total">
      <?php echo $total; ?>
    </span>
  </div>
  <div class="ec-checkout">
    <?php echo $checkout; ?>
  </div>
</div>

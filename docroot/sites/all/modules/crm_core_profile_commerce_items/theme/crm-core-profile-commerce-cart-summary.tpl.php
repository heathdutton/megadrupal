<?php

/**
 * @file
 * Default implementation of a crm_core_profile_commerce cart summary template.
 *
 * Available variables:
 * - $total_label: A text label for the total value; "Total:" by default.
 * - $total: The currency formatted total value of items in the cart.
 * - $total_raw: The currency total value of items in the cart.
 *
 * @see template_preprocess()
 * @see template_process()
 */
?><div class="cart-summary">
  <?php if (isset($total)): ?>
  <div class="cart-summary-total">
    <span class="cart-summary-total-label"><?php print $total_label; ?></span>
    <span class="cart-summary-total-value"><?php print $total; ?></span>
  </div>
  <?php endif; ?>
</div>

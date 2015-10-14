<?php
/**
 * @file
 * Template file for the checkout/payment page item.
 *
 * Available variables:
 *
 * @var $summary: A summary table containing the item(s), taxes/adjustments, and total.
 * @var $use_coupons: Whether or not coupons should be used.
 * @var $coupon_widget: The form where the user can enter a coupon.
 * @var $payment_button: The payment button.
 */

?>
<!-- membersify-checkout-page template -->
<div class="membersify_checkout_page">
  <div class='membersify_checkout_summary'>
    <?php print $summary; ?>
  </div>

  <?php if ($use_coupons) { ?>
  <div class='membersify_checkout_coupon_widget'>
    <?php print $coupon_widget; ?>
  </div>
  <?php } ?>

  <div class='membersify_checkout_payment_button'>
    <?php print $payment_button; ?>
  </div>
</div>
<!-- /membersify-checkout-page template -->

<?php

/**
 * @file
 * Template for the checkout page.
 *
 * Variables:
 * ----------
 *
 * @var $checkout_steps
 *   The HTML for the checkout steps widget.
 * @var $order_details
 *   The table with the order details and quantity and option widgets.
 * @var $checkout_fields
 *   Any fields that are on the checkout form, such as the order information.
 * @var $checkout_form
 *   The checkout form, which can include the payment gateways, user and
 *   shipping information, coupon fields, etc.
 */
?>

<div id='ms-core-checkout-page'>
  <div id='ms-core-checkout-page-steps'>
    <?php print $checkout_steps; ?>
  </div>
  <div id='ms-core-checkout-page-order-details'>
    <?php print $order_details; ?>
  </div>
  <div id='ms-core-checkout-page-fields'>
    <?php print $checkout_fields; ?>
  </div>
  <div id='ms-core-checkout-page-form'>
    <?php print $checkout_form; ?>
  </div>
</div>

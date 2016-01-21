<?php
/**
 * @file
 * Template file for the purchase link.
 *
 * Available variables:
 *
 * @var $url: The purchase link url.
 * @var $plan: The plan object.
 */
?>
<!-- membersify-purchase-link template -->
<div class='membersify-purchase-link'>
  <a class="membersify_button" href='<?php print $url; ?>'>
    <?php print t('@name - @price', array('@name' => $plan->name, '@price' => membersify_get_price_string($plan->payment_plan))); ?>
  </a>
</div>
<!-- /membersify-purchase-link template -->

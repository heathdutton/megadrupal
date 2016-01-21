<?php
// $Id$

/**
 * Theme the PayPal payment page.
 * 
 * Available variables:
 *   $nid 			 the node id of the content being paid for, if this payment is to view a single node
 *   $bundle  	 the content type being purchased, if this payment is to create or view a content type
 *   $op	 			 the transaction type ('instance', 'type' or 'create')
 *   $button		 the button form
 */

$classes = "paypal-pay paypal-page paypal-pay-$op";
if ($bundle) {
  $classes .= " paypal-pay-$bundle";
}
if ($nid) {
  $classes .= " paypal-pay-$nid";
}
?>
<div class='<?php print $classes; ?>'>
	<div class='content'>
		<?php print t("This content requires payment for access.  Click on the PayPal button below to pay for this content.  You will be redirected to PayPal for the actual transaction."); ?>	
	</div>  
	<div class='paypal-form'>
		<?php print $button; ?>
	</div>
</div>

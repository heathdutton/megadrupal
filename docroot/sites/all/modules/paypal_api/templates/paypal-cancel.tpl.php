<?php
// $Id$

/**
 * Theme the cancel page, which is shown after a cancelled transaction.
 * 
 * ATTENTION:  You cannot trust this data to perform transaction logic on your site.  Do not assume sales are final
 * 						 based on information coming to this page.
 * 
 *             Instead, implement hook_paypal_payment(), which only gets called when the transaction has been verified with Paypal.
 *             
 *             This page should only serve as a courtesy to confirm that the transaction has been initiated with Paypal.
 *             
 * Available variables:
 * 
 * $nid: 		the node id (if present) of the content being purchased
 * $bundle: the content type of the content being purchased
 * $uid: 		the uid of the user making the purchase
 * $op: 		the operation carried out, ie 'create', 'view' or 'instance'
 */
?>
<div class='paypal-cancel'>
	<?php print t("Your transaction with Paypal was cancelled."); ?>
</div>
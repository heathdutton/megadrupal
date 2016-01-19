<?php

/**
 * @file
 * Template for the failure page
 *
 * In the case that the transaction was not accepted by Transbank, this page
 * will be displayed.
 *
 * - int $order_id: order id.
 * - string $redirection: the url for redirect.
 * - string $title_redirection: the text for the redirection.
 */
?>

<strong><?php print t('Failed Transaction')?></strong>
<table>
	<tr>
		<td><?php print t('Purchase order number')?></td>
		<td><?php print $order_id; ?></td>
	</tr>
</table>
<?php print t('The possible reasons for this failure are');?>:
<ul>
	<li><?php print t('Error in credit/debit card details entered (date and/or security code). Please re-enter your details and try to make the purchase again');?></li>
  <li><?php print t('Your credit/debit card does not have sufficient funds to complete the purchase');?></li>
  <li><?php print t('Your credit/debit card has not yet been authorized by your bank');?></li>
  <li><?php print t('If the problem persists please contact your bank');?></li>
</ul>
<?php if($redirection):?>
	<a class="webpay-redirect-link" href="<?php print url($redirection);?>"><?php print $title_redirection;?></a>
<?php endif;?>

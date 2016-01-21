<?php
/**
 * @file
 * Checkout.fi on-site gateway template.
 *
 * Available variables:
 * - $banks: Array of available payment providers. Following keys are available:
 *   - name: The name of this payment provider.
 *   - url: The POST url for this payment provider.
 *   - parameters: Rendered hidden form elements for this provider.
 *   - icon: Url to the logo of a payment provider.   
 * - $merchant: Array of merchant information
 *   provided by Checkout.fi. Available keys:
 *   - id: Merchant ID.
 *   - company: Company name.
 *   - name: Marketing name of company.
 *   - email: Merchant e-mail.
 *   - address: Merchant address.
 *   - vatId: Merchant VAT id.
 *   - helpdeskNumber: Help desk telephone number. 
 * - $payment: The payment object that initiated the payment execution.
 */
?>
<div id="checkoutFiGateway">
	<?php foreach($banks as $bank):?>
    <div class="bankButton">
      <form action='<?php print $bank['url'];?>' method='post'>
        <?php print $bank['parameters'];?>
        <span><input type='image' src='<?php print $bank['icon'];?>'> </span>
        <div>
          <?php print $bank['name'];?>
        </div>
      </form>
    </div>
	<?php endforeach;?>
</div>
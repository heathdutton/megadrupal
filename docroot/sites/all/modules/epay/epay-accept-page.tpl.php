<?php

/**
 * @file epay-accept-page.tpl.php
 *
 * Theme implementation to display the ePay accept page
 *
 * Available variables:
 * - $transaction: Full transaction object with all info about
 *   the transaction.
 *
 * @see template_preprocess()
 * @see template_preprocess_epay_accept_page()
 */
?>
<div id="epay-accept-page-<?php print $transaction['api_module']; ?>-<?php print $transaction['api_delta']; ?>" class="epay-accept-page clear-block">
  <p><?php print t('Your payment was received. Thank you for doing business with us.'); ?></p>
</div>

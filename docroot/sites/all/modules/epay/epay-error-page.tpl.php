<?php

/**
 * @file epay-error-page.tpl.php
 *
 * Theme implementation to display the ePay accept page
 *
 * Available variables:
 * - $transaction: Full transaction object with all info about
 *   the transaction.
 *
 * @see template_preprocess()
 * @see template_preprocess_epay_error_page()
 */
?>
<div id="epay-error-page-<?php print $transaction['api_module']; ?>-<?php print $transaction['api_delta']; ?>" class="epay-error-page clear-block">
  <p><?php print t('An error occurred while verifying the payment.'); ?></p>
</div>

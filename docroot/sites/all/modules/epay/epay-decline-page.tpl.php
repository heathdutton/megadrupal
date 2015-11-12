<?php

/**
 * @file epay-decline-page.tpl.php
 *
 * Theme implementation to display the ePay accept page
 *
 * Available variables:
 * - $transaction: Full transaction object with all info about
 *   the transaction.
 *
 * @see template_preprocess()
 * @see template_preprocess_epay_decline_page()
 */
?>
<div id="epay-decline-page-<?php print $transaction['api_module']; ?>-<?php print $transaction['api_delta']; ?>" class="epay-decline-page clear-block">
  <p><?php print t('The payment was cancelled.'); ?></p>
</div>

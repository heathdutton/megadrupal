<?php

/**
 * @file dibs-cancel-page.tpl.php
 *
 * Theme implementation to display the dibs cancel page
 *
 * Available variables:
 * - $form:  The whole form element as a string.
 * - $settings: Full DIBS settings array.
 * - $transaction: Full transaction array with all info about
 *   the transaction.
 *
 * @see template_preprocess()
 * @see template_preprocess_dibs_cancel_page()
 */
?>
<div id="dibs-cancel-page-<?php print $transaction['api_module']; ?>-<?php print $transaction['api_delta']; ?>" class="dibs-cancel-page clear-block">
  <p><?php print t('The payment was cancelled. Please click the button below to retry.'); ?></p>
  <?php print render($form); ?>
</div>

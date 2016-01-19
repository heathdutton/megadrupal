<?php

/**
 * Template file for the seller dashboard page.
 *
 * Available variables:
     $balance
     $available
     $commissions
     $account
     $payment_options
     $min_payout
     $payment_link
 */
?>

<div id="affiliate-sales-header">
  <h2><?php print t('Account Overview'); ?></h2>
</div>

<?php
  print t('Balance: @amount (@avail Available)',
    array(
      '@amount' => $balance,
      '@avail' => $available,
    ));
  if (ms_affiliates_request_payment_access_test()) {
    print ' - ' . $payment_link;
  }

  if (variable_get('ms_affiliates_min_payout', 0)) {
    print "<br />" . t('Minimum Payout') . ': ' . $min_payout;
  }
?>

<div class='marketplace_payment_options'>
  <?php print $payment_options; ?>
</div>

<div id="marketplace-sales-header">
  <h2><?php print t('Sales Overview'); ?></h2>
</div>

<div class='marketplace_sales_overview_item'>
  <?php print t('Sales Today') . ': ' . $commissions['today']; ?>
</div>

<div class='marketplace_sales_overview_item'>
  <?php print t('Sales last 7 Days') . ': ' . $commissions['7days']; ?>
</div>

<div class='marketplace_sales_overview_item'>
  <?php print t('Sales last 365 Days') . ': ' . $commissions['365days']; ?>
</div>

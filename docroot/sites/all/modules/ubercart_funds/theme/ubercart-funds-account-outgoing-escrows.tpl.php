<?php

/**
 * @file
 * definition of ubercart-funds-account-outgoing-escrows.tpl.php.
 */
$user = user_load($variables['uid']);

global $base_url;

$transactions = db_query("SELECT * FROM {ubercart_funds_transactions} WHERE uid='$user->uid' AND type='Escrow Payment'");

$header = array(t('Time'), t('Amount'), t('Allocated to'), t('Details'), t('Operations'));

$rows = array();

foreach ($transactions as $transaction) {
  $to_user = user_load($transaction->reference);
  $rows[] = array(
      date('d/m/Y   g:i:s A', $transaction->created), uc_currency_format/*commerce_currency_format*/($transaction->amount/100/*, commerce_default_currency()*/), $to_user->mail, '<a href="' . $base_url . '/user/funds/release-escrow/' . $transaction->transaction_id . '">Release Payment</a>', $transaction->notes
  );
}

print t('<h2>Outgoing Escrows</h2>');

print theme('table', array('header' => $header, 'rows' => $rows));
?>


<?php

/**
 * @file
 * definition of ubercart-funds-account-transactions.tpl.php.
 */
$user = user_load($variables['uid']);

$transactions = db_query("SELECT * FROM {ubercart_funds_transactions} WHERE uid='$user->uid'");

$header = array(t('Time'), t('Amount'), t('Transaction Type'), t('Details'));

$rows = array();

foreach ($transactions as $transaction) {

  if ($transaction->type == 'Transfer') {
    $to_user = user_load($transaction->reference);
    $rows[] = array(
        date('d/m/Y   g:i:s A', $transaction->created), uc_currency_format/*commerce_currency_format*/($transaction->amount/100/*, commerce_default_currency()*/), $transaction->type, t('Transfer to: ') . $to_user->mail . '<br ><br>' . $transaction->notes
    );
  }
  elseif ($transaction->type == 'Escrow Payment') {
    $to_user = user_load($transaction->reference);
    $rows[] = array(
        date('d/m/Y   g:i:s A', $transaction->created), uc_currency_format/*commerce_currency_format*/($transaction->amount/100/*, commerce_default_currency()*/), $transaction->type, t('Escrow allocated to: ') . $to_user->mail . '<br ><br>' . $transaction->notes
    );
  }
  else {
    $rows[] = array(
        date('d/m/Y   g:i:s A', $transaction->created), uc_currency_format/*commerce_currency_format*/($transaction->amount/100/*, commerce_default_currency()*/), $transaction->type, $transaction->notes
    );
  }
}

print t('<h2>Your Transactions</h2>');

print theme('table', array('header' => $header, 'rows' => $rows));
?>


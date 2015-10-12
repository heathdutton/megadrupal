<?php

function paypal_reports_instant_page() {
  $transactions = db_select('paypal_reports_instant', 'i')
    ->fields('i')
    ->execute();

  $rows = array();
  foreach ($transactions as $transaction) {
    $rows[] = array(
      $transaction->txn_id,
      $transaction->item_name,
      $transaction->payer_email,
      $transaction->mc_gross,
      $transaction->mc_fee,
      $transaction->mc_currency,
      $transaction->residence_country,
      !empty($transaction->payment_date) ? format_date($transaction->payment_date, 'short') : '',
    );
  }

  $header = array(
    t('Id'),
    t('Name'),
    T('Email'),
    t('Gross'),
    t('Fee'),
    t('Currency'),
    t('Country'),
    t('Date'),
  );

  return theme('table', array('header' => $header, 'rows' => $rows));
}

function paypal_reports_subscriptions_page() {
  $transactions = db_select('paypal_reports_subscription', 's')
    ->fields('s')
    ->execute();

  $rows = array();
  foreach ($transactions as $transaction) {
    $rows[] = array(
      $transaction->subscr_id,
      $transaction->item_name,
      $transaction->mc_currency,
      $transaction->mc_amount1,
      $transaction->mc_amount2,
      $transaction->mc_amount3,
      $transaction->period1,
      $transaction->period2,
      $transaction->period3,
      $transaction->payer_email,
      $transaction->residence_country,
      !empty($transaction->subscr_date) ? format_date($transaction->subscr_date, 'short') : '',
      !empty($transaction->cancel_date) ? format_date($transaction->cancel_date, 'short') : '',
    );
  }

  $header = array(
    t('Id'),
    t('Name'),
    t('Currency'),
    t('Amount 1'),
    t('Amount 2'),
    t('Amount 3'),
    t('Period 1'),
    t('Period 2'),
    t('Period 3'),
    t('Email'),
    t('Country'),
    t('Subscription date'),
    t('Cancel date'),
  );

  return theme('table', array('header' => $header, 'rows' => $rows));
}

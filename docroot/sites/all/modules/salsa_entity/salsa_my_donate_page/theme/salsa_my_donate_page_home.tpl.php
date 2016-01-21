<?php

// Campaigns.
if (!empty($my_donate_pages)) {
  foreach ($my_donate_pages as $my_donate_page) {
    echo '<h3>' . $my_donate_page->Title . '</h3>';
    $uri = $my_donate_page->uri();
    echo l(t('Create a new fundraising page'), 'salsa/my_donate_page/' . $my_donate_page->my_donate_page_KEY . '/create') . '<br />';
    echo l(t('View your fundraising pages'), $uri['path']) . '<br />';
  }
}

// Donations.
echo '<h3>' . t('Donation/Purchase History') . '</h3>';
// @todo Object contains several codes, check to which code relates.
$header = array(
  t('Date'),
  t('Code'),
  t('Type'),
  t('Form'),
  t('Info'),
  t('Status'),
  t('Amount'),
  t('Notes'),
);

$rows = array();
foreach ($donations as $donation) {
  $rows[] = array(
    format_date(strtotime($donation->Transaction_Date)),
    $donation->Currency_Code,
    $donation->Transaction_Type,
    $donation->Form_Of_Payment,
    $donation->RESPMSG,
    $donation->Status,
    $donation->amount,
    isset($donation->Note) ? $donation->Note : '',
  );
}

echo theme('table', array('header' => $header, 'rows' => $rows, 'empty' => t('No donations')));

// Reccuring Donations.
echo '<h3>' . t('Recurring Donations') . '</h3>';
// @todo Object contains several codes, check to which code relates.
$header = array(
  t('Date'),
  t('Code'),
  t('Status'),
  t('Amount'),
  t('Start Date'),
  t('Term'),
  t('Pay Period'),
);

$rows = array();
foreach ($recurring_donations as $recurring_donation) {
  $transaction_date = strtotime($recurring_donation->Transaction_Date);
  $start_date = strtotime($recurring_donation->Start_Date);
  $rows[] = array(
    $transaction_date ? format_date($transaction_date) : t('Unknown'),
    $recurring_donation->Tracking_Code,
    $recurring_donation->RESPMSG,
    $recurring_donation->amount,
    $start_date ? format_date($start_date) : t('Unknown'),
    $recurring_donation->TERM,
    $recurring_donation->PAYPERIOD,
  );
}

echo theme('table', array('header' => $header, 'rows' => $rows, 'empty' => t('No recurring donations')));


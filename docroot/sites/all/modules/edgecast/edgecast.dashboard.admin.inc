<?php

/**
 * @file
 * EdgeCast CDN Dashboard
 */
function edgecast_dashboard() {
  $customer = variable_get('edgecast_customer');
  $ec_api_token = variable_get('edgecast_token');
  if (empty($customer) || empty($ec_api_token)) {
    $page_output = t('<strong>EdgeCast Account Dashboard</strong>');
    $page_output .= t('<p>You must set your customer id and your EdgeCast API token before using this module. </p>');
    $page_output .= l('Click Here', 'admin/config/development/edgecast/api') . ' to configure your EdgeCast Credentials';

    return $page_output;
  }
  $dashboard = array();

  //Total Data Transferred
  //Create a begin date as the first of the month
  $begin = new DateTime('now');
  $begin->modify('first day of this month');
  //Create an end date that is today
  $end = new DateTime('now');
  $query_string = 'begindate=' . $begin->format('Y-m-d') . '&enddate=' . $end->format('Y-m-d');
  $url = 'https://api.edgecast.com/v2/reporting/customers/' . $customer . '/bytestransferred?' . $query_string;
  try {
    $ch = curl_init($url);
    curl_setopt($ch, CURLOPT_SSL_VERIFYHOST, false);
    curl_setopt($ch, CURLOPT_SSL_VERIFYPEER, false);
    curl_setopt($ch, CURLOPT_RETURNTRANSFER, true);
    curl_setopt($ch, CURLOPT_HTTPHEADER, array(
      'Authorization: TOK:' . $ec_api_token,
      'Accept: application/json',
      'Content-Type: application/json',
      'Host: api.edgecast.com',
    ));

    $output = json_decode(curl_exec($ch));
    $dashboard[] = array('Total Transfered', number_format($output->Bytes / 1073741824, 2) . ' GB');
    curl_close($ch);
  } catch (Exception $e) {
    watchdog('edgecast', 'Dashboard error: ' . $e);
    drupal_set_message(t('Dashboard Error: ') . $e, 'error');
    curl_close($ch);
    return FALSE;
  }
  //Get Current Storage
  $url = 'https://api.edgecast.com/v2/reporting/customers/' . $customer . '/lateststorageusage';
  try {
    $ch = curl_init($url);
    curl_setopt($ch, CURLOPT_SSL_VERIFYHOST, false);
    curl_setopt($ch, CURLOPT_SSL_VERIFYPEER, false);
    curl_setopt($ch, CURLOPT_RETURNTRANSFER, true);
    curl_setopt($ch, CURLOPT_HTTPHEADER, array(
      'Authorization: TOK:' . $ec_api_token,
      'Accept: application/json',
      'Content-Type: application/json',
      'Host: api.edgecast.com',
    ));

    $output = json_decode(curl_exec($ch));
    $dashboard[] = array('Storage Usage', number_format($output->UsageResult) . ' GB');
    curl_close($ch);
  } catch (Exception $e) {
    watchdog('edgecast', 'Dashboard error: ' . $e);
    drupal_set_message(t('Dashboard Error: ') . $e, 'error');
    curl_close($ch);
    return FALSE;
  }

  $header = array(t('Reporting Type'), t('Values'));

  $page_output = t('<strong>EdgeCast Account Dashboard</strong>');
  $page_output .= t('<p>This is your dashboard into your EdgeCast account. Here we show an overview of your account witout haveing to login to your control panel.</p>');

  $page_output .= theme('table', array(
    'header' => $header,
    'rows' => $dashboard,
    'attributes' => array('width' => 400),
    'empty' => t('We have no results to show')
  ));
  return $page_output;
}
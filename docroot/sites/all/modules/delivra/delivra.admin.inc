<?php

/**
 * @file
 * Delivra settings administration UI.
 */

/**
 * Form definition; admin settings.
 */
function delivra_admin_settings() {
  $form = array();
  $form['delivra_intro'] = array(
    '#markup' => t('Provide your Delivra API access credentials and the list you would like to use.'),
  );
  $form['delivra_username'] = array(
    '#type' => 'textfield',
    '#title' => t('Delivra Username'),
    '#size' => 35,
    '#maxlength' => 255,
    '#default_value' => variable_get('delivra_username'),
    '#description' => t('Delivra username for your account.'),
    '#required' => TRUE,
  );
  $form['delivra_password'] = array(
    '#type' => 'password_confirm',
    '#title' => t('Delivra Password'),
    '#description' => t('Delivra pasword to your account'),
    '#required' => TRUE,
  );
  $form['delivra_listname'] = array(
    '#type' => 'textfield',
    '#title' => t('Delivra Listname'),
    '#description' => t('Delivra List Name from your acocunt.'),
    '#size' => 35,
    '#default_value' => variable_get('delivra_listname'),
    '#required' => TRUE,
  );
  $form['delivra_log'] = array(
    '#type' => 'checkboxes',
    '#title' => t('Log the following messages for debugging'),
    '#options' => array(
      'request' => t('API request messages'),
      'response' => t('API response messages'),
    ),
    '#default_value' => variable_get('delivra_log', array()),
  );
  return system_settings_form($form);
}

/**
 * Helping function to load reports from web service.
 */
function delivra_load_reports() {
  // Delivra API works on EST so all request need to be in EST.
  date_default_timezone_set('EST');
  // Create a 48 hour time span in ISO 8701 Time format.
  $end = date('c');
  $start = date('c', strtotime('-48 hours', strtotime($end)));
  $request = array(
    'StartDate' => $start,
    'EndDate' => $end,
  );
  // Call service to get 48 hour list of unsubscribes.
  $service = 'ReportService';
  $method = 'ListUnsubscribeEvent';
  $response = delivra_soap_client($request, $service, $method);
  $report = delivra_xpath_response($response, $method = 'ListUnsubscribeEvent');
  $count_unsub = count($report);
  // Call service to retrive the user count.
  $response = delivra_soap_client('', $service = 'MemberService', $method = 'ListCountByMemberType');
  $report = delivra_xpath_response($response, $method = 'ListCountByMemberType');
  foreach ($report as $items) {
    switch ($items->MemberType_) {
      case 'expired':
        $type = t('Expired');
        break;

      case 'unsub':
        $type = t('Unsubscribed');
        break;

      case 'normal':
        $type = t('Subscribed');
        break;

      case 'held':
        $type = t('Held');
        break;
    }
    $dashboard[] = array($type, check_plain($items->MemberCount_));
  }
  // This builds a form that has just text for display in the admin overlays.
  $page_markup = '<strong>' . t('Delivra Account General Summary Information') . '</strong>';
  $page_markup .= '<p>' . t('This is your dashboard into your Delivra account. It provides some summary information about your account
    that can be used to judge the general health of your list.') . '</p.';
  $page_markup .= '<p>' . t('Note: Delivras database and all functions works off of Eastern Time Zone.  All data here is reported as such.') . '</p>';
  $page_markup .= '<p><b>' . t('Unsubscriptions for the email list in the last 48 hours: @count', array('@count' => $count_unsub)) . '</b></p>';
  $page = array(
    'intro' => array('#markup' => $page_markup),
    'delivrastatstable' => array(
      '#theme' => 'table',
      '#header' => array(t('Member Type'), t('Values')),
      '#rows' => $dashboard,
      '#attributes' => array('width' => 400),
      '#empty' => t('We have no results to show.'),
    ),
  );

  return $page;
}

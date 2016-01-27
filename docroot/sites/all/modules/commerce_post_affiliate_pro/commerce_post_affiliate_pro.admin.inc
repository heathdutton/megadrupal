<?php
/*
 * @file commerce_post_affiliate_pro.admin.inc
 * Provides admin forms for commerce_post_affiliate_pro
 * 
 */

/**
 * Define the admin settings form
 */
function commerce_post_affiliate_pro_admin_manage() {
  $form['pap_link'] = array(
    '#markup' => t("Your direct link to ") . "<a href=\"" . variable_get('commerce_post_affiliate_pro_merchant_url_prefix', 'http://demo.qualityunit.com/pax4/') . "merchants/\" target=\"_blank\">Post Affiliate Pro server</a>.",
  );

  $form['commerce_post_affiliate_pro_merchant_username'] = array(
    '#type' => 'textfield',
    '#title' => t("Merchant's username"),
    '#description' => t("Enter your username on Post Affiliate Pro server eg. <em>merchant@example.com</em>"),
    '#default_value' => variable_get('commerce_post_affiliate_pro_merchant_username', 'merchant@example.com'),
  );

  $form['commerce_post_affiliate_pro_merchant_id'] = array(
    '#type' => 'textfield',
    '#title' => t("Merchant's ID"),
    '#description' => t("Enter your merchant id for tracking sales. Your PAP network manager would advise of this or you can view it in the section Accounts>Account manager when you are logged in as a network owner"),
    '#default_value' => variable_get('commerce_post_affiliate_pro_merchant_id', NULL),
  );

  $form['commerce_post_affiliate_pro_merchant_password'] = array(
    '#type' => 'password',
    '#title' => t("Merchant's password"),
    '#description' => t("Enter your password on Post Affiliate Pro server eg. <em>demo</em>"),
    '#default_value' => variable_get('commerce_post_affiliate_pro_merchant_password', 'demo'),
  );

  $form['affiliate_user'] = array(
    '#type' => 'fieldset',
    '#title' => t('Affiliate user settings'),
    '#weight' => 5,
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
  );

  $form['affiliate_user']['commerce_post_affiliate_pro_affiliate_activated'] = array(
    '#type' => 'radios',
    '#title' => t('Collect affiliate registration details on user form.'),
    '#description' => t('Should the affiliate registration details be collected on user edit and registration forms.'),
    '#options' => array(
      COMMERCE_POST_AFFILIATE_PRO_MANUAL => t('Enabled - Manual'),
      COMMERCE_POST_AFFILIATE_PRO_AUTOMATIC => t('Enabled - Automatic'),
      COMMERCE_POST_AFFILIATE_PRO_NONE => t('Disabled'),
    ),
    '#default_value' => variable_get('commerce_post_affiliate_pro_affiliate_activated', COMMERCE_POST_AFFILIATE_PRO_NONE),
  );

  $form['affiliate_user']['commerce_post_affiliate_pro_affiliate_auto_login'] = array(
    '#type' => 'checkbox',
    '#title' => t('Automatically login user on Post Affiliate Pro.'),
    '#default_value' => variable_get('commerce_post_affiliate_pro_affiliate_auto_login', FALSE),
  );
  $form['affiliate_user']['commerce_post_affiliate_pro_affiliate_auto_login_roles'] = array(
    '#type' => 'checkboxes',
    '#options' => user_roles(),
    '#title' => t('Enable autologin for following roles'),
    '#states' => array(
      'invisible' => array(
        ':input[name="commerce_post_affiliate_pro_affiliate_auto_login"]' => array('checked' => FALSE),
      ),
    ),
    '#default_value' => variable_get('commerce_post_affiliate_pro_affiliate_auto_login_roles', array()),
  );
  $form['affiliate_user']['commerce_post_affiliate_pro_merchant_url_prefix'] = array(
    '#type' => 'textfield',
    '#title' => t("Merchant's Affiliate website URL link"),
    '#description' => t("Merchant's Affiliate website URL link at Post Affiliate Pro, please replace default value with your own link with slash '/' at the end. eg. <em>Example: http://demo.qualityunit.com/pax4/</em> or <em>http://MY-INSTALATION-OF-PAP.postaffiliatepro.com/</em>"),
    '#default_value' => variable_get('commerce_post_affiliate_pro_merchant_url_prefix', 'http://demo.qualityunit.com/pax4/'),
  );

  return system_settings_form($form);
}

/*
* Provides the reports subpage for the merchant
*/
function commerce_post_affiliate_pro_reports() {
  $pap_session_merchant = commerce_post_affiliate_pro_get_session();
  // order by handling
  if (isset($_REQUEST['order'])) {
    switch ($_REQUEST['order']) {
      case t('Date'):
        $sortBy = "dateinserted";
        break;

      case t('OrderId'):
        $sortBy = "orderid";
        break;

      case t('Name'):
        $sortBy = "lastname";
        break;

      case t('Total'):
        $sortBy = "totalcost";
        break;

      default:
        $sortBy = "dateinserted";
    }
  }
  else {
    $sortBy = "dateinserted";
  }

  $ascending = FALSE;
  if (isset($_REQUEST['sort'])) {
    if ($_REQUEST['sort'] == 'desc') {
      $ascending = FALSE;
    }
    else {
      $ascending = TRUE;
    }
  }
  // get recordset with list of affiliates
  $request = new Pap_Api_TransactionsGrid($pap_session_merchant);

  // sets limit to 30 rows, offset to 0 (first row starts)
  $request->setLimit(0, 50);
  $request->setSorting($sortBy, $ascending);

  // send request
  try {
    $request->sendNow();
  }
  catch(Exception$e) {
    drupal_set_message(t("API call error: %error", array('%error' => $e->getMessage())), 'error');
    watchdog('commerce_post_affiliate_pro', "API call error: @error", array('@error' => $e->getMessage()), WATCHDOG_ERROR);
    return t('An error occurred during processing');
  }

  // request was successful, get the grid result
  $grid = $request->getGrid();

  // table form

  $header = array(
    array('data' => t('OrderId'), 'field' => 'orderid', 'sort' => 'desc'),
    array('data' => t('Date'), 'field' => 'dateinserted', 'sort' => 'desc'),
    array('data' => t('Name'), 'field' => 'username'),
    array('data' => t('Product')),
    array('data' => t('Total'), 'field' => 'totalcost', 'sort' => 'desc'),
  );

  $rows = array();

  // get recordset from the grid
  $recordset = $grid->getRecordset();

  // iterate through the records
  foreach ($recordset as $rec) {
    $rows[] = array(
      array('data' => $rec->get('orderid')),
      array('data' => $rec->get('dateinserted')),
      array('data' => check_plain($rec->get('firstname') . ' ' . $rec->get('lastname') . ' (' . $rec->get('username') . ')')),
      array('data' => $rec->get('data1')),
      array('data' => $rec->get('totalcost')),
    );
  }

  if (empty($rows)) {
    $rows[] = array(array('data' => t('No report available.'), 'colspan' => 5));
  }

  return theme('table', array('header' => $header, 'rows' => $rows, 'attributes' => array('class' => array('commerce-post-affiliate-pro-data'))));
}

/*
* Provides the payouts subpage for the merchant
*/
function commerce_post_affiliate_pro_report_payouts() {

  $pap_session_merchant = commerce_post_affiliate_pro_get_session();

  if (isset($_REQUEST['order'])) {
    // order by handling
    switch ($_REQUEST['order']) {
      case t('UserId'):
        $sortBy = "accountuserid";
        break;

      case t('Date'):
        $sortBy = "dateinserted";
        break;

      case t('Name'):
        $sortBy = "username";
        break;

      case t('Amount'):
        $sortBy = "amounttopay";
        break;

      default:
        $sortBy = "username";
    }
  }
  else {
    $sortBy = "username";
  }

  $ascending = FALSE;
  if (isset($_REQUEST['sort'])) {
    if ($_REQUEST['sort'] == 'desc') {
      $ascending = FALSE;
    }
    else {
      $ascending = TRUE;
    }
  }

  // get recordset with list of payouts

  $payoutRequest = new Pap_Api_PayoutsGrid($pap_session_merchant);
  $payoutRequest->setSorting($sortBy, $ascending);
  // send request
  try {
    $payoutRequest->sendNow();
  }
  catch(Exception$e) {
    drupal_set_message(t("API call error: %error", array('%error' => $e->getMessage())), 'error');
    watchdog('commerce_post_affiliate_pro', "API call error: @error", array('@error' => $e->getMessage()), WATCHDOG_ERROR);
    return t('An error occurred during processing');
  }
  $payoutGrid = $payoutRequest->getGrid();

  // Table form

  $header = array(
    array('data' => t('UserId')),
    array('data' => t('Name'), 'field' => 'username'),
    array('data' => t('Commission')),
    array('data' => t('Amount')),
  );

  $rows = array();

  // get recordset from the grid
  $recordset = $payoutGrid->getRecordset();

  // iterate through the records
  foreach ($recordset as $rec) {

    $rows[] = array(
      array('data' => $rec->get('id')),
      array('data' => check_plain($rec->get('firstname') . ' ' . $rec->get('lastname') . ' (' . $rec->get('username') . ')')),
      array('data' => $rec->get('commission')),
      array('data' => $rec->get('amounttopay')),
    );
  }

  if (empty($rows)) {
    $rows[] = array(array('data' => t('No report available.'), 'colspan' => 4));
  }

  return theme('table', array('header' => $header, 'rows' => $rows, 'attributes' => array('class' => array('commerce-post-affiliate-pro-data'))));
}

/*
* Provides the affiliates subpage for the merchant
*/
function commerce_post_affiliate_pro_affiliates() {
  $pap_session_merchant = commerce_post_affiliate_pro_get_session();

  if (isset($_REQUEST['order'])) {
    // order by handling
    switch ($_REQUEST['order']) {
      case t('UserId'):
        $sortBy = "userid";
        break;

      case t('Name'):
        $sortBy = "username";
        break;

      case t('Parent'):
        $sortBy = "originalparentuserid";
        break;

      case t('Status'):
        $sortBy = "status";
        break;

      case t('Minimum payout'):
        $sortBy = "minimumpayout";
        break;

      case t('Payout method'):
        $sortBy = "payoutmethod";
        break;

      default:
        $sortBy = "userid";
    }
  }
  else {
    $sortBy = "userid";
  }

  $ascending = FALSE;
  if (isset($_REQUEST['sort'])) {
    if ($_REQUEST['sort'] == 'desc') {
      $ascending = FALSE;
    }
    else {
      $ascending = TRUE;
    }
  }

  // get recordset with list of affiliates

  $request = new Pap_Api_AffiliatesGrid($pap_session_merchant);
  // sets limit to 20 rows, offset to 0 (first row starts)
  $request->setLimit(0, 20);

  // send request
  try {
    $request->sendNow();
  }
  catch(Exception$e) {
    drupal_set_message(t("API call error: %error", array('%error' => $e->getMessage())), 'error');
    watchdog('commerce_post_affiliate_pro', "API call error: @error", array('@error' => $e->getMessage()), WATCHDOG_ERROR);
    return t('An error occurred during processing');
  }

  // request was successful, get the grid result
  $grid = $request->getGrid();

  // get recordset from the grid
  $recordset = $grid->getRecordset();

  // table form

  $header = array(
    array('data' => t('UserId'), 'field' => 'userid'),
    array('data' => t('Name'), 'field' => 'username'),
    array('data' => t('Status'), 'field' => 'status'),
  );

  $rows = array();

  $states = commerce_post_affiliate_pro_affiliate_statuses();

  // iterate through the records
  foreach ($recordset as $rec) {
    $rows[] = array(
      array('data' => $rec->get('userid')),
      array('data' => check_plain($rec->get('firstname') . ' ' . $rec->get('lastname') . ' (' . $rec->get('username') . ')')),
      array('data' => $states[$rec->get('rstatus')]),
    );
  }

  if (empty($rows)) {
    $rows[] = array(array('data' => t('No report available.'), 'colspan' => 3));
  }

  return theme('table', array('header' => $header, 'rows' => $rows, 'attributes' => array('class' => array('commerce-post-affiliate-pro-data'))));
}

/*
* Implementation of Report for Affiliate
* @param $account object user account
*/
function commerce_post_affiliate_pro_affiliate_report($account) {
  $pap_session_merchant = commerce_post_affiliate_pro_get_session();
  if (isset($_REQUEST['order'])) {
    // order by handling
    switch ($_REQUEST['order']) {
      case t('Date'):
        $sortBy = "dateinserted";
        break;

      case t('OrderId'):
        $sortBy = "orderid";
        break;

      case t('Name'):
        $sortBy = "lastname";
        break;

      case t('Total'):
        $sortBy = "totalcost";
        break;

      default:
        $sortBy = "dateinserted";
    }
  }
  else {
    $sortBy = "dateinserted";
  }

  $ascending = FALSE;
  if (isset($_REQUEST['sort'])) {
    if ($_REQUEST['sort'] == 'asc') {
      $ascending = TRUE;
    }
    else {
      $ascending = FALSE;
    }
  }

  // load affiliate data
  $affiliate = commerce_post_affiliate_pro_user_load_affiliate($account);

  // get recordset with list of affiliates

  $request = new Pap_Api_TransactionsGrid($pap_session_merchant);

  // sets limit to 30 rows, offset to 0 (first row starts)
  $request->setLimit(0, 30);
  $request->addFilter("username", "=", $affiliate->affiliate_username);
  $request->setSorting($sortBy, $ascending);

  // send request
  try {
    $request->sendNow();
  }
  catch(Exception$e) {
    drupal_set_message(t("API call error: %error", array('%error' => $e->getMessage())), 'error');
    watchdog('commerce_post_affiliate_pro', "API call error: @error", array('@error' => $e->getMessage()), WATCHDOG_ERROR);
    return t('An error occurred during processing');
  }

  // request was successful, get the grid result
  $grid = $request->getGrid();

  // table form

  $header = array(
    array('data' => t('OrderId'), 'field' => 'orderid', 'sort' => 'desc'),
    array('data' => t('Date'), 'field' => 'dateinserted', 'sort' => 'desc'),
    array('data' => t('Name'), 'field' => 'username'),
    array('data' => t('Product')),
    array('data' => t('Total'), 'field' => 'totalcost', 'sort' => 'desc'),
  );

  $rows = array();

  // get recordset from the grid
  $recordset = $grid->getRecordset();

  // iter ate through the records
  foreach ($recordset as $rec) {
    $rows[] = array(
      array('data' => $rec->get('orderid')),
      array('data' => $rec->get('dateinserted')),
      array('data' => check_plain($rec->get('firstname') . ' ' . $rec->get('lastname') . ' (' . $rec->get('username') . ')')),
      array('data' => $rec->get('data1')),
      array('data' => $rec->get('totalcost')),
    );
  }

  if (empty($rows)) {
    $rows[] = array(array('data' => t('No report available.'), 'colspan' => 5));
  }

  return theme('table', array('header' => $header, 'rows' => $rows, 'attributes' => array('class' => array('commerce-post-affiliate-pro-data'))));
}


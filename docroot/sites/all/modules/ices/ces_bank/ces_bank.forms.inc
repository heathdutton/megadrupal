<?php
/**
 * @file
 * Forms definition, submit and validation functions for ces_bank module.
 */

/**
 * @defgroup ces_bank_forms Forms from Ces Bank
 * @ingroup ces_bank
 * @{
 * Forms definition.
 * @ref modulo_ces_bank
 */

/**
 * CesBankExchange form. Create, view and edit forms.
 *
 * @param array  $form
 *   Form.
 * @param array  $form_state
 *   State from.
 * @param string $op
 *   Options.
 * @param int    $exchange_id
 *   Id of exchange.
 */
function ces_bank_exchange_form($form, &$form_state, $op, $exchange_id = NULL) {
  $bank = new CesBank();
  if (isset($form_state['values'])) {
    // Form rebuild.
    $exchange = $form_state['values'];
  }
  else {
    if ($op == 'new') {
      $exchange = $bank->getDefaultExchange();
    }
    elseif ($exchange_id) {
      $exchange = $bank->getExchange($exchange_id);
    }
    else {
      return array(
        'message' => array(
          '#markup' => '<p>' . t('Invalid exchange id.') . '</p>',
        ),
      );
    }
    // We change the exchange field name from "name" to "fullname" so it doesn't
    // conflict with user's form "name" field.
    $exchange['fullname'] = $exchange['name'];
    unset($exchange['name']);
    $form_state['values'] = $exchange;
  }

  $form['exchange'] = array(
    '#type' => 'vertical_tabs',
    '#description' => t('Settings for an individual exchange in Community Exchange System.'),
  );

  $form['exchange']['operation'] = array(
    '#type' => 'value',
    '#value' => $op,
  );

  $form['exchange']['state'] = array(
    '#type' => 'value',
    '#value' => isset($exchange['state']) ? $exchange['state'] : 0,
  );

  if ($op != 'new') {
    $form['exchange']['id'] = array(
      '#type' => 'value',
      '#value' => $exchange['id'],
    );
  }

  $form['exchange']['general'] = array(
    '#type' => 'fieldset',
    '#title' => t('General'),
    '#description' => t('General settings for this exchange.'),
  );

  $form['exchange']['general']['code'] = array(
    '#type' => 'textfield',
    '#description' => t('Four numbers or uppercase letters identifying this exchange.'),
    '#title' => t('Code'),
    '#default_value' => $exchange['code'],
    '#size' => 4,
    '#maxlength' => 4,
    '#required' => TRUE,
  );

  $form['exchange']['general']['shortname'] = array(
    '#type' => 'textfield',
    '#description' => t('Acronym or short title. To be used in small-sized places like menus.'),
    '#title' => t('Short name'),
    '#default_value' => $exchange['shortname'],
    '#size' => 16,
    '#maxlength' => 32,
    '#required' => TRUE,
  );

  $form['exchange']['general']['fullname'] = array(
    '#type' => 'textfield',
    '#description' => t('It can be the same as the short name. To be used as title.'),
    '#title' => t('Full name'),
    '#default_value' => $exchange['fullname'],
    '#required' => TRUE,
  );

  $form['exchange']['general']['website'] = array(
    '#type' => 'textfield',
    '#description' => t('Website or blog of this exchange. Include http:// or https://'),
    '#title' => t('Website'),
    '#default_value' => $exchange['website'],
    '#required' => FALSE,
  );

  // Create new exclusive user for administer this exchange. Later it will be
  // editable from the standard user form.
  if ($op == 'new') {
    $form['exchange']['admin'] = array(
      '#type' => 'fieldset',
      '#title' => t('Administrator'),
      '#description' => t('Administrator settings for this exchange. One Drupal user is the administrator. It is recommended not to use a real person name here.'),
    );
    $form['exchange']['admin'] = user_register_form($form['exchange']['admin'], $form_state);
    unset($form['exchange']['admin']['actions']);
  }
  else {
    $form['exchange']['admin'] = array(
      '#type' => 'value',
      '#value' => $exchange['admin'],
      '#description' => t('Administrator settings for this exchange. One Drupal user is the administrator.'),
    );
  }

  $form['exchange']['location'] = array(
    '#type' => 'fieldset',
    '#title' => t('Location'),
    '#description' => t('Location settings for this exchange.'),
  );

  include_once DRUPAL_ROOT . '/includes/locale.inc';
  $countries = country_get_list();

  $form['exchange']['location']['country'] = array(
    '#type' => 'select',
    '#description' => t('The legal country of this exchange.'),
    '#title' => t('Country'),
    '#default_value' => $exchange['country'],
    '#options' => $countries,
    '#required' => TRUE,
  );

  $form['exchange']['location']['region'] = array(
    '#type' => 'textfield',
    '#description' => t('Region within the country.'),
    '#title' => t('Region'),
    '#default_value' => $exchange['region'],
    '#required' => TRUE,
  );

  $form['exchange']['location']['town'] = array(
    '#type' => 'textfield',
    '#description' => t('Main city or town.'),
    '#title' => t('Town'),
    '#default_value' => $exchange['town'],
    '#required' => TRUE,
  );

  $form['exchange']['location']['map'] = array(
    '#type' => 'textfield',
    '#description' => t('Paste here a Google Map URL pointing to your location. Something like http://maps.google.com/maps?...'),
    '#title' => t('Map'),
    '#default_value' => $exchange['map'],
    '#required' => FALSE,
    '#maxlength' => 200,
  );
  $languages = language_list('enabled');
  $lang_options = array();
  foreach ($languages[1] as $lang) {
    $lang_options[$lang->language] = $lang->native;
  }
  asort($lang_options);
  if (!isset($exchange['data']['default_lang'])) {
    global $language;
    $exchange['data']['default_lang'] = $language->language;
  }
  $form['exchange']['location']['default_lang'] = array(
    '#type' => 'select',
    '#description' => t('Choose the language that will have the new users registered to this exchange. They will be able to change their preferred language once logged in.'),
    '#title' => t('Default language'),
    '#options' => $lang_options,
    '#default_value' => $exchange['data']['default_lang'],
  );
  $form['exchange']['currency'] = array(
    '#type' => 'fieldset',
    '#title' => t('Currency'),
    '#description' => t('Currency settings for this exchange.'),
  );
  $form['exchange']['currency']['currencysymbol'] = array(
    '#type' => 'textfield',
    '#description' => t('Symbol for the currency of this exchange. It can be any 3 unicode characters.'),
    '#title' => t('Symbol'),
    '#default_value' => $exchange['currencysymbol'],
    '#required' => TRUE,
    '#size' => 4,
    '#maxlength' => 3,
  );

  $form['exchange']['currency']['currencyname'] = array(
    '#type' => 'textfield',
    '#description' => t('Name for a single unit of this currency.'),
    '#title' => t('Name'),
    '#default_value' => $exchange['currencyname'],
    '#required' => TRUE,
    '#size' => 32,
    '#maxlength' => 60,
  );

  $form['exchange']['currency']['currenciesname'] = array(
    '#type' => 'textfield',
    '#description' => t('Plural currency name.'),
    '#title' => t('Plural name'),
    '#default_value' => $exchange['currenciesname'],
    '#required' => TRUE,
    '#size' => 32,
    '#maxlength' => 60,
  );

  $form['exchange']['currency']['currencyvalue'] = array(
    '#type' => 'textfield',
    '#description' => t('Value of this currency against the value of an average hour of labor. This is used to convert amounts between two currencies. If your currency is worth 10u/hour then put "0.1". If you want to use your national currency as standard of value, look for the average cost of an hour of labor or choose an approximation based on your community reality.'),
    '#title' => t('Value'),
    '#default_value' => $exchange['currencyvalue'],
    '#required' => TRUE,
    '#size' => 16,
    '#maxlength' => 60,
  );

  $form['exchange']['currency']['currencyscale'] = array(
    '#type' => 'select',
    '#description' => t('Number of decimal digits to be shown.'),
    '#title' => t('Scale'),
    '#default_value' => $exchange['currencyscale'],
    '#options' => array(
      '0' => t('0'),
      '1' => t('1'),
      '2' => t('2'),
      '3' => t('3'),
      '4' => t('4')),
    '#required' => TRUE,
  );
  $form['exchange']['users'] = array(
    '#type' => 'fieldset',
    '#title' => t('Members'),
    '#description' => t('Settings for users registering with exchange. Sub-modules add settings here.'),
  );
  if (!isset($exchange['data']['hide_balance'])) {
    $exchange['data']['hide_balance'] = FALSE;
  }
  $form['exchange']['users']['hide_balance'] = array(
    '#type' => 'checkbox',
    '#title' => t('Hide balance'),
    '#description' => t('If checked the balance is shown only to account owners. Otherwise it is public to any exchange user.'),
    '#default_value' => $exchange['data']['hide_balance'],
  );
  if (!isset($exchange['data']['hide_sales'])) {
    $exchange['data']['hide_sales'] = FALSE;
  }
  $form['exchange']['users']['hide_sales'] = array(
    '#type' => 'checkbox',
    '#title' => t('Hide sales and purchases'),
    '#description' => t('If checked, the total amount purchased and sold is shown only to to account owners. Otherwise it is public to any exchange user.'),
    '#default_value' => $exchange['data']['hide_sales'],
  );
  if (!empty($exchange['state'])) {
    // Exchange has been activated.
    $limitchains = $bank->getAllLimitChains($exchange['id']);
    $options = array();
    foreach ($limitchains as $id => $limit) {
      $options[$id] = $limit['name'];
    }
    $form['exchange']['users']['limitchain'] = array(
      '#type' => 'select',
      '#title' => t('Default limit chain'),
      '#description' => t('The limit chain for new accounts.'),
      '#options' => $options,
      '#default_value' => $exchange['limitchain'],
    );
  }
  switch ($op) {
    case 'activate':
      $value = t('Activate exchange');
      break;

    case 'new':
      $value = t('Create exchange');
      break;

    default:
      $value = t('Update exchange');
  }
  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => $value,
  );

  return $form;
}
/**
 * Validates bank exchange form for new or existing exchanges.
 * @see ces_bank_exchange_form_submit()
 */
function ces_bank_exchange_form_validate($form, &$form_state) {
  // Code.
  $code = $form_state['values']['code'];
  if (drupal_strlen($code) != 4) {
    form_set_error('code', t('Code %code is invalid. Code must have exactly 4 letters.', array('%code' => $code)));
  }
  for ($i = 0; $i < drupal_strlen($code); $i++) {
    $ord = ord(drupal_substr($code, $i, 1));
    if (($ord < ord('A') || $ord > ord('Z')) && ($ord < ord('0') || $ord > ord('9'))) {
      form_set_error('code', t('Code %code is invalid. Code characters must be uppercase ASCII letters and numbers. No accents, no spaces, no special characters.', array('%code' => $code)));
    }
  }
  // Website.
  $website = $form_state['values']['website'];
  if ($website != '' && !valid_url($website, TRUE)) {
    form_set_error('website', t('The url %url is invalid. Please enter a fully qualified URL like http://www.example.org/something or leave the field blank.', array('%url' => $website)));
  }
  // Map.
  $map = $form_state['values']['map'];
  if (!valid_url($map, TRUE)) {
    form_set_error('website', t('The map url %url is invalid. Please enter a fully qualified URL like http://maps.google.com/....', array('%url' => $map)));
  }
  // Currency value.
  $currencyvalue = $form_state['values']['currencyvalue'];
  if (!is_numeric($currencyvalue)) {
    form_set_error('currencyvalue', t('Currency value %value is invalid. Enter a numeric value like 0.1, 0.0666666667 or 1', array('%value' => $currencyvalue)));
  }
  if ($form_state['values']['operation'] == 'new') {
    user_register_validate($form['exchange']['admin'], $form_state);
    user_account_form_validate($form['exchange']['admin'], $form_state);
  }
}
/**
 * Processes bank exchange form for new or existing exchanges.
 */
function ces_bank_exchange_form_submit($form, &$form_state) {
  $bank = new CesBank();
  $op = $form_state['values']['operation'];
  try {
    if ($op == 'new') {
      // Create admin user.
      user_register_submit($form['exchange']['admin'], $form_state);
      // Set the new user as the exchange administrator.
      $form_state['values']['admin'] = $form_state['values']['uid'];
    }
    $form_state['values']['name'] = $form_state['values']['fullname'];
    if (!isset($form_state['values']['data'])) {
      $form_state['values']['data'] = array();
    }
    $form_state['values']['data']['default_lang'] = $form_state['values']['default_lang'];
    $form_state['values']['data']['hide_balance'] = $form_state['values']['hide_balance'];
    $form_state['values']['data']['hide_sales'] = $form_state['values']['hide_sales'];
    if ($op == 'new') {
      $bank->createExchange($form_state['values']);
      drupal_set_message(t('Exchange %code successfully created. You must wait for the system administrator to set up the exchange before it is operative. You will receive an e-mail notification within the next few days.', array('%code' => $form_state['values']['code'])));
    }
    elseif ($op == 'edit') {
      $bank->updateExchange($form_state['values']);
      drupal_set_message(t('Exchange %code successfully updated.', array('%code' => $form_state['values']['code'])));
    }
    elseif ($op == 'activate') {
      $bank->activateExchange($form_state['values']);
      // Activate admin Drupal user.
      $admin = user_load($form_state['values']['admin']);
      $admin->status = 1;
      user_save($admin);
      // Clear cache so register block includes the new exchange.
      cache_clear_all('ces_bank_all_exchanges', 'cache');
      cache_clear_all('ces_bank_all_exchanges_' . $form_state['values']['country'], 'cache');
      drupal_set_message(t('Exchange %code successfully activated. There has been sent an email to the new exchange administrator.', array('%code' => $form_state['values']['code'])));
    }
  }
  catch (Exception $e) {
    drupal_set_message(t('An error occurred while saving the exchange record. Details: %msg', array('msg' => $e->getMessage())), 'error');
  }
}
/**
 * Form for deleting exchange.
 *
 * - Inter-exchange accounts (kind=5) cannot be deleted.
 * - Users with another account in other exchange or with
 *   inter-exchange accounts cannot be deleted.
 *
 * If any of these conditions are true, the exchange cannot be deleted.
 * The admin must prepare the exchange to delete it. A little more secure?.
 */
function ces_bank_exchange_delete_form($form, &$form_state, $exchange_id = NULL) {
  if (!empty($form_state['values']) && !empty($form_state['values']['page'])
      && $form_state['values']['page'] == 'confirm') {
    return ces_bank_exchange_delete_confirm_form($form, $form_state);
  }
  if ($exchange_id == NULL || !is_numeric($exchange_id)) {
    return array(
      'message' => array(
        '#markup' => '<p>' . t('Invalid exchange id.') . '</p>',
      ),
    );
  }
  else {
    $bank = new CesBank();
    $exchange = $bank->getExchange($exchange_id);
    if ($exchange === FALSE) {
      return array(
        'message' => array(
          '#markup' => '<p>' . t('Invalid exchange id.') . '</p>',
        ),
      );
    }
    $form['data-exchange'] = array(
      '#type' => 'item',
      '#markup' => '<p>' . t('Exchange') . ': <b>' . $exchange['code'] . ' - ' . $exchange['name'] . '</b></p><br />',
    );
    $accountscheck = array();
    $queryaccounts = db_query("SELECT id FROM {ces_account} WHERE exchange = :exchange",
      array(
        ':exchange' => $exchange['id'],
      ));
    foreach ($queryaccounts as $acc) {
      $accountscheck[] = $acc->id;
    }
    $check = $bank->deleteAccountsCheck($accountscheck, $exchange['id']);
    $actions = array(
      'exchange' => array(
        'id' => $exchange['id'],
        'code' => $exchange['code'],
        'name' => $exchange['name'],
      ),
      'accounts' => array(
        'delete' => array(
          'num' => 0,
          'aids' => array(),
        ),
        'nothing' => array(
          'num' => 0,
          'aids' => array(),
        ),
      ),
      'users' => array(
        'delete' => array(
          'num' => 0,
          'uids' => array(),
        ),
        'nothing' => array(
          'num' => 0,
          'uids' => array(),
        ),
      ),
      'transactions' => array(
        'totaltrans' => 0,
        'tids' => array(),
      ),
    );
    // Accounts information.
    $numacc = 0;
    $numusers = 0;
    foreach ($check['accounts'] as $acc) {
      // All 'acount_action' must be 'delete' or 'nothing',
      // since 'close' doesn't have sense here.
      if ($acc['account_action'] == 'close') {
        $acc['account_action'] = 'delete';
      }
      $actions['accounts'][$acc['account_action']]['num']++;
      $actions['accounts'][$acc['account_action']]['aids'][] = array(
        'aid' => $acc['aid'],
        'name' => $acc['name'],
      );
      $numacc++;
    }
    $textoutput2 = '';
    // List users with inter-exchange accounts or
    // with accounts that belong to another exchange.
    foreach ($check['users'] as $u => $act) {
      $actions['users'][$act['user_action']]['num']++;
      $actions['users'][$act['user_action']]['uids'][] = $u;
      if (!empty($act['accounts'])) {
        $accsout = '';
        foreach ($act['accounts'] as $acc) {
          $accsout .= l($acc['name'], 'ces/admin/account/' . $acc['aid'] . '/view') . ' ';
        }
        if (strlen($accsout) > 0) {
          $userloaded = user_load($u);
          $username = ces_bank_get_full_username($userloaded);
          $username .= '(' . $userloaded->name . ')';
          $textoutput2 .= '<b>' . l($username, 'user/' . $u) . '</b>: ' . $accsout . '<br />';
        }
      }
      $numusers++;
    }
    // Accounts information.
    $textoutput = t('Total number of accounts:') . ' <b>' . $numacc . '</b><br />' .
      t('Accounts to be deleted:') . ' <b>' . $actions['accounts']['delete']['num'] . '</b><br />';
    if ($actions['accounts']['nothing']['num'] > 0) {
      $textoutput .= t('These accounts') . ' (<b>' . $actions['accounts']['nothing']['num'] . '</b>) ' .
      t('depend on other exchange/s in the system, so they CANNOT be deleted:<br /><i>');
      foreach ($actions['accounts']['nothing']['aids'] as $aid) {
        $textoutput .= l($aid['name'], 'ces/admin/account/' . $aid['aid'] . '/view') . ' ';
      }
    }
    else {
      $textoutput .= t('No accounts depend on other exchanges in the system, so all of them can be deleted.');
    }
    $form['accounts_fieldset'] = array(
      '#type' => 'fieldset',
      '#title' => t('Accounts'),
    );
    $form['accounts_fieldset']['data-accounts'] = array(
      '#type' => 'item',
      '#markup' => $textoutput,
    );
    // Users information.
    $textoutput = t('Total number of users:') . ' <b>' . $numusers . '</b><br />' .
      t('Users to be deleted:') . ' <b>' . $actions['users']['delete']['num'] . '</b><br />';
    if ($actions['users']['nothing']['num'] > 0) {
      $textoutput .= t('These users') . ' (<b>' . $actions['users']['nothing']['num'] . '</b>) ' .
      t('have accounts that depen on other exchanges in the system, so they CANNOT be deleted:') .
      ' <br /><i>' . $textoutput2;
    }
    else {
      $textoutput .= t('All users have accounts that belongs only to this exchange, so they can be deleted.');
    }
    $form['users_fieldset'] = array(
      '#type' => 'fieldset',
      '#title' => t('Users'),
    );
    $form['users_fieldset']['data-users-1'] = array(
      '#type' => 'item',
      '#markup' => $textoutput,
    );
    // Transactions information.
    $totaltrans = $check['transactions']['totaltrans'];
    $actions['transactions']['totaltrans'] = $totaltrans;
    $actions['transactions']['tids'] = $check['transactions']['tids'];
    $textoutput = t('Number of transactions:') . ' <b>' . $totaltrans . '</b><br />';
    $form['trans_fieldset'] = array(
      '#type' => 'fieldset',
      '#title' => t('Transactions'),
    );
    $form['trans_fieldset']['data-users-1'] = array(
      '#type' => 'item',
      '#markup' => $textoutput,
    );
    // To submit or not to submit, that's the question.
    if ($actions['accounts']['nothing']['num'] != 0 || $actions['users']['nothing']['num'] != 0) {
      $form['no-possible-erase'] = array(
        '#type' => 'item',
        '#prefix' => '<br />',
        '#markup' => '<h3>' . t('Sorry, it is not possible to delete the exchange.') . '</h3>',
      );
    }
    else {
      $form['submit'] = array(
        '#type' => 'submit',
        '#prefix' => '<br />',
        '#value' => t('Proceed to delete the exchange'),
      );
    }
    $form_state['check_exchange_data'] = $actions;
    return $form;
  }
}
/**
 * Validate form for deleting exchange.
 */
function ces_bank_exchange_delete_form_submit($form, &$form_state) {
  $form_state['values']['page'] = 'confirm';
  $form_state['rebuild'] = TRUE;
}
/**
 * Confirm form for deleting exchange.
 */
function ces_bank_exchange_delete_confirm_form($form, &$form_state) {
  $textoutput = t('Delete accounts:') . ' <b>' .
    $form_state['check_exchange_data']['accounts']['delete']['num'] . '</b><br />';
  $textoutput .= t('Delete users:') . ' <b>' .
    $form_state['check_exchange_data']['users']['delete']['num'] . '</b><br />';
  $textoutput .= t('Delete transactions:') . ' <b>' .
    $form_state['check_exchange_data']['transactions']['totaltrans'] . '</b><br />';

  $form['confirm_fieldset'] = array(
    '#type' => 'fieldset',
    '#title' => t('Confirm actions for exchange') .
    ' <b>' . $form_state['check_exchange_data']['exchange']['name'] . '</b> (' .
    $form_state['check_exchange_data']['exchange']['code'] . ')',
  );
  $form['confirm_fieldset']['data'] = array(
    '#type' => 'item',
    '#markup' => $textoutput,
  );
  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Confirm'),
    '#prefix' => '<h3>' . t('Do you really want to delete the exchange?') . '</h3>' .
    '<h4>' . t('This action cannot be undone.') . '</h4>',
    '#submit' => array('ces_bank_exchange_delete_confirm_form_submit'),
  );
  $form['cancel'] = array(
    '#markup' => l(t('Cancel'), 'ces/admin/ces'),
  );
  return $form;
}
/**
 * Submit confirm form for deleting exchange.
 */
function ces_bank_exchange_delete_confirm_form_submit($form, &$form_state) {
  $info = $form_state['check_exchange_data'];
  $bank = new CesBank();
  // Delete transactions.
  if (!empty($info['transactions']['tids'])) {
    $transactions = db_delete('ces_transaction')
      ->condition('id', $info['transactions']['tids'])
      ->execute();
    drupal_set_message(t('%trans transactions have been deleted.',
        array('%trans' => $transactions)), 'status');
  }
  // Delete accounts.
  foreach ($info['accounts']['delete']['aids'] as $acc) {
    try {
      $action = $bank->deleteAccount($acc['aid']);
      if ($action) {
        drupal_set_message(t('The account %account has been deleted.',
          array('%account' => $acc['name'])), 'status');
      }
    }
    catch (Exception $e) {
      drupal_set_message(t('An error occurred while deleting the account %account. Details: %msg',
        array('%account' => $acc['name'], '%msg' => $e->getMessage())), 'error');
    }
  }
  // Delete users and content.
  $users_cancel = array();
  foreach ($info['users']['delete']['uids'] as $usr) {
    if ($usr != 1) {
      $users_cancel[] = array(
        'uid' => $usr,
        'action' => 'delete',
      );
    }
  }
  if (!empty($users_cancel)) {
    $bank->deleteUsers($users_cancel);
  }
  // Delete exchange.
  try {
    $action = $bank->deleteExchange($info['exchange']['id']);
    drupal_set_message(t('The exchange %exchg has been deleted.',
      array('%exchg' => $info['exchange']['code'] . ' ' . $info['exchange']['name'])), 'status');
  }
  catch (Exception $e) {
    drupal_set_message(t('An error occurred while deleting the exchange %exchg. Details: %msg',
      array('%exchg' => $info['code'], '%msg' => $e->getMessage())), 'error');
  }
  $form_state['redirect'] = 'ces/admin/ces';
}
/**
 * @defgroup forms_limitchain LimitChain forms
 * @ingroup ces_bank_forms
 *
 * @{
 */
/**
 * Form for controlling limit chains.
 */
function ces_bank_limitchain_form($form, &$form_state, $limitchain = NULL) {
  $bank = new CesBank();
  if (!empty($form_state['values'])) {
    // Form rebuild. Take values from last user input.
    $limitchain = &$form_state['values'];
    $op = $form_state['values']['operation'];
  }
  else {
    // First build. Take values from logic layer.
    if ($limitchain == NULL) {
      $exchange = ces_bank_get_current_exchange();
      if ($exchange === FALSE) {
        return array(
          'message' => array(
            '#markup' => '<p>' . t('Invalid current exchange') . '</p>',
          ),
        );
      }
      $limitchain = $bank->getDefaultLimitChain($exchange['id']);
      $limitchain['name'] = '';
      $op = 'new';
    }
    else {
      $op = 'edit';
    }
  }

  $form = array(
    '#type' => 'fieldset',
    '#title' => t('Limit chain'),
    '#tree' => TRUE,
  );

  // Save values: operation, exchange and id.
  $form['operation'] = array(
    '#type' => 'value',
    '#value' => $op,
  );

  $form['exchange'] = array(
    '#type' => 'value',
    '#value' => $limitchain['exchange'],
  );

  if ($op != 'new') {
    $form['id'] = array(
      '#type' => 'value',
      '#value' => $limitchain['id'],
    );
  }

  $form['name'] = array(
    '#type' => 'textfield',
    '#description' => t('Name for this account limit'),
    '#title' => t('name'),
    '#default_value' => $limitchain['name'],
    '#required' => TRUE,
  );
  // Limits list.
  $form['limits'] = array(
    '#type' => 'fieldset',
    '#title' => t('Account limits'),
    '#description' => t('List of account limits currently belonging to this chain.'),
  );

  if (isset($limitchain['limits'])) {
    foreach ($limitchain['limits'] as $key => $value) {
      $form['limits'][$key] = _ces_bank_limitchain_limit_fieldset(array(), $limitchain['limits'][$key]);
    }
  }
  // Add new limit to the chain.
  $form['newlimit'] = array(
    '#type' => 'fieldset',
    '#title' => t('Add account limit'),
    '#description' => t('Add a new account limit to this limit chain.'),
  );
  $form['newlimit']['classname'] = array(
    '#type' => 'select',
    '#title' => t('Limit class'),
    '#options' => module_invoke_all('account_limit_classes'),
    '#description' => t('The type of limit to add. Each class limits a different aspect of the account balance or in a different way. Once added, you will be able to configure its parameters.'),
  );
  $form['newlimit']['value'] = array(
    '#type' => 'value',
    '#value' => '0',
  );
  $form['newlimit']['block'] = array(
    '#type' => 'value',
    '#value' => FALSE,
  );
  $form['newlimit']['operation'] = array(
    '#type' => 'value',
    '#value' => 'new',
  );

  $form['newlimit']['addlimit'] = array(
    '#type' => 'submit',
    '#value' => t('Add limit'),
    '#submit' => array('ces_bank_limitchain_form_addlimit'),
  );

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => $op == 'new' ? t('Create limit chain') : t('Update limit chain'),
  );
  return $form;
}
/**
 * Delete a limitchain.
 */
function ces_bank_limitchain_delete_form($form, &$form_state, $limitchain = FALSE) {
  $id       = $limitchain['id'];
  $name       = $limitchain['name'];
  $limits       = $limitchain['limits'];
  $limitnames = module_invoke_all('account_limit_classes');
  $form['limitchain_id'] = array(
    '#type' => 'value',
    '#value' => $id,
  );

  $form[$name] = array(
    '#type' => 'item',
    '#title' => t('Delete Limit Chain'),
    '#markup' => $name,
  );
  $out_text = '';
  foreach ($limits as $limit) {
    $limit_value = $limit['value'];
    $limit_classname = $limit['classname'];
    $out_text .= $limitnames[$limit_classname] . ' ' . $limit_value . '<br/>';
  }
  if (!empty($out_text)) {
    $form['limits'] = array(
      '#type' => 'item',
      '#title' => t('Limits'),
      '#markup' => $out_text,
    );
  }

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Confirm'),
  );
  return $form;
}
/**
 * Submit form for deleting limitchain.
 */
function ces_bank_limitchain_delete_form_submit($form, &$form_state) {
  $limitchain_id = $form_state['values']['limitchain_id'];
  if ($form_state['values']['op'] == t('Confirm')) {
    // Delete limitchain.

    $bank = new CesBank();
    try {
      $bank->deleteLimitChain($limitchain_id);
    }
    catch (Exception $e) {
      drupal_set_message(t('An error occurred while delete the limitchain record. Details: %msg', array('%msg' => $e->getMessage())), 'error');
    }
  }
  $form_state['redirect'] = 'ces/admin/limitchain';
}
/**
 * Helper function.
 */
function _ces_bank_limitchain_limit_fieldset($form, &$form_values) {
  if (!isset($form_values['operation'])) {
    $form_values['operation'] = 'edit';
  }
  $options = module_invoke_all('account_limit_classes');
  $limit = array(
    '#type' => 'fieldset',
    '#title' => $options[$form_values['classname']],
    '#description' => $options[$form_values['classname']],
  );
  $limit['classname'] = array(
    '#type' => 'value',
    '#value' => $form_values['classname'],
  );
  $limit['operation'] = array(
    '#type' => 'value',
    '#value' => $form_values['operation'],
  );
  if ($form_values['operation'] != 'new') {
    $limit['id'] = array(
      '#type' => 'value',
      '#value' => $form_values['id'],
    );
  }
  $limit['block'] = array(
    '#type' => 'checkbox',
    '#title' => t('Block'),
    '#description' => t('Block transactions when account reaches the limit vaule.'),
    '#default_value' => $form_values['block'],
  );
  $limit['value'] = array(
    '#type' => 'textfield',
    '#title' => t('Limit value'),
    '#description' => t('Value for this limit class'),
    '#default_value' => $form_values['value'],
    '#required' => TRUE,
  );
  $limit['deletelimit'] = array(
    '#type' => 'submit',
    '#value' => t('Delete'),
    '#name' => (isset($form_values['id'])) ? $form_values['id'] : 'op',
    '#submit' => array('ces_bank_limitchain_form_deletelimit'),
  );
  return $limit;
}
/**
 * TODO: Document.
 */
function ces_bank_limitchain_form_addlimit($form, &$form_state) {
  $form_state['rebuild'] = TRUE;
  $parents = $form_state['clicked_button']['#parents'];
  $value = &$form_state['values'];
  for ($i = 0; $i < count($parents) - 2; $i++) {
    $value = &$value[$parents[$i]];
  }
  $value['limits'][] = $value['newlimit'];
}
/**
 * Delete a limit of limitchain in the form and set flag to rebuild form.
 */
function ces_bank_limitchain_form_deletelimit($form, &$form_state) {
  $form_state['rebuild'] = TRUE;
  $parents = $form_state['clicked_button']['#parents'];
  // Go to the parent element of the clicked button and unset it!
  $value = &$form_state['values'];
  for ($i = 0; $i < count($parents) - 2; $i++) {
    $value = &$value[$parents[$i]];
    $key = $parents[$i + 1];
  }
  $limit_key = $value[$key]['id'];
  // Delete limit.

  $s = new IcesSerializer('CesBankAccountLimit');
  $limit = $s->loadFromUniqueKey('id', $limit_key);
  $s->delete($limit);
  unset($value[$key]);

}
/**
 * Validate the limit chain form.
 */
function ces_bank_limitchain_form_validate($form, &$form_state) {
  $values = $form_state['values'];
  if (!empty($values['limits'])) {
    foreach ($values['limits'] as $limit) {
      $amount = $limit['value'];
      if (!is_numeric($amount)) {
        form_set_error('value', t('The limit amount must be numeric.'));
      }
      if ($limit['classname'] == 'CesBankAbsoluteCreditLimit') {
        if ($amount < 0) {
          form_set_error('value', t('The credit limit must not be negative.'));
        }
      }
      elseif ($limit['classname'] == 'CesBankAbsoluteDebitLimit') {
        if ($amount > 0) {
          form_set_error('value', t('The debit limit must not be positive.'));
        }
      }
    }
  }
}
/**
 * Submit the limit chain form, for new or current limit chains.
 */
function ces_bank_limitchain_form_submit($form, &$form_state) {
  $bank = new CesBank();
  $op = $form_state['values']['operation'];
  try {
    if ($op == 'new') {
      $bank->createLimitChain($form_state['values']);
      drupal_set_message(t('Limit chain %name successfully created.', array('%name' => $form_state['values']['name'])));
      $form_state['redirect'] = 'ces/admin/limitchain';
    }
    elseif ($op == 'edit') {
      $bank->updateLimitChain($form_state['values']);
      drupal_set_message(t('Limit chain %name successfully updated.', array('%name' => $form_state['values']['name'])));
    }
  }
  catch (Exception $e) {
    drupal_set_message(t('An error occurred while saving the record. Details: %msg', array('%msg' => $e->getMessage())), 'error');
  }
}
/** @} */
/**
 * Main bank account form.
 *
 * @todo other operations.
 */
function ces_bank_account_form($form, &$form_state, $account = NULL) {

  $bank = new CesBank();
  if ($account != NULL) {
    if (is_int($account)) {
      $account = $bank->getAccount($account);
    }
    $exchange = $bank->getExchange($account['exchange']);
  }
  else {
    $exchange = ces_bank_get_current_exchange();
    if ($exchange === FALSE) {
      return array(
        'message' => array(
          '#markup' => '<p>' . t('Invalid current exchange') . '</p>',
        ),
      );
    }
    $account = $bank->getDefaultAccount($exchange['id']);
  }
  $admin = ces_bank_access('admin', 'exchange', $exchange['id']);

  if (!empty($form_state['values'])) {
    $account = array_merge($account, $form_state['values']);
  }

  $form['bankaccount'] = array();
  if (isset($account['id'])) {
    $form['bankaccount']['id'] = array(
      '#type' => 'value',
      '#value' => $account['id'],
    );
  }
  $form['bankaccount']['exchangename'] = array(
    '#type' => 'item',
    '#title' => t('Exchange'),
    '#markup' => $exchange['code'] . ' - ' . $exchange['name'],
  );
  $form['bankaccount']['exchange'] = array(
    '#type' => 'value',
    '#value' => $exchange['id'],
  );
  $form['bankaccount']['name'] = array(
    '#type' => 'textfield',
    '#description' => t('Four digits code.'),
    '#title' => t('Code'),
    '#default_value' => $account['name'],
    '#required' => TRUE,
    '#access' => $admin,
  );
  // Account users.
  $form['bankaccount']['users'] = array(
    '#tree' => TRUE,
    '#access' => $admin,
  );
  foreach ($account['users'] as $accuser) {
    $accuserform = array();
    if (isset($accuser['id'])) {
      $accuserform['id'] = array(
        '#type' => 'value',
        '#value' => $accuser['id'],
      );
    }
    if (!empty($accuser['user'])) {
      $accuserform['user'] = array(
        '#type' => 'value',
        '#value' => $accuser['user'],
      );
    }
    else {
      $accuserform['username'] = array(
        '#title' => t('Username'),
        '#description' => t('The username of the owner of this account.'),
        '#type' => 'textfield',
        '#autocomplete_path' => 'ces/bank/user/autocomplete',
        '#default_value' => isset($accuser['username']) ? $accuser['username'] : '',
      );
    }
    $accuserform['privilege'] = array(
      '#type' => 'value',
      '#value' => 0,
    );
    $form['bankaccount']['users'][] = $accuserform;
  }

  $options = array(
    0 => t('Individual'),
    1 => t('Shared'),
    2 => t('Organization'),
    3 => t('Company'),
    4 => t('Public'),
  );
  if ($account['kind'] == 5) {
    $options[5] = t('Virtual');
  }
  $form['bankaccount']['kind'] = array(
    '#type' => 'select',
    '#title' => t('Type'),
    '#description' => t('Select "Individual" for personal use, "Shared" if several people share this account, "Organization" for a non-profit organization, "Company" for a profit-pursuing company and "Public" for a public account.'),
    '#options' => $options,
    '#required' => TRUE,
    '#default_value' => $account['kind'],
  );
  if ($account['kind'] == 5) {
    $form['bankaccount']['kind']['#disabled'] = TRUE;
  }
  $limitchains = $bank->getAllLimitChains($exchange['id']);
  $options = array();
  foreach ($limitchains as $id => $limit) {
    $options[$id] = $limit['name'];
  }
  $form['bankaccount']['limitchain'] = array(
    '#type' => 'select',
    '#title' => t('Limit chain'),
    '#description' => t('The limit chain to apply to this account'),
    '#options' => $options,
    '#default_value' => $account['limitchain'],
    '#access' => $admin,
    '#required' => TRUE,
  );
  $form['bankaccount']['state'] = array(
    '#type' => 'select',
    '#title' => t('State'),
    '#description' => t('Active is the common state.'),
    '#options' => array(
      0 => t('Hidden'),
      1 => t('Active'),
      2 => t('Closed'),
      3 => t('Locked'),
    ),
    '#default_value' => $account['state'],
    '#access' => $admin,
    '#required' => TRUE,
  );

  $form['bankaccount']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save'),
  );
  return $form;
}
/**
 * Validate main CES bank account form.
 */
function ces_bank_account_form_validate($form, &$form_state) {
  $record = &$form_state['values'];
  // Validate username.
  $users = $record['users'];
  foreach ($users as $key => $accuser) {
    if (isset($accuser['username'])) {
      $user = user_load_by_name($accuser['username']);
      if ($user === FALSE) {
        form_set_error('username', t('Username %name does not exist.', array('%name' => $accuser['username'])));
      }
      else {
        $record['users'][$key]['user'] = $user->uid;
      }
    }
  }
  // Validate account number.
  $name = $record['name'];
  if (isset($record['id']) && !empty($record['id'])) {
    // Update account.
    $id_account = $record['id'];
    $bank = new CesBank();
    $account = $bank->getAccount($id_account);
    // Validate no change name of admin account (0000).
    $number_original = drupal_substr($account['name'], 4);
    $number_account  = drupal_substr($name, 4);
    if ($number_original == '0000' && $number_original != $number_account) {
      form_set_error('name', t('The name of the administrator account can not be changed'));
    }
  }
  else {
    // New account.
    $bank = new CesBank();
    $account = $bank->getAccountByName($name);
    if ($account != FALSE) {
      form_set_error('name', t('Account number %name already exists.', array('%name' => $name)));
    }
  }
  // Validate code of exchange.
  $current_exchange = ces_bank_get_current_exchange();
  $code = $current_exchange['code'];
  if (drupal_substr($name, 0, 4) != $code) {
    form_set_error('name', t('The first four carecteres should be the network (%code).', array('%code' => $code)));
  }
  // Validate number of code, omly is not virtual account.
  if ($record['kind'] != 5) {
    $number = drupal_substr($name, 4);
    if (!is_numeric($number)) {
      form_set_error('name', t('The last four digits must be a number. (%number).', array('%number' => $number)));
    }
  }
}
/**
 * Submit bank account form.
 */
function ces_bank_account_form_submit($form, &$form_state) {
  $record = &$form_state['values'];
  foreach ($record['users'] as $key => $accuser) {
    if (isset($record['id'])) {
      $record['users'][$key]['account'] = $record['id'];
    }
  }
  $bank = new CesBank();
  try {
    $activate = FALSE;
    if (!isset($record['id'])) {
      // New account.
      $bank->createAccount($record);
      drupal_set_message(t('Account successfully created.'));
      if ($record['state'] == CesBankLocalAccount::STATE_HIDDEN) {
        drupal_set_message(t("The account will not be available until it is being activated by this exchange's administrator. You will receive an email when this is done."));
      }
      elseif ($record['state'] == CesBankLocalAccount::STATE_ACTIVE) {
        $activate = TRUE;
      }
    }
    else {
      $previous = $bank->getAccount($record['id']);
      $bank->updateAccount($record);
      if ($previous['state'] == CesBankLocalAccount::STATE_HIDDEN && $record['state'] == CesBankLocalAccount::STATE_ACTIVE) {
        $activate = TRUE;
      }
    }
    if ($activate) {
      // Activate Drupal user(s).
      foreach ($record['users'] as $key => $accuser) {
        $user = user_load($accuser['user']);
        if ($user->status == 0) {
          $user->status = 1;
          user_save($user);
        }
      }
      // Activate banking account.
      $bank->activateAccount($record);
      drupal_set_message(t('Account successfully activated.'));
    }
  }
  catch (Exception $e) {
    drupal_set_message(t('An error occurred while saving the account record. Details: %msg', array('%msg' => $e->getMessage())), 'error');
  }
  drupal_set_message(t('Account successfully update.'));
}
/**
 * Form for deleting an account.
 */
function ces_bank_account_delete_form($form, &$form_state, $account = NULL) {
  if (!empty($form_state['values']) && !empty($form_state['values']['page'])
      && $form_state['values']['page'] == 'confirm') {
    return ces_bank_account_delete_confirm_form($form, $form_state);
  }
  $bank = new CesBank();
  if ($account != NULL && !is_array($account)) {
    $account = $bank->getAccount($account);
  }
  if ($account == NULL || $account === FALSE) {
    return array(
      'message' => array(
        '#markup' => '<p>' . t('Invalid account.') . '</p>',
      ),
    );
  }
  $check = $bank->deleteAccountsCheck(array($account['id']));
  $checking = $check['accounts'][0];
  // Display interexchange account information.
  $textoutput = $checking['is_interexchange'] ?
    t('This account depends on other exchanges in the system, so the account will not be deleted, nor closed') .
    '<br /><br />' : '';
  $form['data-account'] = array(
    '#type' => 'item',
    '#markup' => '<h3>' . t('Delete account') . ': <b>' . $checking['name'] . '</b></h3>' . $textoutput,
  );
  if (!$checking['is_interexchange']) {
    // Display transactions information.
    $textoutput = '';
    $texttrans = '';
    if ($checking['num_transactions'] > 0) {
      foreach ($checking['transactions'] as $trans) {
        $texttrans .= l($trans['concept'], 'ces/bank/account/transaction/' . $trans['id'] . '/view') . ' -- ';
      }
      if (strlen($texttrans) > 0) {
        $texttrans = substr($texttrans, 0, -3);
      }
      $textoutput .= t('These transactions') . ' (<b>' . $checking['num_transactions'] . '</b>) ' .
      t('refer to this account, so the account can only be closed, not deleted.') . '<br />' . $texttrans;
    }
    else {
      $textoutput .= t('No transaction refers to this account.');
    }
    $form['trans_fieldset'] = array(
      '#type' => 'fieldset',
      '#title' => t('Transactions'),
    );
    $form['trans_fieldset']['data-trans'] = array(
      '#type' => 'item',
      '#markup' => $textoutput,
    );
    // Display users information.
    $textoutput = '';
    $textusers1 = '';
    $textusers2 = '';
    $numusers = 0;
    $userswithacc = 0;
    foreach ($checking['account_users'] as $accuser) {
      $userloaded = user_load($accuser['uid']);
      $username = ces_bank_get_full_username($userloaded);
      $username .= '(' . $userloaded->name . ')';
      $checking['account_users'][$accuser['uid']]['name'] = $username;
      $textusers1 .= l($username, 'user/' . $userloaded->uid) . ', ';
      $numusers++;
      if (!empty($accuser['accounts'])) {
        $textusers2 .= '<br />' . t('User') . ' <b>' . $userloaded->name . '</b> ' .
          t('has other accounts and cannot be deleted nor canceled:') . '<br />';
        foreach ($accuser['accounts'] as $acc) {
          $textusers2 .= l($acc['name'], 'ces/admin/account/' . $acc['aid'] . '/view') . ' ';
          $userswithacc++;
        }
      }
    }
    if (strlen($textusers1) > 0) {
      $textusers1 = substr($textusers1, 0, -2);
    }
    if ($userswithacc == 0) {
      $textusers2 .= ($numusers == 1) ? '<br />' . t("This user doesn't have more accounts.") :
        '<br />' . t("These users doesn't have more accounts.");
    }
    $textoutput .= ($numusers == 1) ? t('The account belongs to a single user') . ': <b>' . $textusers1 . ' </b>' :
                   t('The account belongs to') . ' ' . $numusers . ' ' . t('users') . ': <b>' . $textusers1 . ' </b>';
    $textoutput .= $textusers2;
    $form['users_fieldset'] = array(
      '#type' => 'fieldset',
      '#title' => t('Users'),
    );
    $form['users_fieldset']['data-users'] = array(
      '#type' => 'item',
      '#markup' => $textoutput,
    );
    // Action options for users account.
    foreach ($checking['account_users'] as $accuser) {
      if ($accuser['user_action'] != 'nothing') {
        $options = ($accuser['user_action'] == 'delete') ? array(
          'delete' => t('Delete user (The user will no longer exist in the system)'),
          'cancel' => t('Cancel user (The user will not be able to login in the system)'),
          'nothing' => t('Do nothing')) : array(
            'cancel' => t('Cancel user (The user will not be able to login in the system)'),
            'nothing' => t('Do nothing'),
          );
        $default_value = ($accuser['user_action'] == 'delete') ? 'delete' : 'cancel';
        $form['user-' . $accuser['uid']] = array(
          '#title' => check_plain(t('Action with user') . ' ' . $accuser['name']),
          '#type' => 'radios',
          '#prefix' => '<br />',
          '#description' => t('Select the action you want to do with this user.'),
          '#options' => $options,
          '#default_value' => $default_value,
          '#required' => TRUE,
        );
      }
    }
    // Submit action account.
    $submittext = '';
    switch ($checking['account_action']) {
      case 'delete':
        $submittext = t('Delete account');
        break;

      case 'close':
        $submittext = t('Close account');
        break;

      case 'nothing':
        $submittext = t('It is NOT possible to delete nor to close the account.');
        break;

    }
    if ($checking['account_action'] == 'nothing') {
      $form['not-possible'] = array(
        '#type' => 'item',
        '#markup' => $submittext,
      );
    }
    else {
      $form['submit'] = array(
        '#type' => 'submit',
        '#prefix' => '<br />',
        '#value' => $submittext,
      );
    }
  }
  $form_state['check_account_data'] = $checking;
  return $form;
}
/**
 * Validate form for deleting account.
 */
function ces_bank_account_delete_form_validate($form, &$form_state) {
  foreach ($form_state['check_account_data']['account_users'] as $user) {
    if (isset($form_state['values']['user-' . $user['uid']])) {
      $check = $form_state['check_account_data']['account_users'][$user['uid']]['user_action'];
      $option = $form_state['values']['user-' . $user['uid']];
      if (($check == 'cancel' && $option == 'delete') ||
        ($check == 'nothing' && ($option == 'delete' || $option == 'cancel'))) {
        form_set_error('user-' . $user['uid'], t("Hey! That's not a valid option!"));
      }
      else {
        $form_state['check_account_data']['account_users'][$user['uid']]['user_action'] = $option;
      }
    }
  }
}
/**
 * Submit form for deleting account.
 */
function ces_bank_account_delete_form_submit($form, &$form_state) {
  $form_state['values']['page'] = 'confirm';
  $form_state['rebuild'] = TRUE;
}
/**
 * Confirm form for deleting account.
 */
function ces_bank_account_delete_confirm_form($form, &$form_state) {
  $textoutput = '';
  switch ($form_state['check_account_data']['account_action']) {
    case 'delete':
      $textoutput .= t('Delete account');
      break;

    case 'close':
      $textoutput .= t('Close account');
      break;

    case 'nothing':
      $textoutput .= t('Do nothing with account');
      break;

  }
  $textoutput .= ' <i>' . $form_state['check_account_data']['name'] . '</i><br />';
  foreach ($form_state['check_account_data']['account_users'] as $user) {
    switch ($user['user_action']) {
      case 'delete':
        $textoutput .= t('Delete user (and all content: offers, wants, comments...)');
        break;

      case 'cancel':
        $textoutput .= t('Cancel user');
        break;

      case 'nothing':
        $textoutput .= t('Do nothing with user');
        break;

    }
    $textoutput .= ' <i>' . $user['name'] . '</i><br />';
  }
  $form['confirm_fieldset'] = array(
    '#type' => 'fieldset',
    '#title' => t('Confirm actions for account') . ' <b>' . $form_state['check_account_data']['name'] . '</b>',
  );
  $form['confirm_fieldset']['data'] = array(
    '#type' => 'item',
    '#markup' => $textoutput,
  );
  $notice = ($form_state['check_account_data']['account_action'] == 'delete') ?
    '<h3>' . t('This action cannot be undone.') . '</h3>' : '<br />';
  $form['submit'] = array(
    '#type' => 'submit',
    '#prefix' => $notice,
    '#value' => t('Confirm'),
    '#submit' => array('ces_bank_account_delete_confirm_form_submit'),
  );
  $form['actions']['cancel'] = array(
    '#markup' => l(t('Cancel'), 'ces/admin/account'),
  );
  return $form;
}
/**
 * Submit confirm form for deleting account.
 */
function ces_bank_account_delete_confirm_form_submit($form, &$form_state) {
  $bank = new CesBank();
  $check = $form_state['check_account_data'];
  switch ($check['account_action']) {
    case 'delete':
      try {
        $action = $bank->deleteAccount($check['aid']);
        if ($action) {
          drupal_set_message(t('The account %account has been deleted.',
            array('%account' => $check['name'])), 'status');
        }
      }
      catch (Exception $e) {
        drupal_set_message(t('An error occurred while deleting the account. Details: %msg',
          array('%msg' => $e->getMessage())), 'error');
      }
      break;

    case 'close':
      try {
        $account = $bank->getAccount($check['aid']);
        $account['state'] = 2;
        $bank->updateAccount($account);
        drupal_set_message(t('The account %account has been closed.',
          array('%account' => $check['name'])), 'status');
      }
      catch (Exception $e) {
        drupal_set_message(t('An error occurred while closing the account. Details: %msg',
          array('%msg' => $e->getMessage())), 'error');
      }
      break;

    case 'nothing':
      drupal_set_message(t('The account %account remains as before. No action has done to this account.',
        array('%account' => $check['name'])), 'status');
      break;

  }
  $users_cancel = array();
  foreach ($check['account_users'] as $user) {
    if ($user['user_action'] == 'delete' || $user['user_action'] == 'cancel') {
      $users_cancel[] = array(
        'uid' => $user['uid'],
        'action' => $user['user_action'],
      );
    }
    else {
      drupal_set_message(t('The user %user remains as before. No action has done to this user.',
        array('%user' => $user['name'])), 'status');
    }
  }
  if (!empty($users_cancel)) {
    $bank->deleteUsers($users_cancel);
  }
  $form_state['redirect'] = 'ces/admin/account';
}
/**
 * Single transaction form.
 *
 * @param array $form
 *   The Drupal form.
 * @param array $form_state
 *   The form input values.
 * @param string $op
 *   The operation to perform on the transaction:
 *     - 'new': Create new transaction.
 *     - 'edit': Edit transaction.
 * @param array|int $arg
 *   Argument to this form depending on the operation. If the operation is 'new'
 *   $arg is the id of the exchange where the transaction is going to take
 *   place. If the operation is 'edit', $arg is the transaction array.
 * @param int $to_account_id
 *   Optional. The seller account id.
 * @param int $from_account_id
 *   Optional. The buyer account.
 *
 * @todo Set the currency according the toaccount exchange currency.
 * @todo Make an easy system to find account names by exchange/username.
 */
function ces_bank_transaction_form($form, &$form_state, $op, $arg, $to_account_id = NULL, $from_account_id = NULL) {
  if (!isset($form_state['values']) || !isset($form_state['values']['page'])
      || $form_state['values']['page'] == 'edit') {
    global  $user;
    $bank = new CesBank();
    // Exchange options for selects.
    $exchanges = ces_bank_get_exchanges();
    if (empty($exchanges)) {
      return array(
        'message' => array(
          '#markup' => '<p>' . t('There are no exchanges yet.') . '</p>',
        ),
      );
    }
    $exoptions = array();
    foreach ($exchanges as $exchange) {
      $exoptions[$exchange['id']] = $exchange['code'];
    }
    if ($op == 'new') {
      $exchangeid = $arg;
      if (!isset($exchanges[$exchangeid])) {
        return array(
          'message' => array(
            '#markup' => '<p>' . t('Invalid exchange id.') . '</p>',
          ),
        );
      }
    }
    elseif ($op == 'edit') {
      if (!isset($form_state['values'])) {
        $form_state['values'] = array();
      }
      $form_state['values'] += $arg;
      $from = $bank->getTransactionFromAccount($arg);
      $to = $bank->getTransactionToAccount($arg);
      $toexchange = $exchanges[$to['exchange']];
      // During the first form build, there are no $form_state['values'], but
      // there may be subsequent executions (due to ajax calls) where we want to
      // preserve the existing value from the user.
      if (empty($form_state['values']['toexchange'])) {
        $form_state['values']['toexchange'] = $to['exchange'];
      }
      if (empty($form_state['values']['toaccountname'])) {
        $form_state['values']['toaccountname'] = $to['name'];
      }
      if (empty($form_state['values']['fromexchange'])) {
        $form_state['values']['fromexchange'] = $from['exchange'];
      }
      if (empty($form_state['values']['fromaccountname'])) {
        $form_state['values']['fromaccountname'] = $from['name'];
      }
    }
    $form['operation'] = array(
      '#type' => 'value',
      '#value' => $op,
    );
    if (isset($form_state['values']['id'])) {
      $form['id'] = array(
        '#type' => 'value',
        '#value' => $form_state['values']['id'],
      );
    }
    // Select seller's exchange.
    $toexchangeid = isset($form_state['values']['toexchange']) ? $form_state['values']['toexchange'] : $exchangeid;
    $toexchange = $exchanges[$toexchangeid];
    $accountconditions = array(
      'state' => 1,
      'kind' => array(
        CesBankLocalAccount::TYPE_INDIVIDUAL,
        CesBankLocalAccount::TYPE_SHARED,
        CesBankLocalAccount::TYPE_ORGANIZATION,
        CesBankLocalAccount::TYPE_COMPANY,
        CesBankLocalAccount::TYPE_PUBLIC,
      ),
    );
    if ($to_account_id == NULL) {
      $form['toexchange'] = array(
        '#title' => t('Seller exchange'),
        '#description' => t("The seller's account exchange."),
        '#type' => 'select',
        '#options' => $exoptions,
        '#default_value' => $toexchangeid,
        '#ajax' => array(
          'callback' => 'ces_bank_toexchange_dropdown_callback',
        ),
        '#weight' => 30,
      );
      // Select seller's account.
      $accountconditions['exchange'] = $toexchangeid;
      $accounts = $bank->getAllAccounts($accountconditions);
      $tooptions = array();
      foreach ($accounts as $account) {
        $useracc = reset($account['users']);
        $tooptions[$account['name']] = $account['name'] . ' - ' . ces_user_get_name(user_load($useracc['user']));
      }
      $form['toaccountname'] = array(
        '#title' => t('Seller account'),
        '#description' => t('The recipient of the transaction. The Exchange four letters code plus four numbers.'),
        '#type' => 'select',
        '#options' => $tooptions,
        '#prefix' => '<div id="dropdown-toaccountname-replace">',
        '#suffix' => '</div>',
        '#weight' => 40,
      );
      if (isset($form_state['values']['toaccountname'])) {
        $form['toaccountname']['#default_value'] = $form_state['values']['toaccountname'];
      }
    }
    else {
      $toaccount = $bank->getAccount($to_account_id);
      if (empty($toaccount)) {
        drupal_set_message(t('Account id %id does not exist, is not active yet, or does not belong to exchange %ex', array('%id' => $to_account_id, '%ex' => $toexchange['code'])), 'error');
        return FALSE;
      };
      $form['toexchange'] = array(
        '#type' => 'value',
        '#value' => $toexchangeid,
      );
      $form['toaccountname'] = array(
        '#type' => 'value',
        '#value' => $toaccount['name'],
      );
    }
    $fromexchangeid = isset($form_state['values']['fromexchange']) ? $form_state['values']['fromexchange'] : $exchangeid;
    if ($from_account_id == NULL) {
      // Select buyer's exchange.
      $form['fromexchange'] = array(
        '#title' => t('Buyer exchange'),
        '#description' => t("The buyer's account exchange."),
        '#type' => 'select',
        '#options' => $exoptions,
        '#default_value' => $fromexchangeid,
        '#ajax' => array(
          'callback' => 'ces_bank_fromaccountname_dropdown_callback',
          'wrapper' => 'dropdown-fromaccountname-replace',
        ),
        '#weight' => 10,
      );
      // Select buyer's account.
      if ($fromexchangeid == $toexchangeid && !empty($tooptions)) {
        $fromoptions = &$tooptions;
      }
      else {
        $fromoptions = array();
        $accountconditions['exchange'] = $fromexchangeid;
        $accounts = $bank->getAllAccounts($accountconditions);
        foreach ($accounts as $account) {
          $useracc = reset($account['users']);
          $fromoptions[$account['name']] = $account['name'] . ' - ' . ces_user_get_name(user_load($useracc['user']));
        }
      }
      $form['fromaccountname'] = array(
        '#title' => t('Buyer account'),
        '#description' => t('The payer of the transaction. The Exchange four letters code plus four numbers.'),
        '#type' => 'select',
        '#options' => $fromoptions,
        '#prefix' => '<div id="dropdown-fromaccountname-replace">',
        '#suffix' => '</div>',
        '#weight' => 20,
      );
      if (isset($form_state['values']['fromaccountname'])) {
        $form['fromaccountname']['#default_value'] = $form_state['values']['fromaccountname'];
      }
    }
    else {
      $fromaccount = $bank->getAccount($from_account_id);
      $fromexchange = $exchanges[$fromexchangeid];
      if (empty($fromaccount)) {
        drupal_set_message(t('Account id %id does not exist, is not active yet, or does not belong to exchange %ex', array('%id' => $from_account_id, '%ex' => $fromexchange['code'])), 'error');
        return FALSE;
      }

      $form['fromexchange'] = array(
        '#type' => 'value',
        '#value' => $fromexchangeid,
      );
      $form['fromaccountname'] = array(
        '#type' => 'value',
        '#value' => $fromaccount['name'],
      );
    }
    $form['concept'] = array(
      '#type'  => 'textfield',
      '#title' => t('Description'),
      '#description' => t('Enter the concept of the transference.'),
      '#default_value' => !empty($form_state['values']['concept']) ? $form_state['values']['concept'] : '',
      '#weight' => 50,
      '#required' => TRUE,
    );
    $form['amount'] = array(
      '#type' => 'textfield',
      '#size' => '5',
      '#title' => t('Amount in @currenciesname', array('@currenciesname' => $toexchange['currenciesname'])),
      '#description' => t('The amount to be transferred.'),
      '#default_value' => !empty($form_state['values']['amount']) ? $form_state['values']['amount'] : '',
      '#weight' => 60,
      '#field_suffix' => $toexchange['currencysymbol'],
      '#prefix' => '<div id="textfield-amount-replace">',
      '#suffix' => '</div>',
      '#required' => TRUE,
    );
    $form['user'] = array(
      '#type' => 'value',
      '#value' => isset($form_state['values']['user']) ? $form_state['values']['user'] : $user->uid,
    );
    $form['submit'] = array(
      '#type' => 'submit',
      '#value' => $op == 'new' ? t('Create transaction') : t('Update transaction'),
      '#weight' => 100,
    );
  }
  elseif ($form_state['values']['page'] == 'confirm') {
    $form = ces_bank_transaction_confirm_form($form, $form_state, $form_state['values']['operation']);
  }
  return $form;
}
/**
 * Form for accept transaction.
 */
function ces_bank_transaction_accept_form($form, &$form_state, $transaction) {
  global $user;
  $user = user_load($user->uid);
  // Choose current account.
  $account_id = $transaction['fromaccount'];
  $user->data['ces_current_account'] = $account_id;
  user_save($user);
  // Prepare form.
  drupal_set_title(t('Authorize transaction'), PASS_THROUGH);
  $form['transaction'] = array(
    '#type' => 'value',
    '#value' => $transaction['id'],
  );
  $form['account'] = array(
    '#type' => 'value',
    '#value' => $account_id,
  );
  $form['#attributes']['class'][] = 'confirmation';
  $form['description'] = array(
    '#markup' => t('Do you accept the following transaction from your account?'),
    '#prefix' => '<h3>',
    '#suffix' => '</h3>',
  );
  module_load_include('pages.inc', 'ces_bank');
  $details = ces_bank_transaction_view($transaction);
  $form['details'] = array(
    '#markup' => drupal_render($details),
  );
  $form['actions'] = array('#type' => 'actions');
  $form['actions']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Accept'),
    '#submit' => array('ces_bank_transction_authorize_form_submit'),
  );
  $form['actions']['reject'] = array(
    '#type' => 'submit',
    '#value' => t('Reject'),
    '#submit' => array('ces_bank_transction_reject_form_submit'),
  );
  $form['actions']['cancel'] = array(
    '#type' => 'link',
    '#title' => t('Cancel'),
    '#href' => 'ces/bank/account/transaction',
  );
  return $form;
}
/**
 * Submit function for ces_bank_accept_transaction_form.
 */
function ces_bank_transction_authorize_form_submit($form, &$form_state) {
  $transaction_id = $form_state['values']['transaction'];
  $account_id = $form_state['values']['account'];
  $bank = new CesBank();
  $res = $bank->acceptTransaction($transaction_id, $account_id);
  $record = $bank->getTransaction($transaction_id);
  _ces_bank_set_transaction_message($res, $record);
  $form_state['redirect'] = 'ces/bank/account/transaction/' . $transaction_id . '/view';
}
/**
 * Subit function for reject button of ces_bank_accept_transaction_form.
 */
function ces_bank_transction_reject_form_submit($form, &$form_state) {
  $transaction_id = $form_state['values']['transaction'];
  $bank = new CesBank();
  $res = $bank->rejectTransaction($transaction_id);
  $record = $bank->getTransaction($transaction_id);
  _ces_bank_set_transaction_message($res, $record, 'reject');
  $form_state['redirect'] = 'ces/bank/account/transaction/' . $transaction_id . '/view';
}
/**
 * AJAX callback for transaction form.
 */
function ces_bank_toexchange_dropdown_callback($form, $form_state) {
  // Using ajax commands since we need to replace two form elements. Note that
  // this way the drupal testing module cannot handle this calls.
  $commands = array();
  $commands[] = ajax_command_replace('#dropdown-toaccountname-replace', drupal_render($form['toaccountname']));
  $commands[] = ajax_command_replace('#textfield-amount-replace', drupal_render($form['amount']));
  return array('#type' => 'ajax', '#commands' => $commands);
}
/**
 * AJAX callback for transaction form.
 */
function ces_bank_fromaccountname_dropdown_callback($form, &$form_state) {
  return $form['fromaccountname'];
}
/**
 * Submit transaction form.
 */
function ces_bank_transaction_form_submit($form, &$form_state) {
  $form_state['values']['page'] = 'confirm';
  $form_state['rebuild'] = TRUE;
}
/**
 * Submit confirm transaction form.
 *
 * @param string $op
 *   One of:
 *   - new: create and trigger transaction.
 *   - edit: edit a new transaction.
 *   - trigger: trigger an already created transaction.
 *   - discard: discard a rejected transaction (TODO).
 *   - revoke: revoke a committed transaction (TODO).
 * @param int $id
 *   the id of the transaction. If $op='new', then it is ignored
 *   in favor of $form_state values coming from ces_bank_transaction_form.
 */
function ces_bank_transaction_confirm_form($form, &$form_state, $op, $id = NULL) {
  $bank = new CesBank();
  $transaction = $form_state['values'];
  // Copy values from previous form in a safe place.
  $form_state['edit_page'] = $transaction;
  if (!empty($transaction['id'])) {
    $id = $transaction['id'];
  }
  if (!empty($id)) {
    try {
      $transaction += $bank->getTransaction($id);
    }
    catch (Exception $e) {
      drupal_set_message($e->getMessage(), 'error');
      return array();
    }
  }
  if (empty($transaction)) {
    drupal_set_message(t('No transaction provided.'), 'error');
    return array();
  }
  if ($op != 'new' && $op != 'trigger' && $op != 'edit') {
    drupal_set_message(t('Operation not permitted or not implemented yet.'), 'error');
    return array();
  }
  if (isset($transaction['id'])) {
    $form['id'] = array(
      '#type' => 'value',
      '#value' => $transaction['id'],
    );
  }
  $form['operation'] = array(
    '#type' => 'value',
    '#value' => $op,
  );
  // This value is used in ces_bank_transaction_form().
  $form['page'] = array(
    '#type' => 'value',
    '#value' => 'confirm',
  );
  $html = '<p>' . t('Are you sure you want to delete the transaction?') . '</p>';
  $html = '<p>' . t('Transaction details:') . '</p>';
  require_once drupal_get_path('module', 'ces_bank') . '/ces_bank.pages.inc';
  if ($op == 'new') {
    $transaction['state'] = 0;
    $transaction['created'] = REQUEST_TIME;
    $transaction['modified'] = REQUEST_TIME;
  }
  $view = ces_bank_transaction_view($transaction);
  $html .= '<p>' . drupal_render($view) . '</p>';
  $form['transaction'] = array(
    '#markup' => $html,
  );

  $form['back'] = array(
    '#type' => 'submit',
    '#value' => t('Back'),
    '#submit' => array('ces_bank_transaction_confirm_form_back'),
    '#limit_validation_errors' => array(),
  );
  $form['submit'] = array(
    '#type' => 'submit',
    '#submit' => array('ces_bank_transaction_confirm_form_submit'),
  );
  switch ($op) {
    case 'new':
      $form['submit']['#value'] = t('Confirm transaction');
      drupal_set_message(t('Confirm the transaction to end the process'), 'warning');
      break;

    case 'trigger':
      $form['submit']['#value'] = t('Trigger transaction');
      break;

    case 'edit':
      $form['submit']['#value'] = t('Update transaction');
      break;

  }
  return $form;
}
/**
 * Back button submit function for transaction confirm form.
 */
function ces_bank_transaction_confirm_form_back($form, &$form_state) {
  if (isset($form_state['edit_page'])) {
    $form_state['values'] = $form_state['edit_page'];
    $form_state['values']['page'] = 'edit';
    $form_state['rebuild'] = TRUE;
  }
  else {
    drupal_goto('ces/bank');
  }
}
/**
 * Validate transaction form.
 */
function ces_bank_transaction_form_validate($form, &$form_state) {
  if (!empty($form_state['values']['page']) && $form_state['values']['page'] == 'confirm') {
    return;
  }

  $bank = new CesBank();
  $record = $form_state['values'];
  $error = FALSE;
  $account1 = $bank->getAccountByName($record['toaccountname']);
  if ($account1 === FALSE) {
    form_set_error('toaccountname', t('Seller account %acc is incorrect.', array('%acc' => $account1)));
    $error = TRUE;
  }
  $account2 = $bank->getAccountByName($record['fromaccountname']);
  if ($account2 === FALSE) {
    form_set_error('fromaccountname', t('Buyer account %acc is incorrect.', array('%acc' => $account2)));
    $error = TRUE;
  }
  if ($account1['id'] == $account2['id']) {
    form_set_error('fromaccountname', t('Buyer account cannot be the same as seller account.'));
    $error = TRUE;
  }
  $amount = $record['amount'];
  if (!is_numeric($amount)) {
    form_set_error('amount', t('The amount entered is not a numeric value. Use the dot "." character for the decimal point and do not use any thousands separator.'));
    $error = TRUE;
  }
  if (!($amount > 0.0)) {
    form_set_error('amount', t('The amount must be positive.'));
    $error = TRUE;
  }
  $concept = trim($form_state['values']['concept']);
  if (!$error) {
    $transaction = array(
      'toaccount' => $account1['id'],
      'fromaccount' => $account2['id'],
      'amount' => $amount,
      'concept' => $concept,
      'user' => $form_state['values']['user'],
    );
    if (!ces_transaction_access('use', $transaction)) {
      form_set_error('toaccountname', t("You don't have permission to apply this transaction."));
    }
  }
}
/**
 * Submit confirmation form for transaction.
 */
function ces_bank_transaction_confirm_form_submit($form, &$form_state) {
  $bank = new CesBank();
  $result = TRUE;
  $op = $form_state['values']['operation'];
  $id = '';
  try {
    switch ($op) {
      case 'new':
        $record = $form_state['edit_page'];
        $bank->createTransaction($record);
        $id = $record['id'];
        $result = $bank->applyTransaction($id);
        break;

      case 'trigger':
        $id = $form_state['values']['id'];
        $result = $bank->applyTransaction($id);
        break;

      case 'edit':
        $record = $form_state['edit_page'];
        $result = $bank->updateTransaction($record);
        $id = $record['id'];
        break;

      default:
        throw new Exception(t('Operation not permitted or not implemented yet.'));
    }
    $record = $bank->getTransaction($record['id']);
    _ces_bank_set_transaction_message($result, $record, $op);
    $form_state['redirect'] = 'ces/bank/account/transaction/' . $id . '/view';
  }
  catch (Exception $e) {
    drupal_set_message($e->getMessage(), 'error');
    return;
  }
}
/**
 * Helper.
 */
function _ces_bank_set_transaction_message($result, $transaction, $op = 'trigger') {
  if ($result) {
    switch ($op) {
      case 'new':
      case 'trigger':
        drupal_set_message(t('Transaction successfully applied.'));
        break;

      case 'edit':
        drupal_set_message(t('Transaction successfully updated.'));
        break;

      case 'reject':
        drupal_set_message(t('Transaction has been rejected and will NOT be applied.'));
        break;

    }
  }
  else {
    $bank = new CesBank();
    $messages = $bank->getTransactionLog($transaction);
    $state = $bank->getTransactionState($transaction);
    if ($state == CesBankTransactionInterface::STATE_TRIGGERED) {
      foreach ($messages as $message) {
        // This filter_xss is here only so that coder module shuts up.
        drupal_set_message(filter_xss($message), 'warning');
      }
    }
    else {
      drupal_set_message(t('The transaction has NOT been applied.'), 'error');
      if (!empty($messages)) {
        $text = t('Details:');
        $text .= '<ul>';
        foreach ($messages as $message) {
          $text .= '<li>' . $message . '</li>';
        }
        $text .= '</ul>';
        // This filter_xss is here only so that coder module shuts up.
        drupal_set_message(filter_xss($text), 'error');
      }
    }
  }
}
/**
 * Multiple transaction form.
 */
function ces_bank_multipletransactions_form($form, &$form_state) {
  if (!empty($form_state['values']) && !empty($form_state['values']['page'])
      && $form_state['values']['page'] == 'confirm') {
    return ces_bank_multipletransactions_confirm_form($form, $form_state);
  }
  global  $user;
  $bank = new CesBank();
  $currentexchange = ces_bank_get_current_exchange();
  $currentaccount = ces_bank_get_current_account();
  if (empty($currentexchange) || empty($currentaccount)) {
    return array(
      'message' => array(
        '#markup' => '<p>' . t('Invalid exchange or account.') . '</p>',
      ),
    );
  }
  $exchangeid = $currentexchange['id'];
  $to_account_id = $currentaccount['id'];
  $toaccount = $bank->getAccount($to_account_id);
  if (empty($toaccount)) {
    drupal_set_message(t('Account id %id does not exist, is not active yet, or does not belong to exchange %ex', array('%id' => $to_account_id, '%ex' => $currentexchange['code'])), 'error');
    return FALSE;
  };
  $default = !empty($form_state['values']['howmany_transactions']) ? $form_state['values']['howmany_transactions'] : 10;
  $form['howmany_transactions_real'] = array(
    '#type' => 'hidden',
    '#value' => $default,
  );
  $form['howmany_transactions'] = array(
    '#attributes' => array('class' => array('container-inline')),
    '#type' => 'select',
    '#options' => array(10 => 10, 20 => 20, 30 => 30, 40 => 40, 50 => 50),
    '#default_value' => $default,
    '#ajax' => array(
      'callback' => 'ces_bank_multipletransactions_form_numtrans_callback',
      'wrapper' => 'multiple-transactions-div',
      'method' => 'replace',
      'effect' => 'fade',
    ),
  );
  $form['toexchange'] = array(
    '#type' => 'value',
    '#value' => $exchangeid,
  );
  $form['toaccountname'] = array(
    '#type' => 'value',
    '#value' => $toaccount['name'],
    '#suffix' => '',
  );

  $form['transaction_fieldset'] = array(
    '#prefix' => '<div id="multiple-transactions-div"><table class="ces-table" style="margin:20px 0;"><thead><tr><th></th><th>' . t('Account') . '</th><th>' . t('Concept') . '</th><th>' . $currentexchange['currenciesname'] . '</th></tr></thead><tbody>',
    '#suffix' => '</tbody></table></div>',
    '#type' => 'fieldset',
    '#attributes' => array('class' => array('element-invisible')),
  );

  for ($i = 1; $i <= $default; $i++) {
    $form['transaction_fieldset']['fromaccountname-' . $i] = array(
      '#title' => '',
      '#description' => '',
      '#prefix' => '<tr style="border-top:1px dotted #ccc;"><td style="padding: 5px 0;">' . $i . '</td><td>',
      '#suffix' => '</td>',
      '#type' => 'textfield',
      '#maxlength' => 32,
      '#autocomplete_path' => 'ces/bank/account/sell/multiple/autocomplete',
      '#default_value' => isset($form_state['values']['fromaccountname-' . $i]) ? $form_state['values']['fromaccountname-' . $i] : '',
    );
    $form['transaction_fieldset']['concept-' . $i] = array(
      '#type'  => 'textfield',
      '#title' => '',
      '#description' => '',
      '#prefix' => '<td>',
      '#suffix' => '</td>',
      '#size' => 25,
      '#default_value' => !empty($form_state['values']['concept-' . $i]) ? $form_state['values']['concept-' . $i] : '',
    );
    $form['transaction_fieldset']['amount-' . $i] = array(
      '#type' => 'textfield',
      '#size' => '5',
      '#title' => '',
      '#description' => '',
      '#prefix' => '<td>',
      '#suffix' => '</td></tr>',
      '#default_value' => !empty($form_state['values']['amount-' . $i]) ? $form_state['values']['amount-' . $i] : '',
    );
  }
  $form['user'] = array(
    '#type' => 'value',
    '#prefix' => '',
    '#value' => $user->uid,
  );
  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Create transactions'),
    '#weight' => 100,
  );
  return $form;
}
/**
 * AJAX callback for selector in multiple transactions form.
 */
function ces_bank_multipletransactions_form_numtrans_callback($form, $form_state) {
  return $form['transaction_fieldset'];
}
/**
 * AJAX autocomplete callback.
 * @return string
 *   Accounts in current exchange.
 */
function ces_bank_multipletransactions_form_autocomplete_callback($string) {
  $exchange = ces_bank_get_current_exchange();
  $bank = new CesBank();
  $accountconditions = array(
    'exchange' => $exchange['id'],
    'state' => 1,
    'kind' => array(
      CesBankLocalAccount::TYPE_INDIVIDUAL,
      CesBankLocalAccount::TYPE_SHARED,
      CesBankLocalAccount::TYPE_ORGANIZATION,
      CesBankLocalAccount::TYPE_COMPANY,
      CesBankLocalAccount::TYPE_PUBLIC,
    ));
  $accounts = $bank->getAllAccounts($accountconditions);
  $string = strtoupper($string);
  $matches = array();
  $items = 0;
  foreach ($accounts as $acc) {
    $accname = strtoupper($acc['name']);
    $name_user_account = '';
    foreach ($acc['users'] as $u) {
      $name_user_account .= ces_user_get_name(user_load($u['user'])) . ' ';
    }
    $accname .= ' ' . strtoupper($name_user_account);
    $pos = strpos($accname, $string);
    if ($pos !== FALSE) {
      $matches[$acc['name']] = $acc['name'] . ' ' . $name_user_account;
      $items++;
    }
    if ($items > 10) {
      break;
    }
  }
  // Return for JS.
  drupal_json_output($matches);
}
/**
 * Validate multiple transactions form.
 */
function ces_bank_multipletransactions_form_validate($form, &$form_state) {
  if (!empty($form_state['values']['page']) && $form_state['values']['page'] == 'confirm') {
    return;
  }
  $bank = new CesBank();
  $record = $form_state['values'];
  $account1 = $bank->getAccountByName($record['toaccountname']);
  if ($account1 === FALSE) {
    form_set_error('toaccountname', t('Seller account %acc is incorrect.', array('%acc' => $account1)));
  }
  $numbertrans = $record['howmany_transactions_real'];
  for ($i = 1; $i <= $numbertrans; $i++) {
    $amount = $record['amount-' . $i];
    $account2 = $record['fromaccountname-' . $i];
    $concept = trim($form_state['values']['concept-' . $i]);
    if (!($amount == '' && $account2 == '' && $concept == '')) {
      if (!is_numeric($amount)) {
        form_set_error('amount-' . $i, t('The amount entered is not a numeric value. Use the dot "." character for the decimal point and do not use any thousands separator.'));
        if (!($amount > 0.0)) {
          form_set_error('amount-' . $i, t('The amount must be positive.'));
        }
      }
      $account2 = $bank->getAccountByName($account2);
      if ($account2 === FALSE) {
        form_set_error('fromaccountname-' . $i, t('Buyer account %acc is incorrect.', array('%acc' => $account2)));
      }
      if ($account1['id'] == $account2['id']) {
        form_set_error('fromaccountname-' . $i, t('Buyer account cannot be the same as seller account.'));
      }
      if (empty($concept)) {
        form_set_error('concept-' . $i, t('You must specify the concept of this transaction.'));
      }
      $transaction = array(
        'toaccount' => $account1['id'],
        'fromaccount' => $account2['id'],
        'amount' => $amount,
        'concept' => $concept,
        'user' => $form_state['values']['user'],
      );
      if (!ces_transaction_access('use', $transaction)) {
        form_set_error('toaccountname', t("You don't have permission to apply this transaction."));
      }
    }
  }
}
/**
 * Multiple transactions submit.
 */
function ces_bank_multipletransactions_form_submit($form, &$form_state) {
  $form_state['values']['page'] = 'confirm';
  $form_state['rebuild'] = TRUE;
}
/**
 * Confirm multiple transactions form.
 */
function ces_bank_multipletransactions_confirm_form($form, &$form_state) {
  $values = $form_state['values'];
  // Copy values from previous form in a safe place.
  $form_state['edit_page'] = $values;
  $toexchange = ces_bank_get_exchange($values['toexchange']);
  $whichuser = ces_user_get_name(user_load($values['user']));
  if (isset($values['id'])) {
    $form['id'] = array(
      '#type' => 'value',
      '#value' => $values['id'],
    );
  }
  $bank = new CesBank();
  // This value is used in ces_bank_multipletransactions_form().
  $form['page'] = array(
    '#type' => 'value',
    '#value' => 'confirm',
  );
  $form['itemtoexchange'] = array(
    '#type' => 'item',
    '#title' => t('Exchange'),
    '#markup' => $toexchange['name'],
  );
  $form['itemtoaccountname'] = array(
    '#type' => 'item',
    '#title' => t('Seller account and user'),
    '#markup' => $values['toaccountname'] . ' - ' . $whichuser,
  );
  $form['transaction_fieldset'] = array(
    '#prefix' => '<table class="ces-table" style="margin:20px 0;"><thead><tr><th></th><th>' . t('Account') . '</th><th>' . t('Concept') . '</th><th>' . $toexchange['currenciesname'] . '</th></tr></thead><tbody>',
    '#suffix' => '</tbody></table>',
    '#type' => 'fieldset',
    '#attributes' => array('class' => array('element-invisible')),
  );
  $numbertrans = $values['howmany_transactions'];
  $j = 1;
  for ($i = 1; $i <= $numbertrans; $i++) {
    if (!empty($values['amount-' . $i]) && !empty($values['fromaccountname-' . $i]) && !empty($values['concept-' . $i])) {
      $amount = $values['amount-' . $i];
      $account2 = $values['fromaccountname-' . $i];
      $account2data = $bank->getAccountByName($account2);
      $name_user_account = '';
      foreach ($account2data['users'] as $u) {
        $name_user_account .= ces_user_get_name(user_load($u['user'])) . ', ';
      }
      $name_user_account = substr($name_user_account, 0, -2);
      if (strlen($name_user_account) > 30) {
        $name_user_account = substr($name_user_account, 0, 27) . '...';
      }
      $concept = trim($values['concept-' . $i]);
      $form['transaction_fieldset']['itemfromaccountname-' . $j] = array(
        '#type' => 'item',
        '#prefix' => '<tr style="border-top:1px dotted #ccc;"><td style="padding: 5px 0;">' . $i . '</td><td>',
        '#suffix' => '</td>',
        '#markup' => $account2 . ': ' . $name_user_account,
      );
      $form['transaction_fieldset']['itemconcept-' . $j] = array(
        '#type' => 'item',
        '#prefix' => '<td>',
        '#suffix' => '</td>',
        '#markup' => $concept,
      );
      $form['transaction_fieldset']['itemamount-' . $j] = array(
        '#type' => 'item',
        '#prefix' => '<td>',
        '#suffix' => '</td></tr>',
        '#markup' => $amount,
      );
      $j++;
    }
  }
  $form['back'] = array(
    '#type' => 'submit',
    '#value' => t('Back'),
    '#submit' => array('ces_bank_multipletransactions_confirm_form_back'),
    '#limit_validation_errors' => array(),
  );
  $form['submit'] = array(
    '#type' => 'submit',
    '#submit' => array('ces_bank_multipletransactions_confirm_form_submit'),
  );
  $form['submit']['#value'] = t('Confirm all transactions');
  return $form;
}
/**
 * Back action in confirm multiple transactions form.
 */
function ces_bank_multipletransactions_confirm_form_back($form, &$form_state) {
  if (isset($form_state['edit_page'])) {
    $form_state['values'] = $form_state['edit_page'];
    $form_state['values']['page'] = 'edit';
    $form_state['rebuild'] = TRUE;
  }
  else {
    drupal_goto('ces/bank');
  }
}
/**
 * Submit confirmed multiple transactions form.
 */
function ces_bank_multipletransactions_confirm_form_submit($form, &$form_state) {
  $bank = new CesBank();
  $result = TRUE;
  $id = '';
  $values = $form_state['edit_page'];
  $numbertrans = $values['howmany_transactions'];
  for ($i = 1; $i <= $numbertrans; $i++) {
    $amount = $values['amount-' . $i];
    $account2 = $values['fromaccountname-' . $i];
    $concept = trim($values['concept-' . $i]);
    if (($amount != '' && $account2 != '' && $concept != '')) {
      $record = array();
      $record['toaccountname'] = $values['toaccountname'];
      $record['fromaccountname'] = $account2;
      $record['concept'] = $concept;
      $record['amount'] = $amount;
      $record['user'] = $values['user'];
      $op = $values['op'];
      try {
        $bank->createTransaction($record);
        $id = $record['id'];
        $result = $bank->applyTransaction($id);
        $record = $bank->getTransaction($record['id']);
        _ces_bank_set_transaction_message($result, $record, $op);
      }
      catch (Exception $e) {
        drupal_set_message($e->getMessage(), 'error');
        return;
      }
    }
  }
  drupal_set_message(t('All transactions have been properly registered. For more information, see') . ' '
   . l(t('your account statement'), 'ces/bank/account/transaction') . '.');
  $form_state['redirect'] = 'ces/bank/account/sell';
}
/**
 * Form to delete a transaction permanently.
 */
function ces_bank_transaction_delete_form($form, &$form_state, $transaction) {
  $form = array();
  $form['transaction'] = array('#type' => 'value', '#value' => $transaction);

  $message = '<p>' . t('Are you sure you want to delete the transaction?') . '</p>';
  $caption = '<p>' . t('Transaction details:') . '</p>';
  require_once drupal_get_path('module', 'ces_bank') . '/ces_bank.pages.inc';
  $view = ces_bank_transaction_view($transaction);
  $caption .= '<p>' . drupal_render($view) . '</p>';
  $caption .= '<p>' . t('This action cannot be undone.') . '</p>';
  return confirm_form($form, filter_xss($message), 'ces/bank/account/transaction', filter_xss($caption), t('Delete'));
}
/**
 * Submit to delete a transaction permanently.
 */
function ces_bank_transaction_delete_form_submit($form, &$form_state) {
  $bank = new CesBank();
  $transaction = $form_state['values']['transaction'];
  $bank->deleteTransaction($transaction['id']);
  drupal_set_message(t('Transaction %concept has been deleted.', array('%concept' => $transaction['concept'])));
  $form_state['redirect'] = 'ces/bank/account/transaction';
}

/**
 * CesBank account statement form.
 *
 * @return array
 *   page where to find the last transactions with this account.
 * @see ces_bank_account_statement_form_submit()
 * @see ces_bank_account_statement_page()
 */
function ces_bank_account_statement_form($form, &$form_state) {
  $form = array(
    '#method' => 'get',
    '#token' => FALSE,
  );
  // Filter controls.
  $form['controls'] = array(
    '#type' => 'container',
    '#attributes' => array(),
  );
  if (isset($form_state['values']['from']) && isset($form_state['values']['to'])) {
    $from = $form_state['values']['from'];
    $to = $form_state['values']['to'];
  }
  else {
    $lastmonth = mktime(0, 0, 0, date('m') - 1, date('d'), date('Y'));
    $from = array(
      'year' => date('Y', $lastmonth),
      'month' => date('n', $lastmonth),
      'day' => date('d', $lastmonth),
    );
    $to = array(
      'year' => date('Y'),
      'month' => date('n'),
      'day' => date('d'),
    );
  }
  $form['controls']['from'] = array(
    '#type' => 'date',
    '#default_value' => $from,
    '#title' => t('From'),
  );
  $form['controls']['to'] = array(
    '#type' => 'date',
    '#default_value' => $to,
    '#title' => t('To'),
  );
  $form['controls']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Filter'),
  );
  return $form;
}
/**
 * CesBank account statement form submit.
 */
function ces_bank_account_statement_form_submit($form, &$form_state) {
  $form_state['rebuild'] = TRUE;
}
/**
 * CesBank permission form.
 */
function ces_bank_permission_form($form, &$form_state, $op, $id = NULL) {
  $form = array();
  if ($op != 'new') {
    $bank = new CesBank();
    $values = $bank->getPermission($id);
  }
  else {
    $values = array(
      'permission' => '',
      'object' => '',
      'objectid' => 0,
      'scope' => '',
      'scopeid' => 0,
    );
  }
  $form['operation'] = array(
    '#type' => 'value',
    '#value' => $op,
  );
  if ($op != 'new') {
    $form['id'] = array(
      '#type' => 'value',
      '#value' => $id,
    );
  }
  $form['permission'] = array(
    '#title' => t('Permission type'),
    '#type' => 'select',
    '#options' => array(
      10 => t('View'),
      20 => t('Use'),
      30 => t('Edit'),
      40 => t('Admin'),
    ),
    '#default_value' => $values['permission'],
    '#required' => TRUE,
  );
  $form['object'] = array(
    '#type' => 'select',
    '#title' => t('Object type'),
    '#description' => t('The type of the piece of data on which the permission is set.'),
    // @TODO: move this array to the bank core file.
    '#options' => array(
      'global' => t('Global'),
      'global statistics' => t('Global statistics'),
      'global exchangecreator' => t('Global exchange creation'),
      'exchange' => t('Exchange'),
      'exchange details' => t('Exchange configuration details'),
      'exchange accountcreator' => t('Exchange account creation'),
      'account' => t('Account'),
      'account details' => t('Account details'),
      'account transactions' => t('Account transactions'),
      'account seller' => t('Account seller'),
      'account buyer' => t('Account buyer'),
    ),
    '#default_value' => $values['object'],
    '#required' => TRUE,
  );
  // @TODO: use ajax to provide an usable interface instead of the following
  // element.
  $form['objectid'] = array(
    '#type' => 'textfield',
    '#title' => t('Object identifier'),
    '#default_value' => $values['objectid'],
    '#required' => TRUE,
  );
  $form['scope'] = array(
    '#type' => 'select',
    '#title' => t('Scope type'),
    '#description' => t('The group of users who have this permission.'),
    '#options' => array(
      'user' => t('User'),
      'account' => t('Account'),
      'exchange' => t('Exchange'),
      'global' => t('Global'),
    ),
    '#default_value' => $values['scope'],
    '#required' => TRUE,
  );
  // @TODO: use ajax to provide an usable interface instead of the following
  // element.
  $form['scopeid'] = array(
    '#type' => 'textfield',
    '#title' => t('Scope identifier'),
    '#default_value' => $values['scopeid'],
    '#required' => TRUE,
  );
  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => $op == 'new' ? t('Create permission') : t('Update permission'),
  );
  return $form;
}
/**
 * Check if object exists to host a permission.
 */
function _ces_bank_permission_check_object_exists($object, $objectid) {
  $space = strpos($object, ' ');
  if ($space > 0) {
    $object = substr($object, 0, $space);
  }
  switch ($object) {
    case 'global':
      return TRUE;

    case 'user':
      return user_load($objectid) !== FALSE;

    case 'exchange':
      $bank = new CesBank();
      return $bank->getExchange($objectid) !== FALSE;

    case 'account':
      $bank = new CesBank();
      return $bank->getAccount($objectid) !== FALSE;

  }
  return FALSE;
}
/**
 * Validation for bank permission form.
 */
function ces_bank_permission_form_validate($form, &$form_state) {
  $bank = new CesBank();
  $values = $form_state['values'];
  if (!$bank->access('admin', $values['object'], $values['objectid'])) {
    form_set_error('object', t('You are not authorized to modify permissions on this object.'));
  }
  if (!_ces_bank_permission_check_object_exists($values['object'], $values['objectid'])) {
    form_set_error('object', t('Object with given id does not exist.'));
  }
  if (!_ces_bank_permission_check_object_exists($values['scope'], $values['scopeid'])) {
    form_set_error('scope', t('Scope with given id does not exist.'));
  }
}
/**
 * Submit bank permission form.
 */
function ces_bank_permission_form_submit($form, &$form_state) {
  $bank = new CesBank();
  $op = $form_state['values']['operation'];
  try {
    if ($op == 'new') {
      $bank->createPermission($form_state['values']);
      drupal_set_message(t('Permission successfully created.'));
    }
    elseif ($op == 'edit') {
      $bank->updatePermission($form_state['values']);
      drupal_set_message(t('Permission successfully updated.'));
    }
  }
  catch (Exception $e) {
    drupal_set_message(t('An error occurred while saving the record. Details: %msg', array('msg' => $e->getMessage())), 'error');
  }
}
/** @} */

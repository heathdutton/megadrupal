<?php

/**
 * @file
 * Admin settings page for the coins wallet module.
 *
 * @author ssm2017 Binder
 * inspired by a module made by m0Ray.
 *
 * @license GNU/GPL, http://www.gnu.org/licenses/gpl-2.0.html
 * Coins is free software. This version may have been modified pursuant
 * to the GNU General Public License, and as distributed it includes or
 * is derivative of works licensed under the GNU General Public License or
 * other free or open source software licenses.
 */

/**
 * Admin page form
 */
function coins_wallet_admin_form($form, &$form_state) {
  // coins server config
  $form['server'] = array(
    '#type' => 'fieldset',
    '#title' => t('Server config'),
    '#description' => t('Set the server config.')
  );
  $form['server']['coins_wallet_server_scheme'] = array(
    '#type' => 'textfield',
    '#title' => t('Scheme'),
    '#default_value' => variable_get('coins_wallet_server_scheme', COINS_WALLET_DEFAULT_SERVER_SCHEME),
    '#description' => t('Enter the server scheme.'),
    '#maxlength'=> 5,
    '#size'=> 5,
  );
  $form['server']['coins_wallet_server_host'] = array(
    '#type' => 'textfield',
    '#title' => t('Host'),
    '#default_value' => variable_get('coins_wallet_server_host', COINS_WALLET_DEFAULT_SERVER_HOST),
    '#description' => t('Enter the server host.')
  );
  $form['server']['coins_wallet_server_port'] = array(
    '#type' => 'textfield',
    '#title' => t('Port'),
    '#default_value' => variable_get('coins_wallet_server_port', COINS_WALLET_DEFAULT_SERVER_PORT),
    '#description' => t('Enter the server port.'),
    '#maxlength'=> 5,
    '#size'=> 5,
  );
  $form['server']['coins_wallet_server_username'] = array(
    '#type' => 'textfield',
    '#title' => t('Username'),
    '#default_value' => variable_get('coins_wallet_server_username', COINS_WALLET_DEFAULT_SERVER_USERNAME),
    '#description' => t('Enter the server username.')
  );
  $form['server']['coins_wallet_server_password'] = array(
    '#type' => 'textfield',
    '#title' => t('Password'),
    '#default_value' => variable_get('coins_wallet_server_password', COINS_WALLET_DEFAULT_SERVER_PASSWORD),
    '#description' => t('Enter the server password.')
  );

  // accounts config
  $form['accounts'] = array(
    '#type' => 'fieldset',
    '#title' => t('Accounts config'),
    '#description' => t('Set the accounts config.')
  );
  $form['accounts']['coins_wallet_enable_internal_payments'] = array(
    '#type' => 'checkbox',
    '#title' => t('Enable internal payments'),
    '#default_value' => variable_get('coins_wallet_enable_internal_payments', COINS_WALLET_DEFAULT_ENABLE_INTERNAL),
    '#description' => t('The internal payments are not working in some cryptochains networks.<br/><strong>WARNING!!! Enable this option only if you know what you are doing because people can lose money with that</strong>')
  );
  $form['accounts']['coins_wallet_account_prefix'] = array(
    '#type' => 'textfield',
    '#required' => TRUE,
    '#default_value' => variable_get('coins_wallet_account_prefix', COINS_WALLET_DEFAULT_ACCOUNT_PREFIX),
    '#title' => t('Account prefix'),
    '#description' => t('Devcoind/Bitcoind accounts will be created and accessed by name consisting of this prefix and numeric Drupal user id. <br/><strong>WARNING!!! Do not change it after any account is created, or you definitely lose access to your accounts.</strong>'),
  );
  $form['accounts']['coins_wallet_max_addresses'] = array(
    '#type' => 'textfield',
    '#required' => TRUE,
    '#default_value' => variable_get('coins_wallet_max_addresses', COINS_WALLET_DEFAULT_MAX_ADDRESSES),
    '#title' => t('Maximum addresses'),
    '#description' => t('Maximum coins addresses per account. Only applicable to users without "coins unlimited addresses" credential.'),
  );
  $form['accounts']['coins_wallet_max_transactions'] = array(
    '#type' => 'textfield',
    '#required' => TRUE,
    '#default_value' => variable_get('coins_wallet_max_transactions', COINS_WALLET_DEFAULT_MAX_TRANSACTIONS),
    '#title' => t('Maximum transactions'),
    '#description' => t('How many last transactions to display on log page.'),
  );

  // block explorer
  $form['block_explorer'] = array(
    '#type' => 'fieldset',
    '#title' => t('Block explorer'),
  );
  $block_explorers_list = t('Here are some famous blockexplorers.').
  '<ul>
    <li>Devcoin</li>
      <ul>
        <li>http://www.devda.ch:2750/tx</li>
        <li>http://darkgamex.ch:2751/tx</li>
      </ul>
    <li>Bitcoin</li>
      <ul>
        <li>http://blockchain.info/tx</li>
        <li>http://blockexplorer.com/block</li>
      </ul>
  </ul>';
  $form['block_explorer']['coins_wallet_block_explorer_link'] = array(
    '#type' => 'textfield',
    '#required' => TRUE,
    '#default_value' => variable_get('coins_wallet_block_explorer_link', ''),
    '#title' => t('Block explorer link'),
    '#description' => t('Enter a link to your prefeared block explorer to use it as a link on the transferts page.<br/>Do not add the trailing slash.'). ' <p>'. $block_explorers_list. '</p>',
  );

  return system_settings_form($form);
}

/**
 * Admin page form validation handler
 */
function coins_wallet_admin_form_validate($form, &$form_state) {
  if (intval($form_state['values']['coins_wallet_max_addresses']) <= 0) {
    form_set_error('coins_wallet_max_addresses', t('Must be a positive value.'));
  }
  if (intval($form_state['values']['coins_wallet_max_transactions']) <= 0) {
    form_set_error('coins_wallet_max_transactions', t('Must be a positive value.'));
  }
  // check if the server is responding
  $params = array(
    'scheme' => $form_state['values']['coins_wallet_server_scheme'],
    'username' => $form_state['values']['coins_wallet_server_username'],
    'password' => $form_state['values']['coins_wallet_server_password'],
    'host' => $form_state['values']['coins_wallet_server_host'],
    'port' => $form_state['values']['coins_wallet_server_port'],
    'key_file' => ''
  );
  $server =  _coins_wallet_get_server($params);
  if (is_null($server)) {
    form_set_error('', t('Unable to access the coins server.'));
  }
  else {
    try {
      $server->can_connect();
    }
    catch (BitcoinClientException $e) {
      form_set_error('', t('Unable to access the coins server. Fault: !fault', array('!fault' => $e)));
    }
  }
}

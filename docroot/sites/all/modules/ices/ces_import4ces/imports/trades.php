<?php
/**
 * @file
 * Functions from parse trades
 */

/**
 * @defgroup ces_import4ces_trades Parse trades from CES
 * @ingroup ces_import4ces
 * @{
 * Functions from parse trades
 */

/**
 * Parse trades.
 */
function ces_import4ces_parse_trades($import_id, $data, $row, &$context, $width_ajax = TRUE) {
  global $user;
  if (isset($context['results']['error'])) {
    return;
  }
  $tx = db_transaction();
  try {
    ob_start();
    $context['results']['import_id'] = $import_id;
    $bank = new CesBank();
    $account_seller = _ces_import4ces_trades_get_account($import_id, $data['Seller'], $data);
    if ($account_seller === FALSE) {
      throw new Exception(t('Acount @account not found.', array('@account' => $data['Seller'])));
    }
    $account_buyer = _ces_import4ces_trades_get_account($import_id, $data['Buyer'], $data);
    if ($account_buyer === FALSE) {
      throw new Exception(t('Acount @account not found.', array('@account' => $data['Buyer'])));
    }
    // Find uid from user.
    $query = db_query('SELECT uid FROM {users} where name=:name', array(':name' => $data['EnteredBy']));
    $trade_user_id = $query->fetchColumn(0);
    if (!$trade_user_id) {
      $trade_user_id = $user->uid;
    }

    $extra_info = array(
      'ID' => $data['ID'],
      'RemoteExchange' => $data['RemoteExchange'],
      'RemoteBuyer' => $data['RemoteBuyer'],
      'RecordID' => $data['RecordID'],
      'Levy' => $data['Levy'],
      'LevyRate' => $data['LevyRate'],
    );

    $trans = array(
      'fromaccountname' => $account_buyer['name'],
      'toaccountname' => $account_seller['name'],
      'amount' => $data['Amount'],
      'concept' => $data['Description'],
      'user' => $trade_user_id,
      'created' => strtotime($data['DateEntered']),
      'modified' => strtotime($data['DateEntered']),
    );
    variable_set('ces_import4ces_mail', FALSE);
    $bank->createTransaction($trans);
    $bank->applyTransaction($trans['id']);
    variable_set('ces_import4ces_mail', CES_IMPORT4CES_SEND_MAILS);

    db_insert('ces_import4ces_objects')
      ->fields(array(
        'import_id' => $import_id,
        'object' => 'trades',
        'object_id' => $trans['id'],
        'row' => $row,
        'data' => serialize($extra_info),
      ))->execute();
    ces_import4ces_update_row($import_id, $row);
    ob_end_clean();
  }
  catch (Exception $e) {
    ob_end_clean();
    $tx->rollback();
    $context['results']['error'] = check_plain($e->getMessage());
    $_SESSION['ces_import4ces_row_error']['row']  = $row;
    $_SESSION['ces_import4ces_row_error']['m']    = $e->getMessage();
    $_SESSION['ces_import4ces_row_error']['data'] = $data;
    if ($width_ajax) {
      $result = array('status' => FALSE, 'data' => check_plain($e->getMessage()));
      die(json_encode($result));
    }
    else {
      ces_import4ces_batch_fail_row($import_id, array_keys($data), array_values($data), $row, $context);
    }
  }
}

/**
 * Get accounts.
 * 
 * Get an account record for this trade. If the trade is 
 * inter-exchange, it returns the proper interexchange 
 * virtual account, creating it if necessary.
 * 
 * @param int $import_id
 *   The id of this import.
 * @param string $name
 *   The name of the account to be retrieved.
 * @param array $data
 *   The full import line.
 * 
 * @return array
 *   The accout record or FALSE if it donesn't exist.
 */
function _ces_import4ces_trades_get_account($import_id, $name, $data) {
  $bank = new CesBank();
  if (substr($name, 4) == 'VIRT') {
    // This is an inter-exchange transaction. Use the corresponding virtual
    // account.
    if ($data['RemoteExchange'] != '0000') {
      $name = substr($name, 0, 4) . $data['RemoteExchange'];
    }
    else {
      // This is a remote transaction. The cen ID appears to be in the RecordID.
      $offset = ($name == $data['Seller']) ? 0 : 4;
      $name = substr($name, 0, 4) . 'cen' . substr($data['RecordID'], $offset, 4);
    }
    $account_seller = $bank->getAccountByName($name);
    if ($account_seller === FALSE) {
      // The virtual account does not exist yet, so let's create it.
      $import = ces_import4ces_import_load($import_id);
      $exchange = $bank->getExchange($import->exchange_id);
      $account_seller = array(
        'id' => NULL,
        'exchange' => $exchange['id'],
        'name' => $name,
        'balance' => 0.0,
        'state' => CesBankLocalAccount::STATE_HIDDEN,
        'kind' => CesBankLocalAccount::TYPE_VIRTUAL,
        'limitchain' => $exchange['limitchain'],
        'users' => array(
          array(
            'account' => NULL,
            'user' => 1,
            'role' => CesBankAccountUser::ROLE_ACCOUNT_ADMINISTRATOR,
          ),
        ),
      );
      $bank->createAccount($account_seller);
      $bank->activateAccount($account_seller);
    }
  }
  else {
    $account_seller = $bank->getAccountByName($name);
  }
  return $account_seller;
}
/** @} */

<?php
/**
 * @file
 * All page callback functions for bank module.
 */

/**
 * @defgroup ces_bank_pages Pages from Ces Bank
 * @ingroup ces_bank
 * @{
 * All page callback functions for bank module.
 */

 /**
  * TODO: document.
  */
function ces_bank_exchange_admin() {
  $bank = new CesBank();
  $exchange = ces_bank_get_current_exchange();
  $page = array();
  // New accounts request.
  $accounts = $bank->getAllAccounts(array('state' => 0, 'exchange' => $exchange['id']));
  if (!empty($accounts)) {
    $page['new_accounts'] = array(
      'type' => 'fieldset',
      'title' => t('New accounts'),
      'description' => t('List of new account requests for this exchange. Activate or delete all them.'),
    );
    $list = array(
      '#theme' => 'item_list',
      '#items' => array(),
    );
    foreach ($accounts as $account) {
      $list['#items'][] = l($account['name'], 'ces/bank/exchange/' . $exchange['id'] . '/account/' . $account['id'] . '/activate');
    }
    $page['new_accounts']['new_accounts_list'] = $list;
  }
  else {
    $page['no_new_accounts'] = array(
      '#type' => 'item',
      '#title' => t('No requests for new accounts.'),
    );
  }
  $page['limitchains'] = array(
    '#markup' => l(t('Administer limit chains'), 'ces/bank/exchange/' . $exchange['id'] . '/limitchain'),
  );
  return $page;
}
/**
 * TODO: document.
 */
function ces_bank_accounts_admin($exchange_id) {
  $bank = new CesBank();
  if (!isset($exchange_id) || empty($exchange_id) || $exchange_id == 'ces_current_exchange') {
    $exchange = ces_bank_get_current_exchange();
  }
  else {
    $exchange = $bank->getExchange($exchange_id);
  }
  if ($exchange === FALSE) {
    return array(
      'message' => array(
        '#markup' => '<p>' . t('Invalid current exchange') . '</p>',
      ),
    );
  }
  $page = array();
  $page['table'] = array(
    '#theme' => 'table',
    '#attributes' => array(
      'class' => array('ces-table'),
    ),
    '#header' => array(
      array(
        'data' => t('Account'),
        'sort' => 'asc',
        'field' => 'name',
      ),
      array(
        'data' => t('Type'),
        'sort' => 'asc',
        'field' => 'kind',
      ),
      array(
        'data' => t('State'),
        'sort' => 'asc',
        'field' => 'state',
      ),
      array(
        'data' => t('Limit'),
        'sort' => 'asc',
        'field' => 'limitchain',
      ),
      array(
        'data' => t('Balance'),
        'sort' => 'asc',
        'field' => 'balance',
      ),
      array(
        'data' => t('Actions'),
      ),
    ),
    '#rows' => array(),
  );
  $accounts = $bank->getAllAccounts(array('exchange' => $exchange['id']));
  $kinds = array(
    0 => t('Individual'),
    1 => t('Shared'),
    2 => t('Organization'),
    3 => t('Company'),
    4 => t('Public'),
    5 => t('Virtual'),
  );
  $states = array(
    0 => t('Hidden'),
    1 => t('Active'),
    2 => t('Closed'),
    3 => t('Locked'),
  );
  $limitchains = $bank->getAllLimitChains($exchange['id']);
  $limits = array();
  foreach ($limitchains as $id => $limit) {
    $limits[$id] = $limit['name'];
  }
  $rows = array();
  foreach ($accounts as $account) {
    $row = array(
      $account['name'],
      $kinds[$account['kind']],
      $states[$account['state']],
      $limits[$account['limitchain']],
      // Quitamos moneda de la cantidad para poder ser ordenado.
      // $bank->formatAmount($account['balance'], $exchange, TRUE),
      number_format($account['balance'], 2),
      l(t('View'), 'ces/admin/account/' . $account['id'] . '/view') . ' ' .
      l(t('Edit'), 'ces/admin/account/' . $account['id'] . '/edit') . ' ' .
      l(t('Delete'), 'ces/admin/account/' . $account['id'] . '/delete'),
    );
    $rows[] = $row;
  }
  _ces_bank_sort_table($page['table']['#header'], $rows);
  $page['table']['#rows'] = $rows;
  return $page;
}
/**
 * Page with the list of all exchanges organized by country.
 */
function ces_bank_exchange_list_page($country) {
  $exchanges = ces_bank_get_exchanges($country);
  $page = array();
  if (empty($exchanges)) {
    require_once DRUPAL_ROOT . '/includes/locale.inc';
    $countries = country_get_list();
    $page['text'] = array(
      '#type' => 'markup',
      '#value' => t('There are no active exchanges in %country.', array('%country' => $countries[$country])),
    );
  }
  else {
    $page['table'] = array(
      '#theme' => 'table',
      '#attributes' => array(
        'class' => array('ces-table'),
      ),
      '#header' => array(
        array(
          'data' => t('Code'),
          'field' => 'code',
          'sort' => 'asc',
        ),
        array(
          'data' => t('Name'),
          'field' => 'name',
          'sort' => 'asc',
        ),
        array(
          'data' => t('Region'),
          'field' => 'region',
          'sort' => 'asc',
        ),
        array(
          'data' => t('Town'),
          'field' => 'town',
          'sort' => 'asc',
        ),
        array(
          'data' => t('Accounts')  ,
          'field' => 'accounts',
          'sort' => 'asc',
        ),
        array(
          'data' => t('Currency'),
          'field' => 'currencysymbol',
          'sort' => 'asc',
        ),
      ),
      '#rows' => array(),
    );
    foreach ($exchanges as $exchange) {
      $row = array(
        $exchange['code'],
        l($exchange['name'], 'user/register/' . $exchange['code'], array('title' => t('Click to open an account in this exchange'))),
        $exchange['region'],
        $exchange['town'],
        $exchange['accounts'],
        $exchange['currencysymbol'],
      );
      $page['table']['#rows'][] = $row;
    }
  }
  return $page;
}
/**
 * Page with a table of all accounts with the user name and balance.
 */
function ces_bank_exchange_accounts_page() {
  $bank = new CesBank();
  $exchange = ces_bank_get_current_exchange();
  $accounts = $bank->getAllAccounts(array(
    'exchange' => $exchange['id'],
    'state' => 1,
    'kind' => array(0, 1, 2, 3, 4),
  ));
  $page = array();
  $page['table'] = array(
    '#theme' => 'table',
    '#attributes' => array(
      'class' => array('ces-table'),
    ),
    '#header' => array(
      array(
        'data' => t('User'),
        'field' => 'user',
        'sort' => 'asc',
      ),
      array(
        'data' => t('Account'),
        'field' => 'account',
        'sort' => 'asc',
      ),
      array(
        'data' => t('Balance'),
        'field' => 'balance',
        'sort' => 'asc',
      ),
    ),
  );
  // Load all users with a single database select.
  $uids = array();
  foreach ($accounts as $account) {
    foreach ($account['users'] as $user) {
      $uids[] = $user['user'];
    }
  }
  $uids = array_unique($uids);
  $users = user_load_multiple($uids);
  // Fill table rows.
  $rows = array();
  foreach ($accounts as $account) {
    $accountsperuser = $account['name'];
    $users_out = '';
    $count = 0;
    foreach ($account['users'] as $user) {
      $count++;
      if (isset($users[$user['user']])) {
        $user = $users[$user['user']];
        $uo = ces_bank_get_full_username($user);
      }
      else {
        $user = FALSE;
        $uo = 'Nonexistent user';
      }
      if ($count > 1) {
        $users_out .= '<br/>';
      }
      // First cell begins with 8 chars of the name user in capital letters
      // only for sort case insensitive proposals.
      if ($user) {
        if ( $count == 1 ) {
          $users_out .= strtoupper(sprintf('%8.8s', $uo));
        } 
        $users_out .= '<div class="ces-user-column"> <div class="user-picture">' .
        theme('user_picture', array('account' => $user)) . '</div>' .
        l(ucfirst($uo), 'user/' . $user->uid) .
        '</div>';
      }
      else {
        if ( $count == 1 ) {
          $users_out .= strtoupper(sprintf('%8.8s', $uo));
        } 
        $users_out .= '<div class="ces-user-column">' .
        $uo .
        '</div>';
      }
    }
    $row = array(
      $users_out,
      $accountsperuser,
      $account['balance'],
    );
    $rows[] = $row;
  }
  _ces_bank_sort_table($page['table']['#header'], $rows);
  // Format amounts after sort the table.
  $lenghtrows = count($rows);
  for ($i = 0; $i < $lenghtrows; $i++) {
    $rows[$i][0] = substr($rows[$i][0], 8);
    $rows[$i][2] = $bank->formatAmount($rows[$i][2], $exchange, TRUE);
  }
  $page['table']['#rows'] = $rows;
  return $page;
}
/**
 * Balances with other exchanges and currency exchange rates.
 */
function ces_bank_other_exchanges_page() {
  $bank = new CesBank();
  $exchange = ces_bank_get_current_exchange();
  // Get virtual accounts.
  $accounts = $bank->getAllAccounts(array(
    'exchange' => $exchange['id'],
    'state' => 1,
    'kind' => 5,
  ));
  $page = array();
  $page['table'] = array(
    '#theme' => 'table',
    '#attributes' => array(
      'class' => array('ces-table'),
    ),
    '#header' => array(
      array(
        'data' => t('Exchange'),
        'field' => 'exchange',
        'sort' => 'asc',
      ),
      array(
        'data' => t('Currency'),
      ),
      array(
        'data' => t('Imports'),
        'field' => 'import',
        'sort' => 'asc',
      ),
      array(
        'data' => t('Exports'),
        'field' => 'export',
        'sort' => 'asc',
      ),
      array(
        'data' => t('Balance'),
        'field' => 'balance',
        'sort' => 'asc',
      ),
    ),
  );
  $rows = array();
  $imports = 0;
  $exports = 0;
  foreach ($accounts as $account) {
    $import = $bank->getAccountHistoricSales($account['id'], 0);
    $export = $bank->getAccountHistoricPurchases($account['id'], 0);
    if ($import != 0 || $export != 0) {
      $row = array();
      $code = drupal_substr($account['name'], 4);
      $exc = $bank->getExchangeByName($code);
      if ($exc !== FALSE) {
        // Exchange name.
        $row[0] = $code . ' - ' . $exc['name'];
        // Currency value.
        if ($exc['currencyvalue'] > $exchange['currencyvalue']) {
          $valuea = 1;
          $valueb = $exc['currencyvalue'] / $exchange['currencyvalue'];
        }
        else {
          $valuea = $exchange['currencyvalue'] / $exc['currencyvalue'];
          $valueb = 1;
        }
        $row[1] = $exc['currenciesname'] . ' (' . $bank->formatAmount($valuea, $exc, TRUE)
          . ' = ' . $bank->formatAmount($valueb, $exchange, TRUE) . ')';
      }
      else {
        // This a remote exchange. We don't know the name nor the currency
        // value.
        $row[0] = $code . ' (' . t('Remote') . ')';
        $row[1] = '';
      }
      $row[2] = $import;
      $imports += $import;
      $row[3] = $export;
      $exports += $export;
      // Balance.
      $row[4] = $export - $import;
      $rows[] = $row;
    }
  }
  _ces_bank_sort_table($page['table']['#header'], $rows);
  // Format amounts after sort the table.
  foreach ($rows as $i => $row) {
    $rows[$i][2] = $bank->formatAmount($row[2], $exchange, TRUE);
    $rows[$i][3] = $bank->formatAmount($row[3], $exchange, TRUE);
    $rows[$i][4] = $bank->formatAmount($row[4], $exchange, TRUE, TRUE);
  }
  $rows[] = array(
    t('TOTAL'),
    '',
    $bank->formatAmount($imports, $exchange, TRUE),
    $bank->formatAmount($exports, $exchange, TRUE),
    $bank->formatAmount($exports - $imports, $exchange, TRUE, TRUE),
  );
  $page['table']['#rows'] = $rows;
  return $page;
}
/**
 * TODO: document.
 */
function ces_bank_account_list_page() {
  global $user;
  $bank = new CesBank();
  $page = array();
  $accounts = $bank->getUserAccounts($user->uid);
  foreach ($accounts as $account) {
    $exchange = $bank->getExchange($account['exchange']);
    $accform = array(
      '#type' => 'fieldset',
      '#title' => t('Account') . ' ' . $account['name'],
    );
    $accform['exchange'] = array(
      '#type' => 'item',
      '#title' => t('Exchange'),
      '#markup' => $exchange['name'],
    );
    $accform['account'] = array(
      '#type' => 'item',
      '#title' => t('Account'),
      '#markup' => $account['name'],
    );
    $accform['balance'] = array(
      '#type' => 'item',
      '#title' => t('Balance'),
      '#markup' => $account['balance'],
    );
    $accform['statement'] = array(
      '#type' => 'item',
      '#markup' => l(t('Statement'), 'ces/bank/exchange/' . $exchange['id'] . '/account/' . $account['id'] . '/transaction'),
    );
    $accform['edit'] = array(
      '#type' => 'item',
      '#markup' => l(t('Edit'), 'ces/bank/exchange/' . $exchange['id'] . '/account/' . $account['id'] . '/edit'),
    );
    $accform['transaction'] = array(
      '#type' => 'item',
      '#markup' => l(t('Charge a sale'), 'ces/bank/exchange/' . $exchange['id'] . '/account/' . $account['id'] . '/transaction/sell'),
    );
    $page[$account['name']] = $accform;
  }
  return $page;
}
/**
 * TODO: document.
 */
function ces_bank_admin_page() {
  $bank = new CesBank();
  $page = array();
  $page['new_exchanges'] = array(
    '#type' => 'fieldset',
    '#title' => t('Exchange requests'),
  );
  $exchanges = $bank->getAllExchanges(array('state' => 0));
  if (!empty($exchanges)) {
    $list = array(
      '#theme' => 'item_list',
      '#items' => array(),
    );
    foreach ($exchanges as $exchange) {
      $list['#items'][] = l($exchange['code'] . ' - ' . $exchange['name'], 'ces/admin/exchange/' . $exchange['id'] . '/activate');
    }
    $page['new_exchanges']['new_exchanges_list'] = $list;
  }
  else {
    $page['new_exchanges']['text'] = array(
      '#type' => 'item',
      '#title' => t('No requests for new exchanges.'),
    );
  }
  $page['edit_exchanges'] = array(
    '#type' => 'fieldset',
    '#title' => t('Active exchanges'),
  );
  $page['edit_exchanges']['table'] = array(
    '#theme' => 'table',
    '#attributes' => array(
      'class' => array('ces-table'),
    ),
    '#header' => array(
      array(
        'data' => t('Exchange'),
        'sort' => 'asc',
        'field' => 'name',
      ),
      array(
        'data' => t('Actions'),
      ),
    ),
    '#rows' => array(),
  );
  $exchanges = $bank->getAllExchanges(array('state' => 1));
  if (!empty($exchanges)) {
    $rows = array();
    foreach ($exchanges as $exchange) {
      $row = array(
        $exchange['code'] . ' - ' . $exchange['name'],
        l(t('Edit'), 'ces/admin/ces/' . $exchange['id'] . '/edit') . ' ' .
        l(t('Delete'), 'ces/admin/ces/' . $exchange['id'] . '/delete') . ' -- ' .
        l(t('Accounts'), 'ces/admin/ces/' . $exchange['id'] . '/accounts') . ' ' .
        l(t('Transactions'), 'ces/admin/ces/' . $exchange['id'] . '/transactions'),
      );
      $rows[] = $row;
    }
    _ces_bank_sort_table($page['edit_exchanges']['table']['#header'], $rows);
    $page['edit_exchanges']['table']['#rows'] = $rows;
  }
  else {
    $page['edit_exchanges']['table']['#rows'] = array(array(t('No exchanges activated yet.'), ''));
  }
  return $page;
}
/**
 * TODO: Document.
 */
function ces_bank_permissions_page() {
  $bank = new CesBank();
  $page = array();
  $page['table'] = array(
    '#theme' => 'table',
    '#attributes' => array(
      'class' => array('ces-table'),
    ),
    '#header' => array(
      array(
        'data' => t('Permission type'),
        'field' => 'permission',
        'sort' => 'asc',
      ),
      array(
        'data' => t('Object type'),
        'field' => 'object',
        'sort' => 'asc',
      ),
      array(
        'data' => t('Object identifier'),
        'field' => 'objectid',
      ),
      array(
        'data' => t('Scope type'),
        'field' => 'scope',
        'sort' => 'asc',
      ),
      array(
        'data' => t('Scope identifier'),
        'field' => 'scopeid',
      ),
      array(
        'data' => t('Actions'),
      ),
    ),
    '#rows' => array(),
  );
  global $user;
  $perms = $bank->getAdministrablePermissions($user->uid);
  foreach ($perms as $perm) {
    $row = array(
      $perm['permission'],
      $perm['object'],
      $perm['objectid'],
      $perm['scope'],
      $perm['scopeid'],
    );
    $actions = array();
    if ($bank->access('admin', 'permission', $perm['id'])) {
      $actions[] = l(t('Edit'), 'ces/bank/permission/' . $perm['id'] . '/edit');
      $actions[] = l(t('Delete'), 'ces/bank/permission/' . $perm['id'] . '/delete');
    }
    $row[] = implode(' ', $actions);
    $page['table']['#rows'][] = $row;
  }
  return $page;
}
/**
 * List the limitchains for the given exchange.
 */
function ces_bank_limitchain_list_page() {
  $exchange = ces_bank_get_current_exchange();
  if ($exchange === FALSE) {
    return array(
      'message' => array(
        '#markup' => '<p>' . t('Invalid current exchange') . '</p>',
      ),
    );
  }
  $bank = new CesBank();
  $limitchains = $bank->getAllLimitChains($exchange['id']);
  $page = array();
  $page['table'] = array(
    '#theme' => 'table',
    '#attributes' => array(
      'class' => array('ces-table'),
    ),
    '#header' => array(
      array(
        'data' => t('Name'),
        'field' => 'name',
        'sort' => 'asc',
      ),
      array(
        'data' => t('Limits'),
      ),
      array(
        'data' => t('Accounts'),
      ),
      array(
        'data' => t('Actions'),
      ),
    ),
  );
  $limitnames = module_invoke_all('account_limit_classes');
  $rows = array();
  foreach ($limitchains as $limitchain) {
    $row = array();
    $row[] = $limitchain['name'];
    $limits = array();
    foreach ($limitchain['limits'] as $limit) {
      $limits[] = $limitnames[$limit['classname']] . ' ' . $bank->formatAmount($limit['value'], $exchange, TRUE) . ($limit['block'] ? ' ' . t('block') : '');
    }
    $row[] = implode('<br/>', $limits);
    $actions = array();
    if (ces_bank_access(CesBankPermission::PERMISSION_VIEW, 'exchange limitchains', CES_BANK_CURRENT_EXCHANGE)) {
      $actions[] = l(t('View'), 'ces/admin/limitchain/' . $limitchain['id'] . '/view');
    }
    if (ces_bank_access(CesBankPermission::PERMISSION_EDIT, 'exchange', CES_BANK_CURRENT_EXCHANGE)) {
      $actions[] = l(t('Edit'), 'ces/admin/limitchain/' . $limitchain['id'] . '/edit');
    }
    if (ces_bank_access(CesBankPermission::PERMISSION_ADMIN, 'exchange', CES_BANK_CURRENT_EXCHANGE)) {
      $actions[] = l(t('Delete'), 'ces/admin/limitchain/' . $limitchain['id'] . '/delete');
    }
    $accounts = $bank->getAllAccounts(array('exchange' => $exchange['id'], 'limitchain' => $limitchain['id']));
    $row[] = count($accounts);
    $row[] = implode(' ', $actions);
    $rows[] = $row;
  }
  _ces_bank_sort_table($page['table']['#header'], $rows);
  $page['table']['#rows'] = $rows;
  return $page;
}
/**
 * TODO: document.
 */
function ces_bank_limitchain_view($limitchain) {
  $bank = new CesBank();
  foreach ($limitchain['limits'] as $key => $value) {
    $limitchain['limits'][$key]['value'] = $bank->formatAmount($value['value'], ces_bank_get_current_exchange(), TRUE);
  }
  $page = array(
    '#theme' => 'ces_bank_limitchain_view',
    'limitchain' => $limitchain,
    'limitclasses' => module_invoke_all('account_limit_classes'),
  );
  return $page;
}
/**
 * TODO: document.
 */
function ces_bank_exchange_view() {
  $exchange = ces_bank_get_current_exchange();
  if ($exchange !== FALSE) {
    $page = array(
      'exchange' => $exchange,
      '#theme' => 'ces_bank_exchange_view',
    );
  }
  else {
    $page = array(
      'message' => array(
        '#markup' => '<p>' . t("You don't belong to any exchange yet.") .
        '</p>',
      ),
    );
  }

  return $page;
}
/**
 * TODO: document.
 */
function ces_bank_account_view($account) {
  if ($account == FALSE) {
    $account = ces_bank_get_current_account();
  }
  $page = array(
    'account' => $account,
    '#theme' => 'ces_bank_account_view',
  );
  // Add and override some items.
  $bank = new CesBank();
  $exchange = $bank->getExchange($account['exchange']);
  $page['account']['exchange'] = $exchange['name'];
  $users = reset($account['users']);
  $accuser = user_load($users['user']);
  if ($accuser) {
    $page['account']['username'] = $accuser->name;
  }
  else {
    $page['account']['username'] = t('Nonexistent user');
  }
  // TODO: these arrays are repeated in ces_bank_account_form(). They should be
  // shared.
  $kinds = array(
    0 => t('Individual'),
    1 => t('Shared'),
    2 => t('Organization'),
    3 => t('Company'),
    4 => t('Public'),
    5 => t('Virtual'),
  );
  $states = array(
    0 => t('Hidden'),
    1 => t('Active'),
    2 => t('Closed'),
    3 => t('Locked'),
  );
  $page['account']['kind'] = $kinds[$account['kind']];
  $page['account']['state'] = $states[$account['state']];
  $limitchain = $bank->getLimitChain($account['limitchain']);
  $page['account']['limitchain'] = $limitchain['name'];
  return $page;
}
/**
 * TODO: Document.
 *
 * @param array $transaction
 *   transaction record.
 */
function ces_bank_transaction_view($transaction) {
  $bank = new CesBank();
  if (isset($transaction['fromaccountname']) && isset($transaction['toaccountname'])) {
    // Transaction is comming from new/edit form.
    $fromaccount = $bank->getAccountByName($transaction['fromaccountname']);
    $toaccount = $bank->getAccountByName($transaction['toaccountname']);
  }
  else {
    $fromaccount = $bank->getTransactionFromAccount($transaction);
    $toaccount = $bank->getTransactionToAccount($transaction);
  }
  $page = array(
    '#theme' => 'ces_bank_transaction_view',
  );
  $page['transaction'] = $transaction;
  $states = _ces_bank_get_transaction_states();
  $page['transaction']['state'] = $states[$bank->getTransactionState($transaction)];
  $exchange = ces_bank_get_current_exchange();
  $page['transaction']['amount'] = $bank->formatAmount($transaction['amount'], $exchange, TRUE);
  $page['fromaccount'] = $fromaccount;
  $page['toaccount'] = $toaccount;
  $page['toexchange'] = $bank->getExchange($toaccount['exchange']);
  if ($fromaccount['exchange'] != $toaccount['exchange']) {
    $page['fromexchange'] = $bank->getExchange($fromaccount['exchange']);
  }
  else {
    $page['fromexchange'] = &$page['toexchange'];
  }
  return $page;
}

/**
 * Sort the rows of a table.
 *
 * The second parameter $rows gets sorted depending on the $header definition
 * and the parameters passed by URL.
 *
 * @param array $headers
 *   Table heeaders as in theme_table.
 * @param array $rows
 *   Table rows as in theme_table, by reference.
 */
function _ces_bank_sort_table(array $headers, array &$rows) {
  global $_ces_bank_sort_table_column;
  $order = tablesort_get_order($headers);
  $sort = tablesort_get_sort($headers);
  // Find the column number to sort.
  $i = 0;
  foreach ($headers as $header) {
    if (isset($header['field']) && ($header['field'] == $order['sql'])) {
      $_ces_bank_sort_table_column = $i;
      break;
    }
    $i++;
  }
  if ($sort == 'asc') {
    usort($rows, '_ces_bank_sort_table_cmp');
  }
  else {
    usort($rows, '_ces_bank_sort_table_cmpr');
  }
}

/**
 * Table ows comparator callback.
 */
function _ces_bank_sort_table_cmp($a, $b) {
  global $_ces_bank_sort_table_column;
  $ca = $a[$_ces_bank_sort_table_column];
  $cb = $b[$_ces_bank_sort_table_column];
  if (is_numeric($ca) && is_numeric($cb)) {
    return ($ca > $cb);
  }
  else {
    return strcmp($ca, $cb);
  }
}

/**
 * Reverse table rows comparator callback.
 */
function _ces_bank_sort_table_cmpr($a, $b) {
  global $_ces_bank_sort_table_column;
  $ca = $a[$_ces_bank_sort_table_column];
  $cb = $b[$_ces_bank_sort_table_column];
  if (is_numeric($ca) && is_numeric($cb)) {
    return ($cb > $ca);
  }
  else {
    return strcmp($cb, $ca);
  }
}
/**
 * Lastest transactions admin page.
 */
function ces_bank_transactions_admin_page($exchange_id) {
  $bank = new CesBank();
  if (!isset($exchange_id) || empty($exchange_id) || $exchange_id == 'ces_current_exchange') {
    $exchange = ces_bank_get_current_exchange();
  }
  else {
    $exchange = $bank->getExchange($exchange_id);
  }
  if ($exchange === FALSE) {
    return array(
      'message' => array(
        '#markup' => '<p>' . t('Invalid exchange') . '</p>',
      ),
    );
  }
  // Use the account statement filter form.
  $form_state = array('method' => 'get');
  require_once drupal_get_path('module', 'ces_bank') . '/ces_bank.forms.inc';
  $form = drupal_build_form('ces_bank_account_statement_form', $form_state);
  $from = $form_state['values']['from'];
  $to = $form_state['values']['to'];

  $timefrom = mktime(0, 0, 0, $from['month'], $from['day'], $from['year']);
  $timeto = mktime(23, 59, 59, $to['month'], $to['day'], $to['year']);
  // Transaction table.
  $form['transactions'] = array(
    '#type' => 'container',
    '#attributes' => array(),
  );
  $form['transactions']['table'] = array(
    '#theme' => 'table',
    '#attributes' => array(
      'class' => array('ces-table'),
    ),
    '#header' => array(
      array(
        'data' => t('Date'),
        'field' => 'created',
        'sort' => 'asc',
      ),
      array(
        'data' => t('State'),
        'class' => 'ces-statement-state',
      ),
      array(
        'data' => t('Buyer'),
        'class' => 'ces-statement-user',
      ),
      array(
        'data' => t('Seller'),
        'class' => 'ces-statement-user',
      ),
      array(
        'data' => t('Description'),
        'class' => 'ces-statement-description',
      ),
      array(
        'data' => t('Amount'),
        'class' => 'ces-currency-amount',
      ),
    ),
  );
  $transactions = $bank->getTransactions(array(
    'exchange' => $exchange['id'],
    'createdsince' => $timefrom,
    'createduntil' => $timeto,
  ));
  $dateformat = $format = variable_get('date_format_short', 'm/d/Y');
  $space = strpos($format, ' ');
  if ($space !== FALSE) {
    $dateformat = substr($dateformat, 0, $space);
  }
  $rows = array();
  foreach ($transactions as $transaction) {
    $row = array();
    $row[] = array(
      'data' => format_date($transaction['created'], 'custom', $dateformat),
    );
    $statestring = _ces_bank_get_transaction_states();
    $row[] = array('data' => $statestring[$bank->getTransactionState($transaction)]);
    $buyer = $bank->getTransactionFromAccount($transaction);
    $useracc = reset($buyer['users']);
    $buyer_uid = $useracc['user'];
    $row[] = array(
      'data' => $buyer['name'] . ' - '
      . l(ces_user_get_name(user_load($buyer_uid)), 'user/' . $buyer_uid),
    );
    $seller = $bank->getTransactionToAccount($transaction);
    $useracc = reset($seller['users']);
    $seller_uid = $useracc['user'];
    $row[] = array(
      'data' => $seller['name'] . ' - '
      . l(ces_user_get_name(user_load($seller_uid)), 'user/' . $seller_uid),
    );
    $row[] = array(
      'data' => l($transaction['concept'],
      'ces/bank/account/transaction/' . $transaction['id'] . '/view'),
    );
    $row[] = array(
      'data' => $bank->formatAmount($transaction['amount'], $exchange, TRUE, FALSE),
      'class' => 'ces-currency-amount',
    );
    $rows[] = $row;
  }
  // Get sort parameter from URL.
  $sort = tablesort_get_sort($form['transactions']['table']['#header']);
  if ($sort == 'desc') {
    $rows = array_reverse($rows);
  }
  $form['transactions']['table']['#rows'] = $rows;
  return $form;
}

/**
 * Page handler for account statement list.
 */
function ces_bank_account_statement_page() {
  $account = ces_bank_get_current_account();
  if ($account === FALSE) {
    $page = array();
    $page['message'] = array(
      '#markup' => '<p>' . t("You don't have any exchange account yet.")
      . '</p>',
    );
    return $page;
  }

  $form_state = array('method' => 'get');
  require_once drupal_get_path('module', 'ces_bank') . '/ces_bank.forms.inc';
  $form = drupal_build_form('ces_bank_account_statement_form', $form_state);

  $bank = new CesBank();
  $exchange = $bank->getExchange($account['exchange']);

  $from = $form_state['values']['from'];
  $to = $form_state['values']['to'];

  $timefrom = mktime(0, 0, 0, $from['month'], $from['day'], $from['year']);
  $timeto = mktime(23, 59, 59, $to['month'], $to['day'], $to['year']);
  // Transaction table.
  $form['transactions'] = array(
    '#type' => 'container',
    '#attributes' => array(),
  );
  $form['transactions']['table'] = array(
    '#theme' => 'table',
    '#attributes' => array(
      'class' => array('ces-table'),
    ),
    '#header' => array(
      array(
        'data' => t('Date'),
        'field' => 'created',
        'sort' => 'asc',
      ),
      array(
        'data' => t('State'),
        'class' => 'ces-statement-state',
      ),
      array(
        'data' => t('Account'),
        'class' => 'ces-statement-user',
      ),
      array(
        'data' => t('Description'),
        'class' => 'ces-statement-description',
      ),
      array(
        'data' => t('Amount'),
        'class' => 'ces-currency-amount',
      ),
      array(
        'data' => t('Balance'),
        'class' => 'ces-currency-amount',
      ),
    ),
  );
  $transactions = $bank->getTransactions(array(
    'account' => $account['id'],
    'createdsince' => $timefrom,
    'createduntil' => $timeto,
  ));

  if (!$transactions) {
    drupal_set_message(t('No Transaction'));
  }

  $inibalance = $bank->getAccountHistoricBalance($account['id'], $timefrom);
  $dateformat = $format = variable_get('date_format_short', 'm/d/Y');
  $space = strpos($format, ' ');
  if ($space !== FALSE) {
    $dateformat = substr($dateformat, 0, $space);
  }
  $rows = array();
  foreach ($transactions as $transaction) {
    $row = array();
    $row[] = array(
      'data' => format_date($transaction['created'], 'custom', $dateformat));

    if ($transaction['fromaccount'] != $account['id']) {
      $other_account = $bank->getTransactionFromAccount($transaction);
      $type = 0;
    }
    elseif ($transaction['toaccount'] != $account['id']) {
      $other_account = $bank->getTransactionToAccount($transaction);
      $type = 1;
    }
    else {
      // This is impossible, but just in case.
      $other_account = $account;
      $type = 2;
    }
    $statestring = _ces_bank_get_transaction_states();
    $state = $bank->getTransactionState($transaction);
    $row[] = array('data' => $statestring[$state]);

    $useracc = reset($other_account['users']);
    $uid = $useracc['user'];

    $row[] = array('data' => $other_account['name'] . ' - ' . l(ces_user_get_name(user_load($uid)), 'user/' . $uid));
    $row[] = array('data' => l($transaction['concept'], 'ces/bank/account/transaction/' . $transaction['id'] . '/view'));
    $amount = ($transaction['amount']) * ($type == 0 ? 1 : -1);
    $row[] = array(
      'data' => $bank->formatAmount($amount, $exchange, TRUE, TRUE),
      'class' => 'ces-currency-amount',
    );

    if ($state == 3) {
      $inibalance += $amount;
    }
    $row[] = array(
      'data' => $bank->formatAmount($inibalance, $exchange, TRUE, FALSE),
      'class' => 'ces-currency-amount',
    );
    $rows[] = $row;
  }
  // Get sort parameter from URL.
  $sort = tablesort_get_sort($form['transactions']['table']['#header']);
  if ($sort == 'desc') {
    $rows = array_reverse($rows);
  }
  $form['transactions']['table']['#rows'] = $rows;
  return $form;

}
/**
 * Return the array of state number => Description.
 */
function _ces_bank_get_transaction_states() {
  return array(
    0 => t('New'),
    1 => t('Triggered'),
    2 => t('Accepted'),
    3 => t('Committed'),
    4 => t('Archived'),
    5 => t('Rejected'),
    6 => t('Discarded'),
    7 => t('Revoke triggered'),
    8 => t('Revoke accepted'),
    9 => t('Revoke rejected'),
    10 => t('Revoked'),
    11 => t('Error'),
  );
}
/** @} */

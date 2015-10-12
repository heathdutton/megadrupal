<?php
/**
 * @file
 * CES banking logic layer.
 */

/**
 * @defgroup ces_bank_logic Logic from Ces Bank
 * @ingroup ces_bank
 * @{
 * CES banking logic layer.
 */

/**
 * The controller for all the banking logic. This is the only accessible class
 * from outside this package.
 */
class CesBank {
  /**
   * TODO Documentation.
   */
  public function access($permission, $object, $objectid, $account = NULL) {
    $perm = new CesBankUserPermission($account);
    return $perm->access($permission, $object, $objectid);
  }
  /**
   * Create exchange. This exchange is not functional yet. It must be activated.
   *
   * @param array $record
   *   exchange object record.
   */
  public function createExchange(array &$record) {
    $t = IcesSerializer::DBTransaction();
    try {
      // Create exchange record.
      if (isset($record['id'])) {
        unset($record['id']);
      }
      $record['state'] = CesBankExchange::STATE_INACTIVE;
      $this->updateObject('CesBankExchange', $record);
      // Create new default category from exchange.
      $category = (object) array(
        'title' => 'general',
        'parent' => 0,
        'description' => 'General category',
        'exchange' => $record['id'],
        'context' => 3,
      );
      ces_category_save($category);
      ces_message_send_notify('new_exchange', array('exchange' => $record), array(1), 1);
    }
    catch (Exception $e) {
      $t->rollback();
      throw $e;
    }
  }
  /**
   * Updates the exchange record.
   *
   * @param array $record
   *   exchange object record.
   */
  public function updateExchange(array &$record) {
    $t = IcesSerializer::DBTransaction();
    try {
      return $this->updateObject('CesBankExchange', $record);
    }
    catch (Exception $e) {
      $t->rollback();
      throw $e;
    }
  }
  /**
   * Updates and activates an exchange.
   *
   * @param array $record
   *   exchange object record.
   */
  public function activateExchange(array &$record) {
    $t = IcesSerializer::DBTransaction();
    global $user;
    try{
      $s = new IcesSerializer('CesBankExchange');
      // Activate exchange.
      $exchange = $s->loadFromRecord($record);
      $exchange->activate();
      $s->save($exchange);
      // Create permission records. By the moment they are static,but they could
      // potentially be configured and stored in $record.
      // Let exchange users see the exchange details.
      $perm = array(
        'permission' => CesBankPermission::PERMISSION_VIEW,
        'object' => 'exchange details',
        'objectid' => $exchange->id,
        'scope' => CesBankPermission::SCOPE_EXCHANGE,
        'scopeid' => $exchange->id,
      );
      $this->createPermission($perm);
      // Let everybody register accounts to this exchange.
      $perm = array(
        'permission' => CesBankPermission::PERMISSION_USE,
        'object' => 'exchange accountcreator',
        'objectid' => $exchange->id,
        'scope' => CesBankPermission::SCOPE_GLOBAL,
        'scopeid' => 0,
      );
      $this->createPermission($perm);
      // Let exchange users see other exchange accounts.
      $perm = array(
        'permission' => CesBankPermission::PERMISSION_VIEW,
        'object' => 'exchange accounts',
        'objectid' => $exchange->id,
        'scope' => CesBankPermission::SCOPE_EXCHANGE,
        'scopeid' => $exchange->id,
      );
      $this->createPermission($perm);
      // Let exchange creator administer the exchange.
      $perm = array(
        'permission' => CesBankPermission::PERMISSION_ADMIN,
        'object' => 'exchange',
        'objectid' => $exchange->id,
        'scope' => CesBankPermission::SCOPE_USER,
        'scopeid' => $exchange->admin,
      );
      $this->createPermission($perm);
      // Let exchange users see blog posts and comment them.
      $perm = array(
        'permission' => CesBankPermission::PERMISSION_USE,
        'object' => 'exchange blog',
        'objectid' => $exchange->id,
        'scope' => CesBankPermission::SCOPE_EXCHANGE,
        'scopeid' => $exchange->id,
      );
      $this->createPermission($perm);

      // Create a new zero account for the admin of this exchange.
      $account = $this->getDefaultAccount($exchange->id);
      $account['users'] = array(
        $exchange->admin => array(
          'user' => $exchange->admin,
          'role' => CesBankAccountUser::ROLE_ACCOUNT_ADMINISTRATOR,
        ),
      );
      $account['name'] = $exchange->code . '0000';
      $this->createAccount($account);

      ces_message_send_notify('exchange_activated', array('exchange' => $exchange->getRecord()), array($exchange->admin), $user->uid);
      $this->activateAccount($account);
      $record = $exchange->getRecord();
    }
    catch (Exception $e) {
      $t->rollback();
      throw $e;
    }
  }
  /**
   * Updates and activates an account.
   *
   * @param array $record
   *   account object record.
   */
  public function activateAccount(array &$record) {
    $t = IcesSerializer::DBTransaction();
    global $user;
    try{
      $s = new IcesSerializer('CesBankLocalAccount');
      $account = $s->loadFromRecord($record);
      $account->activate();
      $s->save($account);
      // Create permission entry for account owner(s).
      $accountusers = $account->getUsers();
      foreach ($accountusers as $accountuser) {
        $perm = array(
          'permission' => CesBankPermission::PERMISSION_EDIT,
          'object' => 'account',
          'objectid' => $account->id,
          'scope' => CesBankPermission::SCOPE_USER,
          'scopeid' => $accountuser->user,
        );
        $this->createPermission($perm);
      }
      // Build recipients array.
      $recipients = array();
      $accusers = $account->getUsers();
      foreach ($accusers as $accuser) {
        $recipients[] = $accuser->user;
      }
      ces_message_send_notify('account_activated', array('account' => $account->getRecord()), $recipients, $user->uid);
    }
    catch (Exception $e) {
      $t->rollback();
      throw $e;
    }
  }
  /**
   * Gets the exchange from its id.
   *
   * @param int $id
   *   exchange identifier.
   */
  public function getExchange($id) {
    return $this->getObjectRecord('CesBankExchange', $id);
  }
  /**
   * Gets the exchange object by name.
   *
   * @param string $name
   *   Four letters exchange code.
   *
   * @return array
   *   an exchange object record, or FALSE if not available.
   */
  public function getExchangeByName($name) {
    $s = new IcesSerializer('CesBankExchange');
    $exchange = $s->loadFromUniqueKey('code', $name);
    if ($exchange === FALSE) {
      return FALSE;
    }
    return $exchange->getRecord();
  }
  /**
   * Deletes an exchange.
   *
   * @param int $id
   *   exchange identifier.
   */
  public function deleteExchange($id) {
    $t = IcesSerializer::DBTransaction();
    try{
      $s = new IcesSerializer('CesBankExchange');
      $exchange = $s->load($id);
      $s->delete($exchange);
      $this->deletePermissionsToScope('exchange', $id);
      return TRUE;
    }
    catch (Exception $e) {
      $t->rollback();
      throw $e;
    }
  }
  /**
   * Gets all exchanges under certain conditions.
   *
   * @return array
   *   with all exchanges in this server that satisfy certain conditions.
   *
   * @param array $conditions
   *   conditions filter on the fields of exchange records. By default return
   *   the active ones.
   */
  public function getAllExchanges(array $conditions = array('state' => 1)) {
    $s = new IcesSerializer('CesBankExchange');
    $exchanges = $s->loadCollection($conditions);
    $records = array();
    $as = new IcesSerializer('CesBankLocalAccount');
    foreach ($exchanges as $exchange) {
      $record = $exchange->getRecord();
      $record['accounts'] = $as->count('exchange', $exchange->id);
      $records[$exchange->id] = $record;
    }
    return $records;
  }
  /**
   * Get all accounts that meet certain conditions.
   *
   * @return array
   *   with all accounts that meet certain conditions.
   *
   * @param array $conditions
   *   filter the records. A common example is:
   *   <code>
   *   $conditions = array( 'exchange' => $id, 'state' => 1 )
   *   </code>
   */
  public function getAllAccounts(array $conditions = array('state' => 1), $order = NULL) {
    $as = new IcesSerializer('CesBankLocalAccount');
    $accounts = $as->loadCollection($conditions, $order);
    $records = array();
    foreach ($accounts as $account) {
      $records[$account->id] = $account->getRecord();
    }
    return $records;
  }
  /**
   * Creates a transaction object.
   *
   * The transaction is not made effective until it is applied calling:
   * <code>
   * $this->applyTransaction();
   * </code>
   * so it can safely be modified.
   */
  public function createTransaction(array &$record) {
    $t = IcesSerializer::DBTransaction();
    try {
      $transaction = $this->newTransaction($record);
      $s = new IcesSerializer(get_class($transaction));
      $s->save($transaction);
      $record['id'] = $transaction->getId();
    }
    catch (Exception $e) {
      $t->rollback();
      throw $e;
    }
  }
  /**
   * Updates the transaction record.
   *
   * In fact it deletes the old transaction and creates and applies a new one.
   * The <code>$record['id']</code> will change.
   *
   * @param array $record
   *   The transaction object record.
   *
   * @return bool
   *   TRUE if the transaction could be successfully updated and FALSE
   *   otherwise.
   */
  public function updateTransaction(array &$record) {
    $t = IcesSerializer::DBTransaction();
    try {
      $this->deleteTransaction($record['id']);
      unset($record['id']);
      $this->createTransaction($record);
      return $this->applyTransaction($record['id']);
    }
    catch (Exception $e) {
      $t->rollback();
      throw $e;
    }
  }
  /**
   * Get a transaction record.
   *
   * @param int $id
   *   transaction identifier.
   *
   * @return CesBankTransactionInterface
   *   a transaction record given its id.
   */
  public function getTransaction($id) {
    return $this->getObjectRecord('CesBankBasicTransaction', $id);
  }
  /**
   * Get the buyer's account given a transaction.
   *
   * @param array $record
   *   transaction object record.
   *
   * @return array
   *   the account object record which is the buyer of the given transaction.
   */
  public function getTransactionFromAccount(array $record) {
    $s = new IcesSerializer('CesBankBasicTransaction');
    $transaction = $s->loadFromRecord($record);
    $name = $transaction->getFromAccountName();
    return $this->getAccountByName($name);
  }
  /**
   * Get the seller's account given a transaction.
   *
   * @param array $record
   *   transaction object record.
   *
   * @return array
   *   the account object record which is the seller of the given transaction.
   */
  public function getTransactionToAccount(array $record) {
    $s = new IcesSerializer('CesBankBasicTransaction');
    $transaction = $s->loadFromRecord($record);
    $name = $transaction->getToAccountName();
    return $this->getAccountByName($name);
  }
  /**
   * Get a transaction amount in a specific exchange currency.
   *
   * @param array $transaction
   *   transaction object record.
   * @param array $exchange
   *   exchange object record.
   *
   * @return float
   *   the transaction amount.
   */
  public function getTransactionAmount(array $transaction, array $exchange) {
    $st = new IcesSerializer('CesBankBasicTransaction');
    $trans = $st->loadFromRecord($transaction);
    $se = new IcesSerializer('CesBankExchange');
    $excha = $se->loadFromRecord($exchange);
    return $trans->getAmount($excha->getCurrency());
  }
  /**
   * Return the array of transaction messages for the current transaction state.
   *
   * @param array $transaction
   *   transaction object record.
   *
   * @return array
   *   $log array of strings with log messages for this transaction and current
   *   state.
   */
  public function getTransactionLog(array $transaction) {
    $st = new IcesSerializer('CesBankBasicTransaction');
    $trans = $st->loadFromRecord($transaction);
    return $trans->getLog($trans->getState());
  }
  /**
   * Get the state of this trasaction.
   *
   * Note that the result of this function may be different of the content of
   * $transaction['state'] for example on interexchange transactions where one
   * is accepted and the other still triggered.
   *
   * @param array $transaction
   *   The transaction record.
   *
   * @return int
   *   The true state of the trasaction.
   */
  public function getTransactionState(array $transaction) {
    $st = new IcesSerializer('CesBankBasicTransaction');
    $trans = $st->loadFromRecord($transaction);
    return $trans->getState();
  }
  /**
   * Gets transaction object records based on conditions.
   *
   * @param array $conditions
   *   Associative array with conditions to be met by the returned transaction
   *   records. Possible values include:
   *   - account: id. Get transactions from or to the specified account id.
   *   If this parameter is set then the following two must not be set.
   *   - fromaccount: id. Get transactions where specified account id is buyer.
   *   - toaccount: id. Get transactions where the specified account id is
   *   seller.
   *   - createdsince: time. Get transactions created after this timestamp in
   *   seconds.
   *   - createduntil: time. Get transactions created before this timestamp in
   *   seconds.
   *   - limit: int. Get only n transactions.
   * @param string $order_field
   *   A field to order by.
   * @param string $order_type
   *   DESC: Descending, ASC Ascending.
   *
   * @return array
   *   Transaction records meeting all conditions specified by $conditions
   *   parameter. The amount is expressed in a currency depending on the given
   *   conditions: if 'account' is set, then it is used the account's currency.
   *   If 'fromaccount' is set but 'toaccount' is omitted, then the
   *   fromaccount's currency is used. Otherwise the currency is the seller's
   *   account one for each exchenge.
   */
  public function getTransactions(array $conditions, $order_field = 'created', $order_type = 'ASC') {
    $s = new IcesSerializer('CesBankBasicTransaction');
    // Choose currency.
    $currencyaccount = NULL;
    $currency = NULL;
    if (isset($conditions['account'])) {
      $currencyaccount = $conditions['account'];
    }
    if (isset($conditions['fromaccount']) && !isset($conditions['toaccount'])) {
      $currencyaccount = $conditions['fromaccount'];
    }
    if ($currencyaccount != NULL) {
      $as = new IcesSerializer('CesBankLocalAccount');
      $account = $as->load($currencyaccount);
      $currency = $account->getExchange()->getCurrency();
    }
    // Load collection.
    if (isset($conditions['account'])) {
      // An array condition is an OR of its contents.
      $account = $conditions['account'];
      unset($conditions['account']);
      $conditions['or'] = array(
        'fromaccount' => $account,
        'toaccount' => $account,
      );
      $transactions = $s->loadCollection($conditions, $order_field, $order_type);
    }
    else {
      if (isset($conditions['exchange'])) {
        $exchange_id = $conditions['exchange'];
        unset($conditions['exchange']);
        $conditions['join'] = array(
          'field' => 'toaccount',
          'class' => 'CesBankLocalAccount',
          'condition' => array(
            'field' => 'exchange',
            'value' => $exchange_id,
          ),
        );
      }
      $transactions = $s->loadCollection($conditions, $order_field, $order_type);
    }
    $records = array();
    foreach ($transactions as $t) {
      $record = $t->getRecord();
      if ($currency != NULL) {
        $record['amount'] = $t->getAmount($currency);
      }
      $records[] = $record;
    }
    return $records;
  }
  /**
   * Merge two sorted collections of transaction objects.
   *
   * @param array $c1
   *   The first TansactionInterface collection.
   * @param array $c2
   *   The second TansactionInterface collection.
   *
   * @return array
   *   The sorted merge of $c1 and $c2.
   */
  private function mergeSortedTransactionCollections(array $c1, array $c2) {
    $i1 = 0;
    $n1 = count($c1);
    $i2 = 0;
    $n2 = count($c2);
    $c = array();
    while ($i1 < $n1 && $i2 < $n2) {
      $c[] = ($c1[$i1]->getCreated() < $c2[$i2]->getCreated()) ? $c1[$i1++] : $c2[$i2++];
    }
    while ($i1 < $n1) {
      $c[] = $c1[$i1++];
    }
    while ($i2 < $n2) {
      $c[] = $c2[$i2++];
    }
    return $c;
  }
  /**
   * Delete transaction.
   *
   * @param int $id
   *   Transaction identifier.
   *
   * @return TRUE
   *   If a problem occurs an exception is thrown.
   */
  public function deleteTransaction($id) {
    $t = IcesSerializer::DBTransaction();
    try{
      $s = new IcesSerializer('CesBankBasicTransaction');
      $transaction = $s->load($id);
      if ($transaction === FALSE) {
        throw new Exception(t('Transaction id %id is invalid',
          array('%id' => $id)));
      }
      if ($transaction->getState() == CesBankTransactionInterface::STATE_COMMITTED) {
        $transaction->revoke();
      }
      // The following statement will throw an exception if the transaction is
      // not new, revoked or rejected.
      $transaction->setState(CesBankTransactionInterface::STATE_DISCARDED);
      $s->delete($transaction);
      return TRUE;
    }
    catch (Exception $e) {
      $t->rollback();
      throw $e;
    }
  }
  /**
   * Creates a banking account.
   *
   * @param array $record
   *   account object record.
   * @param bool $send_mail
   *   (T/F)
   */
  public function createAccount(array &$record, $send_mail = TRUE) {
    $t = IcesSerializer::DBTransaction();
    try {
      // Create account record.
      unset($record['id']);
      $record['balance'] = 0;
      $account = new CesBankLocalAccount($record);
      $s = new IcesSerializer('CesBankLocalAccount');
      $s->save($account);
      // saveExtraData() after create account record because we need the new
      // account id in order to create the CesBankAccountUser record(s).
      $account->saveExtraData();
      $record = $account->getRecord();
      $exchange = $account->getExchange();
      ces_message_send_notify('new_account', array('account' => $account->getRecord()), array($exchange->admin), 1, $send_mail);
    }
    catch (Exception $e) {
      $t->rollback();
      throw $e;
    }
  }
  /**
   * Updates a banking account.
   *
   * @param array $record
   *   account object record.
   */
  public function updateAccount(array &$record) {
    $t = IcesSerializer::DBTransaction();
    unset($record['balance']);
    try{
      return $this->updateObject('CesBankLocalAccount', $record);
    }
    catch (Exception $e) {
      $t->rollback();
      throw $e;
    }
  }
  /**
   * Gets a banking account.
   *
   * @param int $id
   *   account identifier.
   *
   * @return array
   *   account object record.
   */
  public function getAccount($id) {
    return $this->getObjectRecord('CesBankLocalAccount', $id);
  }
  /**
   * Gets an account object record by its name.
   *
   * @param string $name
   *   account name. 4 letters and 4 numbers.
   *
   * @return array
   *   an account given its name
   */
  public function getAccountByName($name) {
    $s = new IcesSerializer('CesBankLocalAccount');
    $account = $s->loadFromUniqueKey('name', $name);
    if ($account === FALSE) {
      return FALSE;
    }
    return $account->getRecord();
  }
  /**
   * Gets an account's historic value.
   * @todo: Use protected function?
   */
  protected function getAccountHistoricValue($id, $field, $from, $to) {
    $s = new IcesSerializer('CesBankLocalAccount');
    $account = $s->load($id);
    $st = new IcesSerializer('CesBankBasicTransaction');
    $result = (double) $st->selectFunctionField('SUM', 'amount',
      array(
        array($field, $account->id),
        array('created', $from, '>='),
        array('created', $to, '<='),
        array('state', array(3)),
      ));
    return $result;
  }
  /**
   * Gets an account's historic sales.
   */
  public function getAccountHistoricSales($id, $from, $to = NULL) {
    if ($to == NULL) {
      $to = time();
    }
    return $this->getAccountHistoricValue($id, 'toaccount', $from, $to);
  }
  /**
   * Gets an account's historic purchases.
   */
  public function getAccountHistoricPurchases($id, $from, $to = NULL) {
    if ($to == NULL) {
      $to = time();
    }
    return $this->getAccountHistoricValue($id, 'fromaccount', $from, $to);
  }
  /**
   * Gets an account's historic balance at a certain time point.
   */
  public function getAccountHistoricBalance($id, $time) {
    $s = new IcesSerializer('CesBankLocalAccount');
    $account = $s->load($id);
    $balance = $account->balance;
    $positive = $this->getAccountHistoricSales($id, $time);
    $negative = $this->getAccountHistoricPurchases($id, $time);
    $timebalance = $balance - $positive + $negative;
    return $timebalance;
  }
  /**
   * Get user accounts.
   *
   * @return array
   *   of this user accounts.
   */
  public function getUserAccounts($uid) {
    $s = new IcesSerializer('CesBankAccountUser');
    $useraccounts = $s->loadCollection(array('user' => $uid));
    $accounts = array();
    foreach ($useraccounts as $useracc) {
      $accounts[] = $this->getAccount($useracc->account);
    }
    return $accounts;
  }
  /**
   * Deletes an account.
   */
  public function deleteAccount($id) {
    $t = IcesSerializer::DBTransaction();
    try{
      $s = new IcesSerializer('CesBankLocalAccount');
      $account = $s->load($id);
      $s->delete($account);
      $this->deletePermissionsToScope('account', $id);
      return TRUE;
    }
    catch (Exception $e) {
      $t->rollback();
      throw $e;
    }
  }
  /**
   * Check if it's possible to delete accounts or exchange.
   *
   * @param array $arrayaids
   *   array of account identifiers.
   *
   * @param string $context
   *   'account': checking for delete one or more accounts of an exchange.
   *   exchange id: checking for delete all accounts of an entire exchange.
   *
   * @return array
   *   array of accounts:
   *     'aid': account id, 'name': account name.
   *     'account_action': recommended action for the account.
   *       'delete', 'close', 'nothing'(do nothing).
   *     'is_interexchange': TRUE if it's an interexchange account and
   *       refers to an existing exchange in the system.
   *     'num_transactions': number of transactions that refers to the account.
   *     'transactions': array of transactions that refer to the account.
   *     'account_users': array of account users.
   *       'uid', 'accounts'(array of other accounts if any),
   *       'user_action'(recomended action for this user).
   *   array of users: (only is $context != 'account')
   *     All users, recommended action and other accounts if there are any.
   *   array of transactions: (only is $context != 'account')
   *     Total number of transactions and array of transactions
   *     that refers from or to another exchange (inter-exchange).
   */
  public function deleteAccountsCheck($arrayaids, $context = 'account') {
    $arrayreturn = array(
      'accounts' => array(),
      'users' => array(),
      'transactions' => array(),
    );
    $joinusers = array();
    foreach ($arrayaids as $singleaid) {
      $account = $this->getAccount($singleaid);
      $return = array(
        'aid' => $account['id'],
        'name' => $account['name'],
        'account_action' => 'nothing',
        'is_interexchange' => FALSE,
        'num_transactions' => 0,
        'transactions' => array(),
        'account_users' => array(),
      );
      // Checking if it's an interexchange account.
      if ($account['kind'] == 5) {
        $return['is_interexchange'] = TRUE;
      }
      // Checking if account has transactions, only if $context == 'account'.
      if ($context == 'account') {
        $transactions = $this->getTransactions(array('account' => $account['id']));
        foreach ($transactions as $trans) {
          $return['transactions'][] = $trans;
          $return['num_transactions']++;
        }
      }
      // Checking if users has any other accounts in the system.
      foreach ($account['users'] as $u) {
        $return['account_users'][$u['user']] = array(
          'uid' => $u['user'],
          'accounts' => array(),
          'user_action' => ($return['num_transactions'] == 0) ? 'delete' : 'cancel',
        );
        if ($context != 'account') {
          if (!array_key_exists($u['user'], $joinusers)) {
            $joinusers[$u['user']]['raw_user_action'] = array('delete');
          }
        }
        $useraccounts = $this->getUserAccounts($u['user']);
        foreach ($useraccounts as $accs) {
          if ($accs['id'] != $account['id']) {
            $return['account_users'][$u['user']]['accounts'][] = array(
              'aid' => $accs['id'],
              'name' => $accs['name'],
            );
            $return['account_users'][$u['user']]['user_action'] = 'nothing';
            if ($context != 'account') {
              $joinusers[$u['user']]['raw_user_action'][] = 'nothing';
            }
          }
        }
      }
      if ($return['is_interexchange']) {
        $return['account_action'] = 'nothing';
      }
      else {
        if ($return['num_transactions'] == 0) {
          $return['account_action'] = 'delete';
        }
        else {
          $return['account_action'] = 'close';
        }
      }
      $arrayreturn['accounts'][] = $return;
    }
    if ($context != 'account') {
      // Select priority action if there are several actions for one user.
      foreach ($joinusers as $u => $ra) {
        if (count($ra['raw_user_action']) == 1) {
          $arrayreturn['users'][$u]['user_action'] = $ra['raw_user_action']['0'];
        }
        else {
          if (in_array('nothing', $ra['raw_user_action'])) {
            $priority_action = 'nothing';
          }
          else {
            $priority_action = 'delete';
          }
          $arrayreturn['users'][$u]['user_action'] = $priority_action;
        }
        // Look into users with severals accounts if the accounts:
        // - belong to another exchange.
        // - are inter-exchange accounts.
        if ($arrayreturn['users'][$u]['user_action'] == 'nothing') {
          $useraccounts = $this->getUserAccounts($u);
          foreach ($useraccounts as $uacc) {
            if ($uacc['exchange'] != $context || $account['kind'] != 5) {
              $arrayreturn['users'][$u]['accounts'][] = array(
                'aid' => $uacc['id'],
                'name' => $uacc['name'],
              );
            }
          }
          if (empty($arrayreturn['users'][$u]['accounts'])) {
            $arrayreturn['users'][$u]['user_action'] = 'delete';
          }
        }
      }
      // All transactions of the exchange.
      $querytrans = db_query("
        SELECT t.id
        FROM {ces_transaction} t
        INNER JOIN {ces_account} a
        ON (a.exchange = :exchange
        AND a.id = t.fromaccount)",
        array(
          ':exchange' => $context,
        ));
      $arrayreturn['transactions']['tids'] = array();
      $arrayreturn['transactions']['totaltrans'] = 0;
      foreach ($querytrans as $record) {
        $arrayreturn['transactions']['tids'][] = $record->id;
        $arrayreturn['transactions']['totaltrans']++;
      }
    }
    return $arrayreturn;
  }
  /**
   * Delete or cancel drupal users and all content associated.
   *
   * @param array $users
   *   'uid': user id.
   *   'action':
   *     delete -> delete oferswants, comments and user.
   *     cancel -> hide offerswants, delete comments, cancel user.
   */
  public function deleteUsers(array $users) {
    $module_offerswants = module_exists('ces_offerswants');
    $table_offerwant = db_table_exists('ces_offerwant');
    $table_offercomment = db_table_exists('ces_offercomment');
    foreach ($users as $user) {
      if ($user['uid'] != 1) {
        if ($module_offerswants && $table_offerwant) {
          $userloaded = user_load($user['uid']);
          $username = ces_bank_get_full_username($userloaded);
          if ($user['action'] == 'delete') {
            $offerswants = db_delete('ces_offerwant')
              ->condition('user', $user['uid'])
              ->execute();
            if ($offerswants > 0) {
              drupal_set_message(t('%offrs offers and wants of user %usr have been deleted.',
                array('%offrs' => $offerswants, '%usr' => $username)), 'status');
            }
          }
          if ($user['action'] == 'cancel') {
            $offerswants = db_update('ces_offerwant')
              ->condition('user', $user['uid'])
              ->fields(array('state' => 0))
              ->execute();
            if ($offerswants > 0) {
              drupal_set_message(t('%offrs offers and wants of user %usr have been hidden.',
                array('%offrs' => $offerswants, '%usr' => $username)), 'status');
            }
          }
          if ($table_offercomment) {
            $offercomments = db_delete('ces_offercomment')
              ->condition('uid', $user['uid'])
              ->execute();
            if ($offercomments > 0) {
              drupal_set_message(t('%offrcomm offer comments and ratings of user %usr have been deleted.',
                array('%offrcomm' => $offercomments, '%usr' => $username)), 'status');
            }
          }
        }
        $arg_cancel_user = ($user['action'] == 'delete') ? 'user_cancel_delete' : 'user_cancel_block_unpublish';
        user_cancel(array(), $user['uid'], $arg_cancel_user);
      }
    }
    $batch =& batch_get();
    $batch['progressive'] = FALSE;
    batch_process();
  }
  /**
   * Creates a limit chain object.
   */
  public function createLimitChain(array &$record) {
    $t = IcesSerializer::DBTransaction();
    try {
      // Create account record.
      unset($record['id']);
      $limitchain = new CesBankLimitChain($record);
      $s = new IcesSerializer('CesBankLimitChain');
      $s->save($limitchain);
      // Save extra data because we need to have the limitchain id in orde to
      // properly save the atomic limits.
      $limitchain->saveExtraData();
      $record = $limitchain->getRecord();
    }
    catch (Exception $e) {
      $t->rollback();
      throw $e;
    }
  }
  /**
   * Updates a limit chain object.
   */
  public function updateLimitChain(array &$record) {
    $t = IcesSerializer::DBTransaction();
    try{
      return $this->updateObject('CesBankLimitChain', $record);
    }
    catch (Exception $e) {
      $t->rollback();
      throw $e;
    }
  }
  /**
   * Gets a limit chain object record.
   * @return array
   *   a limit chain given its id.
   */
  public function getLimitChain($id) {
    return $this->getObjectRecord('CesBankLimitChain', $id);
  }
  /**
   * Deletes a limit chain.
   */
  public function deleteLimitChain($id) {
    $t = IcesSerializer::DBTransaction();
    try{
      $s = new IcesSerializer('CesBankLimitChain');
      $chain = $s->load($id);
      $s->delete($chain);
      return TRUE;
    }
    catch (Exception $e) {
      $t->rollback();
      throw $e;
    }
  }
  /**
   * Gets the default exchange object record.
   * @return array
   *   the default exchange. To be used as a model for new exchanges.
   */
  public function getDefaultExchange() {
    // @TODO: hardcode it!
    $exchange = CesBankExchange::getDefaultExchange();
    $exchange->admin = NULL;
    $exchange->id = NULL;
    return $exchange->getRecord();
  }
  /**
   * Gets default account object record for given exchange.
   * @return array
   *   the exchange's default account. To be used as a model for new
   *   accounts for this exchange.
   */
  public function getDefaultAccount($exchange_id) {
    if ($exchange_id == '') {
      drupal_set_message(t('CES Debug: getDefaultAccount: No exchangeId.'));
    }
    $exchange = $this->getObject('CesBankExchange', $exchange_id);
    if ($exchange == FALSE) {
      drupal_set_message(t('CES Debug: getDefaultAccount: exchange is false.'));
    }
    return $exchange->getDefaultAccount()->getRecord();
  }
  /**
   * Gets default limit chain object record.
   * @return array
   *   the default limit chain given an exchange_id. To be used as template
   *   for new limit chains.
   */
  public function getDefaultLimitChain($exchange_id) {
    $exchange = $this->getObject('CesBankExchange', $exchange_id);
    $account  = $exchange->getDefaultAccount();
    return $account->getLimitChain()->getRecord();
  }
  /**
   * Get all limit chains object records for this exchange.
   * @return array
   *   all defined limit chains for this exchange.
   */
  public function getAllLimitChains($exchange_id) {
    $s = new IcesSerializer('CesBankLimitChain');
    $limits = $s->loadCollection(array('exchange' => $exchange_id));
    $records = array();
    foreach ($limits as $limit) {
      $records[$limit->id] = $limit->getRecord();
    }
    return $records;
  }
  /**
   * Applies a transaction given its id.
   *
   * @param int $transaction_id
   *   the id of the transaction object record.
   *
   * @return bool
   *   TRUE if the transaction is successfully applied and
   *   committed at the end of this function. FALSE otherwise, and some log
   *   messages can be accessed through the log field of the transaction.
   *
   * @throw Exception
   *   And rollback the database transaction if something goes unexpectedly.
   */
  public function applyTransaction($transaction_id) {
    $t = IcesSerializer::DBTransaction();
    try {
      // Get transaction. May be any type of transaction.
      $transaction = $this->getObject('CesBankBasicTransaction', $transaction_id);
      if ($transaction->getState() == CesBankTransactionInterface::STATE_NEW) {
        // Trigger transaction application.
        $transaction->trigger();
      }
      if ($transaction->getState() == CesBankTransactionInterface::STATE_TRIGGERED) {
        // Check if we can apply the transaction.
        $transaction->check();
      }
      if ($transaction->getState() == CesBankTransactionInterface::STATE_ACCEPTED) {
        // Effectively apply transaction.
        $transaction->apply();
      }
      // Check for error.
      if ($transaction->getState() != CesBankTransactionInterface::STATE_ACCEPTED
        && $transaction->getState() != CesBankTransactionInterface::STATE_REJECTED
        && $transaction->getState() != CesBankTransactionInterface::STATE_TRIGGERED
        && $transaction->getState() != CesBankTransactionInterface::STATE_COMMITTED) {

        $errors = $transaction->getLog($transaction->getState());
        $message = t('There has been an error applying the transaction.');
        if (!empty($errors)) {
          $message .= '<ul><li>' . implode('</li><li>', $errors) . '</li></ul>';
        }
        throw new Exception($message);
      }

      $s = new IcesSerializer('CesBankBasicTransaction');
      $s->save($transaction);

      $this->notifyTransaction($transaction);
      return ($transaction->getState() == CesBankTransactionInterface::STATE_COMMITTED);

    }
    catch (Exception $e) {
      $t->rollback();
      throw $e;
    }
  }
  /**
   * Accept a transaction.
   *
   * After accept the transaction, it will be automatically applied.
   *
   * @param int $transaction_id
   *   The transaction must be already triggered.
   *
   * @param int $account_id
   *   Account id.
   *
   * @param bool $force
   *   If set to TRUE, the transaction will be accepted even if the account
   *   limits do not allow the transaction. Use this parameter only from
   *   administrative users.
   *
   * @return bool
   *   TRUE if the transaction has been successfully applied, FALSE otherwise.
   */
  public function acceptTransaction($transaction_id, $account_id, $force = FALSE) {
    $t = IcesSerializer::DBTransaction();
    try {
      // Get transaction. May be any type of transaction.
      $transaction = $this->getObject('CesBankBasicTransaction', $transaction_id);
      if (!$force) {
        $transaction->acceptByAccount($account_id);
      }
      else {
        $transaction->setState(CesBankTransactionInterface::STATE_ACCEPTED);
      }
      $s = new IcesSerializer('CesBankBasicTransaction');
      $s->save($transaction);

      return $this->applyTransaction($transaction_id);
    }
    catch (Exception $e) {
      $t->rollback();
      throw $e;
    }
  }
  /**
   * Reject a transaction.
   */
  public function rejectTransaction($transaction_id) {
    $t = IcesSerializer::DBTransaction();
    try {
      $transaction = $this->getObject('CesBankBasicTransaction', $transaction_id);
      $transaction->setState(CesBankTransactionInterface::STATE_REJECTED);
      $s = new IcesSerializer('CesBankBasicTransaction');
      $s->save($transaction);
      $this->notifyTransaction($transaction);
      return ($transaction->getState() == CesBankTransactionInterface::STATE_REJECTED);
    }
    catch (Exception $e) {
      $t->rollback();
      throw $e;
    }
  }
  /**
   * Get names of classes implementing CesBankDecoratedTransaction interface.
   *
   * @return array
   *   the names of classes that implement the CesBankDecoratedTransaction
   *   interface.
   */
  public function getCesBankDecoratedTransactionClasses() {
    $classes = module_invoke_all('decorated_transaction_classes');
    return $classes;
  }
  /**
   * Get the permission object record.
   * @return array
   *   the permission given its id.
   */
  public function getPermission($id) {
    return $this->getObjectRecord('CesBankPermission', $id);
  }
  /**
   * Create permission object.
   */
  public function createPermission(array &$record) {
    $t = IcesSerializer::DBTransaction();
    try {
      unset($record['id']);
      return $this->updateObject('CesBankPermission', $record);
    }
    catch (Exception $e) {
      $t->rollback();
      throw $e;
    }
  }
  /**
   * Delete permission given its id.
   *
   * @parm $id int
   *   The identifier of the permission object.
   */
  public function deletePermission($id) {
    $t = IcesSerializer::DBTransaction();
    try{
      $s = new IcesSerializer('CesBankPermission');
      $permission = $s->load($id);
      $s->delete($permission);
      return TRUE;
    }
    catch (Exception $e) {
      $t->rollback();
      throw $e;
    }
  }
  /**
   * Update permission.
   */
  public function updateCesBankPermission(array &$record) {
    $t = IcesSerializer::DBTransaction();
    try {
      return $this->updateObject('CesBankPermission', $record);
    }
    catch (Exception $e) {
      $t->rollback();
      throw $e;
    }
  }

  /**
   * Delete all permissions granted to a specific sope.
   *
   * When an object is deleted, all its associated permissions should be
   * deleted too and this function is a helper to do so.
   *
   * @param string $scope
   *   The name of the scope. It can be one of 'exchange', 'account' or 'user'.
   * @param int $scope_id
   *   The identfier of the scope object.
   */
  private function deletePermissionsToScope($scope, $scope_id) {
    $s = new IcesSerializer('CesBankPermission');
    $permissions = $s->loadCollection(
      array(
        'scope' => $scope,
        'scopeid' => $scope_id,
      )
    );
    foreach ($permissions as $permission) {
      $s->delete($permission);
    }
  }
  /**
   * Get the records with permissions for this user id.
   *
   * @param int $userid
   *   the user identifier.
   *
   * @return array
   *   The records to which the userid has permissions.
   */
  public function getAdministrablePermissions($userid = NULL) {
    $userperms = new CesBankUserPermission($userid);
    $permissions = $userperms->getPermissions();
    $result = array();
    foreach ($permissions as $perm) {
      $result[] = $perm->getRecord();
    }
    return $result;
  }
  /**
   * Formats the amount for an exchange.
   *
   * @param double $amount
   *   The amount to be formatted.
   * @param array $exchange
   *   The exchange where this amount applies.
   * @param bool $symbol
   *   Include the currency symbol.
   * @param bool $sign
   *   Include the positive sign.
   *
   * @return string
   *   A string representation of $amount following $exchange preference
   *   and $format option.
   */
  public function formatAmount($amount, $exchange, $symbol = FALSE, $sign = FALSE) {
    $decimals = $exchange['currencyscale'];
    $number = number_format($amount, $decimals, '.', ' ');
    if ($symbol) {
      $number .= $exchange['currencysymbol'];
    }
    if ($sign && $amount > 0) {
      $number = '+' . $number;
    }
    return $number;
  }
  /**
   * TODO: document.
   */
  protected function getObject($class_name, $id) {
    $s = new IcesSerializer($class_name);
    return $s->load($id);
  }
  /**
   * Get record in object based on id with given classname.
   */
  protected function getObjectRecord($class_name, $id) {
    $object = $this->getObject($class_name, $id);
    if ($object === FALSE) {
      throw new Exception(t('Object of type %classname with id %id does not exist.', array('%classname' => $class_name, '%id' => $id)));
    }
    return $object->getRecord();
  }
  /**
   * Update object in classname with array of records.
   */
  protected function updateObject($class_name, array &$record) {
    $s = new IcesSerializer($class_name);
    $object = $s->loadFromRecord($record);
    $s->save($object);
    if (isset($object->id)) {
      $record['id'] = $object->id;
    }
    return TRUE;
  }
  /**
   * Auxiliar.
   * @see newTransaction()
   */
  protected function transactionFormToRecord($record) {
    $record['state'] = CesBankTransactionInterface::STATE_NEW;
    // Load accounts.
    if (isset($record['toaccount'])) {
      $toaccount = $this->getAccount($record['toaccount']);
    }
    elseif (isset($record['toaccountname'])) {
      $toaccount = $this->getAccountByName($record['toaccountname']);
    }
    else {
      throw new Exception(t('Seller account not set.'));
    }
    if (isset($record['fromaccount'])) {
      $fromaccount = $this->getAccount($record['fromaccount']);
    }
    elseif (isset($record['fromaccountname'])) {
      $fromaccount = $this->getAccountByName($record['fromaccountname']);
    }
    else {
      throw new Exception(t('Seller account not set.'));
    }
    if ($toaccount === FALSE) {
      throw new Exception(t('Error loading seller account.'));
    }
    if ($fromaccount === FALSE) {
      throw new Exception(t('Error loading buyer account.'));
    }
    // Set up account id's.
    $record['toaccount'] = $toaccount['id'];
    $record['fromaccount'] = $fromaccount['id'];
    if (!isset($record['data'])) {
      $record['data'] = array();
    }
    return $record;
  }
  /**
   * Creates a new transaction object based on the record array.
   * @return CesBankTransactionInterface
   *   New transaction object based on the record array.
   */
  protected function newTransaction(array &$record) {
    unset($record['id']);
    $record = $this->transactionFormToRecord($record);
    $fromaccount = $this->getObject('CesBankLocalAccount', $record['fromaccount']);
    $toaccount = $this->getObject('CesBankLocalAccount', $record['toaccount']);
    // Basic transaction. Within one exchange.
    if ($toaccount->exchange == $fromaccount->exchange) {
      $transaction = new CesBankBasicTransaction($record);
    }
    else {
      // Complex transaction interexchange.
      $transaction = CesBankInterExchangeTransaction::newCesBankInterExchangeTransaction($record);
    }
    return $transaction;
  }
  /**
   * Notifies the applied transaction.
   *
   * Sends a message to both buyer and seller noticing that their balances have
   * changed.
   *
   * @see CesBank::notifyTransactionAccount()
   */
  protected function notifyTransaction(CesBankTransactionInterface $transaction) {
    $account_from_name = $transaction->getFromAccountName();
    $account_to_name = $transaction->getToAccountName();
    $details = $transaction->getLog($transaction->getState());
    $params = array(
      'transaction' => array(
        'id' => $transaction->getId(),
        'amount' => $transaction->getAmount(),
        'concept' => $transaction->getConcept(),
        'fromaccount' => $account_from_name,
        'toaccount' => $account_to_name,
        'details' => $details,
        'user' => $transaction->getUser(),
      ),
    );
    switch ($transaction->getState()) {
      case CesBankTransactionInterface::STATE_COMMITTED:
        $this->notifyTransactionAccount('account_debited', $account_from_name, $params);
        $this->notifyTransactionAccount('account_credited', $account_to_name, $params);
        break;

      case CesBankTransactionInterface::STATE_REJECTED:
        $this->notifyTransactionAccount('account_debit_rejected', $account_from_name, $params);
        $this->notifyTransactionAccount('account_credit_rejected', $account_to_name, $params);
        break;

      case CesBankTransactionInterface::STATE_TRIGGERED:
        $this->notifyTransactionAccount('account_debit_pending', $account_from_name, $params);
        $this->notifyTransactionAccount('account_credit_pending', $account_to_name, $params);
        break;
    }
  }

  /**
   * Triggers notification for transaction account.
   */
  protected function notifyTransactionAccount($message_key, $account_name, $params) {
    $s = new IcesSerializer('CesBankLocalAccount');
    $account = $s->loadFromUniqueKey('name', $account_name);
    $account_users = $account->getUsers();
    global $user;
    foreach ($account_users as $account_user) {
      // Don't send mail if the user is the current one.
      $send_mail = ($user->uid != $account_user->user) ? TRUE : FALSE;
      ces_message_send_notify($message_key, $params, array($account_user->user), $user->uid, $send_mail);
    }
  }
}

/**
 * An exchange community.
 */
class CesBankExchange extends IcesDBObject {
  public static $table = 'ces_exchange';
  /**
   * @var int
   */
  public $id;
  /**
   * @var string
   */
  public $code;
  /**
   * @var int
   */
  public $state;
  /**
   * @var Currency
   */
  protected $currency;
  /**
   * @var int
   */
  public $limitchain;
  /**
   * @var string
   */
  public $shortname;
  /**
   * @var string
   */
  public $name;
  /**
   * @var string
   */
  public $country;
  /**
   * @var string
   */
  public $region;
  /**
   * @var string
   */
  public $town;
  /**
   * @var string
   */
  public $website;
  /**
   * @var string
   */
  public $map;
  /**
   * Administrator user uid.
   * @var int
   */
  public $admin;
  /**
   * @var int
   */
  public $created;
  /**
   * @var int
   */
  public $modified;
  /**
   * @var array
   */
  public $data = array();

  const DEFAULT_EXCHANGE_ID = 1;
  const STATE_INACTIVE = 0;
  const STATE_ACTIVE = 1;
  /**
   * Loads record.
   */
  public function loadRecord($record) {
    parent::loadRecord($record);
    $this->currency = new CesBankCurrency($record);
  }
  /**
   * Gets record.
   */
  public function getRecord() {
    $record = parent::getRecord();
    $record = array_merge($record, $this->currency->getRecord());
    unset($record['currency']);
    return $record;
  }
  /**
   * Returns a default exchange as model for new exchanges.
   *
   * @return CesBankExchange
   *   The default exchange. To be used as a model for new
   *   exchanges.
   */
  public static function getDefaultExchange() {
    $record = array(
      'id' => 0,
      'code' => 'XCHG',
      'active' => 0,
      'shortname' => 'Xchange',
      'name' => 'Xchange Default',
      'country' => 'Spain',
      'region' => 'Barcelonès',
      'town' => 'Barcelona',
      'website' => 'http://www.example.com',
      'map' => 'https://maps.google.com/maps?q=barcelona,+spain',
      'admin' => 1,
      'limitchain' => 1,
      'currencysymbol' => 'XCR',
      'currencyname' => 'Xcredit',
      'currenciesname' => 'Xcredits',
      'currencyvalue' => '1',
      'currencyscale' => 2,
      'created' => REQUEST_TIME,
      'modified' => REQUEST_TIME,
      'data' => array(),
    );
    $s = new IcesSerializer('CesBankExchange');
    $exchange = $s->loadFromRecord($record);
    return $exchange;
  }

  /**
   * Returns a default, hidden CesBankLocalAccount as model for new accounts.
   *
   * @return CesBankLocalAccount
   *   Account to be used as a model for new accounts.
   */
  public function getDefaultAccount() {
    $record = array(
      'exchange' => $this->id,
      'name' => $this->getFreeAccountName(),
      'balance' => 0.0,
      'state' => CesBankLocalAccount::STATE_HIDDEN,
      'kind' => CesBankLocalAccount::TYPE_INDIVIDUAL,
      'limitchain' => $this->limitchain,
      'users' => array(
        array(
          'account' => NULL,
          'user' => 0,
          'role' => CesBankAccountUser::ROLE_ACCOUNT_ADMINISTRATOR,
        ),
      ),
    );
    $s = new IcesSerializer('CesBankLocalAccount');
    $account = $s->loadFromRecord($record);
    return $account;
  }
  /**
   * TODO: Document.
   * @see getDefaultAccount()
   */
  protected function getFreeAccountName() {
    // NOTE: this function is highly inefficient. It can be changed with a
    // single specific DB query if causes performance problems.
    $s = new IcesSerializer('CesBankLocalAccount');
    $accounts = $s->loadCollection(array('exchange' => $this->id, 'kind' => array(0, 1, 2, 3, 4)), 'name');
    $start = floor((count($accounts) - 1) / 2);
    // This is a bit tricky.
    $code = !empty($accounts) ? $accounts[$start]->getCode() : 0;
    if (!is_numeric($code)) {
      $code = 0;
    }
    for ($i = $start + 1; $i < count($accounts); $i++) {
      $acccode = $accounts[$i]->getCode();
      if (is_numeric($acccode)) {
        if ($acccode - $code > 1) {
          break;
        }
        else {
          $code = $acccode;
        }
      }
    }
    $code = '' . ($code + 1);
    while (drupal_strlen($code) < 4) {
      $code = '0' . $code;
    }
    return $this->code . $code;
  }
  /**
   * Activates a newly created exchange, with new structures required.
   *
   * Creates all the additional structures an exchange needs in order
   * to be operative. This includes:
   *  - default limit chain
   * And sets up the active flag to this exchange.
   * Exchange needs to be saved after this call so the state flag persists.
   */
  public function activate() {
    $es = new IcesSerializer('CesBankExchange');
    // Setup default limit chain.
    if (empty($this->limitchain)) {
      // Create default limit chain.
      $record = array(
        'exchange' => $this->id,
        'name' => 'default',
      );
      $ls = new IcesSerializer('CesBankLimitChain');
      $def_limit_chain = $ls->loadFromRecord($record);
      $ls->save($def_limit_chain);
      // Update this exchange with the default limit chain.
      $this->limitchain = $def_limit_chain->id;
      $es->updateField($this, 'limitchain');
    }
    $this->state = CesBankExchange::STATE_ACTIVE;
  }
  /**
   * Get virtual account for this exchange, or create if needed.
   *
   * @return Account
   *   the virtual account for this exchange. It is created if it
   *   doesn't exist.
   * @todo notify exchange admin in case of virtual user creation.
   */
  public function getVirtualAccount(CesBankExchange $exchange) {
    $s = new IcesSerializer('CesBankLocalAccount');
    $name = $this->code . $exchange->code;
    $virt_account = $s->loadFromUniqueKey('name', $name);
    if ($virt_account === FALSE) {
      $record = array(
        'id' => NULL,
        'exchange' => $this->id,
        'name' => $name,
        'balance' => 0.0,
        'state' => CesBankLocalAccount::STATE_HIDDEN,
        'kind' => CesBankLocalAccount::TYPE_VIRTUAL,
        'limitchain' => $this->limitchain,
        'users' => array(
          array(
            'account' => NULL,
            'user' => 1,
            'role' => CesBankAccountUser::ROLE_ACCOUNT_ADMINISTRATOR,
          ),
        ),
      );
      // This dependence is a bit weird. Isn't it?
      $bank = new CesBank();
      $bank->createAccount($record);
      $virt_account = new CesBankLocalAccount($record);
    }
    return $virt_account;
  }
  /**
   * Get currency of this exchange.
   * @return CesBankCurrency
   *   This exchange's currency.
   * @see CesBankCurrency
   */
  public function getCurrency() {
    return $this->currency;
  }
}
/**
 * A currency. A currency belongs to a unique Exchange.
 */
class CesBankCurrency extends IcesDBObject {
  /**
   * @var string
   */
  public $currencysymbol;
  /**
   * @var string
   */
  public $currencyname;
  /**
   * @var string
   */
  public $currenciesname;
  /**
   * @var double
   */
  public $currencyvalue;
  /**
   * @var int
   */
  public $currencyscale;
  /**
   * Converts amount from a currency into a new amount.
   * @see CesBankBasicTransaction
   * @return float
   *   the amount in this currency of the value represented by the
   *   two given parameters.
   *
   * @param float $amount
   *   the amount to be converted.
   * @param CesBankCurrency $currency
   *   the original currency.
   */
  public function convert($amount, CesBankCurrency $currency) {
    $rate = ($currency->currencyvalue / $this->currencyvalue);
    $newamount = $amount * $rate;
    return $newamount;
  }
}

/**
 * Account interface names basic methods for an account applying and checking
 * transactions.
 */
interface CesBankAccountInterface {
  /**
   * In most implementations, gets name of account holder.
   */
  public function getName();
  /**
   * Apply transaction.
   * @see CesBankLocalAccount
   */
  public function applyTransaction(CesBankTransactionInterface &$transaction);
  /**
   * Check transaction.
   * @see CesBankLocalAccount
   */
  public function checkTransaction(CesBankTransactionInterface &$transaction);
}
/**
 * CesBankLocalAccount is a banking account. Implements CesBankAccountInterface.
 */
class CesBankLocalAccount extends IcesDBObject implements CesBankAccountInterface {
  /**
   * @var int
   */
  public $id;
  /**
   * @var int
   */
  public $exchange;
  /**
   * @var string
   */
  public $name;
  /**
   * @var double
   */
  public $balance;
  /**
   * @var int
   */
  public $limitchain;
  /**
   * @var int
   */
  public $created;
  /**
   * @var int
   */
  public $modified;
  /**
   * @var int
   */
  public $kind;
  /**
   * @var int
   */
  public $state;
  /**
   * @var array
   */
  public $data = array();
  /**
   * @var array of users related to this account. Tipically this array is a
   * singleton with the account's administrator user. Each entry is an
   * CesBankAccountUser object.
   */
  protected $users;
  public static $table = 'ces_account';
  const STATE_HIDDEN = 0;
  const STATE_ACTIVE = 1;
  const STATE_LOCKED = 2;
  const STATE_CLOSED = 3;
  const TYPE_INDIVIDUAL = 0;
  const TYPE_SHARED = 1;
  const TYPE_ORGANIZATION = 2;
  const TYPE_COMPANY = 3;
  const TYPE_PUBLIC = 4;
  const TYPE_VIRTUAL = 5;
  /**
   * LoadRecord loads loadRecord function from the parent IcesDBObject.
   */
  public function loadRecord($record) {
    parent::loadRecord($record);
    if (isset($record['users']) && is_array($record['users'])) {
      $this->users = array();
      foreach ($record['users'] as $accuser) {
        $this->users[] = new CesBankAccountUser($accuser);
      }
    }
    else {
      $s = new IcesSerializer('CesBankAccountUser');
      $this->users = $s->loadCollection(array('account' => $this->id));
    }
    // Default value for manual accept payments.
    if (empty($this->data['accept'])) {
      $this->data['accept'] = array(
        'manual' => FALSE,
        'whitelist' => array(),
      );
    }
  }
  /**
   * GetRecord loads getRecord function from the parent IcesDBObject.
   */
  public function getRecord() {
    $record = parent::getRecord();
    $record['users'] = array();
    foreach ($this->users as $useraccount) {
      $uarecord = $useraccount->getRecord();
      $record['users'][$uarecord['user']] = $uarecord;
    }
    return $record;
  }
  /**
   * Saves extra data.
   * @see CesBank::createAccount()
   */
  public function saveExtraData() {
    if (!empty($this->id)) {
      foreach ($this->users as $accuser) {
        $accuser->account = $this->id;
        $s = new IcesSerializer('CesBankAccountUser');
        $s->save($accuser);
      }
    }
  }
  /**
   * Delete extra data.
   * @see CesBank::deleteAccount()
   */
  public function deleteExtraData() {
    $s = new IcesSerializer('CesBankAccountUser');
    foreach ($this->users as $accuser) {
      $s->delete($accuser);
    }
  }
  /**
   * Delete user account.
   */
  public function deleteUserAccount($id) {
    $s = new IcesSerializer('CesBankAccountUser');
    foreach ($this->users as $accuser) {
      if ($accuser->user == $id) {
        $s->delete($accuser);
      }
    }
  }
  /**
   * Gets limit chain.
   * @return CesBankLimitChain
   *   The limit chain associated to this account.
   */
  public function getLimitChain() {
    $s = new IcesSerializer('CesBankLimitChain');
    return $s->load($this->limitchain);
  }
  /**
   * Updates this account's amount with this transaction if accepted.
   *
   * If it is accepted, the updated account is safely saved to the DB.
   */
  public function applyTransaction(CesBankTransactionInterface &$transaction) {
    $s = new IcesLockSerializer('CesBankLocalAccount');
    // Work with updated data and lock this account in database.
    $account = $s->load($this->id);
    $amount = $transaction->getAmount();
    if ($transaction->getFromAccountName() == $this->name) {
      $account->balance = $account->balance - $amount;
    }
    if ($transaction->getToAccountName() == $this->name) {
      $account->balance = $account->balance + $amount;
    }
    $this->balance = $account->balance;
    // Update actual object amount because it could be reused.
    $s->updateField($account, 'balance');
  }
  /**
   * Verifies if an account can accept a transaction by being active.
   *
   * @return bool|array
   *   TRUE if the transaction is accepted or an array with messages why the
   *   transaction is not accepted.
   */
  public function checkTransaction(CesBankTransactionInterface &$transaction) {
    if ($this->state == CesBankLocalAccount::STATE_ACTIVE) {
      return $this->getLimitChain()->checkTransaction($this, $transaction);
    }
    else {
      return array(t('User %user cannot accept transactions because it is not active.', array('%user' => $this->name)));
    }
  }
  /**
   * Changes local account state to active.
   */
  public function activate() {
    $this->state = CesBankLocalAccount::STATE_ACTIVE;
  }
  /**
   * Gets name of account holder.
   */
  public function getName() {
    return $this->name;
  }
  /**
   * TODO: document.
   */
  public function getCode() {
    return drupal_substr($this->name, 4);
  }
  /**
   * TODO: document.
   */
  public function &getUsers() {
    return $this->users;
  }
  /**
   * Get the Exchange of this account.
   *
   * @return CesBankExchange
   *   the exchange object to which this account belongs.
   */
  public function getExchange() {
    $es = new IcesSerializer('CesBankExchange');
    return $es->load($this->exchange);
  }

  /**
   * Check whether the transaction must be accepted by this account.
   *
   * This account must be the buyer account of the transaction parameter.
   *
   * @return bool
   *   TRUE if the transaction is manually accepted by this account and hence
   *   may be immediately applied, FALSE if the transaction has to be manually
   *   accepted.
   */
  public function manualCheck($transaction) {
    if ($this->data['accept']['manual']) {
      $toaccount = $transaction->getToAccountName();
      if (!in_array($toaccount, $this->data['accept']['whitelist'])) {
        return array(t('The transaction must be manually authorized.'));
      }
    }
    return TRUE;
  }

}
  /**
   * TODO: document. Class CesBankAccountUser.
   */
class CesBankAccountUser extends IcesDBObject {
  public static $table = 'ces_accountuser';
  public static $primaryKey = array('user', 'account');
  public $id;
  public $user;
  public $account;
  public $privilege;
  public $role;
  const ROLE_ACCOUNT_ADMINISTRATOR = 0;
}
/**
 * A chain of account limits. It is used by account.
 */
class CesBankLimitChain extends IcesDBObject {
  public static $table = 'ces_limitchain';
  /**
   * @var int
   */
  public $id;
  /**
   * @var int
   */
  public $exchange;
  /**
   * @var String
   */
  public $name;
  const DEFAULT_LIMITCHAIN_ID = 1;
  /**
   * @var array of CesBankAccountLimit
   */
  protected $limits;
  /**
   * TODO: Document.
   * @override
   */
  public function loadRecord($record) {
    // Load fields.
    parent::loadRecord($record);
    // Load limits.
    $this->limits = array();
    $s = new IcesSerializer('CesBankAccountLimit');
    if (isset($record['limits'])) {
      // Record comes with limits. This is because it comes from UI.
      foreach ($record['limits'] as $limit_record) {
        $limit_record['limitchain'] = $this->id;
        $this->limits[] = $s->loadFromRecord($limit_record);
      }
    }
    else {
      $this->limits = $s->loadCollection(array('limitchain' => $this->id));
    }
  }
  /**
   * TODO: Document.
   */
  public function getRecord() {
    $record = parent::getRecord();
    $record['limits'] = array();
    foreach ($this->limits as $limit) {
      $record['limits'][] = $limit->getRecord();
    }
    return $record;
  }
  /**
   * Saves instances of CesBankAccountLimit that belong to CesBankLimitChain.
   *
   * @override
   */
  public function saveExtraData() {
    $s = new IcesSerializer('CesBankAccountLimit');
    $newids = array();
    // create/update limits.
    foreach ($this->limits as $limit) {
      if (!isset($limit->data)) {
        $limit->data = array();
      }
      $limit->limitchain = $this->id;
      $s->save($limit);
      $newids[] = $limit->id;
    }
    // Delete unused limits.
    // This is inefficient (it could be done in a single DB query),
    // but it doesn't matter since this is a very occasional operation.
    $limits = $s->loadCollection(array('limitchain' => $this->id));
    $allids = array();
    foreach ($limits as $limit) {
      $allids[] = $limit->id;
    }
    $oldids = array_diff($allids, $newids);
    foreach ($oldids as $id) {
      $s->delete($limits[$id]);
    }
  }
  /**
   * Delets associated CesBankAccountLimit.
   */
  public function deleteExtraData() {
    $s = new IcesSerializer('CesBankAccountLimit');
    foreach ($this->limits as $limit) {
      $s->delete($limit);
    }
  }
  /**
   * TODO: document.
   */
  public function checkAccount(CesBankLocalAccount &$account) {
    $messages = array();
    foreach ($this->limits as $limit) {
      if ($limit->checkAccount($account) !== TRUE) {
        $messages[] = $limit->getFailMessage($account);
      }
    }
    if (empty($messages)) {
      return TRUE;
    }
    else {
      return $messages;
    }
  }
  /**
   * Check whether the transaction can be applied.
   *
   * @return bool|array
   *   TRUE if transaction is accepted, or an array of messages why the
   *   transaction has not been accepted.
   */
  public function checkTransaction(CesBankLocalAccount &$account, CesBankTransactionInterface &$transaction) {
    $messages = array();
    foreach ($this->limits as $limit) {
      if (!$limit->checkTransaction($account, $transaction)) {
        $messages[] = $limit->getFailMessage($account);
      }
    }
    if (empty($messages)) {
      return TRUE;
    }
    else {
      return $messages;
    }
  }
  /**
   * TODO: document.
   */
  public function getCesBankAccountLimits() {
    return $this->limits;
  }
}
/**
 * Generic class for account limits. Each account limit limites one aspect of
 * accounts.
 */
abstract class CesBankAccountLimit extends IcesDBObject {
  public static $table = 'ces_limit';
  /**
   * @var int
   */
  public $id;
  /**
   * @var double
   */
  public $value;
  /**
   * @var boolean
   *
   * If so, blocks the transaction
   */
  public $block;
  /**
   * @var int
   */
  public $limitchain;
  /**
   * @var string
   */
  public $classname;
  /**
   * @var array
   */
  public $data;
  /**
   * Todo: document.
   * @return bool
   *   The state of this account against this CesBankAccountLimit. TRUE
   *   for pass and FALSE for fail.
   */
  public function checkAccount(CesBankLocalAccount &$account) {
    return ($this->getAccountValue($account) > $this->getLimit());
  }
  /**
   * Check if the transaction is accepted by this limit.
   *
   * @return bool
   *   TRUE if transaction is accepted and FALSE if not.
   */
  public function checkTransaction(CesBankLocalAccount &$account, CesBankTransactionInterface &$transaction) {
    if (!$this->block) {
      return TRUE;
    }
    // Non-blocking limits always approve transactions.
    else {
      $value = $this->getAccountValue($account);
      $newvalue = $value;
      $transaction_value = $this->getTransactionValue($transaction);
      if ($account->getName() == $transaction->getFromAccountName()) {
        $newvalue -= $transaction_value;
      }
      elseif ($account->getName() == $transaction->getToAccountName()) {
        $newvalue += $transaction_value;
      }
      if ($newvalue >= $this->getLimit()) {
        // Account is ok after transaction.
        return TRUE;
      }
      elseif ($newvalue >= $value) {
        // Account is not ok after transaction but improves its mark.
        return TRUE;
      }
      else {
        // Block transaction.
        return FALSE;
      }
    }
  }
  /**
   * TODO: document.
   */
  public function getLimit() {
    return $this->value;
  }
  /**
   * TODO: document.
   * @return double
   *   The amount of this account.
   */
  public abstract function getAccountValue(CesBankLocalAccount &$account);
  /**
   * TODO: document.
   * @return double
   *   The amount of this transaction.
   */
  public abstract function getTransactionValue(CesBankTransactionInterface &$transaction);
  /**
   * TODO: document.
   */
  public abstract function getFailMessage(CesBankLocalAccount &$account);
}
/**
 * Limits an account if its balance is less than a given value.
 */
class CesBankAbsoluteDebitLimit extends CesBankAccountLimit {
  /**
   * TODO: document.
   */
  public function getAccountValue(CesBankLocalAccount &$account) {
    return $account->balance;
  }
  /**
   * TODO: document.
   */
  public function getTransactionValue(CesBankTransactionInterface &$transaction) {
    return $transaction->getAmount();
  }
  /**
   * TODO: document.
   */
  public function getFailMessage(CesBankLocalAccount &$account) {
    return t('Account %account exceeds the debit limit of %limit.', array('%limit' => $this->getLimit(), '%account' => $account->getName()));
  }
}
/**
 * Limits an account if its balance is more than a given value.
 */
class CesBankAbsoluteCreditLimit extends CesBankAccountLimit {
  /**
   * TODO: document.
   */
  public function getAccountValue(CesBankLocalAccount &$account) {
    return -$account->balance;
  }
  /**
   * TODO: document.
   */
  public function getTransactionValue(CesBankTransactionInterface &$transaction) {
    return -$transaction->getAmount();
  }
  /**
   * TODO: document.
   */
  public function getLimit() {
    return -$this->value;
  }
  /**
   * TODO: document.
   */
  public function getFailMessage(CesBankLocalAccount &$account) {
    return t('Account %account exceeds the credit limit of %limit.', array('%limit' => $this->value, '%account' => $account->getName()));
  }
}
/**
 * Transaction interface
 *
 *
 * Transactions
 * ============
 * A transaction is an amount of money to be transferred from one account to
 * another account. The basic transaction moves certanin quantity of money
 * through two accounts within the same exchange. There are other kind of
 * transactions moving currency between servers or third party applications. In
 * each exchange there is an special account called 'virtual account'. This
 * account acts as the foreign account for all inter-exchange transactions. It
 * is also used to temporally save the money in delayed transactions.
 */
interface CesBankTransactionInterface {

  /**
   * Get the identifier of this transaction.
   *
   * @return int
   *   The identifier of this transaction.
   */
  public function getId();
  /**
   * Get the amount of this transaction, in the seller's exchange currency.
   *
   * @return float
   *   The currency amount of this transaction.
   */
  public function getAmount(CesBankCurrency $currency = NULL);
  /**
   * Get the user id that has created this transaction.
   *
   * @return int
   *   The id of the user who has created this transaction.
   */
  public function getUser();
  /**
   * Get the short description of this transaction.
   *
   * @return string
   *   The short description of this transaction.
   */
  public function getConcept();
  /**
   * Get the name of the buyer account.
   *
   * @return string
   *   The buyer account public ID
   */
  public function getFromAccountName();
  /**
   * Get the name of the seller account.
   *
   * @return string
   *   The seller account public ID
   */
  public function getToAccountName();
  /**
   * Get the state of this transaction.
   *
   * @return int
   *   The current state of this transaction.
   */
  public function getState();
  /**
   * Get messages logged for this transaction.
   *
   * @return array
   *   Array of messages with details about given $state.
   */
  public function getLog($state);
  /**
   * Get the time the transaction was created.
   *
   * @return int
   *   The timestamp of the transaction creation.
   */
  public function getCreated();
  /**
   * Trigger this transaction.
   */
  public function trigger();
  /**
   * Check this transaction.
   */
  public function check();
  /**
   * Apply this transaction.
   */
  public function apply();
  /**
   * Revoke this transaction.
   */
  public function revoke();
  /**
   * Set that the account accepted this transaction.
   */
  public function acceptByAccount($account_id);

  /**
   * The transaction has been created but hasn't already been triggered to
   * apply. This is the state while the transaction can be edited. Waiting
   * for the user to apply this transaction.
   */
  const STATE_NEW = 0;
  /**
   * The transaction has been triggered to be applied. Waiting for acceptance
   * from all authorities.
   */
  const STATE_TRIGGERED = 1;
  /**
   * The transaction has been accepted by all authorities. It is being
   * effectively applied by the system. This state is short-lived in local
   * transactions.
   */
  const STATE_ACCEPTED = 2;
  /**
   * The transaction has been successfully applied. It can however be revoked.
   * If there aren't issues for this transaction after a prudencial time, it
   * will be archieved.
   */
  const STATE_COMMITTED = 3;
  /**
   * The transaction is archieved and successfully applied. Nothing can be done
   * with it. It exists only as historical data.
   */
  const STATE_ARCHIVED = 4;
  /**
   * The transaction -triggered to apply- has been rejected by some of the
   * authorities. It will be discarded in some days if nobody changes their
   * opinion.
   */
  const STATE_REJECTED = 5;
  /**
   * The transaction is discarded and has not been applied. Nothing can be done
   * with it. It exists only as historical data.
   */
  const STATE_DISCARDED = 6;
  /**
   * The transaction has been triggered to be revoked. Waiting for acceptance of
   * this operation by all authorities.
   */
  const STATE_REVOKE_TRIGGERED = 7;
  /**
   * The revoke operation has been accepted by all authorities. The system is
   * effectively doing the revoke. Short-lived state for local transactions.
   */
  const STATE_REVOKE_ACCEPTED = 8;
  /**
   * Some authorities rejected the revoke operation, so the transaction will
   * remain applied.
   */
  const STATE_REVOKE_REJECTED = 9;
  /**
   * The transaction has been successfully revoked. Marked to be discarded after
   * a prudential time.
   */
  const STATE_REVOKED = 10;
  /**
   * There has been a system error with this transaction. It requires
   * administrator attention.
   */
  const STATE_ERROR = 11;

}
/**
 * Concrete class for the basic transaction. It can be enhanced using
 * DecoratorTransactions.
 */
class CesBankBasicTransaction extends IcesDBObject implements CesBankTransactionInterface {
  /**
   * @var int
   *   Transaction identifier.
   */
  public $id;
  /**
   * @var int
   *   Buyer account identifier.
   */
  public $fromaccount;
  /**
   * @var int
   *   Seller account identifier.
   */
  public $toaccount;
  /**
   * @var float
   *   Transaction amount.
   */
  public $amount;
  /**
   * @var int
   *   Transaction creator.
   */
  public $user;
  /**
   * @var string
   *   Short description.
   */
  public $concept;
  /**
   * @var int
   *   State.
   */
  public $state;
  /**
   * @var int
   *   Date.
   */
  public $created;
  /**
   * @var int
   *   Date.
   */
  public $modified;
  /**
   * @var array
   */
  public $data;
  public static $table = 'ces_transaction';
  /**
   * TODO: document.
   */
  public function getId() {
    return $this->id;
  }
  /**
   * TODO: document.
   *
   * @return float
   *   the amount of this transaction in the given currency.
   *
   * @param CesBankCurrency $currency
   *   The currency to be used. If omitted it returns the value
   *   in the seller's exchange currency.
   */
  public function getAmount(CesBankCurrency $currency = NULL) {
    if ($currency != NULL) {
      // Get current (seller's) currency.
      $as = new IcesSerializer('CesBankLocalAccount');
      $toaccount = $as->load($this->toaccount);
      return $currency->convert($this->amount, $toaccount->getExchange()->getCurrency());
    }
    else {
      return $this->amount;
    }
  }
  /**
   * TODO: document.
   */
  public function getUser() {
    return $this->user;
  }
  /**
   * TODO: document.
   */
  public function getConcept() {
    return $this->concept;
  }
  /**
   * TODO: document.
   */
  public function getFromAccountName() {
    $s = new IcesSerializer('CesBankLocalAccount');
    $acc = $s->load($this->fromaccount);
    return $acc->name;
  }
  /**
   * TODO: document.
   */
  public function getToAccountName() {
    $s = new IcesSerializer('CesBankLocalAccount');
    $acc = $s->load($this->toaccount);
    return $acc->name;
  }
  /**
   * TODO: document.
   */
  public function getState() {
    return $this->state;
  }
  /**
   * TODO: document.
   */
  public function log(array $messages) {
    if (empty($messages)) {
      return;
    }
    if (!isset($this->data)) {
      $this->data = array();
    }
    if (!isset($this->data['log'])) {
      $this->data['log'] = array();
    }
    if (!isset($this->data['log'][$this->state])) {
      $this->data['log'][$this->state] = array();
    }
    $this->data['log'][$this->state] = array_merge($this->data['log'][$this->state], $messages);
  }
  /**
   * TODO: document.
   */
  public function getLog($state) {
    if (isset($this->data) && isset($this->data['log']) && isset($this->data['log'][$state])) {
      return $this->data['log'][$state];
    }
    else {
      return array();
    }
  }
  /**
   * Changes the transaction state.
   */
  public function setState($state) {
    if ($this->state != $state) {
      $flow = array(
        CesBankTransactionInterface::STATE_NEW => array(CesBankTransactionInterface::STATE_TRIGGERED, CesBankTransactionInterface::STATE_DISCARDED),
        CesBankTransactionInterface::STATE_TRIGGERED => array(CesBankTransactionInterface::STATE_ACCEPTED, CesBankTransactionInterface::STATE_REJECTED),
        CesBankTransactionInterface::STATE_ACCEPTED => array(CesBankTransactionInterface::STATE_COMMITTED, CesBankTransactionInterface::STATE_ERROR),
        CesBankTransactionInterface::STATE_REJECTED => array(CesBankTransactionInterface::STATE_TRIGGERED, CesBankTransactionInterface::STATE_DISCARDED),
        CesBankTransactionInterface::STATE_COMMITTED => array(CesBankTransactionInterface::STATE_ARCHIVED, CesBankTransactionInterface::STATE_REVOKE_TRIGGERED),
        CesBankTransactionInterface::STATE_REVOKE_TRIGGERED => array(CesBankTransactionInterface::STATE_REVOKE_ACCEPTED, CesBankTransactionInterface::STATE_REVOKE_REJECTED),
        CesBankTransactionInterface::STATE_REVOKE_ACCEPTED => array(CesBankTransactionInterface::STATE_REVOKED, CesBankTransactionInterface::STATE_ERROR),
        CesBankTransactionInterface::STATE_REVOKE_REJECTED => array(CesBankTransactionInterface::STATE_REVOKE_TRIGGERED),
        CesBankTransactionInterface::STATE_REVOKED => array(CesBankTransactionInterface::STATE_DISCARDED, CesBankTransactionInterface::STATE_TRIGGERED),
      );
      $candidates = $flow[$this->state];
      if (!in_array($state, $candidates)) {
        throw new Exception(t('Invalid state transition for transaction %trans.', array('%trans' => $this->id)));
      }
      $this->state = $state;
    }
  }
  /**
   * TODO: document.
   */
  public function trigger() {
    $this->setState(CesBankTransactionInterface::STATE_TRIGGERED);
  }
  /**
   * Checks if this transaction can be applied and updates the state.
   *
   * The effect of this function can be this transaction to be accepted,
   * rejected or nothing (if the transaction cannot be checked immediately due
   * to the need of human intervention).
   */
  public function check() {
    $s = new IcesSerializer('CesBankLocalAccount');
    $account_from = $s->load($this->fromaccount);
    $account_to = $s->load($this->toaccount);
    $messages_from = $account_from->checkTransaction($this);
    $messages_to   = $account_to->checkTransaction($this);
    if ($messages_from === TRUE && $messages_to === TRUE) {
      // Check manual accept.
      if (empty($this->data['authorized_from'])) {
        $messages_from = $account_from->manualCheck($this);
      }
      if ($messages_from === TRUE) {
        $this->setState(CesBankTransactionInterface::STATE_ACCEPTED);
      }
      else {
        $this->log($messages_from);
      }
    }
    else {
      $this->setState(CesBankTransactionInterface::STATE_REJECTED);
      if ($messages_from === TRUE) {
        $messages = $messages_to;
      }
      elseif ($messages_to === TRUE) {
        $messages = $messages_from;
      }
      else {
        $messages = array_merge($messages_from, $messages_to);
      }
      $this->log($messages);
    }
  }
  /**
   * Applies, or commits, a basic transaction, or sets state to error.
   * @TODO: document.
   */
  public function apply() {
    // Load account objects.
    $s = new IcesSerializer('CesBankLocalAccount');
    $account_from = $s->load($this->fromaccount);
    $account_to = $s->load($this->toaccount);
    // Apply transaction to both accounts.
    try {
      $account_to->applyTransaction($this);
      $account_from->applyTransaction($this);
      $this->setState(CesBankTransactionInterface::STATE_COMMITTED);
      return TRUE;
    }
    catch (Exception $e) {
      $this->setState(CesBankTransactionInterface::STATE_ERROR);
      $this->log(array($e->getMessage()));
      throw $e;
    }
  }
  /**
   * Revokes a basic transaction from both accounts.
   * @TODO: document.
   */
  public function revoke() {
    // Load account objects.
    $s = new IcesSerializer('CesBankLocalAccount');
    $account_from = $s->load($this->fromaccount);
    $account_to = $s->load($this->toaccount);
    // Revoke transaction from both accounts.
    try {
      $this->setState(CesBankTransactionInterface::STATE_REVOKE_TRIGGERED);
      $this->amount = -$this->amount;
      $account_to->applyTransaction($this);
      $account_from->applyTransaction($this);
      $this->amount = -$this->amount;
      // Go through the three necessary workflow states.
      $this->setState(CesBankTransactionInterface::STATE_REVOKE_ACCEPTED);
      $this->setState(CesBankTransactionInterface::STATE_REVOKED);
      return TRUE;
    }
    catch (Exception $e) {
      $this->setState(CesBankTransactionInterface::STATE_ERROR);
      $this->log(array($e->getMessage()));
      throw $e;
    }
  }
  /**
   * TODO: document.
   */
  protected static function getStateString($state) {
    $string = array(
      0 => 'NEW',
      1 => 'TRIGGERED',
      2 => 'ACCEPTED',
      3 => 'COMMITTED',
      4 => 'ARCHIVED',
      5 => 'REJECTED',
      6 => 'DISCARDED',
      7 => 'REVOKE TRIGGERED',
      8 => 'REVOKE ACCEPTED',
      9 => 'REVOKE REJECTED',
      10 => 'REVOKED',
      11 => 'ERROR',
    );
    return $string[$state];
  }
  /**
   * Gets the creation timestamp.
   */
  public function getCreated() {
    return $this->created;
  }
  /**
   * Implements CesBankTransactionInterface::acceptByAccount().
   */
  public function acceptByAccount($account_id) {
    if ($this->fromaccount == $account_id) {
      $this->data['authorized_from'] = TRUE;
    }
  }
}
/**
 * Abstract class for transaction decorators.
 */
abstract class CesBankDecoratedTransaction extends IcesDBObject implements CesBankTransactionInterface {
  /**
   * @var Transaction
   */
  protected $parent;
  public static $table = 'ces_transaction';
  /**
   * TODO: document.
   */
  public function getId() {
    return $this->parent->getId();
  }
  /**
   * TODO: document.
   * @return float
   *   The amount in this currency in the transaction.
   */
  public function getAmount(CesBankCurrency $currency = NULL) {
    return $this->parent->getAmount($currency);
  }
  /**
   * TODO: document.
   */
  public function getUser() {
    return $this->parent->getUser();
  }
  /**
   * TODO: document.
   */
  public function getConcept() {
    return $this->parent->getConcept();
  }
  /**
   * TODO: document.
   */
  public function getFromAccountName() {
    return $this->parent->getFromAccountName();
  }
  /**
   * TODO: document.
   */
  public function getToAccountName() {
    return $this->parent->getToAccountName();
  }
  /**
   * TODO: document.
   */
  public function getParent() {
    return $this->parent;
  }
  /**
   * TODO: document.
   */
  public function getState() {
    return $this->parent->getState();
  }
  /**
   * TODO: document.
   */
  public function getLog($state) {
    return $this->parent->getLog($state);
  }
  /**
   * TODO: document.
   */
  public function log($messages) {
    return $this->parent->log($messages);
  }
  /**
   * TODO: document.
   */
  public function trigger() {
    return $this->parent->trigger();
  }
  /**
   * TODO: document.
   */
  public function check() {
    return $this->parent->check();
  }
  /**
   * TODO: document.
   */
  public function apply() {
    return $this->parent->apply();
  }
  /**
   * TODO: document.
   */
  public function revoke() {
    return $this->parent->revoke();
  }
  /**
   * Set the state of this transction.
   *
   * @param int $state
   *   One of the constants defined in CesBankTransacionInterface.
   */
  public function setState($state) {
    return $this->parent->setState($state);
  }
  /**
   * Implements CesBankTransactionInterface::getCreated().
   */
  public function getCreated() {
    return $this->parent->getCreated();
  }
  /**
   * TODO: document.
   */
  public function __construct($parent, $record) {
    $this->parent = $parent;
    parent::__construct($record);
  }
  /**
   * Implements CesBankTransactionInterface::acceptByAccount().
   */
  public function acceptByAccount($account_id) {
    $this->parent->acceptByAccount($account_id);
  }
}
/**
 * This class extends the basic transaction features to allow transactions
 * between distinct exchanges. The amount of this transaction has always to be
 * understood in the seller's exchange currency.
 */
class CesBankInterExchangeTransaction extends CesBankDecoratedTransaction {
  /**
   * @var int The identifier of the other transaction
   */
  public $pairid;
  /**
   * @var int one of CesBankInterExchangeTransaction::$FROM or
   * CesBankInterExchangeTransaction::$TO.
   */
  public $side;

  public static $FROM = 0;
  public static $TO = 1;
  /**
   * Creates a transaction between two accounts from distinct exchanges.
   *
   * @return CesBankInterExchangeTransaction
   *   A new transaction object between two distinct exchanges.
   */
  public static function newCesBankInterExchangeTransaction($record) {
    $sa = new IcesSerializer('CesBankLocalAccount');
    $fromaccount = $sa->load($record['fromaccount']);
    $toaccount = $sa->load($record['toaccount']);
    $se = new IcesSerializer('CesBankExchange');
    $fromexchange = $se->load($fromaccount->exchange);
    $toexchange = $se->load($toaccount->exchange);

    $torecord = $record;
    $torecord['fromaccount'] = $toexchange->getVirtualAccount($fromexchange)->id;
    $totransaction = new CesBankInterExchangeTransaction(
      new CesBankBasicTransaction($torecord), array(
        'side' => CesBankInterExchangeTransaction::$TO,
        'pairid' => NULL,
      ));
    $st = new IcesSerializer('CesBankInterExchangeTransaction');
    $st->save($totransaction);

    $fromrecord = $record;
    $fromrecord['toaccount'] = $fromexchange->getVirtualAccount($toexchange)->id;
    $fromrecord['amount'] = $totransaction->getAmount($fromexchange->getCurrency());
    $fromtransaction = new CesBankInterExchangeTransaction(
      new CesBankBasicTransaction($fromrecord), array(
        'side' => CesBankInterExchangeTransaction::$FROM,
        'pairid' => $totransaction->getId(),
      ));
    $st->save($fromtransaction);
    $totransaction->pairid = $fromtransaction->getId();
    return $totransaction;
  }
  /**
   * Implements trigger().
   */
  public function trigger() {
    $this->getParent()->trigger();
    $pair = $this->getPair();
    $pair->getParent()->trigger();
    $s = new IcesSerializer('CesBankBasicTransaction');
    $s->save($pair);
    // @TODO: If the first transaction fails, don't try the second.
    // If the second fails, undo the first.
  }
  /**
   * Implements check().
   */
  public function check() {
    // Check two virtual transactions.
    $this->getParent()->check();
    $pair = $this->getPair();
    $pair->getParent()->check();
    $s = new IcesSerializer('CesBankBasicTransaction');
    $s->save($pair);
  }
  /**
   * Implements apply().
   */
  public function apply() {
    // Apply the two virtual transactions.
    $this->getParent()->apply();
    $pair = $this->getPair();
    $pair->getParent()->apply();
    $s = new IcesSerializer('CesBankBasicTransaction');
    $s->save($pair);
  }
  /**
   * Implements revoke().
   */
  public function revoke() {
    // Revoke the two virtual transactions.
    $this->getParent()->revoke();
    $pair = $this->getPair();
    $pair->getParent()->revoke();
    $s = new IcesSerializer('CesBankBasicTransaction');
    $s->save($pair);
  }
  /**
   * Deletes the associated transaction.
   */
  public function deleteExtraData() {
    $pair = $this->getPair()->getParent();
    $s = new IcesSerializer('CesBankBasicTransaction');
    $s->delete($pair);
  }
  /**
   * The seller account name.
   */
  public function getToAccountName() {
    return $this->getVirtualTransactionTo()->getToAccountName();
  }
  /**
   * The buyer account name.
   */
  public function getFromAccountName() {
    return $this->getVirtualTransactionFrom()->getFromAccountName();
  }
  /**
   * Implements getState().
   *
   * Only works for states from 0 to 5.
   */
  public function getState() {
    $state1 = $this->getParent()->getState();
    $state2 = $this->getPair()->getParent()->getState();
    $statemin = min($state1, $state2);
    $statemax = max($state1, $state2);
    if ($statemax < 4) {
      // NEW, TRIGGERED, ACCEPTED, COMMITED.
      return $statemin;
    }
    else {
      // ARCHIVED, REJECTED.
      return $statemax;
    }
  }
  /**
   * Implements getLog().
   */
  public function getLog($state) {
    $log1 = $this->getParent()->getLog($state);
    $log2 = $this->getPair()->getParent()->getLog($state);
    return array_merge($log1, $log2);
  }
  /**
   * The virtual transaction in buyer's exchange.
   */
  protected function getVirtualTransactionFrom() {
    if ($this->side == CesBankInterExchangeTransaction::$FROM) {
      $t = $this->getParent();
      return $t;
    }
    else {
      $t = $this->getPair()->getParent();
      return $t;
    }
  }
  /**
   * The virtual transaction in seller's exchange.
   */
  protected function getVirtualTransactionTo() {
    if ($this->side == CesBankInterExchangeTransaction::$TO) {
      $t = $this->getParent();
      return $t;
    }
    else {
      $t = $this->getPair()->getParent();
      return $t;
    }
  }
  /**
   * Implements CesBankTransactionInterface::acceptByAccount.
   */
  public function acceptByAccount($account_id) {
    $this->getVirtualTransactionFrom()->acceptByAccount($account_id);
  }
  /**
   * The associated transaction.
   */
  protected function getPair() {
    $s = new IcesSerializer('CesBankBasicTransaction');
    $iet = $s->load($this->pairid);
    return $iet;
  }
}
/**
 * CesBankLevyedTransaction, a kind of decorated transaction.
 */
class CesBankLevyedTransaction extends CesBankDecoratedTransaction {
  /**
   * @var double percentage (over 100)
   */
  public $levy;
  /**
   * Apply.
   *
   * @todo Document and finish this!
   */
  public function apply() {
    // TODO.
  }

}
/**
 * The set of permissions of a particular user.
 */
class CesBankUserPermission {
  /**
   * @var int
   */
  protected $userid;
  /**
   * @var array of Permission
   */
  protected static $permissions = array();
  /**
   * TODO: document.
   *
   * @param int $userid
   *   The user id to connect this permission with.
   */
  public function __construct($userid) {
    global $user;
    if ($userid === NULL) {
      $this->userid = $user->uid;
    }
    else {
      $this->userid = $userid;
    }
  }
  /**
   * Checks whether the user has access to certain permission.
   *
   * @param int|string $permission
   *   The type of action: 'view', 'use', 'edit' or 'admin', or teir respective
   *   numeric equivalents.
   * @param string $object
   *   The name of the object class to be accessed: 'echange', 'account',
   *   'transaction', etc.
   * @param int $objectid
   *   The identifier of the object instance to be accessed.
   *
   * @return bool
   *   whether user of this Permissions object has access of type given by
   *   the $permission parameter over the object ($object, $objectid).
   */
  public function access($permission, $object, $objectid) {
    if ($this->userid == 1) {
      return TRUE;
    }
    if (is_string($permission)) {
      $equiv = array(
        'view' => 10,
        'use' => 20,
        'edit' => 30,
        'admin' => 40,
      );
      $permission = $equiv[$permission];
    }
    // Treat here the recursive case of transaction.
    if ($object == 'transaction') {
      $ts = new IcesSerializer('CesBankBasicTransaction');
      $as = new IcesSerializer('CesBankLocalAccount');
      $transaction = $ts->load($objectid);
      $fromaccount = $as->loadFromUniqueKey('name', $transaction->getFromAccountName());
      $toaccount = $as->loadFromUniqueKey('name', $transaction->getToAccountName());
      return $this->access($permission, 'account buyer', $fromaccount->id)
          || $this->access($permission, 'account seller', $toaccount->id);
    }
    // Compute permissions.
    $userperms = &$this->getPermissions();
    foreach ($userperms as $userperm) {
      if ($userperm->permission >= $permission
        && $this->belongs($object, $objectid, $userperm->object, $userperm->objectid)) {
        return TRUE;
      }
    }
    return FALSE;
  }
  /**
   * Compute the user's permissions and set up internal $permissions.
   *
   * Compute the user's permissions and populate the protected internal variable
   * $permissions.
   * Note: this function easily needs 10 or 15 database queries. It is
   * recommendable to make cache of its result. Further, it could be easily
   * improved to do only 3 or 4 queries if it is necessary (changing the foreach
   * loops for IN(...) SQL statements).
   */
  public function &getPermissions() {
    if (!isset(self::$permissions[$this->userid])) {
      $sp = new IcesSerializer('CesBankPermission');
      if ($this->userid == 1) {
        $permissions = $sp->loadCollection(array());
      }
      else {
        $permissions = array();
        // Get permissions directly granted to this user.
        $userperms = $sp->loadCollection(array('scope' => 'user', 'scopeid' => $this->userid));
        $permissions = array_merge($permissions, $userperms);
        // Get permissions granted to user's accounts. Compute exchanges.
        $sau = new IcesSerializer('CesBankAccountUser');
        $useraccounts = $sau->loadCollection(array('user' => $this->userid));
        foreach ($useraccounts as $useraccount) {
          $accountperms = $sp->loadCollection(array('scope' => 'account', 'scopeid' => $useraccount->account));
          $permissions = array_merge($permissions, $accountperms);

        }
        // Compute exchanges.
        $exchanges = array();
        $sla = new IcesSerializer('CesBankLocalAccount');
        foreach ($useraccounts as $useraccount) {
          $account = $sla->load($useraccount->account);
          if (!in_array($account->exchange, $exchanges)) {
            $exchanges[] = $account->exchange;
          }
        }
        // Get permissions granted to user's accounts exchanges.
        foreach ($exchanges as $exchange) {
          $exchangeperms = $sp->loadCollection(array('scope' => 'exchange', 'scopeid' => $exchange));
          $permissions = array_merge($permissions, $exchangeperms);
        }
        // Get global permissions.
        $globalperms = $sp->loadCollection(array('scope' => 'global'));
        $permissions = array_merge($permissions, $globalperms);
      }
      // Set the static variable.
      self::$permissions[$this->userid] = &$permissions;
    }
    return self::$permissions[$this->userid];
  }
  /**
   * TODO document.
   * @return bool
   *   Whether (object1,objectid1) belongs to (object2, objectid2).
   */
  protected function belongs($object1, $objectid1, $object2, $objectid2) {
    // Case permission. Permission belongs to A if the object of that permission
    // belongs to A.
    if ($object1 == 'permission') {
      $s = new IcesSerializer('CesBankPermission');
      $perm = $s->load($objectid1);
      return $this->belongs($perm->object, $perm->objectid, $object2, $objectid2);
    }
    // Case same scope. Compare identifiers.
    if (substr($object1, 0, strlen($object2)) == $object2) {
      return $objectid1 == $objectid2 || (substr($object1, 0, 6) == 'global');
    }
    elseif (strpos($object2, ' ') === FALSE) {
      // All belongs to global scope.
      if ($object2 == CesBankPermission::SCOPE_GLOBAL) {
        return TRUE;
      }
      // Strip non-scope keywords from object1.
      $object1 = substr($object1, 0, strpos($object1, ' '));
      // At this point we know that $object2 is one of 'account', 'exchange' and
      // $object1 one of 'account', 'exchange', 'global'
      // and $object2 != $object1
      // so the only favorable possibility is the following.
      if ($object1 == CesBankPermission::SCOPE_ACCOUNT && $object2 == CesBankPermission::SCOPE_EXCHANGE) {
        $s = new IcesSerializer('CesBankLocalAccount');
        $account = $s->load($objectid1);
        return ($account->exchange == $objectid2);
      }
      else {
        return FALSE;
      }
    }
    else {
      return FALSE;
    }
  }
}
  /**
   * TODO: document.
   */
class CesBankPermission extends IcesDBObject {
  /**
   * @var int
   */
  public $id;
  /**
   * @var int
   */
  public $permission;
  /**
   * @var string
   * Determines, together with $objectid, the target data of this permission.
   * It begins with a scope string and optionally contain another
   * keyword separated by space. eg. 'account' or 'account balance'. This scope
   * cannot be user. It can be only 'global', 'exchange' or 'account'
   */
  public $object;
  /**
   * @var int
   */
  public $objectid;
  /**
   * @var string
   * Determines, together with scopeid, the group of users having this
   * permission.
   * Is a single keyword. Must be one of 'global', 'exchange', 'account', 'user'
   */
  public $scope;
  /**
   * @var int
   */
  public $scopeid;
  public static $table = 'ces_permission';
  const PERMISSION_VIEW = 10;
  const PERMISSION_USE = 20;
  const PERMISSION_EDIT = 30;
  const PERMISSION_ADMIN = 40;
  const SCOPE_GLOBAL = 'global';
  const SCOPE_EXCHANGE = 'exchange';
  const SCOPE_ACCOUNT = 'account';
  const SCOPE_USER = 'user';
}
/**
 * @} End of "ingroup ces_bank".
 */

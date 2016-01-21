<?php

/**
 * @file
 * Balanced Payments Drupal Commerce Integration Class.
 */

/**
 * Balanced Payments abstraction class for Drupal Commerce.
 *
 * @author Rob Lee
 */
class BalancedCommerce {
  /**
   * @var string $marketplaceId
   *   A string representing the marketplace ID.
   */
  protected $marketplaceId;

  /**
   * Initialize the Balanced Payments API.
   *
   * @return \BalancedCommerce The Marketplace object.
   */
  public function __construct() {
    $marketplace = $this->getMarketPlace();

    return $marketplace;
  }

  /**
   * Initialize and return the Marketplace.
   *
   * @return \Balanced\Marketplace
   *   The Marketplace object.
   */
  public function getMarketPlace() {
    // Load the balanced payments library.
    libraries_load('balanced-php-master');
    Httpful\Bootstrap::init();
    RESTful\Bootstrap::init();
    Balanced\Bootstrap::init();

    $rule = rules_config_load('commerce_payment_balancedpayments_bank');
    foreach ($rule->actions() as $action) {
      if (is_array($action->settings['payment_method']['settings'])) {
        $vars = $action->settings['payment_method']['settings'];
      }
    }

    if ($vars['server'] == 'sandbox') {
      $api_key = $vars['sandbox_key'];
    }
    else {
      $api_key = $vars['production_key'];
    }

    Balanced\Settings::$api_key = $api_key;
    $marketplace = Balanced\Marketplace::mine();

    $this->marketplaceId = $marketplace->id;

    return $marketplace;
  }

  /**
   * Check to see if a Customer exists, if not create one.
   *
   * @param string $uid
   *   The drupal user ID.
   *
   * @return object
   *   The customer object.
   */
  public function customerExists($uid) {
    // See if the customer already exists.
    $cid = db_select('balanced_payments_customer', 'c')
      ->fields('c', array('cid'))
      ->condition('uid', $uid)
      ->execute()
      ->fetchAssoc();

    if (isset($cid['cid'])) {
      // Get the Custoemr.
      $customer = Balanced\Customer::get("/v1/customers/" . $cid['cid']);
      return $customer;
    }
    else {
      // Return FALSE, no Customer.
      return NULL;
    }
  }

  /**
   * Create a Customer through the Balanced Payments API.
   *
   * @param object $user
   *   The User object.
   * @param array $vars
   *   An array of customer variables.
   *
   * @return object
   *   Returns the Customer object.
   */
  public function createCustomer($user, $vars) {
    $customer = new \Balanced\Customer(array(
      'name' => $vars['name_line'],
      'email' => $user->mail,
      'address' => array(
        'line1' => $vars['thoroughfare'],
        'line2' => $vars['premise'],
        'city' => $vars['locality'],
        'state' => $vars['administrative_area'],
        'postal_code' => $vars['postal_code'],
        'country_code' => $vars['country'],
      ),
    ));

    // Save the Customer.
    $customer->save();

    // Save the Customer ID to the database.
    db_insert('balanced_payments_customer')
      ->fields(array(
        'cid' => $customer->id,
        'uid' => $user->uid,
      )
    )
    ->execute();

    return $customer;
  }

  /**
   * Create a Bank Account and add it to a Customer.
   *
   * @param object $customer
   *   The customer object.
   * @param array $bank_info
   *   An array of bank account info.
   * @param string $user_id
   *   The drupal user id.
   *
   * @return \Balanced\Errors\Error|\Balanced\BankAccount
   *   A bank account object or an error object.
   */
  public function createBankAccount($customer, $bank_info, $user_id) {
    $bank_account = new Balanced\BankAccount(array(
      'account_number' => $bank_info['account_number'],
      'name' => $bank_info['account_name'],
      'routing_number' => $bank_info['routing_number'],
      'type' => $bank_info['account_type'],
    ));

    // Save the Bank Account.
    try {
      $bank_account->save();
    }
    catch (Balanced\Errors\Error $e) {
      drupal_set_message($e->description, 'error');
      return $e;
    }

    // Add the Bank Account to the Customer.
    try {
      $customer->addBankAccount($bank_account);
    }
    catch (Balanced\Errors\Error $e) {
      drupal_set_message($e->description, 'error');
      return $e;
    }

    return $bank_account;
  }

  /**
   * Create a Bank Account and add it to a Customer.
   *
   * @param object $customer
   *   A customer object.
   * @param array $info
   *   An array of credit card info.
   *
   * @return \Balanced\Errors\Error|\Balanced\Card
   *   A card object or an error object.
   */
  public function createCreditCard($customer, $info) {
    try {
      // Create a Card.
      $marketplace = $this->getMarketPlace();

      $card = $marketplace->cards->create(array(
        'card_number' => $info['card_num'],
        'expiration_month' => $info['card_exp_month'],
        'expiration_year' => $info['card_exp_year'],
        'security_code' => $info['cvv2'],
      ));

      // Add the Card to the Customer.
      $customer->addCard($card->uri);
    }
    catch (Balanced\Errors\Error $e) {
      return $e;
    }

    return $card;
  }

  /**
   * Initiate a Bank Account Verification.
   *
   * @param object $bank_account
   *   A bank account object.
   *
   * @return \Balanced\Errors\Error|\Balanced\Verification
   *   A verification object or an error object.
   */
  public function initiateVerification($bank_account) {
    // Initiate a bank account verification.
    try {
      $verification = $bank_account->verify();
    }
    catch (Balanced\Errors\Error $e) {
      drupal_set_message($e->description, 'error');
      return $e;
    }

    return $verification;
  }

  /**
   * Get all Bank Accounts by a Drupal user id.
   *
   * @param string $uid
   *   A Drupal user id.
   *
   * @return object|NULL
   *   A list of bank account objects or NULL.
   */
  public function getBankAccountsByUser($uid) {
    $customer = $this->customerExists($uid);

    if (isset($customer)) {
      return Balanced\Customer::get('/v1/customers/' . $customer->id)->bank_accounts->query()->all();
    }
    else {
      return NULL;
    }
  }

  /**
   * Get a Bank Account by remote id.
   *
   * @param string $id
   *   The remote id of the bank account.
   *
   * @return \Balanced\Errors\Error|\Balanced\BankAccount
   *   A bank account object or an error object.
   */
  public function getBankAccountById($id) {
    try {
      $account = Balanced\BankAccount::get('/v1/bank_accounts/' . $id);
    }
    catch (Balanced\Errors\Error $e) {
      return $e;
    }

    return $account;
  }

  /**
   * Get a Credit Card by remote id.
   *
   * @param string $id
   *   The remote id of the credit card.
   *
   * @return \Balanced\Errors\Error|\Balanced\Card
   *   A credit card object or an error object.
   */
  public function getCreditCardById($id) {
    try {
      $card = Balanced\Card::get('/v1/marketplaces/' . $this->marketplaceId . '/cards/' . $id);
    }
    catch (Balanced\Errors\Error $e) {
      return $e;
    }

    return $card;
  }

  /**
   * Debit a Bank Account.
   *
   * @param object $customer
   *   The customer object.
   * @param string $aid
   *   The account to debit.
   * @param string $amount
   *   The amount to debit.
   * @param string $text
   *   Text to appear on the statement.
   * @param string $description
   *   A description of the debit.
   *
   * @return \Balanced\Errors\Error|\Balanced\Debit
   *   A debit object or an error object.
   */
  public function debitBankAccount($customer, $aid, $amount, $text = NULL, $description = NULL) {
    try {
      $debit = $customer->debit(
        $amount,
        $text,
        NULL,
        $description,
        '/v1/marketplaces/' . $this->marketplaceId . '/bank_accounts/' . $aid
      );
    }
    catch (Balanced\Errors\Error $e) {
      return $e;
    }

    return $debit;
  }

  /**
   * Debit a Credit Card.
   *
   * @param object $customer
   *   The customer object.
   * @param string $cid
   *   The card id to debit.
   * @param string $amount
   *   The amount to debit.
   * @param string $text
   *   Text to appear on the statement.
   * @param string $description
   *   A description of the debit.
   *
   * @return \Balanced\Errors\Error|\Balanced\Debit
   *   A debit object or an error object.
   */
  public function debitCreditCard($customer, $cid, $amount, $text = NULL, $description = NULL) {
    try {
      $debit = $customer->debit(
        $amount,
        $text,
        NULL,
        $description,
        '/v1/marketplaces/' . $this->marketplaceId . '/cards/' . $cid
      );
    }
    catch (Balanced\Errors\Error $e) {
      return $e;
    }

    return $debit;
  }

  /**
   * Get a Verification object.
   *
   * @param string $uri
   *   The verification URI.
   *
   * @return \Balanced\Errors\Error|\Balanced\Verification
   *   A verification object or an error object.
   */
  public function getVerification($uri) {
    try {
      $verification = Balanced\BankAccountVerification::get($uri);
    }
    catch (Balanced\Errors\Error $e) {
      return $e;
    }

    return $verification;
  }

  /**
   * Confirm a Verification.
   *
   * @param object $verification
   *   The verification object.
   * @param string $amount1
   *   The first amount to verify.
   * @param string $amount2
   *   The second amount to verify.
   *
   * @return object
   *   A verification object or an error object.
   */
  public function confirmVerification($verification, $amount1, $amount2) {
    // Verify the bank account.
    try {
      $response = $verification->confirm($amount1, $amount2);
    }
    catch (Balanced\Errors\BankAccountVerificationFailure $e) {
      $response = $e;
    }

    return $response;
  }

  /**
   * Delete a Bank Account by account id.
   *
   * @param string $aid
   *   The Bank Account remote id.
   *
   * @return \Balanced\Errors\Error|\Balanced\BankAccount
   *   A bank account object or an error object.
   */
  public function deleteBankAccount($aid) {
    try {
      $account = $this->getBankAccountById($aid);
      if (isset($account->status_code)) {
        return $account;
      }
      else {
        $account->unstore();
      }
    }
    catch (Balanced\Errors\Error $e) {
      return $e;
    }

    return $account;
  }

  /**
   * Delete a Creidt Card by card id.
   *
   * @param string $cid
   *   The credit card remote id.
   *
   * @return \Balanced\Card|\Balanced\Errors\Error
   *   A credit card object or an error object.
   */
  public function deleteCreditCard($cid) {
    try {
      $card = $this->getCreditCardById($cid);
      if (isset($card->status_code)) {
        return $card;
      }
      else {
        $card->unstore();
      }
    }
    catch (Balanced\Errors\Error $e) {
      return $e;
    }

    return $card;
  }
}

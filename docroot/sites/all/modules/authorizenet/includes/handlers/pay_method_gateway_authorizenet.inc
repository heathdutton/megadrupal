<?php

/**
 * @file
 */

class pay_method_gateway_authorizenet extends pay_method_gateway {
  private $_authorizenet_login;
  var $authorizenet_email_customer = 0;
  var $authorizenet_email_merchant = 1;
  var $authorizenet_cim = 0;

  var $gateway_supports_ach = FALSE;
  var $gateway_supports_cc = TRUE;
  var $gateway_supports_recurring = FALSE;

  var $authorizenet_payment_profile;

  /**
   * Load the appropriate Authorize.Net class for this instance.
   */
  function authorizenet() {
    // Set default user, which may get overridden if necessary.
    global $user;
    $account = $user;

    if (!isset($this->authorizenet)) {

      // Use Authorize.Net's AIM handler by default.
      $type = 'aim';

      // CIM has been enabled: attempt to use that instead.
      if ($this->authorizenet_cim) {

        if (isset($this->activity)) {

          // We're continuing an existing activity history.
          if ($history = $this->activity->history()) {

            // A profile id was logged on a pending/authorize/etc. transaction.
            foreach ($this->activity->history() as $previous) {
              $res = db_query("SELECT authorizenet_payment_profile_id
                FROM {authorizenet_pay_activity}
                WHERE paid = :paid", array(':paid' => $previous->paid));

              if ($id = $res->fetchField()) {
                $this->authorizenet_payment_profile = $id;
                $this->total = $previous->total;
                $account = $previous->user();
              }
            }
          }

          // The current activity is tied to a user.
          elseif ($this->activity->uid) {
            $account = $this->activity->user();
          }
        }

        // If under any circumstances we have a real user, user CIM.
        if ($account->uid) {
          $type = 'cim';
        }
      }

      $login = $this->authorizenet_login();
      $this->authorizenet = authorizenet($login, $type, $account);

      // Transfer our configuration settings to the instance.
      $this->authorizenet->emailMerchant = $this->authorizenet_email_merchant;
      $this->authorizenet->emailCustomer = $this->authorizenet_email_customer;
      $this->authorizenet->testMode = $this->gateway_testmode;
    }
    return $this->authorizenet;
  }

  /**
   * Return TRUE if this transaction is using the CIM.
   */
  function cim() {
    return is_a($this->authorizenet(), 'AuthorizenetCim');
  }

  /**
   * List of currencies that Authorize.Net supports.
   */
  function available_currencies() {
    return array('USD');
  }

  /**
   * CIM might have work to do if the transaction is left in 'pending' state.
   * Specifically, it may collect payment information for later processing.
   *
   * Implementing this method allows this to happen.
   */
  function pending_action() {
    if ($this->cim()) {
      $this->execute();
      $this->activity->result = 1;
    }
  }

  /**
   * Determine whether an action is valid and appropraite for a transaction.
   */
  function valid_action($action, $transaction, $history) {
    $valid = parent::valid_action($action, $transaction, $history);

    // We can only authorize a transaction whose card info we already have.
    if ($valid && $action == 'authorize') {
      if ($this->cim()) {
        // TODO determine if user has a valid default payment profile and return
        // T/F based on its validity.
        $valid = FALSE;
      }
      else {
        $valid = FALSE;
      }
    }

    return $valid;
  }

  /**
   * Form method used in pay_method_gateway_authorizenet.
   */
  function form(&$form, &$form_state) {

    // CIM is enabled: Allow users to select from existing payment profiles.
    if ($this->cim()) {

      // Present a list of payment profiles to choose from.
      if ($profile = $this->authorizenet()->profile()) {
        $path = drupal_get_path('module', 'authorizenet');
        drupal_add_js($path . '/js/authorizenet_payment_profile.js');
        drupal_add_css($path . '/theme/authorizenet.base.css');

        $options = array();
        foreach ($profile->paymentProfiles(TRUE) as $id => $payment) {
          $options[$id] = theme('authorizenet_payment_profile', array('profile' => $payment));
        }
        // Add an additional value for enabling the credit card form.
        $options[0] = 'New';

        $group = $this->pay_form->handler();
        $form[$group]['pay_method'][$this->pmid]['authorizenet_payment_profile'] = array(
          '#type' => 'radios',
          '#options' => $options,
          '#default_value' => key($options),
          '#attributes' => array('class' => 'authorizenet-payment-profile'),
          '#theme' => 'authorizenet_payment_profile_radios',
        );
      }
    }

    // Get the default credit card form from pay_method_gateway.
    parent::form($form, $form_state);
  }

  // This is called from the form_validate function in a pay_form class.
  function pay_method_validate($form, &$form_state, $element) {

    // If an existing payment profile has been selected, use it and change
    // $this->payment_type to prevent spurious credit card validation.
    if ($this->cim() && $this->authorizenet_payment_profile) {
      $this->payment_type = 'authorizenet_payment_profile';
    }

    // Use pay_method_gateway validation to confirm credit card form details.
    parent::pay_method_validate($form, $form_state, $element);
  }

  /**
   *  Return the URL that is supported by our API instance (AIM/CIM/etc).
   */
  function gateway_url() {
    return $this->authorizenet()->requestUrl();
  }

  /**
   *  Return the URL that is supported by our API instance (AIM/CIM/etc).
   */
  function gateway_headers() {
    return $this->authorizenet()->requestHeaders();
  }

  /**
   *  Return the request data from our API instance (AIM/CIM/etc).
   */
  function gateway_request() {
    return $this->authorizenet()->transactionRequestData($this->transaction);
  }

  /**
   *  Return the success/failure of the transaction as provided by the API.
   */
  function gateway_response($result) {
    $this->authorizenet()->transactionResponse($this->transaction, $result);

    // Save the transaction ID for tracking and/or future operations.
    $this->activity->identifier = $this->transaction->transId;

    $this->activity->data = array(
      'response_code'   => $this->transaction->resultCode,
      'response_string' => $this->transaction->resultString,
      'approval_code'   => $this->transaction->approvalCode,
      'avs_response'    => $this->transaction->resultData[6],
      'caav_response'   => $this->transaction->resultData[40],
    );

    $this->activity->result = (int) ($this->transaction->result == 1);

    /* Return TRUE if result = 1. Possible values:
     1 = Approved
     2 = Declined
     3 = Error
     4 = Held for Review
     */

    if (!$this->activity->result) {
      watchdog('payment', "Error processing payment: Authorize.Net gateway returned '@err'", array('@err' => $this->activity->data['response_string']));
      drupal_set_message("Error processing payment: Authorize.Net gateway returned'" . $this->activity->data['response_string'] . "'");
    }

    return ($this->activity->result);
  }

  function settings_form(&$form, &$form_state) {
    parent::settings_form($form, $form_state);
    $group = $this->handler();

    // Allow users to select from pre-existing Authorize.Net credentials.
    $form[$group]['authorizenet_login']['#type'] = 'fieldset';
    $form[$group]['authorizenet_login']['#collapsible'] = FALSE;
    $form[$group]['authorizenet_login']['#title'] = t('Authorize.Net credentials');
    $form[$group]['authorizenet_login']['#group'] = $group;

    if ($logins = authorizenet_login_list()) {
      $form[$group]['authorizenet_login']['login_type'] = array(
        '#type' => 'radios',
        '#options' => array(
          'use' => t('Use an existing Authorize.Net login'),
          'new' => t('Create a new login'),
        ),
        '#default_value' => 'use',
      );

      $form[$group]['authorizenet_login']['authorizenet_login_id'] = array(
        '#type' => 'select',
        '#title' => t('Existing Authorize.Net login'),
        '#options' => $logins,
        '#parents' => array($group, 'authorizenet_login_id'),
      );
      if ($this->authorizenet_login()) {
        $form[$group]['authorizenet_login']['authorizenet_login_id']['#default_value'] = $this->authorizenet_login()->id;
      }
    }
    else {
      $form[$group]['authorizenet_login']['login_type'] = array(
        '#type' => 'value',
        '#value' => 'new',
      );
    }

    // Add a settings form for a new login entry.
    $form[$group]['authorizenet_login']['new'] = array();
    $login = authorizenet_login_load();
    $login->settingsForm($form[$group]['authorizenet_login']['new'], $form_state);

    // Enable certain products/features.
    $form[$group]['authorizenet_settings'] = array(
      '#type' => 'fieldset',
      '#title' => t('Authorize.Net settings'),
      '#collapsible' => FALSE,
    );

    // CIM.
    $form[$group]['authorizenet_settings']['authorizenet_cim'] = array(
      '#type' => 'checkbox',
      '#default_value' => $this->authorizenet_cim,
      '#title' => t('Enable Customer Information Manager (CIM)'),
      '#description' => t('CIM allows you to store your customers’ sensitive payment information on Authorize.Net\'s ssecure servers, simplifying payments for returning customers and recurring transactions. This is an optional service and must be enabled in your !account.', array('!account' => l('Authorize.Net account settings', 'https://merchant.authorize.net/'))),
      '#parents' => array($group, 'authorizenet_cim'),
    );

    // Per-method settings.
    $form[$group]['authorizenet_settings']['authorizenet_email_customer'] = array(
      '#type' => 'checkbox',
      '#title' => t('E-mail a receipt to the customer'),
      '#default_value' => $this->authorizenet_email_customer,
      '#parents' => array($group, 'authorizenet_email_customer'),
    );
    $form[$group]['authorizenet_settings']['authorizenet_email_merchant'] = array(
      '#type' => 'checkbox',
      '#title' => t('E-mail a receipt to the merchant'),
      '#default_value' => $this->authorizenet_email_merchant,
      '#parents' => array($group, 'authorizenet_email_merchant'),
    );
  }

  function settings_form_validate($form, &$form_state) {
    $values = $this->form_values($form_state);

    // Validate the new login information.
    if ($values['authorizenet_login']['login_type'] == 'new') {
      $login = authorizenet_login_load();
      $login->validate($values['authorizenet_login']['new']);
    }

    // Validate the selected entry.
    else {
      if (!$values['authorizenet_login_id']) {
        form_set_error('authorizenet_login_id', t('Please select an existing Authorize.Net login.'));
      }
    }
  }

  function settings_form_submit($form, &$form_state) {
    $values = $this->form_values($form_state);
    $group = $this->handler();

    // Create a new login entry to be saved back to the database.
    if ($values['authorizenet_login']['login_type'] == 'new') {
      $login = authorizenet_login_load();
      $login->save($values['authorizenet_login']['new']);
      $form_state['values'][$group]['authorizenet_login_id'] = $login->id;
    }

    // Remove settings from $form_state so they're not saved to the object.
    foreach (array('new', 'login_type') as $item) {
      unset($form_state['values'][$group]['authorizenet_login'][$item]);
    }

    // Call parent handler, which will save this payment method.
    parent::settings_form_submit($form, $form_state);
  }

  function save($values = array()) {
    // Find a reference to the authorizenet_login_id and prpeare to save it.
    $entry = array('authorizenet_login_id' => $values['authorizenet_login_id']);
    unset($this->authorizenet_login_id);

    // Let my parent handler do its save(), which will give me a pmid.
    parent::save($values);
    $entry['pmid'] = $this->pmid;

    // Update/save the reference to the authorizenet_login entry.
    if (!$this->new) {
      db_delete('authorizenet_method_login')
        ->condition('pmid', $entry['pmid'])
        ->execute();
    }
    drupal_write_record('authorizenet_method_login', $entry);
  }

  /**
   * Return the instance of authorizeNetLogin that is associated with this
   * payment method instance.
   */
  function authorizenet_login() {
    if (!isset($this->_authorizenet_login)) {
      if ($id = db_query("SELECT authorizenet_login_id
        FROM {authorizenet_method_login} WHERE pmid = :pmid", array(':pmid' => $this->pmid))->fetchField()) {

        $this->_authorizenet_login = authorizenet_login_load($id);
      }
    }
    return $this->_authorizenet_login;
  }

  /**
   * Execute a payment activity, overriding pay_method_gateway's version.
   *
   * This function is complicated by the fact that the processing for CIM
   * transactions is completely different from that of AIM calls. Accordingly,
   * this function works as follows:
   *
   *  If this is a CIM transaction:
   *    1. Create a new CIM Profile for the user, if none exists.
   *    2. Create a Payment Profile to contain credit card details, or use an
   *       existing Payment Profile as selected by the user.
   *    3. Instantiate a transaction for the selected payment profile.
   *
   *  If this NOT a CIM transaction (AKA it's AIM):
   *    1. Instantiate a transaction for the AIM class and populate it with
   *       credit card details.
   *
   *  In both cases, we have a transaction that can be used for further
   *  processing by the Payment API.
   */
  function execute() {
    // Format the Expiration date for use by Authorize.Net classes.
    $expiration = array(
      'month' => $this->cc_exp_month,
      'year'  => $this->cc_exp_year,
    );

    // Populate the 'email' attribute on the Authorize.net handler.
    $this->authorizenet()->email = $this->mail;

    // Format the customer billing address for Authorize.Net classes.
    $billing = New AuthorizenetAddress();
    $billing->setFirstName($this->first_name);
    $billing->setLastName($this->last_name);
    if ($this->billto) {
      $billing->setAddress($this->billto['street1']);
      $billing->setCity($this->billto['city']);
      $billing->setState($this->billto['state']);
      $billing->setZip($this->billto['zip']);
      $billing->setCountry($this->billto['country']);
    }

    // Set type to a 'pay' action: complete/cancel/refund/delete/authorize.
    $trxtype = $this->activity->action;

    // But it's more nuanced: an identifier means we should use 'capture'.
    if (!is_null($this->activity->identifier)) {
      $trxtype = 'capture';
    }

    // CIM is avaliable:  Make sure there's a Payment Profile to use.
    if ($this->cim()) {

      // Load or create a new AuthorizenetCimCustomerProfile instance.
      $profile = $this->authorizenet()->profile();

      // User has selected an existing payment profile.  Load and validate it.
      if ($id = $this->authorizenet_payment_profile) {
        $payment = $profile->getPaymentProfile($id);
      }

      // Accept new form data and create a AuthorizenetCimPaymentProfile on
      // this user's customer profile.
      else {
        $payment = $profile->addPaymentProfile();

        // Add billTo information to the Payment profile.
        $payment->setBillTo($billing);

        // Set our payment values to the payment profile.
        if ($this->cc_type) {
          $payment->setCardNumber($this->cc_number);
          $payment->setExpirationDate($expiration);
          $payment->setCardCode($this->cc_ccv2);
          $payment->setAccountType($this->cc_type);
        }

        // Save the payment profile to the CIM API (and locally).
        $payment->save();
        $this->authorizenet_payment_profile = $payment->id;
      }

      // Store a relationship between the activity and the payment profile.
      $entry = array(
        'paid' => $this->activity->paid,
        'authorizenet_payment_profile_id' => $this->authorizenet_payment_profile,
      );
      drupal_write_record('authorizenet_pay_activity', $entry);

      // Ensure this activity gets stored with the correct payment_type.
      $this->payment_type = $payment->accountType;

      // Create a transaction for the new/selected payment profile.
      if ($this->activity->action && $this->activity->action != 'pending') {
        $trx = $payment->addTransaction($trxtype);
      }
    }

    // Not using CIM: Create a standalone transaction.
    else {
      $trx = $this->authorizenet()->setTransaction($trxtype);
      if (isset($billing)) $trx->setCustomerBillingAddress($billing);
      if (isset($shipping)) $trx->setCustomerShippingAddress($shipping);

      if ($this->cc_type) {
        $trx->setCardNumber($this->cc_number);
        $trx->setExpirationDate($expiration);
        $trx->setCardCode($this->cc_ccv2);
      }
    }

    // Populate the CIM/standalone transaction with common values.
    if (isset($trx)) {
      // Set the amount and other values pertinent to any transaction.
      $trx->setAmount($this->total());
      $trx->setOrder($this->activity->pxid, $this->activity->pay_transaction()->pay_form()->title);

      if (isset($value)) {
        $trx->setTax($value);
        $trx->setShipping($value);
        $trx->setDuty($value);
      }


      // Set the identifier, which will be populated if a previous successful
      // 'authorize' activity has occurred
      if (isset($this->activity->data['approval_code'])) {
        $trx->setTransId($this->activity->identifier);
        $trx->setApprovalCode($this->activity->data['approval_code']);
      }

      // TODO
      // foreach ($lineitems as $item) $transaction->addLineItem($item);
      if (isset($value)) {
        $trx->setTaxExempt($value);
        $trx->setRecurringBilling($value);
      }
      
      $this->transaction = $trx;
    }

    // Allow the default execute() behavior to go into effect.  This
    // will post gateway_request() data to gateway_url().
    return parent::execute($this->activity);
  }
}

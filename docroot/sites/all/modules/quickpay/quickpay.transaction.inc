<?php
/**
 * @file
 * Quickpay and QuickpayTransaction classes.
 */

class QuickpayException extends Exception {}

/**
 * The Quickpay class abstracts a specific setup. Each QuickpayTransaction is
 * associated with a Quickpay class, containing the configuration.
 */
class Quickpay {
  protected static $currency_info = array();
  protected $test_mode = FALSE;
  protected $debug = FALSE;
  protected $merchant;
  protected $secret;
  protected $order_prefix = '';
  protected $accepted_cards = array('creditcard');
  protected $popup = FALSE;
  // With a 'use user language' default option.
  protected $language = LANGUAGE_NONE;

  /**
   * Constructor. Takes a settings array.
   */
  public function __construct($options) {
    // Check options, throw exception if problems.
    if (empty($options['merchant']) || !preg_match('/^[0-9]{8}$/', $options['merchant'])) {
      throw new QuickpayException(t('Invalid merchant number'));
    }
    $this->merchant = $options['merchant'];
    if (empty($options['secret'])) {
      throw new QuickpayException(t('Empty secret?'));
    }
    $this->secret = $options['secret'];
    if (isset($options['test_mode'])) {
      $this->test_mode = (bool) $options['test_mode'];
    }
    if (isset($options['debug'])) {
      $this->debug = (bool) $options['debug'];
    }
    if (isset($options['order_prefix'])) {
      $this->order_prefix = $options['order_prefix'];
    }
    // @todo validate this
    if (isset($options['accepted_cards'])) {
      $this->accepted_cards = $options['accepted_cards'];
    }
    if (isset($options['language'])) {
      $this->language = $options['language'];
    }
    /* if (isset($options['popup'])) { */
    /*   $this->popup = (bool) $options['popup']; */
    /* } */
  }

  /**
   * Get a transaction associated with this Quickpay instance.
   */
  public function transaction($txnid) {
    return new QuickpayTransaction($this, $txnid);
  }

  /**
   * Load a transaction from $_POST.
   */
  public function transaction_callback() {
    static $md5_order = array(
      'msgtype',
      'ordernumber',
      'amount',
      'currency',
      'time',
      'state',
      'qpstat',
      'qpstatmsg',
      'chstat',
      'chstatmsg',
      'merchant',
      'merchantemail',
      'transaction',
      'cardtype',
      'cardnumber',
      'cardhash',
      'cardexpire',
      'acquirer',
      'splitpayment',
      'fraudprobability',
      'fraudremarks',
      'fraudreport',
      'fee',
    );

    // Check that it validates.
    $md5_string = "";
    foreach ($md5_order as $field) {
      if (isset($_POST[$field])) {
        $md5_string .= $_POST[$field];
      }
    }
    if (md5($md5_string . $this->secret) != $_POST['md5check']) {
      watchdog('quickpay', 'Transaction callback md5 didn\'t verify. MD5 calculated: @md5 Data: @data',
        array(
          '@data' => print_r($_POST, TRUE),
          '@md5' => md5($md5_string . $this->secret),
        ), WATCHDOG_ERROR);
      return NULL;
    }

    $txn = new QuickpayTransaction($this);
    $txn->load_response($_POST);
    return $txn;
  }

  /**
   * Returns a form for redirecting user to Quickpay for payment.
   */
  public function form($order_id, $amount, $currency, $module, $options) {
    $options += array(
      'autocapture' => FALSE,
      'msgtype' => 'authorize',
      'description' => NULL,
      'continue_url' => NULL,
      'cancel_url' => NULL,
      'splitpayment' => TRUE,
    );

    if (empty($options['continue_url'])) {
      throw new QuickpayException(t("Missing required continue_url."));
    }

    if (empty($options['cancel_url'])) {
      throw new QuickpayException(t("Missing required cancel_url."));
    }

    if (!in_array($options['msgtype'], array('authorize', 'subscribe'))) {
      // FIXME: better error handling.
      throw new QuickpayException(t("Bad message type @type, must be 'authorize' or 'subscribe'.", array('@type' => $options['msgtype'])));
    }

    if ($options['msgtype'] == 'subscribe') {
      if (empty($options['description'])) {
        throw new QuickpayException(t("Missing required subscription description."));
      }
      if ($options['autocapture']) {
        throw new QuickpayException(t("Autocapture not possible for subscriptions."));
      }
    }

    if (!module_hook($module, 'quickpay_factory')) {
      throw new QuickpayException(t("%module doesn't implement hook_quickpay_factory.", array('%module' => $module)));
    }

    if (!module_hook($module, 'quickpay_callback')) {
      throw new QuickpayException(t("%module doesn't implement hook_quickpay_callback.", array('%module' => $module)));
    }

    // Required variables.
    $data['protocol'] = QUICKPAY_VERSION_POST;
    $data['msgtype'] = $options['msgtype'];
    $data['merchant'] = $this->merchant;
    // Use the users current language
    if ($this->language == LANGUAGE_NONE) {
      global $language;
      $data['language'] = self::get_country_code($language->language);
    }
    else {
      $data['language'] = $this->language;
    }
    $data['ordernumber'] = $this->order_prefix . $order_id;
    // Ensure that Order number is at least 4 characters. Else Quickpay will
    // reject the request.
    if (strlen($data['ordernumber']) < 4) {
      $data['ordernumber'] = $this->order_prefix . substr('0000' . $order_id,
                             -4+strlen($this->order_prefix));
    }

    $currency_info = Quickpay::currency_info($currency);
    $data['amount'] = Quickpay::wire_amount($amount, $currency_info);
    $data['currency'] = $currency_info['code'];
    $data['continueurl'] = $options['continue_url'];
    $data['cancelurl'] = $options['cancel_url'];

    // End of required variables.
    $data['callbackurl'] = url(_quickpay_md5_path('quickpay/' . $order_id . '/' . $module), array('absolute' => TRUE));

    $data['autocapture'] = ($options['autocapture']) ? '1' : '0';
    $data['splitpayment'] = ($options['splitpayment']) ? '1' : '0';

    if (!empty($options['description'])) {
      $data['description'] = $options['description'];
    }

    if ($this->test_mode) {
      $data['testmode'] = '1';
      $data['ordernumber'] .= "T";
    }
    else {
      $data['testmode'] = '0';
    }

    if (!empty($options['autofee']) && $options['autofee']) {
      $data['autofee'] = 1;
    }

    $data['cardtypelock'] = $this->get_cardtypelock();

    // Other possible variables:
    // $data['CUSTOM_*'];
    $data['md5check'] = $this->md5checksum($data);

    $form['#method'] = 'POST';
    $form['#action'] = 'https://secure.quickpay.dk/form/';

    foreach ($data as $name => $value) {
      $form[$name] = array('#type' => 'hidden', '#value' => $value);
    }

    $button_id = drupal_html_id('quicpay-submit-button');
    $form['submit'] = array(
      '#type' => 'submit',
      '#value' => t('Continue to QuickPay'),
      '#id' => $button_id,
    );

    return $form;
  }

  /**
   * Send a request to Quickpay. Primarily for use by QuickpayTransaction.
   */
  public function request($type, $request_data, QuickpayTransaction $transaction) {
    if (!is_array($request_data)) {
      $request_data = (array) $request_data;
    }

    $request_data['protocol'] = QUICKPAY_VERSION_API;
    $request_data['msgtype'] = $type;
    $request_data['merchant'] = $this->merchant;
    $request_data['md5check'] = $this->md5checksum($request_data);

    if ($this->debug) {
      debug($request_data, 'Quickpay request parameters');
    }

    if (!$this->validate_request($request_data)) {
      throw new QuickpayException(t("Request message didn't pass validation."));
    }
    $request_options = array(
      'headers' => array('Content-Type' => 'application/x-www-form-urlencoded'),
      'method' => 'POST',
      'data' => http_build_query($request_data, FALSE, '&'),
      'max_redirects' => 0,
    );
    /*
     * We're using curl as drupal_http_request cannot be used in unit tests.
     */
    if (!function_exists('curl_init')) {
      throw new QuickpayException(t("cURL not found."));
    }
    $ch = curl_init();
    curl_setopt($ch, CURLOPT_URL, 'https://secure.quickpay.dk/api');
    curl_setopt($ch, CURLOPT_POST, count($request_data));
    curl_setopt($ch, CURLOPT_POSTFIELDS, http_build_query($request_data, FALSE, '&'));
    curl_setopt($ch, CURLOPT_RETURNTRANSFER, TRUE);
    $response = curl_exec($ch);


    if ($this->debug) {
      debug($response, 'Quickpay response');
    }

    if (!$response) {
      throw new QuickpayException(t('Server returned non-success code or empty result'));
    }
    $transaction->load_response($this->response($response));

    return $transaction;
  }

  /**
   * Calculate the md5checksum for the request.
   */
  public function md5checksum($data) {
    // This is the sum of fields in all types of requests.
    // Or used to be, the authorize request has drifted too far, so it's
    // currently not supported.
    $fields = array(
      'protocol',
      'msgtype',
      'merchant',
      'language',
      'ordernumber',
      'amount',
      'finalize',
      'currency',
      'continueurl',
      'cancelurl',
      'callbackurl',
      'autocapture',
      'cardnumber',
      'expirationdate',
      'cvd',
      'autofee',
      'cardtypelock',
      'ipaddress',
      'transaction',
      'description',
      'testmode',
      'splitpayment',
      'forcemobile',
      'deadline',
      'cardhash',
      'fraud_remote_addr',
      'fraud_http_accept',
      'fraud_http_accept_language',
      'fraud_http_accept_encoding',
      'fraud_http_accept_charset',
      'fraud_http_referer',
      'fraud_http_user_agent',
      'apikey',
    );

    $string = "";
    foreach ($fields as $field) {
      if (isset($data[$field])) {
        $string .= $data[$field];
      }
    }
    return md5($string . $this->secret);
  }

  /**
   * Validates that the request fields is formatted as expected by Quickpay.
   *
   * Throws an exception on failure.
   *
   * @param array $data
   *   Associative array of parameters.
   *
   * @return bool
   *   Always true, an exception is thrown on error.
   */
  protected function validate_request($data) {
    $fields = array(
      'protocol' => '/^(' . QUICKPAY_VERSION_API . '|' . QUICKPAY_VERSION_POST . ')$/',
      'msgtype' => '/^[a-z]+$/',
      'merchant' => '/^[0-9]{8}$/',
      'ordernumber' => '/^[a-zA-Z0-9]{4,20}$/',
      'amount' => '/^[0-9]{1,10}$/',
      'finalize' => '/^[0-1]{1}$/',
      'currency' => '/^[A-Z]{3}$/',
      'autocapture' => '/^[0-1]{1}$/',
      'cardnumber' => '/^[0-9]{13,19}$/',
      'expirationdate' => '/^[0-9]{4}$/',
      'cvd' => '/^[0-9]{0,4}$/',
      'cardtypelock' => '/^[a-zA-Z,]{0,128}$/',
      'splitpayment' => '/^[0-1]{1}$/',
      'transaction' => '/^[0-9]{1,32}$/',
      'description' => '/^[\w _\-\.]{0,20}$/',
      'md5check' => '/^[a-z0-9]{32}$/',
      'CUSTOM_' => '/^[\w _\-\.]{0,20}$/',
    );

    foreach ($data as $field => $value) {
      // No NULL values please.
      if (is_null($value)) {
        throw new QuickpayException(t('%field cannot be NULL', array('%field' => $field)));
      }
      elseif ($fields[$field]) {
        if (!preg_match($fields[$field], $value)) {
          // We're not logging the actual value, as that might be
          // sensitive information.
          throw new QuickpayException(t("%field didn't pass validation.",
              array('%field' => $field)));
        }
      }
      elseif (preg_match('/^CUSTOM_/', $field)) {
        if (!preg_match($fields['CUSTOM_'], $value)) {
          throw new QuickpayException(t("%field didn't pass validation.",
              array('%field' => $field)));
        }
      }
      else {
        throw new QuickpayException(t('Unknown field %field.',
            array('%field' => $field)));
      }
    }
    return TRUE;
  }

  /**
   * Parses the XML response from Quickpay into an associative
   * array. Internal use only.
   *
   * @param string $response
   *   The XML response.
   *
   * @return array
   *   XML as an associative array.
   */
  protected function response($response) {
    // Load XML in response into DOM.
    $result = array();
    $dom = new DOMDocument();
    $dom->loadXML($response);
    // Find elements en response and put them in an associative array.
    $xpath = new DOMXPath($dom);
    $elements = $xpath->query('/response/*');
    foreach ($elements as $cn) {
      // If the element has (real) children - this is the case for
      // status->history and chstatus->entry.
      if ($cn->childNodes->length > 1) {
        $child = array();
        foreach ($cn->childNodes as $hn) {
          $child[$hn->nodeName] = $hn->nodeValue;
        }
        $result[$cn->nodeName][] = $child;
      }
      else {
        $result[$cn->nodeName] = $cn->nodeValue;
      }
    }

    return $result;
  }


  /**
   * This method currently doesn't work.
   *
   * Authorize a payment through the API. Only for test use.
   *
   * In order to handle credit card information, you'll either need a full PCI
   * certification which can cost 15000EUR or more, or have a signed
   * dispensation from Nets (which you're very unlikely to get), so this
   * function is prohibited for most sites. It is rather handy for unit
   * testing the API though.
   *
   * Just to be clear: using this function without PCI certification or
   * dispensation could mean that Nets shuts down your agreement without
   * notice.
   *
   * However, if you *are* allowed to use this function, please contact the
   * maintainer, so he knows it's in fact being used.
   */
  public function authorize($cardnumber, $expiration, $cvd, $order_id, $amount, $currency, $options = array()) {
    throw new QuickpayException(t('Authorize currently not supported in the API.'));
    $options += array(
      'autocapture' => FALSE,
      'splitpayment' => FALSE,
    );
    $currency_info = Quickpay::currency_info($currency);
    $request_data = array(
      'cardnumber' => $cardnumber,
      'expirationdate' => $expiration,
      'cvd' => $cvd,
      'ordernumber' => $this->ordernumber($order_id),
      'amount' => Quickpay::wire_amount($amount, $currency_info),
      'currency' => $currency_info['code'],
      'autocapture' => $options['autocapture'] ? '1' : '0',
      'splitpayment' => $options['splitpayment'] ? '1' : '0',
    );
    $transaction = new QuickpayTransaction($this);
    return $this->request('authorize', $request_data, $transaction);
  }

  /**
   * Subscribe test function. See authorize().
   */
  public function subscribe($cardnumber, $expiration, $cvd, $order_id, $description) {
    $request_data = array(
      'cardnumber' => $cardnumber,
      'expirationdate' => $expiration,
      'cvd' => $cvd,
      'ordernumber' => $this->ordernumber($order_id),
      'description' => $description,
    );
    $transaction = new QuickpayTransaction($this);
    return $this->request('subscribe', $request_data, $transaction);
  }

  /**
   * Ensure that ordernumber is properly formatted.
   */
  public function ordernumber($order_id) {
    $ordernumber = $this->order_prefix . $order_id;
    // Ensure that Order number is at least 4 characters.
    if (strlen($ordernumber) < 4) {
      $ordernumber = $this->order_prefix . substr('0000' . $order_id,
                     -4 + strlen($this->order_prefix));
    }
    return $ordernumber;
  }

  /**
   * Return the proper cardtypelock for the accepted cards.
   */
  protected function get_cardtypelock() {
    if (is_array($this->accepted_cards)) {
      $cards = $this->accepted_cards;
      // Aren't supported in cardtypelock.
      unset($cards['ikano']);
      return join(',', $cards);
    }
    // Already set to the proper string.
    return $this->accepted_cards;
  }

  /**
   * Returns an array of languages supported by Quickpay.
   */
  protected static function get_languages() {
    return array(
      'da' => t('Danish'),
      'de' => t('German'),
      'en' => t('English'),
      'fo' => t('Faeroese'),
      'fr' => t('French'),
      'gl' => t('Greenlandish'),
      'it' => t('Italian'),
      'no' => t('Norwegian'),
      'nl' => t('Dutch'),
      'pl' => t('Polish'),
      'se' => t('Swedish'),
    );
  }

  /**
   * Return the associated country code of a language code.
   *
   * @param string $langcode
   */
  protected static function get_country_code($langcode) {
    $langcodes = array(
      'da' => 'da',
      'de' => 'de',
      'en' => 'en',
      'fo' => 'fo',
      'fr' => 'fr',
      'kl' => 'gl',
      'it' => 'it',
      'nb' => 'no',
      'nn' => 'no',
      'nl' => 'nl',
      'pl' => 'pl',
      'sv' => 'se',
    );

    return isset($langcodes[$langcode]) ? $langcodes[$langcode] : 'en';
  }

  /**
   * Returns the settings form.
   *
   * @param array $settings
   *   Previously saved setting.
   */
  public static function settings_form($settings = array()) {
    $settings += array(
      'merchant' => '',
      'secret' => '',
      'order_prefix' => '',
      'language' => LANGUAGE_NONE,
      'accepted_cards' => array('creditcard'),
      'test_mode' => FALSE,
      'debug' => FALSE,
      'popup' => FALSE,
      'splitpayment' => TRUE,
    );

    $form['merchant'] = array(
      '#type' => 'textfield',
      '#title' => t('Merchant number'),
      '#description' => t('Merchant id as shown in the QuickPay admin. NOTE: <em>not</em> the same as PBS id'),
      '#default_value' => $settings['merchant'],
      '#required' => TRUE,
    );

    $form['secret'] = array(
      '#type' => 'textfield',
      '#title' => t('MD5 secret'),
      '#description' => t('MD5 secret as shown in the Quickpay admin.'),
      '#default_value' => $settings['secret'],
      '#required' => TRUE,
    );

    $form['order_prefix'] = array(
      '#type' => 'textfield',
      '#title' => t('Order id prefix'),
      '#description' => t('Prefix for order ids. Order ids must be uniqe when sent to QuickPay, use this to resolve clashes.'),
      '#default_value' => $settings['order_prefix'],
      '#element_validate' => array('quickpay_order_prefix_validate'),
    );

    $languages = self::get_languages() + array(LANGUAGE_NONE => t('Language of the user'));

    $form['language'] = array(
      '#type' => 'select',
      '#title' => t('Language'),
      '#description' => t('The language for the credit card form.'),
      '#options' => $languages,
      '#default_value' => $settings['language'],
    );

    $form['accepted_cards'] = array(
      '#type' => 'quickpay_payment_method',
      '#default_value' => $settings['accepted_cards'],
    );

    $form['splitpayment'] = array(
      '#type' => 'checkbox',
      '#title' => t('Split payments'),
      '#description' => t('Allows for capturing payments in parts.'),
      '#default_value' => $settings['splitpayment'],
    );

    $form['test_mode'] = array(
      '#type' => 'checkbox',
      '#title' => t('Test mode'),
      '#description' => t('When active, transactions will be run in test mode, even if the QuickPay account is in production mode. Order ids will get a T appended.'),
      '#default_value' => $settings['test_mode'],
    );

    $form['debug'] = array(
      '#type' => 'checkbox',
      '#title' => t('Debug log'),
      '#description' => t('Log all request and responses to QuickPay in watchdog.'),
      '#default_value' => $settings['debug'],
    );

    return $form;
  }
  /**
   * Get information about an currency.
   *
   * @param string $code
   *   The ISO 4217 currency code.
   *
   * @return NULL|array
   *   An array with the keys 'code' and 'multiplier', or null if not found.
   */
  public static function currency_info($code) {
    // @todo if commerce module is enabled, use its list.
    if (!array_key_exists($code, Quickpay::$currency_info)) {
      // Use a basic set.
      $base_currencies = array(
        'DKK' => array('code' => 'DKK', 'multiplier' => 100),
        'USD' => array('code' => 'USD', 'multiplier' => 100),
        'EUR' => array('code' => 'EUR', 'multiplier' => 100),
        'GBP' => array('code' => 'GBP', 'multiplier' => 100),
        'SEK' => array('code' => 'SEK', 'multiplier' => 100),
        'NOK' => array('code' => 'NOK', 'multiplier' => 100),
        'ISK' => array('code' => 'ISK', 'multiplier' => 100),
      );

      Quickpay::$currency_info += $base_currencies;
      // If still not found, throw an exception.
      if (!array_key_exists($code, Quickpay::$currency_info)) {
        throw new QuickpayException(t('Unknown currency code %currency', array('%currency' => $code)));
      }
    }
    return Quickpay::$currency_info[$code];
  }

  /**
   * Returns the amount adjusted by the multiplier for the currency.
   *
   * @param decimal $amount
   *   The amount.
   * @param array|string $currency_info
   *   An currency_info() array, or a currency code.
   */
  public static function wire_amount($amount, $currency_info) {
    if (!is_array($currency_info)) {
      $currency_info = Quickpay::currency_info($currency_info);
    }
    return (function_exists('bcmul') ?
        bcmul($amount, $currency_info['multiplier']) :
        $amount * $currency_info['multiplier']);

  }

  /**
   * Reverses wire_amount().
   *
   * @param int $amount
   *   The amount.
   * @param array|string $currency_info
   *   An currency_info() array, or a currency code.
   */
  public static function unwire_amount($amount, $currency_info) {
    if (!is_array($currency_info)) {
      $currency_info = Quickpay::currency_info($currency_info);
    }
    return (function_exists('bcdiv') ?
        bcdiv($amount, $currency_info['multiplier'], log10($currency_info['multiplier'])) :
        $amount / $currency_info['multiplier']);

  }
}

/**
 * Abstracts a transaction.
 */
class QuickpayTransaction {
  // State codes.
  const INITIAL = 0;
  const AUTHORIZED = 1;
  const CAPTURED = 3;
  const CANCELLED = 5;
  const REFUNDED = 7;
  const SUBSCRIBED = 9;

  protected $qp;
  protected $data;
  protected $loaded = FALSE;

  protected static $fields = array(
    'msgtype',
    'ordernumber',
    'amount',
    'currency',
    'time',
    'state',
    'qpstat',
    'qpstatmsg',
    'chstat',
    'chstatmsg',
    'merchant',
    'merchantemail',
    'transaction',
    'cardtype',
    'cardnumber',
    'cardexpire',
    'splitpayment',
    'fraudprobability',
    'fraudremarks',
    'fraudreport',
    'fee',
    'md5check',
    'history',
  );

  /**
   * Create a transaction object.
   */
  public function __construct(Quickpay $qp, $txnid = NULL) {
    $this->qp = $qp;
    foreach (QuickpayTransaction::$fields as $field) {
      $this->data[$field] = NULL;
    }
    if (preg_match('/^a:/', $txnid)) {
      list($this->data['transaction'], $this->data['currency']) = unserialize($txnid);
    }
    else {
      $this->data['transaction'] = $txnid;
    }
  }

  /**
   * Magic method.
   */
  public function __get($field) {
    if (!$this->loaded) {
      $this->status();
    }
    if ($field == 'currency' || $field == 'amount' || $field == 'history') {
      return $this->{$field}();
    }
    if (array_key_exists($field, $this->data)) {
      return $this->data[$field];
    }
    throw new QuickpayException(t("Unknown property @name", array('@name' => $field)));
  }

  /**
   * Magic method.
   */
  public function __isset($field) {
    if ($field != 'transaction' && !$this->loaded) {
      $this->status();
    }
    if (isset($this->data[$field])) {
      return $this->data[$field];
    }

  }

  /**
   * Returns the currency for this transaction.
   */
  public function currency() {
    if (!$this->loaded) {
      $this->status();
    }
    // @todo call status command if not set.
    if ($this->data['currency']) {
      return $this->data['currency'];
    }
    throw new QuickpayException(t('No currency defined for transaction @txn_id', array('@txn_id' => $this->data['transaction'])));
  }

  /**
   * Returns the amount for this transaction.
   */
  public function amount() {
    if (!$this->loaded) {
      $this->status();
    }
    if ($this->data['amount']) {
      return Quickpay::unwire_amount($this->data['amount'], $this->currency());
    }
    throw new QuickpayException(t('No amount for transaction @txn_id', array('@txn_id', $this->data['transaction'])));
  }

  /**
   * Returns the history for this transaction.
   */
  public function history() {
    if (!$this->loaded || !isset($this->history)) {
      $this->status();
    }
    if ($this->data['history']) {
      return $this->data['history'];
    }
    return array();
  }

  /**
   * Return the balance for this transaction.
   */
  public function balance() {
    $history = $this->history();
    $balance = 0;
    foreach ($history as $hist) {
      if ($hist['msgtype'] == 'authorize' && $hist['qpstat'] == '000') {
        $balance = $hist['amount'];
      }
      elseif ($hist['msgtype'] == 'capture' && $hist['qpstat'] == '000') {
        $balance -= $hist['amount'];
      }
      elseif ($hist['msgtype'] == 'refund' && $hist['qpstat'] == '000') {
        $balance += $hist['amount'];
      }
    }
    return Quickpay::unwire_amount($balance, $this->currency());
  }
  /**
   * Extract named object properties into an associative array.
   */
  public function extract() {
    // Safety, don't attempt to extract on empty transactions.
    if (empty($this->data['transaction'])) {
      throw new QuickpayException("No transaction id.");
    }
    // PHP 5.2 can't pass the result of func_get_args() directly to another
    // function, so we use a variable.
    $args = func_get_args();
    return array_intersect_key($this->data, array_flip($args));
  }

  /**
   * Returns the amount adjusted by the multiplier for the currency of this
   * transaction.
   */
  public function wire_amount($amount = NULL) {
    if (!$amount) {
      return $this->data['amount'];
    }
    return Quickpay::wire_amount($amount, $this->currency());
  }

  /**
   * Load a request response.
   */
  public function load_response(array $response) {
    if (!isset($response['qpstat'])) {
      throw new QuickpayException(t('No state from Quickpay'));
    }
    $success = $response['qpstat'] == '000';
    foreach ($this->data as $field => $dummy) {
      if (isset($response[$field])) {
        // Only overwrite with empty values when the request was a
        // success. Quickpay returns empty fields on failure.
        if ($success || !empty($response[$field])) {
          $this->data[$field] = $response[$field];
        }
      }
    }
    $this->loaded = TRUE;
  }

  /**
   * Encode the minimal needed information about this transaction, for later
   * loading. This is what modules using Quickpay should use as a reference.
   */
  public function store() {
    if ($this->data['currency']) {
      return serialize(array($this->data['transaction'], $this->data['currency']));
    }
    return $this->transaction;
  }

  /**
   * Capture on an authorised transaction.
   */
  public function capture($amount, $finalize = FALSE) {
    $this->qp->request('capture', $this->extract('transaction') + array('amount' => $this->wire_amount($amount), 'finalize' => $finalize ? '1' : '0'), $this);
  }

  /**
   * Update this transaction with status information from Quickpay.
   */
  public function status() {
    $this->qp->request('status', $this->extract('transaction'), $this);
    // Throw an exception if Quickpay complains.
    if (!$this->success()) {
      watchdog('quickpay', 'Error from Quickpay on status request. Message: @message',
      array('@message' => $this->data['qpstatmsg']), WATCHDOG_ERROR);
      throw new QuickpayException(t("Status error."));
    }
  }

  /**
   * Renew an authorisation.
   */
  public function renew() {
    $this->qp->request('renew', $this->extract('transaction'), $this);
  }

  /**
   * Cancel an authorised transaction.
   */
  public function cancel() {
    $this->qp->request('cancel', $this->extract('transaction'), $this);
  }

  /**
   * Refund previously captured money.
   */
  public function refund($amount) {
    $this->qp->request('refund', $this->extract('transaction') + array('amount' => $this->wire_amount($amount)), $this);
  }

  /**
   * Create a new payment on a subscription transaction.
   */
  function recurring($order_id, $amount, $currency, $options = array()) {
    $options += array(
      'autocapture' => FALSE,
    );
    $currency_info = Quickpay::currency_info($currency);
    $request_data = array(
      'transaction' => $this->data['transaction'],
      'ordernumber' => $this->qp->ordernumber($order_id),
      'amount' => Quickpay::wire_amount($amount, $currency_info),
      'currency' => $currency_info['code'],
      'autocapture' => $options['autocapture'] ? '1' : '0',
    );
    $transaction = new QuickpayTransaction($this->qp);
    return $this->qp->request('recurring', $request_data, $transaction);
  }

  /**
   * Whether this transaction was a success.
   */
  public function success() {
    if (!isset($this->data['qpstat'])) {
      return FALSE;
    }
    return $this->data['qpstat'] == '000';
  }
}

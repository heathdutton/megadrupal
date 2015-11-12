<?php

/**
 * @file
 * PayPoint's Advanced Payment class.
 */

/**
 * Class implementing PayPoint's 3rd generation PSP payment service referred as Advanced Payments.
 */
class PPOAdvancedPayment {

  private $url = PAYPOINT_ADV_MITE_HOSTED_ENDPOINT;
  private $instId;
  private $hosted = TRUE;
  private $username, $password;
  private $request, $response; // Last request/response.

  // Default request objects.
  private $transaction  = array();
  private $customer     = array();
  private $customFields = array(); // @todo: Needs setters.
  private $locale       = array();
  private $session      = array();
  private $features     = array();
  // private $financialServices = array(); // @todo: Mandatory for Fin-Services.
  /**
   * Constructor to initialize PayPoint's class instance.
   *
   * @param
   *  $url string mandatory
   *  $username string mandatory
   *    The username for the PayPoint transaction.
   *  $password string mandatory
   *    The password for the PayPoint transaction.
   *  $instId string mandatory
   *    The installation identifier which you will need to submit with each request.
   *  $hosted string
   *    When TRUE, your access to PayPoint services is via Hosted Cashier solution, otherwise it's via an API or using an SDK.
   */

  function __construct($url, $username, $password, $instId, $hosted = TRUE) {
    $this->url = $url ?: PAYPOINT_ADV_MITE_HOSTED_ENDPOINT;
    $this->hosted = $hosted ?: TRUE;
    $this->username = $username;
    $this->password = $password;
    $this->instId = (int) $instId;
  }

  /**
   * REQUESTS .**/

  /**
   * Test connectivity to Intelligent Commerce using the Ping request.
   */

  function sendPing() {
    return $this->sendRawRequest(
        $this->hosted ? '/hosted/rest/sessions/ping' : '/acceptor/rest/transactions/ping',
        array(),
        'GET');
  }

  /**
   * Make a payment.
   *
   * Submit customer payments initiated online or over the phone using the Payment Request
   * for a one-step authorisation and capturing of funds. The request can additionally
   * be processed with 3D Secure. Configuring your account for 3D Secure can be performed
   * at boarding or at a later date – it’s up to you.
   *
   * Notes:
   *   -  Authorisations expire by default after 7 days. After this time, you won’t be able to Capture or Cancel the request.
   *
   * @see: https://developer.paypoint.com/payments/docs/#payments/make_a_payment
   */

  function sendProcessPayment() {

    $request = array();
    !empty($this->transaction)    && $request['transaction']  = $this->transaction;
    !empty($this->customFields)   && $request['customFields'] = $this->customFields;
    !empty($this->customer)       && $request['customer']     = $this->customer;
    !empty($this->locale)         && $request['locale']       = $this->locale;
    !empty($this->session)        && $request['session']      = $this->session;
    !empty($this->features)       && $request['features']     = $this->features;

    return $this->sendRawRequest(
      $this->hosted ? "/hosted/rest/sessions/{$this->instId}/payments" : "/acceptor/rest/transactions/{$this->instId}/payment",
      $request
    );
  }

  /**
   * Send raw request to PayPoint end point.
   *
   *  Example hosted response:
   *  {
   *    sessionId string Our ID for the hosted session.
   *    redirectUrl string The URL you should direct your customer to to start the hosted session.
   *    status string Possible Values: SUCCESS, FAILED. Indicates the status of the session creation.
   *    reasonCode string Further information about the status of the session creation.
   *    reasonMessage string Further information about the status of the session creation.
   *                         This is where we will provide detailed information about any errors.
   *  }
   *
   */

  function sendRawRequest($uri, $data, $method = 'POST') {

    $headers = array(
      'Accept' => 'application/json',
      'Content-Type'  => 'application/json',
      'Authorization' => "Basic " . base64_encode($this->username . (!empty($this->password) ? ":" . $this->password : '')),
    );

    $this->request = is_array($data) ? json_encode($data) : $data;
    if (function_exists('drupal_http_request')) {
      $response = drupal_http_request($this->url . $uri, array(
        'headers' => $headers,
        'method' => $method,
        'data' => $this->request,
      ));
    }
    else {
      // @todo: Use curl for non-Drupal usage.
    }
    if ($response->code == 201) {
      unset($response->error); // @fixme: Workaround for Drupal bug: https://www.drupal.org/node/2350033, fixed in: D7.40
    }
    $this->response = $response;
    if (!empty($response->error)) {
      throw new Exception($response->error, $response->code);
    }
    return $response;
  }

  /**
   *
   */
  function paymentRequest() {
    // @todo
  }

  /**
   * GETTERS .**/

  /**
   * Return last response data.
   */

  function getLastData() {
    return $this->response ? json_decode($this->response->data) : FALSE;
  }

  /**
   * Return last response numeric code.
   */

  function getLastCode() {
    return $this->response ? $this->response->code : FALSE;
  }

  /**
   * Return last response reason code.
   */

  function getLastReasonCode() {
    $data = $this->getLastData();
    return (!empty($data->reasonCode) ? $data->reasonCode : "UNKNOWN");
  }

  /**
   * Return last response status message.
   */

  function getLastStatus() {
    $data = $this->getLastData();
    return (!empty($data->status) ? $data->status : "UNKNOWN");
  }

  /**
   * Return last request object.
   */

  function getLastRequest() {
    return $this->request ? $this->request : FALSE;
  }

  /**
   * Return last response object.
   */

  function getLastResponse() {
    return $this->response ? $this->response : FALSE;
  }

  /**
   * Return transaction money amount and currency.
   *
   * @param
   * $key string
   * Sub-key for getting value from transaction array.
   *
   * @return
   * If $key is specified, return value of selected key, otherwise return the corresponding array.
   */

  function getTransactionMoney($key = NULL) {
    return $key ? $this->transaction['money'][$key] : $this->transaction['money'];
  }

  /**
   * The URL you should direct your customer to to start the hosted session.
   *
   * @return
   *   Return redirect URL from the last response, otherwise return empty string if it's not available.
   */

  function getRedirectUrl() {
    $data = $this->getLastData();
    return !empty($data->redirectUrl) ? $data->redirectUrl : '';
  }

  /**
   * SETTERS .**/

  /**
   * Transaction Setters .**/

  /**
   * Set details of the transaction you want to create for PayPoint hosted session.
   *
   *  @param
   *    $transaction array
   *      Mandatory: The transaction array.
   */

  function setTransaction($transaction) {
    $this->transaction = $transaction;
  }

  /**
   * Set reference for the transaction for PayPoint hosted session.
   *
   * @param
   *  $merchantReference string
   *    Your reference for the transaction.
   */

  function setTransactionMerchantReference($merchantReference) {
    $this->transaction['merchantReference'] = $merchantReference;
  }

  /**
   * The amount and currency of your Customer's transaction for PayPoint hosted session.
   */

  function setTransactionMoneyFixed($amount, $currency = 'GBP') {
    $this->transaction['money']['amount'] = array('fixed' => $amount);
    $this->transaction['money']['currency'] = $currency;
  }

  /**
   * Set description of the transaction you want to create.
   *
   * @param
   * $description string
   * The description of the transaction.
   */

  function setTransactionDescription($description) {
    $this->transaction['description'] = $description;
  }

  /**
   * Customer Setters .**/

  /**
   * Set customer details for PayPoint hosted session.
   *
   *  @param
   *    $customer array
   *      Mandatory: The customer array.
   *
   */

  function setCustomer($customer) {
    $this->customer = $customer;
  }

  /**
   * Set platform customer identity for PayPoint hosted session.
   *
   * @param
   *  $platformCustomerId string
   *    Our ID for your customer.
   */

  function setCustomerPlatformId($id) {
    // Set new customer for PayPoint hosted session.
    $this->customer['registered'] = TRUE;
    $this->customer['identity']['platformCustomerId'] = $id;
  }

  /**
   * Set merchant customer identity for PayPoint hosted session.
   *
   * @param
   *  $merchantCustomerId string
   *    Your ID for the customer.
   */

  function setCustomerMerchantId($id) {
    // Set new customer for PayPoint hosted session.
    $this->customer['registered'] = TRUE;
    $this->customer['identity']['merchantCustomerId'] = $id;
  }

  /**
   * Set customer details for PayPoint hosted session.
   *
   * @param
   *  $name string Mandatory
   *    The Customer's name.
   *  $address array of strings
   *    The Customer's address consisting of the following fields:
   *      line1 string
   *        Line 1 of the Customer's billing address.
   *      line2 string
   *        Line 2 of the Customer's billing address.
   *      line3 string
   *        Line 3 of the Customer's billing address.
   *      line4 string
   *        Line 4 of the Customer's billing address.
   *      city string
   *        City of the Customer's billing address.
   *      region string
   *        Region of the Customer's billing address.
   *      postcode string
   *        Post Code of the Customer's billing address.
   *      countryCode string
   *        The 3 character ISO-3166-1 code for the Customer's billing address country.
   *  $telephone string
   *    Telephone number for the Customer.
   *  $emailAddress string
   *    Email address for the Customer.
   *  $ipAddress string
   *    The Customer's IP address.
   *
   * Example structure:
   *  'details' => array (
   *    'name' => 'Mr Foo Bar',
   *    'address' => array (
   *      'line1' => 'Address Line 1',
   *      'line2' => 'Address Line 2',
   *      'city' => 'City',
   *      'region' => 'Region',
   *      'postcode' => 'AVS111',
   *      'countryCode' => 'GBR',
   *    ),
   *    'telephone' => '0044111111111',
   *    'emailAddress' => 'example@example.com',
   *    'ipAddress' => '127.0.0.1',
   *    'defaultCurrency' => 'GBP',
   *  ),
   *
   */

  function setCustomerDetails($name, $address, $telephone, $emailAddress, $ipAddress) {
    $this->customer['details']['name'] = $name;
    $this->customer['details']['address'] = $address;
    $this->customer['details']['telephone'] = $telephone;
    $this->customer['details']['emailAddress'] = $emailAddress;
    $this->customer['details']['ipAddress'] = $ipAddress;
  }

  /** Custom Fields Setters **/

  /**
   * Set custom fields for PayPoint hosted session.
   *
   *  @param
   *    $customFields array
   *      Mandatory: The array containing custom fields.
   *      Example:
   *        customFields {
   *          dataFieldOrTextFieldOrLabelField [ array
   *            {
   *              name string Mandatory
   *              value string
   *              transient boolean
   *            }
   *
   *          ]
   *
   *        }
   *
   */

  function setCustomFields($customFields) {
    $this->customFields = $customFields;
  }

  /**
   * Session Setters .**/

  /**
   * Set session for PayPoint hosted session.
   *
   *  @param
   *    $session array
   *      Mandatory: The session array.
   */
  function setSession($session) {
    $this->session = $session;
  }

  /*
   * Set returnUrl for PayPoint hosted session.
   *
   *  @param
   *    $returnUrl string Mandatory
   *      Mandatory: The URL that we will return your customer after the processing the transaction.
   */
  function setSessionReturnUrl($returnUrl) {
    $this->session['returnUrl']['url']  = $returnUrl;
  }

  /*
   * Set cancelUrl for PayPoint hosted session.
   *
   *  @param
   *    $cancelUrl string Mandatory
   *      The URL that we will return your customer to if they cancel the hosted session.
   *      If omitted the returnUrl is used if they cancel.
   */
  function setSessionCancelUrl($cancelUrl) {
    $this->session['cancelUrl']['url']  = $cancelUrl;
  }

  /*
   * Set restoreUrl for PayPoint hosted session.
   *
   *  @param
   *    $restoreUrl string Mandatory
   *      The URL of PayPoint hosted session.
   *      We'll take your customer to this URL in the event they cancel their PayPal payment
   *      with the "Cancel and return to...." link. You will need to use this field if you iFrame our hosted product.
   *      You will need to use this field if you iFrame the hosted product.
   */
  function setSessionRestoreUrl($restoreUrl) {
    $this->session['restoreUrl']['url'] = $restoreUrl;
  }

  /**
   * Set preAuthCallback and format for PayPoint hosted session.
   * Details of the callback made before the transaction is sent for authorisation.
   *
   * @todo: Needs testing if works.
   *
   *  @param
   *    $url string Mandatory
   *      The URL you want the callback or notification to be sent to.
   *      This will override any defaults set on your account.
   *      Where a default is set and a blank URL field is specified, no callback or notification will be sent.
   *    $format string
   *      Possible Values: REST_XML, REST_JSON
   *      The format of the callback content.
   */

  function setSessionPreAuthCallbackUrl($url, $format = 'REST_XML') {
    $this->session['preAuthCallback']['url'] = $url;
    $this->session['preAuthCallback']['format'] = $format;
  }

  /**
   * Set postAuthCallback for PayPoint hosted session.
   * Details of the callback made after the transaction is sent for authorisation.
   *
   * @todo: Needs testing if works.
   *
   *  @param
   *    $url string Mandatory
   *      The URL you want the callback or notification to be sent to.
   *      This will override any defaults set on your account.
   *      Where a default is set and a blank URL field is specified, no callback or notification will be sent.
   *    $format string
   *      Possible Values: REST_XML, REST_JSON
   *      The format of the callback content.
   *
   */
  function setSessionPostAuthCallbackUrl($url, $format = 'REST_XML') {
    $this->session['postAuthCallback']['url'] = $url;
    $this->session['postAuthCallback']['format'] = $format;
  }

  /**
   * Set transactionNotification for PayPoint hosted session.
   * Details of the notification sent after transaction completion.
   * @fixme: Needs testing if works.
   *
   *  @param
   *    $url string Mandatory
   *      The URL you want the callback or notification to be sent to.
   *      This will override any defaults set on your account.
   *      Where a default is set and a blank URL field is specified, no callback or notification will be sent.
   *    $format string
   *      Possible Values: REST_XML, REST_JSON
   *      The format of the callback content.
   */
  function setSessionNotificationUrl($url, $format = 'REST_XML') {
    $this->session['transactionNotification']['url'] = $url;
    $this->session['transactionNotification']['format'] = $format;
  }

  /**
   * Set skin for PayPoint hosted session.
   * The skin used to drive look and feed for this session.
   *
   * @todo: Needs testing if works.
   *
   *  @param
   *    $skin string
   *      The ID of the skin used to drive look and feed for this session.
   *      Refer to Customise hosted look and feel for more information.
   */

  function setSessionSkin($skin) {
    $this->features['skin'] = $skin;
  }

  /**
   * Set features for PayPoint hosted session.
   * Holder of features that can be enabled/disabled during a hosted session.
   *
   * @todo: Needs testing if works.
   *
   *  @param
   *    $paymentMethodRegistration string
   *      Possible Values: always, optional
   *      Allow the customer to choose if they wish their payment method to be registered
   */

  function setFeatures($paymentMethodRegistration) {
    $this->features['paymentMethodRegistration'] = $paymentMethodRegistration;
  }

  /**
   * Translate hosted response code into plain message.
   * Below is a list of the codes you could receive in response to your hosted request.
   *
   *  @param
   *    $reasonCode
   *      The exception/validation code you could receive in response to your hosted request.
   *
   *  @return
   *      Response message.
   */

  function getMessageFromResponse($reasonCode = NULL) {
    if (empty($reasonCode)) {
      $reasonCode = $this->getLastReasonCode();
    }

    switch ($reasonCode) {
      // Hosted response codes.
      case 'exception.authenticationFailure.noAuth':
        return "Authentication Failed";

      case 'exception.authenticationFailure.noPriv':
        return "Authorisation Failed";

      case 'exception.cache.invalidResourcePath':
        return "Invalid Resource Path";

      case 'exception.config.noCustomerCardDomain':
        return "No customer-card domain configured";

      case 'exception.config.noReturnUrl':
        return "No return url provided";

      case 'exception.error.amo':
        return "CSC Address match only";

      case 'exception.error.dcr':
        return "Daily card registrations";

      case 'exception.error.mcr':
        return "Max card register";

      case 'exception.error.ndm':
        return "CSC No data match";

      case 'exception.error.nvd':
        return "Net balance";

      case 'exception.initialiseHosted.noCustomer':
        return "Customer not found";

      case 'exception.installation.lookup':
        return "Invalid installation";

      case 'exception.landing.invalidFlow':
        return "Invalid flow provided";

      case 'exception.manageCards.noRegisteredCards':
        return "No cards registered";

      case 'exception.manageCards.noRegisteredCards.message':
        return "Currently you have no registered cards which are available for manage cards.";

      case 'exception.manageCards.noValidCards':
        return "No cards active";

      case 'exception.payout.noValidCards':
        return "No valid cards for withdrawal";

      case 'exception.payout.noValidCards.message':
        return "Currently you have no registered cards which are available for withdrawal.";

      case 'exception.permission.denied':
        return "Permission Denied";

      case 'exception.permission.denied.message':
        return "You do not have permission to perform that action";

      case 'exception.resumeFailure.invalidSession':
        return "Session invalid for resumption";

      case 'exception.resumeFailure.noSession':
        return "No session found";

      case 'exception.skin.installation':
        return "Skin specified in installation not valid";

      case 'exception.skin.lookup':
        return "No skin configured";

      case 'exception.skin.merchant.lookup':
        return "No merchant configured";

      case 'exception.skin.request':
        return "Skin specified in request not valid";

      case 'exception.config.url.not.valid':
        return "Callback URL is not valid";

      case 'exception.request.contentType':
        return "Content type {0} not supported. Content type application/json required.";

      case 'exception.invalid.email':
        return "Email address provided is not valid";

      // Hosted validation response codes.
      case 'validation.transaction.pan':
        return "Must be 3 or 4 digits";

      case 'validation.transaction.issueNumber':
        return "Issue number must be between 0 and 9";

      case 'validation.transaction.card.selection':
        return "Invalid card selection";

      case 'validation.transaction.card.billingAddress':
        return "Max number of permitted characters is 255";

      case 'validation.transaction.amount.range':
        return "Value must be in range";

      case 'validation.transaction.amount.other':
        return "The amount you entered must be greater than zero";

      case 'validation.transaction.amount.invalidnumber':
        return "The amount you entered must be numeric";

      case 'validation.transaction.amount.invalidcsc':
        return "When a pan is invalid (null or empty, invalid format or length)";

      case 'validation.transaction.amount.csc':
        return "Please provide the CSC number of your credit/debit card.";

      case 'validation.transaction.amount':
        return "You must enter an amount";

      case 'Range.cardRegistration.cardNickname':
        return "Invalid nickname length. Max number of permitted characters is 20.";

      case 'NotEmpty.cardRegistration.cardNickname':
        return "Please enter a nickname for your card. This will help you quickly identify your card.";

      case 'NotEmpty.cardRegistration.cardHolder':
        return "Cardholder name cannot be blank";

      case 'NotBlank.cardRegistration.cardNickname':
        return "Please ensure nickname consists of more than just spaces.";

      case 'label.error.maxCardsLimitReached':
        return "You've reached the maximum number of cards allowed.";

      case 'label.error.cannotDeleteCard':
        return "You don't have permission to delete cards.";

      case 'Invalid.cardRegistration.cardNickname':
        return "Invalid characters. Only latin alphanumeric characters, spaces and . - '' permitted.";
    }
  }

}

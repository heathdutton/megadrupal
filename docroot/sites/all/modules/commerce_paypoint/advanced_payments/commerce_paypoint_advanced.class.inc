<?php

/**
 * @file
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
  // private $financialServices = array(); // @todo: Mandatory for Fin-Services

  function __construct($url, $username, $password, $instId, $hosted = TRUE) {
    $this->url = $url ?: PAYPOINT_ADV_MITE_HOSTED_ENDPOINT;
    $this->hosted = $hosted ?: TRUE;
    $this->username = $username;
    $this->password = $password;
    $this->instId = (int)$instId;
  }

  /** REQUESTS **/

  /*
   * Test connectivity to Intelligent Commerce using the Ping request.
   */
  function sendPing() {
    return $this->sendRawRequest(
        $this->hosted ? '/hosted/rest/sessions/ping' : '/acceptor/rest/transactions/ping',
        array(),
        'GET');
  }

  /*
   * Make a payment.
   *
   * Submit customer payments initiated online or over the phone using the Payment Request
   * for a one-step authorisation and capturing of funds. The request can additionally
   * be processed with 3D Secure. Configuring your account for 3D Secure can be performed
   * at boarding or at a later date – it’s up to you.
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
      $this->hosted ? "/hosted/rest/sessions/$this->instId/payments" : "/acceptor/rest/transactions/$this->instId/payment",
      $request
    );
  }

  /*
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
    $response = drupal_http_request($this->url . $uri, array(
      'headers' => $headers,
      'method' => $method,
      'data' => $this->request,
    ));
    if ($response->code == 201) {
      unset($response->error); # Workaround for Drupal bug: https://www.drupal.org/node/2350033
    }
    $this->response = $response;
    if (!empty($response->error)) {
      throw new Exception($response->error, $response->code);
    }
    return $response;
  }

  function paymentRequest() {
    // @todo
  }

  /** GETTERS **/

  /*
   * Return last response data.
   */
  function getLastData() {
    return $this->response ? json_decode($this->response->data) : FALSE;
  }

  /*
   * Return last response code.
   */
  function getLastCode() {
    return $this->response ? $this->response->code : FALSE;
  }

  /*
   * Return last response status message.
   */
  function getLastStatus() {
    $data = $this->getLastData();
    return (!empty($data->status) ? $data->status : "UNKNOWN");
  }

  /*
   * Return last request object.
   */
  function getLastRequest() {
    return $this->request ? $this->request : FALSE;
  }

  /*
   * Return last response object.
   */
  function getLastResponse() {
    return $this->response ? $this->response : FALSE;
  }

  /** SETTERS **/

  /** Transaction Setters **/

  /*
   * Set details of the transaction you want to create for PayPoint hosted session.
   *
   *  @param
   *    $transaction array
   *      Mandatory: The transaction array.
   */
  function setTransaction($transaction) {
    $this->transaction = $transaction;
  }

  /*
   * Set reference for the transaction for PayPoint hosted session.
   *
   * @param
   *  $merchantReference string
   *    Your reference for the transaction.
   */
  function setTransactionMerchantReference($merchantReference) {
    $this->transaction['merchantReference'] = $merchantReference;
  }

  /*
   * The amount and currency of your Customer's transaction for PayPoint hosted session.
   */
  function setTransactionMoneyFixed($amount, $currency = 'GBP') {
    $this->transaction['money']['amount'] = array('fixed' => $amount);
    $this->transaction['money']['currency'] = $currency;
  }

  /*
   * Set description of the transaction you want to create.
   *
   * @param
   *  $description string
   *    The description of the transaction.
   */
  function setTransactionDescription($description) {
    $this->transaction['description'] = $description;
  }

  /** Customer Setters **/

  /*
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

  /*
   * Set new customer for PayPoint hosted session.
   *
   */
  function setCustomerNew() {
    $this->customer['create'] = TRUE;
  }

  /*
   * Set platform customer identity for PayPoint hosted session.
   *
   * @param
   *  $platformCustomerId string
   *    Our ID for your customer.
   */
  function setCustomerPlatformId($id) {
    $this->customer['identity']['platformCustomerId'] = $id;
  }

  /*
   * Set merchant customer identity for PayPoint hosted session.
   *
   * @param
   *  $merchantCustomerId string
   *    Your ID for the customer.
   */
  function setCustomerMerchantId($id) {
    $this->customer['identity']['merchantCustomerId'] = $id;
  }

  /*
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

  /*
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

  /** Session Setters **/

  /*
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
  function setSessionReturnUrl($url) {
    $this->session['returnUrl']['url']  = $url;
  }

  /*
   * Set cancelUrl for PayPoint hosted session.
   *
   *  @param
   *    $cancelUrl string Mandatory
   *      The URL that we will return your customer to if they cancel the hosted session.
   *      If omitted the returnUrl is used if they cancel.
   */
  function setSessionCancelUrl($url) {
    $this->session['cancelUrl']['url']  = $url;
  }

  /*
   * Set restoreUrl for PayPoint hosted session.
   *
   *  @param
   *    $restoreUrl string Mandatory
   *      The URL of PayPoint hosted session.
   *      We'll take your customer to this URL in the event they cancel their PayPal payment
   *      with the "Cancel and return to...." link. You will need to use this field if you iFrame our hosted product.
   */
  function setSessionRestoreUrl($url) {
    $this->session['restoreUrl']['url'] = $url;
  }

  /*
   * Set preAuthCallback and format for PayPoint hosted session.
   * Details of the callback made before the transaction is sent for authorisation.
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

  /*
   * Set postAuthCallback for PayPoint hosted session.
   * Details of the callback made after the transaction is sent for authorisation.
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

  /*
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

  /*
   * Set skin for PayPoint hosted session.
   * The skin used to drive look and feed for this session.
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

  /*
   * Set features for PayPoint hosted session.
   * Holder of features that can be enabled/disabled during a hosted session.
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

}

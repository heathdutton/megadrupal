<?php

/**
 * @file
 * Creates BluePay class with properties and methods for posting
 * transactions via the BP 2.0 Post Gateway.
 */

/**
 * class CommerceBluePay20Post
 */
class CommerceBluePay20Post {
  /* merchant supplied parameters */
  protected $accountId; // ACCOUNT_ID
  protected $userId; // USER_ID (optional)
  protected $tps; // TAMPER_PROOF_SEAL
  protected $transType; // TRANS_TYPE (AUTH, SALE, REFUND, or CAPTURE)
  protected $payType; // PAYMENT_TYPE (CREDIT or ACH)
  protected $mode; // MODE (TEST or LIVE)
  protected $masterId; // MASTER_ID (optional)
  protected $secretKey; // used to generate the TPS
  /* customer supplied fields, (not required if MASTER_ID is set) */
  protected $account; // PAYMENT_ACCOUNT (i.e. credit card number)
  protected $cvv2; // CARD_CVVS
  protected $expire; // CARD_EXPIRE
  protected $ssn; // SSN (Only required for ACH)
  protected $birthdate; // BIRTHDATE (only required for ACH)
  protected $custId; // CUST_ID (only required for ACH)
  protected $custIdState; // CUST_ID_STATE (only required for ACH)
  protected $amount; // AMOUNT
  protected $name1; // NAME1
  protected $name2; // NAME2
  protected $addr1; // ADDR1
  protected $addr2; // ADDR2 (optional)
  protected $city; // CITY
  protected $state; // STATE
  protected $zip; // ZIP
  protected $country; // COUNTRY
  protected $memo; // MEMO (optinal)
  /* feilds for level 2 qualification */
  protected $orderId; // ORDER_ID
  protected $invoiceId; // INVOICE_ID
  protected $tip; // AMOUNT_TIP
  protected $tax; // AMOUNT_TAX
  /* rebilling (only with trans type of SALE or AUTH) */
  protected $doRebill; // DO_REBILL
  protected $rebDate; // REB_FIRST_DATE
  protected $rebExpr; // REB_EXPR
  protected $rebCycles; // REB_CYCLES
  protected $rebAmount; // REB_AMOUNT
  /* additional fraud scrubbing for an AUTH */
  protected $doAutocap; // DO_AUTOCAP
  protected $avsAllowed; // AVS_ALLOWED
  protected $cvv2Allowed; // CVV2_ALLOWED
  // bluepay response output
  protected $response;
  // parsed response values
  protected $transId;
  protected $status;
  protected $avsResp;
  protected $cvv2Resp;
  protected $authCode;
  protected $message;
  protected $rebid;

  // Level 3 Data
  public $level3Data = array();

  // constants
  const MODE = 'TEST'; // either TEST or LIVE
  const POST_URL = 'https://secure.bluepay.com/interfaces/bp20post'; // the url to post to
  const ACCOUNT_ID = 'INSERT GATEWAY ACCOUNT ID HERE'; // the default account id
  const SECRET_KEY = 'INSERT SECRET KEY HERE'; // the default secret key

  // STATUS response constants
  const STATUS_DECLINE = '0';
  const STATUS_APPROVED = '1';
  const STATUS_ERROR = 'E';

  /**
   * __construct()
   *
   * Constructor method, sets the account, secret key, 
   * and the mode properties. These will default to 
   * the constant values if not specified.
   */
  public function __construct($account = self::ACCOUNT_ID, $key = self::SECRET_KEY, $mode = self::MODE) {
    $this->accountId = $account;
    $this->secretKey = $key;
    $this->mode = $mode;
  }

  /**
   * sale()
   *
   * Will perform a SALE transaction with the amount
   * specified.
   */
  public function sale($amount) {
    $this->transType = "SALE";
    $this->amount = self::formatAmount($amount);
  }

  /**
   * rebSale()
   *
   * Will perform a sale based on a previous transaction.
   * If the amount is not specified, then it will use
   * the amount of the previous transaction.
   */
  public function rebSale($transId, $amount = null) {
    $this->masterId = $transId;
    $this->sale($amount);
  }

  /**
   * auth()
   *
   * Will perform an AUTH transaction with the amount
   * specified.
   */
  public function auth($amount) {
    $this->transType = "AUTH";
    $this->amount = self::formatAmount($amount);
  }

  /**
   * autocapAuth()
   *
   * Will perform an auto-capturing AUTH using the
   * provided AVS and CVV2 proofing.
   */
  public function autocapAuth($amount, $avsAllow = null, $cvv2Allow = null) {
    $this->auth($amount);
    $this->setAutocap();
    $this->addAvsProofing($avsAllow);
    $this->addCvv2Proofing($avsAllow);
  }

  /**
   * addLevel2Qual()
   *
   * Adds additional level 2 qualification parameters.
   */
  public function addLevel2Qual($orderId = null, $invoiceId = null, $tip = null, $tax = null) {
    $this->orderId = $orderId;
    $this->invoiceId = $invoiceId;
    $this->tip = $tip;
    $this->tax = $tax;
  }

  public function addLevel3ItemData($productCode, $unitCost, $quantity, $description, $unit, $commodityCode, $taxAmount, $taxRate, $discount, $total) {
    $acceptableUnits = array('5B', 'ACT', 'ANN', 'AU', 'DAY', 'DZN', 'E49',
      'EA', 'FOT', 'FTQ', 'GLL', 'GRM', 'HUR', 'INH', 'KGM', 'KMT', 'LBR',
      'LTR', 'MIN', 'MMT', 'MON', 'MTQ', 'MTR', 'ONZ', 'SEC', 'SMI', 'STN',
      'TNE', 'WEE', 'XBG', 'XBX', 'XCR', 'XCS', 'XCT', 'XPK', 'XPX', 'XSX',
      'YRD',
    );

    $this->level3Data[] = array(
      'PRODUCT_CODE' => substr($productCode, 0, 12),
      'UNIT_COST' => number_format($unitCost, 2),
      'QUANTITY' => (int) $quantity,
      'ITEM_DESCRIPTOR' => substr($description, 0, 26),
      'MEASURE_UNITS' => in_array($unit, $acceptableUnits) ? $unit : 'EA',
      'COMMODITY_CODE' => substr($commodityCode, 0, 12),
      'TAX_AMOUNT' => number_format($taxAmount, 2),
      'TAX_RATE' => number_format($taxRate, 2), // Four digits??
      'ITEM_DISCOUNT' => number_format($discount, 2),
      'LINE_ITEM_TOTAL' => number_format($total, 2),
    );

  }

  /**
   * void()
   *
   * Will do a refund of a previous transaction.
   */
  public function void($transId) {
    $this->transType = "VOID";
    $this->masterId = $transId;
  }

  /**
   * refund()
   *
   * Will do a refund of a previous transaction.
   */
  public function refund($transId) {
    $this->transType = "REFUND";
    $this->masterId = $transId;
  }

  /**
   * capture()
   *
   * Will capture a pending AUTH transaction.
   */
  public function capture($transId) {
    $this->transType = "CAPTURE";
    $this->masterId = $transId;
  }

  /**
   * rebAdd()
   *
   * Will add a rebilling cycle.
   */
  public function rebAdd($amount, $date, $expr, $cycles) {
    $this->doRebill = '1';
    $this->rebAmount = self::formatAmount($amount);
    $this->rebDate = $date;
    $this->rebExpr = $expr;
    $this->rebCycles = $cycles;
  }

  /**
   * addAvsProofing()
   *
   * Will set which AVS responses are allowed (only
   * applicable when doing an AUTH)
   */
  public function addAvsProofing($allow) {
    $this->avsAllowed = $allow;
  }

  /**
   * addCvv2Proofing()
   *
   * Will set which CVV2 responses are allowed (only
   * applicable when doing an AUTH)
   */
  public function addCvv2Proofing($allow) {
    $this->cvv2Allowed = $allow;
  }

  /**
   * setAutocap()
   *
   * Will turn auto-capturing on (only applicable
   * when doing an AUTH)
   */
  public function setAutocap() {
    $this->doAutocap = '1';
  }

  /**
   * setCustACHInfo()
   *
   * Sets the customer specified info.
   */
  public function setCustACHInfo($routenum, $accntnum, $accttype, $name1, $name2, 
    $addr1, $city, $state, $zip, $country, $phone, $email, $customid1 = null, $customid2 = null,
    $addr2 = null, $memo = null) {

    $this->account = $accttype.":".$routenum.":".$accntnum;
    $this->payType = 'ACH';
    $this->name1 = $name1;
    $this->name2 = $name2;
    $this->addr1 = $addr1;
    $this->addr2 = $addr2;
    $this->city = $city;
    $this->state = $state;
    $this->zip = $zip;
    $this->country = "USA";
    $this->phone = $phone;
    $this->email = $email;
    $this->customid1 = $customid1;
    $this->customid2 = $customid2;
    $this->memo = $memo;
  }

  /**
   * setCustInfo()
   *
   * Sets the customer specified info.
   */
  public function setCustInfo($account, $cvv2, $expire, $name1, $name2, 
    $addr1, $city, $state, $zip, $country, $phone, $email, $customid1 = null, $customid2 = null,
    $addr2 = null, $memo = null) {

    $this->account = $account;
    $this->cvv2 = $cvv2;
    $this->expire = $expire;
    $this->name1 = $name1;
    $this->name2 = $name2;
    $this->addr1 = $addr1;
    $this->addr2 = $addr2;
    $this->city = $city;
    $this->state = $state;
    $this->zip = $zip;
    $this->country = "USA";
    $this->phone = $phone;
    $this->email = $email;
    $this->customid1 = $customid1;
    $this->customid2 = $customid2;
    $this->memo = $memo;
  }

  /**
   * formatAmount()
   *
   * Will format an amount value to be in the
   * expected format for the POST.
   */
  public static function formatAmount($amount) {
    return sprintf("%01.2f", (float)$amount);
  }

  /**
   * setOrderId()
   *
   * Sets the ORDER_ID parameter.
   */
  public function setOrderId($orderId) {
    $this->orderId = $orderId;
  }

  /**
   * calcTPS()
   *
   * Calculates & returns the tamper proof seal md5.
   */
  protected final function calcTPS() {
    $hashstr = $this->secretKey . $this->accountId . $this->transType .
    $this->amount . $this->masterId . $this->name1 . $this->account;

    return bin2hex( md5($hashstr, true) );
  }

  /**
   * processACH()
   *
   * Will first generate the tamper proof seal, then 
   * populate the POST query, then send it, and store 
   * the response, and finally parse the response.
   */
  public function processACH() {
    // calculate the tamper proof seal
    $tps = $this->calcTPS();

    // fill in the fields
    $fields = array (
      'ACCOUNT_ID' => $this->accountId,
      'USER_ID' => $this->userId,
      'TAMPER_PROOF_SEAL' => $tps,
      'TRANS_TYPE' => $this->transType,
      'PAYMENT_TYPE' => $this->payType,
      'MODE' => $this->mode,
      'MASTER_ID' => $this->masterId,

      'PAYMENT_ACCOUNT' => $this->account,
      'BIRTHDATE' => $this->birthdate,
      'CUST_ID' => $this->custId,
      'CUST_ID_STATE' => $this->custIdState,
      'AMOUNT' => $this->amount,
      'NAME1' => $this->name1,
      'NAME2' => $this->name2,
      'ADDR1' => $this->addr1,
      'ADDR2' => $this->addr2,
      'CITY' => $this->city,
      'STATE' => $this->state,
      'ZIP' => $this->zip,
      'PHONE' => $this->phone,
      'EMAIL' => $this->email,
      'COUNTRY' => $this->country,
      'MEMO' => $this->memo,
      'CUSTOM_ID' => $this->customid1,
      'CUSTOM_ID2' => $this->customid2,

      'ORDER_ID' => $this->orderId,
      'INVOICE_ID' => $this->invoiceId,
      'AMOUNT_TIP' => $this->tip,
      'AMOUNT_TAX' => $this->tax,

      'DO_REBILL' => $this->doRebill,
      'REB_FIRST_DATE' => $this->rebDate,
      'REB_EXPR' => $this->rebExpr,
      'REB_CYCLES' => $this->rebCycles,
      'REB_AMOUNT' => $this->rebAmount
    );
	 
    // perform the transaction
    $ch = $this->getCurl($fields);
    $this->response = curl_exec($ch);

    curl_close($ch); 

    // parse the response
    $this->parseResponse();
  }

  /**
   * process()
   *
   * Will first generate the tamper proof seal, then 
   * populate the POST query, then send it, and store 
   * the response, and finally parse the response.
   */
  public function process() {
    // Calculate the TPS.
    $tps = $this->calcTPS();

    // Fill in base fields.
    $fields = array (
      'ACCOUNT_ID' => $this->accountId,
      'USER_ID' => $this->userId,
      'TAMPER_PROOF_SEAL' => $tps,
      'TRANS_TYPE' => $this->transType,
      'PAYMENT_TYPE' => $this->payType,
      'MODE' => $this->mode,
      'MASTER_ID' => $this->masterId,
      'DO_AUTOCAP' => $this->doAutocap,
      'AVS_ALLOWED' => $this->avsAllowed,
      'CVV2_ALLOWED' => $this->cvv2Allowed,
      'ORDER_ID' => $this->orderId,
      'INVOICE_ID' => $this->invoiceId,
    );

    // Fill in account fields.
    if ($this->account != '') {
      $fields = array_merge($fields, array(
        'PAYMENT_ACCOUNT' => $this->account,
        'CARD_CVV2' => $this->cvv2,
        'CARD_EXPIRE' => $this->expire,
        'CUST_ID' => $this->custId,
        'AMOUNT' => $this->amount,
        'NAME1' => $this->name1,
        'NAME2' => $this->name2,
        'ADDR1' => $this->addr1,
        'ADDR2' => $this->addr2,
        'CITY' => $this->city,
        'STATE' => $this->state,
        'ZIP' => $this->zip,
        'PHONE' => $this->phone,
        'EMAIL' => $this->email,
        'COUNTRY' => $this->country,
        'MEMO' => $this->memo,
        'CUSTOM_ID' => $this->customid1,
        'CUSTOM_ID2' => $this->customid2,
      ));
    }

    if ($this->doRebill != '') {
      $fields = array_merge($fields, array(
        'DO_REBILL' => $this->doRebill,
        'REB_FIRST_DATE' => $this->rebDate,
        'REB_EXPR' => $this->rebExpr,
        'REB_CYCLES' => $this->rebCycles,
        'REB_AMOUNT' => $this->rebAmount,
      ));
    }

    // Add Level 3 data if it exists.
    if (count($this->level3Data) > 0) {
      for ($i = 0; $i < count($this->level3Data); $i++) {
        foreach ($this->level3Data[$i] as $name => $value) {
          // Set up each field and increment our counter.
          $fields['LV3_ITEM' . ($i + 1) .'_' . $name] = $value;
        }
      }
    }

    // Perform the transaction.
    $ch = $this->getCurl($fields);
    $this->response = curl_exec($ch);
    curl_close($ch);

    // Return a response object.
    return $this->parseResponse($this->response);
  }

  /**
   * Get a cURL instance optionally populated with fields.
   *
   * @param array $fields
   *   The fields to populate the postfields option with.
   *
   * @return resource
   *   Curl resource.
   */
  public function getCurl($fields = array()) {
    $ch = curl_init();

    curl_setopt($ch, CURLOPT_URL, self::POST_URL);
    curl_setopt($ch, CURLOPT_USERAGENT, "BluePayPHP SDK/2.0");
    curl_setopt($ch, CURLOPT_POST, 1);
    curl_setopt($ch, CURLOPT_RETURNTRANSFER, 1);
    curl_setopt($ch, CURLOPT_POSTFIELDS, http_build_query($fields));

    return $ch;
  }

  /**
   * parseResponse()
   *
   * This method will parse the response parameter values
   * into the respective properties.
   */
  protected function parseResponse($vars) {
    return new BluePay20Response($vars);
  }

  /**
   * get[property]()
   *
   * Getter methods, return the respective property
   * values.
   */
  public function getResponse() {
    return $this->response;
  }
  public function getTransId() {
    return $this->transId;
  }
  public function getStatus() {
    return $this->status;
  }
  public function getAvsResp() {
    return $this->avsResp;
  }
  public function getCvv2Resp() {
    return $this->cvv2Resp;
  }
  public function getAuthCode() {
    return $this->authCode;
  }
  public function getMessage() {
    return $this->message;
  }
  public function getRebid() {
    return $this->rebid;
  }

  /*
   * Provide a basic way to get a scrubbed version of this object for logging.
   */
  public function safeRequest() {
    $vars = get_object_vars($this);
    if (isset($vars['account'])) {
      $vars['account'] = str_repeat('X', strlen($vars['account']) - 4) . substr($vars['account'], -4);
    }
    if (isset($vars['ssn'])) {
      $vars['ssn'] = str_repeat('X', strlen($vars['ssn']) - 4) . substr($vars['ssn'], -4);
    }
    return $vars;
  }
}

class BluePay10Response {
  public $transactionId;
  public $authCode;
  public $paymentType;
  public $avsResponse;
  public $cvv2Response;
  public $message;
  public $result;
  public $rebid;
  public $amount;
  public $invoiceId;
  public $bankName;
  public $paymentAccount;
  public $orderId;
  public $cardExpire;
  public $rrno;
  public $cardType;
  public $status;
  public $transactionType;
  public $data;

  public function __construct($vars) {
    $this->data = $vars;
    if (is_string($vars)) {
      parse_str($vars, $responseVars);
    }
    else {
      $responseVars = $vars;
    }

    $this->transactionType = isset($responseVars['TRANS_TYPE']) ? $responseVars['TRANS_TYPE'] : '';
    $this->transactionId = isset($responseVars['INVOICE_ID']) ? $responseVars['INVOICE_ID'] : '';
    $this->bankName = isset($responseVars['BANK_NAME']) ? $responseVars['BANK_NAME'] : '';
    $this->paymentAccount = isset($responseVars['PAYMENT_ACCOUNT']) ? $responseVars['PAYMENT_ACCOUNT'] : '';
    $this->authCode = isset($responseVars['AUTH_CODE']) ? $responseVars['AUTH_CODE'] : '';
    $this->avsResponse = isset($responseVars['AVS']) ? $responseVars['AVS'] : '';
    $this->cvv2Response = isset($responseVars['CVV2']) ? $responseVars['CVV2'] : '';
    $this->status = isset($responseVars['STATUS']) ? $responseVars['STATUS'] : '';
    $this->message = isset($responseVars['MESSAGE']) ? $responseVars['MESSAGE'] : '';
    $this->rebid = isset($responseVars['REBID']) ? $responseVars['REBID'] : '';
    $this->cardType = isset($responseVars['CARD_TYPE']) ? $responseVars['CARD_TYPE'] : '';
    $this->amount = isset($responseVars['AMOUNT']) ? $responseVars['AMOUNT'] : '';
    $this->cardExpire = isset($responseVars['CARD_EXPIRE']) ? $responseVars['CARD_EXPIRE'] : '';
    $this->orderId= isset($responseVars['ORDER_ID']) ? $responseVars['ORDER_ID'] : '';
    $this->result = isset($responseVars['Result']) ? $responseVars['Result'] : '';
    $this->status = isset($responseVars['STATUS']) ? $responseVars['STATUS'] : ($responseVars['Result'] == 'APPROVED' ? 1 : 0);
    $this->transactionType = isset($responseVars['TRANS_TYPE']) ? $responseVars['TRANS_TYPE'] : '';
  }

  /**
   * @return mixed
   */
  public function getTransactionId() {
    return $this->transactionId;
  }

  /**
   * @return mixed
   */
  public function getAuthCode() {
    return $this->authCode;
  }

  /**
   * @return mixed
   */
  public function getPaymentType() {
    return $this->paymentType;
  }

  /**
   * @return mixed
   */
  public function getAvsResponse() {
    return $this->avsResponse;
  }

  /**
   * @return mixed
   */
  public function getCvv2Response() {
    return $this->cvv2Response;
  }

  /**
   * @return mixed
   */
  public function getMessage() {
    return $this->message;
  }

  /**
   * @return mixed
   */
  public function getResult() {
    return $this->result;
  }

  /**
   * @return mixed
   */
  public function getRebid() {
    return $this->rebid;
  }

  /**
   * @return mixed
   */
  public function getAmount() {
    return $this->amount;
  }

  /**
   * @return mixed
   */
  public function getInvoiceId() {
    return $this->invoiceId;
  }

  /**
   * @return mixed
   */
  public function getBankName() {
    return $this->bankName;
  }

  /**
   * @return mixed
   */
  public function getPaymentAccount() {
    return $this->paymentAccount;
  }

  /**
   * @return mixed
   */
  public function getOrderId() {
    return $this->orderId;
  }

  /**
   * @return mixed
   */
  public function getCardExpire() {
    return $this->cardExpire;
  }

  /**
   * @return mixed
   */
  public function getRrno() {
    return $this->rrno;
  }

  /**
   * @return mixed
   */
  public function getData() {
    return $this->data;
  }

  /**
   * @return mixed
   */
  public function getCardType() {
    return $this->cardType;
  }

  /**
   * @return int
   */
  public function getStatus() {
    return $this->status;
  }

  /**
   * @return string
   */
  public function getTransactionType() {
    return $this->transactionType;
  }

}

class BluePay20Response {
  public $transactionId;
  public $status;
  public $avsResponse;
  public $cvv2Response;
  public $authCode;
  public $message;
  public $rebid;
  public $cardType;
  public $invoiceId;
  public $type;
  public $result;
  public $version;
  public $transactionType;
  public $data;

  public function __construct($vars) {
    $this->data = $vars;
    if (is_string($vars)) {
      parse_str($vars, $responseVars);
    }
    else {
      $responseVars = $vars;
    }

    $this->transactionId = $responseVars['TRANS_ID'];
    $this->status = $responseVars['STATUS'];
    $this->avsResponse = $responseVars['AVS'];
    $this->cvv2Response = $responseVars['CVV2'];
    $this->authCode = $responseVars['AUTH_CODE'];
    $this->message = $responseVars['MESSAGE'];
    $this->rebid = $responseVars['REBID'];
    $this->status =  $responseVars['STATUS'];
    $this->transactionType = $responseVars['TRANS_TYPE'];
  }

  /**
   * @return mixed
   */
  public function getTransactionId() {
    return $this->transactionId;
  }

  /**
   * @return mixed
   */
  public function getStatus() {
    return $this->status;
  }

  /**
   * @return mixed
   */
  public function getAvsResponse() {
    return $this->avsResponse;
  }

  /**
   * @return mixed
   */
  public function getCvv2Response() {
    return $this->cvv2Response;
  }

  /**
   * @return mixed
   */
  public function getAuthCode() {
    return $this->authCode;
  }

  /**
   * @return mixed
   */
  public function getMessage() {
    return $this->message;
  }

  /**
   * @return mixed
   */
  public function getRebid() {
    return $this->rebid;
  }

  /**
   * @return mixed
   */
  public function getCardType() {
    return $this->cardType;
  }

  /**
   * @return mixed
   */
  public function getInvoiceId() {
    return $this->invoiceId;
  }

  /**
   * @return mixed
   */
  public function getType() {
    return $this->type;
  }

  /**
   * @return mixed
   */
  public function getResult() {
    return $this->result;
  }

  /**
   * @return mixed
   */
  public function getVersion() {
    return $this->version;
  }

  /**
   * @return mixed
   */
  public function getTransactionType() {
    return $this->transactionType;
  }
}
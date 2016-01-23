<?php

class PayflowProRecurring {
  protected $pid = null;
  protected $profile_id = null;
  protected $profile_name = null;
  protected $last_name = null;
  protected $profile_term = null;
  protected $profile_start = null;
  protected $profile_end = null;
  protected $profile_payperiod = null;
  protected $profile_amt = null;
  protected $profile_tender = null;
  protected $profile_email = null;
  protected $profile_company_name = null;
  protected $profile_billto = null;
  protected $profile_shipto = null;
  protected $profile_payments_left = null;
  protected $profile_aggregate_amt = null;
  protected $profile_aggregate_trans_amt = null;
  protected $profile_num_failed_payments = null;
  protected $profile_retry_num_days = null;
  protected $profile_next_payment = null;
  protected $profile_exp_data = array();
  protected $profile_status = 'N/A';
  protected $optional_trans_amount = null;
  protected $notify_url = null;

  protected $payment_history = array();
  protected $auth = array();
  protected $mode = null;

  protected $return_code = '';
  protected $return_msg = '';

  private $last_payment_status = '';

  protected $update_items = array();
  protected $create_new = false;
  protected $loaded = false;
  /**
   * Takes a profile ID to load
   */
  function __construct($pid = null, $auth = array()) {
    // Empty constructor
    if($pid != null) {
      $this->profile_id = $pid;
      $this->auth = $auth;
      if(substr($pid,0,2) == 'RT') {
        $this->mode = 'test';
      }
      else {
        $this->mode = 'live';
      }
    }
  }

  function createNew() {
    if($this->loaded) {
      throw new Exception('Attempt to CREATE NEW PROFILE on an already loaded PROFILE');
    }
    $this->create_new = true;
  }

  function setMode($mode) {
    $this->mode = $mode;
  }

  public function refresh() {
    $this->load();
  }
  /**
   * Inquiries about the profile and loads the inforamtion
   * needed about the profile.
   */
  private function load() {
    $profile = $this->runInquiry();
    $profile = (object)$profile->RecurringProfileResult;
    $this->loaded = true;

    // Set the varts
    $this->profile_name = (string)$profile->Name;
    $this->profile_start = (string)$profile->Start;
    $this->profile_end = (string)$profile->End;
    $this->profile_term = (string)$profile->Term;
    $this->profile_status = (string)$profile->Status;
    $this->profile_payperiod = (string)$profile->PayPeriod;
    $this->profile_retry_num_days = (string)$profile->RetryNumDays;
    $this->profile_email = (string)$profile->EMail;
    $this->profile_company_name = (string)$profile->CompanyName;
    $this->profile_amt = (string)$profile->Amt['Currency']; // Hack for bug from PFP
    $this->profile_payments_left = (string)$profile->PaymentsLeft;
    $this->profile_next_payment = (string)$profile->NextPayment;
    $this->profile_aggregate_amt = (string)$profile->AggregateAmt;
    $this->profile_aggregate_trans_amt = (string)$profile->AggregateOptionalTransAmt;
    $this->profile_num_failed_payments = (string)$profile->NumFailedPayments;
    $this->profile_tender = (array)$profile->Tender;
    $this->profile_billto = (array)$profile->BillTo->Address;
    $this->profile_shipto = (array)$profile->ShipTo->Address;
    $this->profile_exp_data = (array)$profile->ExpData;

    // Load Payment History
    $history = $this->runInquiry(true); // True to get the history

    $history = (object)$history->RecurringProfileResult;
    $history = array($history);
    foreach ($history as $payment) {
      if(!isset($payment->RPPaymentResult)) {
        continue;
      }
      $payment = (object)$payment->RPPaymentResult;
      $payment->Amount = (float)$payment->Amt['Currency'];
      $this->payment_history[] = (array)$payment;
    }
    if(count($this->payment_history) === 0) {
      $this->last_payment_status = null;
    }
    else {
      $lp_index = count($this->payment_history) - 1;
      $this->last_payment_status = isset($this->payment_history[$lp_index]['Result']) ? (int)$this->payment_history[$lp_index]['Result'] : null;
    }
    return;
  }

  public function getLastPaymentStatus() {
    return $this->last_payment_status;
  }
  public function getLastPayment() {
    $lp_index = count($this->payment_history) - 1;
    if($lp_index >= 0) {
      return $this->payment_history[$lp_index];
    }
    else {
      return null;
    }
  }

  /**
   * Function to abstract the recurring functions methods
   * of the xml
   */
  private function runInquiry($history = false) {
    $options = array();

    # Build XML
    $transaction = '';
    // Wrap tine inquirty
    $transaction .= $this->getActionInquiry($history);
    // Wrap in a profile
    //$options['custref'] = $this->getProfileID();
    $transaction = $this->profileWrap($transaction, $options);

    // Final wrap
    $xml = $this->recurringXMLPayWrap($transaction);

    // Send XML
    $response = $this->sendTransaction($xml);

    // Return the SimpleXml Object
    return $response;
  }

  /**
   * Cancels this subscription
   */
  public function cancel() {
    # Build XML
    // Get the cancel transaction xml
    $action = $this->getCancelXML();
    // Wrap the cancel transction in a profile
    $transaction = $this->profileWrap($action);

    // Final Wrap
    $xml = $this->recurringXMLPayWrap($transaction);

    // Send XML
    $response = $this->sendTransaction(trim($xml));

    if($response->RecurringProfileResult->Result == 0) {
      return true;
    }
    else {
      $this->return_code = (int)$response->RecurringProfileResult->Result;
      $this->return_msg = $response->RecurringProfileResult->Message;
      return false;
    }
  }

  public function getReturnCode() {
    return $this->return_code;
  }
  public function getReturnMsg() {
    return $this->return_msg;
  }

  /**
   * Saves/Updates this profiles
   * This is only used when creating a new
   * profile.
   */
  public function save() {
    $this->isCreateNew();
    # Build XML
    // Fetch the RPData
    $rpdataxml = $this->getAddXML();

    // Wrap in a profile
    //$options['custref'] = $this->getProfileID();
    $transaction = $this->profileWrap($rpdataxml);

    // Final wrap
    $xml = $this->recurringXMLPayWrap($transaction);

    // Send XML
    $response = $this->sendTransaction($xml);
    // Setup the xml

    return $response;
  }

  private function parsePFPResponse($response, $type = '') {
  }

  function payflowproRecurringFactory($args = array()) {
  }

  private function sendTransaction($xml_request) {
    $xml_request = trim($xml_request);

    $response = _ms_pfp_submit_xml($xml_request, $this->mode);

    return $response;
  }

  function findProfile() {
  }

  private function profileWrap($action, $options = array()) {
    $id_opt = '';
    if(!empty($options['id'])) {
      $id_opt = ' ID=' . $options['id'];
    }
    $custref_opt = '';
    if(!empty($options['custref'])) {
      $custref_opt = ' CustRef="' . $options['custref'] . '"';
    }

    $xml = '<RecurringProfile' . $id_opt . $custref_opt . '>' . $action . '</RecurringProfile>';
    return $xml;
  }

  private function getActionInquiry($history = false) {
    if($history) {
      $xml = '<Inquiry>
                <ProfileID>' . check_plain($this->getProfileID()) . '</ProfileID>
                <PaymentHistory>Y</PaymentHistory>
              </Inquiry>';
    }
    else {
      $xml = '<Inquiry>
                <ProfileID>' . check_plain($this->getProfileID()) . '</ProfileID>
              </Inquiry>';
    }
    return $xml;
  }

  private function getCancelXML() {
    $xml = '<Cancel><ProfileID>' . check_plain($this->getProfileID()) . '</ProfileID></Cancel>';
    return $xml;
  }

  private function getUpdateXML() {
    // RPData is another param, and can be used to modify other info about the profile (not implemented yet).
    // Would need to use $this->update_items to build it, or something similar.
    $xml = '<Modify>' . $this->getTenderXML() . '<ProfileID>' . check_plain($this->getProfileID()) . '</ProfileID></Modify>';
    return $xml;
  }

  /**
   * Call this function to add someone.
   * The getRPDataXML is a HELPER function for this one.
   */
  private function getAddXML() {
    $xml = '<Add>' .
      $this->getTenderXML() .
      $this->getRPDataXML() .
      '</Add>';
    return $xml;
  }

  /**
   * Do NOT call this function directly. This is a helper function
   * to other functions.
   */
  private function getRPDataXML() {
    $optionstrans = '';
    if($this->optional_trans_amount != null) {
      $optionstrans = '<OptionalTrans>Sale</OptionalTrans>
              <OptionalTransAmt>' . check_plain($this->optional_trans_amount) . '</OptionalTransAmt>';
    }
    $xml = '
            <RPData>
              <Name>' . check_plain($this->getName()) . '</Name>
              <TotalAmt>' . number_format($this->getAmt(), 2, '.', '') . '</TotalAmt>
              <Start>' . check_plain($this->getStartDate()) . '</Start>
              <Term>' . check_plain($this->getTerm()) . '</Term>
              <PayPeriod>' . check_plain($this->getPayPeriod()) . '</PayPeriod>
              <EMail>' . check_plain($this->getEmail()) . '</EMail>
              ' . $optionstrans .
              $this->getAddressXML('billto') .
              $this->getAddressXML('shipto') .
            '</RPData>';
    return $xml;
  }
  private function getTenderXML() {
    $tender = $this->profile_tender;
    $cvnum = '';
    if($tender['CVNum']) {
      $cvnum = '<CVNum>' . check_plain($tender['CVNum']) . '</CVNum>';
    }
    if (empty($tender['NameOnCard']))
    {
      $address = $this->getBillTo();
      $tender['NameOnCard'] = $address['Name'];
    }
    $xml = '<Tender>
              <Card>
              <CardNum>' . check_plain($tender['CardNum']) . '</CardNum>
              <ExpDate>' . check_plain($tender['ExpDate']) . '</ExpDate>
              <NameOnCard>' . check_plain($tender['NameOnCard']) . '</NameOnCard>
              ' . $cvnum . '
              </Card>
              </Tender>';
    return $xml;
  }

  /**
   * Currenty just allows updating of CC info, could be extended later.
   *
   * @todo - Add support for modifying the frequency.
   */
  public function update() {
    # Build XML
    // Get the update transaction xml
    $action = $this->getUpdateXML();
    // Wrap the update transction in a profile
    $transaction = $this->profileWrap($action);

    // Final Wrap
    $xml = $this->recurringXMLPayWrap($transaction);

    // Send XML
    $response = $this->sendTransaction(trim($xml));

    if($response->RecurringProfileResult->Result == 0) {
      return true;
    }
    else {
      $this->return_code = (int)$response->RecurringProfileResult->Result;
      $this->return_msg = $response->RecurringProfileResult->Message;
      return false;
    }
  }

  public function setUpdate($item, $val) {
    $this->update_items[$item] = $val;
    return;
  }
  public function clearUpdate() {
    $this->update_items = array();
  }
  /**
   * Returns the array of the update states that will be updated
   * upon running the 'save' function.
   */
  public function getCurrentUpdateState() {
    return $this->update_items;
  }

  public function getProfileID() {
    return $this->profile_id;
  }

  public function parseResults(SimpleXMLElement $result) {
    $this->setStatus((string)$result->Status);
  }

  private function setStatus($status) {
    $this->profile_status = $status;
  }

  public function getName() {
    return $this->profile_name;
  }
  public function setName($name) {
    $this->profile_name = $name;
  }

  public function getLastName() {
    return $this->last_name;
  }
  public function setLastName($name) {
    $this->last_name = $name;
  }

  public function getNotifyUrl() {
    return $this->notify_url;
  }
  public function setNotifyUrl($url) {
    $this->notify_url = $url;
  }

  public function getTerm() {
    return $this->profile_term;
  }

  public function setTerm($term) {
    $term = intval($term);
    if ($term < 0) {
      $term = 0;
    }
    $this->isCreateNew();
    $this->profile_term = $term;
  }

  private function isCreateNew() {
    if(!$this->create_new) {
      throw new Exception('ERROR: NOT IN CORRECT STATE');
    }
    return;
  }

  public function setAuth($auth) {
    $this->auth = (array)$auth;
  }
  public function getEndDate($format = 'raw') {
    if( $this->getStatus() == 'ACTIVE' && $this->getTerm() == 0 && $this->profile_end == '' ) {
      return 'Renewed until user cancels';
    }
    if($format != 'raw') {
      $date = $this->parsePayFlowDate($this->profile_end);
      $formatted_date = format_date($date, 'custom', $format);
    }
    else {
      $formatted_date = $this->profile_end;
    }
    return $formatted_date;
  }
  public function setEnd($end) {
    $this->profile_end = $end;
  }
  public function setStartDate($date) {
    $this->isCreateNew();
    $this->profile_start = $date;
  }

  public function getPayPeriod() {
    return $this->profile_payperiod;
  }
  public function setPayPeriod($period) {
    $this->profile_payperiod = $period;
  }

  public function getAmt() {
    return $this->profile_amt;
  }
  public function setAmt($amt) {
    $this->profile_amt = $amt;
  }

  public function getOptionalTransAmt() {
    return $this->optional_trans_amount;
  }
  public function setOptionalTransAmt($amt) {
    $this->optional_trans_amount = $amt;
  }

  public function getEmail() {
    return $this->profile_email;
  }
  public function setEmail($email) {
    $this->profile_email = $email;
  }

  public function getCompanyName() {
    return $this->profile_company_name;
  }
  public function setCompanyName($name) {
    $this->profile_company_name = $name;
  }

  // Pehraps return these as classes
  public function getBillTo() {
    return $this->profile_billto;
  }
  public function setBillTo($billing) {
    if(!$this->create_new) {
      throw new Exception('Attempt to set billing information out of CREATE NEW mode.');
    }
    $this->profile_billto = $this->fixAddress($billing);
  }
  public function getShipTo() {
    return $this->profile_shipto;
  }
  public function setShipTo($shipping) {
    if(!$this->create_new) {
      throw new Exception('Attempt to set shippin information out of CREATE NEW mode.');
    }
    $this->profile_shipto = $this->fixAddress($shipping);
  }
  function fixAddress($address) {
    // The name may be split into parts when passed in.  Convert into a single field to match
    // what PFP expects us to send and will return later.
    if (isset($address['FirstName']) || isset($address['LastName']) )
    {
      $address['Name'] = trim($address['FirstName']);
      $this->last_name = $address['LastName'];
      unset($address['FirstName'],$address['LastName']);
    }

    // Same goes for the street address: it must be one line (but inputs are a little different).
   if (isset($address['Street2']))
    {
      $address['Street'] = trim($address['Street'] . ' ' . $address['Street2']);
      unset($address['Street2']);
    }

    return $address;
  }

  // Read-only vars
  public function getPaymentsLeft() {
    return $this->profile_payments_left;
  }

  public function getNextPaymentDate($format = 'raw') {
    switch($format) {
      case 'raw':
        return $this->profile_next_payment;

      case 'unix':
        return $this->parsePayFlowDate($this->profile_next_payment);

      default:
        $date = $this->parsePayFlowDate($this->profile_next_payment);
        return format_date($date, 'custom', $format);
    }
  }
  public function getAggregateAmt() {
    return $this->profile_aggregate_amt;
  }
  public function getAggregateOptionalTransAmt() {
    return $this->profile_aggregate_trans_amt;
  }
  public function getNumFailedPayments() {
    return $this->profile_num_failed_payments;
  }
  public function getTender() {
    return $this->profile_tender;
  }
  public function setTender($tender) {
    $this->profile_tender = $tender;
  }
  public function getStatus() {
    return $this->profile_status;
  }
  public function getPaymentHistory() {
    return $this->payment_history;
  }

  public function getStartDate($format = 'raw') {
    if($format != 'raw') {
      $date = $this->parsePayFlowDate($this->profile_start);
      $formated_date = format_date($date, 'custom', $format);
    }
    else {
      $formated_date = $this->profile_start;
    }
    return $formated_date;
  }

  // Helpers
  /**
   * Format a date into Payflow time
   * Types are:
   * - pfp
   * - cc
   */
  private function parsePayFlowDate($date) {
    $date = strtotime(substr($date,0,2) . '/' . substr($date,2,2) . '/' . substr($date,4,4));
    return $date;
  }
  private function getAddressXML($type) {
    if($type == 'billto') {
      $type = 'BillTo';
      $addy = $this->getBillTo();
    }
    if($type == 'shipto') {
      $type = 'ShipTo';
      $addy = $this->getShipTo();
    }

    if(empty($addy) || !is_array($addy)) {
      return ;
    }

    // Note: name / street are pre-combined in this context.
    $xml = '<' . $type . '>
            <Name>' . check_plain($addy['Name']) . '</Name>
            <ExtData Name="LASTNAME" Value="' . check_plain($this->last_name) . '"></ExtData>
            <Phone>' . check_plain($addy['Phone']) . '</Phone>
            <EMail>' . check_plain($addy['EMail']) . '</EMail>
            <CustCode>' . check_plain($addy['CustCode']) . '</CustCode>
            <Address>
              <Street>' . check_plain($addy['Street']) . '</Street>
              <City>' . check_plain($addy['City']) . '</City>
              <State>' .  check_plain($addy['State']) . '</State>
              <Zip>' . check_plain($addy['Zip']) . '</Zip>
              <Country>' . check_plain($addy['Country']) . '</Country>
            </Address>
          </' . $type . '>';
    return $xml;
  }


  /************************************************************
   *
   * Static functions
   *
   ************************************************************
   */
  /**
   * Auth array in the args:
   * - vendor
   * - partner
   * - user
   * - password
   */
  private function recurringXMLPayWrap($transaction) {
     $auth = $this->auth;
     $vendor = $auth['vendor'];
     $partner = $auth['partner'];
     $user = $auth['user'];
     $password = $auth['password'];

     $xml = '<?xml version="1.0" encoding="UTF-8"?>
      <XMLPayRequest Timeout="30" version = "2.0" xmlns="http://www.paypal.com/XMLPay">
        <RequestData>
          <Vendor>' . check_plain($vendor) . '</Vendor>
          <Partner>' . check_plain($partner) . '</Partner>
          <NotifyURL>' . check_plain($this->notify_url) . '</NotifyURL>
          <RecurringProfiles>
          ' . $transaction . '
          </RecurringProfiles>
        </RequestData>
        <RequestAuth>
          <UserPass>
            <User>' . check_plain($user) . '</User>
            <Password>' . check_plain($password) . '</Password>
          </UserPass>
        </RequestAuth>
      </XMLPayRequest>';

      return $xml;
  }

}

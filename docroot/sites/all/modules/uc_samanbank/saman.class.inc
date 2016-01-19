<?php

/**
 * @file Contains the Saman Pank Payment system created for Drupal 7
 * @author "Ahmad Hejazee" <mngafa@gmail.com>
 * 
 * Note: Original work by 
 */

/**
 * Saman Bank Payment System
 */
class SBPayment {                                                                                                                       
//  public $action = 'https://acquirer.samanepay.com/payment.aspx';
//  public $webMethodURL = 'https://acquirer.samanepay.com/payments/referencepayment.asmx?WSDL';
  public $action = 'https://sep.shaparak.ir/Payment.aspx';
  public $webMethodURL = 'https://sep.shaparak.ir/payments/referencepayment1.asmx?WSDL';
  public $redirectURL;
  public $totalAmont;
  public $refNum;
  public $resNum;
  protected $payment;
  protected $merchantID;
  protected $password;
  protected $msg = array();
  protected $errorState ;
  protected $errorVerify;
  public $style;
  
  /**
   * Constructor
   */
  function __construct($mID = '',$pass = '') {
    $this->merchantID = $mID;
    $this->password = $pass; 
    $this->redirectURL = url('cart/saman/complete', array('absolute' => TRUE));
    $this->style = array(
      'TableBorderColor' => variable_get('saman_bank_TableBorderColor', ''),
      'TableBGColor'     => variable_get('saman_bank_TableBGColor', ''),
      'PageBGColor'      => variable_get('saman_bank_PageBGColor', ''),
      'PageBorderColor'  => variable_get('saman_bank_PageBorderColor', ''),
      'TitleFont'        => variable_get('saman_bank_TitleFont', ''),
      'TitleColor'       => variable_get('saman_bank_TitleColor', ''),
      'TitleSize'        => variable_get('saman_bank_TitleSize', ''),
      'TextFont'         => variable_get('saman_bank_TextFont', ''),
      'TextColor'        => variable_get('saman_bank_TextColor', ''),
      'TextSize'         => variable_get('saman_bank_TextSize', ''),
      'TypeTextColor'    => variable_get('saman_bank_TypeTextColor', ''),
      'TypeTextColor'    => variable_get('saman_bank_TypeTextColor', ''),
      'TypeTextSize'     => variable_get('saman_bank_TypeTextSize', ''),
      'LogoURI'          => variable_get('saman_bank_LogoURI', ''),
      'language'         => variable_get('uc_saman_website_language', ''),
    );
    
    $this->errorVerify = array(
      '-1'  => t('internal network error'),
      '-2'  => t('deposits are not equal'),
      '-3'  => t('inputs include invalid characters'),
      '-4'  => t('password or seller id is incorrect'),
      '-5'  => t('database error'),
      '-6'  => t('document has already been sent back'),
      '-7'  => t('digital receipt is empty'),
      '-8'  => t('input length is bigger than allowed'),
      '-9'  => t('invalid characters in returned payment'),
      '-10' => t('digital receipt contains invalid characters'),
      '-11' => t('input length is smaller than allowed'),
      '-12' => t('returned payment is negative'),
      '-13' => t('returned payment of partial payment is more than the amount of not returned digital receipt'),
      '-14' => t('such transaction is not defined'),
      '-15' => t('returned payment is entered in decimal'),
      '-16' => t('internal system error'),
      '-17' => t('to refund a transaction done by non-saman bank cards'),
      '-18' => t('seller IP address is invalid' )
    );
    
    $this->errorState = array(
      'Canceled By User'                     => t('transaction cancelled by user'),
      'Invalid Amount'                       => t('the amount of returned payment is more than main transaction'),
      'Invalid Transaction'                  => t(' while the main transaction is not found,transaction return request is received'),
      'Invalid Card Number'                  => t('card number is incorrect'),
      'No Such Issuer'                       => t('there is not such card registrar available'),
      'Expired Card Pick Up'                 => t('card has been expired'),
      'Incorrect PIN'                        => t('pin number is incorrect'),
      'No Sufficient Funds'                  => t('not enough deposit in your account'),
      'Issuer Down Slm'                      => t('card registrar bank system is not ready'),
      'TME Error'                            => t('error in bank network'),
      'Exceeds Withdrawal Amount Limit'      => t('amount of money is beyond the withdrawal limit'),
      'Transaction Cannot Be Completed'      => t('can not register the document'),
      'Allowable PIN Tries Exceeded Pick Up' => t('card password has been entered incorrectly for 3 times, your card is disabled now'),
      'Response Received Too Late'           => t('bank network transaction time-out'),
      'Suspected Fraud Pick Up'              => t('CCV2 or ExpDate fields is incorrect')
    );
  }
  
  /**
   * Generate a (pseudo-random) unique ResNum
   */
  protected function createResNum() {
    do {
      $m = md5(microtime());
      $resNum = substr($m, 0, 20);
      $sql = "
      SELECT count(res_num) FROM {uc_payment_saman}
      WHERE res_num =':num'
      ";
      $searchC = db_query($sql, array(':num' => $resNum))->fetchField();
      if ($searchC < 1 ) {
        break;
      }
    } while(TRUE);
    
    $this->resNum = $resNum;
  }
  
  /**
   * Search and find a ResNum in database.
   */
  protected function searchResNum($resNum) {
    $query = db_select('uc_payment_saman', 'ps');
    $result = $query->fields('ps')
      ->condition('res_num', $resNum)
      ->execute()
      ->fetchAssoc();
    
    return $result;
  }
  
  /**
   * Search and find a RefNum in database.
   */
  protected function searchRefNum($refNum) {
    $query = db_select('uc_payment_saman', 'ps');
    $result = $query->fields('ps')
      ->condition('ref_num', $refNum)
      ->execute()
      ->fetchAssoc();
    
    return $result;
  }
  
  /**
   * Update and save bank information in the database.
   * 
   * Updates the ref_num and payment in DB based on $this->resNum.
   * 
   * @param $payment
   *   The total paid amount (via bank) (if payment is successfull, this should be equal to
   *   the total amount of cart.)
   */
  protected function saveBankInfo($payment) {
    $this->payment = $payment;
    //TODO: See Original code (error handling)  
    db_update('uc_payment_saman')
      ->fields(array(
        'ref_num' => $this->refNum,
        'payment' => $payment,
      ))
      ->condition('res_num', $this->resNum)
      ->execute();
    
    return TRUE;
  }
  
  /**
   * Insert and save the Store info into the database.
   * 
   * Inserts new record in uc_payment_saman table without the ref_num and payment fields.
   * those two fields will be updated later after payment by the saveBankInfo() method.
   */
  public function saveStoreInfo($totalAmont, $id) {
    if ($totalAmont == '') {
      $this->setMsg("Error: TotalAmont");
      return FALSE;
    }
    
    $time = time();
    $this->totalAmont = $totalAmont;
    
    //check if record already exists (if user refreshes the page)
    $exists = db_select('uc_payment_saman', 'ps')
      ->fields('ps')
      ->condition('id', $id)
      ->execute()
      ->fetchAssoc();
      
    if (!$exists) {
      //does not exist, create new record.
      $this->createResNum();
      
      db_insert('uc_payment_saman')
        ->fields(array(
          'id' => $id,
          'res_num' => $this->resNum,
          'total_amont' => $this->totalAmont,
          'date_start' => $time,
        ))
        ->execute();
    }
    else {
      //record already exists. load it to the object
      $this->resNum = $exists['res_num'];
    }
    
    return TRUE;
  }
  
  /**
   * 
   */
  public function receiverParams($resNum = '', $refNum = '', $state = '' ) {
    if ((empty($state) || empty($resNum) || strlen($refNum) < 20 ) || $state != 'OK') {
      if (isset($this->errorState[$state])) {
        $this->setMsg('state', $state);
      }
      else {
        $this->setMsg("error state");
      }
      
      return FALSE;
    }
    
    $searchResNum = $this->searchResNum($resNum);
    if (is_array($searchResNum)) {
      if ($searchResNum['payment'] > 0) {
        $this->setMsg(t('Please go to pursuit area'));
        return FALSE;
      }
    }
    else {
      $this->setMsg(t('This Transaction does not difined at serverside'));
      return FALSE;
    }
    $this->refNum = $refNum;
    $this->resNum = $resNum;
    $this->totalAmont = $searchResNum['total_amont']; 
    return $this->lastCheck();
  }
  
  /**
   * 
   */
  protected function lastCheck() {
    if (empty($this->resNum) || strlen($this->refNum) < 20) {
      $this->setMsg("Error: resNum or refNum is empty");
      return FALSE;
    }
    
    //web method verify transaction
    $verify = $this->verifyTrans();
    
    //dpm("funstion lastCheck() ==> \$this->verifyTrans() returned: \$verify = " 
    //    . print_r($verify, TRUE));
    
    if ($verify > 0) {
      if ($verify == $this->totalAmont) {
        $this->saveBankInfo($verify);
        $this->setMsg(t('Amount has been successfully paid . please note pursuit code.'));
        $this->setMsg(t('pursuit code : @code', array('@code' => $this->resNum)));
        return TRUE;
      }
      elseif($verify > $this->totalAmont) {
        //web method partial reverse transaction
        $revAmont = $verify - $this->totalAmont;
        $reverse = $this->reverseTrans($revAmont);

        $this->setMsg(t('Dear user, the paid amount is more than requested amount.'));
        if ($reverse == 1) {
          $this->setMsg(t( 'Your extra amount has been returned to your account.'));
          $this->saveBankInfo($this->totalAmont);
        }
        else {
          $this->setMsg('verify',$reverse);
          $this->setMsg(t('Your extra anount will be return to your account early.'));
          $this->saveBankInfo($verify);
        }
        $this->setMsg(t('Amount has been successfully paid . please note pursuit code.'));
        $this->setMsg(t('pursuit code : @code', array('@code'=> $this->resNum)));
        return TRUE;
      }
      elseif($verify < $this->totalAmont) {
        //web method full reverse transaction
        $rev = $this->reverseTrans($verify);
        $this->setMsg(t('Your paid amout is lesser than your order\'s amount.'));
        if ($rev == 1) {
          $this->setMsg(t('Total paid amount has been returned to your account.'));
          $this->saveBankInfo(0);
        }
        else {
          $this->setMsg(t('Total paid amount will be return to your account.'));
          $this->setMsg(t( 'pursuit code : @code', array( '@code'=> $this->resNum)));
          $this->setMsg('verify', $rev);
          $this->saveBankInfo($verify);
        }
        return FALSE;
      }
      //Error transaction
    }
    elseif ($verify < 0 || $verify == FALSE) {
      $this->setMsg(t('Dear user , an error occured for payment'));
      $this->setMsg('verify', $verify);
      $this->saveBankInfo(0);
      return FALSE;
    }
  }
  
  /**
   * Verify transaction
   */
  protected function verifyTrans() {
    if (empty($this->refNum) || empty($this->merchantID)) {
      return FALSE;
    }
    
    $soapClient = new soapclient($this->webMethodURL);
    $soapProxy = $soapClient;
    $result = FALSE;

    for ($a = 1; $a < 6; ++$a) {
      $result = $soapProxy->verifyTransaction($this->refNum, $this->merchantID);
      if ($result != FALSE) {
        break;
      }
    }
    
    return $result;
  }
  
  /**
   * Reverse transaction
   */
  protected function reverseTrans($revNumber) {
    if ($revNumber <= 0 || empty( $this->refNum) || empty($this->merchantID) || empty($this->password)) {
      return FALSE;
    }
    $soapClient = new soapclient($this->webMethodURL);
    $soapProxy = $soapClient;//->getProxy();
    $result = FALSE;

    for ($a = 1; $a < 6; ++$a ) {
      $result = $soapProxy->reverseTransaction($this->refNum, $this->merchantID, $this->password, $revNumber);
      if ($result != FALSE) {
        break;
      }
    }
    
    return $result;
  }
  
  /**
   * 
   */
  public function sendParams() {
    //dpm($this);
    if ($this->totalAmont <= 0 || empty($this->redirectURL) || empty($this->resNum) || empty($this->merchantID)) {
      $this->setMsg("Error: function sendParams()");
      return FALSE;
    }
    
    $form = array(
      'Amount' => $this->totalAmont,
      'ResNum' => $this->resNum ,
      'MID' => $this->merchantID,
      'RedirectURL' => $this->redirectURL,
    );
    
    foreach ($this->style as $key => $val) {
      if ($val != '') {
        $form[$key] = $val;
      }
    }
    
    return $form;
  }
  
  /**
   * Set messages in $this->msg[]
   */
  protected function setMsg($type = '', $index = '') {
    if ($type == 'state' && isset($this->errorState[$index])) {
      $this->msg[] = $this->errorState[$index];
    }
    elseif ($type == 'verify' && isset($this->errorVerify[$index])) {
      $this->msg[] = $this->errorVerify[$index];
    }
    elseif ($type != 'verify' && $type != 'state') {
      $this->msg[] = "$type";
    }
  }
  
  /**
   * Get messages stored in the object as html formatted
   */
  public function getMsg($dis = '') {
    if (count($this->msg) == 0 ) {
      return array();
    }
    
    if ($dis == 'display') {
      $msg = "<ul>\n";
      foreach ($this->msg as $v) {
        $msg .= "<li> $v </li>\n";
      }
      $msg .= "</ul>\n";
      
      return $msg;
    }
    
    return $this->msg;
  }
}

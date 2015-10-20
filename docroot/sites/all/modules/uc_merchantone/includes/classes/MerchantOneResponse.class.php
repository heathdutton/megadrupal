<?php

/**
 * Object-oriented wrapper for Merchant One gateway responses
 *
 * @author noahlively
 */
class MerchantOneResponse {
 
  private $originalvalues;
  
  public $response;
  public $responsetext;
  public $authcode;
  public $transactionid;
  public $avsresponse;
  public $cvvresponse;
  public $orderid;
  public $response_code;
  
  public static function Parse($str) {
    
    
    $arr = array();
    parse_str($str, $arr);
    
    $response = new MerchantOneResponse();
    
    $response->originalvalues = $str;
    
    $response->response = $arr['response'];
    $response->responsetext = $arr['responsetext'];
    $response->authcode = $arr['authcode'];
    $response->transactionid = $arr['transactionid'];
    $response->avsresponse = $arr['avsresponse'];
    $response->cvvresponse = $arr['cvvresponse'];
    $response->orderid = $arr['orderid'];
    $response->response_code = $arr['response_code'];
    
    return $response;    
  }
  
  public function isApproved() {
    return ($this->response == '1');
  }
  
  public function log() {
    watchdog('uc_merchantone', 'Debug response: !data', array('!data' => '<pre>' . check_plain(print_r($this, TRUE)) . '</pre>'));
  }
  
}

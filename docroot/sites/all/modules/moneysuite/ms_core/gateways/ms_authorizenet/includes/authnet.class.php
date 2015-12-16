<?php

class Authnet
{
  private $login;
  private $transkey;
  public $params = array();
  public $results = array();

  public $approved = false;
  public $declined = false;
  public $error = true;

  private $test;
  public $fields;
  public $response;

  static $instances = 0;

  public function __construct($login, $transkey, $test = false) {
    if (self::$instances == 0) {
      $login = trim($login);
      $transkey = trim($transkey);
      $this->test = trim($test);
      if ($this->test) {
        $this->url = "https://test.authorize.net/gateway/transact.dll";
      } else {
        $this->url = "https://secure.authorize.net/gateway/transact.dll";
      }
      $this->params['x_delim_data'] = "TRUE";
      $this->params['x_delim_char'] = "|";
      $this->params['x_relay_response'] = "FALSE";
      $this->params['x_url'] = "FALSE";
      $this->params['x_version'] = "3.1";
      $this->params['x_method'] = "CC";
      $this->params['x_type'] = "AUTH_CAPTURE";
      $this->params['x_login'] = $login;
      $this->params['x_tran_key'] = $transkey;

      self::$instances++;
    } else {
      return false;
    }
  }

  public function transaction($cardnum, $expiration, $amount, $cvv = "", $invoice = "", $tax = "") {
    $this->params['x_card_num'] = trim($cardnum);
    $this->params['x_exp_date'] = trim($expiration);
    $this->params['x_amount'] = trim($amount);
    $this->params['x_po_num'] = trim($invoice);
    $this->params['x_tax'] = trim($tax);
    $this->params['x_card_code'] = trim($cvv);
  }

  public function process($retries = 3) {
    $this->_prepareParameters();
    $ch = curl_init($this->url);

    $count = 0;
    while ($count < $retries) {
      curl_setopt($ch, CURLOPT_HEADER, 0);
      curl_setopt($ch, CURLOPT_RETURNTRANSFER, 1);
      curl_setopt($ch, CURLOPT_SSL_VERIFYPEER, 0);
      curl_setopt($ch, CURLOPT_SSL_VERIFYHOST, 0);
      curl_setopt($ch, CURLOPT_POSTFIELDS, rtrim($this->fields, "& "));
      $this->response = curl_exec($ch);

      if ($this->response) {
        $this->_parseResults();
        if ($this->getResultResponseFull() == "Approved") {
          $this->approved = true;
          $this->declined = false;
          $this->error = false;
          break;
        } elseif ($this->getResultResponseFull() == "Declined") {
          $this->approved = false;
          $this->declined = true;
          $this->error = false;
          break;
        }
      } else {
        $this->approved = false;
        $this->declined = true;
        $this->error = true;
        $this->results[3] = 'cURL Error: ' . curl_error($ch);
        break;
      }
      $count++;
    }
    curl_close($ch);
  }

  private function _parseResults() {
    $this->results = explode("|", $this->response);
  }

  public function setParameter($param, $value) {
    $param = trim($param);
    $value = trim($value);
    $this->params[$param] = $value;
  }

  public function setTransactionType($type) {
    $this->params['x_type'] = strtoupper(trim($type));
  }

  private function _prepareParameters() {
    foreach ($this->params as $key => $value) {
      $this->fields .= "$key=" . urlencode($value) . "&";
    }
  }

  public function getResultResponse() {
    return $this->results[0];
  }

  public function getResultResponseFull() {
    $response = array("", "Approved", "Declined", "Error");
    return $response[(int)$this->results[0]];
  }

  public function isApproved() {
    return $this->approved;
  }

  public function isDeclined() {
    return $this->declined;
  }

  public function isError() {
    return $this->error;
  }

  public function getResponseText() {
    return $this->results[3];
  }

  public function getAuthCode() {
    return $this->results[4];
  }

  public function getAVSResponse() {
    return $this->results[5];
  }

  public function getTransactionID() {
    return $this->results[6];
  }
}


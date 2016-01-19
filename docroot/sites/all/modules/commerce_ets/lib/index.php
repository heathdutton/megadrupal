<?php

  /*
  * Name: Http Helper
  */
  require 'helpers/EasyHttp.php';
  require 'helpers/EasyHttp/Error.php';
  require 'helpers/EasyHttp/Curl.php';
  require 'helpers/EasyHttp/Cookie.php';
  require 'helpers/EasyHttp/Encoding.php';
  require 'helpers/EasyHttp/Fsockopen.php';
  require 'helpers/EasyHttp/Proxy.php';
  require 'helpers/EasyHttp/Streams.php';

  /*
  * Name: Constants
  *   - Aliases to payment types
  */
  define("CREATE_BANK_ACCOUNT", "CREATE_BANK_ACCOUNT_CUSTOMER");
  define("CREATE_CREDIT_ACCOUNT", "CREATE_CREDIT_CARD_CUSTOMER");
  define("CREDIT_CARD", "ACCEPT_CREDIT_CARD_PAYMENT");
  define("BANK_ACCOUNT", "ACCEPT_BANK_ACCOUNT_PAYMENT");
  define("EXISTING_ACCOUNT", "ACCEPT_EXISTING_CUSTOMER_PAYMENT");
  define("SCHEDULE_CREDIT_CARD", "SCHEDULE_CREDIT_CARD_PAYMENT");
  define("SCHEDULE_BANK_ACCOUNT", "SCHEDULE_BANK_ACCOUNT_PAYMENT");
  define("SCHEDULE_EXISTING_ACCOUNT", "SCHEDULE_EXISTING_CUSTOMER_PAYMENT");
  define("ECOM_ROOT_URL", "http://dev.etsdev.com/etsemoney.com/hp");

    // Fail over
    define("ECOM_ROOT_URL_FAILOVER", "https://www.etsemoney.com/hp");

  /*
  * Name: HostedEcom
  */
  class HostedEcom {

      /*
      * Name: Properties
      */
      public $RequestID, 
             $amount, 
             $paymentType, 
             $firstName, 
             $lastName, 
             $accountID, 
             $purchaseIdentifier, 
             $StreetAddress1, 
             $ZipCode, 
             $email, 
             $sendDate, 
             $approvalRedirectURL, 
             $declineRedirectURL, 
             $errorRedirectURL;

      private $result,
              $resStatus,
              $resTransAmount;

      /*
      * Name: Constructor
      * @requestId (string)
      *   - Merchant ID
      */
      function __construct($merchant_id = "") {
          $this->RequestID = $merchant_id;
      }

      /*
      * Name: createSession
      * @requestId (string)
      *   - Merchant ID
      */
      public function createSession() { 

          /*
          * Name: HTTP request
          *  - uses EasyHttp to make POST to create session id
          */
          $url = ECOM_ROOT_URL . "/sessions";
          $http = new EasyHttp();

          $response = $http->request($url, array(
            'method' => 'POST',
            'blocking' => true,
            'headers' => array(),
            'body' => http_build_query($this)
          ));

          /*
          * Name: Trims and Decodes
          *  - toJSON and Whitespace cleanup
          */
          if (!isset($response->errors)){

            $this->result = json_decode(trim($response["body"]));
            
            // adds result to session
            session_start(); 
            $_SESSION["ETS_SESSION_ID"] = $this->result->SessionID;

          }

      }

      /*
      * Name: verifyPayment
      * @requestId (string)
      *   - Merchant ID
      */
      public function verifyPayment($transaction) {

          /*
          * Name: session_start
          *  - adds the hosted-ecom session id to the application session
          */
          session_start(); 
          $session = $_SESSION["ETS_SESSION_ID"];

          /*
          * Name: HTTP request
          *  - uses EasyHttp to make POST w/ transaction id
          */
          $url = ECOM_ROOT_URL. "/confirmation/" . $session;
          $http = new EasyHttp();
          $data = array( 'transactionID' => $transaction );
          $response = $http->request($url, array(
              'method' => 'POST',
              'blocking' => true,
              'headers' => array(),
              'body' => http_build_query($data),
          ));

          /*
          * Name: Trims and Decodes
          *  - toJSON and Whitespace cleanup
          */

		  if (!isset($response->errors)){
              $result = json_decode(trim($response["body"]));
		  }

          /*
          * Name: Failure
          *  - sets response status and amount to negatives
          */
          if(!isset($result)) {
              $this->resStatus = "error";
              $this->resTransAmount = "0.00";
              return;
          }

          /*
          * Name: Success
          *  - sets response status and amount to their correct vales
          */
          $this->resStatus = strtolower($result->Status);
          $this->resTransAmount = strtolower($result->TransactionAmount);

      }

      /*
      * Name: isApproved
      */
      public function isApproved() {
          if ($this->resStatus == "approved" || $this->resStatus == "created" || $this->resStatus == "scheduled") return true;
          else return false;
      }

      /*
      * Name: isAmount
      * @requestId (string)
      *   - transaction amount
      */
      public function isAmount($amount) {
          if ($this->resTransAmount == $amount) return true;
          else return false;
      }

      /*
      * Name: the_session_id
      */
      public function the_session_id() {
          echo ( isset( $this->result->SessionID ) ? $this->result->SessionID : "SESSION_NOT_CREATED");
      }

      /*
      * Name: get_session_id
      */
      public function get_session_id() {
          return ( isset( $this->result->SessionID ) ? $this->result->SessionID : "SESSION_NOT_CREATED");
      }

  }

   


<?php

/**
 * @file
 * Connects to Payment Gateway and sends request.
 */

 $current_wd = getcwd();
 include 'header.php';
 chdir($current_wd);

 $site_base_url = isset($_POST['site_base_url']) ? $_POST['site_base_url'] : '';
 $pg_auth_url = isset($_POST['pg_auth_url']) ? $_POST['pg_auth_url'] : '';
 $pg_dual_url = isset($_POST['pg_dual_url']) ? $_POST['pg_dual_url'] : '';
 $pg_response_url1 = isset($_POST['res_ip_1']) ? $_POST['res_ip_1'] : '';
 $pg_response_url2 = isset($_POST['res_ip_2']) ? $_POST['res_ip_2'] : '';
 $pg_response_url3 = isset($_POST['res_ip_3']) ? $_POST['res_ip_3'] : '';
 $trantrackid = isset($_POST['order_id']) ? $_POST['order_id'] : '';
 $tranamount = isset($_POST['amount']) ? $_POST['amount'] : '';
 $tid = isset($_POST['merchant_id']) ? $_POST['merchant_id'] : '';
 $tpswd = isset($_POST['working_key']) ? $_POST['working_key'] : '';

/* Tranportal ID. */

 $id = "id=" . $tid;

/* Tranportal password. */

 $password = "password=" . $tpswd;

 db_update('payment_hdfc_config')
  ->fields(array('transportal_id' => $tid,
    'transportal_pswd' => $tpswd,
    'site_base_url' => $site_base_url,
    'auth_request_url' => $pg_auth_url,
    'dual_verification_request_url' => $pg_dual_url,
    'response_ip_one' => $pg_response_url1,
    'response_ip_two' => $pg_response_url2,
    'response_ip_three' => $pg_response_url3,
  ))
  ->condition('fixedcol', 1020)
  ->execute();

/* Action Code of the transaction, this refers to type of transaction. Action Code 1 stands of 
Purchase transaction and action code 4 stands for Authorization (pre-auth). Merchant should 
confirm from Bank action code enabled for the merchant by the bank. */

 $action = "action=1";

/* Transaction language, THIS MUST BE ALWAYS USA. */

 $langid = "langid=USA";

/* Currency code of the transaction. By default INR i.e. 356 is configured. If merchant wishes 
to do multiple currency code transaction, merchant needs to check with bank team on the available 
currency code. */

 $currencycode = "currencycode=356";

/* Transaction amount send to payment gateway by merchant for processing. */

$amt = "amt=" . $tranamount;

/* Response URL where Payment gateway will send response once transaction processing is completed 
Merchant MUST esure that below points in Response URL
1- Response URL must start with http://
2- The Response URL SHOULD NOT have any additional paramteres or query string */

 $responseurl = "responseurl=" . $site_base_url . "/" . drupal_get_path('module', 'commerce_hdfc') . "/includes/get_handle_response.php";

/* Error URL where Payment gateway will send response in case any issues while processing the transaction 
Merchant MUST esure that below points in errorurl 
1- Error url must start with http://
2- The error url SHOULD NOT have any additional paramteres or query string.
*/

 $errorurl = "errorurl=" . $site_base_url. "/" . drupal_get_path('module', 'commerce_hdfc') . "/includes/process.php";

/* Merchant track id. */

 $trackid = "trackid=" . $trantrackid;

/* Now merchant sets all the inputs in one string for passing to the Payment Gateway URL. */

 $param = $id . "&" . $password . "&" . $action . "&" . $langid . "&" . $currencycode . "&" . $amt . "&" . $responseurl . "&" . $errorurl . "&" . $trackid;

/* This is Payment Gateway Test URL where merchant sends request. This is test enviornment URL,
production URL will be different and will be shared by Bank during production movement. */

 $url = $pg_auth_url;

/*
Now creating a connection and sending request
Note - PHP CURL function is used for sending TCPIP request.
*/
 $ch = curl_init() or die(curl_error());
 curl_setopt($ch, CURLOPT_POST,1);
 curl_setopt($ch, CURLOPT_POSTFIELDS,$param);
 curl_setopt($ch, CURLOPT_PORT, 443); // Port 443.
 curl_setopt($ch, CURLOPT_URL,$url); // Here the request is sent to payment gateway.
 curl_setopt($ch, CURLOPT_RETURNTRANSFER, 1);
 curl_setopt($ch, CURLOPT_SSL_VERIFYHOST,0); // Create a SSL connection object server-to-server.
 curl_setopt($ch, CURLOPT_SSL_VERIFYPEER,0);
 $data1=curl_exec($ch) or die(curl_error());

 curl_close($ch);

 $response = $data1;
  try
  {

   $index=strpos($response,"!-");
   $ErrorCheck=substr($response, 1, $index-1); // Find Error Keyword in response.

    if($ErrorCheck == 'ERROR') // Check for Error in response.
    {
     $failedurl = $site_base_url . "/" . drupal_get_path('module', 'commerce_hdfc') . '/includes/process.php?Message=Transaction Failed&restrackid=' . $trantrackid . '&resamount=' . $tranamount . '&errortext=' . $response;
     header("location:". $failedurl );
    }

    else
    {

     $i =  strpos($response,":"); // If Payment Gateway response has Payment ID & Pay page URL.

     $paymentid = substr($response, 0, $i); // Updating the Payment ID received with the merchant Track Id in database at this place.

     db_update('commerce_order')
      ->fields(array(
       'pymid' => $paymentid,
      ))
     ->condition('order_number', $trantrackid)
     ->execute();

     $paymentpage = substr( $response, $i + 1);

     $r = $paymentpage . "?PaymentID=" . $paymentid; // Redirecting the customer browser from ME site to Payment Gateway Page with the Payment ID.

     header("location:". $r );
    }

  }

  catch(Exception $e)
  {
   var_dump($e->getMessage());
  }

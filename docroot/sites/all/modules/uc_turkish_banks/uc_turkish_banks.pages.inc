<?php

/**
 * @file
 * Turkish Bank Gateway menu items.
 */

/**
 * Finalizes transaction for 3d hosting payment method.
 */
function uc_turkish_banks_3d_hosting_complete($cart_id = 0) {
  
	watchdog('uc_turkish_banks', t('Receiving new order notification for order !order_id.', array('!order_id' => check_plain($_POST['oid']))));
	
	// siparisi yukle
	$order = uc_order_load($_POST['oid']);
	if ($order == FALSE || uc_order_status_data($order->order_status, 'state') != 'in_checkout') {
		drupal_set_message(t('An error has occurred during payment.  Please contact us to ensure your order has submitted.'));
		return;
	}
	
	//var_dump($_POST);
	
	$mdStatus = $_POST["mdStatus"];
	$ErrMsg = $_POST["mdErrorMsg"];
	
	if($mdStatus == 1 || $mdStatus == 2 || $mdStatus == 3 || $mdStatus == 4)
	{
		
		$hashparams = $_POST["HASHPARAMS"];
		$hashparamsval = $_POST["HASHPARAMSVAL"];
		$hashparam = $_POST["HASH"];
		$storekey="";
		$paramsval="";
		$index1=0;
		$index2=0;
		
		while($index1 < strlen($hashparams))
		{
			$index2 = strpos($hashparams,":",$index1);
			$vl = $_POST[substr($hashparams,$index1,$index2- $index1)];
			if($vl == null)
				$vl = "";
			$paramsval = $paramsval . $vl;
			$index1 = $index2 + 1;
		}
		$storekey = variable_get('3d_hosting_storekey');
		$hashval = $paramsval.$storekey;
		
		$hash = base64_encode(pack('H*',sha1($hashval)));
		
		if($paramsval != $hashparamsval || $hashparam != $hash)
		{
			uc_order_comment_save($order->order_id, 0, t('Güvenlik uyarısı. Sayısal imza geçerli değil'), 'admin');
		}
		
		$message = t('Credit card charged: !amount', array('!amount' => uc_currency_format($_POST['amount'])));
		uc_payment_enter($order->order_id, t('Anlaşmalı Banka'), $_POST["amount"], 0, NULL, $message);
		uc_order_comment_save($order->order_id, 0, $message, 'admin');
		$_SESSION['do_complete'] = TRUE;
		$message = t('Alış veriş için sitemizi seçtiğiniz için teşekkür ederiz.');
		$message .= '<br>';
		$message .= t('İşlem numaranız: '.$OrderId);
		$message .= '<br>';
		$message .= t('Otorizasyon kodunuz: '.$AuthCode);
		$message .= '<br>';
		$message .= t('Tahsilat tutarı: '.uc_currency_format($_POST['amount']));
		// TODO: Buraya daha sonra oturum açarak yaptığı işlemin detaylarını tâkip edebileceği bilgisi yazılmalı
		return $message;
		
		
	}
	else
	{
		$message = t('Ödemeniz <b><font color="RED">GERÇEKLEŞEMEMİŞTİR</font></b>. Lütfen ödeme bilgilerinizi kontrol ederek tekrar deneyiniz.');
		$message .= t('<br/><b>Detay:</b>'.$mdStatus.':'.$ErrMsg.'.');
		drupal_set_message($message);
		$_SESSION['do_review'] = TRUE;
		drupal_goto('cart/checkout/review');
	}
  return $build;
}


function uc_turkish_banks_3d_callback($cart_id = 0) {
  
	watchdog('uc_turkish_banks', t('Receiving new order notification for order !order_id.', array('!order_id' => check_plain($_POST['oid']))),WATCHDOG_INFO);
	global $user;
	$order = uc_order_load($_POST['oid']);
		
	
	$mdStatus = $_POST["mdStatus"];
	$ErrMsg = $_POST["mdErrorMsg"];
	
	if($mdStatus == 1 || $mdStatus == 2 || $mdStatus == 3 || $mdStatus == 4)
	{
		
		$hashparams = $_POST["HASHPARAMS"];
		$hashparamsval = $_POST["HASHPARAMSVAL"];
		$hashparam = $_POST["HASH"];
		$storekey=variable_get('3d_storekey');
		$paramsval="";
		$index1=0;
		$index2=0;
		
		while($index1 < strlen($hashparams))
		{
			$index2 = strpos($hashparams,":",$index1);
			$vl = $_POST[substr($hashparams,$index1,$index2- $index1)];
			if($vl == null)
				$vl = "";
			$paramsval = $paramsval . $vl;
			$index1 = $index2 + 1;
		}
		$hashval = $paramsval.$storekey;
		
		$hash = base64_encode(pack('H*',sha1($hashval)));
		
		if($paramsval != $hashparamsval || $hashparam != $hash)
		{
			uc_order_comment_save($order->order_id, 0, t('Güvenlik uyarısı. Sayısal imza geçerli değil'), 'admin');
		}
		
		
    ///////////////////////////////////////////////
    if (!function_exists('curl_init')) {
			drupal_set_message(t('The Bank service requires cURL.  Please talk to your system administrator to get this configured.'));
			return array('success' => FALSE);
		}
		  
		  //$order = uc_order_load($_SESSION['cart_order']);
		
		
			// XML request sablonu
			$request= "DATA=<?xml version=\"1.0\" encoding=\"ISO-8859-9\"?>".
					"<CC5Request>".
		      "<Name>{APIUSER}</Name>".
		      "<Password>{APIPASSWORD}</Password>".
		      "<ClientId>{CLIENTID}</ClientId>".
		      "<IPAddress>{IP}</IPAddress>".
		      "<oid>{OID}</oid>".
		      "<Type>Auth</Type>".
		      "<Number>{NUMBER}</Number>".
		      "<Amount>{AMOUNT}</Amount>".
		      "<Currency>949</Currency>".
		      "<PayerTxnId>{PAYERTXNID}</PayerTxnId>".
		      "<PayerSecurityLevel>{PAYERSECLEVEL}</PayerSecurityLevel>".
		      "<PayerAuthenticationCode>{PAYERAUTHCODE}</PayerAuthenticationCode>".
		      "</CC5Request>";
		
		
			$request=str_replace("{APIUSER}",variable_get('3d_api_username'),$request);
			$request=str_replace("{APIPASSWORD}",variable_get('3d_api_password'),$request);
			$request=str_replace("{CLIENTID}",variable_get('3d_clientid'),$request);
			$request=str_replace("{IP}",ip_address(),$request);
			$request=str_replace("{OID}",$_POST['oid'],$request);  
			$request=str_replace("{NUMBER}",$_POST['md'],$request);
			$request=str_replace("{AMOUNT}",$_POST['amount'],$request);
			$request=str_replace("{PAYERTXNID}",$_POST['xid'],$request);
			$request=str_replace("{PAYERSECLEVEL}",$_POST['eci'],$request);
			$request=str_replace("{PAYERAUTHCODE}",$_POST['cavv'],$request);
			
			
			// TODO: ?? bu ne olacak? $request=str_replace("{TAKSIT}",$taksit,$request);
		
			$url = variable_get('3d_api_url');
			
			watchdog('uc_turkish_banks', t('Request:'.$request), array(), WATCHDOG_INFO);
		
			$ch = curl_init();    // initialize curl handle
			curl_setopt($ch, CURLOPT_URL,$url); // set url to post to
			//curl_setopt($ch, CURLOPT_VERBOSE, 0);
			//curl_setopt($ch, CURLOPT_POST, 1);
			//curl_setopt($ch, CURLOPT_SSL_VERIFYPEER, 0);
			//curl_setopt($ch, CURLOPT_NOPROGRESS, 1);
			//curl_setopt($ch, CURLOPT_FOLLOWLOCATION,0);
			curl_setopt($ch, CURLOPT_SSL_VERIFYHOST,1);
			curl_setopt($ch, CURLOPT_SSL_VERIFYPEER,0);
			curl_setopt($ch, CURLOPT_SSLVERSION, 3);
			curl_setopt($ch, CURLOPT_RETURNTRANSFER,1); // return into a variable
			curl_setopt($ch, CURLOPT_TIMEOUT, 90); // times out after 4s
			curl_setopt($ch, CURLOPT_POSTFIELDS, $request); // add POST fields
		
			$result = curl_exec($ch); // run the whole process
			watchdog('uc_turkish_banks', t('Response:'.$result),array(), WATCHDOG_INFO);
			if ($error = curl_error($ch)) {
				watchdog('uc_turkish_banks', t($error),array(), WATCHDOG_ERROR);
				
			}
			curl_close($ch);
		
		
		
			$Response ="";
			$OrderId ="";
			$AuthCode  ="";
			$ProcReturnCode    ="";
			$ErrMsg  ="";
			$HOSTMSG  ="";
		
			$response_tag="Response";
			$posf = strpos (  $result, ("<" . $response_tag . ">") );
			$posl = strpos (  $result, ("</" . $response_tag . ">") ) ;
			$posf = $posf+ strlen($response_tag) +2 ;
			$Response = substr (  $result, $posf, $posl - $posf) ;
		
			$response_tag="OrderId";
			$posf = strpos (  $result, ("<" . $response_tag . ">") );
			$posl = strpos (  $result, ("</" . $response_tag . ">") ) ;
			$posf = $posf+ strlen($response_tag) +2 ;
			$OrderId = substr (  $result, $posf , $posl - $posf   ) ;
		
			$response_tag="AuthCode";
			$posf = strpos (  $result, "<" . $response_tag . ">" );
			$posl = strpos (  $result, "</" . $response_tag . ">" ) ;
			$posf = $posf+ strlen($response_tag) +2 ;
			$AuthCode = substr (  $result, $posf , $posl - $posf   ) ;
		
			$response_tag="ProcReturnCode";
			$posf = strpos (  $result, "<" . $response_tag . ">" );
			$posl = strpos (  $result, "</" . $response_tag . ">" ) ;
			$posf = $posf+ strlen($response_tag) +2 ;
			$ProcReturnCode = substr (  $result, $posf , $posl - $posf   ) ;
		
			$response_tag="ErrMsg";
			$posf = strpos (  $result, "<" . $response_tag . ">" );
			$posl = strpos (  $result, "</" . $response_tag . ">" ) ;
			$posf = $posf+ strlen($response_tag) +2 ;
			$ErrMsg = substr (  $result, $posf , $posl - $posf   ) ;
		
			if ($Response != 'Approved') {
				$message = t('Odemeniz <b><font color="RED">GERÇEKLEŞEMEMİŞTİR!</font></b>. Lütfen sepetinizi kontrol ederek tekrar deneyiniz.');
		    $message .= t('<br/><b>Detay:</b>'.$ProcReturnCode.':'.$ErrMsg.'.');
				drupal_set_message($message);
				$_SESSION['do_review'] = TRUE;
				drupal_goto('cart/checkout/review');
			}
			else {
				$message = t('Credit card charged: !amount', array('!amount' => uc_currency_format($_POST['amount'])));
				uc_payment_enter($order->order_id, t('Anlaşmalı Banka'), $_POST["amount"], 0, NULL, $message);
				uc_order_comment_save($order->order_id, 0, $message, 'admin');
				$_SESSION['do_complete'] = TRUE;
				$message = t('Alış veriş için sitemizi seçtiğiniz için teşekkür ederiz.');
				$message .= '<br>';
				$message .= t('İşlem numaranız: '.$OrderId);
				$message .= '<br>';
				$message .= t('Otorizasyon kodunuz: '.$AuthCode);
				$message .= '<br>';
				$message .= t('Tahsilat tutarı: '.uc_currency_format($_POST['amount']));
				// TODO: Buraya daha sonra oturum açarak yaptığı işlemin detaylarını tâkip edebileceği bilgisi yazılmalı
				return $message;
			}
	}
	else
	{
		$message = t('Ödemeniz <b><font color="RED">GERÇEKLEŞEMEMİŞTİR</font></b>. Lütfen ödeme bilgilerinizi kontrol ederek tekrar deneyiniz.');
		$message .= t('<br/><b>Detay:</b>'.$mdStatus.':'.$ErrMsg.'.');
		drupal_set_message($message);
		$_SESSION['do_review'] = TRUE;
		drupal_goto('cart/checkout/review');
	}
}

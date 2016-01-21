<?php
function perfectmoney_sci_prefillform($form, &$form_state) {
  $form=array(); $currencies_ar=array();

	$currency_settings = unserialize(variable_get('perfectmoney_sci_currencies', serialize(_perfectmoney_sci_GetDefCurSetts())));
	foreach($currency_settings as $key=>$value){
		if($value['enabled']==1) $currencies_ar[$key]=$key;
	}

	
	$form['currency'] = array(
		'#type' => 'radios',
		'#title' => t("Currency"),
		'#options' => $currencies_ar,
		'#default_value' => 'USD',
		//'#description' => t("Select payment currency."),
		'#required' => TRUE
	);
 

	$form['amount'] = array(
		'#type' => 'textfield', 
		'#title' => t('Amount'), 
		'#default_value' => '', 
		'#size' => 10, 
		'#maxlength' => 12, 
		'#required' => TRUE
	);

	$form['memo'] = array(
		'#type' => 'textarea', 
		'#title' => t('Memo'), 
		'#default_value' => t('Payment to !sitename', array('!sitename' => variable_get('site_name', 'Drupal'))), 
		'#description' => t("Payment description."),
		'#required' => TRUE
	);


	$form['submit'] = array(
		'#type' => 'submit',
		'#value' => t('Cerate payment'),
	);

	return $form;
}

function perfectmoney_sci_prefillform_submit(&$form, $form_state) {
	$payment=_perfectmoney_sci_createpayment(array(
		'amount'=>$form_state['values']['amount'],
		'currency'=>$form_state['values']['currency'],
		'memo'=>$form_state['values']['memo'],
	));

  if (is_array($payment) && $payment['pid'] > 0) {
    drupal_set_message(t("Please confirm payment details"));
    drupal_goto('perfectmoney/payment/' . $payment['pid']);
  }
}

function perfectmoney_sci_success() {
  return theme('perfectmoney_sci_success');
}

function perfectmoney_sci_fail() {
  return theme('perfectmoney_sci_fail');
}

function perfectmoney_sci_status() {
 
	global $base_path;

	drupal_add_http_header('Content-type', 'text/html; charset=iso-8859-1');

    // check url
	$url = trim($_SERVER['REQUEST_URI'], '/');
	$alias = trim($base_path . drupal_get_path_alias('perfectmoney/status'), '/');
	if ($url != $alias) {
		watchdog('Perfect Money', 'Wrong Result URL accessed "%url"', array("%url" => $url), WATCHDOG_WARNING);
		die("");
	}

	$created=time();

	$string=
		  $_POST['PAYMENT_ID'].':'.$_POST['PAYEE_ACCOUNT'].':'.
		  $_POST['PAYMENT_AMOUNT'].':'.$_POST['PAYMENT_UNITS'].':'.
		  $_POST['PAYMENT_BATCH_NUM'].':'.
		  $_POST['PAYER_ACCOUNT'].':'.strtoupper(md5(variable_get('perfectmoney_sci_secretkey', ''))).':'.
		  $_POST['TIMESTAMPGMT'];

	$hash=strtoupper(md5($string));

	if(preg_match("/^[0-9]{1,10}$/", $_POST['PAYMENT_ID']) && $hash==$_POST['V2_HASH']){ // proccessing payment if only hash is valid

	   /* In section below you must implement comparing of data you recieved
	   with data you sent. This means to check if $_POST['PAYMENT_AMOUNT'] is
	   particular amount you billed to client and so on. */

	   $payment=_perfectmoney_sci_pid_load($_POST['PAYMENT_ID']);
	   if(!is_array($payment)){
			watchdog('Perfect Money', 'Bad PAYMENT_ID, vars "%vars"', array("%vars" => print_r($_POST,TRUE)), WATCHDOG_ALERT);
			die("");
	   }elseif ($payment['enrolled'] > 0) {
			watchdog('Perfect Money', 'Order PAYMENT_ID already enrolled, vars "%invid"', array("%invid" =>  print_r($_POST,TRUE)), WATCHDOG_WARNING);
			die("");
	   }


	   $currency_settings = unserialize(variable_get('perfectmoney_sci_currencies', serialize(_perfectmoney_sci_GetDefCurSetts())));
	   $presc = $currency_settings["{$payment['currency']}"]['presc'];


	   if($_POST['PAYMENT_AMOUNT']==round($payment['amount'], $presc) && $_POST['PAYEE_ACCOUNT']==$payment['payee_account'] && $_POST['PAYMENT_UNITS']==$payment['currency']){

		  /* ...insert some code to proccess valid payments here... */

		  _perfectmoney_sci_enrollpayment($payment['pid'], $_POST['PAYER_ACCOUNT'], $created);

		  die();

	   }else{ // you can also save invalid payments for debug purposes

		  watchdog('Perfect Money', "Can't process order PAYMENT_ID %invid, vars '%vars'", array(
			  "%invid" => $_POST['PAYMENT_ID'],
			  '%vars' => print_r($_POST,TRUE)
		  ), WATCHDOG_ERROR);

		  die();

	   }


	}else{ // you can also save invalid payments for debug purposes

	   // uncomment code below if you want to log requests with bad hash
	   watchdog('Perfect Money', 'Bad hash, vars "%vars"; our hash: '.$hash, array("%vars" => print_r($_POST,TRUE)), WATCHDOG_ALERT);
	   die();

	}










}
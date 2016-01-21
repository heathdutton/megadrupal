<?php 

	require_once("index.php");

	$payment = new HostedPayments("D9C196A3-AE39-43EA-9B4E-FED869566276");

	$session_id = $payment->get("session_id");

	$request = json_decode( $_REQUEST['result'] );

	$transaction_id = $request->transactions->id;

	$verify = $payment->set("action", "verify")
			->set("sessionID", $session_id)
			->set("transactionID", $transaction_id)
			->send();

	echo "Result was: " . $verify->status;
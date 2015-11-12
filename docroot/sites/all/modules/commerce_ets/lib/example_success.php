<?php

	require_once("index.php");

	$payment = new HostedEcom("D9C196A3-AE39-43EA-9B4E-FED869566276");

	$payment->verifyPayment( $_REQUEST["TransactionID"] );

	echo $payment->isApproved() ? "Result was: success" : "Result was: failed";


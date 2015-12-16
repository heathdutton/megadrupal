<?php

	require_once("index.php");

	$payment = new HostedEcom("D9C196A3-AE39-43EA-9B4E-FED869566276");
	
	$payment->amount = 12;
	$payment->paymentType = CREDIT_CARD;
	$payment->approvalRedirectURL = "example_success.php";
	$payment->createSession();

?>

<!doctype html>
<html lang="en">
	<head>
		<meta charset="UTF-8">
		<title>Hosted Payments | PHP Example v1</title>
		<link rel="stylesheet" href="styles/normalize.css">
	</head>
	<body>

		<style>
			.wrapper { margin: 0 auto; width: 960px; }
			.etsFormGroup { margin: 10px 0; } 
		</style>

		<div class="wrapper">

			<h1>My Payment Page</h1>

			<!-- HTML5 Magic -->
			<div data-ets-key="<?php $payment->the_session_id(); ?>">
				<img src="http://www.loadinfo.net/main/download?spinner=3875&disposition=inline" alt="">
			</div>

			<script src="<?php echo ECOM_ROOT_URL ?>/init"></script>
			
		</div>

	</body>
</html>

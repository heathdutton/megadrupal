<?php

	require_once("index.php");

	$payment = new HostedPayments("39603DE6-3A38-4D1C-BAC5-C6A241996252"); 

	$session = $payment->set('action', 'session')
			   		   ->set('amount', 5)
			   		   ->set('successRedirectUrl', 'example_success.php')
					   ->send();
	
?>	

<!doctype html>
<html lang="en">
	<head>
		<meta charset="UTF-8">
		<title>Hosted Payments | PHP Example v2</title>
		<link rel="stylesheet" href="../styles/normalize.css">
	</head>
	<body>

		<style>
			.wrapper { margin: 0 auto; width: 960px; }
			.etsFormGroup { margin: 10px 0; } 
		</style>

		<div class="wrapper">

			<h1>My Payment Page <?php echo trim($session->id); ?></h1>

			<!-- HTML5 Magic -->
			<div data-ets-key="<?php echo trim($session->id); ?>">
				<img src="http://www.loadinfo.net/main/download?spinner=3875&disposition=inline" alt="">
			</div>

			<script src="<?php echo ECOM_ROOT_URL ?>/init"></script>

		</div>

	</body>
</html>



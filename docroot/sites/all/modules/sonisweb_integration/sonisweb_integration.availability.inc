<?php
function check_availability () {
	// Make a test sql call to Sonis to check it's availability
	if(sonisweb_integration_test_sql()) {
		echo "Success"; 
	}
	else {
		$email_addresses = variable_get('sonisweb_integration_availability_email_addresses', '');

		if($email_addresses != '') {
			$to      = $email_addresses;
			$subject = 'Sonis is down';
			$message = 'The Sonisweb Integration module has detected that Sonis is down.';
			mail($to, $subject, $message);
		}
	}
}
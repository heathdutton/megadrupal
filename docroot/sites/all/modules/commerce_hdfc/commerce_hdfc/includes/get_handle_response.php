<?php

/**
 * @file
 * Receives response from Payment Gateway and sends 
 * it to process.php for further processing.
 */

$current_wd = getcwd();
include 'header.php';
chdir($current_wd);

try
{
    $query = db_select('payment_hdfc_config', 'n');
    $query->condition('n.fixedcol', '1020', '=')
          ->fields('n', array('response_ip_one'));
    $sql = $query->execute()->fetchField();
    $db_ip_one = $sql;

    $query = db_select('payment_hdfc_config', 'n');
    $query->condition('n.fixedcol', '1020', '=')
          ->fields('n', array('response_ip_two'));
    $sql = $query->execute()->fetchField();
    $db_ip_two = $sql;

    $query = db_select('payment_hdfc_config', 'n');
    $query->condition('n.fixedcol', '1020', '=')
          ->fields('n', array('response_ip_three'));
    $sql = $query->execute()->fetchField();
    $db_ip_three = $sql;

/* Capture the IP Address from where the response has been received. */

$strresponseipadd = ip2long(getenv('REMOTE_ADDR'));
$longipdb1 = ip2long($db_ip_one);
$longipdb2 = ip2long($db_ip_two);
$longipdb3 = ip2long($db_ip_three);

/* Check whether the IP Address from where response is received is PG IP. */
if ($strresponseipadd == $longipdb1 || $strresponseipadd == $longipdb2 || $strresponseipadd == $longipdb3)
  {

    $query = db_select('payment_hdfc_config', 'n');
    $query->condition('n.fixedcol', '1020', '=')
          ->fields('n', array('site_base_url'));
    $sql = $query->execute()->fetchField();
    $db_site_base_url = $sql;

    $query = db_select('payment_hdfc_config', 'n');
    $query->condition('n.fixedcol', '1020', '=')
          ->fields('n', array('dual_verification_request_url'));
    $sql = $query->execute()->fetchField();
    $db_pg_dual_url = $sql;

    $reserrortext = isset($_POST['errortext']) ? $_POST['errortext'] : ''; 	// Error Text message.
    $respaymentid = isset($_POST['paymentid']) ? $_POST['paymentid'] : '';	// Payment Id.
    $restrackid = isset($_POST['trackid']) ? $_POST['trackid'] : '';        // Merchant Track ID.
    $reserrorno = isset($_POST['Error']) ? $_POST['Error'] : '';            // Error Number.

/* To collect transaction result. */

    $resresult = isset($_POST['result']) ? $_POST['result'] : '';           // Transaction Result.

/* To collect Payment Gateway Transaction ID, this value will be used in dual verification request. */

    $restranid = isset($_POST['tranid']) ? $_POST['tranid'] : '';           // Transaction ID.
    $resauth = isset($_POST['auth']) ? $_POST['auth'] : '';                 // auth Code.
    $resavr = isset($_POST['avr']) ? $_POST['avr'] : '';                    // Transaction avr.
    $resref = isset($_POST['ref']) ? $_POST['ref'] : '';                    // Reference Number also called Seq Number.

/* To collect amount from response. */

    $resamount = isset($_POST['amt']) ? $_POST['amt'] : '';                 // Transaction Amount.

 db_update('commerce_order')
           ->fields(array(
             'auth' => $resauth,
             'avr' => $resavr,
                         )
                   )
          ->condition('order_number', $restrackid)
          ->execute();

/* list of parameters received from payment gateway ends here. */

/* If error number is NOT present, then create Dual Verification
	request, send to Paymnent Gateway. */

    if ($reserrorno == '')
    {

/* Result is captured or approved i.e. successful. */
      if ($resresult == 'CAPTURED' || $resresult == 'APPROVED')
      {

/* Parameter validation code. */

/* The Below condition will check the Required Parameter From PG
 side Blank or not,if any field is blank in the below condition
 then it will redirected to Failed page. */
        if ($respaymentid == '' || $restrackid == '' || $restranid == '' || $resauth == '' || $resref == '' || $resamount == '')
        {

					$REDIRECT = 'REDIRECT=' . $db_site_base_url . "/" . drupal_get_path('module', 'commerce_hdfc') . '/includes/process.php?Message=PARMETER VALIDATION FAILED';
					echo $REDIRECT;
        }
		
        else
        {
/* Parameter validation code end. */

	$query = db_select('payment_hdfc_config', 'n');
	$query->condition('n.fixedcol', '1020', '=')
	      ->fields('n', array('transportal_id'));
	$sql = $query->execute()->fetchField();
	$db_id_val = $sql;

	$query = db_select('payment_hdfc_config', 'n');
	$query->condition('n.fixedcol', '1020', '=')
	      ->fields('n', array('transportal_pswd'));
	$sql = $query->execute()->fetchField();
	$db_pass_val = $sql;

/* If CAPTURED or APPROVED then below Code is execute for dual inquiry. */

/* Tranportal ID, same iD that was passed in initial request. */
					$reqtranportalid = "<id>" . $db_id_val . "</id>";

/* Tranportal Password, same password that was passed in initial request. */
					$reqtranportalpassword = "<password>" . $db_pass_val . "</password>";

/* Pass DUAL VERIFICATION action code, always pass "8" for DUAL VERIFICATION. */
					$inqaction = "<action>8</action>";

/* Pass PG Transaction ID for Dual Verification. */
					$inqtransid  = "<transid>" . $restranid . "</transid>";

/* String for request of input parameters. */
					$inqrequest = $reqtranportalid . $reqtranportalpassword . $inqaction . $inqtransid;

					$inqurl = $db_pg_dual_url;

/* Connection and posting the request starts here. */

					$dvreq = curl_init() or die(curl_error());
					curl_setopt($dvreq, CURLOPT_POST,1);
					curl_setopt($dvreq, CURLOPT_POSTFIELDS,$inqrequest);
					curl_setopt($dvreq, CURLOPT_URL,$inqurl);
					curl_setopt($dvreq, CURLOPT_PORT, 443);
					curl_setopt($dvreq, CURLOPT_RETURNTRANSFER, 1);
					curl_setopt($dvreq, CURLOPT_SSL_VERIFYHOST,0);
					curl_setopt($dvreq, CURLOPT_SSL_VERIFYPEER,0);
					$dataret = curl_exec($dvreq) or die(curl_error());
					curl_close($dvreq);

/* Connection and posting the request ends here. */

/* XML response received for dual verification. */

					$traninqresponse = $dataret;
					$genxmlform = "<xmltg>" . $traninqresponse . "</xmltg>";
					$xmlstr = simplexml_load_string( $genxmlform,null,true);

/* Collect dual verification result. */
					$inqcheck = $xmlstr-> result;

					db_update('commerce_order')
                    ->fields(array(
                        'result' => $inqcheck,
                                  )
                            )
                    ->condition('order_number', $restrackid)
                    ->execute();

/* If dual verification result is captured or approved or success. */

          if ($inqcheck == 'CAPTURED' || $inqcheck == 'APPROVED' || $inqcheck == 'SUCCESS')
          {

/* Collect dual verification result. */

						$inqresresult = $xmlstr->result; // It will give dualinquiry Result.
						$inqresamount = $xmlstr->amt; // It will give amount.
						$inqrestrackid = $xmlstr->trackid; // It will give trackid enrolled.
						$inqrespayid = $xmlstr->payid; // It will give payid.
						$inqresref = $xmlstr->ref; // It will give ref no.
						$inqrestranid = $xmlstr->tranid; // It will give tranid.

					db_update('commerce_order')
                    ->fields(array(
                        'tranid' => $inqrestranid,
						'refno' => $inqresref,
						'pymid' => $inqrespayid,
                                  )
                            )
                    ->condition('order_number', $inqrestrackid)
                    ->execute();

					$REDIRECT = 'REDIRECT=' . $db_site_base_url . "/" . drupal_get_path('module', 'commerce_hdfc') . '/includes/process.php?resresult=' . $inqresresult . '&restrackid=' . $inqrestrackid . '&resamount=' . $inqresamount . '&respaymentid=' . $inqrespayid . '&resref=' . $inqresref . '&restranid=' . $inqrestranid . '&resauth=' . $resauth . '&resavr=' . $resavr . '&errortext=' . $reserrortext . '&errorno=' . $reserrorno;

                    echo $REDIRECT;

          }

          else
          {
/* Error in transaction: redirects customer on failure page with respective message. */

						$REDIRECT = 'REDIRECT=' . $db_site_base_url . "/" . drupal_get_path('module', 'commerce_hdfc') . '/includes/process.php?Message=Transaction Failed&restrackid=' . $restrackid . '&resamount=' . $resamount . '&errortext=' . $reserrortext . '&errorno=' . $reserrorno;

						echo $REDIRECT;

          }

        }
      }

      else
      {

/* Error in transaction: redirects customer on failure page with respective message. */

$REDIRECT = 'REDIRECT=' . $db_site_base_url . "/" . drupal_get_path('module', 'commerce_hdfc') . '/includes/process.php?Message=Transaction Failed&restrackid=' . $restrackid . '&resamount=' . $resamount . '&resavr=' . $resavr . "&resauth=" . $resauth . '&errortext=' . $reserrortext . '&errorno=' . $reserrorno;

				echo $REDIRECT;

      }
    }

	else
    {
/* Error in transaction: redirects customer on failure page with respective message. */

 $REDIRECT = 'REDIRECT=' . $db_site_base_url . "/" . drupal_get_path('module', 'commerce_hdfc') . '/includes/process.php?Message=Transaction Failed&restrackid=' . $restrackid . '&resamount=' . $resamount . '&errortext=' . $reserrortext . '&errorno=' . $reserrorno;

      echo $REDIRECT;

    }

  }

    else
  {

/* If ip address mismatches, redirects customer on failure page with respective message. */

        $REDIRECT = 'REDIRECT=' . $db_site_base_url . "/" . drupal_get_path('module', 'commerce_hdfc') . '/includes/process.php?Message=--IP MISSMATCH-- Response IP Address is: ' . $strresponseipadd . '&errortext=' . $reserrortext . '&errorno=' . $reserrorno;
        echo $REDIRECT;
  }
}

catch(Exception $e)
{
 var_dump($e->getMessage());
}

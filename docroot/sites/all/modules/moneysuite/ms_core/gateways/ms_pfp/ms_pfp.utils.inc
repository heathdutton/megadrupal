<?php

/*
 * There are some error messages that are helpful to the user while others need to be
 * a bit more vauge so that the user gets enough information to make a good choice about
 * what to do next.
 *
 * @see _payflow_process_submit for usage
 */
function _ms_pfp_process_error_message($result_code) {
  switch ($result_code) {
    case 12:
    case 23:
    case 24:
    case 50:
      return _ms_pfp_code_to_string($result_code);
    case 112:
      return t('Transaction declined, your address is not on file with your credit card company. Your credit card was not charged.');
    case 114:
      return t('Transaction declined, security code does not match. Your credit card was not charged.');
    case 117:
      return t('Transaction declined, your address is not on file with your credit card company. Your credit card was not charged.');
    default:
      return t('Transaction declined. Your credit card was not charged');
  }
}


function _ms_pfp_code_to_string($result_code) {
  $result_code = (int) $result_code;

  if ($result_code < 0) {
    return _ms_pfp_error_code_to_string($result_code);
  }

  return _ms_pfp_transaction_code_to_string($result_code);
}

function _ms_pfp_error_code_to_string($error_code) {
  switch ($error_code) {
    case -1:
      return t('Failed to connect to host');
    case -2:
      return t('Failed to resolve hostname');
    case -5:
      return t('Failed to initialize SSL context');
    case -6:
      return t('Parameter list format error: & in name');
    case -7:
      return t('Parameter list format error: invalid [ ] name length clause');
    case -8:
      return t('SSL failed to connect to host');
    case -9:
      return t('SSL read failed');
    case -10:
      return t('SSL write failed');
    case -11:
      return t('Proxy authorization failed');
    case -12:
      return t('Timeout waiting for response');
    case -13:
      return t('Select failure');
    case -14:
      return t('Too many connections');
    case -15:
      return t('Failed to set socket options');
    case -20:
      return t('Proxy read failed');
    case -21:
      return t('Proxy write failed');
    case -22:
      return t('Failed to initialize SSL certificate');
    case -23:
      return t('Host address not specified');
    case -24:
      return t('Invalid transaction type');
    case -25:
      return t('Failed to create a socket');
    case -26:
      return t('Failed to initialize socket layer');
    case -27:
      return t('Parameter list format error: invalid [ ] name length clause');
    case -28:
      return t('Parameter list format error: name');
    case -29:
      return t('Failed to initialize SSL connection');
    case -30:
      return t('Invalid timeout value');
    case -31:
      return t('The certificate chain did not validate, no local certificate found');
    case -32:
      return t('The certificate chain did not validate, common name did not match URL');
    case -40:
      return t('Unexpected Request ID found in request.');
    case -41:
      return t('Required Request ID not found in request');
    case -42:
      return t('Required Response ID not found in request');
    case -43:
      return t('Unexpected Response ID found in request');
    case -44:
      return t('Response ID not found in the response received from the server');
    case -99:
      return t('Out of memory');
    case -100:
      return t('Parameter list cannot be empty');
    case -103:
      return t('Context initialization failed');
    case -104:
      return t('Unexpected transaction state');
    case -105:
      return t('Invalid name value pair request');
    case -106:
      return t('Invalid response format');
    case -107:
      return t('This XMLPay version is not supported');
    case -108:
      return t('The server certificate chain did not validate');
    case -109:
      return t('Unable to do logging');
    case -111:
      return t('The following error occurred while initializing from message file: ');
    case -113:
      return t('Unable to round and truncate the currency value simultaneously');
    default:
      return t('Unknown communications error');
  }
}

function _ms_pfp_transaction_code_to_string($result_code) {
  switch ($result_code) {
    case 0:
      return t('Approved');
    case 1:
      return t('User authentication failed.');
    case 2:
      return t('Invalid tender type. Your merchant bank account does not support the following credit card type that was submitted.');
    case 3:
      return t('Invalid transaction type. Transaction type is not appropriate for this transaction. For example, you cannot credit an authorization-only transaction.');
    case 4:
      return t('Invalid amount format. Use the format: “#####.##�? Do not include currency symbols or commas.');
    case 5:
      return t('Invalid merchant information. Processor does not recognize your merchant account information. Contact your bank account acquirer to resolve this problem.');
    case 6:
      return t('Invalid or unsupported currency code');
    case 7:
      return t('Field format error. Invalid information entered. See RESPMSG.');
    case 8:
      return t('Not a transaction server');
    case 9:
      return t('Too many parameters or invalid stream');
    case 10:
      return t('Too many line items');
    case 11:
      return t('Client time-out waiting for response');
    case 12:
      return t('Declined. Check the credit card number, expiration date, and transaction information to make sure they were entered correctly. If this does not resolve the problem, have the customer call their card issuing bank to resolve.');
    case 13:
      return t('Referral. Transaction cannot be approved electronically but can be approved with a verbal authorization. Contact your merchant bank to obtain an authorization and submit a manual Voice Authorization transaction.');
    case 14:
      return t('Invalid Client Certification ID. Check the HTTP header. If the tag, X-VPS-VIT-CLIENT-CERTIFICATION-ID, is missing, RESULT code 14 is returned.');
    case 19:
      return t('Original transaction ID not found. The transaction ID you entered for this transaction is not valid. See RESPMSG.');
    case 20:
      return t('Cannot find the customer reference number');
    case 22:
      return t('Invalid ABA number');
    case 23:
      return t('Invalid account number. Check credit card number and re-submit.');
    case 24:
      return t('Invalid expiration date. Check and re-submit.');
    case 25:
      return t('Invalid Host Mapping. You are trying to process a tender type such as Discover Card, but you are not set up with your merchant bank to accept this card type.');
    case 26:
      return t('Invalid vendor account');
    case 27:
      return t('Insufficient partner permissions');
    case 28:
      return t('Insufficient user permissions');
    case 29:
      return t('Invalid XML document. This could be caused by an unrecognized XML tag or a bad XML format that cannot be parsed by the system.');
    case 30:
      return t('Duplicate transaction');
    case 31:
      return t('Error in adding the recurring profile');
    case 32:
      return t('Error in modifying the recurring profile');
    case 33:
      return t('Error in canceling the recurring profile');
    case 34:
      return t('Error in forcing the recurring profile');
    case 35:
      return t('Error in reactivating the recurring profile');
    case 36:
      return t('OLTP Transaction failed');
    case 37:
      return t('Invalid recurring profile ID');
    case 50:
      return t('Insufficient funds available in account');
    case 99:
      return t('General error. See RESPMSG.');
    case 100:
      return t('Transaction type not supported by host');
    case 101:
      return t('Time-out value too small');
    case 102:
      return t('Processor not available');
    case 103:
      return t('Error reading response from host');
    case 104:
      return t('Timeout waiting for processor response. Try your transaction again.');
    case 105:
      return t('Credit error. Make sure you have not already credited this transaction, or that this transaction ID is for a creditable transaction. (For example, you cannot credit an authorization.)');
    case 106:
      return t('Host not available');
    case 107:
      return t('Duplicate suppression time-out');
    case 108:
      return t('Void error. See RESPMSG. Make sure the transaction ID entered has not already been voided. If not, then look at the Transaction Detail screen for this transaction to see if it has settled. (The Batch field is set to a number greater than zero if the transaction has been settled). If the transaction has already settled, your only recourse is a reversal (credit a payment or submit a payment for a credit).');
    case 109:
      return t('Time-out waiting for host response');
    case 111:
      return t('Capture error. Either an attempt to capture a transaction that is not an authorization transaction type, or an attempt to capture an authorization transaction that has already been captured.');
    case 112:
      return t('Failed AVS check. Address and ZIP code do not match. An authorization may still exist on the cardholder’s account.');
    case 113:
      return t('Merchant sale total will exceed the sales cap with current transaction. ACH transactions only.');
    case 114:
      return t('Card Security Code (CSC) Mismatch. An authorization may still exist on the cardholder’s account.');
    case 115:
      return t('System busy, try again later');
    case 116:
      return t('VPS Internal error. Failed to lock terminal number');
    case 117:
      return t('Failed merchant rule check. One or more of the following three failures occurred: 1) An attempt was made to submit a transaction that failed to meet the security settings specified on the Payflow Pro Java SDK Security Settings page. If the transaction exceeded the Maximum Amount security setting, then no values are returned for AVS or CSC. 2) AVS validation failed. The AVS return value should appear in the RESPMSG. 3) CSC validation failed. The CSC return value should appear in the RESPMSG.');
    case 118:
      return t('Invalid keywords found in string fields');
    case 122:
      return t('Merchant sale total will exceed the credit cap with current transaction. ACH transactions only.');
    case 125:
      return t('Fraud Protection Services Filter — Declined by filters');
    case 126:
      return t('Fraud Protection Services Filter — Flagged for review by filters');
    case 127:
      return t('Fraud Protection Services Filter — Not processed by filters');
    case 128:
      return t('Fraud Protection Services Filter — Declined by merchant after being flagged for review by filters');
    case 131:
      return t('Version 1 Payflow Pro SDK client no longer supported. Upgrade to the most recent version of the Payflow Pro client.');
    case 150:
      return t('Issuing bank timed out');
    case 151:
      return t('Issuing bank unavailable');
    case 1000:
      return t('Generic host error. This is a generic message returned by your credit card processor. The RESPMSG will contain more information describing the error.');
    case 1001:
      return t('Buyer Authentication Service unavailable');
    case 1002:
      return t('Buyer Authentication Service — Transaction timeout');
    case 1003:
      return t('Buyer Authentication Service — Invalid client version');
    case 1004:
      return t('Buyer Authentication Service — Invalid timeout value');
    case 1011:
      return t('Buyer Authentication Service unavailable');
    case 1012:
      return t('Buyer Authentication Service unavailable');
    case 1013:
      return t('Buyer Authentication Service unavailable');
    case 1014:
      return t('Buyer Authentication Service — Merchant is not enrolled for Buyer Authentication Service (3-D Secure).');
    case 1016:
      return t('Buyer Authentication Service — 3-D Secure error response received. Instead of receiving a PARes response to a Validate Authentication transaction, an error response was received.');
    case 1017:
      return t('Buyer Authentication Service — 3-D Secure error response is invalid. An error response is received and the response is not well formed for a Validate Authentication transaction.');
    case 1021:
      return t('Buyer Authentication Service — Invalid card type');
    case 1022:
      return t('Buyer Authentication Service — Invalid or missing currency code');
    case 1023:
      return t('Buyer Authentication Service — merchant status for 3D secure is invalid');
    case 1041:
      return t('Buyer Authentication Service — Validate Authentication failed: missing or invalid PARES');
    case 1042:
      return t('Buyer Authentication Service — Validate Authentication failed: PARES format is invalid');
    case 1043:
      return t('Buyer Authentication Service — Validate Authentication failed: Cannot find successful Verify Enrollment');
    case 1044:
      return t('Buyer Authentication Service — Validate Authentication failed: Signature validation failed for PARES');
    case 1045:
      return t('Buyer Authentication Service — Validate Authentication failed: Mismatched or invalid amount in PARES');
    case 1046:
      return t('Buyer Authentication Service — Validate Authentication failed: Mismatched or invalid acquirer in PARES');
    case 1047:
      return t('Buyer Authentication Service — Validate Authentication failed: Mismatched or invalid Merchant ID in PARES');
    case 1048:
      return t('Buyer Authentication Service — Validate Authentication failed: Mismatched or invalid card number in PARES');
    case 1049:
      return t('Buyer Authentication Service — Validate Authentication failed: Mismatched or invalid currency code in PARES');
    case 1050:
      return t('Buyer Authentication Service — Validate Authentication failed: Mismatched or invalid XID in PARES');
    case 1051:
      return t('Buyer Authentication Service — Validate Authentication failed: Mismatched or invalid order date in PARES');
    case 1052:
      return t('Buyer Authentication Service — Validate Authentication failed: This PARES was already validated for a previous Validate Authentication transaction');
    default:
      return t('Unknown transaction code');
  }
}

// This was in pre-D6 HEAD, but never used.  Left here because the text may be useful at some point.
/*function _ms_pfp_recurring_payment_code_to_string($code) {
 switch($code) {
 case 1:
 return t('Error');
 case 6:
 return t('Settlement pending');
 case 7:
 return t('Settlement in progress');
 case 8:
 return t('Settlement completed successfully');
 case 11:
 return t('Settlement failed');
 case 14:
 return t('Settlement incomplete');
 default:
 return t('Unknown transction code');
 }
 }*/

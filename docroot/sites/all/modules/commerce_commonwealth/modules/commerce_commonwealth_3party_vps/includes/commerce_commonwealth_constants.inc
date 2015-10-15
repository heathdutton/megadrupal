<?php

/**
 * @file
 * Commerce_commonwealth_constants.
 * Provides constants for most other Commerce Commonwealth module
 */

class CommerceCommonwealth3partyVpsConstants {
  /**
   * @var array - values that remain constant and do not need placing commerce
   * payment config page
   */
  public static $request = array(
    'vpc_Version' => 1,
    'vpc_Command' => 'pay',
  );
  /**
   * @var array - default values for the commerce payment config page
   */
  public static $default = array(
    'vpc_SecureHashSecret' => '',
    'vpc_URL' => 'https://migs.mastercard.com.au/vpcpay',
    'vpc_Merchant' => '',
    'vpc_AccessCode' => '',
    'vpc_CSCLevel' => 'M',
  );
  /**
   * @var array - response error codes
   */
  public static $status = array(
    '0' => 'Transaction Successful',
    '?' => 'Transaction status is unknown"',
    '1' => 'Unknown Error',
    '2' => 'Bank Declined Transaction',
    '3' => 'No Reply from Bank',
    '4' => 'Expired Card',
    '5' => 'Insufficient funds',
    '6' => 'Error Communicating with Bank',
    '7' => 'Payment Server System Error',
    '8' => 'Transaction Type Not Supported',
    '9' => 'Bank declined transaction (Do not contact Bank)',
    'A' => 'Transaction Aborted',
    'C' => 'Transaction Cancelled',
    'D' => 'Deferred transaction has been received and is awaiting processing',
    'E' => 'Issuer Returned a Referral Response',
    'F' => '3D Secure Authentication failed',
    'I' => 'Card Security Code verification failed',
    'L' => 'Shopping Transaction Locked (Please try the transaction again later)',
    'N' => 'Cardholder is not enrolled in Authentication scheme',
    'P' => 'Transaction has been received by the Payment Adaptor and is being processed',
    'R' => 'Transaction was not processed - Reached limit of retry attempts allowed',
    'S' => 'Duplicate SessionID (OrderInfo)',
    'T' => 'Address Verification Failed',
    'U' => 'Card Security Code Failed',
    'V' => 'Address Verification and Card Security Code Failed',
  );
  /**
   * @var array - required valid response keys from CBA
   */
  public static $requiredResponse = array(
    'vpc_TxnResponseCode',
    'vpc_TransactionNo',
    'vpc_Message',
  );
  /**
   * @var array - options for Minimum Acceptable CSC
   */
  public static $cscLevel = array(
    'M' => 'Exact code match',
    'S' => 'Merchant indicates CSC not present on card, (2-Party Only)',
    'P' => 'Code not processed',
    'U' => 'Card issuer is not registered and/or certified',
    'N' => 'Code invalid or not matched',
  );
  /**
   * @var array - credit card payment options
   */
  public static $paymentMethods = array(
    'visa' => 'Visa',
    'mastercard' => 'Mastercard',
  );
}

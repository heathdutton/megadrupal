<?php
/**
 * @file
 * Contains messages for the Auriga payment method.
 *
 * This has been taken straight from the example files.
 */

/**
 * Returns the error message specified by the error code.
 *
 * @param $code
 *  The error code for the message that we"re looking for.
 *
 * @return
 *  A string containging the message.
 */
function commerce_auriga_get_auriga_message($code) {
  switch ($code) {
    case 0:
      return "The payment/order placement has been carried out: Paid";
    case 1:
      return "";
    case 2:
      return "";
    case 3:
      return "An obligatory field is lacking or is incorrectly formatted.";
    case 4:
      return "Incorrect Merchant ID (Merchant_id).";
    case 5:
      return "";
    case 6:
      return "Incorrect amount (Amount or VAT, goodís value or VAT).";
    case 7:
      return "Unable to process or Authorize";
    case 8:
      return "";
    case 9:
      return "";
    case 10:
      return "";
    case 11:
      return "Was not possible to contact bank";
    case 12:
      return "Error in conjunction with validation of received MAC.";
    case 13:
      return "Amount too large";
    case 14:
      return "Wrong date/time format (Purchase_date, must be exactly 12 digits the format yyyymmddhhmm)";
    case 15:
      return "Incorrect purchase date (Purchase_date)";
    case 16:
      return "";
    case 17:
      return "Invalid payment method (Payment_method). Payment method not configured for the Shop. Or payment method not supported by Version=1";
    case 18:
      return "No contact with the bank.";
    case 19:
      return "Card Payment: Purchase denied at the bank (the card's validity period has expired), contact the bank.";
    case 20:
      return "Card Payment: Purchase rejected at the bank, contact the bank.";
    case 21:
      return "Country for card-issuing bank is not permitted.";
    case 22:
      return "The risk assessment value for the transaction exceeds the permissible value.";
    case 23:
      return "Card inactive";
    case 24:
      return "Error Request_type in Order Administration order.";
    case 25:
      return "Insufficient funds";
    case 26:
      return "Suspected fraud";
    case 27:
      return "Purchase amount (Amount) must be greater than 0.";
    case 28:
      return "Too many attempts";
    case 29:
      return "";
    case 30:
      return "Timeout. No answer from bank";
    case 31:
      return "Purchase terminated (by the buyer)";
    case 32:
      return "Error in order, transaction already registered and paid. This Customer_refno is already registered on another transaction.";
    case 33:
      return "eKöp: Technical error in communication with Postgirot/Nordea. Please try again later";
    case 34:
      return "Provided recipient account (To_accnt) incorrect";
    case 35:
      return "Provided sending account (From_accnt) incorrect.";
    case 36:
      return "eKöp: Provided sending account (From_accnt) temporarily frozen";
    case 37:
      return "";
    case 38:
      return "";
    case 39:
      return "eKöp: Provided certificate (Cert) incorrect.";
    case 40:
      return "eKöp: Account not connected to the service.";
    case 41:
      return "Payment Selection Page/Card Payment Page: Merchant not connected to the service.";
    case 42:
      return "Bad Auth_null - Should be YES or NO";
    case 43:
      return "Bad Capture_now - Should be YES or NO";
    case 44:
      return "";
    case 45:
      return "For Order Administration: The status of the transaction does not permit the operation";
    case 46:
      return "";
    case 47:
      return "";
    case 48:
      return "For Order Administration: The transaction does not exist.";
    case 49:
      return "";
    case 50:
      return "Card Payment: Merchant not configured for currency/card type combination (Currency/Card_type)";
    case 51:
      return "Card Payment: Invalid Exp_date (must be exactly 4 digits in the format mmyy).";
    case 52:
      return "Card Payment: Invalid Card_num (card number).";
    case 53:
      return "Card Payment: Incorrect format for the cardís control number Cvx2 (3 or 4 digits)";
    case 54:
      return "Order Administration: The status of the transaction does not permit the operation";
    case 55:
      return "Bad MOTO method. Should be MAIL or PHONE";
    case 56:
      return "The purchase has been discontinued by the purchaser or denied by the bank";
    case 57:
      return "Incorrect Customer_refno";
    case 58:
      return "Incorrect Version, incorrect format for version parameter.";
    case 59:
      return "Bad card fee";
    case 60:
      return "Bad Prepaid_card_num";
    case 61:
      return "Bad Prepaid_load_amount";
    case 62:
      return "Bad Prepaid_activation_code";
    case 63:
      return "";
    case 64:
      return "";
    case 65:
      return "Transaction already registered and awaiting a response from the bank";
    case 66:
      return "";
    case 67:
      return "Crediting could not be carried out.";
    case 68:
      return "";
    case 69:
      return "Technical error, Posten Betalväxel";
    case 70:
      return "For Order Administration: The function is not supported";
    case 71:
      return "Wrong format or size on the Response URL (Response_URL)";
    case 72:
      return "Incorrect Currency Code (Currency)";
    case 73:
      return "Incorrect Language Code (Language)";
    case 74:
      return "";
    case 75:
      return "Incorrect Comments (Comment)";
    case 76:
      return "Incorrect goods description (Goods_description)";
    case 77:
      return "Customer_refno does not match the Transaction_id";
    case 78:
      return "Card Payment: Authorisation Reversal could not be carried out (Request_type= AuthRev)";
    case 79:
      return "Card Payment: Acquiring could not be carried out (capture)";
    case 80:
      return "";
    case 81:
      return "Failed 3DSecure identification";
    case 82:
      return "Timed out 3DSecure identification";
    case 83:
      return "";
    case 84:
      return "";
    case 84:
      return "";
    case 86:
      return "";
    case 87:
      return "";
    case 88:
      return "";
    case 89:
      return "";
    case 90:
      return "Subscription (Transaction_id) has ended";
    case 91:
      return "subscription (Transaction_id) not found";
    case 92:
      return "Subscription (Transaction_id) not captured";
    case 93:
      return "Payment Selection Page: Wrong format or size for Cancel URL (Cancel_URL)";
    case 94:
      return "";
    case 95:
      return "Invoice purchase: Incorrect OCR number, e.g. incorrect control digit.";
    case 96:
      return "Incorrect length for invoice parameters.";
    case 97:
      return "Incorrect guarantee parameter.";
    case 98:
      return "Bad postal code";
    case 99:
      return "";
    case 100:
      return "";
    case 101:
      return "";
    case 102:
      return "";
    case 103:
      return "";
    case 104:
      return "";
    case 105:
      return "";
    case 106:
      return "";
    case 107:
      return "";
    case 108:
      return "";
    case 109:
      return "";
    case 110:
      return "Invoice purchase: Invalid personal number or organisation number.";
    case 111:
      return "UC denied";
    case 112:
      return "UC ok";
    case 113:
      return "";
    case 114:
      return "";
    case 115:
      return "Incorrect price per unit";
    case 116:
      return "Incorrect quantity per product";
    case 117:
      return "Incorrect VAT percentage";
    case 118:
      return "Incorrect discount percentage";
    case 119:
      return "";
    case 120:
      return "Prepaid transaction does not exist";
    case 121:
      return "Incorrect parameter value for VAT_Code";
    case 122:
      return "Incorrect parameter value for Fee_type_id";
    case 123:
      return "Incorrect parameter value for Invoice_carrier";
    case 124:
      return "Incorrect parameter value for Fee_amount";
    case 125:
      return "Incorrect parameter value for Discount";
    case 126:
      return "Incorrect parameter value for Round";
    default:
      return "An unkown error occured. Please contact the store or try again later.";
  }
}

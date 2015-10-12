<?php

/**
 * @param integer $id
 * @param array $args
 * @return string
 */
function sofortbanking_strings($id, $args = array()) {
  switch ($id) {
    case 1001:
      return t('SOFORT Banking');
    case 1002:
      return t('with buyer protection');
    case 1003:
      return ' (' . t('recommended payment method') . ')';
    case 1004:
      return t('<li>Payment system with TÜV-certified data protection</li><li>No registration required</li><li>Immediate shipping of stock goods</li><li>Please have your online banking details ready</li>');
    case 1005:
      return t('<li>Buyer protection for payment by SOFORT Banking! <a href="!url" target="_blank">More information</a>.</li>', $args);
    case 1006:
      return t('After order confirmation, you will be redirected to the SOFORT Banking payment system to carry out the transfer online. You will need your online banking data, i.e. an online bank account, bank account number, PIN and confirmation code.');
    case 1101:
      return t('Payment by SOFORT Banking has been carried out successfully (Transaction ID: !transactionid).', $args);
    case 1201:
      return t('Payment is unfortunately not possible or has been cancelled by the customer. Please select another payment method.');
    case 1202:
      return t('For reasons of security, the time has expired to carry out the payment. No transaction has been carried out. Please carry out the payment again.');
    case 1601:
      return t('Version: !version_number', $args);
    case 1602:
      return t('If you have not registered with SOFORT yet, you can register <a href="!url" target="_blank">here</a>.', $args);
    case 1603:
      return t('SOFORT Banking is the free of charge, TÜV certified payment method by SOFORT AG.');
    case 1604:
      return t('Activate SOFORT Banking');
    case 1605:
      return t('Makes the payment method available in the selection of payment methods.');
    case 1606:
      return t('Configuration key');
    case 1607:
      return t('Configuration key assigned by SOFORT - serves e.g. as a password.');
    case 1611:
      return t('Extended settings');
    case 1612:
      return t('The following settings do not required any adjustment and should already filled with correct values.');
    case 1613:
      return t('Banner or text in the selection of payment methods');
    case 1614:
      return t('Determines whether a logo and descriptive text or a banner is to be displayed in the selection of payment methods.');
    case 1615:
      return t('Banner');
    case 1616:
      return t('Logo & text');
    case 1617:
      return t('Display buyer protection text / banner');
    case 1618:
      return t('Determines whether the informative text or the buyer protection banner is to be displayed in the selection of payment methods. It can only be displayed if buyer protection has been activated in the project.');
    case 1619:
      return t('recommended payment method');
    case 1620:
      return t('Mark this payment method as "Recommended payment method" in the selection of payment methods.');
    case 1621:
      return t('Position in the selection of payment methods');
    case 1622:
      return t('Determines at which position this payment method will be displayed in the selection of payment methods. The smaller the number, the further up.');
    case 1623:
      return t('Reason 1');
    case 1624:
      return t('Determines the text to be displayed in the reason field of the transfer (max. 27 characters - special characters will be replaced/deleted). The following placeholders will be replaced by specific values:  {{order_id}}                         ==> Order number {{order_date}}                    ==> Order date {{customer_id}}                  ==> End customer number {{customer_name}}          ==> End customer name  {{customer_company}}   ==> Company name of end customer {{customer_email}}          ==> Email address of end customer {{transaction_id}}              ==> SOFORT Banking transaction ID');
    case 1625:
      return t('Reason 2');
    case 1626:
      return t('Currency selection');
    case 1627:
      return t('Determines the currencies this payment method can be paid with. Options: EUR, CHF, GBP, PLN.');
    case 1628:
      return t('Country selection');
    case 1629:
      return t('Determines the countries in which this payment method can be paid with. Options: DE, EN, …');
    case 1630:
      return t('Activate logging');
    case 1631:
      return t('Activates the logging of actions of the module. Should only be activated for troubleshooting. The log file can be found in Drupal\'s watchdog.');
    case 1632:
      return t('Assign status changes at SOFORT to the order status of your shop in the following:');
    case 1633:
      return t('Temporary');
    case 1634:
      return t('Define the status for orders which have not been paid yet.');
    case 1635:
      return t('Confirmed payment');
    case 1636:
      return t('Define the status for orders which have been successfully completed by SOFORT Banking.');
    case 1637:
      return t('Receipt of payment');
    case 1638:
      return t('Define the status for orders for which payment has been received on the account. Only available with SOFORT Bank account.');
    case 1639:
      return t('Aborted payment');
    case 1640:
      return t('Define the status for orders which have been aborted.');
    case 1641:
      return t('Check order');
    case 1642:
      return t('Define the status for orders which require a check. Only available with SOFORT Bank account.');
    case 1643:
      return t('Partial refund');
    case 1644:
      return t('Define the status for orders which will be refunded.');
    case 1645:
      return t('Refund');
    case 1646:
      return t('Define the status for orders which will be refunded.');
    case 1648:
      return t('Temporary');
    case 1649:
      return t('Confirmed');
    case 1650:
      return t('Aborted');
    case 1651:
      return t('Check order');
    case 1652:
      return t('Refund');
    case 1653:
      return t('*no status change*');
    case 1654:
      return t('Discard aborted payments');
    case 1655:
      return t('Orders with aborted payment will not be saved.');
    case 1801:
      return t('Redirection to SOFORT - Payment has not been made yet.', $args);
    case 1802:
      return t('SOFORT Banking has been transmitted successfully - Transaction ID: !transactionid.', $args);
    case 1803:
      return t('Payment receipt on account.', $args);
    case 1804:
      return t('Payment aborted.', $args);
    case 1805:
      return t('Payment confirmed. Payment has not been received yet. Transaction ID: !transactionid.', $args);
    case 1806:
      return t('Amount will be refunded.', $args);
    case 1807:
      return t('Partial amount will be refunded - !refunded_amount.', $args);

    // Following some Drupal specific strings.
    case 9001:
      return t('Error when sending a payment request to SOFORT Banking. Please contact the shop administrator.');
    case 9002:
      return t('Options');
    case 9003:
      return t('Payment Status');
    case 9004:
      return t('Confirmed payment (not credited yet). Only available with SOFORT Bank account.');
    case 9005:
      return t('Define the status for orders which have been successfully completed by SOFORT Banking.');
  }
  return '';
}

<?php

/**
 * Functionality that extends the basic Authorize.Net code with support for
 * the Authorized Recurring Billing API.
 */
class AuthorizenetArb extends Authorizenet {

  public function subscriptionXML($subscription) {
    $xml = $this->XMLElement($subscription, 'name');
    $xml .= $this->arbPaymentScheduleXML($subscription->paymentSchedule());
    $xml .= $this->XMLElement($subscription, 'amount');
    $xml .= $this->XMLElement($subscription, 'trialAmount');
    $xml .= $this->profileXML($subscription, 'customer');
    $xml .= $this->addressXML($subscription, 'billTo');
    $xml .= $this->addressXML($subscription, 'shipTo');
    return $xml;
  }

  /**
   * Authorize.Net ARB API: Create a new subscription request.
   */
  public function ARBCreateSubscriptionRequest($subscription) {
    $data = $this->subscriptionXML($subscription);
    return $this->request('ARBCreateSubscriptionRequest', $data);
  }

  /**
   * Authorize.Net ARB API: Create a new subscription request.
   */
  public function ARBUpdateSubscriptionRequest($subscription) {
    $data = $this->subscriptionXML($subscription);
    return $this->request('ARBUpdateSubscriptionRequest', $data);
  }

  /**
   * Authorize.Net ARB API: Check the status of an ARB subscription.
   */
  public function ARBGetSubscriptionStatusRequest($subscription) { }

  /**
   * Authorize.Net ARB API: Check the status of an ARB subscription.
   */
  public function ARBCancelSusbscriptionRequest($subscription) { }
}

class AuthorizenetARBSubscription {
  // Authorize.Net API properties.
  public $name;
  public $paymentSchedule;
  public $amount;
  public $trialAmount;
  public $creditCard;
  public $bankAccount;
  public $order;
  public $customer;
  public $billTo;
  public $shipTo;
}

class AuthorizenetArbPaymentSchedule {
  // Authorize.Net API properties.
  public $interval;
  public $startDate;
  public $totalOccurrences;
  public $trialOccurrences;
}

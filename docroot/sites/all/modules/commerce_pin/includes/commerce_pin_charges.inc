<?php

/**
 * @file
 * Handle the functionality for querying the Pin charges API.
 */


/**
 * A class for Pin charges.
 */
class CommercePinTransactionCharge extends CommercePinTransactionBase {

  // The endpoint we will be querying.
  const ENDPOINT = 'charges';

  /**
   * Query the endpoint with our transaction details.
   *
   * @param array $variables
   *   The variables required for a credit card request.
   *
   * @return CommercePinResponse
   *   A response object containing the response information.
   */
  public function chargeCreditCard($variables) {
    // Query the API with our card information.
    return $this->queryPinApi(self::ENDPOINT, $variables);
  }

}

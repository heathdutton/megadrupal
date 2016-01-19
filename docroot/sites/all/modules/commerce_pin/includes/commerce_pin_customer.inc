<?php

/**
 * @file
 * Handle the functionality for querying the Pin charges API.
 */


/**
 * A class for Pin charges.
 */
class CommercePinTransactionCustomer extends CommercePinTransactionBase {

  // The endpoint we will be querying.
  const ENDPOINT = 'customers';

  /**
   * Query the endpoint with our customer details.
   *
   * @param array $variables
   *   The variables required for a create customer request.
   *
   * @return CommercePinResponse
   *   A response object containing the response information.
   */
  public function createCustomer($variables) {
    // Query the API with our customer information.
    return $this->queryPinApi(self::ENDPOINT, $variables);
  }

  /**
   * Deletes a customer.
   *
   * @param string $customer_token
   *   Customer token.
   *
   * @return bool
   *   TRUE on success, else FALSE.
   */
  public function deleteCustomer($customer_token) {
    // The endpoint URL we will query.
    $endpoint_url = $this->getApiBase() . '/' . self::ENDPOINT . '/' . $customer_token;
    return $this->jsonDeleteRequest($endpoint_url);
  }

}

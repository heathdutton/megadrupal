<?php

/**
 * @file
 * This file contains a class for handling responses from the Pin gateway.
 */


/**
 * A response object to handle and parse Pin payments.
 */
class CommercePinResponse {

  // The raw response object.
  protected $response = FALSE;

  /**
   * Construct the object with the API response.
   *
   * @param Object $api_response
   *   The response object returned from the payment gateway.
   */
  public function __construct($api_response) {
    $this->response = $api_response;
  }

  /**
   * Check if the transaction was a success.
   *
   * @return Boolean
   *   If our transaction was successful.
   */
  public function wasSuccessful() {
    return
      isset($this->response->response->success)
      && $this->response->response->success == TRUE;
  }

  /**
   * Get the remote token that represents the transaction.
   *
   * @return String
   *   The unique token that represents our transaction.
   */
  public function getToken() {
    return $this->response->response->token;
  }

  /**
   * Get the remote card token that can be used for recurring charges.
   *
   * @return string
   *   Card token.
   */
  public function getCardToken() {
    return $this->response->response->card->token;
  }

  /**
   * Get all the errors returned by the gateway.
   *
   * @return String
   *   A correctly formatted, human readable error message.
   */
  public function getErrorMessage() {

    // There is no error message for successful transactions.
    if ($this->wasSuccessful()) {
      return '';
    }

    // Collect all the errors from the response.
    $error_message = array($this->response->error_description);
    if (isset($this->response->messages)) {
      foreach ($this->response->messages as $response_message) {
        if (isset($response_message->message)) {
          $error_message[] = $response_message->message;
        }
      }
    }

    return implode("\n", $error_message);
  }

  /**
   * Return a raw response array.
   *
   * @return Object
   *   The raw response object.
   */
  public function getRawResponse() {
    return $this->response;
  }
}

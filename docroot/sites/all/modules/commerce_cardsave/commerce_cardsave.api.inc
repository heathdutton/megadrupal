<?php

/**
 * @file
 * CardSave API implementation for Drupal.
 */

class CommerceCardsave {
  const DOMAIN = 'cardsaveonlinepayments.com';
  const PORT = '4430';
  protected $message;
  protected $crossReference;
  protected $success = FALSE;
  protected $threeDS = FALSE;

  /**
   * Basic constructor. Applies the settings to the object.
   *
   * @param array $settings
   *   Setting for the payment rules action.
   */
  public function __construct($settings) {
    $this->merchant_id = trim($settings['merchant_id']);
    $this->password = trim($settings['password']);
    $this->transaction_type = strtoupper($settings['transaction_type']);
  }

  /**
   * Performs a payment using card details (card number, CVV, etc).
   *
   * @param int $amount
   *   Positive transaction amount in minor units (cents, pences etc).
   * @param string $currency_code
   *   Currency code in ISO 4217.
   * @param array $credit_card
   *   Card details. See commerce_payment_credit_card_form() for help.
   * @param sarray $billing
   *   Customer billing information. Keys are equal to customer
   *   profile field names.
   * @param string $description
   *   Transaction description.
   * @param string $type
   *   Transaction type: COLLECT, REFUND, VOID etc. Gets the default value form
   *   payment mathod settings if not provided.
   */
  public function directPayment($order_id, $amount, $currency_code, $credit_card, $billing, $description, $type = '', $email = '') {
    $country = country_load($billing['country']);
    $currency = currency_load($currency_code);

    if (!valid_email_address($email)) {
      $user = $GLOBALS['user'];
      $email = $user->mail;
    }

    $data = array(
      '@amount' => $amount,
      '@currency_code' => $currency->ISO4217Number,
      '@order_id' => $order_id,
      '@type' => $type ? $type : $this->transaction_type,
      '@order_description' => $description,
      '@card_name' => $billing['name_line'],
      '@card_number' => $credit_card['number'],
      '@exp_month' => $credit_card['exp_month'],
      '@exp_year' => substr($credit_card['exp_year'], 2, 2),
      '@issue_number' => '',
      '@cv2' => $credit_card['code'],
      '@address1' => $billing['thoroughfare'],
      '@address2' => $billing['premise'],
      '@city' => $billing['locality'],
      '@state' => $billing['administrative_area'],
      '@postcode' => $billing['postal_code'],
      '@countrycode' => $country->numcode,
      '@email' => $email,
      '@phone' => '',
      '@ip' => ip_address(),
    );

    $this->request('CardDetailsTransaction', $data);
  }

  /**
   * Performs a payment using a cross reference.
   *
   * Used with card on file, refund and capture transactions.
   *
   * @param mixed $order_id
   *   Order ID of an existing order or any string to recognize the transaction.
   * @param int $amount
   *   Positive transaction amount in minor units (cents, pences etc.)
   * @param string $currency_code
   *   Currency code in ISO 4217.
   * @param string $description
   *   Transaction description.
   * @param string $cross_reference
   *   Cross reference of the relevant transaction.
   * @param string $type
   *   Transaction type: COLLECT, REFUND, VOID etc. Gets the default value form
   *   payment mathod settings if not provided.
   * @param string $new
   *   Indicates whether it is a new transaction or a continuation of previous,
   */
  public function crossPayment($order_id, $amount, $currency_code, $description, $cross_reference, $type = '', $new = 'TRUE') {
    $currency = currency_load($currency_code);
    $order = commerce_order_load($order_id);

    $data = array(
      '@amount' => $amount ? $amount : $order->commerce_order_total[LANGUAGE_NONE][0]['amount'],
      '@currency_code' => $currency->ISO4217Number,
      '@type' => $type ? $type : $this->transaction_type,
      '@new' => $new,
      '@cross_refernce' => $cross_reference,
      '@order_id' => $order_id,
      '@description' => $description,
      '@ip' => ip_address(),
    );

    $this->request('CrossReferenceTransaction', $data, 'cross');
  }

  /**
   * Returns the status of the current request.
   *
   * @return bool
   *   Status of the recent transaction.
   */
  public function getSuccess() {
    return $this->success;
  }

  /**
   * Returns a message if any is set.
   *
   * @return string
   *   A response message if any set. Used for clarifying errors.
   */
  public function getMessage() {
    return $this->message;
  }

  /**
   * Returns the 3DSecure authorization condition.
   *
   * @return bool
   *   TRUE if the merchant requires it and supported by the card.
   */
  public function requireThreeDS() {
    return $this->threeDS;
  }

  /**
   * Returns the cross reference (remote id) of the current transaction.
   *
   * @return mixed
   *   Cross reference or FALSE if any successful transaction wasn't completed.
   */
  public function getCrossReference() {
    return $this->crossReference;
  }

  /**
   * Called on 3DSecure callback to send the received POST-data to CardSave.
   *
   * @param string $pares
   *   'PaRes' from POST data.
   * @param string $md
   *   'MD' from POST data.
   */
  public function finalize3ds($pares, $md) {
    $data = array(
      '@pares' => $pares,
      '@md' => $md,
    );

    $this->request('ThreeDSecureAuthentication', $data, '3ds');
  }

  /**
   * Retrieves the cross reference from the XML response.
   *
   * @param string $xml
   *   XML markup returned by CardSave.
   *
   * @return mixed
   *   Returns the cross reference or FALSE if the XML has invalid format.
   */
  protected function getXMLCrossReference($xml) {
    return preg_match('#<TransactionOutputData CrossReference="(.+)">#iU', $xml, $element) ? $element[1] : FALSE;
  }

  /**
   * XML parser. Looks for the desired element and returns it's value.
   *
   * @param string $xml
   *   XML markup returned by CardSave.
   * @param string $element
   *   Tag name to search for.
   * @param string $pattern
   *   Regexp pattern piece.
   */
  protected function getXMLValue($xml, $element, $pattern = '.+') {
    return preg_match('#<' . $element . '>(' . $pattern . ')</' . $element . '>#iU', $xml, $element) ? $element[1] : FALSE;
  }

  /**
   * Common function used for performing HTTP requests to the payment gateway.
   *
   * @param string $action
   *   SOAP action
   * @param array $data
   *   Array of the key => value pairs to replace in the XML template.
   * @param string $template
   *   Filename of the necessary xml template from the "xml" folder.
   */
  protected function request($action, $data, $template = 'basic') {
    $transattempt = 1;
    $gateway_id = 1;

    $options = array(
      'method' => 'POST',
      'data' => $this->generateXML($template, $data),
      'headers' => array(
        'SOAPAction' => 'https://www.thepaymentgateway.net/' . $action,
        'Content-Type' => 'text/xml; charset = utf-8',
        'Connection' => 'close',
      ),
    );

    // It will attempt each of the gateway servers (gw1, gw2 & gw3) 3 times
    // each before totally failing.
    while (!$this->success && $gateway_id <= 3 && $transattempt <= 3) {
      // Builds the URL to post to (rather than it being hard coded - means we
      // can loop through all 3 gateway servers).
      $url = 'https://gw' . $gateway_id . '.' . self::DOMAIN . ':' . self::PORT . '/';

      $result = drupal_http_request($url, $options);

      $this->manageReturnedXML($result->data);

      // Increment the transaction attempt if <=2.
      if ($transattempt <= 2) {
        $transattempt++;
      }
      else {
        // Reset transaction attempt to 1 & incremend $gateway_id (to use
        // next numeric gateway number (eg. use gw2 rather than gw1 now)).
        $transattempt = 1;
        $gateway_id++;
      }
    }

    return $result->data;
  }

  /**
   * Common function to perform further actions based on transaction status.
   *
   * @param string $xml
   *   XML markup returned by CardSave.
   */
  protected function manageReturnedXML($xml) {
    $message = $this->getXMLValue($xml, 'Message');
    $this->crossReference = $this->getXMLCrossReference($xml);

    switch ($this->getXMLValue($xml, 'statusCode', '[0-9]+')) {
      case 0:
        // Transaction authorised.
        $this->success = TRUE;
        break;

      case 3:
        // 3D Secure authorization required.
        $_SESSION['commerce_cardsave_md'] = $this->crossReference;
        $_SESSION['commerce_cardsave_pareq'] = $this->getXMLValue($xml, 'PaREQ');
        $_SESSION['commerce_cardsave_acsurl'] = $this->getXMLValue($xml, 'ACSURL');

        $this->threeDS = TRUE;
        $this->success = TRUE;
        break;

      case 4:
        // Card Referred - treat as a decline.
        $this->message = 'Card referred';
        break;

      case 5:
        // Card declined.
        $messages = array('Authorization failed.');

        if ($this->getXMLValue($xml, 'AddressNumericCheckResult') === 'FAILED') {
          $messages[] = 'Billing address check failed. Please check your billing address.';
        }

        if ($this->getXMLValue($xml, 'PostCodeCheckResult') === 'FAILED') {
          $messages[] = 'Billing postcode check failed. Please check your billing postcode.';
        }

        if ($this->getXMLValue($xml, 'CV2CheckResult') === 'FAILED') {
          $messages[] = 'The CV2 number you entered is incorrect.';
        }

        if ($this->getXMLValue($xml, 'ThreeDSecureAuthenticationCheckResult') === 'FAILED') {
          $messages[] = 'Your bank declined the transaction due to Verified by Visa / MasterCard SecureCode.';
        }

        if ($message === 'Card declined' || $message === 'Card referred') {
          $messages[] = 'Your bank declined the payment.';
        }

        $this->message = implode('<br />', $messages);
        break;

      default:
        $this->message = 'An error has occurred: ' . $message;
        break;
    }
  }

  /**
   * Generates and returns the XML markup to send in the request.
   *
   * @param string $template
   *   Template file name.
   * @param array $data
   *   Key => value pairs to replace in the template.
   */
  protected function generateXML($template, $data) {
    $data['@merchant_id'] = $this->merchant_id;
    $data['@password'] = $this->password;

    return strtr(file_get_contents('xml/' . $template . '.xml', FILE_USE_INCLUDE_PATH), $data);
  }
}

<?php

/**
 * A class which helps us to interact with SwipeHQ payment gateway
 */
class CommerceSwipeHq {

  /**
   * User settings for SwipeHQ
   */
  protected $settings;

  /**
   * Order for this transaction 
   */
  protected $order;

  /**
   * Currency code in use for this order
   */
  protected $currencyCode;

  /**
   * Accepted currencies from SwipeHQ 
   */
  protected $acceptedCurrencies;

  /**
   * Identifier for the transaction, requested from SwipeHQ 
   */
  protected $transactionIdentifier;

  /**
   * Transaction ID, different with transaction identifier
   */
  protected $transactionID;

  /**
   * Verification of a transaction
   */
  protected $verification;

  const WATCHDOG_NAME = 'Swipe Checkout';
  const PATH_FETCH_CURRENCY_CODES = 'fetchCurrencyCodes.php';
  const PATH_CREATE_TRANSACTION_IDENTIFIER = 'createTransactionIdentifier.php';
  const PATH_VERIFY_TRANSACTION = 'verifyTransaction.php';

  public function __construct(array $settings, $order) {
    $this->settings = $settings;
    $this->order = $order;
    $this->currencyCode = $this->order->commerce_order_total[LANGUAGE_NONE][0]['currency_code'];
    $this->acceptedCurrencies = NULL;
    $this->transactionIdentifier = NULL;
  }

  public function getAcceptedCurrencies() {
    if (!$this->acceptedCurrencies) {
      $this->setAcceptedCurrencies();
    }

    return $this->acceptedCurrencies;
  }

  public function setAcceptedCurrencies() {
    $response = $this->doPostRequest($this->settings['api_credentials']['api_url'] . self::PATH_FETCH_CURRENCY_CODES, array(), TRUE);
    $this->acceptedCurrencies = $response['data'];
  }

  public function getTransactionIdentifier() {
    if (!$this->transactionIdentifier) {
      $this->setTransactionIdentifier();
    }

    return $this->transactionIdentifier;
  }

  public function setTransactionIdentifier($transactionIdentifier = NULL) {
    global $base_url;

    if (!$transactionIdentifier) {
      $this->acceptedCurrencies = $this->getAcceptedCurrencies();
      if ($this->currencyCode && in_array($this->currencyCode, $this->acceptedCurrencies)) {
        if (count($this->order->commerce_line_items)) {
          foreach ($this->order->commerce_line_items[LANGUAGE_NONE] as $order_line_item) {
            $line_item = commerce_line_item_load($order_line_item['line_item_id']);
            if ($line_item->type == 'product') {
              $description .= (int) $line_item->quantity . ' x ' . $line_item->line_item_label . ': ' .
                  commerce_currency_format($line_item->commerce_total[LANGUAGE_NONE][0]['amount'], $this->currencyCode) . '<br/>';
            } else {
              
            }

            if ($line_item->type == 'shipping') {
              $description .= 'Shipping: ' . commerce_currency_format($line_item->commerce_total[LANGUAGE_NONE][0]['amount'], $this->currencyCode) . '<br/>';
            }
          }
        }

        // Currently user data is always empty in response form payment page, needs to check more
        $response = $this->doPostRequest($this->settings['api_credentials']['api_url'] . self::PATH_CREATE_TRANSACTION_IDENTIFIER, array(
          'td_item' => 'Order ID: ' . $this->order->order_id,
          'td_description' => $description,
          'td_amount' => substr($this->order->commerce_order_total[LANGUAGE_NONE][0]['amount'], 0, -2) . '.' . substr($this->order->commerce_order_total[LANGUAGE_NONE][0]['amount'], -2),
          'td_currency' => $this->currencyCode,
          //        'td_user_data' => $this->order->order_id,
          'td_callback_url' => $base_url . '/checkout/' . $this->order->order_id . '/payment/return/' . $this->order->data['payment_redirect_key'],
        ));

        if ($response) {
          // Do something here
          if ($response->response_code == 200 && !empty($response->data->identifier)) {
            $this->transactionIdentifier = $response->data->identifier;
          }
        }
      } else {
        watchdog(self::WATCHDOG_NAME, 'Swipe does not support currency: ' . $this->order->currency .
            ', it supports: ' . implode(', ', $this->acceptedCurrencies) . '.', array(), WATCHDOG_ERROR);
      }
    } else {
      $this->transactionIdentifier = $transactionIdentifier;
    }
  }

  public function getTransactionID() {
    return $this->transactionID;
  }

  public function setTransactionID($transactionID) {
    $this->transactionID = $transactionID;
  }

  public function getVerification() {
    if (!$this->verification) {
      $this->setVerification();
    }

    return $this->verification;
  }

  /**
   * Currently verify using identifier value, which is not really good. Needs to find a way
   * to get transaction ID and verify by this one.
   */
  public function setVerification() {
    if ($this->transactionIdentifier) {
      $response = $this->doPostRequest($this->settings['api_credentials']['api_url'] . self::PATH_VERIFY_TRANSACTION, array(
        'identifier_id' => $this->transactionIdentifier,
      ));

      if (in_array($response->data->status, array('accepted', 'test-accepted')) && $response->data->transaction_approved == 'yes') {
        $this->verification = TRUE;
        $this->transactionID = $response->data->transaction_id;
      } else {
        $this->verification = FALSE;
      }
    } else {
      $this->verification = FALSE;
    }
  }

  /**
   * A helper method which sends request POST to Swipe API
   */
  protected function doPostRequest($url, $data, $assoc = FALSE) {
    //These params are needed for every request
    $data['merchant_id'] = $this->settings['api_credentials']['merchant_id'];
    $data['api_key'] = $this->settings['api_credentials']['api_key'];
    $ch = curl_init($url);
    curl_setopt($ch, CURLOPT_POST, 1);
    curl_setopt($ch, CURLOPT_POSTFIELDS, $data);
    curl_setopt($ch, CURLOPT_RETURNTRANSFER, 1);
    curl_setopt($ch, CURLOPT_FOLLOWLOCATION, 1);
    curl_setopt($ch, CURLOPT_SSL_VERIFYPEER, 0);
    curl_setopt($ch, CURLOPT_SSL_VERIFYHOST, 0);
    $response = curl_exec($ch);
    curl_close($ch);

    if (!$response) {
      watchdog(self::WATCHDOG_NAME, 'CURL request error: ' . curl_error($ch) . ' (' . curl_errno($ch) . ')', array(), WATCHDOG_ERROR);
    }

    return json_decode($response, $assoc);
  }

}

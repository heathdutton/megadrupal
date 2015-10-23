<?php

/**
 * @file
 * Wrapper class for the Etrans service.
 */

/**
 * Etrans connector using the drupal stored credentials.
 */
class CommerceEtransConnector {
  public function __construct() {
    $key = variable_get('commerce_etrans_key', FALSE);
    $secret = variable_get('commerce_etrans_secret', FALSE);
    if (!$key || !$secret) {
      throw new CommerceEtransException(t('Etrans credentials not set.'));
    }

    $this->etrans = new Etrans($key, $secret);

    $this->insurance = variable_get('commerce_etrans_insurance', FALSE);
    $this->pickup_time = variable_get('commerce_etrans_pickup_time', FALSE);

    if ($this->insurance === FALSE) {
      throw new CommerceEtransException(t('Etrans insurance not set.'));
    }
    if ($this->pickup_time === FALSE) {
      throw new CommerceEtransException(t('Etrans pickup time not set.'));
    }
  }

  /**
   * Rate the shipment for an order in Etrans.
   *
   * Wrapper method for Etrans::crear_parametros() that receives a commerce
   * order and extracts the values needed by Etrans.
   */
  public function crearParametros($order) {
    $values = $this->buildRequest($order);
    return $this->buildResponse($this->etrans->crear_parametros($values));
  }

  /**
   * Request the shipment for an order in Etrans.
   *
   * Wrapper method for Etrans::setEnvio() that receives a commerce order
   * and extracts the values needed by Etrans.
   */
  public function setEnvio($order) {
    $values = $this->buildRequest($order);
    return $this->buildResponse($this->etrans->setEnvio($values));
  }

  /**
   * Returns an array with the values to send to etrans.
   */
  protected function buildRequest($order) {
    $request = $this->processAddress($order);
    $request['email'] = $order->mail;
    $request['seguro'] = $this->insurance;
    $request['horario_retiro'] = $this->pickup_time;

    $shipping_details = commerce_etrans_get_order_shipping_details($order);
    if ($shipping_details) {
      $request['horario_entrega'] = $shipping_details['delivery_time'];
    }
    else {
      $request['horario_entrega'] = '1';
    }

    $this->addPackages($order, $request);

    return $request;
  }

  /**
   * Process the response and returns only the relevant data.
   *
   * Checks the status and if it's ok we return an array with Costo, Fecha
   * Retiro and Fecha Entrega. If it's not we throw a CommerceEtransException.
   */
  protected function buildResponse($response) {
    if ($response['status'] == 200 && empty($response['response']['message']) && !empty($response['response']['response'])) {
      $response = $response['response']['response'];
      return array(
        // Etrans will always use ARS.
        'amount' => commerce_currency_decimal_to_amount($response['Costo'], 'ARS'),
        'currency' => 'ARS',
        'pickup' => strtotime($response['Fecha Retiro']),
        'delivery' => strtotime($response['Fecha Entrega']),
      );
    }

    $messages = array();
    if (!empty($response['response']['message'])) {
      foreach ($response['response']['message'] as $message) {
        foreach ($message as $m) {
          $messages[] = check_plain($m);
        }
      }
    }
    $msg = t('Error code @code calling etrans: !msg',
      array(
        '!msg' => '<ul><li>' . implode('</li><li>', $messages) . '</li></ul>',
        '@code' => $response['status'],
      )
    );
    throw new CommerceEtransException($msg);
  }

  /**
   * Process the addresfield values.
   */
  protected function processAddress($order) {
    $address = commerce_etrans_get_order_shipping_address($order);
    $this->validateAddress($address);

    module_load_include('inc', 'addressfield', 'addressfield.administrative_areas');
    $administrative_areas = addressfield_get_administrative_areas($address['country']);
    if (isset($administrative_areas[$address['administrative_area']])) {
      $address['administrative_area'] = $administrative_areas[$address['administrative_area']];
    }
    $values = array(
      'calle' => $address['thoroughfare_name'],
      'numero_puerta' => $address['thoroughfare_number'],
      'piso' => !empty($address['premise_number']) ? $address['premise_number'] : '',
      'dpto_oficina' => !empty($address['sub_premise_number']) ? $address['sub_premise_number'] : '',
      'bque_torre' => !empty($address['premise_name']) ? $address['premise_name'] : '',
      'cp' => $address['postal_code'],
      'localidad' => $address['locality'],
      'partido' => !empty($address['sub_administrative_area']) ? $address['sub_administrative_area'] : '',
      'provincia' => $address['administrative_area'],
      'nombre_razon_social' => $address['name_line'],
    );

    return $values;
  }

  /**
   * Checks that the address in the order is valid for etrans.
   */
  public function validateAddress($address) {
    $required = array(
      'thoroughfare_name',
      'thoroughfare_number',
      'postal_code',
      'locality',
      'administrative_area',
    );

    foreach ($required as $field) {
      if (empty($address[$field])) {
        throw new CommerceEtransException(t("Required field missing from address, can't use etrans"));
      }
    }

    if (!$this->supportedDestination($address)) {
      throw new CommerceEtransUnsupportedDestinationException();
    }
  }

  /**
   * Checks if the destination is supported by etrans.
   */
  public function supportedDestination($address) {
    if ($address['country'] == 'AR') {
      // CABA.
      if ($address['administrative_area'] == 'C') {
        return TRUE;
      }
      // TODO: Confirmar los códigos postales soportados.
      // Extract the numeric part of the postal code.
      $postal_code = preg_replace('/[^0-9]/', '', $address['postal_code']);
      $postal_code = intval($postal_code);

      if ($address['administrative_area'] == 'B') {
        // GBA postal codes.
        if ($postal_code >= 1000 && $postal_code < 1900) {
          return TRUE;
        }
        // Besides CABA and GBA etrans supports sending packages to Campana
        // (2804), La Plata (1900) and Luján (6700).
        if ($postal_code == 2804 || $postal_code == 1900 || $postal_code == 6700) {
          return TRUE;
        }
      }
    }
    return FALSE;
  }

  /**
   * Process the order adding the packages to the request.
   *
   * @param object $order
   *   The order being shipped.
   * @param array $request
   *   The request array.
   */
  protected function addPackages($order, &$request) {
    $order_wrapper = entity_metadata_wrapper('commerce_order', $order);
    $n = 0;
    // Loop over each line item in the order to get the packages. We need to
    // send each product in the line item individually to etrans if
    // quantity > 1.
    foreach ($order_wrapper->commerce_line_items as $line_item_wrapper) {
      if (commerce_physical_line_item_shippable($line_item_wrapper->value())) {
        $dimensions = commerce_etrans_product_dimensions($line_item_wrapper->commerce_product->value());
        $weight = commerce_etrans_product_weight($line_item_wrapper->commerce_product->value());
        $value = commerce_etrans_product_price($line_item_wrapper->commerce_product->value());

        $package = array(
          'alto' => $dimensions['height'],
          'ancho' => $dimensions['width'],
          'profundidad' => $dimensions['length'],
          'peso' => $weight['weight'],
          'valor_declarado' => commerce_currency_amount_to_decimal($value['amount'], 'ARS'),
        );
        for ($i = 0; $i < intval($line_item_wrapper->quantity->value()); ++$i) {
          $request['bulto_' . ++$n] = $package;
        }
      }
    }
  }

}

/**
 * Exception that indicates the destination is not supported.
 */
class CommerceEtransUnsupportedDestinationException extends Exception {
}

/**
 * Custom exception for etrans.
 */
class CommerceEtransException extends Exception {
}

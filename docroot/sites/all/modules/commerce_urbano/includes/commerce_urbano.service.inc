<?php

/**
 * @file
 * Wrapper class for the Urbano web service.
 */

define('COMMERCE_URBANO_URL_RATE', 'https://www.urbano.com.ar/urbano3/wbs/consulta_tarifa/');
define('COMMERCE_URBANO_URL_PICKUP_SANDBOX', 'http://200.0.230.242/urbano3/wbs/carga_cliente/');
define('COMMERCE_URBANO_URL_PICKUP_LIVE', 'https://www.urbano.com.ar/webservice/carga_cliente/');

/**
 * @file
 * Wrapper class for the Urbano service.
 */

/**
 * Urbano connector using the drupal stored credentials.
 */
class CommerceUrbanoConnector {
  private $client;
  private $key;

  public function __construct() {
    $client = variable_get('commerce_urbano_client', FALSE);
    $key = variable_get('commerce_urbano_key', FALSE);
    if (!$client || !$key) {
      throw new CommerceUrbanoException(t('Urbano credentials not set.'));
    }

    $this->client = $client;
    $this->key = $key;
  }

  /**
   * Returns the URL to the specified Urbano's API server.
   *
   * @return string
   *   The URL to use to submit requests to the Urbano's API server.
   */
  private function getServer() {
    $server = variable_get('commerce_urbano_server', 'sandbox');

    switch ($server) {
      case 'sandbox':
        return COMMERCE_URBANO_URL_PICKUP_SANDBOX;

      case 'live':
        return COMMERCE_URBANO_URL_PICKUP_LIVE;
    }
  }

  /**
   * Rates the shipping of a Commerce Order.
   */
  public function rateShipping($order) {
    $order_wrapper = entity_metadata_wrapper('commerce_order', $order);

    // Determine the shipping profile reference field name for the order.
    $field_name = commerce_physical_order_shipping_field_name($order);
    if (!empty($order_wrapper->{$field_name}->commerce_customer_address)) {
      $postal_code = $order_wrapper->{$field_name}->commerce_customer_address->postal_code->value();
      $postal_code = preg_replace('/[^0-9]/', '', $postal_code);
    }

    // There is no delivery information.
    if (empty($postal_code)) {
      return FALSE;
    }

    $products = $this->extractProducts($order);

    // The are no shippable products.
    if (empty($products)) {
      return FALSE;
    }

    if (count($products) == 1) {
      $product = array_shift($products);

      $weight = $product['weight'];
      $length = $product['length'];
      $height = $product['height'];
      $width = $product['width'];

      $volumetric_weight = ($length * $height * $width) / 5000;
    }
    else {
      $weight = $volumetric_weight = $length = $height = $width = 0;

      foreach ($products as $product) {
        $weight += $product['weight'];
        $volumetric_weight += ($product['length'] * $product['height'] * $product['width']) / 5000;
      }
    }

    $shipper = str_pad($this->client, 6, '0', STR_PAD_LEFT);

    $url_options = array(
      'external' => TRUE,
      'query' => array(
        'shipper' => $shipper,
        'cp' => $postal_code,
        'peso_especifico' => $weight,
        'peso_volumetrico' => $volumetric_weight,
        'largo' => $length,
        'alto' => $height,
        'ancho' => $width,
      ),
    );

    $url = url(COMMERCE_URBANO_URL_RATE, $url_options);

    $response = drupal_http_request($url);

    if ($response->code != '200') {
      throw new CommerceUrbanoException(t("There was a problem connecting with Urbano's WS."));
    }

    if (
      (empty($response->data)) ||
      (!$response_xml = @simplexml_load_string($response->data))
    ) {
      throw new CommerceUrbanoException(t("Urbano's WS returned an invalid XML."));
    }

    if (!empty($response_xml->codError)) {
      throw new CommerceUrbanoException(t('There was an error rating the shipment: @error', array('@error' => $response_xml->descError)));
    }

    if (!floatval($response_xml->valorTarifa)) {
      return FALSE;
    }

    return array(
      // Urbano will always use ARS.
      'amount' => commerce_currency_decimal_to_amount(strval($response_xml->valorTarifa), 'ARS'),
      'currency' => 'ARS',
    );
  }

  /**
   * Ask for the pickup of the order.
   */
  public function setPickup($order) {
    $order_wrapper = entity_metadata_wrapper('commerce_order', $order);

    $shipper = str_pad($this->client, 6, '0', STR_PAD_LEFT);
    $key = $this->key;

    $url_options = array(
      'external' => TRUE,
      'query' => array(
        'cliente' => $shipper,
        'key' => $key,
      ),
    );

    $url = url($this->getServer(), $url_options);

    $data = $this->pickupDefaultData();

    $data['codigo_seguimiento'] = $order_wrapper->uid->value() . '_' . $order_wrapper->order_number->value();
    $data['dato_numerico'] = $order_wrapper->order_number->value();

    $shipping_address = array();

    $field_name = commerce_physical_order_shipping_field_name($order);
    if (!empty($order_wrapper->{$field_name}->commerce_customer_address)) {
      $shipping_address = $order_wrapper->{$field_name}->commerce_customer_address->value();
    }

    if (isset($shipping_address['name_line'])) {
      $data['nombre'] = $shipping_address['name_line'];
    }
    if (isset($shipping_address['thoroughfare_name'])) {
      $data['calle'] = $shipping_address['thoroughfare_name'];
    }
    if (isset($shipping_address['thoroughfare_number'])) {
      $data['altura'] = $shipping_address['thoroughfare_number'];
    }
    if (isset($shipping_address['premise_number'])) {
      $data['piso'] = $shipping_address['premise_number'];
    }
    if (isset($shipping_address['sub_premise_number'])) {
      $data['departamento'] = $shipping_address['sub_premise_number'];
    }
    if (isset($shipping_address['postal_code'])) {
      $data['codigo_postal'] = $shipping_address['postal_code'];
    }
    if (isset($shipping_address['locality'])) {
      $data['localidad'] = $shipping_address['locality'];
    }
    if (
      (isset($shipping_address['country'])) &&
      (isset($shipping_address['administrative_area']))
    ) {
      module_load_include('inc', 'addressfield', 'addressfield.administrative_areas');

      if (
        ($administrative_areas = addressfield_get_administrative_areas($shipping_address['country'])) &&
        (isset($administrative_areas[$shipping_address['administrative_area']]))
      ) {
        $data['provincia'] = $administrative_areas[$shipping_address['administrative_area']];
      }
    }
    if (isset($shipping_address['phone_number'])) {
      $data['telefono'] = $shipping_address['phone_number'];
    }

    if ($products = $this->extractProducts($order)) {
      $data['cantidad'] = count($products);
    }

    $request_data = 'datos=' . json_encode(array(1 => $data));

    $request_options = array(
      'method' => 'POST',
      'headers' => array(
        'Content-Type' => 'application/x-www-form-urlencoded',
      ),
      'data' => $request_data,
    );

    $response = drupal_http_request($url, $request_options);

    if ($response->code != '200') {
      throw new CommerceUrbanoException(t("There was a problem connecting with Urbano's WS."));
    }

    if (
      (empty($response->data)) ||
      (!$response_xml = @simplexml_load_string($response->data))
    ) {
      throw new CommerceUrbanoException(t("Urbano's WS returned an invalid XML."));
    }

    if (!empty($response_xml->codErr)) {
      throw new CommerceUrbanoException(t("There was an error setting the shipment's pickup: @error", array('@error' => $response_xml->descErr)));
    }

    if (
      (!isset($response_xml->info->registrosRecibidos)) ||
      (!isset($response_xml->info->registrosEmitir)) ||
      ((int) $response_xml->info->registrosRecibidos != (int) $response_xml->info->registrosEmitir)
    ) {
      throw new CommerceUrbanoException(t("There was an error setting the shipment's pickup"));
    }
  }

  /**
   * Get information about the shippable products of the order.
   */
  protected function extractProducts($order) {
    $products = array();

    $order_wrapper = entity_metadata_wrapper('commerce_order', $order);
    foreach ($order_wrapper->commerce_line_items as $line_item_wrapper) {
      if (
        (commerce_physical_line_item_shippable($line_item_wrapper->value())) &&
        ($product = $line_item_wrapper->commerce_product->value())
      ) {
        $order_product = array();

        $product_wrapper = entity_metadata_wrapper('commerce_product', $product);

        // Dimensions.
        $field_name = commerce_physical_entity_dimensions_field_name('commerce_product', $product);
        if (!empty($field_name) && !empty($product->{$field_name})) {
          // Extract the dimensions value from the product.
          $dimensions = $product_wrapper->{$field_name}->value();

          // Convert to cm.
          $dimensions = physical_dimensions_convert($dimensions, 'cm');

          $order_product['height'] = $dimensions['height'];
          $order_product['width'] = $dimensions['width'];
          $order_product['length'] = $dimensions['length'];
        }

        // Weight.
        $field_name = commerce_physical_entity_weight_field_name('commerce_product', $product);
        if (!empty($field_name) && !empty($product->{$field_name})) {
          // Extract the weight value from the product.
          $weight = $product_wrapper->{$field_name}->value();

          // Convert to kg.
          $weight = physical_weight_convert($weight, 'kg');

          $order_product['weight'] = $weight['weight'];
        }

        if (!empty($order_product)) {
          for ($i = 0; $i < intval($line_item_wrapper->quantity->value()); ++$i) {
            $products[] = $order_product;
          }
        }
      }
    }

    return $products;
  }

  /**
   * Returns the default data for pickup.
   */
  private function pickupDefaultData() {
    return array(
      'codigo_seguimiento' => '',
      'codigo_alternativo' => '',
      'dato_numerico' => '',
      'nombre' => '',
      'calle' => '',
      'altura' => '',
      'piso' => '',
      'departamento' => '',
      'codigo_postal' => '',
      'localidad' => '',
      'provincia' => '',
      'telefono' => '',
      'sku' => '0',
      'descripcion_producto' => '',
      'cantidad' => '',
      'servicio' => 'E',
      'valor_seguro' => 0,
      'valor_contrareembolso' => '',
      'autorizante' => '',
      'modelo_sms' => '',
      'modelo_carta_mail' => '',
      'direccion_mail' => '',
      'marca_de_agua' => '',
    );
  }

}

/**
 * Custom exception for Urbano.
 */
class CommerceUrbanoException extends Exception {
}

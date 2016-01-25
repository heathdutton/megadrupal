<?php
/**
 * @file
 * Glue code for interacting with the yellowcube-php library.
 */

// Load the yellowcube-php via the autoload script provided by composer or composer manager.
$localAutoloader = __DIR__ . '/vendor/autoload.php';
if (file_exists($localAutoloader)) {
    require $localAutoloader;
} else {
    composer_manager_register_autoloader();
}

use YellowCube\ART\Article;
use YellowCube\ART\ChangeFlag;
use YellowCube\ART\UnitsOfMeasure\ISO;
use YellowCube\WAB\AdditionalService\AdditionalShippingServices;
use YellowCube\WAB\AdditionalService\BasicShippingServices;
use YellowCube\WAB\Order;
use YellowCube\WAB\OrderHeader;
use YellowCube\WAB\Partner;
use YellowCube\WAB\Position;

/**
 * Generate the config object for the YellowCube\Service object.
 *
 * @return \YellowCube\Config
 */
function yellowcube_client_get_config() {
  $config = new YellowCube\Config(
    variable_get('yellowcube_sender', ''),
    variable_get('yellowcube_endpoint', ''),
    null,
    variable_get('yellowcube_mode', 'T')
  );

  $cert_path = variable_get('yellowcube_certificate', '');

  //Certificate handling
  if (!empty($cert_path)) {
    $config->setCertificateFilePath($cert_path, variable_get('yellowcube_password'));
  }

  return $config;
}

/**
 * Returns true if the connection to YellowCube succeeds for the given config.
 *
 * @param array $credentials
 * @return bool True if connection succeeds, false otherwise.
 */
function yellowcube_client_validate_connection(array $credentials) {
  $config = new YellowCube\Config(
    $credentials['yellowcube_sender'],
    $credentials['yellowcube_endpoint'],
    null,
    $credentials['yellowcube_mode']
  );

  if (!empty($credentials['yellowcube_certificate'])) {
    $config->setCertificateFilePath($credentials['yellowcube_certificate'], $credentials['yellowcube_password']);
  }

  try {
    $service = new YellowCube\Service($config);

    // A array response means that the connection worked and there is a result.
    return is_array($service->getInventory());
  } catch(Exception $e)  {
  }

  return false;
}

/**
 * Helper function to instantiate the yellowcube-php service object.
 *
 * @return YellowCube\Service
 */
function yellowcube_client_get_service() {
  $service = new YellowCube\Service(yellowcube_client_get_config());
  $minLogLevel = variable_get('yellowcube_log_level', '');

  if (!empty($minLogLevel)) {
    $logger = new \YellowCube\Util\Logger\MinLevelFilterLogger(
      $minLogLevel,
      new YellowCubeWatchdogLogger('yellowcube')
    );

    $service->setLogger($logger);
  }

  return $service;
}

/**
 * Send product data to YellowCube.
 *
 * @param string $sku
 *   SKU of the product to be added.
 *
 * @return bool
 *   TRUE on success, FALSE on failure.
 */
function yellowcube_client_update_product($sku) {
  $product = commerce_product_load_by_sku($sku);

  $article = new Article;
  $article
    ->setChangeFlag(ChangeFlag::INSERT)
    ->setPlantID(variable_get('yellowcube_plant', ''))
    ->setDepositorNo(variable_get('yellowcube_depositor', ''))
    ->setArticleNo($product->sku)
    ->setBaseUOM(ISO::PCE)
    ->setAlternateUnitISO(ISO::PCE)
    ->addArticleDescription($product->title, 'de');

  //Enable / Disable product based on the status
  switch ($product->status) {
    case FALSE:
      $article->setChangeFlag(ChangeFlag::DEACTIVATE);
      break;

    default:
      $article->setChangeFlag(ChangeFlag::INSERT);
  }

  //Setting the weight
  if (!empty($product->field_weight['und'][0]['weight'])) {
    $weight_unit = $product->field_weight['und'][0]['unit'];

    switch ($weight_unit) {
      case 'kg':
        $weight_unit_iso = ISO::KGM;
        break;
      case 'g':
        $weight_unit_iso = ISO::GRM;
        break;
      default:
        $weight_unit_iso = ISO::KGM;
    }

    $article->setNetWeight(round($product->field_weight['und'][0]['weight'], 3), $weight_unit_iso);

    // Gross weight with tara factor
    $tara_factor = variable_get('yellowcube_tara_factor', '1.1');
    $article->setGrossWeight(round($product->field_weight['und'][0]['weight'] * $tara_factor, 3), $weight_unit_iso);

  }

  //Setting the dimensions
  if (!empty($product->field_dimensions['und'][0]['unit'])) {
    $dimension_unit = $product->field_dimensions['und'][0]['unit'];

    switch ($dimension_unit) {
      case 'm':
        $dimension_unit = ISO::MTR;
        $volume_unit = ISO::MTQ;
        break;
      case 'cm':
        $dimension_unit = ISO::CMT;
        $volume_unit = ISO::CMQ;
        break;
      default:
        $dimension_unit = ISO::MTR;
        $volume_unit = ISO::MTQ;
    }

    $length = round($product->field_dimensions['und'][0]['length'], 3);
    $width = round($product->field_dimensions['und'][0]['width'], 3);
    $height = round($product->field_dimensions['und'][0]['height'], 3);
    $article->setLength($length, $dimension_unit);
    $article->setWidth($width, $dimension_unit);
    $article->setHeight($height, $dimension_unit);
    $article->setVolume(round($width * $height * $length, 3), $volume_unit);
  }


  $response = yellowcube_client_get_service()->insertArticleMasterData($article);

  $product_link = l(
    t('view'),
    'admin/commerce/products/' . $product->product_id,
    array(
      'query' => array('destination' => 'admin/reports/dblog'),
    )
  );

  if ($response->isSuccess()) {
    watchdog(
      'yellowcube',
      'Product @sku was successfully transmitted to YellowCube. Received reference number @ref and status message “@status”.',
      array(
        '@sku' => $response->getReference1(),
        '@ref' => $response->getReference(),
        '@status' => $response->getStatusText(),
      ),
      WATCHDOG_NOTICE,
      $product_link
    );

    $current_batch = batch_get();
    if (empty($current_batch)) {
      drupal_set_message(
        t(
          'Product @sku was successfully transmitted to YellowCube. Received reference number @ref and status message “@status”.',
          array(
            '@sku' => $response->getReference1(),
            '@ref' => $response->getReference(),
            '@status' => $response->getStatusText(),
          )
        )
      );
    }

    return TRUE;
  }
  else {
    watchdog(
      'yellowcube',
      'Product @sku could not be transmitted to YellowCube: “@status”.',
      array(
        '@sku' => $response->getReference1(),
        '@status' => $response->getStatusText(),
      ),
      WATCHDOG_ERROR,
      $product_link
    );

    return FALSE;
  }
}

/**
 * Returns a partner reference in format "FL-PLZ (customer no)", e.g. PM-8004 (33).
 *
 * FL = first name, last name
 * PLZ = postal code
 *
 * @param array $ship_addr
 *
 * @return string Partner reference.
 */
function yellowcube_client_get_partner_reference(array $ship_addr, stdClass $order) {
    $partner_reference = yellowcube_client_name_initials($ship_addr);

    if (!empty($ship_addr['postal_code'])) {
        $partner_reference .= '-' . $ship_addr['postal_code'];
    }

    if (!empty($order->commerce_customer_billing['und'][0]['profile_id'])) {
        $partner_reference .= ' (' . $order->commerce_customer_billing['und'][0]['profile_id'] . ')';
    }

    return $partner_reference;
}

/**
 * Returns the initials of the name in the shipping address.
 *
 * @param array $ship_addr
 *
 * @return string Uppercase initials, e.g. PM.
 */
function yellowcube_client_name_initials(array $ship_addr) {
    $initials = array();

    $names = explode(' ', $ship_addr['name_line'], 2);

    foreach ($names as $name) {
        if (!empty($name['0'])) {
            $initials[] = $name[0];
        }
    }

    return strtoupper(join('', $initials));
}

/**
 * Send order to YellowCube.
 *
 * @param array $ship_addr
 *   Shipping address, in array format available on the customer profile.
 * @param array $yc_product
 *   YellowCube product array.
 * @param stdClass $order
 *   Drupal Commerce order object.
 *
 * @return bool
 *   TRUE on success, FALSE on failure.
 */
function yellowcube_client_add_order(array $ship_addr, array $yc_product, stdClass $order) {
  $partner = new Partner();
  $partner
    ->setPartnerType('WE')
    ->setPartnerNo(variable_get('yellowcube_partner', ''))
    ->setPartnerReference(yellowcube_client_get_partner_reference($ship_addr, $order))
    ->setName1($ship_addr['name_line'])
    ->setName2($ship_addr['premise'])
    ->setStreet($ship_addr['thoroughfare'])
    ->setCountryCode($ship_addr['country'])
    ->setZIPCode($ship_addr['postal_code'])
    ->setCity($ship_addr['locality'])
    ->setEmail($order->mail)
    // A user is created for each (anonymous) order. Get the language of that user.
    // (The language property contains a ISO language code).
    // If there is no language configured, this returns en (english).
    ->setLanguageCode(user_preferred_language($order->uid)->language);


  $yc_order = new Order();
  $yc_order
    ->setOrderHeader(new OrderHeader(variable_get('yellowcube_depositor', ''), $order->order_id, date('Ymd', REQUEST_TIME)))
    ->setPartnerAddress($partner)
    ->setOrderDocumentsFlag(FALSE);

  $yc_order->addValueAddedService(new BasicShippingServices($yc_product['basic']));
  if (!empty($yc_product['additional'])) {
    $yc_order->addValueAddedService(new AdditionalShippingServices($yc_product['additional']));
  }

  foreach ($order->commerce_line_items['und'] as $key => $row) {
    $line_item = commerce_line_item_load($row['line_item_id']);

    //Only submit products and skip shipping cost line item
    if (!empty($line_item->commerce_product['und'][0]['product_id'])){
      $product = commerce_product_load($line_item->commerce_product['und'][0]['product_id']);
      $position = new Position();
      $position
        ->setPosNo($key + 1)
        ->setArticleNo($product->sku)
        ->setPlant(variable_get('yellowcube_plant', ''))
        ->setQuantity($line_item->quantity)
        ->setQuantityISO('PCE')
        ->setShortDescription($product->title_original);

      $yc_order->addOrderPosition($position);
    }
  }

  $response = yellowcube_client_get_service()->createYCCustomerOrder($yc_order);

  $order_link = l(
    t('view'),
    'admin/commerce/orders/' . $order->order_id,
    array(
      'query' => array('destination' => 'admin/reports/dblog'),
    )
  );

  if ($response->isSuccess()) {
    watchdog(
      'yellowcube',
      'Order #@num was successfully transmitted to YellowCube. Received reference number @ref and status message “@status”.',
      array(
        '@num' => $order->order_number,
        '@ref' => $response->getReference(),
        '@status' => $response->getStatusText(),
      ),
      WATCHDOG_NOTICE,
      $order_link
    );

    db_merge('yellowcube_order')
      ->key(array('order_id' => $order->order_id))
      ->fields(array(
        'yc_reference' => $response->getReference(),
      )
    )->execute();

    return TRUE;
  }
  else {
    watchdog(
      'yellowcube',
      'Order #@num could not be transmitted to YellowCube: “@status”.',
      array(
        '@num' => $order->order_number,
        '@status' => $response->getStatusText(),
      ),
      WATCHDOG_ERROR,
      $order_link
    );

    return FALSE;
  }
}

<?php

/**
 * @file
 * Builds placeholder replacement tokens for Commerce Pickup.
 */

/**
 * Implements hook_token_info().
 */
function commerce_pickup_token_info() {
  $order = array();

  // Plain-text pickup location of an order.
  $order['pickup-location'] = array(
    'name' => t('Pickup location'),
    'description' => t('The location chosen for order pickup.'),
  );

  // Relative path of the pickup location entity.
  $order['pickup-location-path'] = array(
    'name' => t('Pickup location path'),
    'description' => t('The relative path to the pickup location entity selected for an order.'),
  );

  // Absolute URL of the pickup location entity.
  $order['pickup-location-url'] = array(
    'name' => t('Pickup location URL'),
    'description' => t('The absolute URL of the pickup location entity selected for an order.'),
  );

  // Return tokens with the order object.
  return array(
    'tokens' => array('commerce-order' => $order),
  );
}

/**
 * Implements hook_tokens().
 */
function commerce_pickup_tokens($type, $tokens, array $data = array(), array $options = array()) {
  $replacements = array();

  // Provide tokens for the commerce-order type.
  if ($type == 'commerce-order' && !empty($data['commerce-order'])) {
    $order = $data['commerce-order'];

    // If 'pickup-location', 'pickup-location-path', or 'pickup-location-url' tokens are requested, do some prep work.
    if (array_key_exists('pickup-location', $tokens) || array_key_exists('pickup-location-path', $tokens) || array_key_exists('pickup-location-url', $tokens)) {

      // Load the pickup line item.
      $line_item = _commerce_pickup_order_line_item($order);

      // If there's no line item, bail.
      if (empty($line_item)) {
        return;
      }

      // Parse the location value.
      $location = _commerce_pickup_parse_location($line_item->data['shipping_service']['name']);

      // If there's no location data, bail.
      if (empty($location)) {
        return;
      }

      // Load the entity.
      $entities = entity_load($location['entity_type'], array($location['id']));
      $entity = reset($entities);

      // If no entity was found, bail.
      if (empty($entity)) {
        return;
      }
    }

    // Generate each token.
    foreach ($tokens as $name => $original) {
      switch ($name) {

        // Pickup location (plain text).
        case 'pickup-location':

          // Load the entity label.
          $label = entity_label($location['entity_type'], $entity);

          // Replace the original with a clean label.
          $replacements[$original] = check_plain($label);
          break;

        // Pickup location entity path (relative).
        case 'pickup-location-path':

          // Load the entity URI information.
          $uri = entity_uri($location['entity_type'], $entity);

          // Return an absolute URL to the entity.
          $replacements[$original] = url($uri['path']);
          break;

        // Pickup location entity URL (absolute).
        case 'pickup-location-url':

          // Load the entity URI information.
          $uri = entity_uri($location['entity_type'], $entity);

          // Return an absolute URL to the entity.
          $replacements[$original] = url($uri['path'], array('absolute' => TRUE));
          break;
      }
    }
  }

  return $replacements;
}

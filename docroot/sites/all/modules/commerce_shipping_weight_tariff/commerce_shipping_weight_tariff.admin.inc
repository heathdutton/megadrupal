<?php
/**
 * @file commerce_shipping_weight_tariff.commerce.inc
 * Contains admin page callbacks.
 */

/**
 * Page callback for the matrix overview.
 */
function commerce_shipping_weight_tariff_admin_matrix() {
  // Get all products of our bundle.
  $products = _commerce_shipping_weight_tariff_fetch_tariff_products();
  if(empty($products)) {
    return t('No tariff products found.');
  }
  // Get the services for our method.
  $services = commerce_shipping_services('weight_tariff_shipping');
  //dsm($services);

  // Build up an array of services => tariffs, where tariffs is an array of
  // weight => product id.
  $tariffs = array_fill_keys(array_keys($services), array());
  // Build up an array of of all the different weights we see.
  $weights = array();
  foreach ($products as $product) {
    $wrapper = entity_metadata_wrapper('commerce_product', $product);
    try {
        $weight = $wrapper->commerce_product_tariff_weight->weight->value();
        $unit = $wrapper->commerce_product_tariff_weight->unit->value();

        if($unit !== 'kg') {
            $weight = physical_weight_convert(array('weight' => $weight, 'unit' => $unit), 'kg');
            $weight = $weight['weight'];
        }
        $service = $wrapper->commerce_product_tariff_service->value();

        $tariffs[$service]["{$weight}"] = $product->product_id;
        // Build the array of weights we've seen.
        $weights["{$weight}"] = $weight;
    }
    catch(EntityMetadataWrapperException $e) {
        //if no product tariff weight just ignore. This is a result of an uninstall disconnecting products
        //but not being able to delete them as they can be attached to orders.
    }
  }

  // Sort the weights. These will form the columns of the table.
  asort($weights);

  // Build a table with rows of services and columns of weights.
  $rows = array();
  foreach ($tariffs as $service_name => $service_tariffs) {
    $row = array();

    // First column is the shipping method label.
    $row[] = $services[$service_name]['title'];

    // Column for each weight, which may be empty if the current method has
    // no price point for this weight.
    foreach ($weights as $weight) {
      if (isset($service_tariffs["{$weight}"])) {
        $product_id = $service_tariffs["{$weight}"];
        $product = $products[$product_id];
        $wrapper = entity_metadata_wrapper('commerce_product', $product);
        $price = $wrapper->commerce_price->value();
        $price_format = commerce_currency_format($price['amount'], $price['currency_code']);
        $row[] = l($price_format, 'admin/commerce/products/' . $product_id, array('query' => drupal_get_destination()));
      }
      else {
        $row[] = '';
      }
    }

    $rows[] = $row;
  }

  // The weights form the header, with a first column to account for the service
  // names.
  $header = array();
  $header[] = t('Shipping service');
  foreach ($weights as $weight) {
    $header[] = '< ' . number_format($weight, 3) . 'kg';
  }

  $table = theme('table', array(
    'rows' => $rows,
    'header' => $header,
  ));

  return $table;
}


/**
 * Builds the form for adding and editing services.
 */
function commerce_shipping_weight_tariff_service_form($form, &$form_state, $shipping_service) {
  // Store the initial shipping service in the form state.
  $form_state['shipping_service'] = $shipping_service;

  $form['cswt_shipping_service'] = array(
    '#tree' => TRUE,
  );

  $form['cswt_shipping_service']['title'] = array(
    '#type' => 'textfield',
    '#title' => t('Title'),
    '#default_value' => $shipping_service['title'],
    '#description' => t('The administrative title of this service. It is recommended that this title begin with a capital letter and contain only letters, numbers, and spaces.'),
    '#required' => TRUE,
    '#size' => 32,
    '#field_suffix' => ' <small id="edit-cswt_shipping_service-title-suffix">' . t('Machine name: @name', array('@name' => $shipping_service['name'])) . '</small>',
    '#maxlength' => 255
  );

  if (empty($shipping_service['name'])) {
    $form['cswt_shipping_service']['name'] = array(
      '#type' => 'machine_name',
      '#title' => t('Machine name'),
      '#default_value' => $shipping_service['name'],
      '#maxlength' => 32,
      '#required' => TRUE,
      '#machine_name' => array(
        'exists' => 'commerce_shipping_service_load',
        'source' => array('cswt_shipping_service', 'title'),
      ),
      '#description' => t('The machine-name of this service. This name must contain only lowercase letters, numbers, and underscores. It must be unique.'),
    );
  }
  else {
    $form['cswt_shipping_service']['name'] = array(
      '#type' => 'value',
      '#value' => $shipping_service['name'],
    );
  }

  $form['cswt_shipping_service']['display_title'] = array(
    '#type' => 'textfield',
    '#title' => t('Display title'),
    '#default_value' => $shipping_service['display_title'],
    '#description' => t('The front end display title of this service shown to customers. Leave blank to default to the <em>Title</em> from above.'),
    '#size' => 32,
  );

  $form['cswt_shipping_service']['description'] = array(
    '#type' => 'textarea',
    '#title' => t('Description'),
    '#description' => t('Describe this service if necessary. The text will be displayed in the services overview table.'),
    '#default_value' => $shipping_service['description'],
    '#rows' => 3,
  );

  $form['actions'] = array(
    '#type' => 'container',
    '#attributes' => array('class' => array('form-actions')),
    '#weight' => 40,
  );

  $form['actions']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save service'),
  );

  if (!empty($form_state['shipping_service']['name'])) {
    $form['actions']['delete'] = array(
      '#type' => 'submit',
      '#value' => t('Delete service'),
      '#suffix' => l(t('Cancel'), 'admin/commerce/config/shipping/services/weight-tariff-shipping'),
      '#submit' => array('commerce_shipping_weight_tariff_service_form_delete_submit'),
      '#weight' => 45,
    );
  }
  else {
    $form['actions']['submit']['#suffix'] = l(t('Cancel'), 'admin/commerce/config/shipping/services/weight-tariff-shipping');
  }

  return $form;
}

/**
 * Submit handler: saves the new or updated flat rate service.
 */
function commerce_shipping_weight_tariff_service_form_submit($form, &$form_state) {
  $shipping_service = $form_state['shipping_service'];

  // Update the shipping service array with values from the form.
  foreach (array('name', 'title', 'display_title', 'description') as $key) {
    $shipping_service[$key] = $form_state['values']['cswt_shipping_service'][$key];
  }

  // Save the shipping service.
  $op = commerce_shipping_weight_tariff_service_save($shipping_service);

  if (!$op) {
    drupal_set_message(t('The service failed to save properly. Please review the form and try again.'), 'error');
    $form_state['rebuild'] = TRUE;
  }
  else {
    drupal_set_message(t('Service saved.'));
    $form_state['redirect'] = 'admin/commerce/config/shipping/services/weight-tariff-shipping';
  }
}

/**
 * Displays the delete confirmation form for an existing service.
 *
 * @param $name
 *   The machine-name of the service to delete.
 */
function commerce_shipping_weight_tariff_service_delete_page($name) {
  return drupal_get_form('commerce_shipping_weight_tariff_service_delete_form', commerce_shipping_service_load($name));
}


/**
 * Displays the edit form for an existing service.
 *
 * @param $name
 *   The machine-name of the service to edit.
 */
function commerce_shipping_weight_tariff_service_edit_page($name) {
  return drupal_get_form('commerce_shipping_weight_tariff_service_form', commerce_shipping_service_load($name));
}

/**
 * Submit handler: redirects to the service delete confirmation form.
 *
 * @see commerce_shipping_weight_tariff_service_form()
 */
function commerce_shipping_weight_tariff_service_form_delete_submit($form, &$form_state) {
  $form_state['redirect'] = 'admin/commerce/config/shipping/services/weight-tariff-shipping-' . strtr($form_state['shipping_service']['name'], '_', '-') . '/delete';
}

/**
 * Builds the form for deleting services.
 */
function commerce_shipping_weight_tariff_service_delete_form($form, &$form_state, $shipping_service) {
  $form_state['shipping_service'] = $shipping_service;

  $form = confirm_form($form,
    t('Are you sure you want to delete the <em>%title</em> service?', array('%title' => $shipping_service['title'])),
    'admin/commerce/config/shipping/services/weight-tariff-shipping',
    '<p>' . t('This action cannot be undone.') . '</p>',
    t('Delete'),
    t('Cancel'),
    'confirm'
  );

  return $form;
}

/**
 * Submit callback for commerce_shipping_weight_tariff_service_delete_form().
 */
function commerce_shipping_weight_tariff_service_delete_form_submit($form, &$form_state) {
  $shipping_service = $form_state['shipping_service'];

  commerce_shipping_weight_tariff_service_delete($shipping_service['name']);

  drupal_set_message(t('The service <em>%title</em> has been deleted.', array('%title' => $shipping_service['title'])));
  watchdog('commerce_shipping_weight_tariff', 'Deleted service <em>%title</em>.', array('%title' => $shipping_service['title']), WATCHDOG_NOTICE);

  $form_state['redirect'] = 'admin/commerce/config/shipping/services/weight-tariff-shipping';
}

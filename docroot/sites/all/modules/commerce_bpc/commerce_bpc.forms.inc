<?php
/**
 * @file
 *   Form generation functions for the Commerce bulk product creation module
 */

/**
 * Form constructor for the bulk creation form.
 *
 * Paths: admin/commerce/products/add-bulk/PRODUCT_TYPE
 *
 * @see commerce_bpc_menu()
 * @see commerce_bpc_create_bulk_form_validate()
 * @see commerce_bpc_create_bulk_form_submit()
 * @ingroup forms
 */
function commerce_bpc_create_bulk_form($form, &$form_state, $product_type) {

  $form['#parents'] = array();

  // Create a temporary product object attach fields.
  $new_product = commerce_product_new($product_type);
  $language = !empty($new_product->language) ? $new_product->language : LANGUAGE_NONE;

  // Store the product type for later use.
  $form['product_type'] = array(
    '#type' => 'value',
    '#value' => $product_type,
  );

  $form['description'] = array(
    '#type' => 'item',
    '#title' => t('Bulk Product Creation Form'),
  );

  $form['product'] = array(
    '#type' => 'fieldset',
    '#title' => t('Product Info'),
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,

  );
  $form['combinations'] = array(
    '#type' => 'fieldset',
    '#title' => t('Combinations'),
    '#description' => t('A product will be created for every possible combination of the values you select here.'),
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
    '#tree' => TRUE,
    );

  $form['static_values'] = array(
    '#type' => 'fieldset',
    '#title' => t('Static values'),
    '#description' => t('The values of these fields will be shared by all generated products.'),
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
    '#tree' => TRUE,
    '#parents' => array('static_values'),
  );

  $form['product']['sku_fragment'] = array(
    '#type' => 'textfield',
    '#title' => t('SKU'),
    '#required' => TRUE,
    '#description' => t("The part of the SKU that is common to all products to be generated. The SKU of each individual product will be composed of this value together with values of the selected options."),
    '#size' => 60,
    '#maxlength' => 255,
    '#weight' => 0,
    '#process' => array('commerce_bpc_process_fragment_field'),
  );

  $form['product']['title_fragment'] = array(
    '#type' => 'textfield',
    '#title' => t('Title'),
    '#required' => TRUE,
    '#description' => t("The part of the product title that is common to all products to be generated. The SKU of each individual product will be composed of this value together with values of the selected options."),
    '#size' => 60,
    '#maxlength' => 255,
    '#process' => array('commerce_bpc_process_fragment_field'),
  );


  // We simply attach all fields to the 'static_values' fieldset, and then
  // allow modules to act on each of the fields.
  field_attach_form('commerce_product', $new_product, $form['static_values'], $form_state, $language);
  commerce_bpc_process_field_form_elements($form, $form_state);

  $form['static_values']['status'] = array(
    '#type' => 'radios',
    '#title' => t('Status'),
    '#description' => t('Disabled products cannot be added to shopping carts and may be hidden in administrative product lists.'),
    '#options' => array(
      '1' => t('Active'),
      '0' => t('Disabled'),
    ),
    '#default_value' => 1,
    '#required' => TRUE,
    '#weight' => 200,
  );

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Create products'),
    '#submit' => array('commerce_bpc_create_bulk_form_create_products_submit'),
    '#validate' => array('commerce_bpc_create_bulk_form_validate'),
  );

  if (commerce_bpc_setting('display', 'creation_method', $product_type) == 'wizard') {
    // Show the Display Node option if applicable.
    $display_nodes = commerce_bpc_get_node_types($product_type);
    if (!empty($display_nodes)) {
      $form['next'] = array(
        '#type' => 'submit',
        '#value' => 'Create products and Create display',
        '#submit' => array('commerce_bpc_create_bulk_form_create_products_submit'),
      );
    }
  }

  return $form;
}
/**
 * Implements hook_commerce_bpc_MODULE_NAME_form_element_alter().
 *
 * Moves the price field from the static values fieldset to the
 * product fieldset.
 */
function commerce_bpc_commerce_bpc_commerce_price_form_element_alter(&$form, &$form_state, &$path) {
  $field_widget = drupal_array_get_nested_value($form, $path);
  $lang = $field_widget['#language'];
  $field_name = $field_widget[$lang]['#field_name'];
  $form['product'][$field_name] = drupal_array_get_nested_value($form, $path);
  drupal_array_set_nested_value($form, $path, NULL);
  $path = array('product', array('product', $field_name));

  // Record what we have done.
  if (!isset($form_state['commerce_bpc']['commerce_price'])
  || !in_array($field_name, $form_state['commerce_bpc']['commerce_price']['moved_fields'])) {
    $form_state['commerce_bpc']['commerce_price']['moved_fields'][] = $field_name;
  }

}
/**
 * Implements hook_commerce_bpc_submit_alter().
 */
function commerce_bpc_commerce_bpc_submit_alter(&$form, &$form_state) {
  if (isset($form_state['commerce_bpc']['commerce_price']['moved_fields'])) {
    foreach ($form_state['commerce_bpc']['commerce_price']['moved_fields'] as $field_name) {
      $form_state['values']['static_values'][$field_name] = $form_state['values'][$field_name];
      unset($form_state['values'][$field_name]);
    }
  }
}

/**
 * Form validation hanlder for for commerce_bpc_create_bulk_form().
 *
 * @see commerce_bpc_create_bulk_form_submit()
 */
function commerce_bpc_create_bulk_form_validate($form, &$form_state) {
  // Check to make sure all of the SKUs to be generated are unique and not taken
  // yet.
  $skus = array();
  $data = array();
  $product_type = $form_state['values']['product_type'];

  $data['bulk_data'] = array(
    'product_type' => $product_type,
    'sku_fragment' => $form_state['values']['sku_fragment'],
    'title_fragment' => $form_state['values']['title_fragment'],
  );

  $combinations = commerce_bpc_get_combinations($form, $form_state);
  foreach ($combinations as $combination) {
    $data['bulk_data']['combination'] = $combination;
    $sku_pattern = commerce_bpc_setting('default', 'sku_pattern', $product_type);

    $sku = token_replace($sku_pattern, $data, array('sanitize' => FALSE));
    if (!commerce_product_validate_sku_unique($sku, '')) {
      form_set_error('sku_pattern', t('This pattern yielded one or more invalid SKUs, please use a different pattern. Check for token values that may have spaces.'));
      break;
    }
    elseif (in_array($sku, $skus)) {
      form_set_error('sku_pattern', t('This pattern yielded one or more duplicate SKUs, please use a different pattern.'));
      break;
    }
    else {
      $skus[] = $sku;
    }
  }
}

/**
 * Form submission handler for commerce_bpc_create_bulk_form().
 *
 * @see commerce_bpc_create_bulk_form_validate()
 */
function commerce_bpc_create_bulk_form_create_products_submit($form, &$form_state) {

  global $user;

  $product_type = $form_state['values']['product_type'];

  $combinations = commerce_bpc_get_combinations($form, $form_state);

  $data['bulk_data'] = array(
    'product_type' => $product_type,
    'sku_fragment' => $form_state['values']['sku_fragment'],
    'title_fragment' => $form_state['values']['title_fragment'],
  );

  $sku_pattern = commerce_bpc_setting('default', 'sku_pattern', $product_type);
  $sku_pattern = token_replace($sku_pattern, $data, array('sanitize' => FALSE));
  $title_pattern = commerce_bpc_setting('default', 'title_pattern', $product_type);
  $title_pattern = token_replace($title_pattern, $data, array('sanitize' => FALSE));
  $extras = array(
    'sku_pattern'   => $sku_pattern,
    'title_pattern' => $title_pattern,
    'uid' => $user->uid,
    'status' => $form_state['values']['static_values']['status'],
  );

  drupal_alter('commerce_bpc_submit', $form, $form_state);

  $static_values = $form_state['values']['static_values'];
  unset($static_values['status']);

  $product_ids = commerce_bpc_create_bulk_products($product_type, $combinations, $static_values, $extras);

  // Set a success message.
  if (count($product_ids) > 0) {
    drupal_set_message(t("Successfully generated @count new products.", array('@count' => count($product_ids))));
  }
  $method = commerce_bpc_setting('display', 'creation_method', $product_type);
  switch ($method) {
    case 'auto':
    case 'onetoone':
      $node_type = commerce_bpc_setting('display', 'auto_content_type', $product_type);
      $title_pattern = commerce_bpc_setting('display', 'auto_node_title_pattern', $product_type);

      $node_title = token_replace($title_pattern, $data, array('sanitize' => FALSE));

      if ($method == 'auto') {
        $node = commerce_bpc_create_display_node($node_type, $node_title, $product_ids);
      }
      elseif ($method == 'onetoone') {
        foreach ($product_ids as $product_id) {
          $data = array(
            'commerce-product' => commerce_product_load($product_id),
          );
          // Do another round of token replacement, replacing product-specific
          // tokens.
          $product_node_title = token_replace($node_title, $data, array('sanitize' => FALSE));
          $node = commerce_bpc_create_display_node($node_type, $product_node_title, $product_id);
        }
      }
      switch (commerce_bpc_setting('display', 'auto_redirect', $product_type)) {
        case 'display node' :
          $form_state['redirect'] = 'node/' . $node->nid;
          break;
        case 'custom' :
          $form_state['redirect'] = commerce_bpc_setting('display', 'auto_redirect_custom', $product_type);
          break;
        case 'product listing' :
        default:
          $form_state['redirect'] = 'admin/commerce/products/list';
          break;
      }
      break;
    case 'wizard':
    default:
      // If selected, rebuild the form to let the user choose a display node.
      if ($form_state['input']['op'] == 'Create products and Create display') {
        $form_state['product_type'] = $product_type;

        // Save the product IDs in the SESSION.
        // Generate a id to make sure different bulk creations do not
        // interfere with each other.
        $id = uniqid();
        $_SESSION['bulk_product_ids'][$id] = $product_ids;
        $title_pattern = commerce_bpc_setting('display', 'auto_node_title_pattern', $product_type);
        $_SESSION['bulk_title'][$id] = token_replace($title_pattern, $data, array('sanitize' => FALSE));

        $form_state['redirect'] = 'admin/commerce/products/add-bulk/' . $product_type . '/display/' . $id;

      }
      else {
        $form_state['redirect'] = 'admin/commerce/products';
      }
      break;
  }
}
/**
 * Menu callback: Display list of node types or redirect to node type form.
 *
 * Path: admin/commerce/products/add-bulk/PRODUCT_TYPE/display/BULK_CREATION_ID`
 *
 * @param string $product_type
 *   The type of the created products.
 * @param string $bulk_creation_id
 *   The temporary id of the bulk creation set. Used to find the array of
 *   created product IDs in the $_SESSION array.
 *
 * @see commerce_bpc_menu()
 * @see commerce_bpc_create_bulk_form_submit()
 */
function commerce_bpc_create_bulk_form_display_node($product_type, $bulk_creation_id) {
  $node_type = commerce_bpc_setting('display', 'auto_content_type', $product_type);
  if ($node_type == '_none') {
    $node_type = NULL;
  }
  if (empty($node_type)) {
    $node_types = commerce_bpc_get_node_types($product_type);
    if (count($node_types) == 1) {
      $node_type = reset($node_types);
    }
  }
  if (!empty($node_type)) {
    // Redirect to the node/add form.
    drupal_goto('node/add/' . str_replace('_', '-', $node_type), array('query' => array('bulk_creation_id' => $bulk_creation_id)));
  }
  else {
    $form['description'] = array(
      '#type' => 'item',
      '#title' => t('Bulk Product Creation Form - Choose Display Node type'),
    );

    // Store the product type for later use.
    $form['product_type'] = array(
      '#type' => 'value',
      '#value' => $product_type,
    );
    $form['bulk_creation_id'] = array(
      '#type' => 'value',
      '#value' => $bulk_creation_id,
    );

    $form['node_type'] = array(
      '#type' => 'radios',
      '#title' => t('Display Node'),
      '#options' => $node_types,
      '#required' => TRUE,
      '#description' => t("Select which Display Node Type to use.
        Note that the Node Type must have a Product Reference Field with multiple values which can accept products of the chosen type to be considered as a candidate."),
    );

    $form['submit'] = array(
      '#type' => 'submit',
      '#value' => t('Create node'),
      '#submit' => array('commerce_bpc_create_bulk_form_create_display_submit'),
      '#validate' => array(),
    );

    return $form;
  }
}

/**
 * The bulk product choose display form submit handler.
 */
function commerce_bpc_create_bulk_form_create_display_submit($form, &$form_state) {

  // Redirect to the node/add form.
  drupal_goto('node/add/' . str_replace('_', '-', $form_state['values']['node_type']), array('query' => array('bulk_creation_id' => $form_state['values']['bulk_creation_id'])));
}

/**
 * Process the attached form elements.
 */
function commerce_bpc_process_field_form_elements(&$form, &$form_state) {
  $product_type = $form['product_type']['#value'];
  foreach (element_children($form['static_values']) as $field_key) {
    $element = $form['static_values'][$field_key];
    if (@empty($element[$element['#language']])) {
      continue;
    }
    $field_name = $element[$element['#language']]['#field_name'];
    $info = field_info_field($field_name);
    $instance_info = field_info_instance('commerce_product', $field_name, $product_type);
    // Filter out fields that are set to be hidden.
    if (!_commerce_bpc_get_value($instance_info, array('commerce_bpc', 'show_field'), TRUE)) {
      unset($form['static_values'][$field_key]);
    }
    else {
      $types = array();
      $types[] = 'commerce_bpc_' . $info['type'] . '_form_element';
      if ($info['type'] != $info['module']) {
        $types[] = 'commerce_bpc_' . $info['module'] . '_form_element';
      }
      $types[] = 'commerce_bpc_form_element';
      $path = array('static_values', $field_key);
      drupal_alter($types, $form, $form_state, $path);
    }
  }
}

/**
 * Process SKU and title fragment fields.
 */
function commerce_bpc_process_fragment_field($element, &$form_state, &$form) {
  $product_type = $form['product_type']['#value'];
  $tokens = commerce_bpc_token_info_by_product_type($product_type);
  $samples = commerce_bpc_token_sample_values($product_type);

  $data['bulk_data']['combination'] = $samples['values'];
  $data['bulk_data']['product_type'] = $product_type;

  switch ($element['#name']) {
    case 'sku_fragment':
      $pattern = commerce_bpc_setting('default', 'sku_pattern', $product_type);
      $token = '[bulk_defaults:entered_sku]';
      $target_id = 'sku_example';
      $label = t('Example variant SKU:');
      break;
    case 'title_fragment':
      $pattern = commerce_bpc_setting('default', 'title_pattern', $product_type);
      $token = '[bulk_defaults:entered_title]';
      $target_id = 'title_example';
      $label = t('Example variant title:');
      break;
  }
  $pattern = token_replace($pattern, $data, array('sanitize' => FALSE));
  $wrapper_id = $target_id . '_wrapper';
  $element['#field_suffix'] = '<small id="' . $wrapper_id . '">' . $label . ' <span id="' . $target_id . '"></span></small>';


  $js_settings = array(
    'type' => 'setting',
    'data' => array(
      'commerce_bpc' => array(
        '#' . $element['#id'] => array(
          'pattern' => $pattern,
          'target' => '#' . $target_id,
          'target_wrapper' => '#' . $wrapper_id,
          'token' => $token,
        ),
      ),
    ),
  );
  $element['#attached']['js'][] = drupal_get_path('module', 'commerce_bpc') . '/js/pattern.js';
  $element['#attached']['js'][] = $js_settings;

  return $element;
}
/**
 * Create a display node that links to a set of products.
 *
 * @param string $node_type
 *   The machine name of the node type of the node to be created.
 * @param string $title
 *   The title of the node to be created.
 * @param int|array $product_ids
 *   Either a single product ID or an array of product IDs that will be linked
 *   from the newly-created node.
 */
function commerce_bpc_create_display_node($node_type, $title, $product_ids) {
  if (!is_array($product_ids)) {
    $product_ids = array($product_ids);
  }

  $node = new stdClass();
  $node->type = $node_type;
  node_object_prepare($node);
  $node->language = LANGUAGE_NONE;
  $node->title = $title;

  $ref_field_name = _commerce_bpc_get_reference_field_name($node_type);
  foreach ($product_ids as $id) {
    $node->{$ref_field_name}[LANGUAGE_NONE][] = array('product_id' => $id);
  }
  drupal_alter('commerce_bpc_display_node', $node);
  node_save($node);
  return $node;
}

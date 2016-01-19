<?php
/**
 * @file
 * File to store the quantity forms.
 */

/**
 * Default calculation method form.
 */
function commerce_stock_calculation_default_method($form, &$form_state) {

  $default = variable_get('stock_calculation_default_method', STOCK_CALCULATION_DEFAULT_METHOD);
  $options = commerce_stock_calculation_get_plugins_as_options('stock_calculation');

  $form['stock_calculation_default_method'] = array(
    '#title' => t('Default calculation method'),
    '#type' => 'radios',
    '#options' => $options,
    '#default_value' => $default,
  );

  return system_settings_form($form);
}

/**
 * Implements hook_form().
 */
function commerce_stock_calculation_settings($form, &$form_state, $product) {

  global $user;

  $form['#product'] = $product;
  $form['#account'] = $user;

  // Load all plugins of type "stock_calculation".
  $plugins = commerce_stock_calculation_get_plugins_as_options('stock_calculation');

  // The active plugin for this product.
  if (!empty($form_state['values']['plugins']['plugin'])) {
    $plugin_type = $form_state['values']['plugins']['plugin'];
  }
  else {
    $plugin_type = commerce_stock_calculation_get_active_plugin($product);
  }

  $contexts = array(
    'product' => $product,
  );

  $plugin = commerce_stock_calculation_get_plugin_instance($plugin_type, $contexts);

  $form['plugins'] = array(
    '#title' => t('Choose the method to calculate the stock.'),
    '#type' => 'fieldset',
    '#tree' => TRUE,
  );

  $form['plugins']['plugin'] = array(
    '#title' => t('Method.'),
    '#type' => 'radios',
    '#options' => $plugins,
    '#default_value' => $plugin_type,
    '#ajax' => array(
      'callback' => 'plugin_setttings_choice_js',
      'wrapper' => 'plugin-settings',
      'method' => 'replace',
      'effect' => 'fade',
    ),
  );

  $plugin_settings_form = $plugin->settingsForm($form, $form_state);

  $form['plugins']['plugin_settings'] = array(
    '#prefix' => '<div id="plugin-settings">',
    '#suffix' => '</div>',
  ) + $plugin_settings_form;

  $form['actions'] = array(
    '#type' => 'actions',
  );

  $form['actions']['submit'] = array(
    '#value' => t('Save'),
    '#type' => 'submit',
  );

  return $form;
}

/**
 * Menu callback for AJAX additions. Render the new plugin settings choices.
 */
function plugin_setttings_choice_js($form, $form_state) {
  return $form['plugins']['plugin_settings'];
}

/**
 * Implements hook_form_submit().
 */
function commerce_stock_calculation_settings_submit($form, &$form_state) {

  $plugins = $form_state['values']['plugins'];
  $product = $form['#product'];
  $product_id = $product->product_id;
  $plugin_id = "stock_calculation:$product_id";

  $calculation_plugin = $plugins['plugin'];
  $calculation_plugin_settings = !empty($plugins['plugin_settings']) ? $plugins['plugin_settings'] : array();

  // Save the calculation plugin.
  commerce_stock_calculation_save_config($plugin_id, $calculation_plugin, $calculation_plugin_settings);

  $plugin = commerce_stock_calculation_get_plugin_instance($calculation_plugin, array('product' => $product));

  $quantity = $plugin->calculate();

  // Save the total stock.
  $wrapper = entity_metadata_wrapper('commerce_product', $form['#product']);
  $wrapper->commerce_stock->set($quantity);
  $wrapper->save();

}

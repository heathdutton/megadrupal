<?php

/**
 * @file
 * Admin page callbacks for the Commerce Product URLs module.
 */

/**
 * Form constructor for the realm settings form.
 */
function commerce_product_urls_settings_form($form, &$form_state) {
  $form = array();

  $form['commerce_product_urls_url_key'] = array(
    '#type' => 'select',
    '#title' => t('URL query property'),
    '#options' => array(
      'id' => t('Product ID'),
      'sku' => t('Product SKU'),
    ),
    '#default_value' => variable_get('commerce_product_urls_url_key', 'id'),
    '#description' => t('Select which product property should be used as the product identifier in the product display URL.'),
  );

  $form['commerce_product_urls_update_url'] = array(
    '#type' => 'checkbox',
    '#title' => t('Update page URL on attribute change'),
    '#default_value' => variable_get('commerce_product_urls_update_url', FALSE),
    '#description' => t('Dynamically update product display page URL when selected value of any of the product fields enabled as an attribute field on <em>Add to Cart</em> forms changes. This feature is based on HTML5 <em>history.pushState()</em>, therefore it will not work in older browsers which do not support HTML5.'),
  );

  $form['commerce_product_urls_update_url_fallback'] = array(
    '#type' => 'checkbox',
    '#title' => t('Fallback to <em>location.search</em> update for older browsers'),
    '#default_value' => variable_get('commerce_product_urls_update_url_fallback', FALSE),
    '#description' => t('For older browsers which do not support HTML5 <em>history.pushState()</em>, product display page URL could still be updated through changing <em>location.search</em> value, this will however reload the page each time the value of the product field enabled as an attribute field on <em>Add to Cart</em> forms changes.'),
    '#states' => array(
      'visible' => array(
        ':input[name="commerce_product_urls_update_url"]' => array('checked' => TRUE),
      ),
    ),
  );

  return system_settings_form($form);
}

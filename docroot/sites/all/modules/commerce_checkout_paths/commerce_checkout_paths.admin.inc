<?php
/**
 * @file
 * Drupal Commerce Custom Checkout pages paths admin page form.
 */

/**
 * Settings form for Commerce checkout pages custom paths.
 */
function commerce_checkout_paths_settings_form() {
  $form = array(); $t = get_t();
  $form['commerce_checkout_paths_settings'] = array(
    '#type'        => 'fieldset',
    '#title'       => $t('Commerce checkout paths configuration.'),
    '#description' => $t('Set your custom paths for a checkout pages like: shop/step1'),
  );

  foreach (_commerce_checkout_paths_get_checkout_pages_list() as $name => $data) {
    $title = $t($data['title']) . ' <i>(machine_name: ' . $t($name) . ')</i>';
    $value = variable_get('commerce_checkout_paths_' . $name, FALSE);
    $form['commerce_checkout_paths_settings']['commerce_checkout_paths_' . $name] = array(
      '#type'           => 'textfield',
      '#title'          => $title,
      '#default_value'  => $value,
      '#description'    => $data['description'],
    );
  }
  $form['commerce_checkout_paths_settings']['token_tree'] = array(
    '#type' => 'fieldset',
    '#title' => $t('Replacement patterns'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
    '#description' => theme('token_tree', array('token_types' => array('commerce-order'))),
  );
  $form['commerce_checkout_paths_settings']['commerce_checkout_paths_auto_redirect'] = array(
    '#type'           => 'checkbox',
    '#title'          => $t('Apply form redirect alter'),
    '#default_value'  => variable_get('commerce_checkout_paths_auto_redirect', FALSE),
    '#description'    => $t('Automatically add a custom form submits with redirect to next/previous step with using custom URLs.'),
  );
  return system_settings_form($form);
}

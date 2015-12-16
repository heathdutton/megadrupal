<?php

/**
 * @file
 * Admin page callbacks.
 */

/**
 * Display the liste of rates for a shipping service.
 *
 * @param $name
 *   The name of the view to embed.
 * @param $display_id
 *   The display id to embed. If unsure, use 'default', as it will always be
 *   valid. But things like 'page' or 'block' should work here.
 * @param $method_id
 *   The shipping method id.
 * @param $service_name
 *   The shipping service machine name.
 *
 * @return array
 *   The renderable array.
 */
function commerce_shipping_rate_views_embed_view($name, $display_id = 'default', $method_id, $service_name) {
  $content = array();

  $content['rates'] = array(
    '#markup' => commerce_embed_view($name, $display_id, array($method_id, $service_name), current_path()),
  );

  return $content;
}

/**
 * Display an overview of all the shipping services by method.
 *
 * @return array
 *   The renderable array.
 */
function commerce_shipping_rates_methods_overview($method) {
  $items['head'] = array(
    '#prefix' => '<h2>',
    '#suffix' => '</h2>',
    '#markup' => t('Select a shipping method')
  );
  foreach (commerce_shipping_methods() as $method_id => $method) {
    // Convert underscores to hyphens for the menu item argument.
    $method_name_arg = strtr($method_id, '_', '-');
    $items[] = array(
      '#prefix' => '<p>',
      '#suffix' => '</p>',
      '#markup' => l($method['title'], 'admin/commerce/config/shipping/shipping-rates/' . $method_name_arg),
    );
  }
  return $items;
}

/**
 * Display an overview of all the shipping services by method.
 *
 * @return array
 *   The renderable array.
 */
function commerce_shipping_rates_services_overview($method_id) {
  $method = commerce_shipping_method_load($method_id);
  $content = array();

  $content['head'] = array(
    '#prefix' => '<h2>',
    '#suffix' => '</h2>',
    '#markup' => t('Select a shipping service'),
  );

  // Convert underscores to hyphens for the menu item argument.
  $method_name_arg = strtr($method['name'], '_', '-');

  // Create links foreach services to manage rates.
  foreach (commerce_shipping_services($method['name']) as $service_name => $service) {
    $content['services'][strtr($service_name, '-', '_')] = array(
      '#type' => 'container',
      '#attributes' => array('class' => array('shipping-rates-service')),
    );
    // Convert underscores to hyphens for the menu item argument.
    $service_name_arg = 'shipping-rates' . '-' . strtr($service_name, '_', '-');

    $content['services'][strtr($service_name, '-', '_')]['link'] = array(
      '#prefix' => '<p>',
      '#suffix' => '</p>',
      '#markup' => l(t('%service rates', array('%service' => $service['title'])) . '<br><small>(' . $service_name . ')</small>', 'admin/commerce/config/shipping/shipping-rates/' . $method_name_arg . '/' . $service_name_arg, array('html' => TRUE)),
    );
  }

  return $content;
}

/**
 * Form callback: Import rates for shipping services.
 */
function commerce_shipping_rate_import_form($form, &$form_state) {
  $form['shipping_rate_import'] = array(
    '#tree' => TRUE,
    '#type' => 'fieldset',
    '#description' => t('Import rates for shipping services.'),
    '#title' => t('Import'),
  );

  $form['shipping_rate_import']['file'] = array(
    '#type' => 'file',
    '#title' => t('Import CSV file'),
    '#title_display' => 'invisible',
    '#description' => t('Only .csv file are allowed (comma separator).'),
  );

  $form['shipping_rate_import']['clear'] = array(
    '#type' => 'checkbox',
    '#title' => t('Erase all previous rates data.'),
  );

  $form['actions']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Import'),
  );

  return $form;
}

/**
 * Validation form callback: Checks data integrity.
 */
function commerce_shipping_rate_import_form_validate($form, &$form_state) {
  $file = reset($_FILES);
  $file_name = reset($file['tmp_name']);
  $file_type = reset($file['type']);
  $element = $form['shipping_rate_import']['file'];

  if (!empty($file_name) && !empty($file_type)) {
    if ($file_type !== 'text/csv') {
      form_error($element, t('Only CSV extension are allowed.'));
    }
    if (empty($file_name)) {
      form_error($element, t('Error when writing the file in the temporary directory.'));
    }

    $lines = array();
    if (($handle = fopen($file_name, "r")) !== FALSE) {
      while (($data = fgets($handle, 1024)) !== FALSE) {
        $lines[] = $data;
      }
      fclose($handle);
    }

    $data = _commerce_shipping_rate_import_parse_data($lines);

    if (empty($data)) {
      form_error($element, t('No data found in the passed file.'));

    }
    elseif (isset($data['message'])) {
      form_error(
        $element, t(
          'Error on line !delta: !message',
          array(
            '!delta' => ($data['delta'] + 1),
            '!message' => $data['message'],
          )
        )
      );
    }
    else {
      form_set_value($element, $data, $form_state);
    }
  }
  else {
    form_error($element, t('Please, attach a file.'));
  }
}

/**
 * Submission form callback: saves the import values in database.
 */
function commerce_shipping_rate_import_form_submit($form, &$form_state) {
  $data = $form_state['values']['shipping_rate_import']['file'];

  if ($form_state['values']['shipping_rate_import']['clear'] === 1) {
    // Drop every existing rates.
    $rate_ids = array_keys(entity_load('commerce_shipping_rate'));
    commerce_shipping_rate_delete_multiple($rate_ids);
  }

  if (_commerce_shipping_rate_import_save_data($data)) {
    drupal_set_message(t('The data have been successfully imported in database.'), 'status');
  }
  else {
    drupal_set_message(t('Something wrong happens during the import.'), 'error');
  }
}

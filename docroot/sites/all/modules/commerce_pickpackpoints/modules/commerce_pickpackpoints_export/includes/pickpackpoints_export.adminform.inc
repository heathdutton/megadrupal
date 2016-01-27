<?php

/**
 * @file
 * Implements pickpack pont export form.
 */

/**
 * Implements hook_form().
 *
 * Generate administration form to select orders to generate CSV file.
 */
function commerce_pickpackpoints_export_admin_form($form, &$form_state) {
  // Build the table header.
  $header = array(
    'name' => t('Name'),
    'smethod' => t('Shipping method'),
    'created' => t('Created'),
    'pmethod' => t('Payment method'),
  );

  // Build the rows.
  $options = array();

  // All status what we want to exporting to CSV file.
  $statuses = array_filter(variable_get('pickpack_export_statuses', array('pending')));

  $result = NULL;
  if (!empty($statuses)) {
    // Get orders what need.
    $query = new EntityFieldQuery();
    $result = $query
        ->entityCondition('entity_type', 'commerce_order')
        ->propertyCondition('status', $statuses, 'in')
        ->fieldCondition('pickpack_field_pickpack', 'value', '', '<>')
        ->execute();
  }

  if (!empty($result)) {
    $ids = array_keys($result['commerce_order']);
    $commerce_orders = commerce_order_load_multiple($ids);

    // Load payment methods.
    $payment_methods = commerce_payment_methods();

    foreach ($commerce_orders as $order) {
      $method = '';
      $payment_method = explode('|', $order->data['payment_method']);
      if (!empty($payment_method)) {
        $method = $payment_methods[$payment_method[0]];
      }

      $profile = commerce_pickpackpoints_export_get_commerce_profile($order);
      if (empty($profile)) {
        // If no valid profile, this will not export to CSV file.
        continue;
      }

      $name = $profile->commerce_customer_address[LANGUAGE_NONE][0]['name_line'];
      if (empty($name)) {
        $name = $profile->commerce_customer_address[LANGUAGE_NONE][0]['last_name'] . ' ' . $profile->commerce_customer_address[LANGUAGE_NONE][0]['first_name'];
      }

      $shipping_method = isset($order->data['shipping_service']) ? $order->data['shipping_service']->line_item_label : 'Pick pack';
      $options[$order->order_number] = array(
        'name' => $name,
        'smethod' => $shipping_method,
        'created' => format_date($order->created, 'short'),
        'pmethod' => $method['title'],
      );
    }
  }
  // Build the tableselect.
  $form['orders'] = array(
    '#type' => 'tableselect',
    '#header' => $header,
    '#options' => $options,
    '#empty' => t('No content available.'),
  );

  $form['descriiption'] = array(
    '#markup' => t('Please select the orders what upload to pick-pack pont site.'),
    '#prefix' => '<p>',
    '#suffix' => '</p>',
  );

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Start export'),
  );

  return $form;
}

/**
 * Export form submit. Generate CSV file from needle data.
 */
function commerce_pickpackpoints_export_admin_form_submit($form, &$form_state) {
  // Filter to checked orders.
  $order_ids = array_filter($form_state['values']['orders']);
  if (!empty($order_ids)) {
    // Load all needle order if not empty the order id array.
    $orders = commerce_order_load_multiple($order_ids);

    // Create filename and open file to write.
    $url = drupal_realpath('public://ppp_exports/');
    $filename = 'pickpack_exp_' . date("YmdHis") . '.csv';
    $fp = fopen($url . '/' . $filename, 'w');

    // Build first row of CSV file.
    $row = array(
      iconv("UTF-8", "ISO-8859-2", "Bolt azonosito"),
      iconv("UTF-8", "ISO-8859-2", "Vevő név"),
      iconv("UTF-8", "ISO-8859-2", "Vevő email"),
      iconv("UTF-8", "ISO-8859-2", "Vevő telefonszáma"),
      iconv("UTF-8", "ISO-8859-2", "Csomag azonositó"),
      iconv("UTF-8", "ISO-8859-2", "Csomag ár"),
      iconv("UTF-8", "ISO-8859-2", "Csomag méret"),
      iconv("UTF-8", "ISO-8859-2", "Csomag súly"),
      iconv("UTF-8", "ISO-8859-2", "Szállítmány azonosító"),
    );
    fputcsv($fp, $row, ';');

    // Fetch data rows.
    foreach ($orders as $order) {
      // Load customer profile.
      $profile = commerce_pickpackpoints_export_get_commerce_profile($order);

      $name = $profile->commerce_customer_address[LANGUAGE_NONE][0]['name_line'];
      if (empty($name)) {
        $name = $profile->commerce_customer_address[LANGUAGE_NONE][0]['last_name'] . ' ' . $profile->commerce_customer_address[LANGUAGE_NONE][0]['first_name'];
      }

      // Get customer phone number.
      $phone_field = variable_get('ppp_phone_field', 'pickpack_phone');
      $phone = isset($profile->{$phone_field}[LANGUAGE_NONE][0]['value']) ? $profile->{$phone_field}[LANGUAGE_NONE][0]['value'] : '1111111111';

      // Build row to CSV file.
      $row = array(
        $order->data['pick_pack_shop'],
        iconv("UTF-8", "ISO-8859-2", $name),
        $order->mail,
        $phone,
        iconv("UTF-8", "ISO-8859-2", variable_get('ppp_prefix', 'ppp') . '_' . $order->order_number),
        $order->commerce_order_total[LANGUAGE_NONE][0]['amount'],
        '0',
        '1',
        iconv("UTF-8", "ISO-8859-2", variable_get('ppp_prefix', 'ppp') . '_' . date("Ymd")),
      );

      // Write row to CSV file.
      fputcsv($fp, $row, ';');
    }

    // Close file.
    fclose($fp);

    // Redirect to result page.
    $form_state['redirect'] = 'admin/commerce/pickpack-export-result/' . $filename;
  }
}

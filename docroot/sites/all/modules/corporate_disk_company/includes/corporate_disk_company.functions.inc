<?php

/**
 * @file
 * Corporate Disk Fulfillment functions.
 */

/**
 * Function to send order to disk.com.
 */
function corporate_disk_company_process($caller) {
  // Check to make sure this is not running from some core scans.
  // This eliminates acidental runs of this code that could cause shipments and hence cost money.
  if ($caller != 'cron' && $caller != 'testpage') {
    return FALSE;
  }
  $order_number = 0;
  $commerce_shipping_services = commerce_shipping_services();
  foreach ($commerce_shipping_services as $key => $value) {
    $commerce_shipping_service_names[$value['title']] = $key;
  }
  $disk_shipping_services = variable_get('corporate_disk_company_shipping_services');
  if (empty($disk_shipping_services)) {
    watchdog('corporate_disk_company', '<h2>Configuration</h2><br /><b>You have not configured any shipping services for Disk.com</b>');
    return FALSE;
  }
  $shipping_default_conus = variable_get('corporate_disk_company_shipping_default_conus', '');
  $shipping_default_oconus = variable_get('corporate_disk_company_shipping_default_oconus', '');
  if (empty($shipping_default_conus) || empty($shipping_default_oconus)) {
    watchdog('corporate_disk_company', '<h2>Configuration</h2><br /><b>You must set a default shipping method for CONUS and OCONUS.</b>');
    return FALSE;
  }
  $diskcom_sku_list = '';
  $diskcom_base_sku_list = list_extract_allowed_values(variable_get('corporate_disk_company_base_sku_list'), 'list_text', FALSE);
  $diskcom_bundles_list = list_extract_allowed_values(variable_get('corporate_disk_company_bundle_sku_list'), 'list_text', FALSE);
  foreach ($diskcom_base_sku_list as $key => $value) {
    $diskcom_sku_list .= '\'' . $value . '\', ';
  }
  if (!empty($diskcom_bundles_list)) {
    foreach ($diskcom_bundles_list as $key => $value) {
      $diskcom_sku_list .= '\'' . $key . '\', ';
    }
  }
  // Pull a variable for the backtrack amount, by default we go 24hours.
  $now = time();
  $startdate = $now - variable_get('corporate_disk_company_backtrack_time', 86400);
  $diskcom_sku_list = rtrim($diskcom_sku_list, ', ');
  // Query to get customer shipping information.
  $ordersquery = "select distinct a.*
                from
                (
                    select o.order_number, o.created as created, o.changed as changed, s.revision_id, s.commerce_customer_shipping_profile_id, a.commerce_customer_address_name_line,
                    a.commerce_customer_address_country,
                    a.commerce_customer_address_administrative_area,
                    a.commerce_customer_address_sub_administrative_area,
                    a.commerce_customer_address_locality,
                    a.commerce_customer_address_dependent_locality,
                    a.commerce_customer_address_postal_code,
                    a.commerce_customer_address_thoroughfare,
                    a.commerce_customer_address_premise,
                    a.commerce_customer_address_sub_premise,
                    o.mail
                    from {field_data_commerce_customer_shipping} s,
                    {field_data_commerce_customer_address} a,
                    {commerce_order} o
                    where a.entity_id = s.commerce_customer_shipping_profile_id
                    and o.revision_id = s.revision_id
                    and o.status like 'completed'
                    and o.changed between $startdate and $now 
                ) a
                left outer join
                (
                    select o.order_number, o.uid, o.created AS created, l.line_item_label, l.quantity, p.commerce_unit_price_amount/100 commerce_unit_price_amount, o.status
                    from {commerce_order} o, {commerce_line_item} l, {field_data_commerce_unit_price} p
                    where o.order_id = l.order_id
                    and l.line_item_id = p.entity_id
                    and o.status = 'completed'
                    and l.line_item_label in ($diskcom_sku_list)
                    and o.changed between $startdate and $now
                ) b
                on a.order_number = b.order_number
                where b.line_item_label is not null
                order by a.order_number;";
  if ($order_query_result = db_query($ordersquery)) {
    foreach ($order_query_result as $order) {
      $order_number = $order->order_number;
      $name = rawurlencode($order->commerce_customer_address_name_line);
      $address1 = rawurlencode($order->commerce_customer_address_thoroughfare);
      $address2 = rawurlencode($order->commerce_customer_address_premise);
      $city = rawurlencode($order->commerce_customer_address_locality);
      $state = rawurlencode($order->commerce_customer_address_administrative_area);
      $zip = rawurlencode($order->commerce_customer_address_postal_code);
      $country = $order->commerce_customer_address_country;
      $phone = '';
      $shippingType = '';
      $items = '';
      $emailAddress = rawurlencode($order->mail);
      include_once DRUPAL_ROOT . '/includes/locale.inc';
      $countryarray = country_get_list();
      $countryname = rawurlencode($countryarray[$country]);
      // Query to get the shipping method selected for the order.
      $shippingrow = db_select('commerce_line_item', 'c')
          ->fields('c')
          ->condition('order_id', $order->order_number, '=')
          ->condition('type', 'shipping', 'like')
          ->execute()
          ->fetchAssoc();
      if (!empty($shippingrow)) {
        $shippingvar = 'corporate_disk_company_shipping_services_' . $commerce_shipping_service_names[$shippingrow['line_item_label']];
        $shippingtype = variable_get($shippingvar, '');
      }
      else {
        if ($country == 'US') {
          $shippingtype = $shipping_default_conus;
        }
        else {
          $shippingtype = $shipping_default_oconus;
        }
      }
      // Query to prepare orders and SKUs
      $orderitemsquery = "select o.order_number, o.uid, o.created AS created, l.line_item_label, l.quantity, p.commerce_unit_price_amount/100 commerce_unit_price_amount, o.status
                        from {commerce_order} o, {commerce_line_item} l, {field_data_commerce_unit_price} p
                        where o.order_id = l.order_id
                        and l.line_item_id = p.entity_id
                        and o.status = 'completed'
                        and l.line_item_label in ($diskcom_sku_list)
                        and o.changed between $startdate and $now
                        and o.order_number = $order->order_number";
      // The next section of code will prepare and perform our SKU replacements.
      if (!empty($diskcom_bundles_list)) {
        foreach ($diskcom_bundles_list as $key => $value) {
          // Process the UI saved data into a nested array.
          $sku_replacements_bundles[$key] = explode(' ', $value);
        }
      }
      else {
        $sku_replacements_bundles = array();
      }
      // Do we have a global SKU replacement?
      $needle = variable_get('corporate_disk_company_base_sku_substitution_needle', '');
      $haystack = variable_get('corporate_disk_company_base_sku_substitution_haystack', '');
      if ($orderitems_query_result = db_query($orderitemsquery)) {
        foreach ($orderitems_query_result as $orderitems) {
          // Lets walk through the SKUs.
          $current_sku = $orderitems->line_item_label;
          // Does the current SKU exist in the bundle SKU replacement array from above.
          if (array_key_exists($current_sku, $sku_replacements_bundles)) {
            // If it exists lets take the nested array from the bundle data and create our data for Disk.com call
            foreach ($sku_replacements_bundles[$current_sku] as $values) {
              $items .= $values . ':' . intval($orderitems->quantity) . '|';
            }
          }
          else {
            // Do we have a global SKU replacement set?
            if (!empty($needle) && !empty($haystack)) {
              // Replace the global SKUs
              $items .= str_replace($needle, $haystack, $orderitems->line_item_label) . ':' . intval($orderitems->quantity) . '|';
            }
            // Last option is we take the SKU's and build our disk call if all of the above logic was skipped.
            // This doesn't work... It will run and screw shit up from the bundles.
            else {
              $items .= $orderitems->line_item_label . ':' . intval($orderitems->quantity) . '|';
            }
          }
        }
      }
      $items = rtrim($items, '|');
      $disk_shiptocompany = '';
      $disk_requestedby = variable_get('corporate_disk_company_requested_by_name', 'cart');
      $teststatus = variable_get('corporate_disk_company_live_test', 0);
      if ($teststatus == 1) {
        $disk_attention = 'TEST';
      }
      else {
        $disk_attention = $name;
      }
      $disk_address1 = $address1;
      $disk_address2 = $address2;
      $disk_city = $city;
      $disk_state = $state;
      $disk_zipcode = $zip;
      $disk_country = $countryname;
      $disk_phonenumber = $phone;
      $disk_faxnumber = '';
      $disk_shippingmethod = rawurlencode($shippingtype);
      $disk_shippingnumber = $order->order_number;
      $disk_shippingnotes = 'Web%20Order%20No.%20' . $order->order_number;
      $disk_notes = rawurlencode($order->changed);
      $disk_emailaddress = $emailAddress;
      $disk_userdef1 = '';
      $disk_userdef2 = '';
      $disk_userdef3 = '';
      $disk_ending_string = 'Order%20' . $order->order_number;
      $disk_ending_string .= '&items=' . $items;
      $disk_ending_string .= '&companyid=' . variable_get('corporate_disk_company_companyid', '');
      $disk_ending_string .= '&shipacct=';
      $url = 'https://customers.disk.com/remotepackageship.jsp?os=';
      $url .= $disk_shiptocompany . '|' . $disk_requestedby . '|' . $disk_attention . '|' . $disk_address1 . '|' . $disk_address2 . '|';
      $url .= $disk_city . '|' . $disk_state . '|' . $disk_zipcode . '|' . $disk_country . '|' . $disk_phonenumber . '|' . $disk_faxnumber . '|';
      $url .= $disk_shippingmethod . '|' . $disk_shippingnumber . '|' . $disk_shippingnotes . '|' . $disk_notes . '|' . $disk_emailaddress . '|' . $disk_userdef1 . '|';
      $url .= $disk_userdef2 . '|' . $disk_userdef3 . '|' . $disk_ending_string;

      $result = db_select('corporate_disk_company_shipments', 'c')
          ->fields('c')
          ->condition('order_number', $order->order_number, '=')
          ->execute()
          ->rowCount();
      // This is to make certain the orders have not been sent before.
      if ($result < 1) {
        // UI Switch to turn live transactions on or off.
        $status = variable_get('corporate_disk_company_live_status', 0);
        if ($status == 1) {
          $options = array(
            CURLOPT_RETURNTRANSFER => TRUE, // return web page
            CURLOPT_HEADER => FALSE, // don't return headers
            CURLOPT_FOLLOWLOCATION => TRUE, // follow redirects
            CURLOPT_MAXREDIRS => 10, // stop after 10 redirects
            CURLOPT_CONNECTTIMEOUT => 120, // time-out on connect
            CURLOPT_TIMEOUT => 120, // time-out on response
            CURLOPT_SSL_VERIFYHOST => FALSE, // Don't verify host
            CURLOPT_SSL_VERIFYPEER => FALSE, // Don't verify peer
            CURLOPT_CUSTOMREQUEST => 'GET', // HTTP request method
          );
          $ch = curl_init($url);
          curl_setopt_array($ch, $options);
          $content = curl_exec($ch);
          if (curl_errno($ch)) {
            watchdog('corporate_disk_company', '<h2>cURL Fault</h2><br /><b>cURL Error Code:</b> @exc', array('@exc' => curl_errno($ch)));
            $disk_error_number = curl_errno($ch);
            // Now to handle the error code for reprocessing.
            switch ($disk_error_number) {
              case '7':
                $confirmation = 'DISK-RETRY';
                break;
              default:
                $confirmation = 'DISK-FAILURE-cURL-ERROR-NUMBER-' . $disk_error_number;
                break;
            }
          }
          else {
            $resarr = array();
            $resarr = json_decode($content);
            $confirmation = trim(filter_xss($resarr));
          }
          curl_close($ch);
          // Only insert data into the databse if we got a response.
          // This is to handle situations where disk.com is unreachable.
          if (!empty($confirmation)) {
            $queue = db_insert('corporate_disk_company_shipments')
                ->fields(array(
                  'order_number' => $order->order_number,
                  'confirmation' => $confirmation,
                  'service_post' => $url,
                  'shipping_service' => $shippingtype,
                  'created_at' => REQUEST_TIME,
                ))
                ->execute();
          }
          unset($response);
        }
        else {
          $confirmation = 'TRANS-DISABLED';
          $queue = db_insert('corporate_disk_company_shipments')
              ->fields(array(
                'order_number' => $order->order_number,
                'confirmation' => $confirmation,
                'service_post' => $url,
                'shipping_service' => $shippingtype,
                'created_at' => REQUEST_TIME,
              ))
              ->execute();
        }
      }
    }
    return TRUE;
  }
  // Default to fail.
  return FALSE;
}

/**
 * Function to update accounts with tracking numbers.
 */
function corporate_disk_company_tracking($caller = NULL) {
  if ($caller != 'cron' && $caller != 'testpage') {
    return FALSE;
  }
  // Lets pull orders that need to be updated with tracking numbers.
  $sql = 'SELECT * from {corporate_disk_company_shipments} '
      . 'where order_updated = \'0\' '
      . 'AND shipping_service NOT LIKE \'USPS First Class\' '
      . 'AND '
      . '(confirmation IS NOT NULL '
      . 'OR confirmation != \'\' '
      . 'OR confirmation NOT LIKE \'DISK\' '
      . 'OR confirmation NOT LIKE \'TRANS\')';
  $result = db_query($sql)
      ->fetchAll();
  $count = $result->num_rows;
  if ($count == 0) {
    // Stop processing if there are no orders to update.
    return TRUE;
  }
  $orders = array();
  $orders_update = array();
  $responsearray = array();
  // Lets create an array of items that need to be updated.
  foreach ($result as $items) {
    $orders[] = array(
      'ordernumber' => $items->order_number,
      'orderconfirm' => $items->confirmation,
    );
  }
  $id = variable_get('corporate_disk_company_companyid', '');
  try {
    foreach ($orders as $key => $value) {
      $context = stream_context_create(array('http' => array('header' => 'Accept: application/xml')));
      $url = 'http://customers.disk.com/shippinginfo.jsp?cid=' . $id . '&packageshipmentid=' . trim($value['orderconfirm']);
      $response = file_get_contents($url, FALSE, $context);
      $response = filter_xss($response);
      $response = simplexml_load_string($response);
      $vars = get_object_vars($response);
      foreach ($vars as $key => $val) {
        $responsearray[$key] = $val;
      }
      if ($responsearray['status'] == 'completed' && !empty($responsearray['tracking-number'])) {
        // Create an array of order numbers and tracking numbers to do DB updates with.
        $orders_update[] = array(
          'status' => $responsearray['status'],
          'ordernumber' => $value['ordernumber'],
          'trackingno' => $responsearray['tracking-number'],
        );
      }
    }
    foreach ($orders_update as $items) {
      db_update('corporate_disk_company_shipments')
          ->fields(array(
            'tracking_number' => $items['trackingno'],
            'order_updated' => 1,
          ))
          ->condition('order_number', $items['ordernumber'], '=')
          ->execute();
    }
  }
  catch (Exception $e) {
    watchdog('corporate_disk_company', '<h2>General Fault</h2><br /><b>Code:</b> @exc', array('@exc' => $exc));
    return FALSE;
  }
  // We need to label orders that are First class that there is no tracking
  // @todo If Disk.com changes the names of shipping this code must be updated
  try {
    // $sql = 'SELECT * from {corporate_disk_company_shipments} where order_updated = \'0\' and shipping_service LIKE \'USPS First Class\'';
    $query = db_select('corporate_disk_company_shipments', 'c')
        ->fields('c')
        ->condition('order_updated', '0', '=')
        ->condition('shipping_service', 'USPS First Class', 'LIKE');
    $result = $query->execute();
    foreach ($result as $items) {
      db_update('corporate_disk_company_shipments')
          ->fields(array(
            'tracking_number' => 'Tracking number not supported',
            'order_updated' => 1,
          ))
          ->condition('order_number', $items->order_number, '=')
          ->execute();
    }
    return TRUE;
  }
  catch (Exception $e) {
    watchdog('corporate_disk_company', '<h2>General Fault</h2><br /><b>Code:</b> @exc', array('@exc' => $exc));
    return FALSE;
  }
}

/**
 * Cron function to retry failed transactions that came back as 404s.
 */
function corporate_disk_company_scrap_failures($caller = NULL) {
  if ($caller != 'cron' && $caller != 'testpage') {
    return FALSE;
  }
  $rows = array();
  $results = db_select('corporate_disk_company_shipments', 'c')
      ->fields('c')
      ->condition('confirmation', 'DISK-RETRY', 'LIKE')
      ->execute();
  $count = $results->rowCount();
  if ($count < 1) {
    //Stop we have nothing to do
    return TRUE;
  }
  else {
    foreach ($results as $items) {
      $options = array(
        CURLOPT_RETURNTRANSFER => TRUE, // return web page
        CURLOPT_HEADER => FALSE, // don't return headers
        CURLOPT_FOLLOWLOCATION => TRUE, // follow redirects
        CURLOPT_MAXREDIRS => 10, // stop after 10 redirects
        CURLOPT_CONNECTTIMEOUT => 120, // time-out on connect
        CURLOPT_TIMEOUT => 120, // time-out on response
        CURLOPT_SSL_VERIFYHOST => FALSE, // Don't verify host
        CURLOPT_SSL_VERIFYPEER => FALSE, // Don't verify peer
        CURLOPT_CUSTOMREQUEST => 'GET', // HTTP request method
      );
      $ch = curl_init($items->service_post);
      curl_setopt_array($ch, $options);
      $content = curl_exec($ch);
      if (curl_errno($ch)) {
        watchdog('corporate_disk_company', '<h2>cURL Fault - Order Retry Subroutine</h2><br /><b>cURL Error Code:</b> @exc', array('@exc' => curl_errno($ch)));
      }
      else {
        $resarr = array();
        $resarr = json_decode($content);
        $confirmation = trim(filter_xss($resarr));
        $queue = db_update('corporate_disk_company_shipments')
            ->fields(array(
              'confirmation' => $confirmation,
            ))
            ->condition('order_number', $items->order_number, '=')
            ->execute();
      }
      curl_close($ch);
      return TRUE;
    }
  }
}

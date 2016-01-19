<?php
/**
 * @file
 * UTMP Javascript: method and functions.
 */

/**
 * GA Push Method callback: UTMP (js).
 */
function ga_push_method_utmp_js($push, $type, $options) {
  $push_info = array(
    'push' => $push,
    'type' => $type,
  );
  $_SESSION['ga_push_utmp'][] = $push_info;
}

/**
 * Send the ga push to JS on page load using Universal Analytics Event Tracking.
 */
function ga_push_method_utmp_js_push() {
  if (isset($_SESSION['ga_push_utmp'])) {
    $id = variable_get('googleanalytics_account', '');
    if (!empty($id)) {
      $script = "var ga = ga || [];\n";

      // Process each push data that needs to be sent to Google Analytics.
      foreach ($_SESSION['ga_push_utmp'] as $queued) {
        $push = $queued['push'];
        $type = $queued['type'];

        switch ($type) {
          case GA_PUSH_TYPE_EVENT:
            $script .= _ga_push_method_utmp_js_push_event_script($push);
            break;

          case GA_PUSH_TYPE_ECOMMERCE:
            $script .= _ga_push_method_utmp_js_push_ecommerce_script($push);
            break;

          case GA_PUSH_TYPE_EXCEPTION:
            $script .= _ga_push_method_utmp_js_push_exception_script($push);
            break;

          case GA_PUSH_TYPE_PAGEVIEW:
            $script .= _ga_push_method_utmp_js_push_send_script($push, 'pageview');
            break;

          case GA_PUSH_TYPE_SOCIAL:
            $script .= _ga_push_method_utmp_js_push_send_script($push, 'social');
            break;
        }
      }
      _ga_push_method_utmp_js_add_script($script);
      unset($_SESSION['ga_push_utmp']);
    }
  }
}


/**
 * Generates the ga JS code for pushing an event to GA.
 *
 * @param array $push
 *   Event data.
 *
 * @return string
 *   JS code with push code.
 */
function _ga_push_method_utmp_js_push_event_script($push) {

  // Optional: default values.
  $push += array(
    'label' => '',
    'value' => 1,
    'non-interaction' => FALSE,
  );

  $push_map = array(
    'category' => 'eventCategory',
    'action'   => 'eventAction',
    'label'    => 'eventLabel',
    'value'    => 'eventValue',
  );

  // Mapping.
  $mapped_push = ga_push_push_mapper($push_map, $push);

  // Convert to JS function.
  $script = _ga_push_method_utmp_js_push_send_script($mapped_push, 'event');

  return $script;
}

/**
 * Generates the ga JS code for pushing an ecommerce to GA.
 *
 * @param array $push
 *   Event data.
 *
 * @return string
 *   JS code with push code.
 */
function _ga_push_method_utmp_js_push_ecommerce_script($push) {

  // Include needed additional ga e-commerce library.
  $script = "ga('require', 'ecommerce', 'ecommerce.js');\n";

  // Transaction.
  $transaction_push_map = array(
    'order_id'       => 'id',
    'affiliation'    => 'affiliation',
    'total'          => 'revenue',
    'total_shipping' => 'shipping',
    'total_tax'      => 'tax',
    'currency'       => 'currency',
  );

  // Mapping.
  $transaction_mapped_push = ga_push_push_mapper($transaction_push_map, $push['trans']);

  // Build up the transaction.
  $script .= "ga('ecommerce:addTransaction', " . json_encode($transaction_mapped_push) . ");\n";

  // Items.
  $item_push_map = array(
    'order_id' => 'id',
    'name'     => 'name',
    'sku'      => 'sku',
    'category' => 'category',
    'price'    => 'price',
    'quantity' => 'quantity',
    'currency' => 'currency',
  );

  // Build up the items.
  foreach ($push['items'] as $value) {
    // Mapping.
    $item_mapped_push = ga_push_push_mapper($item_push_map, $value);
    $script .= "ga('ecommerce:addItem', " . json_encode($item_mapped_push) . ");\n";
  }

  // Sending!
  $script .= "ga('ecommerce:send');\n";

  return $script;
}

/**
 * Generates the ga JS code for pushing an exception to GA.
 *
 * @param array $push
 *   Event data.
 *
 * @return string
 *   JS code with push code.
 */
function _ga_push_method_utmp_js_push_exception_script($push) {
  // Remove empty parameters:
  $push = array_filter($push);

  // Convert to JS function.
  $script = "ga('send', 'exception', " . json_encode($push) . ");\n";

  return $script;
}

/**
 * Generates the ga send JS code.
 *
 * @param array $push
 *   Data.
 * @param string $hit_type
 *   The hit type: social, pageview...
 *
 * @return string
 *   JS code with push code.
 */
function _ga_push_method_utmp_js_push_send_script($push, $hit_type) {
  // Remove empty parameters:
  $push = array_filter($push);

  // Add the hitType: social.
  $push['hitType'] = $hit_type;

  // Convert to JS function.
  $script = "ga('send', " . json_encode($push) . ");\n";

  return $script;
}

/**
 * Adds generated script with push data.
 *
 * Used by ga_push_method_utmp_js_push.
 *
 * @see ga_push_method_utmp_js_push()
 *
 * @param string $script
 *   Generated script code to add.
 */
function _ga_push_method_utmp_js_add_script($script) {
  $options = array(
    'type' => 'inline',
    'scope' => 'footer',
    // 'weight' => 0,
    // 'group' => JS_DEFAULT,
  );
  drupal_add_js($script, $options);
}

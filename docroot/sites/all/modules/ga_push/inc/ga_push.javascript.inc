<?php
/**
 * @file
 * Classic GA Javascript: method and functions.
 */

/**
 * GA Push Method callback: Javascript Classic (javascript).
 */
function ga_push_method_javascript($push, $type, $options) {

  $push_info = array(
    'push' => $push,
    'type' => $type,
  );
  $_SESSION['ga_push'][] = $push_info;
}

/**
 * Send the ga push to JS on page load.
 *
 * @see https://developers.google.com/analytics/devguides/collection/gajs
 */
function ga_push_method_javascript_push() {
  if (isset($_SESSION['ga_push'])) {

    $id = variable_get('googleanalytics_account', '');

    if (!empty($id)) {
      $script = "var _gaq = _gaq || [];\n";

      foreach ($_SESSION['ga_push'] as $queued) {

        $push = $queued['push'];
        $type = $queued['type'];

        switch ($type) {

          case GA_PUSH_TYPE_EVENT:

            // Optional: default values:
            $push += array(
              'label' => '',
              'value' => 1,
              // 'non-interaction' => false,
            );

            $push_map = array(
              'category' => 'category',
              'action'   => 'action',
              'label'    => 'label',
              'value'    => 'value',
              'non-interaction' => 'non-interaction',
            );

            // Mapping.
            $mapped_push = ga_push_push_mapper($push_map, $push);

            // Add method.
            array_unshift($mapped_push, '_trackEvent');

            // Convert to JSON to avoid special chars problems that could broke syntax.
            $script_params = json_encode(array_values($mapped_push));

            // Convert to JS function.
            $script .= '_gaq.push(' . $script_params . ");\n";

            break;

          case GA_PUSH_TYPE_ECOMMERCE:

            // Transaction.
            $transaction_push_map = array(
              'order_id'       => 'order_id',
              'affiliation'    => 'affiliation',
              'total'          => 'total',
              'total_tax'      => 'total_tax',
              'total_shipping' => 'total_shipping',
              'city'           => 'city',
              'region'         => 'region',
              'country'        => 'country',
            );

            // Mapping and initialize empty values assure param order (so no
            // param is in place of another).
            $transaction_mapped_push = ga_push_push_mapper($transaction_push_map, $push['trans'], TRUE);

            $transaction_mapped_push = ga_push_clean_empty_array_tail($transaction_mapped_push);

            // Add method.
            array_unshift($transaction_mapped_push, '_addTrans');

            // Build up the transaction.
            $script .= "_gaq.push(" . json_encode(array_values($transaction_mapped_push)) . ");\n";

            // Items.
            $item_push_map = array(
              'order_id'       => 'order_id',
              'sku'      => 'sku',
              'name'     => 'name',
              'category' => 'category',
              'price'    => 'price',
              'quantity' => 'quantity',
            );

            // Build up the items.
            foreach ($push['items'] as $value) {
              // Mapping.
              $item_mapped_push = ga_push_push_mapper($item_push_map, $value, TRUE);
              $item_mapped_push = ga_push_clean_empty_array_tail($item_mapped_push);

              // Adding method.
              array_unshift($item_mapped_push, '_addItem');

              $script .= "_gaq.push(" . json_encode(array_values($item_mapped_push)) . ");\n";
            }

            // Currency: by default, you can configure a common, global, currency
            // for all transactions and items through the Google Analytics management
            // web interface. By default, the global currency is used for all items
            // and transactions.
            if (!empty($push['trans']['currency'])) {
              $script .= "_gaq.push(['_set', 'currencyCode', '"
                . $push['trans']['currency'] . "']);\n";
            }

            // Transaction - end:
            $script .= "_gaq.push(['_trackTrans']);\n ";

            break;
        }
      }

      unset($_SESSION['ga_push']);

      $options = array(
        'type' => 'inline',
        'scope' => 'footer',
        // 'weight' => 0,
        // 'group' => JS_DEFAULT,
      );
      drupal_add_js($script, $options);
    }
  }
}

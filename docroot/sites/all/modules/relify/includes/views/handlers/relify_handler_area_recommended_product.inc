<?php

/**
 * Recommends a product based on a line item argument.
 */
class relify_handler_area_recommended_product extends views_handler_area {
  function option_definition() {
    $options = parent::option_definition();

    // Undefine the empty option.
    unset($options['empty']);

    // Define an option to select a recommender.
    $recommenders = relify_recommender_load_multiple(relify_client_id());
    $options['recommender_id'] = array('default' => key($recommenders));

    return $options;
  }

  function options_form(&$form, &$form_state) {
    parent::options_form($form, $form_state);

    // Don't display a form element for the undefined empty option.
    unset($form['empty']);

    $options = relify_recommender_load_multiple(relify_client_id());

    foreach ($options as $key => &$value) {
      $value = check_plain($value['name'] . ': ' . $value['recommender_id']);
    }

    $form['recommender_id'] = array(
      '#type' => 'radios',
      '#title' => t('Order identifier used for redirection'),
      '#options' => $options,
      '#default_value' => $this->options['recommender_id'],
    );
  }

  function render($empty = FALSE) {
    foreach ($this->view->argument as $type => $argument) {
      if ($type == 'line_item_id') {
        $line_item_id = reset($argument->value);
        $line_item = commerce_line_item_load($line_item_id);

        // If this isn't a product line item, bail now.
        if (!in_array($line_item->type, commerce_product_line_item_types())) {
          return;
        }

        // Find the most similar item to the one
        $wrapper = entity_metadata_wrapper('commerce_line_item', $line_item);
        $recommender = relify_recommender_load($this->options['recommender_id']);
        $response = relify_request_similar_items($recommender['recommender_id'], $wrapper->commerce_product->raw(), 1);

        $data = drupal_json_decode($response->data);
        $item_identifier = $recommender['item_identifier'];

        // If we didn't get a recommendation...
        if (empty($data[0][$item_identifier])) {
          // Pick a product at random.
          $data[0][$item_identifier] = db_query('SELECT product_id FROM {commerce_product} WHERE status = 1 ORDER BY RAND() LIMIT 1')->fetchField();

          $message = t("Check this out while you're at it:");
        }
        else {
          $message = t("We think you'll love this:");
        }

        // Load the recommended product.
        $product = commerce_product_load($data[0][$item_identifier]);

        // Load its first found product display.
        $query = new EntityFieldQuery();
        $query
          ->entityCondition('entity_type', 'node', '=')
          ->propertyCondition('status', 1, '=')
          ->fieldCondition('field_product', 'product_id', $product->product_id, '=');

        $response = $query->execute();

        if (!empty($response['node'])) {
          $node = node_load(key($response['node']));
          $uri = entity_uri('node', $node);

          return $message . '<br />' . l($node->title, $uri['path']);
        }
      }
    }
  }
}

<?php

/**
 * @file
 * User page callbacks for commerce_product_comparison module.
 */

/**
 * Page callback, display comparison table.
 *
 * @return string
 *   Return html of the comparison page.
 */
function commerce_product_comparison_compare_page() {
  drupal_add_css(drupal_get_path('module', 'commerce_product_comparison') . '/css/commerce_product_comparison.css');
  drupal_set_title(t('Comparison'));

  // Load current compare items.
  $list = commerce_product_comparison_list();

  // Collect header.
  $header = array('');

  // Make add to cart row.
  $add_to_cart_row = array('');

  // Creates the header of the comparison table with themable cells.
  foreach ($list as $item) {
    ctools_include('ajax');
    ctools_add_js('ajax-responder');
    $product = commerce_product_load($item['entity_id']);
    $header[] = theme('commerce_product_comparison_header_table', array(
      'commerce_product' => $product,
      'commerce_product_title' => check_plain($product->title),
      'delete_link' => ctools_ajax_text_button('', "ctools_ajax_compare/" . $item['entity_id'] . "/nojs", t('Delete this product form the compare list'), 'delete-from-comparison'),
    ));

    // Prepare the last row with the add to cart button in it.
    $add_to_cart = drupal_get_form('commerce_product_comparison_add_product_to_cart_form_' . $product->product_id, $product->product_id);
    $add_to_cart_row[] = array(
      'data' => array(
        array(
          'data' => $add_to_cart,
        ),
      ),
    );
  }
  $rows = array();

  // Creates an array of all attributes of the products in comparison.
  $shared_attributes = array();
  foreach (field_info_instances('commerce_product') as $product_type) {
    foreach ($product_type as $field) {
      if (empty($shared_attributes[$field['field_name']])) {
        $shared_attributes[$field['field_name']] = array(
          'label' => $field['label'],
          'html' => array(),
          'count' => 0,
        );
      }
    }
  }
  $saved_fields = variable_get('commerce_product_comparison_fields', array());
  foreach ($list as $commerce_product) {
    $wrapped_product = entity_metadata_wrapper('commerce_product', commerce_product_load($commerce_product['entity_id']));
    foreach ($shared_attributes as $field_name => $values) {
      $attribute = field_read_instance('commerce_product', $field_name, $wrapped_product->getBundle());
      if (!empty($attribute['display']['compare_page']['type']) && $attribute['display']['compare_page']['type'] != 'hidden') {
        // If the attribute is displayed in the view mode then we set its value.
        // Ensures property is attached on wrapped entity.
        // Special treatment if commerce_price
        if ($field_name == 'commerce_price') {
          $commerce_price = $wrapped_product->$field_name->value();
          // Set html value for this attribute.
          $shared_attributes[$field_name]['html'][] = commerce_currency_format($commerce_price['amount'], $commerce_price['currency_code']);
          // If we have a price then we count++.
          if (!empty($commerce_price)) {
            $shared_attributes[$field_name]['count'] = $shared_attributes[$field_name]['count'] + 1;
          }
        }
        else {
          $field_view = field_view_field('commerce_product', $wrapped_product->raw(), $field_name, 'compare_page');
          $html = drupal_render($field_view);
          if (in_array($field_name, $saved_fields)) {
            if ($shared_attributes[$field_name]['count'] == 0 && $shared_attributes[$saved_fields[$field_name]]['count'] > 0) {
              $field_name = $saved_fields[$field_name];
            }
          }
          // If we have html then we count++.
          if (!empty($html)) {
            $shared_attributes[$field_name]['count'] = $shared_attributes[$field_name]['count'] + 1;
          }
          // If not we set an empty cell.
          else {
            $html = '';
          }
          $shared_attributes[$field_name]['html'][] = $html;
        }
      }
      // If the attribute is not display,
      // set an empty html field without counting it.
      // Same thing if the current product doesn't have this attributes.
      elseif (!(empty($attribute) && !empty($saved_fields[$field_name]) && $attribute = field_read_instance('commerce_product', $saved_fields[$field_name], $wrapped_product->getBundle()))) {
        $shared_attributes[$field_name]['html'][] = '';
      }
    }
  }
  // Build the table now know which attributes are shared between the products.
  foreach ($shared_attributes as $delta => $shared_attribute) {
    // Delete attributes from the table if not shared by at least two product.
    // This information is set in the 'count' entry of our array.
    if ($shared_attribute['count'] < 2) {
      unset($shared_attributes[$delta]);
    }
    // Set table row for this attribute.
    else {
      // First cell is the attribute label.
      $row = array(array('data' => check_plain($shared_attribute['label'])));
      foreach ($shared_attribute['html'] as $html) {
        if (isset($html)) {
          // If attribute has a value we set it in the row.
          $row[] = $html;
        }
        else {
          // Else we set a blank cell.
          $row[] = '&nbsp;';
        }
      }
      // Add the row to the table.
      $rows[] = array(
        'data' => $row,
        'id' => 'compare_' . $delta,
      );
    }
  }
  // If there is no shared attributes we return Nothing to compare.
  if (empty($shared_attributes)) {
    return t("Products don't have attributes to compare.");
  }
  // Add a row with add to cart form.
  $rows[] = $add_to_cart_row;

  // Add filters.
  $filter_form = drupal_get_form('commerce_product_comparison_filter_form', $shared_attributes);

  $compare_page = array();
  // Put the all markup into a div with a specific id.
  $compare_page['compare_page_begin'] = array(
    '#markup' => '<div id ="properties-compare-table">',
  );
  // Add filters if they are enabled.
  if (variable_get('commerce_product_comparison_filter_enabled', TRUE)) {
    $compare_page['filters'] = array(
      '#type' => 'markup',
      '#markup' => render($filter_form),
    );
  }
  $compare_page['compare_table'] = array(
    '#theme' => 'table',
    '#header' => $header,
    '#rows' => $rows,
    '#attributes' => array('id' => 'properties-compare-table'),
  );
  $compare_page['compare_page_end'] = array(
    '#markup' => '</div>',
  );

  // Return the html table using theme table.
  return $compare_page;
}

/**
 * Page calllback to remove an entity from the list.
 *
 * @param string $entity_type
 *   The entity type of the product to delete.
 * @param string $entity_id
 *   The entity id of the product to delete.
 * @param string $ajax
 *   Whether we are using ajax or not.
 */
function commerce_product_comparison_compare_delete($entity_type, $entity_id, $ajax = NULL) {
  commerce_product_comparison_list_delete($entity_type, $entity_id);
  if (empty($ajax)) {
    drupal_goto();
  }
}

/**
 * Refresh the compare table on properties/compare when deleting a product.
 *
 * @param int $product_id
 *   The product_id of the product to delete from the compare list
 * @param string $js
 *   Whether the client has javascript activated or not.
 *
 * @return string
 *   The HTML markup of the compare table.
 */
function commerce_product_comparison_refresh_properties_compare_table($product_id, $js = NULL) {
  // Delete the product from the compare list.
  commerce_product_comparison_compare_delete('commerce_product', $product_id, TRUE);
  // Reload the compare table to refresh it in ajax by sending new html.
  $new_table = commerce_product_comparison_compare_page();
  if ($js) {
    $commands = array();
    // If there is only one product in comparison we redirect the user to
    // the page he was on before.
    if (count(commerce_product_comparison_list()) == 1) {
      ctools_include('ajax');
      ctools_include('cache');
      $commands[] = ctools_ajax_command_redirect(ctools_cache_get('simple::wizard::page', _commerce_product_comparison_get_unique_key()));
    }
    else {
      // We refresh the compare table.
      $commands[] = ajax_command_replace('#properties-compare-table', render($new_table));
    }

    print ajax_render($commands);
    exit;
  }
  else {
    return $new_table;
  }
}

/**
 * Implements filter letting the user choose what to show in the compare table.
 */
function commerce_product_comparison_filter_form($form, $form_state, $shared_attributes) {
  $filters = array();
  foreach ($shared_attributes as $key => $value) {
    $filters[$key] = $value['label'];
  }
  $form['commerce_product_comparison_filters'] = array(
    '#type' => 'checkboxes',
    '#title' => t('Filters'),
    '#options' => $filters,
    '#default_value' => array_keys($filters),
    '#ajax' => array(
      'callback' => 'commerce_product_comparison_filter_ajax_callback',
      'wrapper' => 'properties-compare-table',
    ),
  );
  return $form;
}

/**
 * Ajax callback filtering the compare table rows.
 *
 * @param array $form
 *   Nested array of form elements that comprise the form.
 * @param array $form_state
 *   A keyed array containing the current state of the form.
 *
 * @return array
 *   Return an array containing ajax commands.
 */
function commerce_product_comparison_filter_ajax_callback($form, &$form_state) {
  $commands = array();
  foreach ($form_state['values']['commerce_product_comparison_filters'] as $key => $value) {
    $selector = '#compare_' . $key;
    // If the checkbox is unchecked then hide the row.
    if (!($value === $key)) {
      $commands[] = ajax_command_css($selector, array('display' => 'none'));
    }
    // If the checkbox is checked then set the display to its default value
    // by removing it.
    else {
      $commands[] = ajax_command_css($selector, array('display' => ''));
    }
  }
  return array('#type' => 'ajax', '#commands' => $commands);
}

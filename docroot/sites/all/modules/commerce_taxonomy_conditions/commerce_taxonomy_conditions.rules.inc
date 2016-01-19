<?php

/**
 * @file
 * Rules integration for taxonomy conditions.
 */

/**
 * Handles the commerce order conditions.
 * 
 * Handles the commerce order conditions by checking the order for certain 
 * taxomomy terms.
 * 
 * @param object $order 
 *   Commerce order object
 * @param string $field_name 
 *   the name of the taxonomy vocabularies
 * @param int $term_id 
 *   the taxonomy term id
 * @param string $operator 
 *   '<', '<=', '=', '>=','>'
 * @param int $value 
 *   the value that is given for the $operator to use to compare with
 * 
 * @return bool
 *   returns TRUE if value is positive to descripted rule, 
 *   FALSE if there is no match to set properties
 */
function commerce_taxonomy_conditions_rules_contains_product($order, $field_name, $term_ids, $operator, $value) {
  $order_wrapper = entity_metadata_wrapper('commerce_order', $order);

  if (!is_array($term_ids)) {
    $term_ids = array($term_ids => $term_ids);
  }
  $term_key = implode(',', $term_ids);
  $products = array($term_key => 0);

  // If we actually received a valid order.
  if (!empty($order) && !empty($field_name) && !empty($term_ids)) {

    // Run through the order to check on the taxonomy term that was given.
    foreach ($order_wrapper->commerce_line_items as $delta => $line_item_wrapper) {
      $line_item = commerce_line_item_load($line_item_wrapper->line_item_id->value());

      if (!empty($line_item_wrapper->commerce_product)) {
        // Check if the vocabulary exists, otherwise we can skip this part.
        if (!isset($line_item_wrapper->commerce_product->$field_name)) {
          $node = node_load($line_item->data['context']['entity']['entity_id']);
          if (isset($node->$field_name)) {
            $product_terms = commerce_taxonomy_conditions_node_terms_to_array($node->$field_name);
          } else {
            drupal_set_message(t("The vocabulary %field_name wasn't found in the commerce product or the node linked to the product, maybe the vocabulary is not attached to the node or the commerce product?", array('%field_name' => $field_name)), 'error');
          }
        } else {
          $obj_terms = $line_item_wrapper->commerce_product->$field_name->value();
          if (!is_array($obj_terms)) {
            $obj_terms = array($obj_terms);
          }

          $product_terms = array();
          foreach ($obj_terms as $obj_term) {
            $product_terms[$obj_term->tid] = $obj_term->tid;
          }
        }
        // Check if there are terms and if the arrays got matching keys.
        if (!empty($product_terms) && commerce_taxonomy_conditions_arrays_got_match($product_terms, $term_ids)) {
          // If taxonomy was found we need to add the quantity.
          $quantity = $line_item_wrapper->quantity->value();
          if (empty($products[$term_key])) {
            $products[$term_key] = $quantity;
          } else {
            $products[$term_key] += $quantity;
          }
        }
      }
    }
  }

  // Make a quantity comparison based on the operator.
  switch ($operator) {
    case '<':
      return $products[$term_key] < $value;
      break;
    case '<=':
      return $products[$term_key] <= $value;
      break;
    case '=':
      return $products[$term_key] == $value;
      break;
    case '>=':
      return $products[$term_key] >= $value;
      break;
    case '>':
      return $products[$term_key] > $value;
      break;
  }

  return FALSE;
}

/**
 * Check if an 2 arrays contains the same key.
 * 
 * @param array $array1
 *   The first array to match
 * @param array $array2
 *   The second array to match with the first array
 * 
 * @return boolean 
 *   TRUE if atleast one same key is found, FALSE if no key matched
 */
function commerce_taxonomy_conditions_arrays_got_match($array1, $array2) {
  foreach ($array1 as $key => $val) {
    if (!empty($array2[$key])) {
      return TRUE;
    }
  }

  return FALSE;
}

/**
 * Convert a node term list to a array
 * 
 * @param array $node_terms
 *   Multilayer array of taxonomy as in $node object
 * 
 * @return array
 *   Returns a simple array so it can be compared
 */
function commerce_taxonomy_conditions_node_terms_to_array($node_terms) {
  $term_list = array();
  foreach ($node_terms as $terms) {
    foreach ($terms as $term) {
      $term_list[$term['tid']] = $term['tid'];
    }
  }
  return $term_list;
}

/**
 * Get taxonomy vocabularies.
 * 
 * @return array
 *   Options list of available vocabs
 */
function commerce_taxonomy_conditions_term_list() {
  $field_options_list = array();
  $fields = field_read_fields(array('type' => 'taxonomy_term_reference'));
  if (!empty($fields)) {
    foreach ($fields as $key => $value) {
      $field_options_list[$key] = $key;
    }
  }
  return $field_options_list;
}

/**
 * Get taxonomy term list based on the given vocabulary name.
 * 
 * @param string $field_name
 *   The name of the taxonomy vocab as field name
 * 
 * @return array
 *   Returns a list of available terms of the chosen vocab.
 */
function commerce_taxonomy_conditions_term_options_list($field_name = NULL) {
  $term_list = array('' => t('Choose a term'));
  if (!empty($field_name)) {
    $field = field_read_field($field_name);
    $vocabulary = taxonomy_vocabulary_machine_name_load($field['settings']['allowed_values'][0]['vocabulary']);
    if (!empty($vocabulary)) {
      $terms = taxonomy_get_tree($vocabulary->vid);

      if (!empty($terms)) {
        foreach ($terms as $term) {
          $term_list[$term->tid] = str_repeat('-', $term->depth) . $term->name;
        }
      }
    }
  }
  return $term_list;
}

/**
 * Function for the ajax call to retrieve the list taxonomy terms.
 * 
 * @param array $form
 *   standard drupal form array
 * @param array $form_state
 *   standard drupal form_state
 * 
 * @return array
 *   Returns form field select box with options based on the chosen vocab.
 */
function commerce_taxonomy_conditions_term_options_list_form($form, $form_state) {
  $terms = commerce_taxonomy_conditions_term_options_list($form_state['values']['parameter']['term_field_names']['settings']['term_field_names']);
  $form['parameter']['term_ids']['settings']['term_ids']['#options'] = $terms;
  return $form['parameter']['term_ids']['settings']['term_ids'];
}

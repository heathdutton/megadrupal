<?php

/**
 * Implements hook_rules_condition_info().
 */
function commerce_discount_product_category_rules_condition_info() {
  $inline_conditions = inline_conditions_get_info();
  $conditions = array();

  $conditions['commerce_discount_product_category_has_specified_terms'] = array(
    'label' => t('Line item product display contains specific terms ID'),
    'parameter' => array(
      'commerce_line_item' => array(
        'type' => 'commerce_line_item',
        'label' => t('Line item'),
        'description' => t('The line item.'),
        'wrapped' => TRUE,
      ),
      'terms' => array(
        'type' => 'text',
        'label' => t('Terms ID'),
        'description' => t('Enter a comma-separated list of term ID to compare against the passed product line item.'),
      ),
    ),
    'module' => 'commerce_discount_product_category',
    'group' => t('Commerce Line Item'),
    'callbacks' => array(
      'execute' => $inline_conditions['commerce_discount_product_category_has_specified_terms']['callbacks']['build'],
    ),
  );
  $conditions['commerce_discount_product_category_order_has_specified_terms'] = array(
    'label' => t('Order any line item product display contains specific terms ID'),
    'parameter' => array(
      'commerce_order' => array(
        'type' => 'commerce_order',
        'label' => t('Order'),
        'description' => t('The order.'),
        'wrapped' => TRUE,
      ),
      'terms' => array(
        'type' => 'text',
        'label' => t('Terms ID'),
        'description' => t('Enter a comma-separated list of term ID to compare against the passed product line item.'),
      ),
    ),
    'module' => 'commerce_discount_product_category',
    'group' => t('Commerce Order'),
    'callbacks' => array(
      'execute' => $inline_conditions['commerce_discount_product_category_order_has_specified_terms']['callbacks']['build'],
    ),
  );
  return $conditions;
}

/**
 * Build callback for commerce_product_has_specified_terms on product.
 *
 * @param EntityDrupalWrapper $wrapper
 *   Wrapped entity type given by the rule.
 * @param array $terms
 *   Values for the condition settings.
 *
 * @return bool
 *   True is condition is valid. false otherwise.
 */
function commerce_discount_product_category_has_specified_terms_build(EntityDrupalWrapper $wrapper, $terms) {
  $terms_to_check = explode(', ', $terms);

  // If commerce_product is attached.
  if (in_array('commerce_product', array_keys($wrapper->getPropertyInfo()))) {

    // Collect product reference fields from the display nodes, to query them.
    $product_reference_fields = array();
    foreach (field_info_field_map() as $field_name => $field_stub) {
      if ($field_stub['type'] == 'commerce_product_reference' && !empty($field_stub['bundles']['node'])) {

        // Not every product reference field signifies that this node is
        // 'a display node for this particular product'. We assume it is, if the
        // "Render fields from the referenced products when viewing this entity"
        // per-instance setting is true.
        $bundles = array();
        foreach ($field_stub['bundles']['node'] as $bundle) {
          $instance_info = field_info_instance('node', $field_name, $bundle);
          if (!empty($instance_info['settings']['field_injection'])) {
            $bundles[] = $bundle;
          }
        }
        if ($bundles) {
          // We should check a relationship through this field, only for
          // nodes of the collected bundles.
          $product_reference_fields[$field_name] = $bundles;
        }
      }
    }

    foreach ($product_reference_fields as $field_name => $bundles) {
      $query = new EntityFieldQuery();
      $result = $query->entityCondition('entity_type', 'node')
        ->entityCondition('bundle', $bundles, 'IN')
        ->propertyCondition('status', 1)
        ->fieldCondition($field_name, 'product_id', $wrapper->commerce_product->getIdentifier())
        ->execute();

      if (isset($result['node'])) {
        foreach (array_keys($result['node']) as $nid) {
          $node = node_load($nid);
          $node_display_wrapper = entity_metadata_wrapper('node', $node);

          foreach ($node_display_wrapper->getPropertyInfo() as $node_field_name => $node_property) {

            // Check if the property is of a taxonomy term type.
            if (isset($node_property['type']) &&
              ($node_property['type'] == 'taxonomy_term' || $node_property['type'] == 'list<taxonomy_term>')) {

              // If the wrapped field is an instance of EntityListWrapper class, that
              // means that field contains multiple values.
              if ($node_display_wrapper->$node_field_name instanceof EntityListWrapper) {
                foreach ($node_display_wrapper->$node_field_name as $wrapper_term) {
                  // We have a match, apply discount
                  if (($key = array_search($wrapper_term->getIdentifier(), $terms_to_check)) !== FALSE) {
                    return TRUE;
                  }
                }
              }
              elseif ($term = $node_display_wrapper->$node_field_name->value()) {
                // The single value matches, return true.
                if (($key = array_search($term->tid, $terms_to_check)) !== FALSE) {
                  return TRUE;
                }
              }
            }
          }
        }
      }
    }
  }
  // Return false by default.
  return FALSE;
}

/**
 * Build callback for commerce_product_has_specified_terms on product.
 *
 * @param EntityDrupalWrapper $wrapper
 *   Wrapped entity type given by the rule.
 * @param array $terms
 *   Values for the condition settings.
 *
 * @return bool
 *   True is condition is valid. false otherwise.
 */
function commerce_discount_product_category_order_has_specified_terms_build(EntityDrupalWrapper $wrapper, $terms) {
  // Loop through each line item.
  try {
    foreach ($wrapper->{'commerce_line_items'} as $delta => $line_item_wrapper) {
      // Skip anything that doesn't have a product reference.
      if (!in_array('commerce_product', array_keys($line_item_wrapper->getPropertyInfo()))) {
        continue;
      }

      // Call our main line item condition function.
      if (commerce_discount_product_category_has_specified_terms_build($line_item_wrapper, $terms)) {
        // If this came back true, break out. No need to iterate.
        return true;
      }
    }
  }
  catch (EntityMetadataWrapperException $e) {
    return false;
  }

  return false;
}

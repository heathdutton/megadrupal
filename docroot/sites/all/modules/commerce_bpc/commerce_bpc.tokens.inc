<?php
/**
 * @file
 *   Token API implementations for Commerce bulk product creation module.
 */
/**
 * Implements hook_token_info().
 */
function commerce_bpc_token_info() {
  $token_type = array(
    'name' => t('Bulk Product'),
    'description' => t('Tokens related to bulk products.'),
    'needs-data' => 'bulk_data',
  );
  $default_token_type = array(
    'name' => t('Bulk Product default patterns'),
    'description' => t('Tokens related to bulk product creation default patterns'),
    'needs-data' => 'bulk_data',
  );

  $bulk_product = array();
  foreach (commerce_product_types() as $bundle_name => $info) {
    if (commerce_bpc_valid_product_type($bundle_name)) {
      $bulk_product += commerce_bpc_token_info_by_product_type($bundle_name);
    }
  }
  $default_tokens = array(
    'entered_sku' => array(
      'name' => 'SKU fragment input',
      'description' => 'The SKU fragment entered by the user on the bulk creation form.',
    ),
    'combination_values' => array(
      'name' => 'Combination values',
      'description' => 'The values of the the combination field, joined by the specified separator.',
    ),
    'entered_title' => array(
      'name' => 'Title fragment input',
      'description' => 'The title fragment entered by the user on the bulk creation form.',
    ),
    'combination_labels' => array(
      'name' => 'Combination value labels',
      'description' => 'The labels of the values of the the combination field, joined by the specified separator.',
    ),
  );
  return array(
    'types' => array(
      'bulk_product' => $token_type,
      'bulk_defaults' => $default_token_type,
    ),
    'tokens' => array(
      'bulk_product' => $bulk_product,
      'bulk_defaults' => $default_tokens,
    ),
  );
}

/**
 * Implements hook_tokens().
 */
function commerce_bpc_tokens($type, $tokens, array $data = array(), array $options = array()) {
  $sanitize = !empty($options['sanitize']);

  $replacements = array();
  if ($type == 'bulk_product' && !empty($data['bulk_data'])) {
    $bulk_data = $data['bulk_data'];
    $field_vals = commerce_bpc_tokens_by_product_type($bulk_data['product_type'], $bulk_data['combination'], $options);
    foreach ($field_vals['values'] as $field_name => $value) {
      if (isset($tokens[$field_name . '-value'])) {
        $replacements[$tokens[$field_name . '-value']] = $value;
      }
    }
    foreach ($field_vals['labels'] as $field_name => $label) {
      if (isset($tokens[$field_name . '-label'])) {
        $replacements[$tokens[$field_name . '-label']] = $label;
      }
    }
  }
  if ($type == 'bulk_defaults' && !empty($data['bulk_data'])) {
    $product_type = $data['bulk_data']['product_type'];

    // Get the names of all tokens for the current product type.
    if (isset($data['bulk_data']['combination'])) {
      // Collect tokens.
      $field_replacements = commerce_bpc_tokens_by_product_type($product_type, $data['bulk_data']['combination'], $options);
      if (!empty($field_replacements)) {
        if (!empty($tokens['combination_values'])) {
          $values = $field_replacements['values'];
          $separator = commerce_bpc_setting('default', 'sku_separator', $product_type);
          $replacements[$tokens['combination_values']] = implode($separator, $values);
        }
        if (!empty($tokens['combination_labels'])) {
          $labels = $field_replacements['labels'];
          $separator = commerce_bpc_setting('default', 'title_separator', $product_type);
          $replacements[$tokens['combination_labels']] = implode($separator, $labels);
        }
      }
    }
    if (!empty($data['bulk_data']['sku_fragment']) && !empty($tokens['entered_sku'])) {

      $replacements[$tokens['entered_sku']] = $data['bulk_data']['sku_fragment'];
    }
    if (!empty($data['bulk_data']['title_fragment']) && !empty($tokens['entered_title'])) {
      $replacements[$tokens['entered_title']] = $data['bulk_data']['title_fragment'];
    }
  }
  return $replacements;
}
/**
 * Collects field-specific tokens for a given product type.
 *
 * @param string $product_type
 *   The product type for which tokens should be returned.
 *
 * @return array
 *   An array of token definitions.
 */
function commerce_bpc_token_info_by_product_type($product_type) {
  $tokens = array();
  $instances = field_info_instances('commerce_product', $product_type);
  foreach ($instances as $instance) {
    $field_name = $instance['field_name'];
    if (commerce_bpc_is_combination_field($instance)) {
      $samples = module_invoke_all('commerce_bpc_token_sample_values', $instance);
      $sample_value = reset($samples['value']);

      $tokens[$field_name . '-value'] = array(
        'name' => $instance['label'] . ' value',
        'description' => t('The internal value of the @field_label field', array(
          '@field_label' => $instance['label'],
        )),
      );
      $tokens[$field_name . '-label'] = array(
        'name' => $instance['label'] . ' label',
        'description' => t('The human-readable name of the value of the @field_label field, e.g. %value', array(
          '@field_label' => $instance['label'],
          '%value' => $samples['label'],
        )),
      );
    }
  }
  return $tokens;
}
/**
 * Collects sample values of tokens for a given product type.
 *
 * @param string $product_type
 *   The product type for whose fields sample values should be returned.
 *
 * @return array
 *   An array with entries of the form 'token_name' => 'sample_value'
 */
function commerce_bpc_token_sample_values($product_type) {
  $samples = array();
  $instances = field_info_instances('commerce_product', $product_type);
  foreach ($instances as $instance) {
    if (commerce_bpc_is_combination_field($instance)) {
      $sample = module_invoke_all('commerce_bpc_token_sample_values', $instance);
      $samples['values'][$instance['field_name']] = $sample['value'];
      if (isset($sample['label'])) {
        $samples['labels'][$instance['field_name']] = $sample['label'];
      }
    }
  }
  return $samples;

}
/**
 * Collects field-specific token replacement for a given product type.
 *
 * @param string $product_type
 *   The product type for which tokens should be requested.
 *
 * @return array
 *   An array of token definitions.
 */
function commerce_bpc_tokens_by_product_type($product_type, $combination, $options) {
  $replacements = array();
  return $replacements += module_invoke_all('commerce_bpc_tokens', $product_type, $combination, $options);
}

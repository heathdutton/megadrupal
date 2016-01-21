<?php
/**
 * @file
 * Provides implementation of inline_conditions.
 */

/**
 * Implements hook_inline_conditions_info().
 */
function commerce_discount_product_category_inline_conditions_info() {
  $conditions = array();

  $conditions['commerce_discount_product_category_has_specified_terms'] = array(
    'label' => t('Product category'),
    'entity type' => 'commerce_line_item',
    'callbacks' => array(
      'configure' => 'commerce_discount_product_category_has_specified_terms_configure',
      'build' => 'commerce_discount_product_category_has_specified_terms_build',
    ),
  );
  $conditions['commerce_discount_product_category_order_has_specified_terms'] = array(
    'label' => t('Product category'),
    'entity type' => 'commerce_order',
    'callbacks' => array(
      'configure' => 'commerce_discount_product_category_order_has_specified_terms_configure',
      'build' => 'commerce_discount_product_category_order_has_specified_terms_build',
    ),
  );

  return $conditions;
}

/**
 * Implements hook_inline_conditions_build_alter().
 */
function commerce_discount_product_category_inline_conditions_build_alter(&$value) {
  switch ($value['condition_name']) {
    case 'commerce_discount_product_category_has_specified_terms':
    case 'commerce_discount_product_category_order_has_specified_terms':
      $terms = $value['condition_settings']['terms'];
      $value['condition_settings']['terms'] = '';
      foreach ($terms as $term) {
        $value['condition_settings']['terms'] .= reset($term);
        if ($term !== end($terms)) {
          $value['condition_settings']['terms'] .= ', ';
        }
      }
      break;
  }
}

/**
 * Configuration callback for commerce_product_has_specified_terms on product.
 *
 * @param array $settings
 *   Values for the form element.
 *
 * @return array
 *   Return a form element.
 */
function commerce_discount_product_category_has_specified_terms_configure($settings) {
  $form = array();

  $form['terms'] = array(
    '#type' => 'textfield',
    '#title' => t('Terms'),
    '#title_display' => 'invisible',
    '#required' => TRUE,
    '#maxlength' => 4096,
    '#default_value' => commerce_discount_product_category_configuration_default_value($settings),
    '#autocomplete_path' => 'inline_conditions/autocomplete/taxonomy_term/0/0',
    '#element_validate' => array('_inline_conditions_autocomplete_validate'),
    '#suffix' => '<div class="condition-instructions">' . t('The discount is active if the product has the selected term(s).') . '</div>',
  );

  return $form;
}

/**
 * Configuration callback for commerce_discount_product_category_order_has_specified_terms on product.
 *
 * @param array $settings
 *   Values for the form element.
 *
 * @return array
 *   Return a form element.
 */
function commerce_discount_product_category_order_has_specified_terms_configure($settings) {
  $form = array();

  $form['terms'] = array(
    '#type' => 'textfield',
    '#title' => t('Terms'),
    '#title_display' => 'invisible',
    '#required' => TRUE,
    '#maxlength' => 4096,
    '#default_value' => commerce_discount_product_category_configuration_default_value($settings),
    '#autocomplete_path' => 'inline_conditions/autocomplete/taxonomy_term/0/0',
    '#element_validate' => array('_inline_conditions_autocomplete_validate'),
    '#suffix' => '<div class="condition-instructions">' . t('The discount is active if the order has products with the selected term(s).') . '</div>',
  );

  return $form;
}

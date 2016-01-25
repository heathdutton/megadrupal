<?php

/**
 * Implements template_preprocess_HOOK()
 */

/**
 * Preprocessor for Add to Cart style plugin
 */
function template_preprocess_commerce_views_display_view_add_to_cart_form(&$vars) {
//dpm(array_keys($vars), '$vars');

  $view = &$vars['view'];
  $style_options = $vars['options'];
  $add_to_cart_form = &$vars['add_to_cart_form'];
  $product_display_id = &$vars['product_display_id'];

  // render add to cart form
  $vars['add_to_cart_form_rendered'] = theme('commerce_views_display_add_to_cart_form', array(
    'form' => $add_to_cart_form,
    'product_display_id' => $product_display_id,
    'view' => $view,
    'options' => $style_options,
  ));

  // render fields
  $vars['product_display_field_elements_rendered'] = theme('commerce_views_display_fields', array(
    'field_elements' => $vars['fields'],
    'product_display_id' => $product_display_id,
    'view' => $view,
    'options' => $style_options,
  ));


  // build classes
  $classes_array = array(
    'commerce-views-display',
    'commerce-views-display-view-add-to-cart-form',
    $product_display_id
  );

  if (!empty($vars['classes_array'])) {
    $classes_array = array_merge($vars['classes_array'], $classes_array);
    $classes_array = array_merge(array_unique($classes_array));
  }

  $vars['classes_array'] = $classes_array;
  $vars['classes'] = implode(' ', $classes_array);


  // build field classes
  $vars['field_classes_array'] = array();
  $vars['field_classes'] = array();


}

/**
 * The add to cart form.
 *
 * @ingroup themeable
 */
function theme_commerce_views_display_add_to_cart_form($vars) {
  if (empty($vars['form'])) {
    return '';
  }

  return drupal_render($vars['form']);
}

/**
 * The field elements.
 *
 * @ingroup themeable
 */
function theme_commerce_views_display_fields($vars) {
  if (empty($vars['field_elements'])) {
    return '';
  }

  return drupal_render($vars['field_elements']);
}

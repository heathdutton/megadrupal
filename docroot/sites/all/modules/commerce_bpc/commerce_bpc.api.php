<?php

/**
 * @file
 * This file contains no working PHP code; it exists to provide additional
 * documentation for doxygen as well as to document hooks in the standard
 * Drupal manner.
 */

/**
 * Specify whether a field instance is used to create combinations.
 *
 * This hook is invoked to determine whether some other hooks should
 * be run on instance (e.g. the token hooks) and to determine whether
 * a product type is fit for bulk product creation (it needs to have
 * at least one combination-creating instance).
 *
 * @param array $instance
 *   Array holding instance information.
 *
 * @return bool
 *   TRUE if the implementing module manages combination creation
 *   through the instance.
 */
function hook_commerce_bpc_is_combination_field($instance) {
  $field = field_info_field($instance['field_name']);
  if ($field['module'] == 'list') {
    return !_commerce_bpc_get_value($instance, array('commerce_bpc', 'is_static'), FALSE);
  }
}

/**
 * Perform alterations on a field form element of the bulk product form.
 *
 * This hook is called during the generation of the bulk create from, once for
 * every field attached to the product type for which bulk products are being
 * created.
 *
 * This hook is mainly provided so that modules do not need to loop through
 * the whole form array to do their field-specific alterations. The whole
 * form and form_state are passed, as a module may want to move form elements
 * to another place in the form. In most cases, it will be a good idea to
 * either use the field type specific hook
 * hook_commerce_bpc_FIELD_TYPE_form_element_alter(), or the field type module
 * specific version hook_commerce_bpc_MODULE_NAME_form_element_alter().
 *
 * A module that enables a field type to be used in the creation of
 * combinations (such as list fields) should use this hook to move the form
 * element to the 'combinations' fieldset of $form and perform any
 * necessary alterations on the form fields.
 *
 * In general, modules that move form elements out of $form['static_values']
 * should keep a record of what they did (by convention, in
 * $form_state['commerce_bpc'][MODULE_NAME]), so they can act on these
 * items at form validation and submission.
 *
 * Care has to be taken, as this hook (and its more specific variants) can
 * be called multiple times in case the form is rebuilt (e.g. after AJAX
 * calls). Thus in recording which fields have been altered in form_state, the
 * implementation should make sure it does not record things twice.
 *
 * @param array $form
 *   Nested array of form elements that comprise the bulk creation form.
 * @param array $form_state
 *   A keyed array containing the current state of the form.
 * @param array $path
 *   An array of keys specifying where in the form the current element
 *   is to be found. This should be reset if the element is moved.
 *
 * @see hook_commerce_bpc_MODULE_NAME_form_element_alter()
 * @see hook_commerce_bpc_FIELD_TYPE_form_element_alter()
 */
function hook_commerce_bpc_form_element_alter(&$form, &$form_state, &$path) {
  $element = drupal_array_get_nested_value($form, $path);
  $lang = $element['#language'];
  $field_name = $element[$lang]['#field_name'];
  $instance = field_info_instance('commerce_product', $field_name, $form['product_type']['#value']);

  if (commerce_bpc_commerce_bpc_is_combination_field($instance)) {
    $element[$lang]['#type'] = 'checkboxes';

    // Get rid of 'none' option---user can just not pick any.
    unset($element[$lang]['#options']['_none']);

    // Move to comibinations-fieldset
    $form['combinations'][$field_name] = $element;
    drupal_array_set_nested_value($form, $path, NULL);

    // Change path to allow subsequent hooks operate on the form element.
    $path = array('combinations', $field_name);


    if (empty($form_state['commerce_bpc']['list']['combination_fields'])
    || !in_array($field_name, $form_state['commerce_bpc']['list']['combination_fields'])) {
      // Record what we have done. As this hook may be run multiple times
      // due to form rebuilds, we need to make sure that we record each field
      // only once.
      $form_state['commerce_bpc']['list']['combination_fields'][] = $field_name;
    }
  }
}

/**
 * Perform alterations on the field form element of a particular field type.
 *
 * Modules can implement
 * hook_commerce_bpc_FIELD_TYPE_form_element_alter() to act only on the
 * form elements of a a specific field type, rather than implementing
 * hook_commerce_bpc_form_element_alter() and checking the field type, or
 * using long switch statements to alter form elements of multiple field
 * types.
 *
 * @param array $form
 *   Nested array of form elements that comprise the bulk creation form.
 * @param array $form_state
 *   A keyed array containing the current state of the form.
 * @param array $path
 *   An array of keys specifying where in the form the current element
 *   is to be found. This should be reset if the element is moved.
 *
 * @see hook_commerce_bpc_form_element_alter()
 * @see hook_commerce_bpc_MODULE_NAME_form_element_alter()
 */
function hook_commerce_bpc_FIELD_TYPE_form_element_alter(&$form, &$form_state, &$path) {
  // Modification for form elements related to a particular field type go
  // here. For example, FIELD_TYPE is "list_integer" this code will run
  // only on fields of this type.
  $element = drupal_array_get_nested_value($form, $path);
  $lang = $element['#language'];
  $field_name = $element[$lang]['#field_name'];
  $instance = field_info_instance('commerce_product', $field_name, $form['product_type']['#value']);

  if (commerce_bpc_commerce_bpc_is_combination_field($instance)) {
    $element[$lang]['#type'] = 'checkboxes';

    // Get rid of 'none' option---user can just not pick any.
    unset($element[$lang]['#options']['_none']);

    // Move to comibinations-fieldset
    $form['combinations'][$field_name] = $element;
    drupal_array_set_nested_value($form, $path, NULL);

    // Change path to allow subsequent hooks operate on the form element.
    $path = array('combinations', $field_name);


    if (empty($form_state['commerce_bpc']['list']['combination_fields'])
    || !in_array($field_name, $form_state['commerce_bpc']['list']['combination_fields'])) {
      // Record what we have done. As this hook may be run multiple times
      // due to form rebuilds, we need to make sure that we record each field
      // only once.
      $form_state['commerce_bpc']['list']['combination_fields'][] = $field_name;
    }
  }
}

/**
 * Perform alterations on the field form elements provide by a field module.
 *
 * Modules can implement hook_commerce_bpc_MODULE_NAME_form_element_alter()
 * to act only on the form elements provided by field types defined by a
 * specific module, rather than implementing
 * hook_commerce_bpc_form_element_alter() and checking the field type, or
 * using long switch statements to alter form elements of multiple field
 * types.
 *
 * This hook is particularly useful in case the field-defining module
 * defines several similar field type which essentially are to be
 * treated identically during bulk creation (such as core's list.module),
 * which would require multiple identical implementations of
 * hook_commerce_bpc_FIELD_TYPE_form_element_alter().
 *
 * @param array $form
 *   Nested array of form elements that comprise the bulk creation form.
 * @param array $form_state
 *   A keyed array containing the current state of the form.
 * @param array $path
 *   An array of keys specifying where in the form the current element
 *   is to be found. This should be reset if the element is moved.
 *
 * @see hook_commerce_bpc_form_element_alter()
 * @see hook_commerce_bpc_FIELD_TYPE_form_element_alter()
 */
function hook_commerce_bpc_MODULE_NAME_form_element_alter(&$form, &$form_state, $path) {
  // Modification for form elements related to field types defined by a
  // specific module go here. For example, MODULE_NAME is "list" this code will
  // run only on fields of type "list_text", "list_integer", "list_float" and
  // "list_boolean".
  $element = drupal_array_get_nested_value($form, $path);
  $lang = $element['#language'];
  $field_name = $element[$lang]['#field_name'];
  $instance = field_info_instance('commerce_product', $field_name, $form['product_type']['#value']);

  if (commerce_bpc_commerce_bpc_is_combination_field($instance)) {
    $element[$lang]['#type'] = 'checkboxes';

    // Get rid of 'none' option---user can just not pick any.
    unset($element[$lang]['#options']['_none']);

    // Move to comibinations-fieldset.
    $form['combinations'][$field_name] = $element;
    drupal_array_set_nested_value($form, $path, NULL);

    // Change path to allow subsequent hooks operate on the form element.
    $path = array('combinations', $field_name);


    if (empty($form_state['commerce_bpc']['list']['combination_fields'])
    || !in_array($field_name, $form_state['commerce_bpc']['list']['combination_fields'])) {
      // Record what we have done. As this hook may be run multiple times
      // due to form rebuilds, we need to make sure that we record each field
      // only once.
      $form_state['commerce_bpc']['list']['combination_fields'][] = $field_name;
    }
  }
}

/**
 * Create combinations for fields that support it.
 *
 * Modules implementing this hook will typically have implemented
 * one of the hook_commerce_bpc_*_form_element_alter()-hooks to
 * prepare some of the form elements to be usable for the creation
 * of combinations.
 *
 * This hook is invoked on form validation and submission to figure
 * out for which combinations of values products should be created.
 *
 * @param array $form
 *   The Form API representation of the form.
 * @param array $form_state
 *   An array holding the current state of the form, in particular the
 *   submitted values.
 * @param array $combinations
 *   An array holding the combinations that have been created so far. Each
 *   combination is an array keyed by field names with full field api value
 *   arrays as values (i.e. a nested array that accommodates languages and
 *   multiple values). The hook should take this parameter by reference and
 *   alter it. In the usual case, if a field managed by the implementing module
 *   has values that indicate that n variants should be created, the module
 *   should replace every member of $combinations with n copies of that member,
 *   one for each variant of the module. The first hook implementation that will
 *   be called will have a value of array(array()) for this parameter, meaning
 *   there is only one (empty) combination.
 */
function hook_commerce_bpc_get_combinations($form, &$form_state, &$combinations) {
  $new_combinations = array();
  $fields = $form_state['commerce_bpc']['list']['combination_fields'];
  foreach ($fields as $field_name) {
    $new_combinations = array();
    $langs = array_keys($form_state['values']['combinations'][$field_name]);
    $lang = reset($langs);
    $values = $form_state['values']['combinations'][$field_name][$lang];

    foreach ($combinations as $combination) {
      foreach ($values as $columns) {
        if (!list_field_is_empty($columns, $field_name)) {
          $new_combinations[] = $combination + array(
            $field_name => array($lang => array($columns)),
          );
        }
        else {
          $new_combinations[] = $combination;
          // If the values for a field are empty, we make sure that
          // we (re)-add the original combination only once.
          break;
        }
      }
    }

    $combinations = $new_combinations;
  }
}

/**
 * Specify token replacement values for combinations.
 *
 * Combination-providing modules should implement this hook.
 *
 * commerce_bpc handles token management for combinations, to ensure
 * a minimally consistent set of tokens. In particular, commerce_bpc
 * will create a FIELD_NAME-value and a FIELD_NAME-label token for
 * each combination-creating field. The -value token should be fit
 * for inclusion in SKUs, while the -label token should be a human readable
 * value.
 *
 * @param string $product_type
 *   The product type to provide tokens for.
 * @param array $combination
 *   An array with field names as keys and their respective
 *   values as values.
 * @param array $options
 *   Options array passed unmodified from hook_tokens(). Make sure
 *   you honor the 'sanitize' property.
 *
 * @return array
 *   An array with two keys 'values' and 'labels', each of which should
 *   have an array keyed by field names with replacements as values.
 *
 * @see hook_commerce_bpc_get_combinations()
 */
function hook_commerce_bpc_tokens($product_type, $combination, $options) {
  $replacements = array();
  $sanitize = !empty($options['sanitize']);

  foreach ($combination as $field_name => $values) {
    $field = field_info_field($field_name);
    $instance = field_info_instance('commerce_product', $field_name, $product_type);
    if ($field['module'] == 'list' &&  !_commerce_bpc_get_value($instance, array('commerce_bpc', 'is_static'), FALSE)) {
      // TODO: Treat languages properly.
      $items = reset($values);
      // We only allow a single value per combination, so we can treat
      // this like a single value field.
      $value = $items[0]['value'];
      $replacements['values'][$field_name] = $value;
      $labels = list_allowed_values($field);
      $replacements['labels'][$field_name] = $sanitize ? check_plain($labels[$value]) : $labels[$value] ;
    }
  }
  return $replacements;
}
/**
 * Provide example values of the tokens corresponding to an instance.
 *
 * A module that manages a field instance for combination-creation
 * should use this hook to provide example values for the two tokens
 * managed by commerce_bpc (the 'label' and 'value' tokens).
 *
 * These example values are used, for example, to provide live previews
 * the SKU and title of the products to be generated.
 *
 * @param array $instance
 *   An array holding the instance info for which example values should be
 *   returned.
 *
 * @return array
 *   An array with two keys 'label' and 'value', holding an example value
 *   for each.
 *
 * @see hook_commerce_bpc_tokens()
 */
function hook_commerce_bpc_token_sample_values($instance) {
  $samples = array();
  $field_name = $instance['field_name'];
  $field = field_info_field($field_name);
  if ($field['module'] == 'list') {
    $options = list_allowed_values($field);
    $values = array_keys($options);
    $samples['value'] = array(LANGUAGE_NONE => array(array('value' => reset($values))));
    $samples['label'] = reset($options);
  }
  return $samples;
}
/**
 * Perform alterations on an auto-created display node, before it is saved.
 *
 * @param object $node
 *   The initialized display node object, with its product reference field
 *   set to the newly created products.
 */
function hook_commerce_bpc_display_node_alter(&$node) {
  // TODO: Needs example implementation.
}
/**
 * Perform alterations on submit.
 *
 * This hook is invoked just before the actual bulk creation happens.
 */
function hook_commerce_bpc_submit_alter($form, $form_state) {
  if (isset($form_state['commerce_bpc']['commerce_price']['moved_fields'])) {
    foreach ($form_state['commerce_bpc']['commerce_price']['moved_fields'] as $field_name) {
      $form_state['values']['static_values'][$field_name] = $form_state['values'][$field_name];
      unset($form_state['values'][$field_name]);
    }
  }
}

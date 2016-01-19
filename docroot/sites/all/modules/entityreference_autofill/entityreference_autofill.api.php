<?php

/**
 * @file
 * Hook documentation for the Entity reference autofill module.
 */

/**
 * Add support for other widgets.
 *
 * @return array
 *   Array of element parents that specify where to attach the AJAX callback,
 *   keyed by widget type.
 *
 * @see entityreference_autofill_supported_widgets()
 */
function hook_entityreference_autofill_supported_widgets() {
  // Add support for organic groups widget.
  // By setting value to NULL, no ajax wrapper will be
  // added to the field itself, but both group and
  // admin selection fields will be autofill-enabled.
  return array(
    'og_complex' => NULL,
  );
}

/**
 * Alter the form state before rendering an autofill field.
 *
 * @param array &$form_state
 *   The current form state for the entity form.
 * @param array $context
 *   Field context variables.
 *   - field: The field info array of the field about to be populated.
 *   - instance: The instance of the field.
 *   - items: The $items belonging to field_name in the
 *     referenced entity.
 *   - langcode: The $langcode.
 *   - reference_field_name: The name of the entityreference
 *     autofill-enabled field that called this autofill request.
 */
function hook_entityreference_autofill_fill_items_alter(&$form_state, $context) {
  // Add entityreference autofill support to addressfield module.
  if ($context['field']['type'] == 'addressfield') {
    // Generate element key for addressfield form state.
    // @see addressfield_field_widget_form()
    $element_key_parts = array(
      $context['instance']['entity_type'],
      $context['instance']['bundle'],
      $context['field']['field_name'],
      $context['langcode'],
    );
    $element_key_base = implode('|', $element_key_parts);
    // Add form_state data for each addressfield value
    // from referenced entity.
    foreach ($context['items'] as $delta => $item) {
      $element_key = $element_key_base . '|' . $delta;
      // Add item to addressfield form_state.
      $form_state['addressfield'][$element_key] = $item;
    }
  }
}

/**
 * Alter the ajax commands returned on entity selection.
 *
 * @param array &$commands
 *   Array of ajax replace commands to return.
 * @param array $context
 *   Form context variables.
 *   - form: The entity form array.
 *   - form_state: Current state of the entity form.
 */
function hook_entityreference_autofill_ajax_commands_alter(&$commands, $context) {
  // Add "ajax-changed" class to body tag.
  $commands[] = ajax_command_changed('body');
}

/**
 * Remove ajax from entityreference autofill enabled field(s).
 *
 * @param string $field_name
 *   The name of the field.
 * @param array $element
 *   The field widget form element as constructed by hook_field_widget_form().
 * @param array $context
 *   An associative array containing the following key-value pairs, matching the
 *   arguments received by hook_field_widget_form():
 *   - form: The form structure to which widgets are being attached. This may be
 *     a full form structure, or a sub-element of a larger form.
 *   - field: The field structure.
 *   - instance: The field instance structure.
 *   - langcode: The language associated with $items.
 *   - items: Array of default values for this field.
 *   - delta: The order of this item in the array of subelements (0, 1, 2, etc).
 *
 * @return bool
 *   return FALSE for fields you do not want to be AJAX enabled.
 */
function hook_entityreference_autofill_detach_ajax($field_name, $element, $context) {
  // Do not attach AJAX callback to OG admin fields.
  if (og_is_group_audience_field($field_name)) {
    if (isset($context['instance']['field_mode']) && $context['instance']['field_mode'] == 'admin') {
      return FALSE;
    }
  }
}

/**
 * Alter target id to fetch referenced values from.
 *
 * @param int &$target_id
 *   Current target id as determined by
 *   entityreference_autofill_field_attach_form().
 * @param array &$form_state
 *   The current $form_state array.
 * @param array $context
 *   An associative array containing the following key-value pairs:
 *   - field_name: The name of the reference field.
 *   - field: Triggering field (The entity reference field) info array.
 *     - field: Field info.
 *     - instance: Instance info.
 *   - form: Current form array.
 *   - langcode: The current langcode.
 */
function hook_entityreference_autofill_target_id_alter(&$target_id, &$form_state, $context) {
  // Fetch value from form input array instead of values.
  if (og_is_group_audience_field($context['field_name'])) {
    $reference_field_parents = $form_state['triggering_element']['#parents'];
    $referenced_target_id = drupal_array_get_nested_value($form_state['input'], $reference_field_parents);
  }
}

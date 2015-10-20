<?php

/**
 * @file
 * Administration pages.
 */

/**
 * Returns all fields in an array directly usable in '#options'.
 */
function coins_config_form_field_options() {
  $field_options = &drupal_static(__FUNCTION__);
  if (!isset($field_options)) {
    $field_options = drupal_map_assoc(array_keys(field_info_field_map()));
  }
  return $field_options;
}

/**
 * Build all form elements for one type detection rule.
 */
function coins_config_form_rule($i, $rule = NULL) {
  $field_options = coins_config_form_field_options();

  if (isset($rule)) {
    $title = t("If @field matches @regex, type is @type", array(
      '@field' => ($rule['field'] == 'type') ? t("content type") : $rule['field'],
      '@regex' => $rule['regex'],
      '@type' => $rule['type'],
    ));
  }
  else {
    $title = t("New rule");
  }

  $element = array(
    '#type' => 'fieldset',
    '#title' => $title,
    '#collapsible' => TRUE,
    '#collapsed' => isset($rule) ? TRUE : FALSE,
  );
  $element['field'] = array(
    '#type' => 'select',
    '#title' => t("Field"),
    '#options' => array(
      'type' => t("Content type"),
    ) + $field_options,
    '#default_value' => !empty($rule['field']) ? $rule['field'] : '',
    '#required' => TRUE,
  );
  $element['regex'] = array(
    '#type' => 'textfield',
    '#title' => t("Matches regex"),
    '#size' => 20,
    '#default_value' => !empty($rule['regex']) ? $rule['regex'] : '',
    '#required' => TRUE,
  );
  $element['type'] = array(
    '#type' => 'select',
    '#title' => t("Type"),
    '#options' => array(
      'book' => t("Book"),
      'section' => t("Book section"),
      'journal' => t("Journal"),
      'thesis' => t("Thesis"),
    ),
    '#default_value' => !empty($rule['type']) ? $rule['type'] : '',
  );
  $element['delete'] = array(
    '#type' => 'button',
    '#value' => t("Delete rule"),
    '#name' => 'delete_rule_' . $i,
    '#ajax' => array(
      'callback' => 'coins_config_form_rule_delete_callback',
      'wrapper' => 'coins_type_detection_rules',
      'method' => 'replace',
    ),
  );

  return $element;
}

/**
 * Build a select element for coins_config_form().
 */
function coins_config_form_mapping_field_select($name, $title, $mappings) {
  $field_options = coins_config_form_field_options();
  $select = array(
    '#type' => 'select',
    '#title' => $title,
    '#options' => $field_options,
    '#default_value' => isset($mappings[$name]) ? $mappings[$name] : NULL,
    '#empty_value' => '',
  );
  return $select;
}

/**
 * COinS configuration form.
 */
function coins_config_form($form, $form_state) {
  $coins_type_detection_rules = variable_get('coins_type_detection_rules', array());
  $field_options = drupal_map_assoc(array_keys(field_info_field_map()));
  $form['coins_type_detection_rules_fieldset'] = array(
    '#type' => 'fieldset',
    '#title' => t("Type detection rules"),
  );
  $form['coins_type_detection_rules_fieldset']['coins_type_detection_rules'] = array(
    '#type' => 'container',
    '#attributes' => array(
      'id' => 'coins_type_detection_rules',
    ),
    '#tree' => TRUE,
  );

  if (isset($form_state['complete form'])) {
    $form['coins_type_detection_rules_fieldset'] = $form_state['complete form']['coins_type_detection_rules_fieldset'];
    $matches = array();
    if (preg_match('/^delete_rule_([0-9]+)$/', $form_state['triggering_element']['#name'], $matches)) {
      $ruleno = $matches[1];
      unset($form['coins_type_detection_rules_fieldset']['coins_type_detection_rules'][$ruleno]);
    }
  }
  else {
    $i = 0;
    foreach ($coins_type_detection_rules as $rule) {
      $form['coins_type_detection_rules_fieldset']['coins_type_detection_rules'][$i] = coins_config_form_rule($i, $rule);
      $i++;
    }
  }

  if (isset($form_state['triggering_element']) && $form_state['triggering_element']['#name'] == 'add_rule') {
    $i = max(array_keys($form['coins_type_detection_rules_fieldset']['coins_type_detection_rules'])) + 1;
    $form['coins_type_detection_rules_fieldset']['coins_type_detection_rules'][$i] = coins_config_form_rule($i);
  }

  $rules = element_children($form['coins_type_detection_rules_fieldset']['coins_type_detection_rules']);
  if (empty($rules)) {
    $form['coins_type_detection_rules_fieldset']['coins_type_detection_rules'][0] = coins_config_form_rule(0);
  }

  $form['coins_type_detection_rules_fieldset']['add_rule'] = array(
    '#type' => 'button',
    '#value' => t("Add another rule"),
    '#name' => 'add_rule',
    '#ajax' => array(
      'callback' => 'coins_config_form_rule_add_callback',
      'wrapper' => 'coins_type_detection_rules',
      'method' => 'replace',
    ),
  );

  $coins_mappings = variable_get('coins_mappings', array());
  $fields = array(
    'btitle' => t("Book title (for a book section)"),
    'stitle' => t("Short title (for a journal)"),
    'series' => t("Series title (for a book or a book section)"),
    'issn' => "ISSN",
    'isbn' => "ISBN",
    'year' => t("Year"),
    'volume' => t("Volume"),
    'pages' => t("Pages"),
    'issue' => t("Issue"),
    'authors' => t("Authors"),
    'publisher' => t("Publisher"),
    'place_published' => t("Place published"),
    'doi' => t("DOI (Digital Object Identifier)"),
  );
  $form['coins_mappings'] = array(
    '#type' => 'fieldset',
    '#title' => t("Mappings"),
    '#tree' => TRUE,
  );
  foreach ($fields as $name => $title) {
    $form['coins_mappings'][$name] = coins_config_form_mapping_field_select($name, $title, $coins_mappings);
  }

  return system_settings_form($form);
}

/**
 * AJAX callback for rule deletion button.
 */
function coins_config_form_rule_delete_callback($form) {
  return $form['coins_type_detection_rules_fieldset']['coins_type_detection_rules'];
}

/**
 * AJAX callback for rule addition button.
 */
function coins_config_form_rule_add_callback($form) {
  return $form['coins_type_detection_rules_fieldset']['coins_type_detection_rules'];
}

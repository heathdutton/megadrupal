<?php

/**
 * @file
 * Admin forms and other for Smartling module administration section.
 */

/*
 * Gets all the fields that are configured to be translated "by fields" and "by nodes".
 * That's a conflict situation that should be avoided.
 */
function get_translatable_fields_with_conflict_settings() {
  $fields_with_problem = array();
  $title_module_fields = array('title_field', 'name_field', 'description_field');

  $node_translate_fields = smartling_settings_get_handler()->nodeGetFieldsSettings();
  foreach ($node_translate_fields as $bundle => $fields) {
    // Error in field settings.
    foreach (field_info_instances('node', $bundle) as $field) {
      $field_machine_name = $field['field_name'];
      if (smartling_nodes_method($bundle) && smartling_field_is_translatable($field_machine_name, 'node') && !in_array($field_machine_name, $title_module_fields)) {
        $fields_with_problem[$field_machine_name] = '<b>' . $field_machine_name . '</b>';
      }
    }
  }

  $term_translate_fields = smartling_settings_get_handler()->taxonomyTermGetFieldsSettings();
  foreach ($term_translate_fields as $bundle => $fields) {
    // Error in field settings.
    $voc = taxonomy_vocabulary_machine_name_load($bundle);
    $vocabulary_mode = i18n_taxonomy_vocabulary_mode($voc->vid);
    foreach (field_info_instances('taxonomy_term', $bundle) as $field) {
      $field_machine_name = $field['field_name'];
      if (($vocabulary_mode == I18N_MODE_TRANSLATE) && smartling_field_is_translatable($field_machine_name, 'taxonomy_term') && isset($term_translate_fields[$bundle][$field['field_name']]) && !in_array($field_machine_name, $title_module_fields)) {
        $fields_with_problem[$field_machine_name] = '<b>' . $field_machine_name . '</b>';
      }
    }
  }

  return $fields_with_problem;
}

/**
 * Smartling settings page callback.
 *
 * @return array
 *   Return render array.
 */
function smartling_admin_configuration_page() {
  // Add ajax library.
  drupal_add_library('system', 'drupal.ajax');
  $output['message'] = array(
    '#type' => 'markup',
    '#title' => 'Link to submission views',
    '#title_display' => 'invisible',
    '#prefix' => t('After you configure the Smartling module you can <a href=\"@url\">submit content for translation</a>.', array('@url' => url('admin/content/smartling-content'))),
  );

  $output['smartling'] = array(
    '#type' => 'vertical_tabs',
    '#attached' => array(
      'js' => array(drupal_get_path('module', 'smartling') . '/js/smartling_admin.js'),
      'css' => array(drupal_get_path('module', 'smartling') . '/css/smartling_admin.css'),
    ),
  );

  $settings_forms = module_invoke_all('smartling_settings_form_info');
  uasort($settings_forms, function($a, $b){return $a['weight'] > $b['weight'];});

  foreach ($settings_forms as $machine_name => $data) {
    $title = $data['title'];
    $form = drupal_get_form($machine_name);
    $output['smartling'][] = smartling_wrap_in_fieldset($form, $title);
  }

  $fields_with_problem = get_translatable_fields_with_conflict_settings();
  if (!empty($fields_with_problem) && is_array($fields_with_problem)) {
    $text = t('The following field(s) should be disabled for translation: !fields.', array('!fields' => implode(', ', $fields_with_problem)));
    $suffix = '<div id="smartling-untranslatable-fix">' . $text . '</div>';
    $output['fix'] = array(
      '#type' => 'markup',
      '#title' => t('Fix field settings'),
      '#title_display' => 'invisible',
      '#suffix' => $suffix,
    );
  }

  return $output;
}

/**
 * Form for smartling account settings.
 *
 * @param array $form
 *   FAPI array.
 * @param array $form_state
 *   FAPI array.
 *
 * @return array
 *   Return FAPI array for smartling account settings.
 */
function smartling_admin_account_info_settings_form(array $form, array &$form_state) {
  return drupal_container()->get('smartling.forms.admin_account_info_settings')->buildForm($form, $form_state);
}

/**
 * Validate smartling account settings form.
 *
 * @param array $form
 *   FAPI array.
 * @param array $form_state
 *   FAPI array.
 */
function smartling_admin_account_info_settings_form_validate(array $form, array &$form_state) {
  return drupal_container()->get('smartling.forms.admin_account_info_settings')->validateForm($form, $form_state);
}

/**
 * Smartling account settings - Form Submit.
 *
 * @param array $form
 *   FAPI array.
 * @param array $form_state
 *   FAPI array.
 */
function smartling_admin_account_info_settings_form_submit(array $form, array &$form_state) {
  return drupal_container()->get('smartling.forms.admin_account_info_settings')->submitForm($form, $form_state);
}

/**
 * Form for smartling log settings.
 *
 * @param array $form
 *   FAPI array.
 * @param array $form_state
 *   FAPI array.
 *
 * @return array
 *   Return FAPI array for smartling log settings.
 */
function smartling_admin_expert_settings_form(array $form, array &$form_state) {
  return drupal_container()->get('smartling.forms.admin_expert_settings')->buildForm($form, $form_state);
}

/**
 * Log Settings - Form Submit.
 *
 * @param array $form
 *   FAPI array.
 * @param array $form_state
 *   FAPI array.
 */
function smartling_admin_expert_settings_form_submit(array $form, array &$form_state) {
  return drupal_container()->get('smartling.forms.admin_expert_settings')->submitForm($form, $form_state);
}

/**
 * Wrapper for most kind of entities.
 *
 * @param array $form
 *   FAPI array.
 * @param array $form_state
 *   FAPI array.
 *
 * @return array
 *   Return FAPI array for smartling entity fields settings.
 */
function smartling_admin_entities_translation_settings_form(array $form, array &$form_state) {
  return drupal_container()->get('smartling.forms.admin_entities_translation_settings')->buildForm($form, $form_state);
}

/**
 * Custom submit handler for entities wrapper form.
 *
 * @see smartling_admin_entities_translation_settings_form()
 */
function smartling_admin_entities_translation_settings_form_submit(array $form, array &$form_state) {
  return drupal_container()->get('smartling.forms.admin_entities_translation_settings')->submitForm($form, $form_state);
}

/**
 * Form for smartling node fields settings.
 *
 * @param array $form
 *   FAPI array.
 * @param array $form_state
 *   FAPI array.
 *
 * @return array
 *   Return FAPI array for smartling node fields settings.
 */
function smartling_admin_node_translation_settings_form(array $form, array &$form_state) {
  return drupal_container()->get('smartling.forms.admin_node_translation_settings')->buildForm($form, $form_state);
}

/**
 * Node Translation Settings - Form Submit.
 *
 * @param array $form
 *   FAPI array.
 * @param array $form_state
 *   FAPI array.
 */
function smartling_admin_node_translation_settings_form_submit(array $form, array &$form_state) {
  return drupal_container()->get('smartling.forms.admin_node_translation_settings')->submitForm($form, $form_state);
}

/**
 * Form for smartling taxonomy fields settings.
 *
 * @param array $form
 *   FAPI array.
 * @param array $form_state
 *   FAPI array.
 *
 * @return array
 *   Return FAPI array for smartling taxonomy fields settings.
 */
function smartling_admin_taxonomy_translation_settings_form(array $form, array &$form_state) {
  return drupal_container()->get('smartling.forms.admin_taxonomy_translation_settings')->buildForm($form, $form_state);
}

/**
 * Taxonomy Translation Settings - Form Submit.
 *
 * @param array $form
 *   FAPI array.
 * @param array $form_state
 *   FAPI array.
 */
function smartling_admin_taxonomy_translation_settings_form_submit(array $form, array &$form_state) {
  return drupal_container()->get('smartling.forms.admin_taxonomy_translation_settings')->submitForm($form, $form_state);
}

/**
 * Wrap form in fieldset.
 *
 * @param array $form
 *   FAPI array.
 * @param string $title
 *   Title for fieldset.
 *
 * @return array
 *   Return FAPI array.
 */
function smartling_wrap_in_fieldset(array $form, $title) {
  return array(
    '#type' => 'fieldset',
    '#group' => 'smartling',
    '#title' => $title,
    '#attributes' => array(
      'class' => array('smartling-' . strtolower(str_replace(' ', '-', $title))),
      'id' => array('smartling-' . strtolower(str_replace(' ', '-', $title))),
    ),
    'children' => $form,
  );
}
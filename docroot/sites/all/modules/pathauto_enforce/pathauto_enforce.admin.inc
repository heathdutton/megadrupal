<?php

/**
 * @file
 * Administration page callbacks for the Pathauto Enforce module.
 */

/**
 * Form builder.
 */
function pathauto_enforce_admin_settings() {

  $form = array();

  $content_type_options = array();
  $content_type_options_not = array();
  $types = node_type_get_types();
  foreach ($types as $t) {
    $content_type_options[$t->type] = $t->name;
    $content_type_options_not[$t->type] = $t->name;
  }
  if ($content_type_options) {
    $form['pathauto_enforce_content_types'] = array(
      '#type'          => 'checkboxes',
      '#title'         => t('Enforce Pathauto patterns for these content types:'),
      '#options'       => $content_type_options,
      '#default_value' => variable_get('pathauto_enforce_content_types', array()),
    );
    $form['pathauto_enforce_not_content_types'] = array(
      '#type'          => 'checkboxes',
      '#title'         => t('Prevent the creation and generation of aliases for these content types:'),
      '#options'       => $content_type_options_not,
      '#default_value' => variable_get('pathauto_enforce_not_content_types', array()),
      '#description'   => t("If the 'Enforce' option is set, setting a value here will not have any effect."),
    );
  }

  if (module_exists('taxonomy')) {
    $vocabulary_options = array();
    $vocabulary_options_not = array();
    $vocabularies = taxonomy_get_vocabularies();
    foreach ($vocabularies as $v) {
      $vocabulary_options[$v->machine_name] = $v->name;
      $vocabulary_options_not[$v->machine_name] = $v->name;
    }
    if ($vocabulary_options) {
      $form['pathauto_enforce_vocabularies'] = array(
        '#type'          => 'checkboxes',
        '#title'         => t('Enforce Pathauto patterns for these vocabularies:'),
        '#options'       => $vocabulary_options,
        '#default_value' => variable_get('pathauto_enforce_vocabularies', array()),
      );
      $form['pathauto_enforce_not_vocabularies'] = array(
        '#type'          => 'checkboxes',
        '#title'         => t('Prevent the creation and generation of aliases for these vocabularies:'),
        '#options'       => $vocabulary_options_not,
        '#default_value' => variable_get('pathauto_enforce_not_vocabularies', array()),
        '#description'   => t("If the 'Enforce' option is set, setting a value here will not have any effect."),
      );
    }
  }

  return system_settings_form($form);
}

<?php
/**
 * @file
 * Internationalization (i18n) hooks.
 */

/**
 * Implements hook_i18n_string_info()
 */
function context_form_alteration_i18n_string_info() {
  $groups['context_form_alteration'] = array(
    'title' => t('Context Form Alteration'),
    'description' => t('Strings related to context altered forms.'),
    'format' => FALSE,
    'list' => TRUE,
  );
  return $groups;
}

/**
 * Implements hook_i18n_string_list().
 */
function context_form_alteration_i18n_string_list($group) {
  if ($group == 'context_form_alteration' || $group == 'all') {
    $strings = array();

    foreach (context_form_alteration_get_contexts() as $context) {
      foreach ($context->reactions['alter_form']['elements'] as $element) {
        if (isset($element['i18n']) && $element['i18n']) {
          $i18n_context = explode(':', context_form_alteration_i18n_context($element, $context));
          drupal_array_set_nested_value($strings, $i18n_context, $element['value']);
        }
      }
    }

    return $strings;
  }
}

/**
 * Implements hook_i18n_string_refresh().
 *
 * Refresh translations for all registered strings.
 */
function context_form_alteration_i18n_string_refresh($group) {
  if ($group == 'context_form_alteration') {
    context_form_alteration_i18n_update_strings();
  }
  return TRUE;
}

<?php

$plugin = array(
  'title' => t('Language Groups'),
  'description' => t('Check if the current language is in a designated group.'),
  'required context' => array(),
  'callback' => 'language_groups_language_groups_ctools_access_check',
  'settings form' => 'language_groups_language_groups_ctools_access_settings',
  'summary' => 'language_groups_language_groups_ctools_access_summary',
);

/**
 * Form API callback for the settings form.
 */
function language_groups_language_groups_ctools_access_settings($form, &$form_state, $conf) {

  $group_keys = array_keys(variable_get('language_groups', array()));
  $groups = array_combine($group_keys, $group_keys);

  $form['settings']['language_group'] = array(
    '#type' => 'checkboxes',
    '#options' => $groups,
    '#title' => 'Select a Language Group',
    '#default_value' => $conf['language_group'],
  );

  return $form;
}

/**
 * Plugin callback for the language groups selection rule.
 */
function language_groups_language_groups_ctools_access_check($conf, $context) {

  global $language;
  $languages = array();
  $groups_config = $conf['language_group'];

  $groups = variable_get('language_groups', array());
  foreach ($groups_config as $group => $selected) {
    if ($selected && !empty($groups[$group])) {
      $languages = array_merge($languages, $groups[$group]);
    }
  }

  if (!in_array($language->language, $languages)) {
    return FALSE;
  }

  return TRUE;
}

/**
 * Plugin callback for the summary of the language groups selection rule.
 */
function language_groups_language_groups_ctools_access_summary($conf, $context) {
  $groups = variable_get('language_groups', array());
  $languages = array();
  $language_groups = array();
  foreach ($conf['language_group'] as $group => $selected) {
    if ($selected && !empty($groups[$group])) {
      $languages = array_merge($languages, $groups[$group]);
      array_push($language_groups, $group);
    }
  }
  return t('Passes if the current language is in the groups: @group which includes langcodes:  @langcodes',
    array('@group' => implode(', ', $language_groups), '@langcodes' => implode(', ', $languages)));
}

<?php
/**
 * Plugins are described by creating a $plugin array which will be used
 * by the system that includes this file.
 */
$plugin = array(
  // Plugin user-friendly name.
  'title' => t('Substitution Match or Empty'),
  // Plugin description.
  'description' => t('@todo'),
  // Function which will allow or restrict access.
  'callback' => 'panels_substitutions_access_access',
  // Plugin settings form.
  'settings form' => 'panels_substitutions_access_settings_form',
  // Provides a summary descriptions depends on settings selected.
  'summary' => 'panels_substitutions_access_summary',
  'all contexts' => TRUE,
);

/**
 * Settings form for our access plugin.
 */
function panels_substitutions_access_settings_form(&$form, &$form_state, $conf) {
  $form['settings']['substitution'] = array(
    '#type' => 'textfield',
    '#title' => t('Substitution'),
    '#description' => t('You may use %keywords from contexts.'),
    '#default_value' => !empty($conf['substitution']) ? $conf['substitution'] : '',
  );
  $form['settings']['check_empty'] = array(
    '#type' => 'checkbox',
    '#title' => t('Check if value is empty'),
    '#description' => t('Check if the substitution value is empty.'),
    '#default_value' => !empty($conf['check_empty']) ? $conf['check_empty'] : 0,
  );
  $form['settings']['check_value'] = array(
    '#type' => 'textfield',
    '#title' => t('Value to Check for match'),
    '#description' => t('You may use also %keywords from contexts or just check for literal value.'),
    '#default_value' => !empty($conf['check_value']) ? $conf['check_value'] : '',
    '#states' => array(
      'invisible' => array(
        ':input[name="settings[check_empty]"]' => array('checked' => TRUE),
      ),
    ),
  );
  $form['settings']['use_regex'] = array(
    '#type' => 'checkbox',
    '#title' => t('Use regex match'),
    '#description' => t('Use regex to find a match.'),
    '#default_value' => !empty($conf['use_regex']) ? $conf['use_regex'] : 0,
    '#states' => array(
      'invisible' => array(
        ':input[name="settings[check_empty]"]' => array('checked' => TRUE),
      ),
    ),
  );
  // @todo How to put 'substitutions list here?
  return $form;
}
/**
 * Provide a summary description based on selected settings.
 */
function panels_substitutions_access_summary($conf, $context) {
  $check_empty = trim($conf['check_empty']);
  if ($check_empty) {
    return t('Substitution is empty @sub', array('@sub' => $conf['substitution']));
  }
  $summary = $conf['substitution'] . ' ' . t('equals') . ' ' . $conf['check_value'];

  if ($conf['use_regex']) {
    $summary = t('Regex Substitution Match') . ' ' . $summary;
  }
  else {
    $summary = t('Substitution Match') . ' ' . $summary;
  }
  return $summary;
}
/**
 * Check for access.
 */
function panels_substitutions_access_access($conf, $contexts) {
  $substitution = trim($conf['substitution']);
  $check_empty = trim($conf['check_empty']);
  $substituted = ctools_context_keyword_substitute($substitution, array(), $contexts);
  if ($check_empty) {
    return empty($substituted);
  }
  $check_value = trim($conf['check_value']);
  $check_value_substituted = ctools_context_keyword_substitute($check_value, array(), $contexts);
  if (!$conf['use_regex']) {
    return $substituted == $check_value_substituted;
  }
  return preg_match($check_value_substituted, $substituted);
}

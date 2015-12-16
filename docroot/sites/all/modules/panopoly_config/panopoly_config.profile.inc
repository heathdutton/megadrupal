<?php

/**
 * @file
 * Profile callbacks for the panopoly config module.
 */

/**
 * Profile callback for Panopoly Config.
 */
function panopoly_config_get_profile_form($form, $form_state) {
  $panopoly_install_config = _panopoly_config_get_install_config();

  foreach ($panopoly_install_config as $group_name => $group_info) {
    $variable_group_name = _panopoly_config_get_config_variable_group_name($group_name);
    if (count($group_info['config'])) {
      $form[$variable_group_name] = array(
        '#title' => $group_info['title'],
        '#type' => 'fieldset',
        '#description' => $group_info['description'],
        '#weight' => $group_info['weight'],
      );

      // Get the variable_edit_form for this group.
      variable_include();
      $group_form = variable_edit_form($form, $form_state, array_keys(variable_list_group($variable_group_name)));

      // Now loop through our group config and add only those required (aka install configs).
      foreach ($group_info['config'] as $config_name => $config_info) {
        $variable_name = _panopoly_config_get_config_variable_name($config_name);
        if (isset($group_form[$variable_name])) {
          $form[$variable_group_name][$variable_name] = $group_form[$variable_name];
        }
      }
    }
  }

  // Submit callback.
  $form['#submit'][] = 'variable_settings_form_submit';
  
  // Set form actions.
  $form['actions'] = array('#type' => 'actions');
  $form['actions']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save and continue'),
  );
  
  // Theme it as a system settings form.
  $form['#theme'] = 'system_settings_form';

  return $form;
}

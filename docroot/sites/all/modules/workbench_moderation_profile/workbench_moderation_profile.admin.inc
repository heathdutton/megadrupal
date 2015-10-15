<?php

/**
 * @file
 * Administrative functions for Workbench Moderation Profile.
 */

/**
 * Form constructor for profile forms.
 *
 * @see workbench_moderation_profile_form_submit()
 * @see workbench_moderation_profile_form_submit_delete()
 *
 * @ingroup forms
 */
function workbench_moderation_profile_form($form, &$form_state, $workbench_moderation_profile = NULL, $op = 'edit') {
  if ($op == 'clone') {
    $workbench_moderation_profile->label .= ' (cloned)';
    $workbench_moderation_profile->profile = '';
  }

  $form['label'] = array(
    '#title' => t('Label'),
    '#type' => 'textfield',
    '#default_value' => isset($workbench_moderation_profile->label) ? $workbench_moderation_profile->label : "",
    '#description' => t('The human-readable name of this transition.'),
    '#required' => TRUE,
    '#size' => 30,
  );
  // Machine-readable type name.
  $form['profile'] = array(
    '#type' => 'machine_name',
    '#default_value' => isset($workbench_moderation_profile->profile) ? $workbench_moderation_profile->profile : '',
    '#maxlength' => 32,
    '#machine_name' => array(
      'exists' => 'workbench_moderation_profile_check_machinename',
      'source' => array('label'),
    ),
    '#description' => t('A unique machine-readable name for this transition. It must only contain lowercase letters, numbers, and underscores.'),
  );

  field_attach_form('workbench_moderation_profile', $workbench_moderation_profile, $form, $form_state);

  $form['actions'] = array('#type' => 'actions');
  $form['actions']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save profile'),
    '#weight' => 40,
  );

  if ($op != 'add' && $op != 'clone') {
    $form['actions']['delete'] = array(
      '#type' => 'submit',
      '#value' => t('Delete profile type'),
      '#weight' => 45,
      '#limit_validation_errors' => array(),
      '#submit' => array('workbench_moderation_profile_form_submit_delete'),
    );
  }
  return $form;
}

/**
 * Form submission handler for profile form.
 */
function workbench_moderation_profile_form_submit(&$form, &$form_state) {
  $workbench_moderation_profile = entity_ui_form_submit_build_entity($form, $form_state);
  // Save and go back.
  $workbench_moderation_profile->save();
  drupal_set_message(t("Profile :profile has been created", array(':profile' => $form_state['values']['label'])));
  $form_state['redirect'] = 'admin/config/workbench/moderation/profile';
}

/**
 * Form delete handler for profile forms.
 */
function workbench_moderation_profile_form_submit_delete(&$form, &$form_state) {
  $form_state['redirect'] = 'admin/structure/profiles/manage/' . $form_state['workbench_moderation_profile']->profile . '/delete';
}

/**
 * Form constructor for profile weighting.
 *
 * @see workbench_moderation_profile_weights_form_submit()
 * @see theme_workbench_moderation_profile_weights_form()
 *
 * @ingroup forms
 */
function workbench_moderation_profile_weights_form($form, &$form_state) {
  $form['profile_modules']['#tree'] = TRUE;

  $mods = _workbench_moderation_profile_weighted_modules();

  // Create a row for each module.
  foreach ($mods as $mod => $weight) {
    $module = system_get_info('module', $mod);

    $form['profile_modules'][$mod] = array(
      'name' => array(
        '#markup' => check_plain($module['name']),
      ),
      'weight' => array(
        '#type' => 'weight',
        '#title' => t('Weight'),
        '#default_value' => $weight,
        '#delta' => 10,
        '#title_display' => 'invisible',
      ),
    );
  }
  $form['actions'] = array('#type' => 'actions');
  $form['actions']['submit'] = array('#type' => 'submit', '#value' => t('Save Changes'));
  drupal_set_title('Workbench Moderation Profile Weights');
  return $form;
}

/**
 * Theme callback for the workbench_moderation_profile_weights_form().
 */
function theme_workbench_moderation_profile_weights_form($variables) {
  $form = $variables['form'];

  // Initialize the variable which will store our table rows.
  $rows = array();

  // Iterate over each element in our $form['profile_modules'] array.
  foreach (element_children($form['profile_modules']) as $id) {
    $form['profile_modules'][$id]['weight']['#attributes']['class'] = array('profile-module-weight');

    $rows[] = array(
      'data' => array(
        drupal_render($form['profile_modules'][$id]['name']),
        drupal_render($form['profile_modules'][$id]['weight']),
      ),
      'class' => array('draggable'),
    );
  }
  $header = array(t('Name'), t('Weight'));
  $table_id = 'profile-modules-table';

  $output = theme('table', array(
    'header' => $header,
    'rows' => $rows,
    'attributes' => array('id' => $table_id),
    'empty' => t('No modules are enabled that provide Workbench Moderation Profile functionality.'),
    'caption' => t('Workbench Moderation Profiles utilizes a weighting structure to determine which profile a given node is assigned.<br/>Listed below are all the modules which implement workbench moderation profiles. You can choose the order in which they are invoked.'),
  ));

  // And then render any remaining form elements (such as our submit button).
  $output .= drupal_render_children($form);

  drupal_add_tabledrag($table_id, 'order', 'sibling', 'profile-module-weight');

  return $output;
}

/**
 * Form submission handler for workbench_moderation_profile_weights_form().
 */
function workbench_moderation_profile_weights_form_submit($form, &$form_state) {
  variable_set('workbench_moderation_profile_weights', $form_state['values']['profile_modules']);
  drupal_set_message(t('Your settings have been saved.'));
}

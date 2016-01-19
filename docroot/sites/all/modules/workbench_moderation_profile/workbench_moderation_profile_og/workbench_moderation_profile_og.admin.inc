<?php

/**
 * @file
 * Admin settings for Workbench Moderation Profile by groups module.
 */

/**
 * Form constructor for the profile choice form on organic groups.
 *
 * @param string $group_type
 *   The type of group being used.
 *
 * @param string $gid
 *   The id of the group being used.
 *
 * @see workbench_moderation_profile_og_profile_form_submit()
 *
 * @ingroup forms
 */
function workbench_moderation_profile_og_profile_form($form, &$form_state, $group_type, $gid) {
  og_set_breadcrumb($group_type, $gid, array(l(t('Group'), "$group_type/$gid/group")));

  $form['group_type'] = array('#type' => 'value', '#value' => $group_type);
  $form['gid'] = array('#type' => 'value', '#value' => $gid);

  // Get profiles list. Not permissions-aware because it's an admin form.
  $options = workbench_moderation_profile_make_options_array(TRUE);
  $current = workbench_moderation_profile_og_get_profile($gid, $group_type);

  $form['workbench_moderation_profile_og'] = array(
    '#title' => t('Moderation Profile'),
    '#type' => 'select',
    '#options' => $options,
    '#default_value' => $current,
    '#description' => t('Set the moderation profile for this group.'),
    '#weight' => 10,
  );

  if (!empty($current)) {
    $state_options = workbench_moderation_profile_get_states($current);
    $state_current = workbench_moderation_profile_og_get_default_state($gid, $group_type);
  }
  else {
    $state_options = workbench_moderation_state_labels();
    $state_current = "";
  }

  $form['workbench_moderation_profile_og_default_state'] = array(
    '#title' => t('Moderation Profile default state'),
    '#type' => 'select',
    '#options' => $state_options,
    '#default_value' => $state_current,
    '#description' => t('Set the default moderation state for this group.'),
    '#weight' => 12,
  );

  // Add JS to change this dynamically, too.
  drupal_add_js(drupal_get_path('module', 'workbench_moderation_profile_og') . "/js/workbench_moderation_profile_og_admin.js", array('type' => 'file'));

  $form['actions'] = array('#type' => 'actions');
  $form['actions']['submit'] = array('#type' => 'submit', '#value' => t('Set Profile'));
  return $form;
}

/**
 * Form submission handler for workbench_moderation_profile_og_profile_form().
 */
function workbench_moderation_profile_og_profile_form_submit($form, &$form_state) {
  $group_type = $form_state['values']['group_type'];
  $gid = $form_state['values']['gid'];

  $form_state['redirect'] = $group_type . '/' . $gid . '/group';

  $profile = $form_state['values']['workbench_moderation_profile_og'];
  $default = $form_state['values']['workbench_moderation_profile_og_default_state'];

  db_merge('workbench_moderation_profile_og')
    ->key(array('gid' => $gid, 'type' => $group_type))
    ->fields(array(
      'gid' => $gid,
      'type' => $group_type,
      'wmpid' => $profile,
      'default_state' => $default,
    ))
    ->execute();
  drupal_set_message(t('Workbench moderation profile has been set for this group'), 'status');
}

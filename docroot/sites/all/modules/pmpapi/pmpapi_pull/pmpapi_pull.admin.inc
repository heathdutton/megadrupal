<?php

/**
 * @file
 * Basic admin forms, validators, and submit handlers.
 */

/**
 * Form constructor for the PMPAPI pull admin form.
 *
 * @see pmpapi_pull_admin_config_validate()
 * @see pmpapi_pull_admin_config_submit()
 *
 * @ingroup forms
 */
function pmpapi_pull_admin_config($form, &$form_state) {
  $form = array();
  $form['pmpapi_pull_pull_active'] = array(
    '#type' => 'radios',
    '#title' => t('Activate pull'),
    '#default_value' => variable_get('pmpapi_pull_pull_active', 1),
    '#options' => array('No', 'Yes'),
  );

  $pull_user_options = array();
  $users = entity_load('user');
  foreach($users as $uid => $user) {
    $pull_user_options[$uid] = $user->name;
  }
  $pull_user_options[0] = variable_get('anonymous', t('Anonymous'));

  $form['pmpapi_pull_pull_user'] = array(
    '#type' => 'select',
    '#title' => t('Pull creator'),
    '#default_value' => variable_get('pmpapi_pull_pull_user', 1),
    '#options' => $pull_user_options,
    '#description' => t('All pulled entities will be created with the uid of the chosen user.'),
  );

  $available_profiles = array(0 => 'None') + pmpapi_get_profile_list();

  foreach (pmpapi_pull_get_entities() as $entity_type => $entity) {
    $bundles = $entity['bundles'];
    if (!empty($bundles)) {
      $form[$entity_type] = array(
        '#type' => 'fieldset',
        '#title' => $entity['label'] . ' ' . t('entities'),
        '#collapsible' => TRUE,
        '#collapsed' => TRUE,
      );
      foreach ($bundles as $bundle_name => $bundle) {
        $label = $bundle['label'];
        $uname = $entity_type . '__' . $bundle_name;
        $form[$entity_type][$bundle_name . '_settings'] = array(
          '#type' => 'fieldset',
          '#title' => $label . ' ' . t('settings'),
          '#collapsible' => TRUE,
          '#collapsed' => TRUE,
        );

        $profile = variable_get('pmpapi_pull_' . $uname . '_profile', 0);
        if (isset($form_state['values'][$uname . '_pull_profile'])) {
          $profile = $form_state['values'][$uname . '_pull_profile'];
        }
        $id = $uname . '-mapping-fields-div';

        $form[$entity_type][$bundle_name . '_settings'][$uname . '_pull_profile'] = array(
          '#type' => 'select',
          '#title' => t('Mapped PMP profile'),
          '#description' => 'The PMP profile (e.g., story, image, audio) which will be mapped to the ' . $label . ' entity type.',
          '#options' => $available_profiles,
          '#default_value' => $profile,
          '#ajax' => array(
            'callback' => 'pmpapi_pull_admin_mapping_callback',
            'wrapper' => $id,
            'method' => 'replace',
            'effect' => 'fade',
            'pmpapi_pull_entity_type' => $entity_type,
            'pmpapi_pull_bundle_name' => $bundle_name,
          ),
        );

        $local_options = array();
        $instances = pmpapi_get_augmented_instances($entity_type, $bundle_name);
        foreach ($instances as $instance_name => $instance) {
          $local_options[$instance_name] = $instance['label'];
        }
        $local_options = array('None') + $local_options;

        $map = variable_get('pmpapi_pull_mapping_' . $uname . '_' . $profile, array());
        $form[$entity_type][$bundle_name . '_settings'][$bundle_name . '_mappings'] = array(
          '#type' => 'fieldset',
          '#title' => $label . ' ' . t('mappings'),
          '#collapsible' => TRUE,
          '#collapsed' => FALSE,
          '#prefix' => '<div id="' . $id . '">',
          '#suffix' => '</div>',
        );
        if (!$profile) {
          $form[$entity_type][$bundle_name . '_settings'][$bundle_name . '_mappings']['#attributes']['class'][] = 'element-invisible';
        }
        $pmp_fields = pmpapi_get_profile_info($profile);
        foreach ($pmp_fields as $pmp_name => $pmp_field) {
          $element = 'pmpapi_pull_mapping_' . $uname . '_'. $pmp_name;
          $form[$entity_type][$bundle_name . '_settings'][$bundle_name . '_mappings'][$element] = array(
            '#type' => 'select',
            '#title' => t('Map the "' . $pmp_name . '" PMP value to the following ' . $bundle_name . ' field:'),
            '#options' => $local_options,
            '#default_value' => !empty($map[$pmp_name]) ? $map[$pmp_name] : 0,
          );
          if (!empty($pmp_field['required'])) {
            $form[$entity_type][$bundle_name . '_settings'][$bundle_name . '_mappings'][$element]['#required'] = TRUE;
          }
        }
      }
    }
  }

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save settings'),
  );
  return $form;
}

/**
 * Hide/show bundle mapping fieldset.
 *
 * If no bundle is chosen, hide the related fieldset of mapping selects. If a bundle
 * is chosen, display the mapping fieldset (for a entity given type and bundle).
 *
 * #ajax callback for pmpapi_pull_admin_config()
 *
 * @return array
 *   The mappings for the chosen bundle
 *
 * @see pmpapi_pull_admin_config()
 */
function pmpapi_pull_admin_mapping_callback($form, $form_state) {
  if (!empty($form_state['triggering_element']['#ajax']['pmpapi_pull_entity_type']) && !empty($form_state['triggering_element']['#ajax']['pmpapi_pull_bundle_name'])) {
    $entity_type = $form_state['triggering_element']['#ajax']['pmpapi_pull_entity_type'];
    $bundle_name = $form_state['triggering_element']['#ajax']['pmpapi_pull_bundle_name'];
    if ($form_state['triggering_element']['#value'] == '0') {
      $form[$entity_type][$bundle_name . '_settings'][$bundle_name . '_mappings']['#attributes']['class'][] = 'element-invisible';
    }
    else {
      $form[$entity_type][$bundle_name . '_settings'][$bundle_name . '_mappings']['#attributes']['class'] = NULL;
    }
    return $form[$entity_type][$bundle_name . '_settings'][$bundle_name . '_mappings'];
  }
}

/**
 * Form validation handler for pmpapi_pull_admin_config().
 *
 * @see pmpapi_pull_admin_config_submit()
 *
 * @ingroup forms
 */
function pmpapi_pull_admin_config_validate($form, &$form_state) {
  $taken_profiles = array();
  foreach (pmpapi_pull_get_entities() as $entity_type => $entity) {
    $bundles = $entity['bundles'];
    foreach ($bundles as $bundle_name => $bundle) {
      $uname = $entity_type . '__' . $bundle_name;
      $profile = $form_state['values'][$uname . '_pull_profile'];
      if ($profile) {
        if (in_array($profile, $taken_profiles)) {
          form_set_error($uname . '_pull_profile', t('A profile can be mapped to no more than one content type.'));
        }
        $taken_profiles[] = $profile;
        $pmp_fields = pmpapi_get_profile_info($profile);
        $local_fields = pmpapi_get_augmented_fields($entity_type, $bundle_name);
        $taken_fields = array();
        foreach ($pmp_fields as $pmp_name => $pmp_field) {
          $value = $form_state['values']['pmpapi_pull_mapping_' . $uname . '_'. $pmp_name];
          if ($value !== '0') {
            if (!in_array($local_fields[$value]['type'], $pmp_field['accepted_types'])) {
              $error_msg = "To map from the PMP value $pmp_name, the (Drupal) field must be one of the following type(s): " . implode(', ', $pmp_field['accepted_types']);
              form_set_error('pmpapi_pull_mapping_' . $uname . '_'. $pmp_name, t('Incompatible field types.') . ' ' . $error_msg);
            }
            if (in_array($value, $taken_fields)) {
              form_set_error('pmpapi_pull_mapping_' . $uname . '_'. $pmp_name, t('Multiple PMP values cannot be mapped to the same (Drupal) field.'));
            }
            $taken_fields[] = $value;
          }
        }
      }
    }
  }
}

/**
 * Form submission handler for pmpapi_pull_admin_config().
 *
 * @see pmpapi_pull_admin_config_validate()
 *
 * @ingroup forms
 */
function pmpapi_pull_admin_config_submit($form, &$form_state) {
  variable_set('pmpapi_pull_pull_active', $form_state['values']['pmpapi_pull_pull_active']);
  variable_set('pmpapi_pull_pull_user', $form_state['values']['pmpapi_pull_pull_user']);
  foreach (pmpapi_pull_get_entities() as $entity_type => $entity) {
    $bundles = $entity['bundles'];
    foreach ($bundles as $bundle_name => $bundle) {
      $uname = $entity_type . '__' . $bundle_name;
      $profile = $form_state['values'][$uname . '_pull_profile'];
      if ($profile) {
        $pmp_fields = pmpapi_get_profile_info($profile);
        $map = array();
        foreach ($pmp_fields as $pmp_name => $pmp_field) {
          if (isset($form_state['values']['pmpapi_pull_mapping_' . $uname . '_'. $pmp_name])) {
            $map[$pmp_name] = $form_state['values']['pmpapi_pull_mapping_' . $uname . '_'. $pmp_name];
          }
        }
        variable_set('pmpapi_pull_mapping_' . $uname . '_' . $profile, $map);
      }
      variable_set('pmpapi_pull_' . $uname . '_profile', $profile);
    }
  }
  drupal_set_message(t('Pull settings have been successfully saved.'));
}

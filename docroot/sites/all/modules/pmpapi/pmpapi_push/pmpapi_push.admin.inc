<?php

/**
 * @file
 * Basic admin forms, validators, and submit handlers for the PMPAPI Push module.
 */

/**
 * Form constructor for the PMPAPI push admin form.
 *
 * @see pmpapi_push_admin_config_validate()
 * @see pmpapi_push_admin_config_submit()
 *
 * @ingroup forms
 */
function pmpapi_push_admin_config($form, &$form_state) {
  $form = array();

  $form['pmpapi_push_push_active'] = array(
    '#type' => 'radios',
    '#title' => t('Activate push'),
    '#default_value' => variable_get('pmpapi_push_push_active', 1),
    '#options' => array('No', 'Yes'),
  );

  $available_profiles = array(0 => 'None') + pmpapi_get_profile_list();

  foreach (pmpapi_push_get_entities() as $entity_type => $entity) {
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

        $profile = variable_get('pmpapi_push_' . $uname . '_profile', 0);
        if (isset($form_state['values'][$uname. '_push_profile'])) {
          $profile = $form_state['values'][$uname . '_push_profile'];
        }

        $id = $uname . '-mapping-fields-div';

        $form[$entity_type][$bundle_name . '_settings'][$uname . '_push_profile'] = array(
          '#type' => 'select',
          '#title' => t('Mapped PMP profile'),
          '#description' => 'The PMP profile (e.g., story, episode) to which the ' . $label . ' entity will be mapped.',
          '#options' => $available_profiles,
          '#default_value' => $profile,
          '#ajax' => array(
            'callback' => 'pmpapi_push_admin_mapping_callback',
            'wrapper' => $id,
            'method' => 'replace',
            'effect' => 'fade',
            'pmpapi_push_entity_type' => $entity_type,
            'pmpapi_push_bundle_name' => $bundle_name,
          ),
        );

        $fields = pmpapi_get_augmented_fields($entity_type, $bundle_name);
        $push_flag_fields = array(t('None'));
        if (!empty($fields)) {
          $push_flag_fields = $push_flag_fields + array_combine(array_keys($fields), array_keys($fields));
        }
        $push_flag = variable_get('pmpapi_push_' . $uname . '_push_flag', 0);

        $form[$entity_type][$bundle_name . '_settings'][$uname . '_push_flag'] = array(
          '#type' => 'select',
          '#title' => t('Push flag'),
          '#description' => 'The ' . $label . ' field which will signal whether or not a given ' . $bundle_name . ' will be pushed (truthy means push, falsey means do not push). If no flag is chosen, ALL (published) ' . $bundle_name . ' entities will be pushed.',
          '#options' => $push_flag_fields,
          '#default_value' => $push_flag,
        );

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

        $map = variable_get('pmpapi_push_mapping_' . $uname . '_' . $profile, array());
        $pmp_fields = pmpapi_get_profile_info($profile);
        $pmp_options = array();
        if (!empty($pmp_fields)) {
          $pmp_options = array_combine(array_keys($pmp_fields), array_keys($pmp_fields));
          $pmp_options = array(t('None')) + $pmp_options;
        }

        foreach ($fields as $field_name => $field) {
          $element = 'pmpapi_push_mapping_' . $uname . '_'. $field_name;
          $form[$entity_type][$bundle_name . '_settings'][$bundle_name . '_mappings'][$element] = array(
            '#type' => 'select',
            '#title' => t('Map "' . $field_name . '" to the following PMP value:'),
            '#options' => $pmp_options,
            '#default_value' => !empty($map[$field_name]) ? $map[$field_name] : 0,
          );
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
 * #ajax callback for pmpapi_push_admin_config()
 *
 * @return array
 *   The mappings for the chosen bundle
 *
 * @see pmpapi_push_admin_config()
 */
function pmpapi_push_admin_mapping_callback($form, $form_state) {
  if (!empty($form_state['triggering_element']['#ajax']['pmpapi_push_entity_type']) && !empty($form_state['triggering_element']['#ajax']['pmpapi_push_bundle_name'])) {
    $entity_type = $form_state['triggering_element']['#ajax']['pmpapi_push_entity_type'];
    $bundle_name = $form_state['triggering_element']['#ajax']['pmpapi_push_bundle_name'];
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
 * Form validation handler for pmpapi_push_admin_config().
 *
 * @see pmpapi_push_admin_config_submit()
 */
function pmpapi_push_admin_config_validate($form, &$form_state) {
  foreach (pmpapi_push_get_entities() as $entity_type => $entity) {
    $bundles = $entity['bundles'];
    if ($bundles) {
      foreach ($bundles as $bundle_name => $bundle) {
        $fields = pmpapi_get_augmented_fields($entity_type, $bundle_name);
        $uname = $entity_type . '__' . $bundle_name;
        $taken = array();
        $profile = $form_state['values'][$uname . '_push_profile'];
        if ($profile) {
          foreach ($fields as $field_name => $field) {
            $pmp_fields = pmpapi_get_profile_info($profile);
            $value = $form_state['values']['pmpapi_push_mapping_' . $uname . '_'. $field_name];
            if ($value !== '0') {
              if (!in_array($field['type'], $pmp_fields[$value]['accepted_types'])) {
                $error_msg = "To map to PMP field $value, the (drupal) field must be one of the following type(s): " . implode(', ', $pmp_fields[$value]['accepted_types']);
                form_set_error('pmpapi_push_mapping_' . $uname . '_' . $field_name, t('Incompatible field types.') . ' ' . $error_msg);
              }
              $taken[] = $value;
            }
          }
          foreach ($pmp_fields as $pmp_name => $pmp_field) {
            if (!empty($pmp_field['required']) && !in_array($pmp_name, $taken)) {
              form_set_error($bundle_name . '_mappings',t('%pmp_name is a required field.', array('%pmp_name' => $pmp_name)));
            }
          }
        }
      }
    }
  }
}

/**
 * Form submission handler for pmpapi_push_admin_config().
 *
 * @see pmpapi_push_admin_config_validate()
 */
function pmpapi_push_admin_config_submit($form, &$form_state) {

  variable_set('pmpapi_push_push_active', $form_state['values']['pmpapi_push_push_active']);
  foreach (pmpapi_push_get_entities() as $entity_type => $entity) {
    $bundles = $entity['bundles'];
    if ($bundles) {
      foreach ($bundles as $bundle_name => $bundle) {
        $uname = $entity_type . '__' . $bundle_name;
        $profile = $form_state['values'][$uname . '_push_profile'];
        if ($profile) {
          $mapping = array();
          $fields = pmpapi_get_augmented_fields($entity_type, $bundle_name);
          foreach ($fields as $field_name => $field) {
            $mapping[$field_name] = $form_state['values']['pmpapi_push_mapping_' . $uname . '_'. $field['field_name']];
          }
          variable_set('pmpapi_push_mapping_' . $uname . '_' . $profile, $mapping);
          variable_set('pmpapi_push_' . $uname . '_push_flag',  $form_state['values'][$uname . '_push_flag']);
        }
        variable_set('pmpapi_push_' . $uname . '_profile', $profile);
      }
    }
  }
  drupal_set_message(t('Push settings have been successfully saved.'));
}

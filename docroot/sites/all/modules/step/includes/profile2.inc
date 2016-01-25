<?php

/**
 * @file
 * Integrates Multi-Step Registration with Profile2.
 */

/**
 * Implements hook_form_FORM_ID_alter().
 */
function step_form_profile2_type_form_alter(&$form, &$form_state, $form_id) {
  $profile2 = $form_state['profile2_type'];

  // Hide the default Profile2 'Show during user account registration' switch.
  $form['data']['registration'] = array(
    '#type' => 'value',
    '#value' => FALSE,
  );


  $form['data']['step_step'] = array(
    '#type' => 'select',
    '#title' => t('Show in user registration, in step'),
    '#description' => t('Select a step from the multi-step user registration wizard in which you want to expose the form of this profile type.'),
    '#options' => step_steps(),
    '#empty_option' => '-' . t('None') . '-',
    '#default_value' => empty($profile2->data['step_step']) ? NULL : $profile2->data['step_step'],
  );

  if (!empty($profile2->type)) {
    $instances = array();
    foreach (field_info_instances('profile2', $profile2->type) as $name => $instance) {
      if (empty($instance['required'])) {
        $instances[$name] = format_string('@label [@name]', array('@label' => $instance['label'], '@name' => $name));
      }
    }
    $form['data']['step_exclude'] = array(
      '#type' => 'checkboxes',
      '#title' => t('Exclude fields'),
      '#description' => t('You may want to restrict the fields that are exposed in the multi-step registration wizard. Note that only optional fields can be excluded from the registration wizard.'),
      '#options' => $instances,
      '#default_value' => empty($profile2->data['step_exclude']) ? array() : $profile2->data['step_exclude'],
      '#states' => array(
        'disabled' => array(
          'select[name="data[step_step]"]' => array('value' => ''),
        ),
      ),
    );
  }
}

/**
 * Implements hook_form_alter().
 *
 * Inject Profile2 forms in registration steps. This was inspired by
 * profile2_form_user_register_form_alter().
 *
 * @see profile2_form_user_register_form_alter().
 */
function step_form_alter(&$form, &$form_state, $form_id) {
  $forms = array_merge(array_keys(step_forms(NULL, NULL)), array('user_register_form'));
  if (!in_array($form_id, $forms) || empty($form_state['step'])) {
    return;
  }

  $step = $form_state['step'];

  foreach (profile2_get_types() as $type_name => $profile_type) {
    if (!empty($profile_type->data['step_step']) && $profile_type->data['step_step'] == $step) {
      $key = 'profile_' . $type_name;

      if (!empty($form_state['user']->uid)) {
        $form_state['profiles'][$type_name] = profile2_load_by_user($form_state['user'], $type_name);
      }
      if (empty($form_state['profiles'][$type_name])) {
        $form_state['profiles'][$type_name] = profile2_create(array('type' => $type_name));
      }
      profile2_attach_form($form, $form_state);

      // Hide hidden fields.
      if (!empty($profile_type->data['step_exclude']) && is_array($profile_type->data['step_exclude'])) {
        foreach ($profile_type->data['step_exclude'] as $field_name) {
          if (!empty($field_name) && isset($form[$key][$field_name]) && empty($form[$key][$field_name]['#required'])) {
            $form[$key][$field_name]['#access'] = FALSE;
          }
        }
      }

      // Wrap each profile form in a container.
      $form[$key] += array(
        '#type' => variable_get('step_profile2_type_fieldset', TRUE) ? 'fieldset' : 'container',
        '#title' => check_plain($profile_type->getTranslation('label')),
      );
    }
  }
}

/**
 * Implements hook_field_delete_instance().
 *
 * We remove this instance in all occurrences in Profile2 types.
 */
function step_field_delete_instance($instance) {
  if ($instance['entity_type'] != 'profile2') {
    return;
  }

  $type = db_select('profile_type', 'p')
    ->fields('p', array('id', 'data'))
    ->condition('type', $instance['bundle'])
    ->execute()
    ->fetch();

  if (!empty($type->id)) {
    $data = unserialize($type->data);
    if (isset($data['step_exclude'][$instance['field_name']])) {
      unset($data['step_exclude'][$instance['field_name']]);
      $data = serialize($data);
      db_update('profile_type')
        ->fields(array('data' => $data))
        ->condition('id', $type->id)
        ->execute();
    }
  }
}

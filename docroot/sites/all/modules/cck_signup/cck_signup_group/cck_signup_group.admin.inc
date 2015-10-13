<?php


/**
 * @file
 * Forms and callback handlers for CCK Group Sign-up module.
 */

/**
 * Allow node-types to be defined as group sign-up nodes.
 */
function _cck_signup_group_form_node_type_form_alter(&$form, $form_state) {
  $type = $form['old_type']['#value'];
  if ($type) {
    $collapsed = !variable_get('cck_signup_group_type_' . $type, 0);
    $form['cck_signup']['cck_signup_group'] = array(
      '#type' => 'fieldset',
      '#collapsible' => TRUE,
      '#collapsed' => $collapsed,
      '#title' => t('CCK Group Sign-up'),
      // @TODO this is putting the fieldset in its own tab.
      '#group' => 'additional_settings',
    );
    if (!$collapsed) {
      $form['cck_signup']['#collapsed'] = $collapsed;
    }
    $form['cck_signup']['cck_signup_group']['cck_signup_group_type'] = array(
      '#type' => 'checkbox',
      '#default_value' => variable_get('cck_signup_group_type_' . $type, 0),
      '#title' => t('Enable this node type as a <em>Group Sign-up</em>.'),
    );

    if (($options = _cck_signup_numeric_fields($type)) && !empty($options)) {
      // Potential group size fields.
      array_unshift($options, t('-- Choose --'));
      $form['cck_signup']['cck_signup_group']['cck_signup_group_field_count'] = array(
        '#type' => 'select',
        '#title' => t('Group size/count field'),
        '#options' => $options,
        '#default_value' => variable_get('cck_signup_group_field_count_' . $type, 0),
        '#description' => t('Must be selected to enable this as a group sign-up node.'),
      );

      // Offer optional field group list for added fanciness.
      if (module_exists('field_group')) {
        $options = _cck_signup_group_fieldgroups($type);
        array_unshift($options, t('-- Choose --'));
        $form['cck_signup']['cck_signup_group']['cck_signup_group_fieldgroup'] = array(
          '#type' => 'select',
          '#title' => t('Associated Field Group'),
          '#options' => $options,
          '#default_value' => variable_get('cck_signup_group_fieldgroup_' . $type, 0),
          '#description' => t('Optionally, select a field group on this node type that will contain group-signup-specific information. The field group will be hidden until a user selects to sign up more than one person for the event.'),
        );
      }
    }
    else {
      $form['cck_signup']['cck_signup_group']['cck_signup_group_type']['#disabled'] = TRUE;
      $form['cck_signup']['cck_signup_group']['cck_signup_group_type']['#default_value'] = 0;
      $form['cck_signup']['cck_signup_group']['cck_signup_group_type']['#description'] = t('In order to enable this node type as a group sign-up, there must be an available integer (number module) field to use as the group size.');
    }

    if (!in_array('cck_signup_group_form_node_type_form_validate', $form['#validate'])) {
      $form['#validate'][] = 'cck_signup_group_form_node_type_form_validate';
    }
  }
}

/**
 * @todo Please document this function.
 * @see http://drupal.org/node/1354
 */
function cck_signup_group_form_node_type_form_validate($form, &$form_state) {
  if ($form_state['values']['cck_signup_group_type'] && !$form_state['values']['cck_signup_group_field_count']) {
    form_set_error('cck_signup_group_field_count', t('Must select an integer field to use as the group size for group sign-ups.'));
  }
}

/**
 * Return a formatted array of fieldgroups available for a given node type.
 */
function _cck_signup_group_fieldgroups($type) {
  $groups = field_group_info_groups();
  $options = array();
  if (!empty($groups['node'][$type]['form'])) {
    foreach ($groups['node'][$type]['form'] as $name => $group) {
      $options[$name] = $group->label;
    }
  }
  return $options;
}

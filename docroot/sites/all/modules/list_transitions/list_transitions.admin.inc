<?php

/**
 * @file
 * Administrative pages and forms for the List Transitions module.
 */

/**
 * Transition form.
 */
function list_transitions_criterion_form($form, &$form_state, $instance, $entity_type) {
  $field = field_info_field($instance['field_name']);
  $settings = list_transitions_get_instance_settings($instance['field_name'], $entity_type, $instance['bundle']);

  // Replicates functionality in list_transitions_field_widget_form() to get the
  // options. Yuck. We effectively have to pull all the pieces from the form.
  $items = $instance['default_value'];
  $value_key = key($field['columns']);
  $type = str_replace('list_transitions_', '', $instance['widget']['type']);
  $multiple = $field['cardinality'] > 1 || $field['cardinality'] == FIELD_CARDINALITY_UNLIMITED;
  $required = $instance['required']['#default_value'];
  $has_value = isset($items[0][$value_key]);
  $properties = _options_properties($type, $multiple, $required, $has_value);

  // Prepare the list of options for the from and to values.
  $options = _options_get_options($field, $instance, $properties, $entity_type, NULL);
  unset($options['_none']);

  ////
  ////
  //// Stores values
  ////
  ////

  $form['instance'] = array(
    '#type' => 'value',
    '#value' => $instance,
  );

  $form['entity_type'] = array(
    '#type' => 'value',
    '#value' => $entity_type,
  );

  $form['bundle_name'] = array(
    '#type' => 'value',
    '#value' => $instance['bundle'],
  );

  $form['settings'] = array(
    '#type' => 'value',
    '#value' => $settings,
  );

  $form['processed'] = array(
    '#type' => 'value',
    '#value' => array(),
  );

  ////
  ////
  //// Add transition
  ////
  ////

  $form['add'] = array(
    '#tree' => TRUE,
    '#type' => 'container',
    '#prefix' => '<div class="clearfix">',
    '#suffix' => '</div>',
    '#states' => array(
      'visible' => array(
        ':input[name="field[list_transitions_allowed]"]' => array('checked' => TRUE),
      ),
    ),
    '#attached' => array(
      'css' => array(drupal_get_path('module', 'list_transitions') . '/list_transitions.css',),
    ),
  );

  $form['add']['from'] = array(
    '#type' => 'select',
    '#title' => t('From value'),
    '#options' => $options,
    '#prefix' => '<div class="list-transitions-setting list-transitions-value-from">',
    '#suffix' => '</div>',
  );

  $form['add']['to'] = array(
    '#type' => 'select',
    '#title' => t('To value'),
    '#options' => $options,
    '#prefix' => '<div class="list-transitions-setting list-transitions-value-to">',
    '#suffix' => '</div>',
  );

  $form['add']['actions'] = array(
    '#type' => 'actions',
    '#prefix' => '<div class="list-transitions-setting list-transitions-submit">',
    '#suffix' => '</div>',
  );

  $form['add']['actions']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Add transition'),
  );

  ////
  ////
  //// Transitions table
  ////
  ////

  $form['transitions'] = array(
    '#type' => 'item',
    '#access' => !empty($settings),
    '#title' => t('Transitions'),
    '#theme' => 'list_transitions_table',
    '#list_transitions' => &$settings,
    '#tree' => TRUE,
  );

  foreach ($settings as $name => $transition) {
    $args = array(
      '%from' => $options[$transition->value_from],
      '%to' => $options[$transition->value_to],
    );
    $form['transitions'][$name]['item'] = array(
      '#markup' => t('%from to %to', $args),
    );
    $form['transitions'][$name]['remove'] = array(
      '#type' => 'link',
      '#title' => t('Remove transition'),
      '#href' => "admin/structure/list-transitions/transition/$name/delete",
    );
  }

  ////
  ////
  //// Criterion settings
  ////
  ////

  $form['criterion_title'] = array(
    '#type' => 'item',
    '#access' => !empty($settings),
    '#title' => t('Transition criterion'),
  );

  $form['criterion'] = array(
    '#type' => 'vertical_tabs',
    '#tree' => TRUE,
  );

  // Gets criterion, organizes into group.
  $grouped_criterion = array();
  $criterion = list_transitions_get_criterion($instance);
  foreach ($criterion as $criteria_name => $criteria) {
    $grouped_criterion[$criteria['group']][$criteria_name] = $criteria;
  }

  // Iterates over transitions, adds a fieldset for each one.
  foreach ($settings as $name => $transition) {

    // Adds the fieldset for the transition.
    $args = array(
      '%from' => $options[$transition->value_from],
      '%to' => $options[$transition->value_to],
    );
    $form['criterion'][$name] = array(
      '#type' => 'fieldset',
      '#title' => t('%from to %to', $args),
      '#group' => 'settings',
      '#tree' => TRUE,
    );

    // Iterates over all of the criteria groups.
    foreach ($grouped_criterion as $group_name => $criterion_group) {

      // Displays the group title.
      $form['criterion'][$name][$group_name]['title'] = array(
        '#type' => 'item',
        '#title' => check_plain($group_name),
      );

      // Iterates over criteria in the group.
      foreach ($criterion_group as $criteria_name => $criteria) {
        // Captures criteria form for code readability, adds default settings.
        $criteria_form = &$form['criterion'][$name][$group_name][$criteria_name];
        if (empty($transition->criterion[$criteria_name])) {
          $transition->criterion[$criteria_name] = array('active' => 0);
        }

        $criteria_form['active'] = array(
          '#type' => 'checkbox',
          '#title' => check_plain($criteria['label']),
          '#default_value' => $transition->criterion[$criteria_name]['active'],
        );

        if ($criteria['settings callback']) {

          // Gets arguments, calls settings callback, adds form elements.
          $defaults = array($instance, $transition->criterion[$criteria_name]);
          $arguments = array_merge($defaults, $criteria['settings arguments']);
          $criteria_form += call_user_func_array($criteria['settings callback'], $arguments);

          // Adds the #states property to all settings.
          //criterion[node:invitation:field_invitation_status:pending:accepted][User reference][user_reference][active]
          $dependent_name = "input[name=\"criterion[$name][$group_name][$criteria_name][active]\"]";
          foreach (element_children($criteria_form) as $child_name) {
            if ('active' != $child_name) {
              $criteria_form[$child_name]['#states'] = array(
                'visible' => array(
                  $dependent_name => array('checked' => TRUE),
                ),
              );
            }
          }
        }

      }
    }
  }

  $form['actions'] = array('#type' => 'actions');
  $form['actions']['submit'] = array(
    '#type' => 'submit',
    '#access' => !empty($settings),
    '#value' => t('Save transition settings'),
  );

  $form['#validate'][] = 'list_transitions_criterion_form_validate';
  $form['#submit'][] = 'list_transitions_criterion_form_submit';

  return $form;
}

/**
 * Form validation handler for field_ui_field_edit_form().
 */
function list_transitions_criterion_form_validate($form, &$form_state) {
  $values = &$form_state['values'];
  $add_values = $values['add'];

  if (t('Add transition') == $form_state['clicked_button']['#value']) {
    if ($add_values['from'] == $add_values['to']) {
      $error = t('Transitions to the same value are not allowed.');
      form_set_error('add', $error);
    }
    else {
      unset($add_values['actions']);
      form_set_value($form['processed'], $add_values, $form_state);
    }
  }
}

/**
 * Form submission handler for field_ui_field_edit_form().
 */
function list_transitions_criterion_form_submit($form, &$form_state) {
  $values = $form_state['values'];

  // Redirect the form or add the transition.
  if (t('Add transition') != $form_state['clicked_button']['#value']) {

    // Flattens criterion, saves each transition object with its criterion.
    unset($values['criterion']['criterion__active_tab']);
    foreach ($values['criterion'] as $name => $grouped_criterion) {
      $criterion = array();
      foreach ($grouped_criterion as $group_name => $criterion_group) {
        foreach ($criterion_group as $criteria_name => $criteria_value) {
          $criterion[$criteria_name] = $criteria_value;
        }
      }
      $transition = $form['settings']['#value'][$name];
      $transition->criterion = $criterion;
      ctools_export_crud_save('list_transitions', $transition);
    }

    // Uses the Field UI module's function to determine redirect path.
    module_load_include('inc', 'field_ui', 'field_ui.admin');
    $form_state['redirect'] = field_ui_next_destination($values['entity_type'], $values['bundle_name']);
  }
  else {
    // If we have processed values, save settings.
    if ($values['processed']) {
      // Gets parts for the name.
      $entity_type = $values['entity_type'];
      $bundle_name = $values['bundle_name'];
      $field_name = $values['instance']['field_name'];
      $from = $values['processed']['from'];
      $to = $values['processed']['to'];

      // Builds the configuration object.
      $transition = $settings = ctools_export_crud_new('list_transitions');
      $transition->name = "$entity_type:$bundle_name:$field_name:$from:$to";
      $transition->field_name = $field_name;
      $transition->entity_type = $entity_type;
      $transition->bundle = $bundle_name;
      $transition->value_from = $from;
      $transition->value_to = $to;
      $transition->criterion = array();

      // Saves the settings.
      ctools_export_crud_save('list_transitions', $transition);
    }
  }
}

/**
 * Returns the transition table.
 *
 * @param $variables
 *   An associative array containing:
 *   - element: A render element representing the form.
 *
 * @ingroup themable
 */
function theme_list_transitions_table($variables) {
  $output = '';

  $header = array(
    'transition' => array('data' => t('Transition')),
    'operations' => array('data' => t('Operations')),
  );

  // Builds field options.
  $rows = array();
  foreach ($variables['element']['#list_transitions'] as $name => $transition) {
    $rows[$name] = array(
      'data' => array(
        drupal_render($variables['element'][$name]['item']),
        drupal_render($variables['element'][$name]['remove']),
      ),
    );
  }

  $output .= drupal_render_children($variables['element']);
  $output .= theme('table', array('header' => $header, 'rows' => $rows));

  return $output;
}

/**
 * Form constructor for the delete form.
 *
 * @ingroup forms
 */
function list_transition_delete_form($form, &$form_state, $transition) {
  $entity_info = entity_get_info($transition->entity_type);
  $bundle_info = $entity_info['bundles'][$transition->bundle];
  $path = isset($bundle_info['admin']['real path']) ? $bundle_info['admin']['real path'] : $bundle_info['admin']['path'];

  $form['#list_transitions'] = array(
    'transition' => $transition,
    'redirect' => "$path/fields/{$transition->field_name}/list-transitions",
  );

  $form['text'] = array(
   '#markup' => '<p>' . t('Are you sure you want to remove the transition?') . '</p>',
  );

  $form['actions']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Remove transition'),
  );

  $form['actions']['cancel'] = array(
    '#type' => 'link',
    '#title' => t('Cancel'),
    '#href' => $form['#list_transitions']['redirect'],
    '#attributes' => array('title' => t('Go back to list transitions')),
  );

  $form['#submit'][] = 'list_transition_delete_form_submit';

  return $form;
}

/**
 * Form submission handler for list_transition_delete_form().
 */
function list_transition_delete_form_submit($form, &$form_state) {
  $transition = $form['#list_transitions']['transition'];
  ctools_export_crud_delete('list_transitions', $transition);
  drupal_set_message(t('The transition has been removed.'));
  $form_state['redirect'] = $form['#list_transitions']['redirect'];
}

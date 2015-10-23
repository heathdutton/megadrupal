<?php

/**
 * @file
 * UI admin for RDFviz rules.
 *
 * A nested form array manager.
 */

/**
 * Recursive FAPI fragment builder. menu page callback.
 */
function rdfviz_ruleset_form($ruleset_id = NULL) {
  $ruleset = array(
    'arbitrary' => 'value',
    'nested' => array(
      'more' => 'values',
      'even more' => 'values and',
      'even more more' => 'values',
      'and deeper' => array(
        'super nested' => 'is possible',
      ),
    ),
    'etc' => 'and things',
  );
  $ruleset = rdfviz_node_rendering_ruleset();

  $form_state = array(
    'values' => array(
      'rules' => $ruleset,
    ),
  );
  $form = drupal_build_form('rdfviz_rules_form', $form_state);
  return drupal_render($form);
}

function rdfviz_rules_form($form, &$form_state) {
  $form = array(
    '#tree' => 'true',
  );

  $form['#attached']['css'] = array(
    drupal_get_path('module', 'rdfviz') . '/rdfviz.ui.css',
  );

  foreach ($form_state['values']['rules'] as $key => $val) {
    $form['rules'][$key] = rdfviz_rule_form($key, $val);
  }


  $form['preview'] = array(
    '#type' => 'button',
    '#value' => 'preview',
  );
  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => 'submit',
  );

  return $form;
}

function rdfviz_rule_form($name, $data) {
  $form = array(
    '#type' => 'fieldset',
    '#title' => $name,
  );

  foreach ($data as $key => $val) {
    switch ($key) {
      case 'LABEL':
        $form[$key] = array(
          '#type' => 'textfield',
          '#title' => $key,
          '#default_value' => $val,
        );
        break;

      case 'ON':
        $form[$key] = array(
          '#type' => 'fieldset',
          '#title' => $key,
        );
        $form[$key] += rdfviz_array_as_table_form($val);
        break;

      case 'IF':
        $form[$key] = array(
          '#type' => 'fieldset',
          '#title' => $key,
        );
        // Build a form representing an array of conditions.
        $form[$key] += rdfviz_array_as_table_form($val, 'rdfviz_condition_form');
        break;

      case 'DO':
        $form[$key] = array(
          '#type' => 'fieldset',
          '#title' => $key,
        );
        // Build a form representing an array of actions.
        $form[$key] += rdfviz_array_as_table_form($val, 'rdfviz_action_form');
        break;
    }
  }

  return $form;
}

/**
 * Build form entry for the functional condition checks OR the boolean groupings
 *
 * @param array $data
 *
 * @return array
 */
function rdfviz_condition_form($data = array('predicate_is' => array('value' => 'dc:title'))) {
  $form = array();
  $form['#type'] = 'container';
  foreach ($data as $key => $val) {
    if (in_array($key, array('AND', 'OR', 'NOT'))) {
      $form[$key] = rdfviz_array_as_table_form($val, 'rdfviz_condition_form');
      $form[$key]['#title'] = $key;
    }
    else {
      // An actual condition.
      $form[$key] = array(
        '#type' => 'fieldset',
        '#title' => $key,
      );
      $form[$key] += array_editor_form_fragment($val);
    }
  }
  return $form;
}

function rdfviz_action_form($data = array('addNode' => array('title' => 'Placeholder'))) {
  $form = array();
  $form['#type'] = 'container';
  foreach ($data as $key => $val) {
    // Actions are not recursive.
    // Though they may contain arguments in several shapes.
    $form[$key] = array(
      '#type' => 'fieldset',
      '#title' => $key,
    );
    $form[$key] += array_editor_form_fragment($val);
  }
  return $form;
}

/**
 * Recusive FAPI fragment builder.
 *
 * The form that gets rendered is entirely dependant on the data submitted.
 */
function array_editor_form_fragment($data = array()) {
  // Compile the contents of this data as exposed fields.
  // We often don't know what we are (what our type or name or context is)
  // so it's our parents job to add the #attributes for us.
  $fragment = array(
    '#tree' => TRUE,
  );
  foreach ($data as $key => $val) {
    if (is_array($val)) {
      $fragment[$key] = array(
        '#type' => 'fieldset',
        '#title' => $key,
      );
      $fragment[$key] += array_editor_form_fragment($val);
    }
    else {
      $fragment[$key] = $child = array(
        '#type' => 'textfield',
        '#title' => $key,
        '#default_value' => $val,
      );
    }
  }
  return $fragment;
}

/**
 * Render a list of data as a form.
 *
 * Given an array of data of arbitrary length, list an edit field for each item
 * and provide an add more button and ability to delete items.
 *
 * @param array $data
 * @param string $callback
 *   What function to call to render each of the the immediate children.
 *
 * @return array
 */
function rdfviz_array_as_table_form($data = array(), $callback = 'array_editor_form_fragment') {
  $form = array();
  $form['#type'] = 'container';

  // Get this form fragment to remember what function it's expected to use to
  // build each row.
  $form['#row_callback'] = $callback;

  foreach ($data as $key => $val) {
    if (is_array($val)) {
      $form[$key] = $callback($val);
    }
    else {
      $form[$key] = array(
        '#type' => 'textfield',
        '#default_value' => $val,
      );
    }
  }

  // Add more button.
  // Multiple buttons on the same form don't work unless we set the name
  // explicitly. That needs to be done in #process.

  // Need to take care when overriding #process to merge with the defaults.
  $element_info = element_info('submit');
  $form['add_more'] = array(
    '#type' => 'submit',
    '#value' => '+',
    '#weight' => 100,
    // The add action happens on submit.
    '#submit' => array('rdfviz_table_widget_update'),
    // Need a custom process to set up AJAX right.
    '#ajax' => array(
      'callback' => 'rdfviz_update_form_fragment',
      // Don't know the wrapper id yet.
    ),
  );


  // Need a custom process to set up AJAX right.
  $form['#process'] = array('rdfviz_set_ajax_table_wrapper_process');
  #$form['#process'] = array('rdfviz_add_table_markup');
  return $form;
}

/**
 * A form element #process wrapper to set the correct wrapper target.
 *
 * We don't know the form element id at build time when creating the action
 * button, so set the AJAX wrapper parameter from above later.
 */
function rdfviz_set_ajax_table_wrapper_process($element, &$form_state, $form) {
  // Did not know the id of this wrapper until now.
  foreach (element_children($element) as $child_id) {
    if ($element[$child_id]['#type'] == 'submit') {
      $element[$child_id]['#ajax']['wrapper'] = $element['#id'];
    }
  }
  // Add more button.
  // multiple buttons on the same form don't work unless we set the name
  // explicitly. Needed to wait to here to get the ID also.
  if (isset($element['add_more'])) {
    #    $element['add_more']['#name'] = $element['#id'] . '-add-more';
  }

  return $element;
}

/**
 * Form submission handler for add/remove buttons,
 */
function rdfviz_table_widget_update($form, &$form_state) {
  // Copied from file_field_widget_submit()

  // During the form rebuild, file_field_widget_form() will create field item
  // widget elements using re-indexed deltas, so clear out $form_state['input']
  // to avoid a mismatch between old and new deltas. The rebuilt elements will
  // have #default_value set appropriately for the current state of the field,
  // so nothing is lost in doing this.
  $parents = array_slice($form_state['triggering_element']['#parents'], 0, -1);
  drupal_array_set_nested_value($form_state['input'], $parents, NULL);

  $button = $form_state['triggering_element'];

  // Go one level up in the form, to the widgets container.
  #$element = drupal_array_get_nested_value($form, array_slice($button['#array_parents'], 0, -1));

  $submitted_values = drupal_array_get_nested_value($form_state['values'], array_slice($button['#parents'], 0, -1));
  if ($button['#value'] == '+') {
    $submitted_values[] = array('a' => array('b' => 'c'));
  }
  // Update form_state values.
  drupal_array_set_nested_value($form_state['values'], array_slice($button['#parents'], 0, -1), $submitted_values);

  $form_state['rebuild'] = TRUE;
}

/**
 * An AJAX callback for refreshing a table subform after adding a row.
 *
 * Note, this does not change the data, that happens in the submit callback.
 */
function rdfviz_update_form_fragment($form, &$form_state) {
  $button = $form_state['triggering_element'];
  // Go one level up in the form, to the widgets container.
  $element = drupal_array_get_nested_value($form, array_slice($button['#array_parents'], 0, -1));
  return $element;
}

/**
 * Use #prefix and #suffix to add table tags to the given array structure.
 *
 * Use this as a #process callback.
 * Trying to format the markup at theme time means that the form IDs don't
 * happen right. So had to inject it here.
 *
 * @param $element
 */
function rdfviz_add_table_markup(&$element, &$form_state, $form) {
  $element['#type'] = 'container';
  $element['#prefix'] = '<table>';
  $element['#suffix'] = '</table>';
  foreach (element_children($element) as $rowid) {
    $element[$rowid]['#prefix'] = '<tr>';
    $element[$rowid]['#prefix'] = '</tr>';
    foreach (element_children($element[$rowid]) as $colid) {
      $element[$rowid][$colid]['#prefix'] = '<td>';
      $element[$rowid][$colid]['#suffix'] = '</td>';
    }
  }
  return $element;
}


<?php

/**
 * @file
 * Administration page callbacks for the Anonymous Posting module.
 */

/**
 * Admin Anonymous Posting module form.
 */
function anonymous_posting_admin_form($form, &$form_state) {
  // Prologue:
  $options_ntype = array(); // hold form option(s)
  $options_enabled = array(); // hold default enabled option(s)
  $node_types = node_type_get_types();
  uasort($node_types, create_function('$a,$b', 'return strcmp($a->name, $b->name);'));
  foreach ($node_types as $node_type) {
    $options_ntype[$node_type->type] = $node_type->name;
    if (_anonymous_posting_activated($node_type->type)) {
      $options_ntype[$node_type->type] .= '&nbsp;&nbsp;' . l(t('configure'),
          'admin/structure/types/manage/' . $node_type->type,
          array('query' => drupal_get_destination()));
      $options_enabled[] = $node_type->type;
    }
  }

  // Inputs:
  $form['nodes_type'] = array(
    '#type' => 'checkboxes',
    '#title' => t('Allow anonymous users to submit their <em>contact informations</em>'),
    '#description' => t('<p>Choose to which content type the <em>anonymous contact informations feature</em> must be enabled.</p>' .
      '<p>Dont forget to allow anonymous users to create new content for the content type you choosed. To do so go to the !p page.</p>',
      array('!p' => l(t('Home » Administration » People » Permissions'), 'admin/people/permissions', array('fragment' => 'module-node')))),
    '#options' => $options_ntype,
    '#default_value' => $options_enabled,
  );

  // Epilogue:
  $form['actions']['#type'] = 'actions';
  $form['actions']['submit'] = array('#type' => 'submit', '#value' => t('Save configuration'));

  if (!empty($_POST) && form_get_errors()) {
    drupal_set_message(t('The settings have not been saved because of the errors.'), 'error');
  }
  // By default, render the form using theme_system_settings_form().
  if (!isset($form['#theme'])) {
    $form['#theme'] = 'system_settings_form';
  }

  return $form;
}

/**
 * Process admin anonymous posting module form submission.
 */
function anonymous_posting_admin_form_submit($form, $form_state) {
  
  $choosen_ntype = array();
  foreach ($form_state['values']['nodes_type'] as $key => $value) {
    $instance = field_info_instance('node', ANONYMOUS_POSTING_FIELD_NAME, $key);
    if ($value) {
      $choosen_ntype[] = $key;
      if (empty($instance)) {
        $instance = array(
          'field_name' => ANONYMOUS_POSTING_FIELD_NAME,
          'entity_type' => 'node',
          'bundle' => $key,
          'label' => t('Anonymous contact informations'),
          'widget_type' => 'anonymous_author',
          'display' => array(
            'default' => array('label' => 'hidden', 'type' => 'hidden'),
            'teaser' => array('label' => 'hidden', 'type' => 'hidden'),
          ),
        );
        $instance = field_create_instance($instance);
        watchdog('Anonymous Posting module', 'Create instance field %fname to content type %ntype',
          array('%fname' => ANONYMOUS_POSTING_FIELD_NAME, '%ntype' => $key));
      }
    }
    else {
      // Delete any evantually existing instance of anonymous_author fields
      if (!empty($instance)) {
        field_delete_instance($instance, FALSE);
        watchdog("Anounymous Posting module", 'Deleted instance field %fname from content type %ntype',
          array('%fname' => ANONYMOUS_POSTING_FIELD_NAME, '%ntype' => $key));
      }
    }
  }

  drupal_set_message(t('The configuration options have been saved.'));
}

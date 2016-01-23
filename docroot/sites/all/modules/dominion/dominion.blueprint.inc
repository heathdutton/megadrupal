<?php
/**
 * @file
 * Admin functions for blueprints.
 */

/**
 * Page callback for the blueprint overview.
 */
function dominion_admin_blueprint_page() {
  $header = array(t('Name'), t('Machine name'), t('Operations'));
  $rows = array();
  $items = db_select('dominion_blueprint', 'b')
    ->fields('b', array('blueprint', 'name', 'is_default'))
    ->execute()
    ->fetchAll();
  foreach ($items as $item) {
    $edit = l(t('edit'), 'admin/structure/dominion/settings/blueprint/' . $item->blueprint . '/edit');
    if ($item->is_default) {
      $delete = '';
    }
    else {
      $delete = l(t('delete'), 'admin/structure/dominion/settings/blueprint/' . $item->blueprint . '/delete');
    }
    $rows[] = array(
      check_plain($item->name),
      check_plain($item->blueprint),
      "$edit $delete",
    );
  }
  $output = array();
  $output['info'] = array(
    '#markup' => '<p>' . t('Blueprints are base configurations for subsites.') . '</p>',
  );
  if (!$rows) {
    $output['norows'] = array(
      '#markup' => '<p>' . t('There are no blueprints defined yet.') . '</p>',
    );
  }
  else {
    $output['table'] = array(
      '#theme' => 'table',
      '#header' => $header,
      '#rows' => $rows,
    );
  }
  return $output;
}

/**
 * Blueprint add / edit form.
 */
function dominion_blueprint_form($form, &$form_state, $blueprint = NULL) {
  $form['#blueprint'] = $blueprint;
  
  if ($blueprint) {
    if (!$config = dominion_blueprint_load($blueprint)) {
      drupal_not_found();
      exit;
    }
  }
  else {
    $config = new stdClass();
    $config->blueprint = '';
    $config->name = '';
    $config->default_functions = array();
    $config->allow_functions_override = TRUE;
    $config->is_default = FALSE;
    $config->allow_tld = TRUE;
    $config->default_frontpage = '';
    $config->allow_custom_frontpage = TRUE;
    $config->frontpage_nodetype = '';
  }

  $form['name'] = array(
    '#type' => 'textfield',
    '#title' => t('Name'),
    '#default_value' => $config->name,
    '#description' => t('Human readable name for this blueprint.'),
  );
  
  $form['blueprint'] = array(
    '#type' => 'machine_name',
    '#title' => t('Machine name'),
    '#default_value' => $config->blueprint,
    '#maxlength' => 32,
    '#description' => t('Unique name for the blueprint. It must only contain lowercase letters, numbers and hyphens.'),
    '#machine_name' => array(
      'source' => array('name'),
      'label' => t('Machine name'),
      'replace_pattern' => '[^a-z0-9-]+',
      'replace' => '-',
      'exists' => 'dominion_blueprint_load',
    ),
  );
  
  $form['is_default'] = array(
    '#type' => 'checkbox',
    '#title' => t('Set as default blueprint'),
    '#default_value' => $config->is_default,
    '#description' => t('The default blueprint will be selected by default when creating new subsites.'),
  );
  
  $form['general'] = array(
    '#type' => 'fieldset',
    '#title' => t('General settings'),
    '#collapsible' => TRUE,
  );
  
  $functions = dominion_get_all_functions();
  if ($functions) {
    $form['general']['default_functions'] = array(
      '#type' => 'checkboxes',
      '#title' => t('Default functions'),
      '#options' => dominion_get_all_functions(),
      '#default_value' => $config->default_functions,
    );
    
    $form['general']['allow_functions_override'] = array(
      '#type' => 'checkbox',
      '#title' => t('Allow editor to override functions'),
      '#default_value' => $config->allow_functions_override,
    );
  }
  
  $form['general']['allow_tld'] = array(
    '#type' => 'checkbox',
    '#title' => t('Allow custom domains for subsites'),
    '#default_value' => $config->allow_tld,
  );

  $form['general']['default_frontpage'] = array(
    '#type' => 'textfield',
    '#title' => t('Default frontpage'),
    '#default_value' => $config->default_frontpage,
    '#description' => t('Internal path. Leave empty to use site default.'),
  );

  $form['general']['allow_custom_frontpage'] = array(
    '#type' => 'checkbox',
    '#title' => t('Allow custom frontpage for subsites'),
    '#default_value' => $config->allow_custom_frontpage,
  );

  $options = array('' => t('Disable')) + node_type_get_names();
  $form['general']['frontpage_nodetype'] = array(
    '#type' => 'select',
    '#title' => t('Nodetype for static frontpage'),
    '#options' => $options,
    '#default_value' => $config->frontpage_nodetype,
  );
  
  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save'),
    '#weight' => 100,
  );

  return $form;
}

/**
 * Submit handler for the blueprint form.
 */
function dominion_blueprint_form_submit(&$form, &$form_state) {
  $blueprint = $form['#blueprint'];
  
  $config = new stdClass();
  $config->blueprint = $form_state['values']['blueprint'];
  $config->name = $form_state['values']['name'];
  $config->is_default = $form_state['values']['is_default'];
  $config->allow_functions_override = !empty($form_state['values']['allow_functions_override']);
  $config->allow_tld = !empty($form_state['values']['allow_tld']);
  $config->default_frontpage = $form_state['values']['default_frontpage'];
  $config->allow_custom_frontpage = !empty($form_state['values']['allow_custom_frontpage']);
  $config->frontpage_nodetype = $form_state['values']['frontpage_nodetype'];
  
  if ($blueprint) {
    drupal_write_record('dominion_blueprint', $config, array('blueprint'));
    db_delete('dominion_blueprint_function')->condition('blueprint', $blueprint)->execute();
  }
  else {
    drupal_write_record('dominion_blueprint', $config);
  }
  
  // Add functions.
  if (!empty($form_state['values']['default_functions'])) {
    foreach ($form_state['values']['default_functions'] as $function => $checked) {
      if ($checked) {
        db_insert('dominion_blueprint_function')->fields(array(
          'blueprint' => $config->blueprint,
          'function' => $function,
        ))->execute();
      }
    }
  }
  
  // If this is the default, disable this setting on all other blueprints.
  if ($config->is_default) {
    db_update('dominion_blueprint')->fields(array(
      'is_default' => 0,
    ))->condition('blueprint', $config->blueprint, '!=');
  }
  
  // Invoke hooks.
  if ($blueprint) {
    module_invoke_all('dominion_blueprint_update', $config->blueprint, $form_state['values']);
  }
  else {
    module_invoke_all('dominion_blueprint_insert', $config->blueprint, $form_state['values']);
  }
  
  // Update the add subsite links.
  menu_rebuild();
  
  drupal_set_message(t('The blueprint has been saved.'));
  $form_state['redirect'] = 'admin/structure/dominion/settings/blueprint';
}

/**
 * Blueprint delete form.
 */
function dominion_blueprint_delete_form($form, &$form_state, $blueprint) {
  $config = db_select('dominion_blueprint', 'b')->fields('b')->condition('b.blueprint', $blueprint)->execute()->fetchObject();
  if (!$config || $config->is_default) {
    drupal_not_found();
    exit;
  }
  return confirm_form(
    array('#blueprint' => $blueprint),
    t('Are you sure you want to delete the blueprint %name?', array('%name' => $config->name)),
    'admin/structure/dominion/settings/blueprint',
    NULL,
    t('Delete')
  );
}

/**
 * Submit handler for blueprint delete form.
 */
function dominion_blueprint_delete_form_submit(&$form, &$form_state) {
  db_delete('dominion_blueprint')->condition('blueprint', $form['#blueprint'])->execute();
  db_delete('dominion_blueprint_function')->condition('blueprint', $form['#blueprint'])->execute();
  
  module_invoke_all('dominion_blueprint_delete', $form['#blueprint']);
  
  // Update the add subsite links.
  menu_rebuild();
  
  drupal_set_message(t('The blueprint has been deleted.'));
  $form_state['redirect'] = 'admin/structure/dominion/settings/blueprint';
}

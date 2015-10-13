<?php

function activetl_settings_form($form, &$form_state) {
  $content_types = node_type_get_types();
  $content_types_options = array();
  foreach ($content_types as $item) {
    $content_types_options[$item->type] = $item->name;
  }
  $form['activetl_content_types'] = array(
    '#title' => t('Content types'),
    '#type' => 'checkboxes',
    '#options' => $content_types_options,
    '#default_value' => variable_get('activetl_content_types', array()),
    '#description' => t('Choose content types to activate taxonomy links'),
  );
  $vocs = taxonomy_get_vocabularies();
  $vocs_options = array();
  foreach ($vocs as $item) {
    $vocs_options[$item->vid] = $item->name;
  }
  $form['activetl_vocs'] = array(
    '#title' => t('Vocabularies'),
    '#type' => 'checkboxes',
    '#options' => $vocs_options,
    '#default_value' => variable_get('activetl_vocs', array()),
    '#description' => t('Choose taxonomy vocabularies, which terms you\'d like to process'),
  );
  $form['activetl_limit'] = array(
    '#title' => t('Limit number of occurrences'),
    '#type' => 'textfield',
    '#element_validate' => array('activetl_validate_integer_not_negative'), 
    '#default_value' => variable_get('activetl_limit', 0),
    '#description' => t('How many matchups will be converted to term links? Default: 0 (unlimited)'),
  );
  $form['activetl_first_case'] = array(
    '#title' => t('First character case insensitivity'),
    '#type' => 'checkbox',
    '#default_value' => variable_get('activetl_first_case', 1),
    '#description' => t('Check to make search is case-insensitive for first character'),
  );
  return system_settings_form($form);
}

function activetl_validate_integer_not_negative($element, &$form_state) {
  $value = $element['#value'];
  if ($value !== '' && (!is_numeric($value) || intval($value) != $value || $value < 0)) {
    form_error($element, t('%name must be a positive (or 0) integer.', array('%name' => $element['#title'])));
  }
}

function activetl_batch_process_form($form, &$forms_state) {
  module_load_include('inc', 'activetl', 'model/activetl.db');
  $content_types = node_type_get_types();
  $activetl_content_types = variable_get('activetl_content_types', array());
  $unprocessed_nodes = activetl_db_unprocessed_nodes_select($activetl_content_types);
  $unprocessed_options = array();
  foreach ($unprocessed_nodes as $item) {
    if (!isset($unprocessed_options[$item->type])) {
      $unprocessed_options[$item->type] = 0;
    }
    $unprocessed_options[$item->type]++;
  }
  $content_types_options = array();
  foreach ($unprocessed_options as $key => $item) {
    $content_types_options[$key] = $content_types[$key]->name . " ($unprocessed_options[$key])";
  }
  $form['unprocessed_nodes_count'] = array(
    '#type' => 'item',
    '#markup' => t('There are !count nodes unprocessed', array('!count' => count($unprocessed_nodes))),
  );
  if (!empty($content_types_options)) {
    $form['content_types_to_process'] = array(
      '#title' => t('Content types to process'),
      '#type' => 'checkboxes',
      '#options' => $content_types_options,
      '#description' => t('Choose content types you\'d like to process via active taxonomy links'),
    );
  }
  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Process nodes'),
  );
  return $form;
}

function activetl_batch_process_form_submit($form, $form_state) {
  module_load_include('inc', 'activetl', 'includes/activetl.api');
  if (isset($form_state['values']['content_types_to_process'])) {
    $content_types_to_process = $form_state['values']['content_types_to_process'];
    $fields_to_process = array();
    foreach (field_info_instances('node', $content_types_to_process) as $key => $item) {
      $field_info = field_info_field($key);
      if ($field_info['module'] == 'text') {
        $fields_to_process[] = $key;
      }
    }
    $unprocessed_nodes = activetl_db_unprocessed_nodes_select($content_types_to_process);
    $unprocessed_nodes_nids = array();
    foreach ($unprocessed_nodes as $item) {
      $item = (array)$item;
      $unprocessed_nodes_nids[] = $item['nid'];
    }
    if (!empty($unprocessed_nodes_nids)) {
      $nodes = node_load_multiple($unprocessed_nodes_nids);
      foreach ($nodes as $node) {
        foreach ($fields_to_process as $field) {
          activetl_node_process($node, $field);
        }
      }
    }
  }
  else {
    drupal_set_message(t('No nodes to process.'));
  }
}

function activetl_batch_unprocess_form($form, &$form_state) {
  module_load_include('inc', 'activetl', 'model/activetl.db');
  $content_types = node_type_get_types();
  $processed_nodes_info = activetl_db_processed_nodes_info_select();
  $processed_options = array();
  foreach ($processed_nodes_info as $item) {
    if (!isset($processed_options[$item->type])) {
      $processed_options[$item->type] = 0;
    }
    $processed_options[$item->type]++;
  }
  $content_types_options = array();
  foreach ($processed_options as $key => $item) {
    $content_types_options[$key] = $content_types[$key]->name . " ($processed_options[$key] fields)";
  }
  $form['processed_nodes_count'] = array(
    '#type' => 'item',
    '#markup' => t('There are some nodes processed by active taxonomy links'),
  );
  if (!empty($content_types_options)) {
    $form['content_types_to_unprocess'] = array(
      '#title' => t('Content types to unprocess'),
      '#type' => 'checkboxes',
      '#options' => $content_types_options,
      '#description' => t('Choose content types you\'d like to unprocess'),
    );
  }
  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Unprocess nodes'),
  );
  return $form;
}

function activetl_batch_unprocess_form_submit($form, $form_state) {
  if (isset($form_state['values']['content_types_to_unprocess'])) {
  	module_load_include('inc', 'activetl', 'includes/activetl.api');
    $content_types_to_unprocess = $form_state['values']['content_types_to_unprocess'];
    $processed_nodes_info = activetl_db_processed_nodes_info_select();
    foreach ($processed_nodes_info as $item) {
      if (in_array($item->type, $content_types_to_unprocess, TRUE)) {
        $node = node_load($item->nid);
        $field = $item->field;
        activetl_node_unprocess($node, $field);
      }
    }
  }
  else {
    drupal_set_message(t('No nodes to unprocess.'));
  }
}
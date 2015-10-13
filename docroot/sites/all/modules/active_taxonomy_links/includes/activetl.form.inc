<?php

function activetl_node_form_edit(&$form, &$form_state) {	
  $node = $form_state['node'];
  if (isset($node->nid)) {
    $fields_to_process = array();
    foreach (field_info_instances('node', $node->type) as $key => $item) {
      $field_info = field_info_field($key);
      if ($field_info['module'] == 'text') {
      	module_load_include('inc', 'activetl', 'model/activetl.db');
        $original_info = activetl_db_node_info_select($node, $key);
        if ($original_info) {
          $form[$original_info['field']][$node->language][0]['#default_value'] = $original_info['value'];
        }
      }
    }
  }
  $form['activetl'] = array(
    '#title' => t('Active taxonomy links'),
    '#type' => 'fieldset',
    '#group' => 'additional_settings',
  );
  $form['activetl']['activetl_process'] = array(
    '#type' => 'checkbox',
    '#title' => t('Process via active taxonomy links'),
    '#default_value' => TRUE,
  );
  $form['actions']['submit']['#submit'][] = 'activetl_node_form_submit';
}
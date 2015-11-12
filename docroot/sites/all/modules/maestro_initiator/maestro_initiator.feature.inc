<?php

/**
 * Implements hook_features_export_options().
 */
function maestro_features_export_options() {
    //first update the guid field in maestro_template to match at import.
  $result = db_select('maestro_template', 'mt')->fields('mt', array('id'))->isNull('guid')->execute();
  foreach ($result as $row) {
    $guid = uniqid('maestro', TRUE);
    db_update('maestro_template')->fields(array('guid' => $guid))
      ->condition('id', $row->id)->execute();
  }
  $result = db_select('maestro_template', 'mt')->fields('mt', array('guid', 'template_name'))
    ->execute()->fetchAllKeyed();
  
  $options = array();
  foreach ($result as $guid => $name) {
    $options[$name . " [id: " . $guid . "]"] = $name;
  }
  return $options;
}

/**
 * Implements hook_features_export().
 */
function maestro_features_export($data, &$export, $module_name) {
  $export['dependencies']['maestro'] = 'maestro';
  $export['dependencies']['maestro_initiator'] = 'maestro_initiator'; //add self.

  foreach ($data as $value) {
    $export['features']['maestro'][$value] = $value;
  }
  return array();
}

/**
 * Implement hook_features_export_render().
 */
function maestro_features_export_render($module_name, $data, $export = NULL) {
  module_load_include('inc', 'maestro', 'maestro.admin');
  //get template id
  foreach (array_keys($data) as $value) {
    if (($pos = strpos($value, '[id:')) !== FALSE) {
      $cpos = strpos($value, ']');
      $guids[] = substr($value, $pos + 5, $cpos - ($pos + 5));
    }
    else {
      $guids[] = $value;
    }
  }
  $result = db_select('maestro_template', 'mt')->fields('mt', array('guid', 'id', 'template_name'))
    ->condition('guid', $guids, 'IN')->execute();
  $code = array();
  $code[] = '$maestro = array();';
  foreach ($result as $row) {
    $tcode = maestro_export($row->id) . "\n" .
      maestro_initiator_export_notifications($row->guid) .  "\n";

    $code[] = "  \$maestro['{$row->guid}'] = array('title' => '" . $row->template_name .
      "', 'code' => "  . features_var_export($tcode) . ");";
  }
  $code[] = "return \$maestro;";
  $code = implode("\n", $code);
  return array('maestro_defaults' => $code);
}

/**
 * get all the notification entries. since machine_name concept is not available
 * we will use the template data offset information. we believe that no two tasks will
 * exactly overlap each other and therefore provide us the uniqueness that is required
 * for identifying a template data.
 */
function maestro_initiator_export_notifications($guid) {
  $code = array('');
  $code[] = "\$guid = '$guid';";
  
      //update the guid & title
  $code[] = "  db_update('maestro_template')->fields(array('guid' => \$guid, 'template_name' => \$mi_title))";
  $code[] = "    ->condition('id', \$templateID)->execute();";
  
  $query = db_select('maestro_template_notification', 'n');
  $query->innerJoin('maestro_template_data', 'd', 'd.id = n.template_data_id');
  $query->innerJoin('maestro_template_variables', 'v', 'v.id = n.notify_id');
  $query->innerjoin('maestro_template', 't', 't.id = d.template_id');
  $query->fields('d', array('offset_left', 'offset_top'))
    ->fields('v', array('variable_name'))
    ->fields('n', array('notify_type', 'notify_by', 'notify_when'));
  $query->condition('guid', $guid);
  $result = $query->execute();
  if (!empty($result)) {
    //1. Load all template variable ids
    $code[] = "\$query = db_select('maestro_template_variables', 'v')->fields('v', array('variable_name', 'id'));";
    $code[] = "\$query->innerjoin('maestro_template', 't', 't.id = v.template_id');";
    $code[] = "\$query->condition('guid', \$guid);";
    $code[] = "\$varids = \$query->execute()->fetchAllKeyed();";
    //2. load all template data ids
    $code[] = "\$query = db_select('maestro_template_data', 'd')->fields('d', array('id', 'offset_left', 'offset_top'));";
    $code[] = "\$query->innerjoin('maestro_template', 't', 't.id = d.template_id');";
    $code[] = "\$query->condition('guid', \$guid);";
    $code[] = "\$result = \$query->execute();";
    $code[] = "\$tdids = array();";
    $code[] = "foreach (\$result as \$row) {";
    $code[] = "  \$tdids[\$row->offset_left][\$row->offset_top] = \$row->id;";
    $code[] = "}";
    
    //3. now insert!
    $code[] = "\$iquery = db_insert('maestro_template_notification')->fields(array('template_data_id', 'notify_type', 'notify_by', 'notify_when', 'notify_id'));";
    foreach ($result as $row) {
      //generate the insert statements
      $code[] = "  \$iquery->values(array(";
      $code[] = "      'template_data_id' => \$tdids[$row->offset_left][$row->offset_top],";
      $code[] = "      'notify_type' => $row->notify_type,"; 
      $code[] = "      'notify_by' => $row->notify_by,";
      $code[] = "      'notify_when' => $row->notify_when,";
      $code[] = "      'notify_id' => \$varids['$row->variable_name']";
      $code[] = "    )";
      $code[] = "  );";
    }
    $code[] = "\$iquery->execute();";
  }
  return implode("\n", $code);
}
 
/**
 * Implements hook_features_enable_feature().
 */
function maestro_features_enable_feature($module_name) {
  maestro_features_rebuild($module_name, FALSE);
}

/**
 * Implements hook_features_disable_feature().
 */
function maestro_features_disable_feature($module_name) {
  $result = features_get_default('maestro', $module_name);
  $titles = array();
  foreach ($result as $data) {
    $titles[] = $data['title'];
  }
  if (!empty($titles)) {
    drupal_set_message(t('Following Maestro Workflows have not been deleted. !workflows',
      array('!workflows' => theme('item_list', array('items' => $titles)))), 'warning');
  }
}

/**
 * Implements function hook_features_revert().
 */
function maestro_features_revert($module_name) {
  maestro_features_rebuild($module_name);
}

/**
 * Implements maestro_features_rebuild().
 */
function maestro_features_rebuild($mi_module_name, $mi_rebuild = TRUE) {
  $mi_maestro_workflows = module_invoke($mi_module_name, 'maestro_defaults');
  $mi_titles = array();
  $mi_errors = array();
  if (!empty($mi_maestro_workflows)) {
    module_load_include('inc', 'maestro', 'maestro.admin');
    
    $mi_result = features_get_default('maestro', $mi_module_name);
    $mi_data = array_keys($mi_result);
    $mi_template_ids = db_select('maestro_template', 'mt')->fields('mt', array('guid', 'id'))
      ->condition('guid', $mi_data, 'IN')->execute()->fetchAllKeyed();
    //get all mapped templates. we will need to update it
    $mi_variables = db_select('variable', 'v')->fields('v', array('name', 'name'))
      ->condition('name', '%maestro_initiator_%', 'LIKE')
      ->execute()->fetchAllKeyed();
    $mi_mapped_ids = array();
    foreach ($mi_variables as $mi_name) {
      $mi_id = variable_get($mi_name, 0);
      if (!empty($mi_id)) {
        $mi_mapped_ids[$mi_id] = $mi_name;
      }
    }
    
    foreach ($mi_maestro_workflows as $mi_guid => $mi_data)  {
      $mi_create = TRUE;
      if (isset($mi_template_ids[$mi_guid])) {
        if ($mi_rebuild) {
        //delete the old one
          maestro_initiator_delete_template($mi_template_ids[$mi_guid]);
        }
        else {
          $mi_create = FALSE;
          $mi_titles[] = $mi_data['title'];
        }
      }
      if ($mi_create) {
        //create new
        $mi_result = maestro_initiator_create_maestro($mi_data['code'], $mi_guid, $mi_data['title'], $mi_template_ids, $mi_mapped_ids);
        if (!empty($mi_result)) {
          $mi_errors[] = $mi_result;
        }
      }
    }
  }
  if (!empty($mi_titles)) {
    drupal_set_message(t('Following Maestro Workflow templates already existed in the database. 
      They have not been updated. To update these templates, use the Revert Components option. !workflows',
      array('!workflows' => theme('item_list', array('items' => $mi_titles)))), 'warning');
  }
  if (!empty($mi_errors)) {
    drupal_set_message(t('Following errors occured during Maestro Workflow creation !errors',
      array('!errors' => theme('item_list', array('items' => $mi_errors)))), 'error');
  }
}

/**
 * Use a separate function so that code of one work flow does not cause problems in
 * another work flow during creation / rebuild / revert 
 */
function maestro_initiator_create_maestro($code, $mi_guid, $mi_title, $mi_template_ids, $mi_mapped_ids) {
  try {
    $mi_ret = eval($code);
    //update mapping $templateID will be set in eval()
    if (isset($mi_template_ids[$mi_guid]) && isset($mi_mapped_ids[$mi_template_ids[$mi_guid]])) {
      variable_set($mi_mapped_ids[$mi_template_ids[$mi_guid]], $templateID);
    }
    return '';
  }
  catch (Exception $mi_ex) {
    watchdog_exception('maestro-import', $mi_ex);
    return t('Failed to import template: %name', array('%name' => $mi_title));
  }
}

/**
 * Deletes a given template
 * This code is copied from maestro_handle_structure_ajax_request() in maestro.admin.inc as
 * this is not a callable function.
 */
function maestro_initiator_delete_template($id) {
  $res = db_query("SELECT id FROM {maestro_template_data} WHERE template_id = :tid", array('tid' => $id));
  foreach ($res as $rec) {
    $query = db_delete('maestro_template_assignment');
    $query->condition('template_data_id', $rec->id, '=');
    $query->execute();

    $query = db_delete('maestro_template_notification');
    $query->condition('template_data_id', $rec->id, '=');
    $query->execute();

    $query = db_delete('maestro_template_data_next_step');
    $query->condition('template_data_from', $rec->id, '=');
    $query->execute();

    $query = db_delete('maestro_template_data');
    $query->condition('id', $rec->id, '=');
    $query->execute();
  }

  $query = db_delete('maestro_template_variables');
  $query->condition('template_id', $id, '=');
  $query->execute();

  $query = db_delete('maestro_template');
  $query->condition('id', $id, '=');
  $query->execute();
}

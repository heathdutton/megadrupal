<?php

/**
 * @file
 * Audit your database for inline PHP in entity fields.
 */

/**
 * Implements hook_drush_command().
 */
function architecture_drush_command() {
  $items = array();
  $items['phpaudit'] = array(
    'description' => "Audit the database for PHP code.",
    'drupal dependencies' => array('architecture'),
    'aliases' => array('apa'),
  );
  return $items;
}

/**
 * Implements hook_drush_help().
 */
function architecture_drush_help($section) {
  switch ($section) {
    case 'drush:phpaudit':
      return dt("Audits your entity fields for hidden PHP in the database.");
  }
}

/**
 * Gets and formats the data.
 */
function drush_architecture_phpaudit() {
  _architecture_batch_process_fields();
}

/**
 * Creates a table of data on the command line.
 *
 * @return array
 *  Array of two arrays:
 *   one for drush_print_table()
 *   one for drush_print_pipe()
 */
function _architecture_list_fields($field) {

  if($field['count'] == 1) {
    $rows[] = array(
      dt('Occurrences of PHP in field'),
      dt('Bundles'),
      dt('Field name'),
      dt('Field type'),
    );
  }

  $field_name = $field['name'];
  $bundle_strs = array();
  foreach ($field['bundles'] as $entity_type => $bundles) {
    $bundle_strs[] = implode(', ', $bundles);
  }
  $php_audit_str = _architecture_scour_fields($field_name . '_value', 'field_data_' . $field_name, '<?php');
  $row = array(
    $php_audit_str,
    implode(' ', $bundle_strs),
    $field_name,
    $field['type'],
  );
  $rows[] = $row;
  $pipe[] = implode(',', $row);
  drush_print_table($rows, TRUE, array(30, 35, 40, 20));
  drush_print_pipe($pipe);
}

/**
 * Queries each field's data table, parses through results for the $needle.
 *
 * @param string $column
 *  Name of the column with the values ("[field_name]_value" for most).
 * @param string $table
 *  Name of the table to query. ("field_data_[field_name]" for most).
 * @param string $needle
 *  The value to search for. E.g. "<?php" to find php opening tags.
 * @return string
 *  Short summary of what was found.
 */
function _architecture_scour_fields($column, $table, $needle) {
  // @todo: add some kind of sql injection protection here.
  // I'm not super worried because a user would have to somehow sneak their
  // malicious code into a field's machine name.
  $column = check_plain($column);
  $table = check_plain($table);
  $needle = check_plain($needle);

  $query = 'SELECT `entity_id`, `entity_type` FROM `' . $table . '`  WHERE `' . $column . '` LIKE \'%' . $needle . '%\'';
  try {
    $result = db_query($query)->fetchAllAssoc('entity_id');
    $result_summary = '';
    foreach ($result as $obj) {
      $id = $obj->entity_id;
      $type = $obj->entity_type;
      // If there are results, give IDs.
      $result_summary .= "$type/$id, ";
    }
    $num = count($result);
    return $num? "$num found: " . trim($result_summary, ', ') . "." : "$num found.";
  }
  catch (Exception $e) {
    // If the field is a weird type, the query'll fail. No big.
    return 'Could not find field contents.';
  }
}

function _architecture_batch_process_fields() {
  $fields = field_info_field_map();
  $operations = array();
  $count = count($names);

  //for every chunk, assign some method to run on that chunk of data
  foreach ($fields as $name => $field) {
    $i++;
    $field['name'] = $name;
    $field['count'] = $i;
    $operations[] = array("_architecture_list_fields", array($field,'details'=> t('(Scouring field @field  of  @count)', array('@field '=>$i, '@count'=>$count))));
  }

  //put all that information into our batch array
  $batch = array(
    'operations' => $operations,
    'title' => t('Scour fields'),
    'init_message' => t('Starting'),
    'error_message' => t('An error occurred'),
    //'finished' => 'mymodule_finished_method'
  );

  //Get the batch process all ready!
  batch_set($batch);
  $batch =& batch_get();

  //Because we are doing this on the back-end, we set progressive to false.
  $batch['progressive'] = TRUE;

  //Start processing the batch operations.
  drush_backend_batch_process();
}
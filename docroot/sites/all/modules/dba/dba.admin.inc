<?php

// TODO: If multiple databases are defined in settings.php, allow connection to
// any of the defined databases.

define('DBA_EXPORT_CSV', 1);
define('DBA_EXPORT_HTML', 2);
define('DBA_EXPORT_MYSQLDUMP', 3);
define('DBA_EXPORT_OOCALC', 4);
define('DBA_EXPORT_OOWRITE', 5);
define('DBA_EXPORT_MSEXCEL', 6);
define('DBA_EXPORT_MSWORD', 7);

/**
 * Module configuration.
 */
function dba_admin_settings() {
  $form = array();

  $keys = dba_list_database_keys();
  $form['active'] = array(
    '#type' => 'fieldset',
    '#title' => t('Administer database'),
  );
  $form['active']['dba_active_database'] = array(
    '#type' => 'select',
    '#options' => $keys,
    '#default_value' => variable_get('dba_active_database', 'default'),
    '#description' => t('Select which database you would like to administer.  You can administer any of the databases in the $databases array typically configured in the settings.php Drupal configuration file.'),
  );
  return system_settings_form($form);
}

function dba_show_active_database() {
  $form = array();
  $keys = dba_list_database_keys();
  if (sizeof($keys) > 1) {
    $form['current_active_database'] = array(
      '#type' => 'markup',
      '#markup' => t('Active database: %active [!change]', array('%active' => variable_get('dba_active_database', 'default'), '!change' => l(t('change'), 'admin/config/development/dba'))),
    );
  }
  return $form;
}

/**
 * Show a list of all tables.
 */
function dba_table_overview($form, &$form_state) {
  $form = dba_show_active_database();

  $conn = db_set_active(variable_get('dba_active_database', 'default'));
  $tables = Database::getConnection('default', variable_get('dba_active_database', 'default'))->schema()->findTables('%');
  db_set_active($conn);
  $form['tables'] = array(
    '#type' => 'select',
    '#title' => t('Select one or more tables'),
    '#options' => $tables,
    '#multiple' => TRUE,
    '#required' => TRUE,
    '#size' => 10,
  );
  // TODO: only enable button if one table is selected
  $form['view'] = array(
    '#type' => 'submit',
    '#value' => t('View'),
  );
  // TODO: only enable button if one or more tables is selected
  $form['describe'] = array(
    '#type' => 'submit',
    '#value' => t('Describe'),
  );
/*
  // TODO: only enable button if one or more tables is selected, and MySQL database
  $form['check'] = array(
    '#type' => 'submit',
    '#value' => t('Check'),
  );
  // TODO: only enable button if one or more tables is selected, and MySQL database
  $form['optimize'] = array(
    '#type' => 'submit',
    '#value' => t('Optimize'),
  );
*/
  // TODO: only enable button if one or more tables is selected
  $form['export'] = array(
    '#type' => 'submit',
    '#value' => t('Export'),
  );
  // TODO: only enable button if one or more tables is selected
  $form['empty'] = array(
    '#type' => 'submit',
    '#value' => t('Empty'),
  );
/*
  // TODO: only enable button if one or more tables is selected
  $form['drop'] = array(
    '#type' => 'submit',
    '#value' => t('Drop'),
  );
*/
  return $form;
}

/**
 * Direct to the correct action.
 */
function dba_table_overview_submit($form, &$form_state) {
  $_SESSION['dba_tables'] = $form_state['input']['tables'];
  $_SESSION['dba_action'] = 'export';
  switch ($form_state['input']['op']) {
    case t('View'):
      drupal_goto('admin/structure/dba/view');
      break;
    case t('Export'):
      drupal_goto('admin/structure/dba/export');
      break;
    case t('Empty'):
      drupal_goto('admin/structure/dba/empty');
      break;
    case t('Describe'):
      drupal_goto('admin/structure/dba/describe');
      break;
    default:
      unset($_SESSION['dba_tables']);
      unset($_SESSION['dba_action']);
      drupal_set_message(t('%op not available, incomplete implementation for %driver database driver.', array('%driver' => db_driver(), '%op' => $form_state['input']['op'])), 'error');
      break;
  }
}

/**
 * View selected tables.
 */
function dba_view_tables() {
  $form = array();
  if ($tables = _dba_active_tables()) {
    $quantity = count($tables);
    if ($quantity == 1) {
      drupal_set_title(t('View !table table', array('!table' => check_plain($tables[0]))));
    }
    else {
      drupal_set_title(t('View tables'));
    }
    foreach ($tables as $table) {
      $form += dba_view_table($table);
    }
  }
  return $form;
}

/**
 * Attempt to get table fields in a db agnositc way.  Simply request one
 * row of the table, and parse the output.  This won't work if the table is
 * empty.
 */
function dba_get_fields($table) {
  $fields = array();
  $conn = db_set_active(variable_get('dba_active_database', 'default'));
  $query = db_select($table)->fields($table)->extend('PagerDefault');
  $rows = $query->limit(1)->execute();
  db_set_active($conn);
  while ($row = $rows->fetchAssoc()) {
    foreach ($row as $key => $value) {
      $fields[$key] = $key;
    }
  }
  return $fields;
}

/**
 *
 */
function dba_view_table($table) {
  static $pager_element = 0;

  $unprefixed_table = dba_strip_prefix($table);

  $fields = dba_get_fields($unprefixed_table);
  $header = array();
  $options = array();
  if (count($fields)) {
    $count = 0;
    foreach ($fields as $field) {
      $header[$field] = array('data' => "$field", 'field' => "$field");
      if (!$count++) {
        // set default sort column as first field
        $header[$field]['sort'] = 'desc';
      }
    }

    // SELECT * FROM $table with pager and tablesort
    $conn = db_set_active(variable_get('dba_active_database', 'default'));
    $query = db_select($unprefixed_table)->fields($unprefixed_table)->extend('PagerDefault')->extend('TableSort')->element($pager_element);
    $rows = $query->limit(25)->orderByHeader($header)->execute();
    db_set_active($conn);
    $count = 0;
    while ($row = $rows->fetchAssoc()) {
      foreach ($fields as $field) {
        $options[$count][$field] = check_plain($row[$field]);
      }
      $count++;
    }
  }
  $form[$table]['rows'] = array(
    '#type' => 'tableselect',
    '#header' => $header,
    '#options' => $options,
    '#empty' => t('The !table table is empty.', array('!table' => $table)),
  );
  $form[$table]['pager'] = array('#markup' => theme('pager', array('tags' => NULL, 'element' => $pager_element)));
  $pager_element++;
  return $form;
}

function dba_view_tables_submit($form, &$form_state) {
  $key = 0;
  while (isset($form_state['input']["table_{$key}"])) {
    $table = $form_state['input']["table_{$key}"];
    $_SESSION["dba_view_fields_{$table}"] = $form_state['input']["{$table}_view_fields"];
    $key++;
  }
}

function dba_strip_prefix($table) {
  global $databases;

  $dba_active_database = variable_get('dba_active_database', 'default');
  if (isset($databases['default'][$dba_active_database]['prefix'])) {
    $prefix = $databases['default'][$dba_active_database]['prefix'];
    $table = preg_replace("/^$prefix/", '', $table);
  }
  return $table;
}

/**
 * Perform an export.
 */
function dba_export_tables() {
  if ($tables = _dba_active_tables()) {
    $quantity = count($tables);
    if ($quantity == 1) {
      $filename = reset($tables);
    }
    else {
      $filename = variable_get('dba_default_filename', 'database');
    }
    $all_tables = db_find_tables('%');
    $form['tables'] = array(
      '#type' => 'fieldset',
      '#title' => t('Tables'),
      '#collapsible' => TRUE,
      '#collapsed' => TRUE,
    );
    $form['tables']['export_tables'] = array(
      '#type' => 'select',
      '#title' => t('Select one or more tables'),
      '#options' => $all_tables,
      '#default_value' => $tables,
      '#multiple' => TRUE,
      '#required' => TRUE,
    );
    $form['filename'] = array(
      '#type' => 'textfield',
      '#title' => t('Filename'),
      '#default_value' => $filename,
      '#size' => 40,
      '#required' => TRUE,
      '#maxlength' => 255,
      '#description' => t("Please specify the filename you wish to give your database export.  A file extension will automatically be added to your filename based on the format of the export you select.  Once you click 'Export' below your web browser will allow you to save the database export to your local computer."),
    );
    $form['format'] = array(
      '#type' => 'radios',
      '#title' => t('Format'),
      '#options' => array(DBA_EXPORT_CSV => t('CSV'), DBA_EXPORT_HTML => t('HTML'), DBA_EXPORT_MYSQLDUMP => t('mysqldump'), DBA_EXPORT_OOCALC => t('OpenOffice Spreadsheet'), DBA_EXPORT_OOWRITE => ('OpenOffice Document'), DBA_EXPORT_MSEXCEL => t('Microsoft Excel'), DBA_EXPORT_MSWORD => t('Microsoft Word')),
      '#default_value' => DBA_EXPORT_CSV,
      '#description' => t("Select the format of your export."),
    );
    // TODO: Only display this option when mysqldump format is selected
    $form['droptable'] = array(
      '#type' => 'checkbox',
      '#title' => t('Add DROP TABLE'),
      '#default_value' => 1,
      '#description' => t('Check this box if you wish to add DROP TABLE IF EXISTS before each table schema in mysqldump formatted exports.  This will allow you to quickly restore from an export without having to manually drop all tables first.'),
    );
    // TODO: Only display this option when CSV format is selected
    $form['header'] = array(
      '#type' => 'checkbox',
      '#title' => t('Include header in export'),
      '#default_value' => 1,
      '#description' => t('Check this box if you wish to include the name of each field as the first row of CSV formatted exports.'),
    );
    $form['tablename'] = array(
      '#type' => 'checkbox',
      '#title' => t('Include table name in export'),
      '#default_value' => 1,
      '#description' => t('Check this box if you wish to include the name of the table before the the first row of HTML, OpenOffice and Microsoft formatted exports.'),
    );
    $form['export'] = array(
      '#type' => 'submit',
      '#value' => t('Export'),
    );
    $form['#redirect'] = 'admin/structure/dba';
    return $form;
  }
  drupal_goto('admin/structure/dba');
}

/**
 * Perform export of one or more tables.
 */
function dba_export_tables_submit($form, &$form_state) {

  // Increase execution time to minimize browser timeouts.
  ini_set('max_execution_time', 300);

  switch ($form_state['input']['format']) {
    case DBA_EXPORT_MYSQLDUMP:
      Header("Content-Type: text/plain");
      Header("Content-Disposition: attachment; filename=". $form_state['input']['filename'] .'.sql');
      echo "-- Drupal dba.module database dump\n\n";
      echo "-- Date: ". format_date(time(), 'large') ."\n\n";
      break;
    case DBA_EXPORT_CSV:
      Header("Content-Type: text/csv");
      Header("Content-Disposition: attachment; filename=". $form_state['input']['filename'] .'.csv');
      break;
    case DBA_EXPORT_HTML:
      Header("Content-Type: text/html; charset=utf-8");
      Header("Content-Disposition: attachment; filename=". $form_state['input']['filename'] .'.html');
      echo "<html>\r\n<head>\r\n<meta http-equiv=\"Content-Type\" content=\"text/html;charset=utf-8\" />\r\n</head>\r\n<body>\r\n";
      break;
    case DBA_EXPORT_OOCALC:
      Header("Content-Type: application/vnd.oasis.opendocument.spreadsheet; charset=utf-8");
      Header("Content-Disposition: attachment; filename=". $form_state['input']['filename'] .'.ods');
      echo "<html>\r\n<head>\r\n<meta http-equiv=\"Content-Type\" content=\"text/html;charset=utf-8\" />\r\n</head>\r\n<body>\r\n";
      break;
    case DBA_EXPORT_OOWRITE:
      Header("Content-Type: application/vnd.oasis.opendocument.text; charset=utf-8");
      Header("Content-Disposition: attachment; filename=". $form_state['input']['filename'] .'.odt');
      echo "<html>\r\n<head>\r\n<meta http-equiv=\"Content-Type\" content=\"text/html;charset=utf-8\" />\r\n</head>\r\n<body>\r\n";
      break;
    case DBA_EXPORT_MSEXCEL:
      Header("Content-Type: application/vnd.ms-excel; charset=utf-8");
      Header("Content-Disposition: attachment; filename=". $form_state['input']['filename'] .'.xls');
      echo "<html>\r\n<head>\r\n<meta http-equiv=\"Content-Type\" content=\"text/html;charset=utf-8\" />\r\n</head>\r\n<body>\r\n";
      break;
    case DBA_EXPORT_MSWORD:
      Header("Content-Type: application/msword; charset=utf-8");
      Header("Content-Disposition: attachment; filename=". $form_state['input']['filename'] .'.doc');
      echo "<html>\r\n<head>\r\n<meta http-equiv=\"Content-Type\" content=\"text/html;charset=utf-8\" />\r\n</head>\r\n<body>\r\n";
      break;
  }

  // Turn off output buffering to minimize browser timeouts.
  while (@ob_end_flush());

  foreach ($form_state['input']['export_tables'] as $table) {
    dba_export_table($table, $form_state['input']);
  }
  switch ($form_state['input']['format']) {
    case DBA_EXPORT_HTML:
    case DBA_EXPORT_OOCALC:
    case DBA_EXPORT_OOWRITE:
    case DBA_EXPORT_MSEXCEL:
    case DBA_EXPORT_MSWORD:
      echo "</body></html>\r\n";
      break;
    case DBA_EXPORT_MYSQLDUMP:
      echo "-- Dump completed on: ". format_date(time(), 'large') ."\n";
      break;
  }
  $table_list = implode(', ', $form_state['input']['export_tables']);
  drupal_set_message(t("Saved %tables to %filename.", array('%filename' => $form_state['input']['filename'], '%tables' => $table_list)));
  unset($_SESSION['dba_tables']);
  unset($_SESSION['dba_action']);
  exit(0);
  
  // TODO: Redirect and then download...
}

function dba_export_table($table, $options = array()) {
  // Determine which fields are currently active
  $checked = NULL;
  if (isset($_SESSION["dba_view_fields_$table"])) {
    $checked = $_SESSION["dba_view_fields_$table"];
    if (!count($checked)) {
      $checked = NULL;
    }
  }

  $unprefixed_table = dba_strip_prefix($table);

  $fields = drupal_schema_fields_sql($unprefixed_table);
  if (count($checked)) {
    $active = array();
    foreach ($fields as $key => $field) {
      $index = $key + 1;
      if (isset($checked[$index]) && $checked[$index] == $index) {
        $active[] = $field;
      }
    }
    $fields = $active;
  }

  $num_fields = sizeof($fields);
  $output = '';

  switch ($options['format']) {
    case DBA_EXPORT_MYSQLDUMP:
      $output = "--\n";
      $output .= "-- Table structure for table '$table'\n";
      $output .= "--\n\n";

      $return = dba_show_create_table($table);
      if ($return !== FALSE) {
        if ($options['droptable']) {
          $output .= "DROP TABLE IF EXISTS $table;\n";
        }
        $output .= $return;
        $output .= ";\n\n";
      }
      else {
        $driver = db_driver();
        $output .= "--\n";
        $output .= "-- ERROR: Unable to dump table structure for $driver tables\n";
        $output .= "--\n\n";
      }

      $output .= "--\n";
      $output .= "-- Dumping data for table '$table'\n";
      $output .= "--\n\n";
  
      break;
    case DBA_EXPORT_CSV:
      if ($options['header']) {
        $output = implode(',', $fields) ."\r\n";
      }
      break;
    case DBA_EXPORT_HTML:
    case DBA_EXPORT_OOCALC:
    case DBA_EXPORT_OOWRITE:
    case DBA_EXPORT_MSEXCEL:
    case DBA_EXPORT_MSWORD:
      if ($options['tablename']) {
        echo "<h2>$table</h2>\r\n";
      }
      $variables = array();
      $variables['header'] = $fields;
      break;
  }

  echo $output;

  // SELECT * FROM $table;
  $query = db_select($unprefixed_table)->fields($unprefixed_table);
  // Add tags to allow query altering
  $rows = $query->addTag('dba')->addTag('export')->execute();
  foreach ($rows as $row) {
    switch ($options['format']) {
      case DBA_EXPORT_MYSQLDUMP:
        $line = "INSERT INTO $table VALUES(";
        $i = 0;
        foreach ($row as $field => $value) {
          $value = addslashes($value);
          $value = preg_replace("/\n/", "\\n", $value);
          $line .= (isset($value)) ? "\"$value\"" : "\"\"";
          $line .= (++$i < $num_fields) ? ',' : ");\n";
        }
        echo $line;
        break;
      case DBA_EXPORT_CSV:
        $values = '';
        foreach ($row as $field => $value) {
          $values[] = '"' . str_replace('"', '""', decode_entities(strip_tags($value))) .'"';
        }
        echo implode(',', $values) ."\r\n";
        break;
      case DBA_EXPORT_HTML:
      case DBA_EXPORT_OOCALC:
      case DBA_EXPORT_OOWRITE:
      case DBA_EXPORT_MSEXCEL:
      case DBA_EXPORT_MSWORD:
        $r = array();
        foreach ($row as $field => $value) {
          $r[]['data'] = decode_entities(strip_tags($value));
        }
        $variables['rows'][] = $r;
    }
  }
  switch ($options['format']) {
    case DBA_EXPORT_MYSQLDUMP:
    case DBA_EXPORT_CSV:
      echo "\n";
      break;
    case DBA_EXPORT_HTML:
    case DBA_EXPORT_OOCALC:
    case DBA_EXPORT_OOWRITE:
    case DBA_EXPORT_MSEXCEL:
    case DBA_EXPORT_MSWORD:
      echo theme('table', $variables) ."\n";
      break;
  }
}

/**
 * Truncate table(s).
 */
function dba_empty_tables() {
  if ($tables = _dba_active_tables()) {
    $form = array();
    $form['empty_tables'] = array(
      '#type' => 'hidden',
      '#value' => serialize($tables),
    );
    return confirm_form($form, t('Are you sure you want to delete all contents from the following tables: "%tables"?', array('%tables' => implode(', ', $tables))), 'admin/structure/dba', t('All data will be truncated from the listed tables.  This action cannot be undone.'), t('Empty'), t('Cancel'));
  }
  drupal_goto('admin/structure/dba');
}

/**
 * Perform export of one or more tables.
 */
function dba_empty_tables_submit($form, &$form_state) {
  $tables = unserialize($form_state['input']['empty_tables']);
  foreach ($tables as $table) {
    $unprefixed_table = dba_strip_prefix($table);
    db_truncate($unprefixed_table)->execute();
    drupal_set_message(t('Emptied table !table.', array('!table' => $table)));
  }
  drupal_goto('admin/structure/dba');
}

/**
 * Describe selected tables.
 */
function dba_describe_tables() {
  $form = array();
  if ($tables = _dba_active_tables()) {
    $quantity = count($tables);
    if ($quantity == 1) {
      drupal_set_title(t('Describe !table table', array('!table' => check_plain($tables[0]))));
    }
    else {
      drupal_set_title(t('Describe tables'));
    }
    foreach ($tables as $table) {
      $form += dba_describe_table($table);
    }
  }
  return $form;
}

/**
 * DESCRIBE table.
 * FIXME: The tableselect is broken.
 */
function dba_describe_table($table) {
  static $pager_element = 0;
  $describe = dba_invoke_driver_specific('describe_table', $table);
  $count = 0;
  $header = array();
  $fields = array();
  $options = array();
  while ($row = $describe->fetchAssoc()) {
    if (!$count) {
      $sort = 0;
      foreach ($row as $key => $value) {
        $fields[$key] = $key;
        $header[$key] = array('data' => "$key", 'field' => "$key");
        if (!$sort++) {
          $header[$key]['sort'] = 'desc';
        }
      }
    }
    foreach ($fields as $field) {
      $options[$count][$field] = check_plain($row[$field]);
    }
    $count++;
  }
  $form[$table]['rows'] = array(
    '#type' => 'tableselect',
    '#header' => $header,
    '#options' => $options,
    '#empty' => t('The !table table has no fields.', array('!table' => $table)),
  );
  $form[$table]['pager'] = array('#markup' => theme('pager', array('tags' => NULL, 'element' => $pager_element)));
  $pager_element++;
  return $form;
}

/**
 * We can't use drupal_get_schema() as we want what is really in the database.
 */
function dba_show_create_table($table) {
  return dba_invoke_driver_specific('show_create_table', $table);
}

/**
 * List all defined database keys.
 */
function dba_list_database_keys() {
  global $databases;
  $keys = array();
  foreach ($databases as $key => $database) {
    $keys[$key] = $key;
  }
  return $keys;
}

function dba_set_active_database() {
  $form = array();

  $keys = dba_list_database_keys();
  if (sizeof($keys) > 1) {
    $form['active'] = array(
      '#type' => 'fieldset',
      '#title' => t('Active database'),
      '#collapsible' => TRUE,
      '#collapsed' => TRUE,
    );
    $form['active']['active_databases'] = array(
      '#type' => 'select',
      '#options' => $keys,
      '#description' => t('You can work with any of the databases in the $databases array typically configured in the settings.php Drupal configuration file.'),
    );
    $form['active']['active_select'] = array(
      '#type' => 'submit',
      '#value' => t('Select'),
      '#validate' => array('dba_set_active_database_validate'),
    );
  }

  return $form;
}

function _dba_active_tables() {
  if (isset($_SESSION['dba_tables'])) {
    return $_SESSION['dba_tables'];
  }
  else {
    return array();
  }
}

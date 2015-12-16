<?php

function shadow_tables_page() {
  $output = '';

  $rows = array();
  $header = array(t('Table'), t('Base table'), t('Queries'), t('Status'), t('Operations'));
  $sql = 'SELECT t.tid, t.db_name, t.base_table, t.ready, (
    SELECT COUNT(*)
    FROM {shadow_query} q
    WHERE q.tid = t.tid
  ) as queries
  FROM {shadow_table} t
  ORDER BY t.db_name';
  $res = db_query($sql);
  foreach ($res as $rec) {
    if ($rec->ready) {
      $status = theme('image', array('path' => drupal_get_path('module', 'shadow') . '/images/ok.png'));
      $status .= ' ' . t('ready');
    }
    else {
      $status = theme('image', array('path' => drupal_get_path('module', 'shadow') . '/images/warning.png'));
      $status .= ' ' . t('needs rebuilding');
    }
    $operations = array();
    $operations[] = l(theme('image', array('path' => drupal_get_path('module', 'shadow') . '/images/rebuild.png', 'title' => t('Rebuild table'), 'alt' => t('rebuild'))), 'admin/structure/shadow/tables/' . $rec->tid . '/rebuild', array('html' => TRUE));
    $operations[] = l(theme('image', array('path' => drupal_get_path('module', 'shadow') . '/images/keys.png', 'title' => t('Edit indexes'), 'alt' => t('indexes'))), 'admin/structure/shadow/tables/' . $rec->tid . '/indexes', array('html' => TRUE));
    $operations[] = l(theme('image', array('path' => drupal_get_path('module', 'shadow') . '/images/minus.png', 'title' => t('Delete table'), 'alt' => t('delete'))), 'admin/structure/shadow/tables/' . $rec->tid . '/delete', array('html' => TRUE));
    $operations = implode(' ', $operations);
    $rows[] = array(
      l($rec->db_name, 'admin/structure/shadow/tables/' . $rec->tid),
      check_plain($rec->base_table),
      $rec->queries,
      $status,
      $operations,
    );
  }
  $output .= theme('table', array(
    'header' => $header,
    'rows' => $rows)
  );
  $output .= theme('pager');
  return $output;
}

function shadow_table_actions_form($form, &$form_state, $tid) {
  _shadow_load_classes();
  
  $form = array();

  $form['#tid'] = $tid;
  
  $sql = 'SELECT tid, db_name, base_table FROM {shadow_table} WHERE tid = :tid';
  if (!$table = db_query($sql, array(':tid' => $tid))->fetchObject()) {
    drupal_not_found();
    return;
  }

  $columns = array();
  $sql = 'SELECT * FROM {shadow_column} WHERE tid = :tid';
  $res = db_query($sql, array(':tid' => $tid));
  foreach ($res as $column) {
    $columns[] = $column;
  }

  $filters = array();
  $sql = 'SELECT * FROM {shadow_filter} WHERE tid = :tid';
  $res = db_query($sql, array(':tid' => $tid));
  foreach ($res as $column) {
    $filters[] = $column;
  }
  
  $sql = 'SELECT COUNT(*) FROM {' . $table->db_name . '}';
  $rows = db_query($sql)->fetchField();
  $base_rows = db_query($sql)->fetchField();
  
  $form['info'] = array(
    '#markup' => '<p>' . t('The table %db_name currently has %rows rows (%base_rows rows in base table).', array('%db_name' => $table->db_name, '%rows' => $rows, '%base_rows' => $base_rows)) . '</p>',
  );
  
  $rows = array();
  $res = db_query ( 'SELECT * FROM {' . $table->db_name . '} LIMIT 0, 5');
  foreach ($res as $rec ) {
    $header = array();
    $row = array();
    foreach ($rec as $name => $value) {
      if (!is_numeric($name)) {
        $header[] = $name;
        $row[] = is_null($value) ? '<em>NULL</em>' : check_plain(substr($value, 0, 32));
      }
    }
    $rows[] = $row;
  }
  if ($rows) {
    $form['table'] = array(
      '#type' => 'markup',
      '#markup' => theme('table', array(
        'header' => $header,
        'rows'   => $rows,
       )) . theme('pager'),
    );
  }

  return $form;
}

function shadow_table_delete_form($form, &$form_state, $tid) {
  $form = array();

  $sql = 'SELECT db_name, base_table FROM {shadow_table} WHERE tid = :tid';
  if (!$table = db_query($sql, array(':tid' => $tid))->fetchObject()) {
    drupal_not_found();
    return;
  }
  $form['#tid'] = $tid;
  $form['#db_name'] = $table->db_name;

  $form['info'] = array(
    '#markup' => '<p>' . t('Are you sure you want to delete the table %db_name?', array('%db_name' => $table->db_name)) . '</p>',
  );

  $form['delete'] = array(
    '#type' => 'submit',
    '#value' => t('Delete table'),
  );

  $form['cancel'] = array(
    '#markup' => l(t('Cancel'), 'admin/structure/shadow/tables/'),
  );

  return $form;
}

function shadow_table_delete_form_submit($form, &$form_state) {
  $sql = 'UPDATE {shadow_query} SET tid = NULL WHERE tid = :tid';
  db_query($sql, array(':tid' => $form['#tid']));
  $sql = 'DELETE FROM {shadow_table} WHERE tid = :tid';
  db_query($sql, array(':tid' => $form['#tid']));
  $sql = 'DELETE FROM {shadow_column} WHERE tid = :tid';
  db_query($sql, array(':tid' => $form['#tid']));
  $sql = 'DELETE FROM {shadow_filter} WHERE tid = :tid';
  db_query($sql, array(':tid' => $form['#tid']));
  $sql = 'DROP TABLE {' . $form['#db_name'] . '}';
  db_query($sql);
  drupal_set_message(t('The table has been deleted.'));
  $form_state['redirect'] = 'admin/structure/shadow';
}

function shadow_table_rebuild_form($form, &$form_state, $tid) {
  $form = array();

  $sql = 'SELECT db_name, base_table FROM {shadow_table} WHERE tid = :tid';
  if (!$table = db_query($sql, array(':tid' => $tid))->fetchObject()) {
    drupal_not_found();
    return;
  }
  $form['#tid'] = $tid;
  $form['#db_name'] = $table->db_name;

  $form['info'] = array(
    '#markup' => '<p>' . t('Are you sure you want to rebuild the table %db_name?', array('%db_name' => $table->db_name)) . '</p>',
  );

  $form['rebuild'] = array(
    '#type' => 'submit',
    '#value' => t('Rebuild table'),
  );

  $form['cancel'] = array(
    '#markup' => l(t('Cancel'), 'admin/structure/shadow/tables'),
  );

  return $form;
}

function shadow_table_rebuild_form_submit($form, &$form_state) {
  _shadow_load_classes();
  module_load_include('inc', 'shadow', 'shadow.index');
  shadow_index_add($form['#tid']);
  drupal_set_message(t('The table has been rebuilt.'));
  $form_state['redirect'] = 'admin/structure/shadow';
}

function shadow_indexes_page($tid) {
  $output = '';

  $rows = array();
  $header = array(t('Index'), t('Operations'));
  $indexes = db_select('shadow_index', 'i')
             ->fields('i', array('iid', 'definition'))
             ->condition('i.tid', $tid)
             ->execute()
             ->fetchAll();
  foreach ($indexes as $index) {
    $operations = array();
    $operations[] = l(theme('image', array('path' => drupal_get_path('module', 'shadow') . '/images/edit.png', 'title' => t('Edit index'), 'alt' => t('edit'))), 'admin/structure/shadow/tables/' . $tid . '/indexes/' . $index->iid . '/edit', array('html' => TRUE));
    $operations[] = l(theme('image', array('path' => drupal_get_path('module', 'shadow') . '/images/minus.png', 'title' => t('Delete index'), 'alt' => t('delete'))), 'admin/structure/shadow/tables/' . $tid . '/indexes/' . $index->iid . '/delete', array('html' => TRUE));
    $operations = implode(' ', $operations);
    $rows[] = array(
      check_plain($index->definition),
      $operations,
    );
  }
  if ($indexes) {
    $output .= theme('table', array(
      'header' => $header,
      'rows' => $rows)
    );
  }
  else {
    $output .= '<p>' . t('There are no indexes defined yet.') . '</p>';
  }
  
  $output .= '<h2>' . t('Create new index') . '</h2>';
  $output .= drupal_render(drupal_get_form('shadow_index_form', $tid));
  
  return $output;
}

function shadow_index_form($form, &$form_state, $tid, $index = NULL) {
  if (!empty($index) && is_numeric($index)) {
    $index = db_select('shadow_index', 'i')
             ->fields('i', array('iid', 'tid', 'definition'))
             ->condition('i.iid', $index)
             ->execute()
             ->fetchObject();
    if (!$index) {
      drupal_not_found();
      module_invoke_all('exit');
      exit;
    }
  }
  
  $form['#tid'] = $tid;
  $form['#index'] = $index;
  
  $form['definition'] = array(
    '#type' => 'textfield',
    '#title' => t('Definition'),
    '#default_value' => empty($index) ? '' : $index->definition,
    '#description' => t('Index definition, i.e.: <em>nid,title(8),created</em>'),
  );
  
  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save'),
  );
  
  return $form;
}

function shadow_index_form_validate($form, &$form_state) {
  _shadow_load_classes();
  try {
    $table = db_select('shadow_table', 't')
             ->fields('t', array('db_name'))
             ->condition('tid', $form['#tid'])
             ->execute()
             ->fetchField();
    if (!$table) {
      throw new Exception('Unknown table');
    }
    if (!$table_schema = drupal_get_schema_unprocessed('shadow', $table)) {
      throw new Exception('Unable to get table schema');
    }
    $index_parser = new ShadowIndex($form_state['values']['definition'], $table_schema);
  }
  catch (Exception $e) {
    form_set_error('definition', $e->getMessage());
  }
}

function shadow_index_form_submit($form, &$form_state) {
  $index = new stdClass;
  $index->tid = $form['#tid'];
  $index->definition = $form_state['values']['definition'];
  if (!empty($form['#index'])) {
    $index->iid = $form['#index']->iid;
    drupal_write_record('shadow_index', $index, array('iid'));
  }
  else {
    drupal_write_record('shadow_index', $index);
  }
  db_update('shadow_table')
    ->fields(array('ready' => 0))
    ->condition('tid', $index->tid)
    ->execute();
  drupal_set_message(t('The index has been saved succesfully. You need to !rebuild the table to activate the index.', array('!rebuild' => l(t('rebuild'), "admin/structure/shadow/tables/{$index->tid}/rebuild"))));
  $form_state['redirect'] = 'admin/structure/shadow/tables/' . $form['#tid'] . '/indexes';
}

function shadow_delete_index_form($form, &$form_state, $tid, $index = NULL) {
  if (!empty($index) && is_numeric($index)) {
    $index = db_select('shadow_index', 'i')
    ->fields('i', array('iid', 'tid', 'definition'))
    ->condition('i.iid', $index)
    ->execute()
    ->fetchObject();
    if (!$index) {
      drupal_not_found();
      module_invoke_all('exit');
      exit;
    }
  }

  $form['#tid'] = $tid;
  $form['#index'] = $index;

  $form['message'] = array(
    '#markup' => '<p>' . t('Are you sure you want to delete the index with definition %definition?', array('%definition' => $index->definition)) . '</p>',
  );

  $form['delete'] = array(
    '#type' => 'submit',
    '#value' => t('Delete'),
  );
  
  $form['cancel'] = array(
    '#markup' => l(t('cancel'), 'admin/structure/shadow/tables/' . $form['#tid'] . '/indexes'),
  );

  return $form;
}

function shadow_delete_index_form_submit($form, &$form_state) {
  db_delete('shadow_index')
    ->condition('iid', $form['#index']->iid)
    ->execute();
  db_update('shadow_table')
    ->fields(array('ready' => 0))
    ->condition('tid', $form['#index']->tid)
    ->execute();
  drupal_set_message(t('The index has been deleted. You need to !rebuild the table to activate the index.', array('!rebuild' => l(t('rebuild'), "admin/structure/shadow/tables/{$form['#tid']}/rebuild"))));
  $form_state['redirect'] = 'admin/structure/shadow/tables/' . $form['#tid'] . '/indexes';
}

function shadow_queries_page() {
  $output = '';

  $rows = array();
  $header = array(t('Guid'), t('Description'), t('Shadow table'), t('Changed'));
  $sql = 'SELECT q.qid, q.guid, q.description, q.query_changed, t.db_name
  FROM {shadow_query} q
  LEFT JOIN {shadow_table} t ON t.tid = q.tid
  ORDER BY q.guid';
  $res = db_query($sql);
  foreach ( $res as $rec) {
    $rows[] = array(
      l($rec->guid, 'admin/structure/shadow/queries/list/' . $rec->qid),
      check_plain($rec->description),
      empty($rec->db_name) ? '<em>' . t('not rewritten') . '</em>' : check_plain($rec->db_name),
      format_date($rec->query_changed, 'short'),
    );
  }
  $output .= theme('table', array(
    'header' => $header,
    'rows' => $rows)
  );
  $output .= theme('pager');
  
  return $output;
}

function shadow_add_query_form($form_state) {
  $form = array();

  $form['guid'] = array(
    '#type' => 'textfield',
    '#title' => t('Global identifier'),
    '#required' => TRUE,
    '#description' => t('Identifier for a query of this kind.'),
  );

  $form['description'] = array(
    '#type' => 'textfield',
    '#title' => t('Short description'),
    '#required' => TRUE,
    '#description' => t('This description is only used for display in the admin section.'),
  );

  $form['query'] = array(
    '#type' => 'textarea',
    '#title' => t('SQL code'),
    '#required' => TRUE,
    '#wysiwyg' => FALSE,
    '#description' => t('Enter the full SQL code while leaving all tokens, values and other decorations intact.'),
   );

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Submit'),
  );

  return $form;
}

function shadow_add_query_form_validate($form, &$form_state) {
  _shadow_load_classes();
  
  $guid = $form_state['values']['guid'];
  $sql = 'SELECT q.qid FROM {shadow_query} q WHERE q.guid = :guid';
  if (db_query($sql, array(':guid' => $guid))->fetchField()) {
    form_set_error('guid', t('The given global identifier is already in use.'));
  }

  $query = new ShadowQuery($form_state['values']['query']);
  $base_table = $query->getBaseTable();
  
  if (empty($base_table)) {
    form_set_error('query', t('Unable to read the base table from query. Please check if this is a valid SELECT query.'));
  }
  if (!_shadow_get_primary_key($base_table)) {
    form_set_error('query', t('The base table %table is not supported because it does not exists or does not have a primary key.', array('%table' => $base_table)));
  }
}

function shadow_add_query_form_submit($form, &$form_state) {
  $query = new stdClass();
  $query->guid = $form_state['values']['guid'];
  $query->description = $form_state['values']['description'];
  $query->query = $form_state['values']['query'];
  $query->query_changed = time();
  $query->last_use = 0;
  drupal_write_record('shadow_query', $query);

  drupal_set_message(t('The query has been saved.'));
  drupal_goto("admin/structure/shadow/queries/list/" . $query->qid . "/new");
}

function shadow_choose_table_form($form, &$form_state, $qid) {
  _shadow_load_classes();
  
  $form = array();
  
  $sql = 'SELECT guid, description, query, tid FROM {shadow_query} WHERE qid = ' . $qid;
  if (!$query = db_query($sql)->fetchObject()){
    drupal_not_found();
    exit;
  }

  $form['#qid'] = $qid;

  $form['query'] = array(
    '#type' => 'textarea',
    '#title' => t('Query'),
    '#disabled' => TRUE,
    '#value' => $query->query,
  );

  $options = array('<none>' => t('Do not rewrite this query'));
  $sql = 'SELECT tid, db_name FROM {shadow_table} ORDER BY db_name ASC';
  $res = db_query($sql);
  foreach ($res as $rec) {
    $options[$rec->tid] = $rec->db_name;
  }
  $options['<new>'] = t('Create a new table');
  $form['table'] = array(
    '#type' => 'radios',
    '#title' => t('Shadow table'),
    '#options' => $options,
    '#required' => TRUE,
    '#default_value' => empty($query->tid) ? '<none>' : $query->tid,
  );

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Submit'),
  );

  return $form;
}

function shadow_choose_table_form_submit($form, &$form_state) {
  $qid = $form['#qid'];
  $tid = $form_state['values']['table'];
  if ($tid == '<new>') {
    $form_state['redirect'] = "admin/structure/shadow/queries/list/$qid/new";
  }
  elseif ($tid == '<none>') {
    $sql = 'UPDATE {shadow_query} SET tid = NULL WHERE qid = :qid';
    db_query($sql, array(':qid' => $qid));
    drupal_set_message(t('The settings has been saved.'));
    $form_state['redirect'] = 'admin/structure/shadow/queries';
  }
  else {
    $sql = 'UPDATE {shadow_query} SET tid = :tid WHERE qid = :qid';
    db_query($sql, array(':tid' => $tid, ':qid' => $qid));
    drupal_set_message(t('The settings has been saved.'));
    $form_state['redirect'] = 'admin/structure/shadow/queries';
  }
}

function shadow_create_table_form($form, &$form_state, $qid) {
  _shadow_load_classes();
  
  $sql = 'SELECT query FROM {shadow_query} WHERE qid = :qid';
  if (!$newSql = db_query($sql, array(':qid' => $qid))->fetchField()) {
    drupal_not_found();
    exit;
  }
  
  $query = new ShadowQuery($newSql);
  $weight_options = array();
  for ($i = 0; $i <= 128; ++$i) {
    $weight_options[$i] = $i;
  }

  $primary_key = _shadow_get_primary_key($query->getBaseTable());
  
  $form['#base_table'] = $query->getBaseTable();
  $form['#primary_key'] = $primary_key;
  $form['#qid'] = $qid;
  
  $sortfields = $query->getSortFields();
  $sortfields[] = '';
  $sortfields[] = '';

  $fields = array_merge($primary_key, $query->getFilterFields());
  $fields = array_filter(array_unique($fields));
  $fields[] = '';
  $fields[] = '';
  
  $filters = $query->getFilters();
  $filters[] = '';
  $filters[] = '';
  
  $table_id = 1;
  while (db_table_exists("shadow_data_$table_id")) {
    ++$table_id;
  }
  $default_tablename = "shadow_data_$table_id";
  
  $form['db_name'] = array(
    '#type' => 'textfield',
    '#title' => t('Tablename'),
    '#default_value' => $default_tablename,
    '#description' => t('Tablename used in database.'),
    '#required' => TRUE,
  );
  
  $form['fields'] = array(
    '#type' => 'fieldset',
    '#title' => t('Fields to include in the table'),
    '#description' => t('These fields are included in the database table. Select the fields which are important for dynamic filtering.'),
    '#collapsible' => FALSE,
    '#theme' => 'shadow_fields',
    'items' => array(),
  );
  
  $c = 0;
  foreach ($fields as $field) {
    ++$c;
    $definition = $field;

    if (empty($field)) {
      $form['fields']['items'][] = array(
        "field$c" => array(
          '#type' => 'checkbox',
          '#title' => t('custom'),
          '#default_value' => TRUE,
          '#disabled' => TRUE,
        ),
        "field_name$c" => array(
          '#type' => 'textfield',
          '#title' => t('Name'),
          '#default_value' => _shadow_clean_name($field),
          '#description' => t('Columnname used in database'),
        ),
        "field_definition$c" => array(
          '#type' => 'textfield',
          '#title' => t('Definition'),
          '#maxlength' => 512,
          '#description' => t('Field definition, i.e.: %example', array('%example' => 'node.uid=users.uid,users.name')),
        ),
        "field_weight$c" => array(
          '#type' => 'select',
          '#title' => t('Weight'),
          '#options' => $weight_options,
          '#default_value' => $c,
          '#attributes' => array('class' => array('shadow-fields-weight')),
        ),
      );
    }
    else {
      $joins = explode(',', $field);
      $field = array_pop($joins);
      $joins = implode(', ', $joins);
      $joins = str_replace('*', ' (' . t('left join') . ')', $joins);
      $default_checked = in_array($field, $primary_key);
      list($field_table, $field_column) = explode('.', $field);
      $schema = drupal_get_schema($field_table);
      if (!is_array($schema) || !isset($schema['fields'][$field_column])) {
        if (!isset($warned)) {
          drupal_set_message(t('Not all fields are shown cause some of them are not found within the schema.'), 'warning');
        }
        $warned = TRUE;
        continue;
      }
      $form['fields']['items'][] = array(
        "field$c" => array(
          '#type' => 'checkbox',
          '#title' => check_plain($field),
          '#default_value' => $default_checked,
          '#disabled' => in_array($field, $primary_key),
        ),
        "field_info$c" => array(
          '#value' => $joins ? t('Joined:') . ' ' . check_plain($joins) : t('Available in base table'),
        ),
        "field_name$c" => array(
          '#type' => 'textfield',
          '#title' => t('Name'),
          '#default_value' => _shadow_clean_name($field),
          '#description' => t('Columnname used in database'),
        ),
        "field_definition$c" => array(
          '#type' => 'hidden',
          '#value' => $definition,
        ),
        "field_weight$c" => array(
          '#type' => 'select',
          '#title' => t('Weight'),
          '#options' => $weight_options,
          '#default_value' => $c,
          '#attributes' => array('class' => array('shadow-fields-weight')),
        ),
      );
    }
  }

  $form['sortfields'] = array(
    '#type' => 'fieldset',
    '#title' => t('Sortfields to include in the table'),
    '#description' => t('These fields are included in the database table. They can be used in the ORDER BY section of the query.'),
    '#collapsible' => FALSE,
    '#theme' => 'shadow_fields',
    'items' => array(),
  );
  $c = 0;
  foreach ($sortfields as $field) {
    ++$c;
    $definition = $field;

    if (empty($field)) {
      $item = array(
        "sortfield$c" => array(
          '#type' => 'checkbox',
          '#title' => t('custom'),
          '#default_value' => TRUE,
          '#disabled' => TRUE,
        ),
        "sortfield_name$c" => array(
          '#type' => 'textfield',
          '#title' => t('Name'),
          '#default_value' => '',
          '#description' => t('Columnname used in database'),
        ),
        "sortfield_definition$c" => array(
          '#type' => 'textfield',
          '#title' => t('Definition'),
          '#maxlength' => 512,
          '#description' => t('Field definition, i.e.: %example', array('%example' => 'node.uid=users.uid,users.name')),
        ),
        "sortfield_weight$c" => array(
          '#type' => 'select',
          '#title' => t('Weight'),
          '#options' => $weight_options,
          '#default_value' => $c,
          '#attributes' => array('class' => array('shadow-fields-weight')),
        ),
        "sortfield_invert$c" => array(
          '#type' => 'checkbox',
          '#title' => t('Invert value'),
          '#default_value' => FALSE,
        ),
        "sortfield_base$c" => array(
          '#type' => 'textfield',
          '#title' => t('Base'),
          '#default_value' => 2147483647,
        ),
      );
    }
    else {
      $definition = preg_replace('/\\*$/', '', $definition);
      $joins = explode(',', $field);
      $field = array_pop($joins);
      $joins = implode(', ', $joins);
      $joins = str_replace('*', ' (' . t('left join') . ')', $joins);
      $field_name = str_replace('*', '', $field);
      $default_base = _shadow_get_default_base($field_name);
      $default_invert = (strstr($field, '*') && $default_base);
      $item = array(
        "sortfield$c" => array(
          '#type' => 'checkbox',
          '#title' => check_plain($field_name),
        ),
        "sortfield_info$c" => array(
          '#value' => $joins ? t('Joined:') . ' ' . check_plain($joins) : t('Available in base table'),
        ),
        "sortfield_name$c" => array(
          '#type' => 'textfield',
          '#title' => t('Name'),
          '#default_value' => _shadow_clean_name($field_name),
          '#description' => t('Columnname used in database'),
        ),
        "sortfield_definition$c" => array(
          '#type' => 'hidden',
          '#value' => $definition,
        ),
        "sortfield_weight$c" => array(
          '#type' => 'select',
          '#title' => t('Weight'),
          '#options' => $weight_options,
          '#default_value' => $c,
          '#attributes' => array('class' => array('shadow-fields-weight')),
        ),
      );
      if ($default_base) {
        $item += array(
          "sortfield_invert$c" => array(
            '#type' => 'checkbox',
            '#title' => t('Invert value'),
            '#default_value' => $default_invert,
          ),
          "sortfield_base$c" => array(
            '#type' => 'textfield',
            '#title' => t('Base'),
            '#default_value' => $default_base,
          ),
        );
      }
    }
    $form['sortfields']['items'][] = $item;
  }

  $form['filters'] = array(
    '#type' => 'fieldset',
    '#title' => t('Enabled filters'),
    '#description' => t('Select the filters which apply to the shadow table. Take care when checking these filters, they may change the output of the query if this filters more rows than the original query.'),
    '#collapsible' => FALSE,
    '#theme' => 'shadow_fields',
    'items' => array(),
  );
  $c = 0;
  foreach ($filters as $filter) {
    ++$c;
    if ($filter) {
      $form['filters']['items'][] = array(
        "filter$c" => array(
          '#type' => 'checkbox',
          '#title' => t('from query'),
        ),
        "filter_definition$c" => array(
          '#type' => 'textfield',
          '#title' => 'definition',
          '#maxlength' => 512,
          '#default_value' => $filter,
          '#description' => t('Field definition, i.e.: %example', array('%example' => 'node.type:\'page\',\'story\'')),
        ),
      );
    }
    else {
      $form['filters']['items'][] = array(
        "filter$c" => array(
          '#type' => 'checkbox',
          '#title' => t('custom'),
          '#default_value' => TRUE,
          '#disabled' => TRUE,
        ),
        "filter_definition$c" => array(
          '#type' => 'textfield',
          '#title' => 'definition',
          '#maxlength' => 512,
          '#description' => t('Field definition, i.e.: %example', array('%example' => 'node.type:\'page\',\'story\'')),
        ),
      );
    }
  }

  $form['build'] = array(
    '#type' => 'checkbox',
    '#title' => t('Fill this table at creation'),
    '#default_value' => TRUE,
  );

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Submit'),
  );

  return $form;
}

function shadow_create_table_form_validate($form, &$form_state) {
  _shadow_load_classes();
  
  $column_parser = new ShadowColumn();
  $filter_parser = new ShadowFilter();
  
  $db_name = $form_state['values']['db_name'];
  if (drupal_get_schema($db_name)) {
    form_set_error('db_name', t('A table with this name already exists.'));
  }
  elseif (!preg_match('/^[a-z][a-z0-9_]*$/', $db_name)) {
    form_set_error('db_name', t('Names may only contain alphanumeric characters and must start with a letter.'));
  }

  $names = array();
  $num_selected = 0;
  foreach ($form_state['values'] as $name => $value) {
    if (!preg_match('/^([a-z]+)(_[a-z]+)?([0-9]+)$/s', $name, $match)) {
      continue;
    }
    $field = $match[1] . $match[2];
    $element = $match[3];
    $definition = $form_state['values'][$match[1] . '_definition' . $element];
    if ( $match[1] != 'filter' ) $field_name = $form_state['values'][$match[1] . '_name' . $element];
    if (empty($definition)) {
      continue;
    }

    switch ($field) {
      case 'field_name':
      case 'sortfield_name':
        if (!empty($value)) {
          if (!preg_match('/^[a-z][a-z0-9_]*$/', $value)) {
            form_set_error($name, t('Names may only contain alphanumeric characters and must start with a letter.'));
          }
          elseif (in_array($value, $names)) {
            form_set_error($name, t('You may not use the same name twice.'));
          }
          $names[] = $value;
        }
        break;
      case 'sortfield_base':
        $max_value = _shadow_get_default_base($definition);
        if (!$max_value) {
          form_set_error($name, t('Invert base is not supported for this field.'));
        }
        else {
          if (!is_numeric($value)) {
            form_set_error($name, t('The base number must be a number.'));
          }
          elseif ($value < 0 || $value > $max_value) {
            form_set_error($name, t('The base number is out of range. The value must be between %min and %max.', array('%min' => 0, '%max' => $max_value)));
          }
        }
        break;
      case 'field':
      case 'sortfield':
        if ($value && !in_array($definition, $form['#primary_key']) && !empty($field_name)) {
          ++$num_selected;
        }
        break;
      case 'filter':
        if ($value) {
          ++$num_selected;
        }
        break;
      case 'field_definition':
      case 'sortfield_definition':
      case 'filter_definition':
        if (!empty($definition)) {
          try {
            if ($field == 'filter_definition') {
              $filter_parser->parse($definition);
            }
            else {
              $column_parser->parse($definition);
            }
          }
          catch (Exception $e) {
            form_set_error($name, t('Syntax error in definition: %error.', array('%error' => $e->getMessage())));
          }
        }
        break;
    }
  }
  
  if (!$num_selected) {
    form_set_error('', t('You must check at least one additional field or filter.'));
  }
}

function shadow_create_table_form_submit($form, &$form_state) {
  _shadow_load_classes();
  
  $db_name = $form_state['values']['db_name'];
  $columns = array();
  $filters = array();

  $table = new stdClass();
  $table->db_name = $db_name;
  $table->base_table = $form['#base_table'];
  $table->ready = 0;
  drupal_write_record('shadow_table', $table);

  foreach ($form_state['values'] as $name => $value) {
    if (!preg_match('/^([a-z]+)(_[a-z]+)?([0-9]+)$/s', $name, $match)) {
      continue;
    }
    $field = $match[1] . $match[2];
    $element = $match[3];
    $definition = $form_state['values'][$match[1] . '_definition' . $element];
    // Check if the checkbox is checked.
    if (empty($form_state['values'][$match[1] . $element])) {
      continue;
    }
    // Check if the definition is empty.
    if (empty($definition)) {
      continue;
    }
    switch ($field) {
      case 'field':
        if (empty($form_state['values']["field_name$element"])) {
          continue;
        }
        $name = $form_state['values']["field_name$element"];
        $definition = $form_state['values']["field_definition$element"];
        $weight = $form_state['values']["field_weight$element"];
        if (!isset($columns[$name])) {
          $columns[$name] = new stdClass();
          $columns[$name]->tid = $table->tid;
          $columns[$name]->db_name = $name;
          $columns[$name]->definition = $definition;
          $columns[$name]->weight = $weight;
        }
        $columns[$name]->index_filter = $weight;
        break;
      case 'sortfield':
        if (empty($form_state['values']["sortfield_name$element"])) {
          continue;
        }
        $name = $form_state['values']["sortfield_name$element"];
        $definition = $form_state['values']["sortfield_definition$element"];
        $weight = $form_state['values']["sortfield_weight$element"] + 128;
        if (isset($form_state['values']["sortfield_weight$element"])) {
          $invert = $form_state['values']["sortfield_invert$element"];
          $base = $form_state['values']["sortfield_base$element"];
        }
        if (!isset($columns[$name])) {
          $columns[$name] = new stdClass();
          $columns[$name]->tid = $table->tid;
          $columns[$name]->db_name = $name;
          $columns[$name]->definition = $definition;
          $columns[$name]->weight = $weight;
        }
        $columns[$name]->index_sort = $weight;
        if ($invert) {
          $columns[$name]->invert_base = $base;
        }
        break;
      case 'filter':
        $definition = $form_state['values']["filter_definition$element"];
        $filters[$name] = new stdClass();
        $filters[$name]->tid = $table->tid;
        $filters[$name]->definition = $definition;
        break;
    }
  }

  foreach ($columns as $column) {
    drupal_write_record('shadow_column', $column);
  }

  foreach ($filters as $filter) {
    drupal_write_record('shadow_filter', $filter);
  }
  
  usort($columns, '_shadow_sort_columns');
  
  module_load_include('inc', 'shadow', 'shadow.schema');
  $definition = shadow_schema_build_table($table, $columns, $filters, array());
  $sql = db_create_table($table->db_name, $definition);

  $sql = 'UPDATE {shadow_query} SET tid = :tid WHERE qid = :qid';
  db_query($sql, array(':tid' => $table->tid, ':qid' => $form['#qid']));

  if ($form_state['values']['build']) {
    usleep(10000); // Wait for INSERT INTO delays...
    module_load_include('inc', 'shadow', 'shadow.index');
    shadow_index_add($table->tid);
  }

  drupal_set_message(t('The new shadow table has been created.'));
  $form_state['redirect'] = 'admin/structure/shadow';
}

function _shadow_sort_columns($a, $b) {
  if ($a->weight == $b->weight) {
    return 0;
  }
  return $a->weight < $b->weight ? -1 : 1;
}

function _shadow_get_default_base($field) {
  // Change field definition to fieldname.
  $field = explode(',', $field);
  $field = array_pop($field);
  $field = str_replace('*', '', $field);

  list($table, $column) = explode('.', $field);
  if (!$schema = drupal_get_schema($table)) {
    return FALSE;
  }
  if (!isset($schema['fields'][$column])) {
    return FALSE;
  }
  $type = $schema['fields'][$column]['type'];
  $unsigned = !empty($schema['fields'][$column]['unsigned']);
  $size = empty($schema['fields'][$column]['size']) ? 'normal' : $schema['fields'][$column]['size'];
  switch ($type) {
    case 'int':
    case 'serial':
      switch ($size) {
        case 'tiny': // 8 bits
          return $unsigned ? 255 : 127;
        case 'small': // 16 bits
          return $unsigned ? 65535 : 32767;
        case 'medium': // 32 bits
        case 'normal': // 32 bits
        default:
          return $unsigned ? 4294967295 : 2147483647;
        case 'big':
          return $unsigned ? 18446744073709551615 : 9223372036854775807;
      }
      break;
    default:
      return FALSE;
  }
}

function _shadow_clean_name($name) {
  $name = strtolower($name);
  $name = preg_replace('/[^a-z0-9]/', '_', $name);
  return $name;
}

/**
 * Theming function for the fields in the create table form.
 *
 * @ingroup themeable
 */
function theme_shadow_fields($variables) {
  $element = $variables['element'];
  
  static $counter = 0;
  ++$counter;
  $id = "shadow-fields-table-$counter";
  
  $header = array(
    t('Field'),
    t('Options'),
  );
  $rows = array();
  foreach ($element['items'] as $item_name => $item) {
    if ($item_name{0} == '#') {
      continue;
    }
    $checkbox = '';
    $options = '';
    $weight = '';
    $c = 0;
    foreach ($item as $name => $field) {
      if ($name{0} == '#') {
        continue;
      }
      ++$c;
      if ($c == 1) {
        $checkbox = drupal_render($field);
      }
      else if (strstr($name, 'weight')) {
        $field[ '#attributes']['class'] = array ("shadow-fields-weight");
        $weight = drupal_render($field);
      }
      else {
        $options .= drupal_render($field);
      }
    }
    if ($weight) {
      $rows[] = array(
        'data' => array($checkbox, $options, $weight),
        'class' => array ('draggable'),
      );
    }
    else {
      $rows[] = array(
        'data' => array($checkbox, $options),
        'class' => array ('draggable'),
      );
    }
  }
  if ($weight) {
    drupal_add_tabledrag($id, 'weight', 'sibling', 'shadow-fields-weight');
    $header[] = t('Weight');
  }
  return theme('table', array (
    'header' => $header,
    'rows' => $rows,
    'attributes' => array ( 'id' => $id ),
    )
  );
}

function shadow_queries_test_form($form, &$form_state) {
  !empty($form_state['storage']) or $form_state['storage'] = array();
  $form = array();
  
  if (!empty($form_state['storage'])) {
    $form['result'] = array(
      '#markup' => shadow_queries_test_form_query_result($form_state['storage']),
    );
  }

  $form['query'] = array(
    '#type' => 'textarea',
    '#title' => t('SQL code'),
    '#required' => TRUE,
    '#wysiwyg' => FALSE,
    '#description' => t('Enter the full SQL code while leaving all tokens, values and other decorations intact.'),
    '#default_value' => empty($form_state['storage']) ? '' : $form_state['storage']['query'],
  );

  $options = array();
  $sql = 'SELECT tid, db_name FROM {shadow_table} ORDER BY db_name ASC';
  $res = db_query($sql);
  while ($rec = $res->fetchAssoc()) {
    $options[$rec['tid']] = $rec['db_name'];
  }
  $form['tables'] = array(
    '#type' => 'checkboxes',
    '#title' => t('Shadow tables'),
    '#options' => $options,
    '#required' => TRUE,
    '#default_value' => empty($form_state['storage']) ? array() : $form_state['storage']['tables'],
  );

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Submit'),
  );

  return $form;
}

function shadow_queries_test_form_submit($form, &$form_state) {
  $form_state['rebuild'] = TRUE;
  $form_state['storage'] = $form_state['values'];
}

function shadow_queries_test_form_query_result($values) {
  _shadow_load_classes();
  
  $query = $values['query'];
  $query_args = array();

  // Profile original query.
  timer_start('shadow');
  try {
    db_query($query, $query_args);
    $ms = timer_read('shadow');
  }
  catch (Exception $e) {
    $ms = '<em>' . t('error') . '</em>';
  }

  $header = array(t('Table'), t('Execution time (ms)'));
  $rows = array();
  $rows[] = array('<em>' . t('Original query') . '</em>', $ms);

  foreach ($values['tables'] as $tid => $checked) {
    if (!$checked) {
      continue;
    }
    $sql = $query;
    $args = $query_args;
    try {
      _shadow_rewrite($sql, $args, $tid);
      if ($sql == $query) {
        $ms = '<em>' . t('not rewritten') . '</em>';
      }
      else {
        timer_start("shadow$tid");
        db_query($sql, $args);
        $ms = timer_read("shadow$tid");
      }
    }
    catch (Exception $e) {
      drupal_set_message($e->getMessage(), 'error');
      $ms = '<em>' . t('error') . '</em>';
    }
    
    $name = db_select('shadow_table', 'st')
      ->fields('st', array('db_name'))
      ->condition('st.tid', $tid)
      ->execute()
      ->fetchField();

    $rows[] = array('<h3>' . check_plain($name) . '</h3>' . nl2br(check_plain($sql)), $ms);
  }
  
  return theme('table', array('header' => $header, 'rows' => $rows));
}

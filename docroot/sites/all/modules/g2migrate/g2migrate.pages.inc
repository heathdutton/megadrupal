<?php

/**
 * @file
 * Define configuration forms for the Gallery migrate module.
 */

/**
 * Describe the admin form for setting paths.
 */
function g2migrate_path_settings_form($form, &$form_state) {
  $form = array();

  $options = array('g2migrate_none' => t('Do not save album information.'));
  foreach (taxonomy_get_vocabularies() as $vid => $vocabulary) {
    $options[$vocabulary->machine_name] = $vocabulary->name;
  }
  $form['g2migrate_vocabulary'] = array(
    '#type' => 'select',
    '#title' => t('Vocabulary'),
    '#description' => t('Select a vocabulary to use for importing your Gallery2 album structure.'),
    '#options' => $options,
    '#default_value' => variable_get('g2migrate_vocabulary', 'g2migrate_album'),
    );

  $options = array();
  foreach (node_type_get_types() as $machine_name => $type) {
    $options[$machine_name] = $type->name;
  }
  $form['g2migrate_node_type'] = array(
    '#type' => 'select',
    '#title' => t('Node type'),
    '#description' => t('Select a node type to use for importing your Gallery2 photos.'),
    '#options' => $options,
    '#default_value' => variable_get('g2migrate_node_type', 'g2migrate_photo'),
    '#required' => TRUE,
  );

  $form['g2migrate_album_view'] = array(
    '#type' => 'select',
    '#title' => t('View for displaying photo albums'),
    '#description' => t('The module provides a view for displaying your
    migrated albums. If you want to use your own view for this, but keep the
    custom breadcrumbs of the provided view, then choose your view from this
    list.'),
    '#options' => views_get_views_as_options(TRUE, 'enabled', NULL, FALSE, TRUE),
    '#default_value' => variable_get('g2migrate_album_view', 'g2migrate_albums'),
    '#required' => TRUE,
  );

  $form['g2migrate_g2data'] = array(
    '#type' => 'textfield',
    '#title' => t('Path to g2data'),
    '#description' => t('Give the absolute path to the g2data directory, which
      is where Gallery2 stores your photos. This should be on your web server.
      Examples: %path or %windows_path.',
      array(
        '%path' => '/home/g2user/g2data',
        '%windows_path' => '/c/Users/g2user/Documents/g2data',
      )
    ),
    '#default_value' => variable_get('g2migrate_g2data', ''),
    '#required' => TRUE,
    );

  $redirect_enabled = module_exists('redirect');
  $redirect_default = variable_get('g2migrate_redirect', '');
  $form['g2migrate_redirect'] = array(
    '#type' => 'textfield',
    '#title' => t('Legacy base path'),
    '#description' => t('If the Redirect module is installed, then you can
    enter here the base path to your existing Gallery2 site. As of this
    writing, you have to apply a patch to the Redirect module: !photo_patch if
    you want to redirect legacy photo pages to your new site, or !album_patch
    if you want to redirect legacy album pages to your new site. As you read
    this, there may be more recent versions of these patches, and one or both
    may have been included in the Redirect module. The current setting gives
    the link !album for the hypothetical MyAlbum item. Example: %path.',
      array(
        '!issue' => l('issue', 'https://drupal.org/node/1116408'),
        '!photo_patch' => l('one', 'https://drupal.org/node/1116408#comment-7603833'),
        '!album_patch' => l('another', 'https://drupal.org/node/1607038#comment-6057284'),
        '!album' => l('album', "$redirect_default/MyAlbum"),
        '%path' => 'gallery/v',
      )
    ),
    '#default_value' => $redirect_default,
    '#disabled' => !$redirect_enabled,
  );

  $readme = drupal_get_path('module', 'g2migrate') . '/README.txt';
  $form['tables'] = array(
    '#type' => 'fieldset',
    '#title' => t('Database tables'),
    '#description' => t(
      'See the Import section of the !readme file. If you plan to import the
      tables using drush or phpMyAdmin, then leave the checkbox selected and
      ignore the rest of this section. If you want this module to import
      tables from SQL dumps, then un-check the box and fill in the required
      information.',
      array('!readme' => l('README', $readme))
    ),
  );

  $form['tables']['g2migrate_skip_import'] = array(
    '#type' => 'checkbox',
    '#title' => t('Skip import'),
    '#default_value' => variable_get('g2migrate_skip_import', 1),
    );

  $prefix = variable_get('g2migrate_prefix', 'g_');
  $form['tables']['g2migrate_prefix'] = array(
    '#type' => 'textfield',
    '#title' => t('Database prefix'),
    '#description' => t('All your Gallery database tables have a common prefix, such as %prefix.', array('%prefix' => 'g_')),
    '#default_value' => $prefix,
    );
  $form['#attached']['js'][]
    = drupal_get_path('module', 'g2migrate') . '/g2migrate.admin.js';

  $tables = array();
  foreach (_g2migrate_tables('') as $table) {
    $tables[] = '<span class="g2migrate-prefix">' . $prefix . '</span>' . $table . '.sql';
  }
  $table_list = theme('item_list', array('items' => $tables));
  $form['tables']['g2migrate_dumpdir'] = array(
    '#type' => 'textfield',
    '#title' => t('Path to SQL directory'),
    '#description' => t(
      'Give the path to the directory containing SQL dump files for these tables from the Gallery2 database: !tables Give the absolute path, which should be on your web server. Example: %path.',
      array(
        '!tables' => $table_list,
        '%path' => '/tmp',
      )
    ),
    '#default_value' => variable_get('g2migrate_dumpdir', ''),
  );

  $form = system_settings_form($form);
  $form['#submit'][] = 'g2migrate_path_settings_form_submit';
  return $form;
}

/**
 * Validation: make sure the paths to the SQL dumps are readable.
 */
function g2migrate_path_settings_form_validate($form, &$form_state) {
  $g2data_path = $form_state['values']['g2migrate_g2data'];
  if ($g2data_path[0] != '/') {
    form_set_error('g2data', t('Please enter an absolute path.'));
  }
  elseif (!is_dir($g2data_path)) {
    form_set_error('g2data', t('g2data directory does not exist.'));
  }
  elseif (!is_readable($g2data_path)) {
    form_set_error('g2data', t('g2data directory is not readable.'));
  }

  // IMPORTANT: Make sure that the table prefix is not malicious.
  $prefix = check_plain(trim($form_state['values']['g2migrate_prefix']));
  form_set_value($form['tables']['g2migrate_prefix'], $prefix, $form_state);

  $path = $form_state['values']['g2migrate_dumpdir'];
  if ($form_state['values']['g2migrate_skip_import']) {
    // Avoid confusion: if we are going to skip the import, then blank out the
    // path to the SQL dump directory.
    form_set_value($form['tables']['g2migrate_dumpdir'], '', $form_state);
  }
  elseif (strlen($path) == 0 || $path[0] != '/') {
    form_set_error('g2migrate_dumpdir', t('Please enter an absolute path.'));
  }
  else {
    // Check that the SQL dump files are readable. The g2migrate_prefix variable
    // has not yet been set, so explicitly pass $prefix as a parameter.
    foreach (_g2migrate_tables($prefix) as $table) {
      $file = "$path/$table.sql";
      if (!is_readable($file)) {
        form_set_error('g2migrate_dumpdir',
          t('The file %file is not readable. Check for typos.',
          array('%file' => $file)));
      }
    }
  }
}

/**
 * Process the path settings form.
 *
 * All the work is done by system_settings_form_submit(). All we do here is
 * direct the user to the next step.
 */
function g2migrate_path_settings_form_submit($form, &$form_state) {
  drupal_set_message(t('Next step: go to the !fields tab.', array('!fields' => l('Fields', 'admin/config/development/gallery2/fields'))));
}

/**
 * Describe the admin form for importing Gallery2 photo albums.
 */
function g2migrate_field_form($form, &$form_state) {
  $form = array();

  $message = t('First, set the node type to use on the previous tab. Next, use the form on this tab to describe which fields to use for the data imported from Gallery2. If you chose the G2&nbsp;Album vocabulary and the Photo content type (g2migrate_photo) on the previous tab, then it is safe to use the defaults on this page.');
  $message .= ' <strong>' . t('If you keep the defaults, you should still submit this form.') . '</strong>';
  $form['help'] = array(
    '#type' => 'markup',
    '#markup' => '<p>' . $message . '</p>',
    );

  // Each form element corresponds to one piece of information being imported.
  // In each case, the available options are the fields attached to the selected
  // content type.
  $vocabulary = variable_get('g2migrate_vocabulary', 'g2migrate_album');
  $type = variable_get('g2migrate_node_type', 'g2migrate_photo');

  $sections = array(
    'taxonomy_term' => array(
      'type' => 'vocabulary',
      'entity' => $vocabulary,
      'default_entity' => 'g2migrate_album',
      'prefix' => 'g2migrate_taxonomy_',
      'default_options' => array(
        'none' => t('Skip import'),
        'name' => t('Name'),
        'description' => t('Description'),
      ),
    ),
    'node' => array(
      'type' => 'content type',
      'entity' => $type,
      'default_entity' => 'g2migrate_photo',
      'prefix' => 'g2migrate_field_',
      'default_options' => array(
        'none' => t('Skip import'),
        'title' => t('Title'),
        'created' => t('Creation timestamp'),
      ),
    ),
  );

  // Vocabulary first.
  foreach ($sections as $entity_type => $info) {
    $sources = _g2migrate_sources($entity_type);
    if ($info['entity'] != $info['default_entity']) {
      $options['none'] = t('Select a field.');
      foreach ($sources as &$source) {
        $source['default'] = 'none';
      }
    }
    if ($entity_type == 'node') {
      $sources['g_originationTimestamp']['default'] = 'created';
    }

    // Add an option for each field available on the selected content type.
    $options = $info['default_options'];
    $fields = field_info_instances($entity_type, $info['entity']);
    foreach ($fields as $machine_name => $field) {
      if ($field['widget']['type'] == 'text_textarea_with_summary') {
        $options["$machine_name:summary"] = t('@label summary', array('@label' => $field['label']));
      }
      $options[$machine_name] = $field['label'];
    }

    $form[$entity_type] = array(
      '#type' => 'fieldset',
      '#collapsible' => TRUE,
      '#title' => t(
        'Select fields from the %bundle @entity.',
        array('%bundle' => $info['entity'], '@entity' => $info['type'])
      ),
      '#description' => t('For each source coming from Gallery2, select the field into which it will be imported.'),
    );

    // Add an option to select the destination for each source field.
    foreach ($sources as $key => $field) {
      $long_key = $info['prefix'] . $key;
      $form[$entity_type][$long_key] = array(
        '#type' => 'select',
        '#title' => $key,
        '#description' => t('Select a %type field.', array('%type' => $field['field_type'])),
        '#options' => $options,
        '#default_value' => variable_get($long_key, $field['default']),
      );
    }
  }

  // Require the user to select a destination field for the image file path.
  $form['node']['g2migrate_field_path']['#required'] = TRUE;
  unset($form['node']['g2migrate_field_path']['#options']['none']);

  // Do not allow the user to choose the album vocabulary here: enforce the
  // choice made on the first tab. The node information is still in $fields.
  $form['node']['g2migrate_field_g_parentid']['#disabled'] = TRUE;
  $form['node']['g2migrate_field_g_parentid']['#access'] = FALSE;
  foreach ($fields as $field => $info) {
    // Look for taxonomy reference fields.
    if ($info['widget']['type'] != 'taxonomy_autocomplete') {
      continue;
    }
    $field_info = field_info_field($field);
    // Find all the vocabularies allowed for this field.
    $allowed_values = array();
    foreach ($field_info['settings']['allowed_values'] as $setting_info) {
      $allowed_values[] = $setting_info['vocabulary'];
    }
    // If $vocabulary is allowed, this is the right one.  I hope no one has two
    // taxonomy fields, each of which allows the album vocabulary.
    if (in_array($vocabulary, $allowed_values)) {
      $form['node']['g2migrate_field_g_parentid']['#default_value'] = $field;
      break;
    }
  }

  $form = system_settings_form($form);

  // If the user ignored the instructions at the top of the page, then repeat
  // them here and disable the form.
  if (!variable_get('g2migrate_node_type', '')) {
    $message = t(
      'Set the node type to use on the !paths tab before submitting this form.',
      array('!paths' => l('Paths', 'admin/config/development/gallery2'))
    );
    drupal_set_message($message, 'warning');
    $form['actions']['submit']['#prefix'] = "<p><strong>$message</strong></p>";
    $form['actions']['submit']['#disabled'] = TRUE;
  }

  $form['#submit'][] = 'g2migrate_field_form_submit';
  return $form;
}

/**
 * Validation: make sure that no field is selected twice.
 */
function g2migrate_field_form_validate($form, &$form_state) {
  $sections = array('taxonomy_term' => 'name', 'node' => 'title');
  foreach ($sections as $entity_type => $required_field) {
    $sources = _g2migrate_sources($entity_type);
    $mapping = array();
    foreach (element_children($form[$entity_type]) as $key) {
      $field = $form_state['values'][$key];
      if ($field == 'none') {
        $short_key = preg_replace('/g2migrate_[^_]*_/', '', $key);
        $message = t(
          'The source %source is not mapped and will not be imported.',
          array('%source' => $short_key)
        );
        drupal_set_message($message, 'warning');
      }
      else {
        $mapping[$key] = $field;
      }
    }
    if (!in_array($required_field, $mapping)) {
      $options = $form[$entity_type][$key]['#options'];
      $message = t(
        'You must map something to %property.',
        array('%property' => $options[$required_field]));
      form_set_error($entity_type, $message);
    }

    $duplicates[] = array();
    foreach (array_count_values($mapping) as $field => $count) {
      if ($count > 1) {
        $duplicates[] = $field;
      }
    }
    foreach ($mapping as $key => $field) {
      if (in_array($field, $duplicates)) {
        form_set_error($key, t('You cannot map more than one source to a single field.'));
      }
    }
  }
}

/**
 * Process the field settings form.
 *
 * All the work is done by system_settings_form_submit(). All we do here is
 * direct the user to the next step.
 */
function g2migrate_field_form_submit($form, &$form_state) {
  drupal_set_message(t('Next step: go to the !import tab.', array('!import' => l('Import', 'admin/config/development/gallery2/import'))));
}

/**
 * Describe the admin form for importing Gallery2 photo albums.
 */
function g2migrate_import_form($form, &$form_state) {
  $form = array();

  $instructions = t('First, set the filesystem paths, node type, and field mappings on the previous tabs. Next, use the form on this tab to import your Gallery2 database tables. If that goes smoothly, proceed to the !migrate page (or use drush from the command line) to import the photos.',
    array('!migrate' => l('Migrate', 'admin/content/migrate'))
  );
  $form['help'] = array(
    '#type' => 'markup',
    '#markup' => '<p>' . $instructions . '</p>',
    );

  $readme = drupal_get_path('module', 'g2migrate') . '/README.txt';
  $form['import'] = array(
    '#type' => 'fieldset',
    '#title' => t('Import tables'),
    '#description' => t(
      'Import database tables from SQL dumps. Instead of using this form, see
      the !readme file for better methods.',
      array('!readme' => l('README', $readme))
    ),
  );

  $form['import']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Import tables'),
  );

  $tables = _g2migrate_tables('g2migrate_');
  $form['import']['tables'] = array(
    '#type' => 'fieldset',
    '#title' => t('Preview'),
    '#description' => '<div>' . _g2migrate_table_preview($tables) . '</div>',
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );

  $form['process'] = array(
    '#type' => 'fieldset',
    '#title' => t('Process tables'),
    '#description' => t('After importing database tables, we need to figure out the full path associted to each album. We do this as a separate step in the hope of avoiding timeout problems.'),
  );

  $form['process']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Process tables'),
  );

  $form['process']['tables'] = array(
    '#type' => 'fieldset',
    '#title' => t('Preview'),
    '#description' => '<div>' . _g2migrate_table_preview('g2migrate_Album') . '</div>',
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );

  // If the user ignored the instructions at the top of the page, then repeat
  // them and disable the submit button(s).
  $skip_import = variable_get('g2migrate_skip_import', 0);
  $sql_dir = variable_get('g2migrate_dumpdir', '');
  $g2_data = variable_get('g2migrate_g2data', '');
  $image_field = variable_get('g2migrate_field_path', '');
  $path_tab = l('Paths', 'admin/config/development/gallery2');

  if ($skip_import) {
    $form['import']['submit']['#disabled'] = TRUE;
  }
  elseif (empty($sql_dir)) {
    $form['import']['submit']['#disabled'] = TRUE;
    $message = t(
      'Set the path to your SQL directory on the !paths tab before submitting this form.',
      array('!paths' => $path_tab)
    );
    drupal_set_message($message, 'warning');
  }

  if (empty($g2_data)) {
    $message = t(
      'Set the path to your %g2data directory on the !paths tab before starting the migration.',
      array(
        '%g2data' => 'g2data',
        '!paths' => $path_tab,
      )
    );
    drupal_set_message($message, 'warning');
    $form['process']['#description'] .= "<p><strong>$message</strong></p>";
    $form['process']['submit']['#disabled'] = TRUE;
  }

  if (empty($image_field)) {
    $message = t(
      'Set the fields to use on the !fields tab before submitting this form.',
      array('!fields' => l('Fields', 'admin/config/development/gallery2/fields'))
    );
    drupal_set_message($message, 'warning');
    $form['process']['#description'] .= "<p><strong>$message</strong></p>";
    $form['process']['submit']['#disabled'] = TRUE;
  }

  return $form;
}

/**
 * Return themed preview of database table(s).
 */
function _g2migrate_table_preview($tables) {
  if (is_string($tables)) {
    $tables = array($tables);
  }

  $summary = '';
  $output = '';

  $names['data']['data'] = array(
    'data' => t('Table'),
    'header' => TRUE,
  );
  $counts['data']['data'] = array(
    'data' => t('Number of rows'),
    'header' => TRUE,
  );
  foreach ($tables as $table_name) {
    $query = db_select($table_name, 'g')->fields('g');

    // Add to the summary table.
    $names['data'][] = $table_name;
    $count = $query->countQuery()->execute()->fetchField();
    $counts['data'][] = $count;
    if ($count == 0) {
      continue;
    }

    // Grab the first five rows.
    $rows = array();
    $result = $query->range(0, 5)->execute();
    foreach ($result as $record) {
      $rows[] = (array) $record;
    }

    // Add a table to the output.
    $output .= theme('table', array(
      'caption' => "<h4>$table_name</h4>",
      'header' => array_keys((array) $record),
      'rows' => $rows,
    ));
  }

  $summary .= theme('table', array(
    'header' => array(),
    'rows' => array($names, $counts),
  ));

  return "$summary\n$output";
}

/**
 * Import tables from SQL dumps and then compute paths for all albums.
 */
function g2migrate_import_form_submit($form, &$form_state) {
  switch ($form_state['values']['op']) {
  case 'Import tables':
    // Import tables from Gallery2.
    $prefix = variable_get('g2migrate_prefix', 'g_');
    foreach (_g2migrate_tables('') as $table) {
      $source_table = "$prefix$table";
      $my_table = "g2migrate_$table";

      // Empty the table.
      db_delete($my_table)->execute();
      drupal_set_message(t('Emptied %table, starting import.',
        array('%table' => $my_table)));

      // Import from the SQL dump.
      $path = variable_get('g2migrate_dumpdir', '');
      $lines = file("$path/$source_table.sql");
      $marker = "/INSERT INTO `$source_table` (\([^)]*\))? *VALUES *\(/";
      foreach ($lines as $sql) {
        if (preg_match($marker, $sql)) {
          db_query(str_replace("`$source_table`", "{$my_table}", $sql));
        }
      }
      drupal_set_message(t('Imported @count rows to the %table table.',
        array(
          '@count' => db_select($my_table, 'g')->fields('g')
            ->countQuery()->execute()->fetchField(),
          '%table' =>$my_table,
        )
      ));
    }
    drupal_set_message(t('Completed importing database tables.'));
    drupal_set_message(t('Next step: click the %process button.', array('%process' => 'Process tables')));
    break;

  case 'Process tables':

    // Add rows to the g2migrate_Album table. For now, path and depth are NULL.
    $result = db_select('g2migrate_Entity', 'e')
      ->fields('e', array('g_id'))
      ->condition('g_entityType', 'GalleryAlbumItem')
      ->orderBy('g_id', 'ASC')
      ->execute();
    // Empty the table.
    db_delete('g2migrate_Album')->execute();
    drupal_set_message(t('Emptied %table, entering basic data.',
      array('%table' => 'g2migrate_Album')));
    // Initialize the query object.
    $query = db_insert('g2migrate_Album')
      ->fields(array('g_id'));
    // Add the values.
    foreach ($result as $album) {
      $query->values(array($album->g_id));
    }
    // Execute the query.
    $query->execute();

    // Set path = '' and depth = 0 for any albums (only one?) with no parents.
    $query = db_select('g2migrate_Album', 'a');
    // Note that join() returns a table alias, not the query object.
    $query->join('g2migrate_ChildEntity', 'c', 'a.g_id = c.g_id');
    $result = $query->fields('a', array('g_id'))
      ->condition('c.g_parentId', 0)
      ->execute();
    foreach ($result as $album) {
      db_update('g2migrate_Album')
        ->fields(array('path' => '', 'depth' => 0))
        ->condition('g_id', $album->g_id)
        ->execute();
    }

    // Recursively set depth and path for all albums.
    drupal_set_message(t('Adding path data to %table.',
      array('%table' => 'g2migrate_Album')));
    $depth = 0;
    while (1) {
      // 'p' is for 'parent'.
      $query = db_select('g2migrate_Album', 'a');
      $query->join('g2migrate_ChildEntity', 'c', 'a.g_id = c.g_id');
      $query->join('g2migrate_Album', 'p', 'c.g_parentId = p.g_id');
      $query->join('g2migrate_FileSystemEntity', 'f', 'a.g_id = f.g_id');
      $result = $query->fields('f', array('g_id', 'g_pathComponent'))
        ->fields('p', array('path'))
        ->condition('p.depth', $depth)
        ->execute();
      // If the result set is empty, then we are done.
      if ($query->countQuery()->execute()->fetchField() == 0) {
        break;
      }
      // Otherwise, increment $depth and update the albums.
      ++$depth;
      foreach ($result as $album) {
        db_update('g2migrate_Album')
          ->condition('g_id', $album->g_id)
          ->fields(array(
            'path' => $album->path . '/' . $album->g_pathComponent,
            'depth' => $depth,
          ))
          ->execute();
      }
    }

    drupal_set_message(t('Processing completed. Added @count rows to the %table table.',
      array(
        '@count' => db_select('g2migrate_Album', 'g')->fields('g')
          ->countQuery()->execute()->fetchField(),
        '%table' => 'g2migrate_Album',
      )
    ));
    drupal_set_message(t('Next step: go to the !migrate page or use drush.', array('!migrate' => l('Migrate', 'admin/content/migrate'))));
    break;
  }

}

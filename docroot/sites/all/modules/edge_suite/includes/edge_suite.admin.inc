<?php
/**
 * @file
 * Edge suite administration functionality.
 */

/**
 * Form constructor for Edge Suite settings.
 */
function edge_suite_settings($form, &$form_state) {

  // Configuration group.
  $form['edge_suite_settings'] = array(
    '#type' => 'fieldset',
    '#title' => t('Configuration'),
  );

  $form['edge_suite_settings']['edge_suite_block_enable'] = array(
    '#type' => 'checkbox',
    '#title' => t('Block usage'),
    '#description' => t('If enabled, a block will be automatically provided for every composition instance.'),
    '#default_value' => variable_get('edge_suite_block_enable', TRUE),
  );

  $form['edge_suite_settings']['edge_suite_max_archive_size'] = array(
    '#type' => 'textfield',
    '#title' => t('Composition size'),
    '#description' => t('Maximum size of the archive in MB. If left empty the size defaults to 8MB.'),
    '#size' => 3,
    '#weight' => 5,
    '#default_value' => variable_get('edge_suite_max_archive_size', 8),
  );

  $form['edge_suite_settings']['edge_suite_xua_compatible'] = array(
    '#type' => 'checkbox',
    '#title' => t('X-UA-Compatible'),
    '#description' => t('Enable X-UA-Compatible with IE=edge. Lets IE use the latest rendering engine.'),
    '#default_value' => variable_get('edge_suite_xua_compatible', FALSE),
  );

  $form['edge_suite_settings']['edge_suite_allowed_asset_formats'] = array(
    '#type' => 'textfield',
    '#title' => t('Asset formats'),
    '#description' => t('Set the file extensions that are allowed for asset files within a composition archive. JavaScript files are always allowed. Comma-separated, e.g. png, jpg, gif'),
    '#default_value' => variable_get('edge_suite_allowed_asset_formats', 'js, png, jpg, gif, svg'),
    '#element_validate' => array('edge_suite_validate_asset_formats'),
  );

  // Rebuild all.
  $form['edge_suite_rebuild'] = array(
    '#type' => 'fieldset',
    '#title' => t('Rebuild'),
  );

  $form['edge_suite_rebuild']['rebuild_all'] = array(
    '#type' => 'submit',
    '#value' => t('Rebuild all definitions'),
    '#submit' => array('edge_suite_comp_rebuild_all'),
  );

  return system_settings_form($form);
}

/**
 * Validation callback for asset formats.
 */
function edge_suite_validate_asset_formats($element, &$form_state) {
  if (!empty($element['#value'])) {
    $element['#value'] = str_replace(' ', '', $element['#value']);
    $form_state['values']['edge_suite_allowed_asset_formats'] = $element['#value'];
    $valid = preg_match('/^([a-zA-Z0-9]{2,6},)*([a-zA-Z0-9]{2,6})$/', $element['#value']);
    if (!$valid) {
      form_error($element, t('Not a valid list of formats.'));
    }
  }
}


/**
 * Administrative form for edge compositions.
 */
function edge_suite_admin_form($form, &$form_state) {
  drupal_add_css(drupal_get_path('module', 'edge_suite') . '/includes/edge_suite_admin.css');

  // Get all available compositions.
  $records = db_select('edge_suite_composition_definition', 'd')
    ->fields('d')
    ->execute();

  // Put together the overview table.
  $header = array(t('Compositions'), t('Operations'));
  $rows = array();
  if ($records->rowCount() > 0) {
    foreach ($records as $record) {
      $def_id = $record->definition_id;
      $comp_def = edge_suite_comp_load_definition($def_id);

      $path_base = 'admin/structure/edge_suite/' . $def_id;
      // TODO: add reupload.
      $def_actions = array(
        l(t('add block'), $path_base . '/add'),
        l(t('rebuild'), $path_base . '/rebuild'),
        l(t('reupload'), $path_base . '/reupload'),
        l(t('delete'), $path_base . '/delete'),
      );

      $def_cells = array();
      $def_cells[] = array(
        'data' => $record->project_name . ' - ' . $record->composition_id,
        'class' => 'es-definition-label',
      );
      $def_cells[] = array(
        'data' => implode(' | ', $def_actions),
        'class' => 'es-definition-operations',
      );
      $rows[] = array(
        'data' => $def_cells,
        'no_striping' => TRUE,
        'class' => array('even', 'definition-row'),
      );


      foreach ($comp_def->instances as $inst_id => $instance) {
        $instance_cells = array();
        $instance_cells[] = array(
          'data' => ucfirst($instance->type) . ' - ' . $instance->label,
          'class' => 'es-instance-label',
        );

        $inst_actions = array(
          l(t('test'), $path_base . '/test/' . $inst_id),
          l(t('edit'), $path_base . '/edit/' . $inst_id),
          l(t('delete'), $path_base . '/delete/' . $inst_id),
        );
        $instance_cells[] = array(
          'data' => implode(' | ', $inst_actions),
          'class' => 'es-instance-operations',
        );
        $rows[] = array(
          'data' => $instance_cells,
          'no_striping' => TRUE,
          'class' => array('instance-row'),
        );
      }
    }
  }
  $table = theme('table', array('header' => $header, 'rows' => $rows));

  // Add the table to the form.
  $form['edge_suite_composition_table'] = array(
    '#markup' => $table,
  );

  // Check if private files are configured, to be able to store archive sources.
  if (file_stream_wrapper_valid_scheme("private")) {
    // Form field to upload a new archive.
    $form['edge_suite_comp_upload'] = array(
      '#type' => 'file',
      '#title' => t('Upload new composition'),
      '#description' => t('Use an Edge Animate OAM file which gets generated when publishing the project with option "Animate Deployment Package".'),
      '#size' => 40,
    );

    // TODO: Add checkbox to allow overwriting of libraries? For now this is
    // possible through a rebuild.
    $form['submit'] = array(
      '#type' => 'submit',
      '#value' => t('Upload'),
    );
  }
  else {
    drupal_set_message(t('Private file directory needs to be <a href="@config">configured</a> to upload compositions.', array('@config' => url('admin/config/media/file-system'))), 'warning');
  }

  return $form;
}

/**
 * Validation handler for the administration form.
 *
 * The uploaded file is checked and the edge composition gets created on
 * success.
 */
function edge_suite_admin_form_validate(&$form, &$form_state) {
  edge_suite_process_archive_upload($form, $form_state);
}

/**
 * Callback to validate and process an uploaded archive.
 */
function edge_suite_process_archive_upload(&$form, &$form_state) {
  $validators = array(
    'file_validate_extensions' => array('zip tar gz oam'),
    'file_validate_size' => array(variable_get('edge_suite_max_archive_size', 8) * 1024 * 1024),
  );


  // Save the file as a temporary file.
  $file = file_save_upload('edge_suite_comp_upload', $validators);
  if ($file === FALSE) {
    form_set_error('edge_suite_comp_upload', t("Failed to upload the archive."));
  }
  elseif ($file !== NULL) {
    // File extension validation is weird, e.g. a JS file test.js gets renamed
    // to test.js.txt, the extension .txt will be added to the validator and the
    // file gets uploaded. Basically we can't be sure we have an archive here so
    // far.
    if (!preg_match('/\.(zip|tar|gz|oam)$/i', $file->filename)) {
      form_set_error('edge_suite_comp_upload', t("Wrong file extension, only zip, tar and gz are allowed."));
      file_delete($file);
    }
    else {
      // Accept file and create.
      $form_state['values']['edge_suite_comp_upload'] = $file;
      $definition_id = 0;
      if (isset($form_state['values']['edge_suite_comp_replace']) && $form_state['values']['edge_suite_comp_replace'] == TRUE && isset($form_state['values']['edge_suite_definition_id']) && $form_state['values']['edge_suite_definition_id'] > 0) {
        $definition_id = $form_state['values']['edge_suite_definition_id'];
      }
      $replace_libs = FALSE;
      $form_state['values']['edge_suite_definition_id'] = edge_suite_comp_create($file, $replace_libs, $definition_id);
    }
  }
}

/**
 * Menu callback for confirmation of edge composition reupload.
 */
function edge_suite_definition_reupload_confirm($form, &$form_state, $definition_id) {
  // Load composition and show confirmation dialog.
  $definition_id = intval($definition_id);
  $definition = edge_suite_comp_load_definition($definition_id);
  $form['edge_suite_definition_id'] = array(
    '#type' => 'value',
    '#value' => $definition_id,
  );

  // Check if private files are configured, to be able to store archive sources.
  if (file_stream_wrapper_valid_scheme("private")) {
    // Form field to upload the new archive.
    $form['edge_suite_comp_upload'] = array(
      '#type' => 'file',
      '#title' => t('Upload new composition'),
      '#description' => t('Use an Edge Animate OAM file which gets generated when publishing the project with option "Animate Deployment Package".'),
      '#size' => 40,
    );
    // TODO: Add checkbox to allow overwriting of libraries? For now this is
    // possible through a rebuild.
  }
  else {
    drupal_set_message(t('Private file directory needs to be <a href="@config">configured</a> to upload compositions.', array('@config' => url('admin/config/media/file-system'))), 'warning');
  }

  return confirm_form($form,
    t('Are you sure you want to reupload %title?', array('%title' => $definition->project_name)),
    'admin/structure/edge_suite',
    t('This action cannot be undone.'),
    t('Reupload'),
    t('Cancel')
  );
}

/**
 * Trigger edge composition reupload.
 */
function edge_suite_definition_reupload_confirm_submit($form, &$form_state) {
  if ($form_state['values']['confirm']) {
    $definition = edge_suite_comp_load_definition($form_state['values']['edge_suite_definition_id']);
    $form_state['values']['edge_suite_comp_replace'] = TRUE;
    edge_suite_process_archive_upload($form, $form_state);

    if ($form_state['values']['edge_suite_definition_id'] > 0) {
      watchdog('edge_suite', 'Reupload of Edge composition %title.', array('%title' => $definition->project_name));
      drupal_set_message(t('Reupload of Edge composition %title was successful.', array('%title' => $definition->project_name)));
    }
    else {
      drupal_set_message(t('Reupload of Edge composition %title was not successful.', array('%title' => $definition->project_name)));
    }
  }
  // Redirect to admin form.
  $form_state['redirect'] = 'admin/structure/edge_suite';
}


/**
 * Menu callback for confirmation of edge composition deletion.
 */
function edge_suite_definition_delete_confirm($form, &$form_state, $definition_id) {
  // Load composition and show confirmation dialog.
  $definition_id = intval($definition_id);
  $definition = edge_suite_comp_load_definition($definition_id);
  $form['edge_suite_definition_id'] = array(
    '#type' => 'value',
    '#value' => $definition_id,
  );
  return confirm_form($form,
    t('Are you sure you want to delete %title?', array('%title' => $definition->project_name)),
    'admin/structure/edge_suite',
    t('This action cannot be undone.'),
    t('Delete'),
    t('Cancel')
  );
}

/**
 * Trigger edge composition deletion.
 */
function edge_suite_definition_delete_confirm_submit($form, &$form_state) {
  if ($form_state['values']['confirm']) {
    // Load composition and delete it.
    $definition = edge_suite_comp_load_definition($form_state['values']['edge_suite_definition_id']);
    $num_deleted = edge_suite_comp_delete_definition($form_state['values']['edge_suite_definition_id']);

    if ($num_deleted > 0) {
      watchdog('edge_suite', 'Deleted Edge composition %title.', array('%title' => $definition->project_name));
      drupal_set_message(t('Edge composition %title has been deleted.', array('%title' => $definition->project_name)));
    }
    else {
      drupal_set_message(t('Edge composition %title has not been deleted.', array('%title' => $definition->project_name)));
    }
  }
  // Redirect to admin form.
  $form_state['redirect'] = 'admin/structure/edge_suite';
}

/**
 * Menu callback for confirmation of edge composition deletion.
 */
function edge_suite_instance_delete_confirm($form, &$form_state, $definition_id, $instance_id) {
  // Load instance and show confirmation dialog.
  $instance_id = intval($instance_id);
  $instance = edge_suite_comp_load_instance($instance_id);
  $form['edge_suite_instance_id'] = array(
    '#type' => 'value',
    '#value' => $instance_id,
  );
  return confirm_form($form,
    t('Are you sure you want to delete the instance %title?', array('%title' => $instance->label)),
    'admin/structure/edge_suite',
    t('This action cannot be undone.'),
    t('Delete'),
    t('Cancel')
  );
}

/**
 * Trigger edge composition deletion.
 */
function edge_suite_instance_delete_confirm_submit($form, &$form_state) {
  if ($form_state['values']['confirm']) {
    // Load composition instance and delete it.
    $instance = edge_suite_comp_load_instance($form_state['values']['edge_suite_instance_id']);
    $num_deleted = edge_suite_comp_delete_instance($form_state['values']['edge_suite_instance_id']);

    if ($num_deleted > 0) {
      watchdog('edge_suite', 'Deleted Edge instance %title.', array('%title' => $instance->label));
      drupal_set_message(t('Edge instance %title has been deleted.', array('%title' => $instance->label)));
    }
    else {
      drupal_set_message(t('Edge instance %title has not been deleted.', array('%title' => $instance->label)));
    }
  }
  // Redirect to admin form.
  $form_state['redirect'] = 'admin/structure/edge_suite';
}

/**
 * Menu callback for confirmation of edge composition rebuild.
 */
function edge_suite_definition_rebuild_confirm($form, &$form_state, $id) {
  // Load composition and show confirmation dialog.
  $id = intval($id);
  $definition = edge_suite_comp_load_definition($id);
  $form['edge_suite_definition_id'] = array(
    '#type' => 'value',
    '#value' => $id,
  );
  $form['edge_suite_replace_libraries'] = array(
    '#type' => 'checkbox',
    '#title' => t('Update/replace shared libraries'),
    '#description' => t('If enabled, the currently shared libraries on the server will be replaced with the libraries form the chosen composition.'),
    '#default_value' => FALSE,
  );
  return confirm_form($form,
    t('Are you sure you want to rebuild %title?', array('%title' => $definition->project_name)),
    'admin/structure/edge_suite',
    t('This action cannot be undone.'),
    t('Rebuild'),
    t('Cancel')
  );
}

/**
 * Trigger edge composition rebuild.
 */
function edge_suite_definition_rebuild_confirm_submit($form, &$form_state) {
  if ($form_state['values']['confirm']) {

    // Check if libraries should be replaced.
    $replace = FALSE;
    if (isset($form_state['values']['edge_suite_replace_libraries']) && $form_state['values']['edge_suite_replace_libraries'] == TRUE) {
      $replace = TRUE;
    }
    // Rebuild composition.
    edge_suite_comp_rebuild($form_state['values']['edge_suite_definition_id'], $replace);

    // Redirect to admin form.
    $form_state['redirect'] = 'admin/structure/edge_suite';
  }
}

/**
 * Form handler for edit instance form.
 */
function edge_suite_instance_form_edit($form, &$form_state, $definition_id, $instance_id) {

  // Validate definition id.
  $definition_id = intval($definition_id);
  $definition = edge_suite_comp_load_definition($definition_id);
  if ($definition == NULL) {
    drupal_set_message(t('Not a valid definition id: @id', array('@id' => $definition_id)), 'error');
    // TODO: watchdog.
    return array();
  }

  $form['definition_id'] = array(
    '#type' => 'value',
    '#value' => $definition_id,
  );

  // Validate instance id. If its valid add it to the form.
  $instance_id = intval($instance_id);
  $instance = NULL;
  if ($instance_id > 0) {
    $instance = edge_suite_comp_load_instance($instance_id);
    if (!isset($instance)) {
      drupal_set_message(t('Not a valid instance id: @id', array('@id' => $instance_id)), 'error');
      // TODO: watchdog.
      return array();
    }
    else {
      $form['instance_id'] = array(
        '#type' => 'value',
        '#value' => $instance_id,
      );
    }
  }

  $form['settings']['label'] = array(
    '#type' => 'textfield',
    '#title' => t('Instance label'),
    '#maxlength' => 64,
    '#description' => t('The label for the instant, internal use only.'),
    '#default_value' => isset($instance->label) ? $instance->label : '',
    '#weight' => 1,
    '#required' => TRUE,
  );

  $form['settings']['type'] = array(
    '#type' => 'value',
    '#value' => 'block',
  );

  $form['config']['embed_mode'] = array(
    '#type' => 'select',
    '#title' => 'Embedding',
    '#options' => array(
      'inline' => 'inline',
      'iframe' => 'iframe',
    ),
    '#description' => t('Inline embedding should be preferred (cleaner embedding). Iframe embedding should be used when experiencing any problems with the composition, especially when running multiple compositions alongside on one page.'),
    '#default_value' => isset($instance->config->iframe) ? $instance->config->iframe : FALSE,
    '#weight' => 3,
  );

  $json_help = t('All keys and strings need to be double quoted. Use the token module for generic data.') . ' ';
  $json_help .= t('The JSON object will be injected into a SCRIPT tag within the scope of the stage DIV.') . ' ';
  $json_help .= t('JSON object example') . ': ' . '{"mytext": "Drupal rocks!", "title" : "[current-page:title]", "slide_delay":10}';

  $json_description = t('Use a JSON object to inject data into the Edge Animate composition, which can then easily be read through the Edge Commons library.');
  $json_description .= t('Resources') . ': ';
  $json_description .= l(t('Edge Commons'), 'http://simonwidjaja.github.com/EdgeCommons/doc/#Injection') . ' / ';
  $json_description .= l(t('Edge Docks'), 'http://edgedocks.com/edge-suite');

  $form['config']['data_injection'] = array(
    '#type' => 'fieldset',
    '#title' => t('Data injection - Edge Commons (experimental)'),
    '#weight' => 4,
    '#collapsible' => TRUE,
    '#collapsed' => !isset($instance->config->json_config) || empty($instance->config->json_config),
    '#description' => $json_description,
  );

  $form['config']['data_injection']['json_config'] = array(
    '#title' => t('JSON object'),
    '#type' => 'textarea',
    '#default_value' => isset($instance->config->json_config) ? $instance->config->json_config : '',
    '#description' => $json_help,
    '#weight' => 5,
  );

  if (module_enable(array('token'))) {
    $form['config']['data_injection']['tokens'] = array(
      '#theme' => 'token_tree',
      '#token_types' => array('node'),
      '#global_types' => TRUE,
      '#click_insert' => TRUE,
      '#weight' => 6,
    );
  }
  $form['actions'] = array('#type' => 'actions');
  $form['actions']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save instance'),
  );

  return $form;
}

/**
 * Validation handler for edit instance form.
 */
function edge_suite_instance_form_edit_validate($form, &$form_state) {
  // Validate JSON.
  if (!empty($form_state['values']['json_config'])) {
    $json_msg = edge_suite_json_validate($form_state['values']['json_config']);
    if (!empty($json_msg)) {
      form_set_error('json_config', $json_msg);
    }
  }
}

/**
 * Submit handler for edit instance form.
 */
function edge_suite_instance_form_edit_submit($form, &$form_state) {
  // TODO: Is the use of checkplain() necessary?
  $inst_record = array(
    'definition_id' => $form_state['values']['definition_id'],
    'instance_id' => $form_state['values']['instance_id'],
    'label' => trim(check_plain($form_state['values']['label'])),
    'type' => $form_state['values']['type'],
    'config' => array(
      'embed_mode' => $form_state['values']['embed_mode'],
      'json_config' => $form_state['values']['json_config'],
    ),
  );
  drupal_write_record('edge_suite_composition_instance', $inst_record, 'instance_id');

  drupal_set_message(t('Instance @label was saved.', array('@label' => $inst_record['label'])));
  $form_state['redirect'] = 'admin/structure/edge_suite';
}

/* ADD INSTANCE FORM */
/**
 * Form handler for add instance form.
 */
function edge_suite_instance_form_add($form, &$form_state, $definition_id) {
  return edge_suite_instance_form_edit($form, $form_state, $definition_id, 0);
}

/**
 * Validation handler for add instance form.
 */
function edge_suite_instance_form_add_validate($form, &$form_state) {
  edge_suite_instance_form_edit_validate($form, $form_state);
}

/**
 * Submit handler for add instance form.
 */
function edge_suite_instance_form_add_submit($form, &$form_state) {
  $inst_record = array(
    'definition_id' => $form_state['values']['definition_id'],
    'label' => trim(check_plain($form_state['values']['label'])),
    'type' => $form_state['values']['type'],
    'config' => array(
      'embed_mode' => $form_state['values']['embed_mode'],
      'json_config' => $form_state['values']['json_config'],
    ),
  );
  drupal_write_record('edge_suite_composition_instance', $inst_record);

  drupal_set_message(t('Instance @label was created.', array('@label' => $inst_record['label'])));
  $form_state['redirect'] = 'admin/structure/edge_suite';

}

/**
 * Validation handler for the block config form.
 */
function edge_suite_json_validate($json) {

  // Check for PHP JSON errors.
  $config = json_decode($json);

  $error_msg = '';
  if (version_compare(phpversion(), '5.3') >= 0) {
    $error = '';
    switch (json_last_error()) {
      case JSON_ERROR_NONE:
        break;

      case JSON_ERROR_DEPTH:
        $error = 'Maximum stack depth exceeded';
        break;

      case JSON_ERROR_STATE_MISMATCH:
        $error = 'Underflow or the modes mismatch';
        break;

      case JSON_ERROR_CTRL_CHAR:
        $error = 'Unexpected control character found';
        break;

      case JSON_ERROR_SYNTAX:
        $error = 'Syntax error, malformed JSON';
        break;

      case JSON_ERROR_UTF8:
        $error = 'Malformed UTF-8 characters, possibly incorrectly encoded';
        break;

      default:
        $error = 'Unknown error';
        break;
    }
    if (!empty($error)) {
      $error_msg = t('Not a valid JSON object: @err', array('@err' => $error));
    }
  }
  else {
    if (!empty($json) && $config == NULL) {
      $error_msg = t('Not a valid JSON object.');
    }
  }

  return $error_msg;
}

<?php

/**
 * @file
 * Administrative callbacks for path metatags UI module.
 */

/**
 * Page callback for module settings page.
 */
function path_metatags_ui_metatags_list($form, &$form_state) {

  // Load path metatags.
  $result = path_metatags_load_all();

  $form_state['storage']['objects'] = $result;
  $form['#tree'] = TRUE;

  foreach ($result as $path) {
    $form[$path->machine_name]['title']['#markup'] = $path->name;
    // If object is overwritten.
    if ($path->is_overwritten) {
      $form[$path->machine_name]['title']['#markup'] .= '<sup>' . $path->type . '</sup>';
    }
    $form[$path->machine_name]['name']['#markup'] = $path->machine_name;
    $form[$path->machine_name]['path']['#markup'] = $path->path;
    // All paths are in $form_state['values']['paths'].
    $form[$path->machine_name]['#parents'] = array('paths', $path->machine_name);

    // Create operations for current metatags.
    $operations = array();

    if ($path->disabled == TRUE) {
      $operations[] = array(
        'title' => t('Enable'),
        'href' => 'admin/structure/path-metatags/enable/' . $path->machine_name,
      );
    }

    $operations[] = array(
      'title' => t('Edit'),
      'href' => 'admin/structure/path-metatags/edit/' . $path->machine_name,
    );
    $operations[] = array(
      'title' => t('Clone'),
      'href' => 'admin/structure/path-metatags/clone/' . $path->machine_name,
    );
    $operations[] = array(
      'title' => t('Export'),
      'href' => 'admin/structure/path-metatags/export/' . $path->machine_name,
    );
    if (empty($path->in_code_only)) {
      $operations[] = array(
        'title' => $path->is_overwritten ? t('Revert') : t('Delete'),
        'href' => 'admin/structure/path-metatags/delete/' . $path->machine_name,
      );
    }

    if ($path->disabled == FALSE) {
      $operations[] = array(
        'title' => t('Disable'),
        'href' => 'admin/structure/path-metatags/disable/' . $path->machine_name,
      );
    }

    $form[$path->machine_name]['actions'] = array(
      '#theme' => 'links__ctools_dropbutton',
      '#links' => $operations,
      '#attributes' => array('class' => array('links', 'inline'))
    );

    $form[$path->machine_name]['weight'] = array(
      '#type' => 'weight',
      '#default_value' => $path->weight,
      '#delta' => 100,
      '#attributes' => array('class' => array('path-metatags-ui-table-weight')),
    );

    $form[$path->machine_name]['disabled'] = array(
      '#type' => 'value',
      '#value' => $path->disabled,
    );
  }

  $form['actions'] = array(
    '#type' => 'action',
  );

  $form['actions']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save'),
  );

  $form['#attached']['css'][] = drupal_get_path('module', 'path_metatags_ui') . '/path_metatags_ui.css';

  return $form;
}

/**
 * Submit callback for path_metatags_ui_metatags_list form.
 */
function path_metatags_ui_metatags_list_submit($form, &$form_state) {
  if (!empty($form_state['values']['paths'])) {
    foreach ($form_state['values']['paths'] as $machine_name => $value) {
      if (isset($value['weight'])) {
        $path_metatags = $form_state['storage']['objects'][$machine_name];
        $path_metatags->weight = $value['weight'];
        path_metatags_save($path_metatags);
      }
    }
  }

  drupal_set_message(t('path metatags was updated.'));
}

/**
 * path metatags settings form.
 */
function path_metatags_ui_settings($form, $form_state) {

  $form['path_metatags_settings'] = array(
    '#type' => 'fieldset',
    '#title' => t('Metatags settings'),
  );

  $form['path_metatags_settings']['path_metatags_skip_tags'] = array(
    '#type' => 'checkbox',
    '#title' => t('Remove all tags from metatags output.'),
    '#description' => t('All tags from metatags output will be removed.'),
    '#default_value' => variable_get('path_metatags_skip_tags', 1),
  );

  return system_settings_form($form);
}


/**
 * Form for path metatags clone.
 */
function path_metatags_clone_metatags($path_metatags) {
  $path_metatags->machine_name .= '_clone';
  $path_metatags->export_type = NULL;
  unset($path_metatags->path_id);
  path_metatags_object_cache_set($path_metatags->machine_name, $path_metatags);
  $form_state = array('storage' => array('machine_name' => $path_metatags->machine_name));
  return drupal_build_form('path_metatags_ui_add_form', $form_state);
}

/**
 * Form for object export.
 */
function path_metatags_export_form($form, $form_state, $path_metatags) {

  drupal_set_title(t('Export path metatags "!name"', array('!name' => $path_metatags->name)));

  ctools_include('export');

  $code = ctools_export_crud_export('path_metatags', $path_metatags);
  $lines = substr_count($code, "\n");

  $form['export'] = array(
    '#title' => t('Export data'),
    '#type' => 'textarea',
    '#value' => $code,
    '#rows' => $lines,
    '#description' => t('Copy the export text and paste it into import area.'),
  );

  return $form;
}

/**
 * Form for object import.
 */
function path_metatags_import_form($form, $form_state) {

  $form['import'] = array(
    '#type' => 'textarea',
    '#title' => t('Paste code here to import path metatags'),
  );

  $form['actions'] = array(
    '#type' => 'action',
  );

  $form['actions']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Import'),
  );

  return $form;
}

/**
 * Submit callback for path metatags import form.
 */
function path_metatags_import_form_submit($form, &$form_state) {
  $code = $form_state['values']['import'];
  ctools_include('export');

  $path_metatags_raw = ctools_export_crud_import('path_metatags', $code);
  if (is_string($path_metatags_raw)) {
    form_error($form['import'], t('Unable to get an import from the code. Errors reported: @errors', array('@errors' => $path_metatags_raw)));
    $form_state['rebuild'] = TRUE;
    return;
  }
  // Because we get raw object from Ctools we should prepare it before saving.
  $path_metatags = path_metatags_load_prepare($path_metatags_raw);
  $path_metatags_original = path_metatags_load_by_name($path_metatags->machine_name);
  // If there is path metatags object with the same machine name,
  // delete the original so that this one writes properly.
  if (!empty($path_metatags_original)) {
    path_metatags_delete($path_metatags_original->machine_name);
  }
  if (path_metatags_save($path_metatags)) {
    if (!empty($path_metatags_original)) {
      drupal_set_message(filter_xss(t('Path metatags "!name" was successfully updated.', array('!name' => $path_metatags->name))));
    }
    else {
      drupal_set_message(filter_xss(t('Path metatags "!name" was successfully imported.', array('!name' => $path_metatags->name))));
    }
  }
  else {
    form_error($form['import'], t('Could not import path metatags.'));
  }

  $form_state['redirect'] = 'admin/structure/path-metatags';
}

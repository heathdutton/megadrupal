<?php
/**
 * @file
 * Administer forms.
 */

/*
 * Pages.
 */

/**
 * Main pages form.
 */
function bigvideo_pages_settings_form($form, &$form_state) {
  $pages = bigvideo_page_load_all();

  $pages_table = array(
    'header' => array(
      t('BPID'),
      t('Name'),
      t('Paths'),
      t('Status'),
      t('Operations'),
    ),
    'rows' => array(),
    'empty' => t('No page available.'),
  );

  foreach ($pages as $page) {
    $pages_table['rows'][] = array(
      $page->bpid,
      $page->name,
      check_plain($page->path),
      ($page->status == BIGVIDEO_PAGE_ENABLE) ? t('Enabled') : t('Disabled'),
      l(t('Edit'), 'admin/config/user-interface/bigvideo/pages/' . $page->bpid . '/edit') . ' ' .
      l(t('Delete'), 'admin/config/user-interface/bigvideo/pages/' . $page->bpid . '/delete'),
    );
  }

  $form['table'] = array(
    '#markup' => theme('table', $pages_table),
  );

  $form['actions'] = array('#type' => 'actions');
  $form['actions']['add'] = array(
    '#type' => 'submit',
    '#value' => t('Add page'),
  );
  return $form;
}

/**
 * Main pages form. Add submit.
 */
function bigvideo_pages_settings_form_submit($form, &$form_state) {
  $form_state['redirect'] = 'admin/config/user-interface/bigvideo/pages/add';
}

/**
 * Add new page form.
 */
function bigvideo_pages_add_settings_form($form, &$form_state) {
  $sources = bigvideo_source_load_all();
  $options = array();
  foreach ($sources as $source) {
    $options[$source->bsid] = $source->name;
  }
  $form['name'] = array(
    '#type' => 'textfield',
    '#title' => t('Page name'),
    '#description' => t('Administrative name'),
    '#required' => TRUE,
  );
  $form['source'] = array(
    '#type' => 'select',
    '#title' => t('Source'),
    '#options' => $options,
    '#required' => TRUE,
  );
  if (!$options) {
    $form['source']['#suffix'] = t('You need to !create first', array('!create' => l(t('create source'), 'admin/config/user-interface/bigvideo/sources')));
  }
  $description_params = array(
    '%blog' => 'blog',
    '%blog-wildcard' => 'blog/*',
    '%front' => '<front>',
  );
  $description = t("Specify pages by using their paths. Enter one path per line. The '*' character is a wildcard. Example paths are %blog for the blog page and %blog-wildcard for every personal blog. %front is the front page.", $description_params);
  $form['path'] = array(
    '#type' => 'textarea',
    '#title' => t('Paths'),
    '#description' => $description,
    '#required' => TRUE,
  );
  $form['status'] = array(
    '#type' => 'checkbox',
    '#title' => t('Enabled'),
    '#default_value' => BIGVIDEO_PAGE_ENABLE,
  );
  $form['actions'] = array('#type' => 'actions');
  $form['actions']['save'] = array(
    '#type' => 'submit',
    '#value' => t('Save page'),
  );
  return $form;
}

/**
 * Save page data to database.
 */
function bigvideo_pages_add_settings_form_submit($form, &$form_state) {
  $values = $form_state['values'];
  $fields = array(
    'name' => $values['name'],
    'source' => $values['source'],
    'path' => $values['path'],
    'status' => $values['status'],
  );
  if ($bpid = bigvideo_page_create((object) $fields)) {
    drupal_set_message('New page has been added');
  }
  else {
    drupal_set_message('Error by adding new page', 'error');
  }
  $form_state['redirect'] = 'admin/config/user-interface/bigvideo/pages';
}

/**
 * Edit page form.
 */
function bigvideo_pages_edit_settings_form($form, &$form_state, $page) {
  $sources = bigvideo_source_load_all();
  $options = array();
  foreach ($sources as $source) {
    $options[$source->bsid] = $source->name;
  }
  $form['bpid'] = array(
    '#type' => 'hidden',
    '#value' => $page->bpid,
  );
  $form['name'] = array(
    '#type' => 'textfield',
    '#title' => t('Page name'),
    '#required' => TRUE,
    '#description' => t('Administrative name'),
    '#default_value' => $page->name,
  );
  $form['source'] = array(
    '#type' => 'select',
    '#title' => t('Source'),
    '#options' => $options,
    '#required' => TRUE,
    '#default_value' => $page->source,
  );
  if (!$options) {
    $form['source']['#suffix'] = t('You need to !create first', array('!create' => l(t('create source'), 'admin/config/user-interface/bigvideo/sources')));
  }
  $description_params = array(
    '%blog' => 'blog',
    '%blog-wildcard' => 'blog/*',
    '%front' => '<front>',
  );
  $description = t("Specify pages by using their paths. Enter one path per line. The '*' character is a wildcard. Example paths are %blog for the blog page and %blog-wildcard for every personal blog. %front is the front page.", $description_params);
  $form['path'] = array(
    '#type' => 'textarea',
    '#title' => t('Paths'),
    '#description' => $description,
    '#required' => TRUE,
    '#default_value' => $page->path,
  );
  $form['status'] = array(
    '#type' => 'checkbox',
    '#title' => t('Enabled'),
    '#default_value' => $page->status,
  );
  $form['actions'] = array('#type' => 'actions');
  $form['actions']['save'] = array(
    '#type' => 'submit',
    '#value' => t('Save page'),
  );
  return $form;
}

/**
 * Update page entry in database.
 */
function bigvideo_pages_edit_settings_form_submit($form, &$form_state) {
  $values = $form_state['values'];
  $fields = array(
    'bpid' => $values['bpid'],
    'name' => $values['name'],
    'source' => $values['source'],
    'path' => $values['path'],
    'status' => $values['status'],
  );
  if ($bpid = bigvideo_page_update((object) $fields)) {
    drupal_set_message('Page has been updated');
  }
  else {
    drupal_set_message('Nothing to update');
  }
  $form_state['redirect'] = 'admin/config/user-interface/bigvideo/pages';
}

/**
 * Delete page form.
 */
function bigvideo_pages_delete_settings_form($form, &$form_state, $page) {
  $form['bpid'] = array(
    '#type' => 'hidden',
    '#value' => $page->bpid,
  );
  return confirm_form($form, t('Are you sure you want to delete the Bigvideo page %name?', array('%name' => $page->name)), 'admin/config/user-interface/bigvideo/pages', '', t('Delete'), t('Cancel'));
}

/**
 * Delete form from database.
 */
function bigvideo_pages_delete_settings_form_submit($form, &$form_state) {
  bigvideo_page_delete($form_state['values']['bpid']);
  drupal_set_message('Page has been deleted.');
  $form_state['redirect'] = 'admin/config/user-interface/bigvideo/pages';
}

/*
 * Sources.
 */

/**
 * Main source form.
 */
function bigvideo_sources_settings_form($form, &$form_state) {
  $sources = bigvideo_source_load_all();

  $sources_table = array(
    'header' => array(
      t('BSID'),
      t('Name'),
      t('Type'),
      t('MP4'),
      t('WebM'),
      t('Operations'),
    ),
    'rows' => array(),
    'empty' => t('No source available.'),
  );

  foreach ($sources as $source) {
    $sources_table['rows'][] = array(
      $source->bsid,
      $source->name,
      ($source->type == BIGVIDEO_SOURCE_TYPE_FILE) ? t('Files') : t('Links'),
      _bigvideo_format_source_link($source, 'mp4'),
      _bigvideo_format_source_link($source, 'webm'),
      l(t('Edit'), 'admin/config/user-interface/bigvideo/sources/' . $source->bsid . '/edit') . ' ' .
      l(t('Delete'), 'admin/config/user-interface/bigvideo/sources/' . $source->bsid . '/delete'),
    );
  }

  $form['table'] = array(
    '#markup' => theme('table', $sources_table),
  );

  $form['actions'] = array('#type' => 'actions');
  $form['actions']['add'] = array(
    '#type' => 'submit',
    '#value' => t('Add source'),
  );
  return $form;
}

/**
 * Create source link for main table.
 */
function _bigvideo_format_source_link($source, $format) {
  $path = $source->{$format};
  if ($source->type == BIGVIDEO_SOURCE_TYPE_FILE && intval($source->{$format})) {
    if ($file = file_load($source->{$format})) {
      $path = file_create_url($file->uri);
    }
  }
  $link = l($path, $path);
  return $link;
}

/**
 * Redirect to add source form.
 */
function bigvideo_sources_settings_form_submit($form, &$form_state) {
  $form_state['redirect'] = 'admin/config/user-interface/bigvideo/sources/add';
}

/**
 * Add source form.
 */
function bigvideo_sources_add_settings_form($form, &$form_state) {
  $form['#attributes'] = array(
    'id' => array('bigvideo-source-edit-form'),
  );
  $form['name'] = array(
    '#type' => 'textfield',
    '#title' => t('Source name'),
    '#description' => t('Administrative name'),
    '#required' => TRUE,
  );
  $form['type'] = array(
    '#type' => 'select',
    '#title' => t('Type'),
    '#options' => array(
      BIGVIDEO_SOURCE_TYPE_FILE => t('Files'),
      BIGVIDEO_SOURCE_TYPE_LINK => t('Links'),
    ),
    '#default_value' => BIGVIDEO_SOURCE_TYPE_FILE,
    '#required' => TRUE,
  );
  $form['files'] = array(
    '#type' => 'fieldset',
    '#title' => t('Files'),
    '#states' => array(
      'visible' => array(
        ':input[name="type"]' => array('value' => BIGVIDEO_SOURCE_TYPE_FILE),
      ),
    ),
  );
  $form['files']['mp4_file'] = array(
    '#type' => 'managed_file',
    '#title' => t('MP4'),
    '#upload_location' => 'public://bigvideo/',
    '#upload_validators' => array(
      'file_validate_extensions' => array('mp4'),
    ),
  );
  $form['files']['webm_file'] = array(
    '#type' => 'managed_file',
    '#title' => t('WebM'),
    '#upload_location' => 'public://bigvideo/',
    '#upload_validators' => array(
      'file_validate_extensions' => array('webm'),
    ),
  );
  $form['links'] = array(
    '#type' => 'fieldset',
    '#title' => t('Links'),
    '#states' => array(
      'visible' => array(
        ':input[name="type"]' => array('value' => BIGVIDEO_SOURCE_TYPE_LINK),
      ),
    ),
  );
  $form['links']['mp4_link'] = array(
    '#type' => 'textfield',
    '#title' => t('MP4'),
  );
  $form['links']['webm_link'] = array(
    '#type' => 'textfield',
    '#title' => t('WebM'),
  );
  $form['actions'] = array('#type' => 'actions');
  $form['actions']['save'] = array(
    '#type' => 'submit',
    '#value' => t('Save source'),
  );
  return $form;
}

/**
 * Create source and save to database.
 */
function bigvideo_sources_add_settings_form_submit($form, &$form_state) {
  $values = $form_state['values'];
  $fields = array(
    'name' => $values['name'],
    'type' => $values['type'],
  );
  $files_fields = array('mp4', 'webm');
  $files = array();
  if ($values['type'] == BIGVIDEO_SOURCE_TYPE_FILE) {
    foreach ($files_fields as $file_field) {
      $fields[$file_field] = NULL;
      if ($file = _bigvideo_sources_save_file($values[$file_field . '_file'])) {
        $files[] = $file;
        $fields[$file_field] = $file->fid;
      }
    }
  }
  else {
    $fields['mp4'] = $values['mp4_link'];
    $fields['webm'] = $values['webm_link'];
  }
  if ($bsid = bigvideo_source_create((object) $fields)) {
    foreach ($files as $file) {
      file_usage_add($file, 'bigvideo', 'source', $bsid);
    }
    drupal_set_message('New source has been added');
  }
  else {
    drupal_set_message('Error by adding new source', 'error');
  }
  $form_state['redirect'] = 'admin/config/user-interface/bigvideo/sources';
}

/**
 * Edit source form.
 */
function bigvideo_sources_edit_settings_form($form, &$form_state, $source) {
  $form['#attributes'] = array(
    'id' => array('bigvideo-source-edit-form'),
  );
  $form_state['storage']['original'] = $source;
  $form['bsid'] = array(
    '#type' => 'hidden',
    '#value' => $source->bsid,
  );
  $form['name'] = array(
    '#type' => 'textfield',
    '#title' => t('Source name'),
    '#description' => t('Administrative name'),
    '#required' => TRUE,
    '#default_value' => $source->name,
  );
  $form['type'] = array(
    '#type' => 'select',
    '#title' => t('Type'),
    '#options' => array(
      BIGVIDEO_SOURCE_TYPE_FILE => t('Files'),
      BIGVIDEO_SOURCE_TYPE_LINK => t('Links'),
    ),
    '#default_value' => $source->type,
    '#required' => TRUE,
  );
  $form['files'] = array(
    '#type' => 'fieldset',
    '#title' => t('Files'),
    '#states' => array(
      'visible' => array(
        ':input[name="type"]' => array('value' => BIGVIDEO_SOURCE_TYPE_FILE),
      ),
    ),
  );
  $form['files']['mp4_file'] = array(
    '#type' => 'managed_file',
    '#title' => t('MP4'),
    '#upload_location' => 'public://bigvideo/',
    '#default_value' => ($source->type == BIGVIDEO_SOURCE_TYPE_FILE) ? $source->mp4 : NULL,
    '#upload_validators' => array(
      'file_validate_extensions' => array('mp4'),
    ),
  );
  $form['files']['webm_file'] = array(
    '#type' => 'managed_file',
    '#title' => t('WebM'),
    '#upload_location' => 'public://bigvideo/',
    '#default_value' => ($source->type == BIGVIDEO_SOURCE_TYPE_FILE) ? $source->webm : NULL,
    '#upload_validators' => array(
      'file_validate_extensions' => array('webm'),
    ),
  );
  $form['links'] = array(
    '#type' => 'fieldset',
    '#title' => t('Links'),
    '#states' => array(
      'visible' => array(
        ':input[name="type"]' => array('value' => BIGVIDEO_SOURCE_TYPE_LINK),
      ),
    ),
  );
  $form['links']['mp4_link'] = array(
    '#type' => 'textfield',
    '#title' => t('MP4'),
    '#default_value' => ($source->type == BIGVIDEO_SOURCE_TYPE_LINK) ? $source->mp4 : NULL,
  );
  $form['links']['webm_link'] = array(
    '#type' => 'textfield',
    '#title' => t('WebM'),
    '#default_value' => ($source->type == BIGVIDEO_SOURCE_TYPE_LINK) ? $source->webm : NULL,
  );
  $form['actions'] = array('#type' => 'actions');
  $form['actions']['save'] = array(
    '#type' => 'submit',
    '#value' => t('Save source'),
  );
  return $form;
}

/**
 * Update source entry in database.
 */
function bigvideo_sources_edit_settings_form_submit($form, &$form_state) {
  $values = $form_state['values'];
  $original = $form_state['storage']['original'];
  $fields = array(
    'bsid' => $values['bsid'],
    'name' => $values['name'],
    'type' => $values['type'],
  );
  $files_fields = array('mp4', 'webm');
  if ($values['type'] == BIGVIDEO_SOURCE_TYPE_FILE) {
    foreach ($files_fields as $file_field) {
      $fields[$file_field] = NULL;
      if (!empty($original->{$file_field}) && !empty($values[$file_field . '_file'])) {
        if ($original->{$file_field} == $values[$file_field . '_file']) {
          $fields[$file_field] = $original->{$file_field};
        }
        else {
          if ($file = _bigvideo_sources_save_file($values[$file_field . '_file'], $values['bsid'])) {
            $fields[$file_field] = $file->fid;
          }
        }
      }
      elseif (!empty($original->{$file_field}) && empty($values[$file_field . '_file'])) {
        if (intval($original->{$file_field})) {
          _bigvideo_sources_delete_file($original->{$file_field});
        }
      }
      elseif (empty($original->{$file_field}) && !empty($values[$file_field . '_file'])) {
        if ($file = _bigvideo_sources_save_file($values[$file_field . '_file'], $values['bsid'])) {
          $fields[$file_field] = $file->fid;
        }
      }
    }
  }
  else {
    if ($original->type == BIGVIDEO_SOURCE_TYPE_FILE) {
      foreach ($files_fields as $file_field) {
        if (intval($original->{$file_field})) {
          _bigvideo_sources_delete_file($original->{$file_field});
        }
      }
    }
    $fields['mp4'] = $values['mp4_link'];
    $fields['webm'] = $values['webm_link'];
  }
  if ($bsid = bigvideo_source_update((object) $fields)) {
    drupal_set_message('Source has been updated');
  }
  else {
    drupal_set_message('Nothing to update');
  }
  $form_state['redirect'] = 'admin/config/user-interface/bigvideo/sources';
}

/**
 * Delete source form.
 */
function bigvideo_sources_delete_settings_form($form, &$form_state, $source) {
  $form['bsid'] = array(
    '#type' => 'hidden',
    '#value' => $source->bsid,
  );
  if ($source->type == BIGVIDEO_SOURCE_TYPE_FILE) {
    $form['mp4'] = array(
      '#type' => 'hidden',
      '#value' => $source->mp4,
    );
    $form['webm'] = array(
      '#type' => 'hidden',
      '#value' => $source->webm,
    );
  }
  return confirm_form($form, t('Are you sure you want to delete the Bigvideo source %name?', array('%name' => $source->name)), 'admin/config/user-interface/bigvideo/sources', '', t('Delete'), t('Cancel'));
}

/**
 * Delete source from database.
 */
function bigvideo_sources_delete_settings_form_submit($form, &$form_state) {
  if (isset($form_state['values']['mp4'])) {
    _bigvideo_sources_delete_file($form_state['values']['mp4']);
  }
  if (isset($form_state['values']['webm'])) {
    _bigvideo_sources_delete_file($form_state['values']['webm']);
  }
  bigvideo_source_delete($form_state['values']['bsid']);
  drupal_set_message('Source has been deleted.');
  $form_state['redirect'] = 'admin/config/user-interface/bigvideo/sources';
}

/**
 * Set file as permanent and add entry to file_usage table.
 *
 * @param int $fid
 *   File ID for save.
 * @param int $bsid
 *   Bigvideo Source ID for usage entry.
 *
 * @return int|null
 *   File ID or NULL.
 */
function _bigvideo_sources_save_file($fid, $bsid = NULL) {
  if ($file = file_load($fid)) {
    $file->status = FILE_STATUS_PERMANENT;
    if (file_save($file)) {
      if ($bsid) {
        file_usage_add($file, 'bigvideo', 'source', $bsid);
      }
      return $file;
    }
  }
  return NULL;
}

/**
 * Delete file by File ID.
 *
 * @param int $fid
 *   File ID for delete.
 * @param int $bsid
 *   Bigvideo Source ID for usage entry.
 *
 * @return bool
 *   TRUE - file loaded and deleted.
 *   FALSE - file can't be loaded.
 */
function _bigvideo_sources_delete_file($fid, $bsid = NULL) {
  if ($file = file_load($fid)) {
    file_delete($file);
    file_usage_delete($file, 'bigvideo', 'source', $bsid);
    return TRUE;
  }
  return FALSE;
}

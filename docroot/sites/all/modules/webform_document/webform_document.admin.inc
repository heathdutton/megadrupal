<?php

/**
 * Main configuration form for editing a webform document settings.
 */
function webform_document_form($form, &$form_state, $node) {
  $form['#node'] = $node;
  if (!isset($node->webform['document_template'])) {
    $node->webform['document_template'] = array();
  }

  $form['nid'] = array(
    '#type' => 'value',
    '#value' => $node->nid,
  );
  $form['#tree'] = TRUE;
  $path = drupal_get_path('module', 'webform_document');
  $form['#attached']['css'][] = $path . '/webform_document.css';

  $custom_replacements = isset($node->webform['document_template']['custom_replacements'])
    ? $node->webform['document_template']['custom_replacements'] : array();

  $form['output_type'] = array(
    '#title' => t('Output type'),
    '#type' => 'radios',
    '#options' => array(
      'docx' => t('Word docx'),
    ),
    '#default_value' => isset($node->webform['document_template']['output_type'])
      ? $node->webform['document_template']['output_type']
      : 'docx',
  );
  if (webform_document_libreoffice_path()) {
    $form['output_type']['#options']['pdf'] = t('PDF');
  }
  else {
    $form['output_type']['#description'] = t('To use PDF as output type you need install LibreOffice as <a href="@cde-url">CDE</a> package.', array('@cde-url' => 'http://www.pgbovine.net/cde.html'));
  }
  $form['document_template'] = array(
    '#title' => t('Document template'),
    '#description' => t('Allowed file types: !extensions.', array('!extensions' => '<strong>docx</strong>')),
    '#type' => 'managed_file',
    '#upload_location' => WEBFORM_DOCUMENT_INPUT_DIR,
    '#default_value' => isset($node->webform['document_template']['fid'])
      ? $node->webform['document_template']['fid']
      : NULL,
    '#upload_validators' => array(
      'file_validate_extensions' => array('docx'),
    ),
  );
  $form['available_tokens'] = array(
    '#title' => t('Replacement patterns'),
    '#type' => 'fieldset',
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );
  $form['available_tokens']['tokens'] = array(
    '#theme' => 'token_tree',
    '#token_types' => array('node', 'submission'),
    '#global_types' => TRUE,
    '#click_insert' => FALSE,
  );

  $form['custom_replacements'] = array(
    '#type' => 'fieldset',
    '#title' => t('Custom replacements'),
    '#description' => t('It is also possible to use here replacements tokens.'),
    '#prefix' => '<div id="custom-replacements-fieldset-wrapper">',
    '#suffix' => '</div>',
  );

  if (!isset($form_state['custom_replacements_count'])) {
    $form_state['custom_replacements_count'] = count($custom_replacements);
  }

  for ($i = 1; $i <= $form_state['custom_replacements_count']; $i++) {
    $form['custom_replacements'][$i] = array(
      '#type' => 'fieldset',
      '#title' => t('Replacement #@num', array('@num' => $i)),
      '#collapsible' => TRUE,
      '#collapsed' => FALSE,
    );

    $form['custom_replacements'][$i]['search'] = array(
      '#type' => 'textfield',
      '#title' => t('Search for'),
      '#size' => 20,
      '#default_value' => isset($custom_replacements[$i]['search'])
        ? $custom_replacements[$i]['search']
        : NULL,
    );
    $form['custom_replacements'][$i]['replace'] = array(
      '#title' => t('Replace with'),
      '#description' => t('It is possible to use tokens.'),
      '#type' => 'textarea',
      '#rows' => 2,
      '#default_value' => isset($custom_replacements[$i]['replace'])
        ? $custom_replacements[$i]['replace']
        : NULL,
    );
    if (module_exists('php') && user_access('use PHP for settings')) {
      $form['custom_replacements'][$i]['replace']['#description'] = t('To use PHP wrap code in &lt;?php ?&gt; tags. It is also possible to use tokens.');
    }
  }

  $form['custom_replacements']['add_new'] = array(
    '#type' => 'submit',
    '#value' => t('Add replacement'),
    '#submit' => array('webform_document_form_add_more'),
    '#limit_validation_errors' => array(),
    '#ajax' => array(
      'callback' => 'webform_document_form_ajax_callback',
      'wrapper' => 'custom-replacements-fieldset-wrapper',
    ),
  );
  if ($form_state['custom_replacements_count'] > 0) {
    $form['custom_replacements']['remove_last'] = array(
      '#type' => 'submit',
      '#value' => t('Remove last'),
      '#submit' => array('webform_document_form_remove_last'),
      '#limit_validation_errors' => array(),
      '#ajax' => array(
        'callback' => 'webform_document_form_ajax_callback',
        'wrapper' => 'custom-replacements-fieldset-wrapper',
      ),
    );
  }

  $form['actions'] = array(
    '#type' => 'actions',
    '#weight' => 300,
  );
  $form['actions']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save'),
  );

  $form['#submit'] = array(
    'webform_document_form_submit',
    'webform_document_form_submit_save',
  );

  return $form;
}

/**
 * Callback for both ajax-enabled buttons.
 *
 * Selects and returns the fieldset with the names in it.
 */
function webform_document_form_ajax_callback($form, $form_state) {
  return $form['custom_replacements'];
}

/**
 * Submit handler for the "Add replacement" button.
 * Increments the max counter and causes a rebuild.
 */
function webform_document_form_add_more($form, &$form_state) {
  $form_state['custom_replacements_count']++;
  $form_state['rebuild'] = TRUE;
}

/**
 * Submit handler for the "Remove last" button.
 * Removes last custom replacement.
 */
function webform_document_form_remove_last($form, &$form_state) {
  if ($form_state['custom_replacements_count'] > 0) {
    $form_state['custom_replacements_count']--;
  }
  $form_state['rebuild'] = TRUE;
}

/**
 * Submit handler for webform_document_form().
 */
function webform_document_form_submit($form, &$form_state) {
  // Edit the node by reference just to shorten it up.
  $node = &$form['#node'];

  $form_state['#document_template_default'] = $node->webform['document_template'];

  // Assign settings.
  $node->webform['document_template']['fid'] = $form_state['values']['document_template'];
  $node->webform['document_template']['output_type'] = $form_state['values']['output_type'];
  if (isset($form_state['values']['custom_replacements']['add_new'])) {
    unset($form_state['values']['custom_replacements']['add_new']);
  }
  if (isset($form_state['values']['custom_replacements']['remove_last'])) {
    unset($form_state['values']['custom_replacements']['remove_last']);
  }
  $node->webform['document_template']['custom_replacements'] = $form_state['values']['custom_replacements'];
}

/**
 * Submit handler for webform_document_form() that saves the node.
 *
 * This is separate from webform_document_form_submit() to allow other
 * modules to add properties if needed into the $form['#node'] object before save.
 */
function webform_document_form_submit_save($form, &$form_state) {
  $node = &$form['#node'];
  $document_templates = variable_get('webform_document', array());

  $document_templates[$node->nid] = array(
    'output_type' => $node->webform['document_template']['output_type'],
    'custom_replacements' => $node->webform['document_template']['custom_replacements'],
    );
  if (!empty($node->webform['document_template']['fid'])) {
    $document_templates[$node->nid]['fid'] = $node->webform['document_template']['fid'];
  }

  variable_set('webform_document', $document_templates);

  $document_template_fid = $form_state['values']['document_template'];

  // Add file.
  if ($document_template_fid != 0) {
    $file = file_load($document_template_fid);
    $file->status = FILE_STATUS_PERMANENT;
    file_save($file);
    file_usage_add($file, 'webform_document', 'webform_document_' . $node->nid, 1);
  }
  // Delete file.
  elseif ($document_template_fid == 0 && !empty($form_state['#document_template_default']['fid'])) {
    $fid = $document_templates[$node->nid];
    $file = $fid ? file_load($fid) : FALSE;
    if ($file) {
      file_usage_delete($file, 'webform_document', 'webform_document_' . $node->nid, 1);
      file_delete($file);
    }
  }

  node_save($node);
  drupal_set_message(t('The document template have been updated.'));
}

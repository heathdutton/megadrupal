<?php

/**
 * @file
 * The administration interface.
 */

/**
 * Menu callback; Admin form for xliff tools.
 */
function xliff_settings($form, &$form_state) {
  drupal_set_message(t('Not yet implemented.'), 'error');
  $form['dummy'] = NULL;
  return $form;
}

/**
 * Export form for a specific node.
 */
function xliff_export_form($form, $form_state, $node) {
  $form['node'] = array(
    '#type' => 'value',
    '#value' => $node,
  );

  $form['help'] = array(
    '#markup' => t('Export the content for external translation.') . '<br />',
  );

  $form['actions'] = array('#type' => 'actions');
  $form['actions']['export'] = array(
    '#type' => 'submit',
    '#value' => t('XLIFF Export'),
  );

  $form['actions']['bilingual_export'] = array(
    '#type' => 'submit',
    '#value' => t('Bilingual XLIFF Export'),
  );

  return $form;
}

/**
 * Submit handler for XLIFF export form.
 */
function xliff_export_form_submit($form, $form_state) {
  $node = $form_state['values']['node'];
  $nid = $node->nid;
  $langcode = (!empty($node->language)) ? $node->language : language_default('language');

  switch ($form_state['clicked_button']['#id']) {
    case 'edit-export':
      if ($output = xliff_export($nid)) {
        header('Content-type: application/xml; encoding=utf8');
        header('Content-Disposition: attachment; filename="node-' . "$nid-$langcode" . '.xlf"');
        print $output;
        exit();
      }
      else {
        drupal_set_message(t('Failed to export this content.'), 'error');
      }
      break;

    case 'edit-bilingual-export':
      if ($output = xliff_bilingual_export($nid, $message)) {
        header('Content-type: application/xml; encoding=utf8');
        header('Content-Disposition: attachment; filename="node-' . "$nid-$langcode" . '.xlf"');
        print $output;
        exit();
      }
      elseif ($message) {
        drupal_set_message(check_plain($message), 'error');
      }
      else {
        drupal_set_message(t('Failed to export this content.'), 'error');
      }
      break;
  }
}

/**
 * Import form for a specific node.
 */
function xliff_import_form($form, $form_state, $node) {
  $form['node'] = array(
    '#type' => 'value',
    '#value' => $node,
  );

  $form['xliff_file'] = array(
    '#type' => 'file',
    '#title' => t('XLIFF file'),
    '#description' => t('Replace or translate the current content with the uploaded XLIFF document.<br />The filename format must be <em>node-$nid-$langcode.xlf</em>. If the file contains a new translation then new content will be created automatically.'),
  );
  $form['revision'] = array(
    '#type' => 'checkbox',
    '#access' => user_access('administer nodes'),
    '#title' => t('Create new revision'),
    '#default_value' => TRUE,
  );

  $form['preserve_path'] = array(
    '#type' => 'checkbox',
    '#access' => user_access('administer url aliases'),
    '#title' => t('Preserve the URL alias'),
    '#description' => t('If this option is enabled then new translations will have the same URL alias as the source content. You may want to disable this option if you have pathauto settings on this content type or if you want to use different URL alias per language. This option does not change the URL alias of existing translations.'),
    '#default_value' => FALSE,
  );

  $form['actions'] = array('#type' => 'actions');
  $form['actions']['import'] = array(
    '#type' => 'submit',
    '#value' => t('Import XLIFF'),
  );

  $form['#attributes'] = array('enctype' => "multipart/form-data");
  return $form;
}

/**
 * Submit handler for XLIFF import form.
 */
function xliff_import_form_submit($form, $form_state) {
  $node = $form_state['values']['node'];
  $revision = $form_state['values']['revision'];
  $preserve_path = $form_state['values']['preserve_path'];
  $error_message = '';

  $validators = array(
    'file_validate_extensions' => array('xlf', 'xliff'),
  );
  if ($file = file_save_upload('xliff_file', $validators)) {
    if (xliff_check_import_file($file, $node->nid, $error_message)) {
      if ($inode = xliff_import($file, $revision, $preserve_path, FALSE, $error_message)) {
        drupal_set_message(t('XLIFF file imported.'));
        global $language;
        $languages = language_list();
        $language = $languages[$inode->language];
        drupal_goto("node/{$inode->nid}");
        return;
      }
    }
  }

  drupal_set_message(t('Failed to import the XLIFF file.') . ' ' . check_plain($error_message), 'error');
}

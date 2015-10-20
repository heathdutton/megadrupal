<?php

/**
 * @file
 * Functions related to administrative UI for the Entity XLIFF module.
 */


/**
 * Page callback (as a form) which provides an extremely basic UI around
 * exporting and importing entities via XLIFF. The UI should never, ever be
 * extended very far beyond what it does here. This is meant to be a developer-
 * centric, API module.
 */
function entity_xliff_actions($form, &$form_state) {
  $type = $form_state['build_info']['args'][0];
  $entity = $form_state['build_info']['args'][1];
  $wrapper = entity_metadata_wrapper($type, $entity);
  $uri = entity_uri($type, $entity);
  $path = $uri['path'];

  // Set the page title.
  drupal_set_title(t('%title XLIFF portal', array('%title' => $wrapper->label())), PASS_THROUGH);

  // Prepare the table.
  $header = array(t('Target language'), t('Export as XLIFF'), t('Import from XLIFF'), t('View'));
  $rows = array();
  foreach (language_list() as $langcode => $language) {
    // Generate a link to download / export this entity as XLIFF.
    $export_link = l('Download', $path . '/as.xlf', array(
      'query' => array(
        'targetLang' => $langcode,
      ),
    ));

    $file_upload = array(
      '#name' => 'files[import-' . $langcode . ']',
      '#type' => 'file',
      '#title' => t('Import'),
      '#title_display' => 'invisible',
    );

    $rows[$langcode] = array(
      'name' => $language->name,
      'export' => $export_link,
      'import' => drupal_render($file_upload),
      'view' => l(t('View'), $path, array('language' => $language)),
    );
  }

  // Because of the way we handle the file fields, we add this manually.
  $form['#attributes']['enctype'] = 'multipart/form-data';

  // Add the table to be rendered.
  $form['entity_xliff_translations'] = array(
    '#theme' => 'table',
    '#header' => $header,
    '#rows' => $rows,
  );

  // Add the submit button.
  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Import'),
  );

  return $form;
}

/**
 * Validation handler for the entity_xliff_actions form. Ensures that uploaded
 * files meet expected criteria.
 */
function entity_xliff_actions_validate($form, &$form_state) {
  foreach (language_list() as $langcode => $language) {
    $field_name = 'import' . '-' . $langcode;
    $file = file_save_upload($field_name, array(
      // Validate extensions.
      'file_validate_extensions' => array('xlf xliff xml'),
    ));

    if ($file === FALSE) {
      form_set_error('files[' . $field_name . ']', t('Failed to upload file: invalid extension.'));
    }
    elseif (is_object($file)) {
      $form_state['storage']['import'][$langcode] = $file;
    }
  }
}

/**
 * Submit handler for the entity_xliff_actions form. Given a list of valid XLIFF
 * file uploads, performs an import on each.
 */
function entity_xliff_actions_submit($form, &$form_state) {
  if (!empty($form_state['storage']['import'])) {
    $type = $form_state['build_info']['args'][0];
    $entity = $form_state['build_info']['args'][1];
    $wrapper = entity_metadata_wrapper($type, $entity);

    foreach ($form_state['storage']['import'] as $langcode => $file) {
      $xliff = file_get_contents($file->uri);
      $data = entity_xliff_set_xliff($wrapper, $langcode, $xliff);

      // If no data was returned, the import was invalid.
      if ($data === array()) {
        drupal_set_message(t('There was a problem importing %name: XLIFF import validation failure.', array(
          '%name' => $file->filename,
        )));
      }
      // Otherwise, hurray!
      else {
        drupal_set_message(t('Successfully imported %name.', array(
          '%name' => $file->filename,
        )));
      }
    }
  }
}

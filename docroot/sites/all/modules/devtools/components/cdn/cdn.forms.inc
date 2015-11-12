<?php

/**
 * Form callback for /admin/config/development/cdn/invalidate;
 * Provides options to invalidate individual site components.
 * @return array Form API array.
 */
function devtools_cdn_admin_invalidate_form($form, &$form_state) {
  $form = array();

  $form['#prefix'] = t('All static files within selected components and paths will be invalidated.');

  $form['path'] = array(
    '#title' => t('File or folder path'),
    '#type' => 'textfield',
    '#description' => t('Path should be relative to the document root.'),
  );

  $form['components'] = array(
    '#title' => t('Components'),
    '#type' => 'checkboxes',
    '#options' => array(
      'themes' => t('Themes'),
    ),
  );

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Submit'),
  );

  return $form;
}

/**
 * Validation handler for the devtools_cdn_admin_invalidate_form form.
 * @param  array $form       The FAPI form.
 * @param  array $form_state Current form state.
 */
function devtools_cdn_admin_invalidate_form_validate($form, &$form_state) {
  $values =& $form_state['values'];

  if (!empty($values['path']) && !file_exists($values['path'])) {
    form_set_error('path', t('The file/path %path does not exist.', array('%path' => $values['path'])));
  }
}

/**
 * Submit handler for the devtools_cdn_admin_invalidate_form form.
 * @param  array $form       The FAPI form.
 * @param  array $form_state Current form state.
 */
function devtools_cdn_admin_invalidate_form_submit($form, &$form_state) {
  $values =& $form_state['values'];

  $paths = array();

  if (!empty($values['path'])) {
    $paths[] = $values['path'];
  }

  if ($values['components']['themes']) {
    foreach (list_themes() as $theme) {
      if ($theme->status) {
        // figure out the theme folder's path from the .info file's filepath
        $path_parts = explode('/', $theme->filename);
        array_pop($path_parts);

        $paths[] = implode('/', $path_parts);
      }
    }
  }

  foreach ($paths as &$path) {
    $path = trim($path, '/');

    if (touch($path)) {
      drupal_set_message(t('Successfully touched %path.', array('%path' => $path)));
    }
    else {
      drupal_set_message(t('Failed to touch %path. Do you have sufficient permissions?', array('%path' => $path)));
    }
  }
}
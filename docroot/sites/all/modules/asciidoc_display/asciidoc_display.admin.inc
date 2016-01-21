<?php

/**
 * @file
 * Administrative pages for AsciiDoc display.
 */

/**
 * Page callback: Outputs the main administrative page for AsciiDoc Display.
 */
function asciidoc_display_main_admin() {
  $configs = asciidoc_display_load_all_configs();

  // Build a table of current config items.

  $headers = array(t('Title'), t('Display URL'), t('File path'), t('Operations'));
  $rows = array();
  foreach ($configs as $config) {
    $ops = array(
      l(t('edit'), 'admin/config/development/asciidoc/edit/' . $config['config_id']),
      l(t('delete'), 'admin/config/development/asciidoc/delete/' . $config['config_id']),
    );
    $rows[] = array($config['title'], l($config['path'], $config['path']), $config['dir'], implode(' ', $ops));
  }

  $build = array();

  $add = l(t('Add new book'), 'admin/config/development/asciidoc/add');

  $build['add'] = array(
    '#markup' => '<p>' . $add . '</p>',
  );


  $clear = l(t('Clear book cache'), 'admin/config/development/asciidoc/clear_cache');

  $build['clear'] = array(
    '#markup' => '<p>' . $clear . '</p>',
  );

  $build['current'] = array(
    '#theme' => 'table',
    '#header' => $headers,
    '#rows' => $rows,
    '#empty' => t('No AsciiDoc books defined yet.') . ' ' . $add,
  );

  return $build;
}

/**
 * Form constructor for the add/edit form for AsciiDoc configuration.
 *
 * @see asciidoc_display_edit_form_submit()
 */
function asciidoc_display_edit_form($form, &$form_state, $config_id = 0) {
  $config = asciidoc_display_load_config($config_id);
  $form['#config_id'] = $config['config_id'];

  $form['title'] = array(
    '#type' => 'textfield',
    '#title' => t('Title of book'),
    '#description' => t('Used in book list block.'),
    '#required' => TRUE,
    '#default_value' => $config['title'],
  );

  $form['path'] = array(
    '#type' => 'textfield',
    '#title' => t('URL path for displaying docs'),
    '#description' => t('No trailing slash. Must not start with edit/. Do not include language codes.'),
    '#field_prefix' => url('<front>', array('absolute' => TRUE)),
    '#required' => TRUE,
    '#default_value' => $config['path'],
  );

  $form['dir'] = array(
    '#type' => 'textfield',
    '#title' => t('AsciiDoc output directory'),
    '#description' => t('File system path, relative to Drupal or absolute, containing output of AsciiDoc processing. No trailing slash. This directory, or language sub-directories if translated, must contain an index.html file.'),
    '#required' => TRUE,
    '#default_value' => $config['dir'],
  );

  $form['copyright_file'] = array(
    '#type' => 'textfield',
    '#title' => t('Copyright file'),
    '#description' => t('File within the output directory containing the copyright notice.'),
    '#default_value' => $config['copyright_file'],
  );

  $form['translations'] = array(
    '#type' => 'fieldset',
    '#title' => t('Translations'),
    '#collapsible' => TRUE,
  );

  $form['translations']['translated'] = array(
    '#type' => 'radios',
    '#title' => t('Translated'),
    '#description' => t('If translated, source and output must be in directories named with two-letter language codes, and URLs will use those codes too. Note that this is not using the site language switching functionality.'),
    '#options' => array(
      1 => t('Translated'),
      0 => t('Single language'),
    ),
    '#default_value' => $config['translated'],
  );

  $form['translations']['primary_language'] = array(
    '#type' => 'textfield',
    '#title' => t('Primary language'),
    '#description' => t('If translated, language code of the language to show when you go to the book home page.'),
    '#default_value' => $config['primary_language'],
  );

  $form['translations']['source_language'] = array(
    '#type' => 'textfield',
    '#title' => t('Source language'),
    '#description' => t('If translated, language code of the source language, to show on edit pages if editing is enabled.'),
    '#default_value' => $config['source_language'],
  );

  $form['project'] = array(
    '#type' => 'fieldset',
    '#title' => t('Project information'),
    '#collapsible' => TRUE,
  );

  $form['project']['project_name'] = array(
    '#type' => 'textfield',
    '#title' => t('Name of project'),
    '#description' => t('Appears in edit block, if this and project URL are both provided.'),
    '#default_value' => $config['project_name'],
  );

  $form['project']['project_url'] = array(
    '#type' => 'textfield',
    '#title' => t('Project URL'),
    '#description' => t('Appears in edit block, if this and project name are both provided.'),
    '#default_value' => $config['project_url'],
  );

  $form['project']['issue'] = array(
    '#type' => 'textfield',
    '#title' => t('Base URL for filing an issue'),
    '#description' => t('Appears in edit block; assumed to work like Drupal.org issues. Example: https://drupal.org/node/add/project-issue/my_project_name'),
    '#default_value' => $config['issue'],
  );

  $form['editor'] = array(
    '#type' => 'fieldset',
    '#title' => t('Editing'),
    '#collapsible' => TRUE,
  );

  $form['editor']['source_path'] = array(
    '#type' => 'textfield',
    '#title' => t('Source file directory'),
    '#description' => t('File system path, relative to Drupal or absolute, containing the source files, or containing language sub-directories for translated books. May be read-only.'),
    '#default_value' => $config['source_path'],
  );

  $form['editor']['line_endings'] = array(
    '#type' => 'radios',
    '#title' => t('Line ending style'),
    '#description' => t('Used for editor output and patch generation.'),
    '#default_value' => $config['line_endings'],
    '#options' => array(
      'dos' => t('DOS'),
      'unix' => t('Unix/Linux'),
    ),
  );
  if (asciidoc_display_patches_allowed()) {
    $form['editor']['editor_result'] = array(
      '#type' => 'radios',
      '#title' => t('Editor result'),
      '#default_value' => $config['editor_result'],
      '#options' => array(
        'patch' => t('Download patch file'),
        'text' => t('Download replacement text file'),
      ),
    );
  }
  else {
    $form['editor']['editor_result'] = array(
      '#type' => 'value',
      '#default_value' => $config['editor_result'],
    );
  }

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save'),
  );

  return $form;
}

/**
 * Form submission handler for asciidoc_display_edit_form().
 */
function asciidoc_display_edit_form_submit($form, &$form_state) {
  // Read in the submitted config information. Note: Save it in unsanitized
  // form; sanitize it on output.
  $config = array(
    'config_id' => $form['#config_id'],
    'title' => $form_state['values']['title'],
    'translated' => $form_state['values']['translated'],
    'primary_language' => $form_state['values']['primary_language'],
    'path' => $form_state['values']['path'],
    'dir' => $form_state['values']['dir'],
    'copyright_file' => $form_state['values']['copyright_file'],
    'project_name' => $form_state['values']['project_name'],
    'project_url' => $form_state['values']['project_url'],
    'issue' => $form_state['values']['issue'],
    'source_path' => $form_state['values']['source_path'],
    'line_endings' => $form_state['values']['line_endings'],
    'editor_result' => $form_state['values']['editor_result'],
  );

  // Replace or add this in configuration.
  $old_configs = variable_get('asciidoc_configs', array());
  $new_configs = array($config);
  foreach ($old_configs as $old) {
    if ($old['config_id'] != $config['config_id']) {
      $new_configs[] = $old;
    }
  }

  variable_set('asciidoc_configs', $new_configs);

  // We need the menu system to be rebuilt.
  asciidoc_display_clear_calculation_caches();
  variable_set('menu_rebuild_needed', TRUE);

  drupal_set_message(t('Book settings updated'));
  $form_state['redirect'] = 'admin/config/development/asciidoc';
}

/**
 * Form constructor for a confirmation form for deleting a configuration.
 */
function asciidoc_display_delete_form($form, &$form_state, $config_id) {
  $config = asciidoc_display_load_config($config_id);
  if ($config['config_id'] != $config_id) {
    $form['not_found'] = array(
      '#markup' => t('The configuration item you are trying to delete was not found'),
    );
    return $form;
  }

  $form['#config_id'] = $config_id;

  return confirm_form($form, t('Are you sure you want to delete @title?', array('@title' => $config['title'])), 'admin/config/development/asciidoc');
}

/**
 * Form submission handler for asciidoc_display_delete_form().
 */
function asciidoc_display_delete_form_submit($form, &$form_state) {
  $config_id = $form['#config_id'];
  $new_configs = array();

  // Save all but the item we are deleting.
  $old_configs = variable_get('asciidoc_configs', array());
  foreach ($old_configs as $old) {
    if (isset($old['config_id']) && $old['config_id'] != $config_id) {
      $new_configs[] = $old;
    }
  }

  variable_set('asciidoc_configs', $new_configs);

  // We need the menu system to be rebuilt.
  asciidoc_display_clear_calculation_caches();
  variable_set('menu_rebuild_needed', TRUE);

  drupal_set_message(t('Book settings deleted'));

  $form_state['redirect'] = 'admin/config/development/asciidoc';
}


/**
 * Page callback: Clears the cache for this module.
 */
function asciidoc_display_clear_cache_page() {
  asciidoc_display_clear_calculation_caches();
  drupal_set_message(t('Book cache cleared'));
  drupal_goto('admin/config/development/asciidoc');
}

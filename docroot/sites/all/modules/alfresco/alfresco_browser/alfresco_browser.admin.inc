<?php

/**
 * @file
 * Administrative page callbacks for the alfresco browser module.
 *
 * @todo Añadir control de acceso para las diferentes acciones del navegador.
 */

// ------------------------------------------------------------------
// Alfresco browser settings page
// ------------------------------------------------------------------

/**
 * Form builder for the alfresco browser settings page.
 *
 * @see system_settings_form()
 */
function alfresco_browser_admin_settings() {
  $path = module_exists('libraries') ? libraries_get_path('ext') : 'sites/all/libraries/ext';
  if (!file_exists('./' . $path . '/ext-all.js') || !file_exists('./' . $path . '/adapter/ext/ext-base.js')) {
    drupal_set_message(t('Alfresco browser module requires the <a href="@extjs-url">Ext JS 2.x or 3.x</a>. Please download the Ext JS library, extract the archive and copy its contents to the following location: %extjs-path. Make sure the main Ext JS file is located at %extjs-all-path', array(
      '@extjs-url' => 'http://www.sencha.com/learn/Ext_Version_Archives',
      '%extjs-path' => $path,
      '%extjs-all-path' => $path . '/ext-all.js',
    )), 'error');
  }

  $form['browser'] = array(
    '#type' => 'fieldset',
    '#title' => t('Browser'),
  );

  $form['browser']['alfresco_browser_home'] = array(
    '#type'          => 'textfield',
    '#title'         => t('Home path'),
    '#description'   => t('Points to the root folder of Alfresco Repository (defaults to !company_home).', array('!company_home' => t('Company Home'))),
    '#required'      => TRUE,
    '#default_value' => variable_get('alfresco_browser_home', ALFRESCO_BROWSER_HOME),
  );

  $form['browser']['alfresco_browser_query_limit'] = array(
    '#type'          => 'textfield',
    '#title'         => t('Page size'),
    '#description'   => t('The number of records to display per page (defaults to 20).'),
    '#required'      => TRUE,
    '#default_value' => variable_get('alfresco_browser_query_limit', 20),
    '#size'          => 8,
  );

  $form['browser']['alfresco_browser_remember_folder'] = array(
    '#type'          => 'checkbox',
    '#title'         => t('Remember last folder used'),
    '#description'   => t('If selected, the Alfresco Browser will be opened in the last folder that the user accessed (defaults to true).'),
    '#required'      => TRUE,
    '#default_value' => variable_get('alfresco_browser_remember_folder', 1),
  );

  $form['browser']['alfresco_browser_search'] = array(
    '#type'          => 'radios',
    '#title'         => t('Search mode'),
    '#description'   => t('Select the search mode for search of the repository.'),
    '#options'       => array(0 => 'Disable', 1 => 'File names only', 2 => 'File names and contents'),
    '#default_value' => variable_get('alfresco_browser_search', 1),
  );

  $form['browser']['alfresco_browser_file_exists'] = array(
    '#type'          => 'radios',
    '#title'         => t('File exists'),
    '#description'   => t('Behavior when the destination file already exists.'),
    '#options'       => array(
      ALFRESCO_FILE_EXISTS_RENAME => 'Rename (appends number until name is unique)',
      ALFRESCO_FILE_EXISTS_REPLACE => 'Replace the existing file',
      ALFRESCO_FILE_EXISTS_ERROR => 'Return error',
    ),
    '#default_value' => variable_get('alfresco_browser_file_exists', ALFRESCO_FILE_EXISTS_RENAME),
  );

  /*
  $form['browser']['upload'] = array(
    '#type'          => 'fieldset',
    '#title'         => t('Add content'),
    '#description'   => t('Upload content to repository from Browser'),
    '#collapsible'   => TRUE,
    '#collapsed'     => FALSE,
  );

  $form['browser']['upload']['alfresco_browser_upload_enabled'] = array(
    '#type'          => 'checkbox',
    '#title'         => t('Enable upload'),
    '#description'   => t('Enable add content to Alfresco repository from browser.'),
    '#required'      => TRUE,
    '#default_value' => variable_get('alfresco_browser_upload_enabled', TRUE),
  );

  $form['browser']['upload']['alfresco_browser_required_properties'] = array(
    '#type'          => 'checkboxes',
    '#title'         => t('Content properties'),
    '#description'   => t('Enable add content to Alfresco repository from browser.'),
    '#options'       => array(
      'title' => t('Title'),
      'description' => t('Description'),
      'author' => t('Author'),
    ),
    '#default_value' => variable_get('alfresco_browser_required_properties', array()),
  );
  */

  $form['browser_cache'] = array(
    '#type' => 'fieldset',
    '#title' => t('Browser cache'),
  );

  $form['browser_cache']['alfresco_browser_cache_enabled'] = array(
    '#type'          => 'checkbox',
    '#title'         => t('Enable cache'),
    '#description'   => t('Enable cache for the Alfresco browser to store the results of web services queries against Alfresco repository.'),
    '#required'      => TRUE,
    '#default_value' => variable_get('alfresco_browser_cache_enabled', TRUE),
  );

  $form['#submit'][] = 'alfresco_browser_admin_settings_submit';
  return system_settings_form($form);
}

/**
 * Validate form values for alfresco browser settings page.
 */
function alfresco_browser_admin_settings_validate($form, &$form_state) {
  alfresco_include('soap');

  if (!alfresco_valid_path($form_state['values']['alfresco_browser_home'])) {
    form_set_error('alfresco_browser_home', t('The home path you specified is not valid.'));
  }
  elseif (!alfresco_soap_node_load($form_state['values']['alfresco_browser_home'])) {
    form_set_error('alfresco_browser_home', t('The home path you specified could not be found.'));
  }

  if (!is_numeric($form_state['values']['alfresco_browser_query_limit']) || $form_state['values']['alfresco_browser_query_limit'] < 1) {
    form_set_error('alfresco_browser_query_limit', t('Page size value must be a number and greater than zero.'));
  }
}

/**
 * Process admin alfresco browser settings form submissions.
 */
function alfresco_browser_admin_settings_submit($form, &$form_state) {
  // Limpiamos siempre la cache
  cache_clear_all(NULL, 'cache_alfresco_browser');
}

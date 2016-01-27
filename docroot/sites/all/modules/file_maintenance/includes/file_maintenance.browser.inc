<?php

/**
 *
 * @file
 * Code for the File browser.
 */

// $Id$

include_once('file_maintenance.admin.inc');

function file_maintenance_browser_form($form, &$form_state) {

  drupal_add_js(drupal_get_path('module', 'file_maintenance') . '/js/file_maintenance.js');

  drupal_add_library('system', 'drupal.ajax');
  drupal_add_library('system', 'ui.resizable');
  //drupal_add_library('system', 'jquery.form');

  /*
   * Help
   */
  $form['options'] = array(
    '#type' => 'fieldset',
    '#title' => t('Options'),
    //'#description' => '<strong>'.t('Browser options.').'</strong>',
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );
  $form['options']['help'] = array(
    '#type' => 'fieldset',
    '#title' => t('Help'),
    '#collapsible' => FALSE,
    '#collapsed' => FALSE,
    '#attributes' => array('class' => array('fieldset-help')),
  );

  $form['options']['help']['filesystem_settings'] = array(
    '#markup' =>
      t('<p>There will be some help text here later.</p><p>See also:</p><ul><li>!file_maintenance_settings</li><li>!file_maintenance_help</li><li>!filesystem_settings</li></ul>',
      array(
        '!file_maintenance_settings' => l(t('File maintenance settings'), 'admin/config/system/file-maintenance', array('attributes' => array('title' => t('File maintenance settings')))),
        '!file_maintenance_help' => l(t('File maintenance help'), 'admin/help/file_maintenance', array('attributes' => array('title' => t('File maintenance help')))),
        '!filesystem_settings' => l(t('File system settings'), 'admin/config/media/file-system', array('attributes' => array('title' => t('File system settings')))),
      )
    )
  );

  /*
   * General options
   */
  $form['options']['general_options'] = array(
    '#type' => 'fieldset',
    '#title' => t('General'),
    '#collapsible' => FALSE,
    '#collapsed' => FALSE,
    '#attributes' => array('class' => array('fieldset-settings')),

    //'#attributes' => array('class' => array('fieldset-help')),
  );

  // Show temporary directories
  $form['options']['general_options']['file_maintenance_show_temp_dirs'] = array(
    '#type' => 'checkbox',
    '#title' => t('Show temporary directories'),
    '#default_value' => variable_get('file_maintenance_show_temp_dirs', FILE_MAINTENANCE_SHOW_TEMP_DIRS),
    '#ajax' => array(
      'callback' => 'file_maintenance_ajax_change_option',
      'wrapper' => 'file-list-wrapper',
      'method' => 'replace',
      //'effect' => 'fade',
      'progress' => array('type' => 'throbber', 'message' => ''),
      'file_maintenance_refresh' => array('directory-tree', 'file-list'),
  ),

    //'#description' => t('Show image thumbnails.'),
  );

  // Recursive
  $form['options']['general_options']['file_maintenance_recursive'] = array(
    '#type' => 'checkbox',
    '#title' => t('Recursive'),
    '#default_value' => variable_get('file_maintenance_recursive', FILE_MAINTENANCE_RECURSIVE),
    //'#description' => t('Show recursive information about subdirectories.'),
  );

  // Files to display
  $form['options']['general_options']['file_maintenance_files_displayed'] = array(
    '#type' => 'radios',
    '#title' => t('Files to display'),
    '#description' => t('Which files should be displayed.'),
    '#options' => array(
     'all' => t('All files'),
     'filesystem_missing' => t('Files missing in the filesystem.'),
     'database_missing' => t('Files existing in filesystem only.'),
    ),
    //'#options' => _file_maintenance_range(),
    '#default_value' => variable_get('file_maintenance_files_displayed', FILE_MAINTENANCE_FILES_DISPLAYED),
    '#ajax' => array(
      'callback' => 'file_maintenance_ajax_change_option',
//      'wrapper' => 'directory-tree-wrapper',
//      'method' => 'replace',
      //'effect' => 'fade',
      'progress' => array('type' => 'throbber', 'message' => ''),
      'file_maintenance_refresh' => array('directory-tree', 'file-list'),
    ),

  );



  /*
   * File list columns
   */
  $form['options']['file_list_columns'] = array(
    '#type' => 'fieldset',
    '#title' => t('File list columns'),
    '#collapsible' => FALSE,
    '#collapsed' => FALSE,
    '#attributes' => array('class' => array('fieldset-settings', 'clearfix')),

    //'#attributes' => array('class' => array('fieldset-help')),
  );

  file_maintenance_add_file_list_column_option($form['options']['file_list_columns'], 'fid', 'FID');
  file_maintenance_add_file_list_column_option($form['options']['file_list_columns'], 'thumbnail', 'Thumbnail');
  file_maintenance_add_file_list_column_option($form['options']['file_list_columns'], 'dimensions', 'Dimensions');
  file_maintenance_add_file_list_column_option($form['options']['file_list_columns'], 'size', 'Size');
  file_maintenance_add_file_list_column_option($form['options']['file_list_columns'], 'filesystem', 'Filesystem');
  file_maintenance_add_file_list_column_option($form['options']['file_list_columns'], 'database', 'Database');
  file_maintenance_add_file_list_column_option($form['options']['file_list_columns'], 'status', 'Status');
  file_maintenance_add_file_list_column_option($form['options']['file_list_columns'], 'usage', 'Usage');

  $directories = _file_maintenance_get_directory_tree();

  $form['directories'] = array(
    'tree' => $directories,
    '#theme_wrappers' => array('directory_tree_wrapper'),
    //'#attributes' => array('class' => array('file-tree')),
    '#prefix' => '<div id="directory-tree-wrapper">',
    '#suffix' => '</div>'

  );

  $form['expanded_list'] = array(
    '#type' => 'hidden',
    '#attributes' => array('id' => array('expanded-list')),
  );

  if (!$form_state['rebuild']) {
    $_SESSION['FileManager']['CurrentURI'] = NULL;
  }

  $form['files'] = _file_maintenance_build_file_list($files, NULL);

  $form['#attributes'] = array('class' => array('file-form'));

  return $form;
}

function file_maintenance_add_file_list_column_option(&$parent, $machine_name, $name) {
  $parent['file_maintenance_file_list_column_' . $machine_name] = array(
    '#type' => 'checkbox',
    '#title' => t($name),
    '#default_value' => variable_get('file_maintenance_file_list_column_' . $machine_name, FILE_MAINTENANCE_FILE_LIST_COLUMN_),
    '#ajax' => array(
      'callback' => 'file_maintenance_ajax_change_option',
      'wrapper' => 'file-list-wrapper',
      'method' => 'replace',
      //'effect' => 'fade',
      'progress' => array('type' => 'throbber', 'message' => ''),
    ),

    //'#description' => t('Show image thumbnails.'),
  );
}

function template_preprocess_file_maintenance_tree_directory(&$variables) {

/*
  $link = l(check_plain($variables['dirname']), 'admin/file-maintenance/file-list',
    array(
      'attributes' => array('class' => array('use-ajax')),
      'query' => array(URI_QUERY_PARAMETER => $variables['fullpath'])
    )
  );
 */

  /*
   * Build directory name with ajax link.
   */
  $link = array(
    '#type' => 'link',
    '#title' => $variables['dirname'],
    '#href' => 'admin/file-maintenance/file-list',
    '#options' => array('query' => array(URI_QUERY_PARAMETER => $variables['uri'])),
    '#ajax' => array(
      // 'callback' => 'file_maintenance_update_file_list',
      // 'wrapper' => 'file-list-wrapper',
      'method' => 'replace',
      'effect' => 'fade',
      'progress' => array('type' => 'throbber', 'message' => ''),
    )
  );
  $variables['dirname'] = render($link);

  $variables['child_count'] = count($variables['subdirs']);

  $img_path = _file_maintenance_get_image_path();

  $suffix = count($variables['error']) ? '_error' : '';

  $config = array('path' => $img_path . '/' . $variables['icon'] . $suffix . '.png');
  $image = theme('image', $config);
  $variables['icon_element'] = render($image);

  if ($variables['child_count']) {
    $toggle_element = array(
      'plus' => theme('image',
        array(
          'path' => $img_path . '/bullet_toggle_plus.png',
          'attributes' => array(
            'class' => 'plus' . ($variables['expanded'] ? ' hidden' : '')
          ),
        )
      ),
      'minus' => theme('image',
        array(
          'path' => $img_path . '/bullet_toggle_minus.png',
          'attributes' => array(
            'class' => 'minus' . ($variables['expanded'] ? '' : ' hidden')
          ),
        )
      )
    );

    $variables['toggle_element'] = render($toggle_element['plus']) . render($toggle_element['minus']);
  }
  else {
    $variables['toggle_element'] = '';
  }

  $variables['filename_classes'] = implode(' ', _file_maintenance_get_filename_classes($variables));
}

function file_maintenance_ajax_change_option($form, $form_state) {

  if (isset($form_state['triggering_element']['#ajax']['file_maintenance_refresh'])) {
    $refresh = $form_state['triggering_element']['#ajax']['file_maintenance_refresh'];
    $refresh_directory_tree = in_array('directory-tree', $refresh);
    $refresh_file_list = in_array('file-list', $refresh);
  }
  else {
    $refresh_directory_tree = FALSE;
    $refresh_file_list = TRUE;
  }

  variable_set($form_state['triggering_element']['#name'], $form_state['triggering_element']['#value']);

  // TODO check if isset
  $uri = $_SESSION['FileManager']['CurrentURI'];

  $commands = array();

  if ($refresh_file_list) {
    /*
     * File list
     */
    $file_list = _file_maintenance_get_file_list($uri);

    $command = ajax_command_replace('#file-list-wrapper', render($file_list));
    //$command['effect'] = 'fade';
    $commands[] = $command;
  }

  if ($refresh_directory_tree) {
    /*
     * Directory tree
     */
    $expanded_list = isset($form_state['values']['expanded_list']) ? explode(',', $form_state['values']['expanded_list']) : array();

    $directories = _file_maintenance_get_directory_tree($expanded_list);

    $directory_tree = array(
      'tree' => $directories,
      '#theme_wrappers' => array('directory_tree_wrapper'),
      //'#attributes' => array('class' => array('file-tree')),
      '#prefix' => '<div id="directory-tree-wrapper">',
      '#suffix' => '</div>'

    );
    $command = ajax_command_replace('#directory-tree-wrapper', render($directory_tree));
    //$command['effect'] = 'fade';
    $commands[] = $command;
  }

  return array('#type' => 'ajax', '#commands' => $commands);

}

/**
 * Ajax callback to update the file list of the file browser.
 *
 * @param $type
 * @param $arg
 */
// TODO rename ajax
function file_maintenance_update_file_list() {
  global $_GET;

  $uri = $_GET[URI_QUERY_PARAMETER];
  $_SESSION['FileManager']['CurrentURI'] = $uri;

  $file_list = _file_maintenance_get_file_list($uri);

  $content = render($file_list);

  $commands[] = ajax_command_replace('#file-list-wrapper', $content);
  $commands[] = ajax_command_invoke('.tree-directory-row', 'removeClass', array('active'));
  $commands[] = ajax_command_invoke('#' . _file_maintenance_get_unique_css_id($uri), 'addClass', array('active'));
  $commands[] = ajax_command_invoke('#' . _file_maintenance_get_unique_css_id($uri) . ' a', 'blur', array());

  // TODO also put in initial page call
  $commands[] = ajax_command_settings(array('currentPath' => $uri), TRUE);

  $page = array('#type' => 'ajax', '#commands' => $commands);
  ajax_deliver($page);

}

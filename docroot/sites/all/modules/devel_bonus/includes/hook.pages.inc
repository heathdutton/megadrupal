<?php

/**
 * @file
 * This file will generate all page of devel/hook
 */

define('DEVEL_BONUS_FILE_SCAN', '.php,.module,.inc,.install');

/**
 * Implements hook_form().
 * Create form select module or dir to analytics
 */
function devel_bonus_hook_form($form, &$form_state) {
  $rows = array();
  // Build the sortable table header.
  $header = array(
    'title' => array('data' => t('Title'), 'field' => 'name'),
    'filename' => array('data' => t('Filename'), 'field' => 'filename'),
  );
  foreach (devel_bonus_hook_list_module($header) as $module) {
    $module->info = unserialize($module->info);
    if (!isset($module->info['hidden']) || $module->info['hidden'] !== TRUE) {
      $rows[$module->name] = array(
        'title' => $module->info['name'],
        'filename' => str_replace('/' . $module->name . '.module', '', $module->filename),
      );
    }
  }
  $form['modules'] = array(
    '#type' => 'tableselect',
    '#header' => $header,
    '#options' => $rows,
    '#default_value' => isset($form_state['values']) ? $form_state['values']['modules'] : array(),
    '#empty' => t('No modules available'),
  );
  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Generate function/hook'),
  );
  return $form;
}

function devel_bonus_hook_form_validate($form, &$form_state) {
  $selected = devel_bonus_hook_check_selected($form_state['values']['modules']);
  if (empty($selected)) {
    form_set_error('modules', t('Please select lastest one module'));
  }
  return $form;
}

function devel_bonus_hook_form_submit($form, &$form_state) {
  // @TODO check all function/hook using batch
  devel_bonus_hook_include();
  $selected = devel_bonus_hook_check_selected($form_state['values']['modules']);
  $infos = db_select('system', 's')
  ->fields('s', array('filename', 'name'))
  ->condition('name', $selected, 'IN')
  ->execute()
  ->fetchAll();
  $dirs = array();
  foreach ($infos as $info) {
    $dir_name = str_replace('/' . $info->name . '.module', '', $info->filename);
    $dir = new DirReader($dir_name, DEVEL_BONUS_FILE_SCAN);
    $files = $dir->getTree();
    $dirs += $files;
  }
  //$batch = devel_bonus_hook_batch($dirs);
  //batch_set($batch);
  $file = new HookParser('modules/aggregator/aggregator.parser.inc');
  debug($file->parse());
}
/**
 * Helper check module was selected
 */
function devel_bonus_hook_check_selected($modules) {
  $selected = array();
  foreach ($modules as $module => $value) {
    if (!empty($value)) {
      $selected[$module] = $value;
    }
  }
  return $selected;
}

function devel_bonus_hook_batch($tree) {
  $num_files = count($tree);
  drupal_set_message(t('Parse function/hook for @num file', array('@num' => $num_files)));
  $operations = array();
  // Setup an operation with $num_files task
  foreach ($tree as $file) {
    $operations[] = array('devel_bonus_hook_batch_op', array($file, t('(Parse file @filename)', array('@filename' => $file))));
  }
  $batch = array(
    'operations' => $operations,
    'finished' => 'devel_bonus_hook_batch_finished',
  );
  return $batch;
}

function devel_bonus_hook_batch_op($file_name, $details, &$context) {
  devel_bonus_hook_include();
  $file = new HookParser($file_name);
  db_insert('devel_bonus_hook')
  ->fields(array(
    'name' => $file_name,
    'content' => serialize($file->parse()),
  ))
  ->execute();
  $context['results'][] = t('Done file @file', array('@file' => $file_name));
  // Optional message displayed under the progressbar.
  $context['message'] = $details;
}

function devel_bonus_hook_batch_finished($success, $results, $operations) {
  if ($success) {
    // Here we could do something meaningful with the results.
    // We just display the number of nodes we processed...
    drupal_set_message(t('@file file was parsed.', array('@file' => count($results))));
  }
  else {
    // An error occurred.
    // $operations contains the operations that remained unprocessed.
    $error_operation = reset($operations);
    drupal_set_message(t('An error occurred while processing @operation with arguments : @args', array('@operation' => $error_operation[0], '@args' => print_r($error_operation[0], TRUE))));
  }
}

/**
 * Load all need file for generate hook
 */
function devel_bonus_hook_include() {
  module_load_include('inc', 'devel_bonus', 'includes/devel_bonus_dir');
}

/**
 * Helper for listing module, using db query faster than read file
 */
function devel_bonus_hook_list_module($header = array()) {
  $modules = db_select('system', 's')
  ->extend('TableSort')
  ->fields('s', array('filename', 'name', 'info'))
  ->condition('type', 'module')
  ->orderByHeader($header)
  ->execute()
  ->fetchAll();
  return $modules;
}

/**
 * Implements hook_menu().
 */
function devel_bonus_detect_all_hook_menu_page() {
  $header = array("path", "Declared by", 'Callback', "Callback Location");
  // Loop all module implement hook_menu().
  foreach (module_implements('menu') as $hook) {
    // Create new object base on ReflectionFunction()
    $func = new ReflectionFunction($hook . '_menu');
    // Get a linenumber of function
    $fun = $func->getStartLine();
    if ($results = $func->invoke()) {
      foreach ($results as $path => $item) {
        if (isset($item['file'])) {
          $row = array();
          $row[] = $path;
          $row[] = "Line " . $func->getStartLine() . " in " . $func->getFileName();
          if (isset($item['page callback']) && function_exists($item['page callback'])) {
            $row[] = $item['page callback'];
            $include_path = drupal_get_path('module', $hook);
            if (isset($item['file path'])) {
              $include_path = $item['file path'];
            }
            require_once($include_path . '/' . $item['file']);
            $callback = new ReflectionFunction($item['page callback']);
            $row[] = "Line " . $callback->getStartLine() . " in " . $callback->getFileName();
            $rows[] = $row;
          }
        }
      }
    }
  }

  return theme('table', array('header' => $header, 'rows' => $rows));
}

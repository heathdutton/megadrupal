<?php

function crush_command_enable($parsed_command) {
  $modules = $parsed_command['args'];
  $errors = array();
  $module_data = system_rebuild_module_data();
  foreach ($modules as $module) {
    if (module_exists($module)) {
      $errors[] = t('Warning: module !module is already enabled.', array('!module' => $module));
    }
    elseif (!isset($module_data[$module])) {
      $errors[] = t('Error: module !module not found.', array('!module' => $module));
    }
  }
  if (!module_enable($modules)) {
    $errors[] = t('Error: one or more modules or dependencies were missing.');
  }
  drupal_flush_all_caches();
  if (count($errors)) {
    return $errors;
  }
  else {
    return 'success';
  }
}


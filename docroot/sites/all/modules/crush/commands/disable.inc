<?php

function crush_command_disable($parsed_command) {
  $modules = $parsed_command['args'];
  $errors = array();
  foreach ($modules as $module) {
    if (!module_exists($module)) {
      $errors[] = t('Warning: module !module is not currently enabled.', array('!module' => $module));
    }
  }
  module_disable($modules);
  drupal_flush_all_caches();
  if (count($errors)) {
    return $errors;
  }
  else {
    return 'success';
  }
}


<?php

function crush_command_uninstall($parsed_command) {
  require_once 'includes/install.inc';
  $modules = $parsed_command['args'];
  $errors = array();
  $module_data = system_rebuild_module_data();
  foreach ($modules as $module) {
    if (!isset($module_data[$module])) {
      $errors[] = t('Error: module !module not found.', array('!module' => $module));
    }
    elseif (drupal_get_installed_schema_version($module) == SCHEMA_UNINSTALLED) {
      $errors[] = t('Warning: module !module has already been uninstalled or was never installed to begin with.', array('!module' => $module));
    }
    if (module_exists($module)) {
      module_disable(array($module));
    }
  }
  if (!drupal_uninstall_modules($modules)) {
    $errors[] = t('Error: an error occurred during uninstallation.');
  }
  if (count($errors)) {
    return $errors;
  }
  else {
    return 'success';
  }
}


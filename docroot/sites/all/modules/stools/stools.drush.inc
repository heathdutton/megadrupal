<?php

/**
 * Implements hook_drush_command().
 */
function stools_drush_command() {

  $items['re-deploy'] = array(
    'description' => 'Disable and deploy a module.',
    'aliases' => array('red'),
    'arguments' => array(
      'prefix' => 'Module prefix ex. gbs',
      'module' => 'Module machine name ex. page',
    ),
    'examples' => array(
      'drush red prefix page' => 'Disables module and enables the deploy module',
      'drush re-deploy prefix page' => 'Disables module and enables the deploy module',
    ),
  );

  return $items;
}

/**
 * Callback for the re-deploy command
 */
function drush_stools_re_deploy($prefix, $module) {
  $args = array_slice(drush_get_arguments(), 2);

  if (!isset($module)) {
    drush_print('You must have 2 arguments, ex. drush red gbs base');
    return;
  }

  foreach ($args as $arg) {
    $disable_module = $prefix . '_' . $arg;
    $deploy_module = $prefix . '_deploy';

    drush_print('Disabling ' . $disable_module . ' and enabling ' . $deploy_module);

    drush_invoke_process("@site", "dis", array($disable_module), array("--y"));
  }

  drush_invoke_process("@site", "en", array($deploy_module), array("--y"));

}

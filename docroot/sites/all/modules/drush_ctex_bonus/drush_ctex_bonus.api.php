<?php

/**
 * @file
 * Api file
 */

/**
 * Implements hook_drush_command().
 */
function drush_ctex_bonus_drush_command() {

  // This is in case you want to create a patch.

  $items['ctex-x'] = array(
    'aliases' => array('cbx'),
    'description' => 'Rebuild x.',
    'arguments' => array(
      'module'  => 'Name of your module.',
    ),
    'examples' => array(
      'drush x export_module' => 'Will search for a file called export_module.ctex_bonus.x.inc and rebuild x.',
    ),

    // Always use the drush_ctex_bonus_perform_action callback.
    // so we can do important checks
    'callback' => 'drush_ctex_bonus_perform_action',

    // Put it in the ctex namespace.
    'commandfile' => 'ctex',

    // Always add these options, no matter what.
    'options' => array(
      'result-file' => 'The name of the result file for database backups,',
      'ctex-log-location' => 'The name of the ctex log directory.',
    ),

    // Whether this command has export capabilities. The file
    // will be called module_name.ctex_bonus.x.inc. The callback
    // for the export function automatically becomes:
    // drush_ctex_bonus_export_x().
    'ctex export' => 'x',
    // In some circumstances though, you can tell it to not have
    // an export. The 'ctex export' key will be only used to load the file.
    'ctex no export' => TRUE,

    // Whether this command will be run when using the cbum command
    // or using the 'All' option when running the cbrm command.
    // Defaults to FALSE.
    'ctex run all' => TRUE,

    // Whether this command has a delete functionality.
    // Defaults to FALSE.
    'ctex delete' => TRUE,

    // Whether this command will write its code to the module file
    // in that case you are responsible to add it to the module file.
    // @see drush_ctex_bonus_export_styles().
    // Defaults to FALSE.
    'ctex write module file' => TRUE,

    // Whether to check for the 'ctex export' file or not.
    // Defaults to FALSE.
    'ctex no file check' => TRUE,

    // Whether this command has a module dependency.
    'ctex dependency' => 'taxonomy',

    // A whole different callback function.
    // Default to 'drush_ctex_bonus_rebuild_' . $command['ctex export']
    'ctex callback' => 'drush_ctex_bonus_compare_dependencies',
  );
}

/**
 * Examples of valid statements for a Drush runtime config (drushrc) file.
 */

// Array of items regexes to exclude from export.
// Export all items by setting to an empty array.
$options['drush_ctex_bonus_exclude_modules'] = array('coder', 'coder_review');
$options['drush_ctex_bonus_exclude_aliases'] = array('node' ,'user');
$options['drush_ctex_bonus_exclude_menus'] = array('devel', 'features');
$options['drush_ctex_bonus_exclude_variables'] = array();
$options['drush_ctex_bonus_exclude_themes'] = array('seven', 'rubik');


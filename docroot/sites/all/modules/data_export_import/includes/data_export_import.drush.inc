<?php
/**
 * @file
 * This file defines the Data Export Import module drush commands.
 *
 * To find out how to use the command run `drush help dei-export`
 * to see the command options.
 */

require_once "profiles/nodes.inc";
require_once "profiles/taxonomy_terms.inc";
require_once "profiles/users.inc";

/**
 * Implements hook_drush_command().
 *
 * This function specifies which commands the drush module has made
 * available.
 *
 * @return array
 *   An associative array describing your command(s).
 */
function data_export_import_drush_command() {

  $items = array();

  // The 'data_export_import-export' command.
  $items['data_export_import-export'] = array(
    'description' => "Exports datasets to file(s).",
    'arguments' => array(
      'dataset_profile' => 'The type of dataset to create; users, terms or nodes.',
    ),
    'options' => array(
      'content-types' => 'List of comma separated content types required when exporting nodes.',
    ),
    'examples' => array(
      'drush data_export_import-export terms' => 'Export taxonomy terms to a file.',
      'drush data_export_import-export nodes --content-types=page,article' => 'Export pages and articles to separate files.',
    ),
    'aliases' => array('dei-ex'),
    'bootstrap' => DRUSH_BOOTSTRAP_DRUPAL_LOGIN,
  );

  // The 'data_export_import-import' command.
  $items['data_export_import-import'] = array(
    'description' => "Imports Data Export Import dataset files.",
    'arguments' => array(
      'dataset_type' => 'Required - the type of dataset being imported; users, terms or nodes.',
    ),
    'options' => array(
      'file' => 'Required - filename of the file being imported (without the path).',
    ),
    'examples' => array(
      'drush data_export_import-import terms --file=20120223_170526_taxonomy_terms.dataset' => 'Import a dataset from a file.',
    ),
    'aliases' => array('dei-im'),
    'bootstrap' => DRUSH_BOOTSTRAP_DRUPAL_LOGIN,
  );

  return $items;
}

/**
 * Implements hook_drush_help().
 *
 * This function is called when a drush user calls 'drush help'. This
 * hook is optional. If a command does not implement this hook, the
 * command's description is used instead.
 *
 * This hook is also used to look up help metadata, such as help
 * category title and summary.
 *
 * @section string
 *   A string with the help section (prepend with 'drush:')
 *
 * @return string
 *   A string with the help text for your command.
 */
function data_export_import_drush_help($section) {

  switch ($section) {

    case 'drush:data_export_import-export':
      return dt("This command will export a dataset to a file.");

    case 'drush:data_export_import-import':
      return dt("This command will import a dataset file.");

    case 'meta:data_export_import:title':
      // The 'title' meta item is used to name a group of
      // commands in `drush help`.  If a title is not defined,
      // the default is "All commands in ___", with the
      // specific name of the commandfile (e.g. sandwich).
      // Command files with less than four commands will
      // be placed in the "Other commands" section, _unless_
      // they define a title.  It is therefore preferable
      // to not define a title unless the file defines a lot
      // of commands.
      return dt("Data Export Import commands");

    case 'meta:data_export_import:summary':
      // The 'summary' meta item is displayed in `drush help --filter`,
      // and is used to give a general idea what the commands in this
      // command file do, and what they have in common.
      return dt("Drush commands to carry out functions of the Data Export Import module.");
  }
}

/**
 * A drush command callback. This is where the action takes place.
 *
 * This function will handle exporting data profiles to data files -
 * it can be extended by the addition of further profiles.
 *
 * @profile string
 *   Which dataset profile to export.
 *
 * @return bool
 *   Set as TRUE if the function ran OK.
 */
function drush_data_export_import_export($dataset_profile) {

  switch ($dataset_profile) {

    case 'terms':
      $msg = data_export_import_export_taxonomy_terms_to_file();
      break;

    case 'users':
      $msg = data_export_import_export_users_to_file();
      break;

    case 'nodes':
      // Get the list of content types which should be exported.
      $list_of_content_types = drush_get_option('content-types');

      // Provide clean warning if content-types argument is not supplied.
      if (empty($list_of_content_types)) {
        drupal_set_message(t("No content types supplied."), 'warning');
        break;
      }

      $array_of_content_types = explode(',', $list_of_content_types);
      $msg = data_export_import_export_nodes_to_file_using_drush($array_of_content_types);
      break;

    default:
      $msg = "No valid argument was supplied.";

  }

  drush_print($msg);
  return TRUE;
}

/**
 * A drush command callback. This is where the action takes place.
 *
 * This function will handle the importing of dataset files.
 *
 * @dataset_file string
 *   The dataset file which will be imported.
 *
 * @return bool
 *   Set as TRUE if the function ran OK.
 */
function drush_data_export_import_import($dataset_type) {

  switch ($dataset_type) {

    case 'terms':
      $filename = drush_get_option('file');
      data_export_import_import_taxonomy_terms($filename);
      break;

    case 'users':
      $filename = drush_get_option('file');
      data_export_import_import_users($filename);
      break;

    case 'nodes':
      $filename = drush_get_option('file');
      data_export_import_import_all_content_types_using_drush($filename);
      break;

    default:
      $msg = "No valid argument was supplied to the dei-import command.";
      drush_print($msg);
  }

  return TRUE;
}

<?php

/**
 * Implements hook_drush_command().
 */
function pm_extras_drush_command() {
  $items = array();

  $items['pm-projectinfo'] = array(
    'description' => 'Show a report of available  projects and their modules.',
    'arguments' => array(
      'projects' => 'Optional. A list of installed projects to show.',
    ),
    'options' => array(
      'drush' => 'Optional. Only incude projects that have one or more Drush commands.',
      'status' => array(
        'description' => 'Filter by project status. Choices: enabled, disabled. A project is considered enabled when at least one of its extensions is enabled.',
        'example-value' => 'enabled',
      ),
    ),
    'outputformat' => array(
      'default' => 'key-value-list',
      'pipe-format' => 'list',
      'field-labels' => array(
        'label'      => 'Name',
        'type'       => 'Type',
        'version'    => 'Version',
        'status'     => 'Status',
        'extensions' => 'Extensions',
        'drush'      => 'Drush Commands',
        'datestamp'  => 'Datestamp',
        'path'       => 'Path',
      ),
      'fields-default' => array('label', 'type', 'version', 'status', 'extensions', 'drush', 'datestamp', 'path'),
      'fields-pipe' => array('label'),
      'output-data-type' => 'format-table',
    ),
    'aliases' => array('pmpi'),
  );

  return $items;
}

/**
 * Implements hook_drush_help().
 *
 * @param
 *   A string with the help section (prepend with 'drush:')
 *
 * @return
 *   A string with the help text for your command.
 */
function pm_extras_drush_help($section) {
  switch ($section) {
    case 'drush:pm-projectinfo':
      return dt("Brief help for Drush command pm-projectinfo.");
    // The 'title' meta item is used to name a group of
    // commands in `drush help`.  If a title is not defined,
    // the default is "All commands in ___", with the
    // specific name of the commandfile (e.g. pm_extras).
    // Command files with less than four commands will
    // be placed in the "Other commands" section, _unless_
    // they define a title.  It is therefore preferable
    // to not define a title unless the file defines a lot
    // of commands.
    case 'meta:pm_extras:title':
      return dt("pm_extras commands");
    // The 'summary' meta item is displayed in `drush help --filter`,
    // and is used to give a general idea what the commands in this
    // command file do, and what they have in common.
    case 'meta:pm_extras:summary':
      return dt("Summary of all commands in this command group.");
  }
}

/**
 * Implementation of drush_hook_COMMAND_validate().
 */
function drush_pm_extras_pm_projectinfo_validate() {
  $status = drush_get_option('status');
  if (!empty($status)) {
    if (!in_array($status, array('enabled', 'disabled'), TRUE)) {
      return drush_set_error('DRUSH_PM_INVALID_PROJECT_STATUS', dt('!status is not a valid project status.', array('!status' => $status)));
    }
  }
}

/**
 * Implementation of drush_hook_COMMAND().
 *
 * Main command implementation goes here.
 */
function drush_pm_extras_pm_projectinfo() {
  // Get specific requests.
  $requests = pm_parse_arguments(func_get_args(), FALSE);

  // Get installed extensions and projects.
  $extensions = drush_get_extensions();
  $projects = drush_get_projects($extensions);

  // If user did not specify any projects, return them all
  if (empty($requests)) {
    $result = $projects;
  }
  else {
    foreach ($requests as $name) {
      if (array_key_exists($name, $projects)) {
        $result[$name] = $projects[$name];
      }
    }
  }

  // Find the Drush commands that belong with each project.
  foreach ($result as $name => $project) {
    $drush_commands = pm_extras_commands_in_project($project);
    if (!empty($drush_commands)) {
      $result[$name]['drush'] = $drush_commands;
    }
  }

  // If user specified --drush, remove projects with no drush extensions
  if (drush_get_option('drush')) {
    foreach ($result as $name => $project) {
      if (!array_key_exists('drush', $project)) {
        unset($result[$name]);
      }
    }
  }


  // If user specified --status=1|0, remove projects with a distinct status.
  if (($status = drush_get_option('status', FALSE)) !== FALSE) {
    $status_code = ($status == 'enabled') ? 1 : 0;
    foreach ($result as $name => $project) {
      if ($project['status'] != $status_code) {
        unset($result[$name]);
      }
    }
  }

  return $result;
}

function pm_extras_commands_in_project($project) {
  $drush_commands = array();
  if (array_key_exists('path', $project)) {
    $commands = drush_get_commands();
    foreach ($commands as $commandname => $command) {
      if (!array_key_exists("is_alias", $command) && ($command['path'] == $project['path'])) {
        $drush_commands[] = $commandname;
      }
    }
  }
  return $drush_commands;
}

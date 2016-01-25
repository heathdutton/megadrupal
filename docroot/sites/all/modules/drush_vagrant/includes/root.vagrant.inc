<?php

/**
 * Prints the path to the current vagrant project, or defaults to printing the
 * path to the root projects directory.
 */
function drush_vagrant_root() {
  // PROJECT_PATH & PROJECTS_PATH are defined in vagrant_drush_init().

  // If we've provided an alias
  if (($alias = drush_get_context('alias', NULL) ||
      // or we're in a project directory
      drush_vagrant_check_project(PROJECT_PATH)) &&
      // and we haven't asked for the projects root
      drush_get_option('root') != 1) {
    // print and return the current project's path
    drush_print(PROJECT_PATH);
    return PROJECT_PATH;
  }
  // Otherwise, return the root projects path
    drush_print(PROJECTS_PATH);
    return PROJECTS_PATH;
}

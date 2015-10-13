<?php

/**
 * @file
 * Drush integration for the bloggit module.
 */

/**
 * Implements hook_drush_command().
 */
function bloggit_drush_command() {
  return array('bloggit-rescan' => array(
    'description' => dt('Rescan Bloggit entry files for changes.'),
  ));
}

/**
 * Command callback for rescanning Bloggit files.
 */
function drush_bloggit_rescan() {
  bloggit_process_files();
}


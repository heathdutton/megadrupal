<?php

/**
 * @file
 * This file handles metric management for the Drupal file management
 */

/**
 * Returns total amount of files.
 */
function drupalmonitor_connector_get_files_filescount() {
  $query = "SELECT count(*) FROM {file_managed}";
  return db_query($query)->fetchField();
}

/**
 * Returns sum of filespace usage.
 */
function drupalmonitor_connector_get_files_filesfoldersize() {
  $query = "SELECT sum(filesize) FROM {file_managed}";
  return db_query($query)->fetchField();
}

<?php

/**
 * @file
 * Node Translation Access model.
 */

/**
 * Check access.
 */
function node_translation_access_denied($nid, $content_type = NULL, $langcode = NULL) {
  if (!$langcode) {
    global $language;
    $langcode = $language->language;
  }
  $table =& drupal_static(__FUNCTION__, array());
  if (!$table) {
    $rows = db_query('SELECT nid, language FROM node_translation_access')->fetchAll();
    foreach ($rows as $row) {
      $table[$row->nid][$row->language] = $row->language;
    }
  }
  $status = FALSE;
  if (isset($table[$nid][$langcode])) {
    $status = TRUE;
  }
  $denied = variable_get('node_translation_access_content_types', array());
  if (!empty($denied[$content_type]) && $denied[$content_type][$langcode]) {
    $status = TRUE;
  }
  return $status;
}

/**
 * Check all denied languages.
 */
function node_translation_access_denied_languages($nid) {
  $languages =& drupal_static(__FUNCTION__, array());
  if (!isset($languages[$nid])) {
    $languages[$nid] = db_query('SELECT language FROM node_translation_access WHERE nid=:nid', array(':nid' => $nid))->fetchAllKeyed(0, 0);
  }
  return $languages[$nid];
}

/**
 * Save denied lanuguages.
 */
function node_translation_access_denied_save($node) {
  if (isset($node->denied_languages)) {
    $nid = $node->nid;
    $languages = $node->denied_languages;
    db_query('DELETE FROM node_translation_access WHERE nid=:nid', array(':nid' => $nid));
    foreach ($languages as $language) {
      if ($language) {
        db_query('INSERT INTO node_translation_access (nid, language) VALUES (:nid, :language)', array(':nid' => $nid, ':language' => $language));
      }
    }
  }
}

/**
 * Delete denied lanuguages.
 */
function node_translation_access_denied_delete($node) {
  db_query('DELETE FROM node_translation_access WHERE nid=:nid', array(':nid' => $node->nid));
}

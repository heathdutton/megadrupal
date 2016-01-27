<?php

/**
 * @file
 * Drush integration for Monster Menus.
 */

/**
 * Implements hook_drush_command().
 */
function monster_menus_drush_command() {
  $items['mm-check-sort'] = array(
    'description' => t('Check for problems in the way menu entries are sorted. See also mm-fix-sort.'),
    'aliases' => array('mm-check', 'mmcs'),
  );
  $items['mm-dump'] = array(
    'description' => t('Dump a section of the tree as CSV.'),
    'aliases' => array('mmd'),
    'arguments' => array(
      'mmtid' => 'The Tree ID of the page to start at',
    ),
    'required-arguments' => TRUE,
  );
  $items['mm-export'] = array(
    'description' => t('Export a section of the tree in a format that can be re-imported using admin/mm/import.'),
    'aliases' => array('mmx'),
    'arguments' => array(
      'mmtid' => 'The Tree ID of the page to start at',
    ),
    'required-arguments' => TRUE,
    'options' => array(
      'nodes' => 'Include nodes',
    ),
  );
  $items['mm-fix-sort'] = array(
    'description' => t('Check for and fix any problems in the way menu entries are sorted.'),
    'aliases' => array('mm-fix', 'mmfs'),
  );
  $items['mm-mark-vgroups'] = array(
    'description' => t('Mark all virtual groups as "dirty", so that they are regenerated during the next cron run.'),
    'aliases' => array('mm-mark', 'mmmv'),
  );
  $items['mm-update-vgroups'] = array(
    'description' => t('Update the membership of any "dirty" virtual groups immediately, instead of during cron.'),
    'aliases' => array('mm-update', 'mmuv'),
  );
  $items['mm-regenerate-vgroups'] = array(
    'description' => t('Update the membership of all virtual groups immediately.'),
    'aliases' => array('mm-regen', 'mmrv'),
  );
  return $items;
}

function drush_monster_menus_mm_check_sort() {
  module_load_include('inc', 'monster_menus', 'mm_admin');
  mm_admin_validate_sort_index(FALSE, 'drush_log');
}

function drush_monster_menus_mm_fix_sort() {
  module_load_include('inc', 'monster_menus', 'mm_admin');
  mm_admin_validate_sort_index(TRUE, 'drush_log');
  mm_content_update_sort_queue();
}

function drush_monster_menus_mm_regenerate_vgroups() {
  mm_content_update_vgroup_view();
  mm_regenerate_vgroup();
}

function drush_monster_menus_mm_mark_vgroups() {
  mm_content_update_vgroup_view();
}

function drush_monster_menus_mm_update_vgroups() {
  mm_regenerate_vgroup();
}

function drush_monster_menus_mm_dump($mmtid) {
  module_load_include('inc', 'monster_menus', 'mm_admin');
  mm_admin_dump($mmtid);
}

function drush_monster_menus_mm_export($mmtid) {
  global $user;
  module_load_include('inc', 'monster_menus', 'mm_import_export');
  $old_user = $user;
  drupal_save_session(FALSE);
  $user = user_load(1);
  print mm_export($mmtid, drush_get_option('nodes'));
  $user = $old_user;
  drupal_save_session(TRUE);
}

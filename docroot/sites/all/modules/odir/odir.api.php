<?php
/**
 * @file
 * Hook definitions of odir project
 */

/**
 * Use this hook for doing some preprocessing of directory.
 * The hook is called within hook_init().
 */
function hook_odir_directory_pre_processing($dir) {
  $_directory_node = odir_load_and_insert_by_path($dir);
}

/**
 * Implement this hook for defining odir based access controls.
 *
 * The array is automatically returned to hook_permission(),
 * as odir_control automatically falls back to drupal standard access control
 * if no odir access module is activated.
 */
function hook_odir_accessrules() {
  return array(
    'edit_permissions' => array(
      'title' => t('edit permissions'),
      'shortname' => t('EP'),
      'default_weight' => 100,
    ),
  );
}

/**
 * This hook allows modules to implement a display for different
 * image types. Actually used in odir_image
 *
 * @param string $file
 *   filepath
 * @param string $link
 *   URL to the file
 * @param string $delta
 *   Maybe  used for different functions (not used yet!)
 *
 * @return array
 *   an array used in _odir_filelist_details().
 */
function hook_odir_show_file_list_item($file, $link, $delta) {
  $content['content'] = "";
  $content['weight'] = 20;
  $content['file'] = $file;
  $content['link'] = odir_encode($link);
  return array($content);
}

/**
 * This hook allows modules to implement a display for different
 * non-image file types. Actually implemented in odir for linking
 * to files.
 *
 * @param string $file
 *   filepath
 *
 * @param string $link
 *   URL to the file
 *
 * @param string $delta
 *   Can be used for different functions (not used yet!)
 *
 * @return string
 *   An  array used in _odir_filelist_details().
 */
function hook_odir_file_list_noimages_item($file, $link, $delta) {
  static $i = 0;
  static $max_cols = 4;
  if (preg_match('/\.jpe?g$/i', $file)) {
    $content['content'] = "";
    if ($i++ >= $max_cols) {
      $i = 1;
    }
    $content['content'] .= _odir_image_show_image("odir_thumbnail_200", "odir_preview_1000", $link);
    $content['weight'] = 1;
    $content['file'] = $file;
    $content['link'] = $link;
    return array($content);
  }
  return NULL;
}


/**
 * hook_odir_control().
 *
 * This hook lets developers add access control checks
 * based on odir directories.
 *
 * @param string $rule
 *   The name of an odir access control rule.
 *
 * @param string $odir_path
 *   The odir directory path on which to check the permission.
 *
 * @return array(boolean)
 *   An array with exactly one boolean value indicating
 */
function hook_odir_control($rule, $odir_path = "") {
  global $user;
  if ($user->uid == 1) {
    return array(TRUE);
  }

  $odir_path = odir_decode($odir_path);
  $odir_path_parent = odir_get_parent($odir_path);

  $groupids = $user->roles;
  $perms_parent = odir_access_read_and_calc($groupids, $odir_path_parent);
  $perms = odir_access_read_and_calc($groupids, $odir_path, $perms_parent);

  if ($rule == "view") {
    if (property_exists($perms, 'view_files') && $perms->view_files == ODIR_ACCESS_PATH_ALLOW ||
      $perms->view_nodes == ODIR_ACCESS_PATH_ALLOW) {
        return array(TRUE);
    }
    elseif (property_exists($perms, 'view_files') && $perms->view_files == ODIR_ACCESS_PATH_DENY ||
      $perms->view_nodes == ODIR_ACCESS_PATH_DENY) {
      return array(FALSE);
    }
  }
  else {
    if (property_exists($perms, $rule) && $perms->$rule == ODIR_ACCESS_PATH_ALLOW) {
      return array(TRUE);
    }
    elseif (property_exists($perms, $rule) && $perms->$rule == ODIR_ACCESS_PATH_DENY) {
      return array(FALSE);
    }
  }
  return array();
}

/**
 * Enables modules to modify configuration of odir blocks.
 * Invoked by an implementation of hook_block_configure().
 */
function hook_odir_block_configure($delta) {
  $form = array();
  $options = array();
  $rows = odir_field_load_fields('odir_field');
  foreach ($rows as $r) {
    $options[$r->field_name] = $r->field_name;
  }
  switch ($delta) {
    case 'folder operations':
      $form['odir_block_fieldname'] = array(
        '#type' => 'select',
        '#title' => t('Associate a field'),
        '#size' => 1,
        '#options' => $options,
        '#default_value' => variable_get('odir_block_fieldname', $rows[0]->field_name),
      );
      break;

    case 'folder list':
      $form['odir_block_fieldname'] = array(
        '#type' => 'select',
        '#title' => t('Associate a field'),
        '#size' => 1,
        '#options' => $options,
        '#default_value' => variable_get('odir_block_fieldname', $rows[0]->field_name),
      );
      break;
  }
  return $form;
}

/**
 * Enables modules to modify configuration of odir blocks.
 * Invoked by an implementation of hook_block_save().
 */
function hook_odir_block_save($delta, $edit) {
  switch ($delta) {
    case 'folder operations':
      variable_set('odir_block_fieldname', $edit['odir_block_fieldname']);
      break;

    case 'folder list':
      variable_set('odir_block_fieldname', $edit['odir_block_fieldname']);
      break;
  }
}

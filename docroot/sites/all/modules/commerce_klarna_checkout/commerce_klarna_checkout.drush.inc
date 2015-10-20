<?php

/**
 * @file
 * Drush commands for commerce_klarna_checkout.
 */

/**
 * Implements hook_drush_command().
 */
function commerce_klarna_checkout_drush_command() {
  $items['kco-php-dl'] = array(
    'description' => dt('Download the Klarna Checkout PHP Library'),
  );
  return $items;
}

/**
 * Drush command callback.
 */
function drush_commerce_klarna_checkout_kco_php_dl() {
  $path = drush_get_context('DRUSH_DRUPAL_ROOT') . '/sites/all/libraries/klarna_checkout';
  // Use a semi unique name, just in case we end up running this command again.
  $temp_path = drush_find_tmp() . '/kco_temp_' . time();
  // Simplest way to check out a github repo without getting the .git folder.
  $dl_cmd = 'svn export https://github.com/klarna/kco_php/trunk ' . $temp_path;
  $mv_cmd = 'mv ' . $temp_path . '/src/Klarna ' . $path;
  if (drush_shell_exec($dl_cmd)) {
    // Cool.
  }
  else {
    drush_log(dt('Drush was unable to download Klarna Checkout PHP Library to !temp_path.', array('!temp_path' => $temp_path)), 'error');
    drush_log(dt('Attempted command: !dl_cmd.', array(
      '!dl_cmd' => $dl_cmd,
    )), 'error');
    return;
  }
  // Then try to move the files in their place.
  if (drush_shell_exec($mv_cmd)) {
    drush_log(dt('Klarna was downloaded to !path.', array('!path' => $path)), 'success');
    // Try to clean up afterwards.
    _commerce_klarne_checkout_delete_temp_files($temp_path);
  }
  else {
    drush_log(dt('Drush was unable to move downloaded files from !temp_path to !path.', array(
      '!temp_path' => $temp_path,
      '!path' => $path,
    )), 'error');
    drush_log(dt('Attempted command: !mv_cmd.', array(
      '!mv_cmd' => $mv_cmd,
    )), 'error');
    // Try to clean up.
    _commerce_klarne_checkout_delete_temp_files($temp_path);
  }
}

/**
 * Helper function to clean up temporary files.
 */
function _commerce_klarne_checkout_delete_temp_files($temp_path) {
  $del_cmd = 'rm -rf ' . $temp_path;
  if (drush_shell_exec($del_cmd)) {
    // All good.
  }
  else {
    drush_log(dt('Failed to delete temporary files after downloading files. Command: !cmd.', array('!cmd' => $del_cmd)), 'warning');
  }
}

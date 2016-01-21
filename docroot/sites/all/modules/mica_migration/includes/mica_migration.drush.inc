<?php
/**
 * @file
 * Drush commands  implementations to help users download required libraries.
 * */

/**
 * Implements hook_drush_command().
 */
function mica_migration_drush_command() {
  $items['get-protobuf'] = array(
    'callback' => 'mica_migration_drush_protobuf',
    'aliases' => 'protolib',
    'bootstrap' => DRUSH_BOOTSTRAP_DRUSH,
    'description' => dt('Get the Protobuf library'),
  );

  return $items;
}

/**
 * Call back function to get un download Protobuf lib.
 */
function mica_migration_drush_protobuf() {
  $tmp = NULL;
  $path = drush_get_context('DRUSH_DRUPAL_ROOT');
  $library = 'obiba-mica-protos';
  // Create the path if it does not exist.
  if (!is_dir($path . '/sites/all/libraries')) {
    drush_op('mkdir', $path . '/sites/all/libraries');
  }
  if (!is_dir($path . '/sites/all/libraries/' . $library)) {
    drush_op('mkdir', $path . '/sites/all/libraries/' . $library);
    drush_log(dt('Directory @path was created', array('@path' => $path)), 'notice');
  }
  if (!is_dir($path . '/sites/all/libraries/' . $library . '/tmp')) {
    drush_op('mkdir', $path . '/sites/all/libraries/' . $library . '/tmp');
    drush_log(dt('temporary Directory @path was created', array('@path' => $path)), 'notice');
    $tmp = $path . '/sites/all/libraries/' . $library . '/tmp';
  }
  else {
    drush_log('No dir @path', array('@path' => $path), 'notice');
  }
  $path = $path . '/sites/all/libraries/' . $library;
  if ($tmp) {
    chdir($tmp);
    $get_cmd = 'wget https://github.com/obiba/obiba-mica-protos/archive/1.0.zip';
    if (drush_shell_exec($get_cmd)) {
      drush_log(dt('Protobuf lib was downloaded to !path.', array('!path' => ' ' . $tmp)), 'success');
      $unzip_cmd = 'unzip 1.0.zip -d ' . $tmp;
      if (drush_shell_exec($unzip_cmd)) {
        drush_log(dt('Protobuf Unzipped  to !path.', array('!path' => ' ' . $tmp)), 'success');
        $path_to_del = $tmp . '/obiba-mica-protos-1.0';
        chdir($path_to_del);
        $mv_cmd = "mv * ../../";
        if (drush_shell_exec($mv_cmd)) {
          drush_log(dt('Protobuf archive moved to   !path.', array('!path' => ' ' . $path)), 'success');
          chdir($path);
          $rm_cmd = "rm " . '-rf ' . $tmp;
          if (drush_shell_exec($rm_cmd)) {
            drush_log(dt('Protobuf archive deleted !path.', array('!path' => ' ' . $tmp)), 'success');
          }
        }

      }
    }
  }

}

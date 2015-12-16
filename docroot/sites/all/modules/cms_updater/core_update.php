<?php
/**
 * @file
 * Receiver for calls from CMS Updater service (https://cms-updater.com) - Create updates of Drupal core.
 */

require_once 'cmsupdaterclasses/CMSUpdateManager.php';

// INITIALIZATION
$cms_update_manager = new CMSUpdateManager(CMS_UPDATER_CLASS_PREFIX);
$cms_updater = $cms_update_manager->get_cmsupdater();
$cms_updater->set_update_manager($cms_update_manager);

// Set all cms specific and site specific local data like, path,key, domain, current version etc.
$cms_updater->set_local_data_from_cms();
// Find files path to use
$files_path = $cms_updater->get_files_path();
// Set log filename
$log_filename = $files_path . '/cms_updater/' . '_cms_updater.log';
// Get post data
$post_data = $cms_update_manager->get_post_data();
// Get absolute root path
$root_dir = $cms_update_manager->get_docroot();
// Check if the root is a valid Drupal root
$valid_root = $cms_updater->check_dir_drupal_root($root_dir);

// Set new log filename by update id
if ($update_id = $cms_update_manager->get_updatenode_nid()) {
  $log_filename_id = $files_path . '/cms_updater/' . $update_id . '/' . $update_id . '_cms_updater.log';
  // If there was no update id set (before update process starts), create new filename and merge content
  if (file_exists($log_filename)) {
    $cms_update_manager->set_logfilename($log_filename);
    $arr1 = $cms_update_manager->log;
    $arr2 = $cms_update_manager->read_log();
    $cms_update_manager->log = array_merge($arr1, $arr2);
    unlink($log_filename);
  }
  // Set new filename and set log content
  $cms_update_manager->set_logfilename($log_filename_id);
  $cms_update_manager->log = $cms_update_manager->read_log();
}
else {
  // Set log filename without update id
  $cms_update_manager->set_logfilename($log_filename);
  $cms_update_manager->log = $cms_update_manager->read_log();
}

// Stop update if the defined root is no valid Drupal root
if (!$valid_root) {
  $cms_update_manager->log['PRE_START_ERROR'] = 'NO_VALID_ROOT_DIR';
  $cms_update_manager->send_site_update_end(0, $cms_update_manager->log);

  exit("No valid CMS-Root");
}

// Set some defaults
$cms_updater->set_root_dir($root_dir);
$file_basename = basename($post_data->url, ".tar.gz");
$extract_path = $cms_updater->get_temppath() . '/cms_updater';
$curl_url = $cms_updater->sanitizePath($cms_update_manager->get_domain()) . '/'
  . $cms_updater->sanitizePath($cms_update_manager->get_local_path()) . '/' . basename(__FILE__);
$cms_update_manager->set_own_curl_url($curl_url);

// Update process steps
if (isset($post_data->step)) {
  // Check if the submitted license key is equal the local license key
  if ($post_data->key == $cms_update_manager->get_local_key()) {
    switch ($post_data->step) {
      // Start Backup
      case CMSUPDATER_BACKUP:
        $cms_update_manager->log['CMSUPDATER_BACKUP_START'] = time();
        $cms_update_manager->write_log();
        // Define backup folder and create if not exists
        $backup_folder = $cms_updater->get_files_path() . '/cms_updater/' . $update_id;
        if (!file_exists($backup_folder)) {
          mkdir($backup_folder, 0755, TRUE);
        }
        // Write some log info
        $cms_update_manager->log['root'] = $root_dir;
        $cms_update_manager->write_log();
        $cms_update_manager->log['CMSUPDATER_BACKUP_FILES_START'] = time();
        // Set backup destination and backup Drupal core files
        $backup_destination = $files_path . '/cms_updater/' . $update_id . '/' . $update_id . '-backupfiles';
        $result_backup_files = $cms_updater->backup_files($root_dir, $backup_destination);

        $cms_update_manager->log['CMSUPDATER_BACKUP_FILES_END'] = time();
        $cms_update_manager->log['CMSUPDATER_BACKUP_FILES'] = $result_backup_files;
        // Stop update process if something fails
        if ($result_backup_files['success'] === FALSE) {
          $cms_update_manager->write_log();
          $cms_update_manager->send_site_update_end(0, $cms_update_manager->log);
          exit("END");
        }

        $cms_update_manager->log['CMSUPDATER_BACKUP_DB_START'] = time();

        // Backup database
        $result_backup = $cms_updater->backup_db();

        $cms_update_manager->log['CMSUPDATER_BACKUP_END'] = time();
        $cms_update_manager->log['CMSUPDATER_BACKUP_DB'] = $result_backup;

        // If backup file exists, proceed:
        $backup_files = $backup_folder . '/' . $update_id . '-backupfiles';
        if (file_exists($backup_files . '.zip') || file_exists($backup_files)) {
          $cms_update_manager->log['CMSUPDATER_BACKUP'] = array('success' => TRUE);
          $cms_update_manager->write_log();
          exit("CMSUPDATER_DOWNLOAD_NEW_CORE");
        }
        else {
          $cms_update_manager->log['CMSUPDATER_BACKUP'] = array('success' => FALSE);
          $cms_update_manager->write_log();

          // The backup file does not exist, something went wrong
          $cms_update_manager->log['CMSUPDATER_BACKUP_END'] = time();
          $cms_update_manager->write_log();
          $cms_update_manager->send_site_update_end(0, $cms_update_manager->log);
        }
        break;

      case CMSUPDATER_DOWNLOAD_NEW_CORE:
        $cms_updater->rrmdir($extract_path);
        // Write some log information
        $cms_update_manager->log = $cms_update_manager->read_log();
        $cms_update_manager->log['CMSUPDATER_DOWNLOAD_NEW_CORE_URL'] = $post_data->url;
        $cms_update_manager->log['CMSUPDATER_DOWNLOAD_NEW_CORE_MD5HASH'] = $post_data->md5hash;
        $cms_update_manager->log['CMSUPDATER_DOWNLOAD_NEW_CORE_START'] = time();
        $cms_update_manager->write_log();

        $extracted_to = $cms_updater->get_cms_core($extract_path, $post_data->files);
        $cms_update_manager->log['CMSUPDATER_DOWNLOAD_NEW_CORE_END'] = time();

        // Check extracted Drupal core before moving
        if (file_exists($cms_updater->sanitizePath($extracted_to) . '/' . $file_basename)) {
          $cms_update_manager->log['CMSUPDATER_DOWNLOAD_NEW_CORE']['success'] = TRUE;
          $cms_update_manager->write_log();
          exit("CMSUPDATER_MOVE_FILES");
        }
        else {
          $cms_update_manager->log['CMSUPDATER_DOWNLOAD_NEW_CORE']['success'] = FALSE;
          $cms_update_manager->write_log();
          $cms_update_manager->send_site_update_end(0, $cms_update_manager->log);
        }
        break;

      case CMSUPDATER_MOVE_FILES:
        $cms_update_manager->log = $cms_update_manager->read_log();
        $cms_update_manager->log['CMSUPDATER_MOVE_FILES_START'] = time();

        // Now move the files from the extract folder to its destination to actually replace the cms core files:
        // but first just, check:
        $result1 = $cms_updater->move_core_files($extract_path . '/' . $file_basename, $root_dir, TRUE);

        // Everything writable? Ok, then lets actually do it.
        if ($result1['success'] !== FALSE) {
          $cms_update_manager->log['CMSUPDATER_MOVE_FILES'] = array('check_permissions' => array('success' => FALSE));

          $cms_update_manager->write_log();

          $result2 = $cms_updater->move_core_files($extract_path . '/' . $file_basename, $root_dir, FALSE);
          $cms_update_manager->log['CMSUPDATER_MOVE_FILES_END'] = time();

          if ($result2['success'] == TRUE) {
            $cms_update_manager->log['CMSUPDATER_MOVE_FILES'] = array(
              'check_permissions' => array('success' => TRUE),
              'move_files'        => array('success' => TRUE),
              'report'            => $result2,
            );
            $cms_update_manager->write_log();
            // Now upgrade db:
            exit("CMSUPDATER_UPDATE_DB");
          }
          else {
            $cms_update_manager->log['CMSUPDATER_MOVE_FILES'] = array(
              'check_permissions' => array('success' => FALSE),
              'move_files'        => array('success' => FALSE),
              'report'            => $result2,
            );
            $cms_update_manager->write_log();
            $cms_update_manager->send_site_update_end(0, $cms_update_manager->log);
          }
        }
        else {
          $cms_update_manager->log['CMSUPDATER_MOVE_FILES'] = (array(
            'checkpermissions' => array('success' => FALSE),
            'report'           => $result1,
          ));
          $cms_update_manager->write_log();

          // When writability check of moving all files fails e.g., some where not writable:
          // send site update_end failure:
          $cms_update_manager->send_site_update_end(0, $cms_update_manager->log);
        }

        break;

      case CMSUPDATER_UPDATE_DB:
        $cms_update_manager->log = $cms_update_manager->read_log();
        $cms_update_manager->log['CMSUPDATER_UPDATE_DB_START'] = time();
        $cms_update_manager->write_log();

        // Now after source files are exchanged, do the DB updates:
        $cms_update_manager->log['CMSUPDATER_UPDATE_DB'] = $cms_updater->update_db();
        $cms_update_manager->log['CMSUPDATER_UPDATE_DB_END'] = time();
        $cms_update_manager->log['SENT_SITE_UPDATE_END'] = TRUE;
        $cms_update_manager->write_log();

        $cms_updater->set_update_success(TRUE);
        $cms_updater->set_last_update();

        $cms_update_manager->send_site_update_end(1, $cms_update_manager->log);
        exit("CMSUPDATER_CLEAN_UP");
        break;

      case CMSUPDATER_CLEAN_UP:
        $cms_updater->cleanup_old_update_data();
        // rrmdir backupfiles if zip exists:
        // necessary only when not using php zip Extension
        if (!class_exists('ZipArchive')) {
          $backup_destination = $cms_updater->get_files_path() . '/cms_updater/' .
            $cms_update_manager->get_updatenode_nid() . '/' .
            $cms_update_manager->get_updatenode_nid() . '-backupfiles';
          if (file_exists($backup_destination) && file_exists($backup_destination . '.zip')) {
            $cms_updater->rrmdir($backup_destination);
          }
        }
        exit("END");
        break;

      default:
        exit;
    }
  }
}
else {
  // Step zero - Start
  $cms_update_manager->set_desired_version($post_data->version);
  $url_check_res = $cms_updater->check_download_url($post_data->url);

  if (!$url_check_res) {
    $cms_update_manager->log = $cms_update_manager->read_log();
    $cms_update_manager->log['DOWNLOAD_URL_CHECK']['SUCCESS'] = FALSE;
    $cms_update_manager->log['DOWNLOAD_URL_CHECK']['URL'] = $post_data->url;
    $cms_update_manager->write_log();
    $cms_update_manager->send_site_update_end(0, $cms_update_manager->log);
    exit("END");
  }

  $cms_updater->set_download_url($post_data->url);

  // Check key
  if ($post_data->key == $cms_update_manager->get_local_key()) {

    // Double check:
    $check_key_result = $cms_update_manager->send_check_key();
    if (intval($check_key_result[0]) > 0) {
      $cms_update_manager->set_website_nid($check_key_result);

      // Check if local version needs a update
      if (version_compare($cms_update_manager->get_desired_version(), $cms_update_manager->get_current_version()) > 0) {


        // Start the update process
        $update_nid = $cms_update_manager->send_site_update_start();
        $cms_update_manager->set_updatenode_nid($update_nid);

        // Check if the log could be created in the drupal files system
        if (!is_writable($files_path)) {
          $cms_update_manager->log['PRE_START_ERROR'] = 'Files Path is not writable ->' . $files_path;
          $cms_update_manager->send_site_update_end(0, $cms_update_manager->log);
          // Stop the curl request
          exit("END");
        }
        // Init log
        $log_folder = $files_path . '/cms_updater/' . $cms_update_manager->get_updatenode_nid();
        $log_filename = $log_folder . '/' . $cms_update_manager->get_updatenode_nid() . '_cms_updater.log';
        $cms_update_manager->set_logfilename($log_filename);

        // Create folder for log and backups
        if (!file_exists($log_folder)) {
          //if using public files folder, add htaccess with deny from all:

          $result = mkdir($log_folder, 0775, TRUE);
          if (!$result) {
            $cms_update_manager->log['PRE_START_ERROR'] = array(
              'error'      => 'could not create dir to store backups',
              'files_path' => $files_path
            );
            $cms_update_manager->send_site_update_end(0, $cms_update_manager->log);
            // Stop the curl request
            exit("END");
          }
        }
        if (!file_exists($files_path . '/cms_updater/.htaccess')) {
          $cms_updater->add_htaccess_to_log_parent();
        }
        // Log start time of update
        $cms_update_manager->log['CMSUPDATER_START'] = array('time' => time());

        $cms_update_manager->write_log();

        $cms_update_manager->send_call_myself(CMSUPDATER_BACKUP);
      }
      else {
        // If the local Drupal is newer as the value on the CMS Updater service, tell the service the actual version.
        $res = $cms_update_manager->send_site_set_version();
      }
    }
  }
}



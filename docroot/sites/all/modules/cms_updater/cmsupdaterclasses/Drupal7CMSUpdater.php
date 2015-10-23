<?php
/**
 * @file
 * Contains Drupal7CMSUpdater
 */
require_once 'BasicCMSUpdater.php';
require_once 'CMSUpdateManager.php';
require_once dirname(dirname(__FILE__)) . '/cms_updater.functions.inc';

/**
 * Class Drupal7CMSUpdater
 */
class Drupal7CMSUpdater extends BasicCMSUpdater {

  /**
   * Backup Drupal core files before copying new Drupal 7 core files:
   * - if zip extension enabled as zip otherwise uncompressed
   * @param $source
   * @param $destination
   * @return array|bool|mixed
   */
  public function backup_files($source, $destination) {
    if (!file_exists($source)) {
      return array('success' => FALSE, 'error' => 'source does not exist', 'source' => $source);
    }
    // Include all Drupal 7 core files without the sites folder
    // We do this to ensure that we only backup Drupal core files and not some other dirs in the Drupal root.
    // This is necessary to get no php time out if there are huge folders in the root like cache or .git
    $include = _cms_updater_get_drupal_root_file_structure();

    $zip_extension = class_exists('ZipArchive');
    if ($zip_extension) {
      $zip = new ZipArchive();
      $destination .= '.zip';
      if (!$zip->open($destination, ZIPARCHIVE::CREATE)) {
        return array('success' => FALSE, 'error' => 'could not open/create zip', 'zip' => $destination);
      }
      // Copy only selected files and dirs
      $ret = _cms_updater_recurse_zip($source, '', $exclude = array(), $include, $zip);
      if (is_array($ret)) {
        $zip->close();
        return $ret;
      }
      $zip->close();
    }
    else {
      // No zip extension - Copy only selected files and dirs
      $ret = _cms_updater_recurse_copy($source, $destination, $exclude = array(), $include);
      if (is_array($ret)) {
        return $ret;
      }
      // Use pclzip to zip the whole backup directory:
      require_once('pclzip/pclzip.lib.php');

      $zipfile = new PclZip($destination . '.zip');
      $v_list = $zipfile->create($destination, '', $destination);

      if ($v_list == 0) {
        $ret = array(
          'success'    => FALSE,
          'error'      => 'could not create backup',
          'error_code' => $zipfile->errorInfo(TRUE)
        );
        return $ret;
      }
      else {
        foreach ($v_list as $kv_list => $v_list_item) {
          if ($v_list_item['status'] <> 'ok' && $v_list_item['filename'] <> $destination) {
            $ret = array('success' => FALSE, 'error' => 'could not backup a file', 'error_item' => $v_list_item);
            return $ret;
          }
        }
        unset($zipfile);
        $this->rrmdir($destination);
      }
    }

    return array('success' => TRUE);
  }

  /**
   * Move core files
   *
   * @param $source
   * @param $target
   * @param bool $justcheck - when true the function will just check writabillity
   * @return mixed - result_array('success'=>1|0,'erroronfiles'=>filenames
   */
  function move_core_files($source, $target, $justcheck = FALSE) {
    $source = $this->sanitizePath($source);
    $target = $this->sanitizePath($target);
    $files = scandir($source);

    static $retarray = array(
      'success'        => TRUE,
      'erronfiles'     => array(),
      'successonfiles' => array(),
    );

    foreach ($files as $fname) {
      $target_file_name = $target . '/' . $fname;
      $source_file_name = $source . '/' . $fname;
      if ($fname != '.' && $fname != '..' && $fname != 'sites' && $fname != 'profiles' && $fname != '.htaccess' && $fname != 'robots.txt' && $fname != '.gitignore') {
        $fileownerid = fileowner($source_file_name);
        if (function_exists('posix_getpwuid')) {
          $fileownerinfo = posix_getpwuid($fileownerid);
        }
        else {
          // File owner of current script
          $fileownerinfo = array('name' => 'not determindable');
        }

        if (!is_dir($source_file_name)) {
          if ($justcheck) {
            if (file_exists($target_file_name)) {
              $ecp = is_writable($target_file_name);
              if ($ecp === FALSE) {
                $ecp = $this->is_writable_chmod($target_file_name);

              }
            }
            else {
              $ecp = TRUE;
            }
          }
          else {
            $ecp = $this->copy_chmod($source . '/' . $fname, $target . '/' . $fname);
          }

          if (!$ecp) {
            $error_case_file_perms = $this->getFilePermission($target . '/' . $fname);
            $retarray['success'] = FALSE;
            $retarray['erronfiles'][] = array(
              'filename'  => $target . '/' . $fname,
              'source'    => $source . '/' . $fname,
              'error'     => 'could not copy file',
              'perms'     => $error_case_file_perms,
              'fileowner' => $fileownerinfo['name'],
            );
          }
        }
        else {
          // Create dir if not exist
          if (!file_exists($target_file_name)) {
            $successmkdir = mkdir($target_file_name, 0775, TRUE);
            if (!$successmkdir) {
              //on failure assume problems with parent dir permission:
              $parent_dir = dirname($target_file_name);
              $orig_perms = $this->getFilePermission($parent_dir);
              if (chmod($parent_dir, octdec('775'))) {
                $successmkdir = mkdir($target_file_name, 0775, TRUE);
                chmod($parent_dir, octdec($orig_perms));
              }
            }
            if (!$successmkdir) {
              $error_case_file_perms = $this->getFilePermission($target_file_name);
              $retarray['success'] = FALSE;
              $retarray['erronfiles'][] = array(
                'filename' => $target_file_name,
                'error'    => 'could not create dir',
                'perms'    => $error_case_file_perms,
              );
            }
          }
          elseif (!is_writable($target_file_name)) {
            $error_case_file_perms = $this->getFilePermission($target_file_name);
            $retarray['success'] = FALSE;
            $retarray['erronfiles'][] = array(
              'filename'  => $target_file_name,
              'error'     => 'could not create dir',
              'perms'     => $error_case_file_perms,
              'dir_owner' => $fileownerinfo['name'],
            );

          }

          $this->move_core_files($source_file_name, $target_file_name, $justcheck);
        }
      }
    }

    return $retarray;
  }

  /**
   * @param $url
   * @param $zipFile
   * @return bool
   */
  function curl_dl_core($url, $zipFile) {
    $zipResource = fopen($zipFile, "w");
    // Get The Zip File From Server
    $ch = curl_init();
    curl_setopt($ch, CURLOPT_URL, $url);
    curl_setopt($ch, CURLOPT_FAILONERROR, TRUE);
    curl_setopt($ch, CURLOPT_HEADER, 0);
    curl_setopt($ch, CURLOPT_FOLLOWLOCATION, TRUE);
    curl_setopt($ch, CURLOPT_AUTOREFERER, TRUE);
    curl_setopt($ch, CURLOPT_BINARYTRANSFER, TRUE);
    curl_setopt($ch, CURLOPT_TIMEOUT, 25);
    curl_setopt($ch, CURLOPT_SSL_VERIFYHOST, 0);
    curl_setopt($ch, CURLOPT_SSL_VERIFYPEER, 0);
    curl_setopt($ch, CURLOPT_FILE, $zipResource);
    $page = curl_exec($ch);
    curl_close($ch);
    if (!$page) {
      return FALSE;
    }
    else {
      return TRUE;
    }
  }


  /**
   * Method get_cms_core($extract_path,$url)
   * downloads from $url and extract the new cms core to the extract_path
   * @param $extract_path
   * @param $dl_files
   * @return mixed
   */
  function get_cms_core($extract_path, $dl_files) {
    $this->get_update_manager()->log = $this->get_update_manager()->read_log();
    if (!file_exists($extract_path)) {
      mkdir($extract_path, 0775, TRUE);
    }

    $url = $dl_files->url_targz;
    if (!class_exists('PharData')) {
      $url = $dl_files->url_zip;
    }

    $filename = basename($url);
    $zipFile = $extract_path . '/' . $filename; // Local Zip File Path
    $curl_success = $this->curl_dl_core($url, $zipFile);
    if (!$curl_success) {
      $this->get_update_manager()->log['CMSUPDATER_DOWNLOAD_NEW_CORE'] = array(
        'success' => FALSE,
        'report'  => 'curl_exec failure',
        'url'     => $url,
      );
      $this->get_update_manager()->write_log();
      $this->get_update_manager()->send_site_update_end(0, $this->get_update_manager()->log);
    }
    else {
      $md5hash_local = md5_file($zipFile);
      // Assume we have a file downloaded
      // Check if zip or tar.gz:
      if (!strpos($zipFile, '.zip')) {
        // Is it a tar.gz then:
        // md5check:
        $this->md5_check($md5hash_local, $dl_files->md5_targz);
        $pathinfo_archive = pathinfo($zipFile);
        // Open the .tar.gz file
        rename($zipFile, $pathinfo_archive['dirname'] . '/' . 'newd7.tar.gz');
        $zipFile = $pathinfo_archive['dirname'] . '/' . 'newd7.tar.gz';
        try {
          $p = new PharData($zipFile);
          $p->decompress();
          $tar = $pathinfo_archive['dirname'] . '/' . basename($zipFile, ".gz");
          // Unarchive from the tar
          $phar = new PharData($tar);
          $phar->extractTo($extract_path);
        } catch (Exception $ex) {
          // OK so something went wrong with Phar....
          // try zip again and do another download:
          $url = str_replace('.tar.gz', '.zip', $url);
          $filename = basename($url);
          $zipFile = $extract_path . '/' . $filename; // Local Zip File Path
          $curl_success = $this->curl_dl_core($url, $zipFile);
          if (!$curl_success) {
            $this->get_update_manager()->log['CMSUPDATER_DOWNLOAD_NEW_CORE'] = array(
              'success' => FALSE,
              'report'  => 'curl_exec failure',
              'url'     => $url,
            );
            $this->get_update_manager()->write_log();
            $this->get_update_manager()->send_site_update_end(0, $this->get_update_manager()->log);
          }
          else {
            $md5hash_local = md5_file($zipFile);
            $this->md5_check($md5hash_local, $dl_files->md5_zip);
            // Open the Zip file
            $res_extrct_zip = $this->extract_zip_file($extract_path, $zipFile);
            if ($res_extrct_zip['success'] == FALSE) {
              $this->get_update_manager()->log['CMSUPDATER_DOWNLOAD_NEW_CORE'] = array(
                'success'       => FALSE,
                'error_message' => 'could not extract zip',
                'error'         => $res_extrct_zip['error'],
              );
              $this->get_update_manager()->write_log();
              $this->get_update_manager()->send_site_update_end(0, $this->get_update_manager()->log);
              EXIT("END");
            }
          }
        }
      }
      else {
        $this->md5_check($md5hash_local, $dl_files->md5_zip);
        // Open the Zip file
        $res_extrct_zip = $this->extract_zip_file($extract_path, $zipFile);
        if ($res_extrct_zip['success'] == FALSE) {
          $this->get_update_manager()->log['CMSUPDATER_DOWNLOAD_NEW_CORE'] = array(
            'success'       => FALSE,
            'error_message' => 'could not extract zip',
            'error'         => $res_extrct_zip['error'],
          );
          $this->get_update_manager()->write_log();
          $this->get_update_manager()->send_site_update_end(0, $this->get_update_manager()->log);
          EXIT("END");
        }
      }
      $this->get_update_manager()->log['CMSUPDATER_DOWNLOAD_NEW_CORE'] = array('success' => TRUE);
      $this->get_update_manager()->write_log();
    }
    return $extract_path;
  }

  /**
   * Extract given zipfile to given extract_path, use pclzip or zipArchive classes
   * @param $extract_path
   * @param $zipFile
   * @return array
   */
  function extract_zip_file($extract_path, $zipFile) {
    // Open the Zip file
    if (!class_exists('ZipArchive')) {
      // Fallback:
      require_once('pclzip/pclzip.lib.php');
      $zip = new PclZip($zipFile);
      $extract_result = $zip->extract(PCLZIP_OPT_PATH, $extract_path);
      if ($extract_result == 0) {
        return array('success' => FALSE, 'error' => $zip->errorInfo(TRUE));
      }
      else {
        $this->get_update_manager()->log = $this->get_update_manager()->read_log();
        foreach ($extract_result as $ker => $extract_result_item) {
          if ($extract_result_item['status'] <> 'ok') {
            $res = array('success' => FALSE, 'error' => $extract_result_item);
            return $res;
          }
        }
        return $res = array('success' => TRUE);
      }
    }
    else {
      $this->get_update_manager()->log = $this->get_update_manager()->read_log();
      $this->get_update_manager()->write_log();
      $zip = new ZipArchive;
      if ($zip->open($zipFile) !== TRUE) {
        $res = array('success' => FALSE, 'error' => $zip->getStatusString());
        return $res;
      }
      // Extract Zip File
      $res_zip_extract = $zip->extractTo($extract_path);
      $zip->close();
      if ($res_zip_extract) {
        $res = array('success' => TRUE);
        return $res;
      }
      else {
        $res = array('success' => FALSE, 'error' => $zip->getStatusString());
        return $res;
      }
    }
  }

  /**
   * Helper function to check writability of the files path
   *
   * @param $files_path
   * @param $drupal_root_dir
   * @return string
   */
  function check_files_path_is_writable($files_path, $drupal_root_dir) {
    if ($files_path && file_exists($drupal_root_dir . '/' . $files_path) && is_writable($drupal_root_dir . '/' . $files_path)) {
      return $drupal_root_dir . '/' . $files_path;
    }
    elseif (is_writable($files_path)) {
      return $files_path;
    }
    return '';
  }

  /**
   * Function determine_files_path
   * @param $drupal_root_dir
   * @return string - '' when empty or not writable
   */
  public function determine_files_path($drupal_root_dir) {
    $files_path = variable_get('file_private_path', '');
    // Private files path is set
    if ($files_path) {
      $files_path = $this->check_files_path_is_writable($files_path, $drupal_root_dir);
    }
    // Probably no private path set, try public:
    if ($files_path == '') {
      $files_path = variable_get('file_public_path', '');
      $files_path = $this->check_files_path_is_writable($files_path, $drupal_root_dir);
    }
    // Files path still not writable? Set Drupal default sites/default/files:
    if ($files_path == '') {
      $files_path = $drupal_root_dir . '/sites/default/files';
      if (!is_writable($drupal_root_dir . '/sites/default/files')) {
        $files_path = '';
      }
    }

    return $files_path;
  }

  /**
   * Set some CMS defaults
   */
  public function set_local_data_from_cms() {
    $doc_root = $this->search_up_document_root(dirname(__FILE__));
    if ($doc_root === FALSE) {
      return;
    }
    define('DRUPAL_ROOT', $doc_root);
    $this->make_bootstrap();

    $key = variable_get('cms_updater_key', '');
    $path = drupal_get_path('module', 'cms_updater');
    $domain = $GLOBALS['base_root'];
    $temp_path = file_directory_temp();
    if (!file_exists($temp_path)) {
      $temp_path = $doc_root . '/' . file_directory_temp();
    }
    $files_path = $this->determine_files_path(DRUPAL_ROOT);
    $this->get_update_manager()->set_local_key($key);
    $this->get_update_manager()->set_local_path($path);
    $this->get_update_manager()->set_docroot(DRUPAL_ROOT);
    $this->get_update_manager()->set_domain($domain);
    $this->get_update_manager()->set_current_version(VERSION);
    $this->get_update_manager()->set_cms(1);
    $this->set_temppath($temp_path);
    $this->set_files_path($files_path);

    // Check if the cms_updater module is enabled, if not exit process and tell the service about it.
    $cms_updater_enabled = db_query("SELECT name, status FROM {system} WHERE type = 'module' AND name = 'cms_updater'")
      ->fetchAllAssoc('name');
    if (isset($cms_updater_enabled['cms_updater']->status) && intval($cms_updater_enabled['cms_updater']->status) == 0) {
      $this->get_update_manager()->send_site_active(0);
      exit("END");
    }

    // Check key
    if ($this->get_update_manager()->get_post_data()->key <> $key) {
      // No valid key, exit here:
      exit("END");
    }


  }

  /**
   * Backup the database with the default settings.
   * @return array
   */
  public function backup_db() {
    $this->make_bootstrap();

    global $databases;
    $ret = FALSE;
    $dbhost = '';
    $dbname = '';
    foreach ($databases as $keydb => $dbdefaultouter) {
      foreach ($dbdefaultouter as $kinner => $database) {
        if ($database['driver'] == 'mysql') {

          $dbhost = $database['host'];
          $dbuser = $database['username'];
          $dbpwd = $database['password'];
          $dbname = $database['database'];

          $dbbackup = $this->get_files_path() . '/cms_updater/' . $this->get_update_manager()
              ->get_updatenode_nid() . '/' . $this->get_update_manager()->get_updatenode_nid() . '-backupdb.sql';

          error_reporting(0);
          set_time_limit(0);

          $conn = mysql_connect($dbhost, $dbuser, $dbpwd) or mysql_error();
          mysql_select_db($dbname);

          $f = fopen($dbbackup, "w");
          $tables = mysql_list_tables($dbname);
          while ($cells = mysql_fetch_array($tables)) {
            $table = $cells[0];
            fwrite($f, "DROP TABLE IF EXISTS `" . $table . "`;\n");
            $res = mysql_query("SHOW CREATE TABLE `" . $table . "`");
            if ($res) {
              $create = mysql_fetch_array($res);
              $create[1] .= ";";
              $line = str_replace("\n", "", $create[1]);
              fwrite($f, $line . "\n");
              $data = mysql_query("SELECT * FROM `" . $table . "`");
              $num = mysql_num_fields($data);
              while ($row = mysql_fetch_array($data)) {
                $line = "INSERT INTO `" . $table . "` VALUES(";
                for ($i = 1; $i <= $num; $i++) {
                  $line .= "'" . mysql_real_escape_string($row[$i - 1]) . "', ";
                }
                $line = substr($line, 0, -2);
                fwrite($f, $line . ");\n");
              }
            }
          }
          $ret = fclose($f);
          if (!$ret) {
            break;
          }

        }
      }
      if (!$ret) {
        break;
      }
    }
    if (!$ret) {
      return array('success' => FALSE, 'dbhost' => $dbhost, 'dbname' => $dbname);
    }
    else {
      return array('success' => TRUE);
    }

  }

  /**
   * update_db for CMS Drupal 7, taken and adapted from update.php and update.inc
   * @return array|mixed
   */
  public function update_db() {
    $this->make_bootstrap();

    require_once DRUPAL_ROOT . '/includes/install.inc';
    require_once DRUPAL_ROOT . '/includes/update.inc';
    drupal_load_updates();

    // Preparations done, now get list of updates:
    $updates = $this->update_get_update_list();
    $this->get_update_manager()->log = $this->get_update_manager()->read_log();
    $this->get_update_manager()->log['CMSUPDATER_UPDATE_DB_WORK']['pending_updates'] = $updates;
    $this->get_update_manager()->write_log();
    $updatequeue = array();
    foreach ($updates as $kupdate => $update) {
      $updatequeue[$kupdate] = $update['start'];
    }
    $updaterresults = $this->cmsupdater_update_modules($updatequeue);
    $return = array(
      'success' => TRUE,
      'report'  => $updaterresults,
    );
    // Set site in maintenance
    variable_set("maintenance_mode", 0);
    if ((isset($updaterresults['results']['#abort'])) && count($updaterresults['results']['#abort']) > 0) {
      $return = array(
        'success' => FALSE,
        'report'  => $updaterresults,
      );
    }
    return $return;
  }

  /**
   * Set last update in a Drupal variable (needs Drupal bootstrap)
   */
  function set_last_update() {
    if ($this->get_update_success()) {
      variable_set('cms_updater_last_update', time());
    }
  }

  /**
   * @return array
   */
  function update_get_update_list() {
    // Make sure that the system module is first in the list of updates.
    $ret = array('system' => array());
    $modules = drupal_get_installed_schema_version(NULL, FALSE, TRUE);
    foreach ($modules as $module => $schema_version) {
      // Skip uninstalled and incompatible modules.
      if ($schema_version == SCHEMA_UNINSTALLED) {
        continue;
      }
      // Otherwise, get the list of updates defined by this module.
      $updates = drupal_get_schema_versions($module);
      if ($updates !== FALSE) {
        // module_invoke returns NULL for nonexisting hooks, so if no updates
        // are removed, it will == 0.
        $last_removed = module_invoke($module, 'update_last_removed');
        if ($schema_version < $last_removed) {
          $ret[$module]['warning'] = '<em>' . $module . '</em> module can not be updated. Its schema version is ' . $schema_version . '. Updates up to and including ' . $last_removed . ' have been removed in this release. In order to update <em>' . $module . '</em> module, you will first <a href="http://drupal.org/upgrade">need to upgrade</a> to the last version in which these updates were available.';
          continue;
        }

        $updates = drupal_map_assoc($updates);

        foreach (array_keys($updates) as $update) {
          if ($update > $schema_version) {
            // The description for an update comes from its Doxygen.
            $func = new ReflectionFunction($module . '_update_' . $update);
            $description = str_replace(array("\n", '*', '/'), '', $func->getDocComment());
            $ret[$module]['pending'][$update] = "$update - $description";
            if (!isset($ret[$module]['start'])) {
              $ret[$module]['start'] = $update;
            }
          }
        }
        if (!isset($ret[$module]['start']) && isset($ret[$module]['pending'])) {
          $ret[$module]['start'] = $schema_version;
        }
      }
    }

    if (empty($ret['system'])) {
      unset($ret['system']);
    }
    return $ret;
  }

  /**
   * @param $start
   * @return array
   */
  function cmsupdater_update_modules($start) {
    // During the update, bring the site offline so that schema changes do not
    // affect visiting users.
    $_SESSION['maintenance_mode'] = variable_get('maintenance_mode', FALSE);
    if ($_SESSION['maintenance_mode'] == FALSE) {
      variable_set('maintenance_mode', TRUE);
    }

    // Resolve any update dependencies to determine the actual updates that will
    // be run and the order they will be run in.

    $updates = update_resolve_dependencies($start);

    // Store the dependencies for each update function in an array which the
    // batch API can pass in to the batch operation each time it is called. (We
    // do not store the entire update dependency array here because it is
    // potentially very large.)
    $dependency_map = array();
    foreach ($updates as $function => $update) {
      $dependency_map[$function] = !empty($update['reverse_paths']) ? array_keys($update['reverse_paths']) : array();
    }

    // Load some modules, because updates may use there functions.
    // For example image_update_7002 needs field_sql_storage module -> _field_sql_storage_tablename()
    module_load_include('module', 'field_sql_storage');

    // For updates we need all includes like a full bootstrap.
    require_once DRUPAL_ROOT . '/' . variable_get('path_inc', 'includes/path.inc');
    require_once DRUPAL_ROOT . '/includes/theme.inc';
    require_once DRUPAL_ROOT . '/includes/pager.inc';
    require_once DRUPAL_ROOT . '/' . variable_get('menu_inc', 'includes/menu.inc');
    require_once DRUPAL_ROOT . '/includes/tablesort.inc';
    require_once DRUPAL_ROOT . '/includes/file.inc';
    require_once DRUPAL_ROOT . '/includes/unicode.inc';
    require_once DRUPAL_ROOT . '/includes/image.inc';
    require_once DRUPAL_ROOT . '/includes/form.inc';
    require_once DRUPAL_ROOT . '/includes/mail.inc';
    require_once DRUPAL_ROOT . '/includes/actions.inc';
    require_once DRUPAL_ROOT . '/includes/ajax.inc';
    require_once DRUPAL_ROOT . '/includes/token.inc';
    require_once DRUPAL_ROOT . '/includes/errors.inc';

    $updateresults = array();
    foreach ($updates as $update) {
      if ($update['allowed']) {
        // Set the installed version of each module so updates will start at the
        // correct place. (The updates are already sorted, so we can simply base
        // this on the first one we come across in the above foreach loop.)
        if (isset($start[$update['module']])) {
          drupal_set_installed_schema_version($update['module'], $update['number'] - 1);
          unset($start[$update['module']]);
        }

        $context = array('sandbox' => array());
        $updateresults[] = $this->cmsupdater_update_do_one($update['module'], $update['number'], $dependency_map, $context);
      }
    }
    return $updateresults;
  }

  /**
   * @param $module
   * @param $number
   * @param $dependency_map
   * @param $context
   * @return bool
   */
  function cmsupdater_update_do_one($module, $number, $dependency_map, &$context) {
    $function = $module . '_update_' . $number;
    // If this update was aborted in a previous step, or has a dependency that
    // was aborted in a previous step, go no further.
    if (!empty($context['results']['#abort']) && array_intersect($context['results']['#abort'], array_merge($dependency_map, array($function)))) {

      return FALSE;
    }
    $ret = array();
    if (function_exists($function)) {
      try {
        $ret['results']['query'] = $function($context['sandbox']);
        $ret['results']['success'] = TRUE;
      }
        // @TODO We may want to do different error handling for different
        // exception types, but for now we'll just log the exception and
        // return the message for printing.
      catch (Exception $e) {
        watchdog_exception('update', $e);

        require_once DRUPAL_ROOT . '/includes/errors.inc';
        $variables = _drupal_decode_exception($e);
        // The exception message is run through check_plain() by _drupal_decode_exception().
        $ret['#abort'] = array(
          'success' => FALSE,
          'query'   => t('%type: !message in %function (line %line of %file).', $variables),
        );
      }
    }

    if (!isset($context['results'][$module])) {
      $context['results'][$module] = array();
    }
    if (!isset($context['results'][$module][$number])) {
      $context['results'][$module][$number] = array();
    }
    $context['results'][$module][$number] = array_merge($context['results'][$module][$number], $ret);

    if (!empty($ret['#abort'])) {
      // Record this function in the list of updates that were aborted.
      $context['results']['#abort'][] = $function;
    }
    else {
      drupal_set_installed_schema_version($module, $number);
    }
    return $context;
  }

  /**
   * Checks if the given directory is the drupal root directory
   *
   * @param $dirname
   * @return bool
   */
  public function check_dir_drupal_root($dirname) {
    if (!is_dir($dirname)) {
      return FALSE;
    }
    $filesndirs = scandir($dirname);
    if ($filesndirs === FALSE) {
      return FALSE;
    }
    $array_compare = array();
    $array_compare[] = 'authorize.php';
    $array_compare[] = 'cron.php';
    $array_compare[] = 'install.php';
    $array_compare[] = 'index.php';
    $array_compare[] = 'misc';
    $array_compare[] = 'modules';
    $array_compare[] = 'sites';
    $array_compare[] = 'themes';
    if (!array_diff($array_compare, $filesndirs)) {
      return TRUE;
    }
    else {
      return FALSE;
    }
  }

  /**
   * Clean up old stored data
   */
  public function cleanup_old_update_data() {
    /* usort helper
     * @param $a
     * @param $b
     * @return bool
     */
    function cmp_modified($a, $b) {
      return $b['modified'] < $a['modified'];
    }

    $cms_updater_updates_path = $this->get_files_path() . '/cms_updater';
    $past_updates = scandir($cms_updater_updates_path);
    $dirs_to_delete = array();
    if ($past_updates !== FALSE) {
      foreach ($past_updates as $kpu => $past_update) {
        if (intval($past_update) > 0 && (($past_update <> '.') || ($past_update <> '..'))) {
          if ($modified = filemtime($cms_updater_updates_path . '/' . $past_update)) {
            $dirs_to_delete[] = array(
              'file_name' => $cms_updater_updates_path . '/' . $past_update,
              'modified'  => $modified
            );
          }
        }
      }
      usort($dirs_to_delete, "cmp_modified");

      // Hold only the last 3 backups and delete the rest
      if (count($dirs_to_delete) > 3) {
        array_pop($dirs_to_delete);
        array_pop($dirs_to_delete);
        array_pop($dirs_to_delete);
        //delete:
        foreach ($dirs_to_delete as $kdtd => $dir_to_delete) {
          if (is_dir($dir_to_delete['file_name'])) {
            $this->rrmdir($dir_to_delete['file_name']);
          }
        }
      }

      // Delete downloaded Drupal core and related stuff from temp
      $this->rrmdir($this->get_temppath());

    }
  }

  /**
   * Check the download URL against manipulation
   *
   * @param $download_url
   * @return bool
   */
  public function check_download_url($download_url) {
    $url_parts = explode('/', $download_url);
    $this->get_update_manager()->write_log();
    if (($url_parts[2] == "ftp.drupal.org") && ($url_parts[3] == "files") && ($url_parts[4] == "projects")) {
      return TRUE;
    }
    return FALSE;
  }

  /**
   * Create a .htaccess file to our backup folder
   *
   * @return bool
   */
  public function add_htaccess_to_log_parent() {
    $htaccess_filename = $this->get_files_path() . '/cms_updater/.htaccess';
    $data = "Deny from all \n";
    $data .= "# Turn off all options we don't need.\n";
    $data .= "Options None\n";
    $data .= "Options +FollowSymLinks\n";
    $data .= "# Set the catch-all handler to prevent scripts from being executed.\n";
    $data .= "SetHandler Drupal_Security_Do_Not_Remove_See_SA_2006_006\n";
    $data .= "<Files *>\n";
    $data .= "# Override the handler again if we're run later in the evaluation list.\n";
    $data .= "SetHandler Drupal_Security_Do_Not_Remove_See_SA_2013_003\n";
    $data .= "</Files>\n";
    $data .= "# If we know how to do it safely, disable the PHP engine entirely.\n";
    $data .= "<IfModule mod_php5.c>\n";
    $data .= "php_flag engine off\n";
    $data .= "</IfModule>";
    if (file_put_contents($htaccess_filename, $data) !== FALSE) {
      return TRUE;
    }
    else {
      return FALSE;
    }
  }

  /**
   * With the given path, check if it is a drupal doc_root,
   * if not, try next higher directory, but only max 100 times.
   *
   * @param type $dir_path
   * @return mixed (full path to document root) or FALSE on failure
   */
  function search_up_document_root($dir_path) {
    $dir_to_check = $dir_path;
    $document_root = $dir_path;
    if (file_exists($dir_path) && (is_dir($dir_path))) {
      if (!$this->check_dir_drupal_root($dir_path)) {
        for ($i = 1; $i <= 100; $i++) {
          $dir_to_check = dirname($dir_to_check);
          if ($this->check_dir_drupal_root($dir_to_check)) {
            $document_root = $dir_to_check;
            break;
          }
        }
      }
    }
    else {
      return FALSE;
    }
    if ($this->check_dir_drupal_root($document_root)) {
      return $document_root;
    }
    else {
      return FALSE;
    }
  }

  /**
   * Init a Drupal bootstrap on session level
   */
  function make_bootstrap() {
    if (!defined('DRUPAL_ROOT')) {
      $doc_root = $this->search_up_document_root(dirname(__FILE__));
      define('DRUPAL_ROOT', $doc_root);
    }
    require_once DRUPAL_ROOT . '/includes/bootstrap.inc';
    require_once DRUPAL_ROOT . '/includes/common.inc';
    require_once DRUPAL_ROOT . '/includes/module.inc';
    require_once DRUPAL_ROOT . '/includes/unicode.inc';
    require_once DRUPAL_ROOT . '/includes/file.inc';

    drupal_bootstrap(DRUPAL_BOOTSTRAP_SESSION);

    include_once DRUPAL_ROOT . '/includes/module.inc';
    $module_list['system']['filename'] = 'modules/system/system.module';
    module_list(TRUE, FALSE, FALSE, $module_list);
    drupal_load('module', 'system');
    drupal_load('module', 'cms_updater');
  }
}
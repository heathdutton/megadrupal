<?php
/**
 * @file
 * Contains CMSUpdateManager
 */

define('CMSUPDATER_PING_RECEIVED', 10); //server has sent ping
define('CMSUPDATER_UPDATE_START_SEND', 20);
define('CMSUPDATER_BACKUP', 21);
define('CMSUPDATER_DOWNLOAD_NEW_CORE', 25);
define('CMSUPDATER_MOVE_FILES', 30);
define('CMSUPDATER_UPDATE_DB', 35);
define('CMSUPDATER_CLEAN_UP', 40);

require_once dirname(dirname(__FILE__)) . '/conf/constants.php';

/**
 * Class CMSUpdateManager
 *
 * This class purpose is to manage the update process which is basically:
 * 1. Validate Key
 * 2. Backup sources
 * 3. Get new version zip and extract it temporarily
 * 4. Move files from new version archive to current files origin
 * 5. Call upgrade DB script
 *
 * This class also updates the internal state of the Update-Process
 * and reports back to the Server/Service using http requests
 *
 * http requests are eventually also used for splitting up the different update process tasks
 * to circumvent the timeout.
 *
 */
class CMSUpdateManager {

  private $cms; // Holds the CMS as integer.
  private $current_version;
  private $desired_version;
  private $state; // Holds the current state.
  private $serviceurl = CMS_UPDATER_SERVICE_URL;
  private $local_key;
  private $local_path;
  private $domain;
  private $updatenode_nid;
  private $cmsupdater = NULL; // Hold the cms-specific CMSUpdater.
  private $post_data; // i.e. the data sent by updateservice to this client.
  private $own_curl_url; // URL to core_update.php.
  public $log = array(); // Structured log_array.
  private $logfilename; // Logs in json file.
  private $docroot; // Full path to the document root of the cms.

  /**
   * @param $cmsname
   */
  function __construct($cmsname) {
    $this->post_data = $this->get_post_request_from_server();
    if (isset($this->post_data->updatenodeid)) {
      $this->set_updatenode_nid($this->post_data->updatenodeid);
    }
    $file_updaterclass = dirname(__FILE__) . '/' . $cmsname . 'CMSUpdater.php';
    if (!file_exists($file_updaterclass)) {
      throw new Exception('Invalid cmsname passed to constructor__' . $file_updaterclass);
    }
    require_once $file_updaterclass;
    $classname = $cmsname . 'CMSUpdater';
    $this->cmsupdater = New $classname();
  }

  /**
   * @return null
   */
  public function get_cmsupdater() {
    return $this->cmsupdater;
  }

  /**
   * @return mixed - Object with data sent by cmsupdater-server-service
   */
  public function get_post_request_from_server() {
    $postdata_json = file_get_contents('php://input');
    $obj = json_decode($postdata_json);
    return $obj;
  }

  /**
   * @param $url
   * @param $postdata
   * @return array|mixed
   */
  public function send_post_request_to_server($url, $postdata) {
    $data = $postdata;

    $datajson = json_encode($data);

    $ch = curl_init($url);
    curl_setopt($ch, CURLOPT_CUSTOMREQUEST, "POST");
    curl_setopt($ch, CURLOPT_POSTFIELDS, $datajson);
    curl_setopt($ch, CURLOPT_RETURNTRANSFER, TRUE);
    curl_setopt($ch, CURLOPT_HTTPHEADER, array(
        'Content-Type: application/json',
        'Content-Length: ' . strlen($datajson),
      )
    );
    $result = curl_exec($ch);
    if ($result === FALSE) {
      $result = array('error_nr' => 1, 'error' => 2, 'curlinfo' => 3);
    }
    curl_close($ch);
    return $result;
  }

  /**
   * Prepare and send the CHECK_KEY POST
   *
   * @return mixed -POST RESULT
   */
  public function send_check_key() {
    $key = $this->get_local_key();
    $domain = $this->get_domain();
    $path = $this->get_local_path();
    $post_data = array(
      'path'   => $path,
      'domain' => $domain,
      'key'    => $key,
    );
    $url = $this->get_serviceurl() . '/api/service/check_key.json';
    $result = $this->send_post_request_to_server($url, $post_data);
    $retval = json_decode($result);
    return $retval;
  }

  /**
   * Prepare and send the SITE_UPDATE_START POST:
   *
   * @return mixed - integer number
   */
  public function send_site_update_start() {
    $key = $this->get_local_key();
    $domain = $this->get_domain();
    $path = $this->get_local_path();
    $start = time();
    $version_old = $this->get_current_version();
    $version_new = $this->get_desired_version();
    $cms = $this->get_cms();
    $post_data = array(
      'path'        => $path,
      'domain'      => $domain,
      'key'         => $key,
      'start'       => $start,
      'version_old' => $version_old,
      'version_new' => $version_new,
      'cms'         => $cms,
    );
    $url = $this->get_serviceurl() . '/api/service/site_update_start.json';

    $result = $this->send_post_request_to_server($url, $post_data);

    // Result is json, so decode:
    $res = json_decode($result);
    // START_UPDATE_POST should return an integer number:
    $resint = $res[0];

    return $resint;
  }

  /**
   * @param $active_inactive - active = 1; inactive = 0
   * @return mixed
   */
  public function send_site_active($active_inactive) {
    $key = $this->get_local_key();
    $domain = $this->get_domain();
    $path = $this->get_local_path();
    $post_data = array(
      'path'   => $path,
      'domain' => $domain,
      'key'    => $key,
      'active' => $active_inactive,
    );
    $url = $this->get_serviceurl() . '/api/service/site_active.json';
    $result = $this->send_post_request_to_server($url, $post_data);
    //result is json, so decode:
    $res = json_decode($result);

    //START_UPDATE_POST should return an integer number:
    $resint = $res[0];
    return $resint;
  }

  /**
   * Prepare and send the SITE_UPDATE_END POST:
   *
   * @param $successfull
   * @param array $log
   * @return mixed - integer number
   */
  public function send_site_update_end($successfull, $log = array()) {
    $key = $this->get_local_key();
    $domain = $this->get_domain();
    $path = $this->get_local_path();
    $end = time();

    $post_data = array(
      'path'        => $path,
      'domain'      => $domain,
      'key'         => $key,
      'end'         => $end,
      'nid'         => $this->get_updatenode_nid(),
      'success'     => $successfull,
      'report_data' => $log,
    );
    $url = $this->get_serviceurl() . '/api/service/site_update_end.json';

    $result = $this->send_post_request_to_server($url, $post_data);

    // Result is json, so decode:
    $res = json_decode($result);

    // START_UPDATE_POST should return an integer number:
    return $res[0];
  }

  /**
   * Prepare and send the SITE_UPDATE_END POST:
   *
   * @return mixed - integer number
   */
  public function send_site_set_version() {
    $version = $this->get_current_version();
    $path = $this->get_local_path();
    $domain = $this->get_domain();
    $key = $this->get_local_key();

    $post_data = array(
      'cms'     => $path,
      'path'    => $path,
      'version' => $version,
      'domain'  => $domain,
      'key'     => $key,
    );
    $url = $this->get_serviceurl() . '/api/service/site_set_version.json';

    $result = $this->send_post_request_to_server($url, $post_data);
    $resarray = json_decode($result);

    return $resarray[0];
  }

  /**
   * Prepare and send curl request to call core_update.php with the next step/task:
   *
   * @param $step -  i.e.: CMSUPDATER_BACKUPED (see defines at the beginning of this file)
   * @return array|mixed - result of request
   */
  public function send_call_myself($step) {

    $current_post_data_json = json_encode($this->get_post_data());
    $current_post_data_array = json_decode($current_post_data_json, TRUE);
    if (is_array($current_post_data_array)) {
      $current_post_data_array['step'] = $step;
      $current_post_data_array['updatenodeid'] = $this->get_updatenode_nid();
    }
    $url = $this->get_own_curl_url();
    $data_json = json_encode($current_post_data_array);

    $ch = curl_init($url);
    curl_setopt($ch, CURLOPT_CUSTOMREQUEST, "POST");
    curl_setopt($ch, CURLOPT_POSTFIELDS, $data_json);
    curl_setopt($ch, CURLOPT_RETURNTRANSFER, TRUE);
    curl_setopt($ch, CURLOPT_HTTPHEADER, array(
        'Content-Type: application/json',
        'Content-Length: ' . strlen($data_json),
      )
    );
    $result = curl_exec($ch);
    curl_close($ch);

    if ($result === FALSE) {
      $error_nr = curl_errno($ch);
      $error = curl_error($ch);
      $curlinfo = curl_getinfo($ch);
      $result = array('error_nr' => $error_nr, 'error' => $error, 'curlinfo' => $curlinfo);
      $this->log = $this->read_log();
      $this->log['CURL_REQUEST']['STEP'][$step] = array('success' => FALSE, 'RESULTDATA' => $result);
      $this->write_log();

    }
    else {
      $this->log = $this->read_log();
      $this->log['CURL_REQUEST']['STEP'][$step] = array('success' => TRUE, 'RESULTDATA' => $result);
      $this->log['CURL_POSTDATA'] = $this->get_post_data();
      $this->write_log();
    }

    if (is_string($result) && ($result <> "")) {
      switch ($result) {
        CASE "CMSUPDATER_DOWNLOAD_NEW_CORE":
          $this->send_call_myself(constant($result));
          break;
        CASE "CMSUPDATER_MOVE_FILES":
          $this->send_call_myself(constant($result));
          break;
        CASE "CMSUPDATER_UPDATE_DB":
          $this->send_call_myself(constant($result));
          break;
        CASE "CMSUPDATER_CLEAN_UP":
          $this->send_call_myself(constant($result));
          break;
        CASE "END":
          return $result;
        default:
          $this->log = $this->read_log();
          $this->log['CURL_REQUEST']['STEP'][$step] = array('success' => FALSE, 'RESULTDATA' => 'unknown STEP');
          $this->write_log();
          return;
      }

    }

    return $result;
  }

  /**
   * @return mixed
   */
  public function get_own_curl_url() {
    return $this->own_curl_url;
  }

  /**
   * @param $own_curl_url
   */
  public function set_own_curl_url($own_curl_url) {
    $this->own_curl_url = $own_curl_url;
  }

  /**
   * @return mixed
   */
  public function get_post_data() {
    return $this->post_data;
  }

  /**
   * @param $post_data
   */
  public function set_post_data($post_data) {
    $this->post_data = $post_data;
  }

  /**
   * @return bool
   */
  public function get_updatenode_nid() {
    if ((isset($this->updatenode_nid)) && (intval($this->updatenode_nid) > 0)) {
      return $this->updatenode_nid;
    }
    else {
      return FALSE;
    }
  }

  /**
   * @param $updatenode_nid
   */
  public function set_updatenode_nid($updatenode_nid) {
    $this->updatenode_nid = $updatenode_nid;
  }

  /**
   * @return mixed
   */
  public function get_website_nid() {
    return $this->websitenid;
  }

  /**
   * @param $websitenid
   */
  public function set_website_nid($websitenid) {
    $this->websitenid = $websitenid;
  }

  /**
   * @return string
   */
  public function get_serviceurl() {
    return $this->serviceurl;
  }

  /**
   * @return mixed
   */
  public function get_domain() {
    return $this->domain;
  }

  /**
   * @param $serviceurl
   */
  public function set_serviceurl($serviceurl) {
    $this->serviceurl = $serviceurl;
  }

  /**
   * @param $domain
   */
  public function set_domain($domain) {
    $this->domain = $domain;
  }

  /**
   * @param $local_key
   */
  public function set_local_key($local_key) {
    $this->local_key = $local_key;
  }

  /**
   * @param $local_path
   */
  public function set_local_path($local_path) {
    $this->local_path = $local_path;
  }

  /**
   * @return mixed
   */
  public function get_local_key() {
    return $this->local_key;
  }

  /**
   * @return mixed
   */
  public function get_local_path() {
    return $this->local_path;
  }

  /**
   * @return mixed
   */
  public function get_state() {
    return $this->state;
  }

  /**
   * @param $state
   */
  public function set_state($state) {
    $this->state = $state;
  }

  /**
   * @return mixed
   */
  public function get_cms() {
    return $this->cms;
  }

  /**
   * @param $cms
   */
  public function set_cms($cms) {
    $this->cms = $cms;
  }

  /**
   * @return mixed
   */
  public function get_current_version() {
    return $this->current_version;
  }

  /**
   * @param $current_version
   */
  public function set_current_version($current_version) {
    $this->current_version = $current_version;
  }

  /**
   * @return mixed
   */
  public function get_desired_version() {
    return $this->desired_version;
  }

  /**
   * @param $desired_version
   */
  public function set_desired_version($desired_version) {
    $this->desired_version = $desired_version;
  }

  /**
   * @return mixed
   */
  public function get_logfilename() {
    return $this->logfilename;
  }

  /**
   * @return mixed
   */
  public function get_docroot() {
    return $this->docroot;
  }

  /**
   * @param $docroot
   */
  public function set_docroot($docroot) {
    $this->docroot = $docroot;
  }

  /**
   * @param $logfilename
   */
  public function set_logfilename($logfilename) {
    $this->logfilename = $logfilename;
  }

  /**
   *
   */
  public function write_log() {
    $data = json_encode($this->log);
    $filename = $this->get_logfilename();
    file_put_contents($filename, $data);
  }

  /**
   * @return mixed|string
   */
  public function read_log() {
    $filename = $this->get_logfilename();
    $data = '';
    if (file_exists($filename)) {
      $data = json_decode(file_get_contents($filename), TRUE);
    }
    return $data;
  }

}

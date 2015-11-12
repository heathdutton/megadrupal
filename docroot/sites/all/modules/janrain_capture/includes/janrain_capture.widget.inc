<?php
/**
 * @file
 * Widget-related functions
 */

/**
 * Check if the screens are hosted remotely
 */
function _janrain_capture_widget_is_remote_screens() {
  $screenPath = variable_get('janrain_capture_screens_folder', NULL);
  return (strpos($screenPath, 'http') === 0);
}

/**
 * Update the local cache of remote JTL and Event files aka "Screens".
 */
function janrain_capture_widget_update_remote_screens() {
  // wtb oop or enums
  static $allowedScreens = array('signin', 'edit-profile', 'public-profile', 'forgot', 'verify');
  static $allowedTypes = array('js', 'html');

  $cacheDir = JANRAIN_CAPTURE_WIDGET_SCREEN_CACHE_DIR;

  // make sure cache directory is ready for files
  if (!file_prepare_directory($cacheDir, FILE_CREATE_DIRECTORY | FILE_MODIFY_PERMISSIONS)) {
    // cache dir failure, notify and bail
    watchdog('janrain_capture',
      'Failed to create screen cache directory: %directory',
      array('%directory' => $cacheDir),
      WATCHDOG_ERROR);
    return;
  }
  // hook cron shouldn't run this but lets sanity check anyway
  if (!_janrain_capture_widget_is_remote_screens()) {
    return;
  }
  $screenSourceDir = variable_get('janrain_capture_screens_folder');
  $sourcePathTemplate = '%s%s.%s';
  $destPathTemplate = '%s/%s.%s';
  // grab each file
  foreach ($allowedScreens as &$name) {
    foreach ($allowedTypes as &$ext) {
      $screenSource = sprintf($sourcePathTemplate, $screenSourceDir, $name, $ext);
      $screenDest = sprintf($destPathTemplate, $cacheDir, $name, $ext);
      $response = drupal_http_request($screenSource);
      if ($response->code != 200) {
        // notify there was an issue, but try to get the other files
        watchdog('janrain_capture', $response->error, array(), WATCHDOG_ERROR);
        continue;
      }
      // use unmanaged since remote source is always authoritative
      $success = file_unmanaged_save_data($response->data, $screenDest, FILE_EXISTS_REPLACE);
      if (FALSE === $success) {
        // failed to save a file that was succesfully downloaded
        watchdog('janrain_capture', "Failed to write %screenDest", array('screenDest' => $screenDest), WATCHDOG_ERROR);
      }
    }
  }
}

/**
 * Get the contents of a JTL or events file
 * @param string screenName
 *   The type of screen (signin, forgot, etc...)
 * @param string fileType
 *   'html' for JTL or 'js' for events
 * @return string
 *   return empty string on error conditions, otherwise returns the JTL or javascript content of the file.
 */
function _janrain_capture_widget_get_screen($screenName, $fileType) {
  global $base_url;

  // wtb oop
  static $allowedScreens = array('signin', 'edit-profile', 'public-profile', 'forgot', 'verify');
  static $allowedTypes = array('js', 'html');

  // sanity check that it's a valid screen request
  if (!(in_array($screenName, $allowedScreens) && in_array($fileType, $allowedTypes))) {
    // no such screen exists, return empty string.
    return '';
  }

  // check for screens module
  if (module_exists('janrain_capture_screens')) {
    $fileName = sprintf('%s.%s', $screenName, $fileType);

    // proceed with screens module
    if (!in_array($fileName, _janrain_capture_get_screens())) {
      return '';
    }

    if ($screen_file = _janrain_capture_get_screen_file($fileName)) {
      return file_get_contents($screen_file);
    }

    return '';
  }

  // screen name and type are valid get the content
  if (_janrain_capture_widget_is_remote_screens()) {
    // files are remote so pull from local cache (updated by _janrain_capture_widget_update_screens();
    $fileName = sprintf('%s/%s.%s', JANRAIN_CAPTURE_WIDGET_SCREEN_CACHE_DIR, $screenName, $fileType);
  }
  else {
    $screenPath = variable_get('janrain_capture_screens_folder', 'file:///sites/all/themes/janrain-capture-screens/');

    if (strpos($screenPath, 'file:///', 0) === 0) {
      // files are local, usually in sites/all/themes
      $fileName = sprintf('%s%s%s.%s', DRUPAL_ROOT, str_replace('file://', '', $screenPath), $screenName, $fileType);
    }
    else {
      // invalid screens folder setting
      $fileName = sprintf('%s/janrain-capture-screens/%s.%s', drupal_get_path('module', 'janrain_capture'), $screenName, $fileType);
    }
  }
  if (!is_readable($fileName)) {
    // let everyone know there was a problem with the file, but don't kill the site.
    drupal_set_message("Unable to read $fileName", 'error');
    return '';
  }
  // file exists and is readable, return the contents.
  return file_get_contents($fileName);
}

/**
 * Adds widget JS settings to the page.
 */
function janrain_capture_widget_add_settings($settings = array()) {
  // Widget settings
  $janrain_capture_main = variable_get('janrain_capture_main2', array());
  $janrain_capture_main = array_merge($janrain_capture_main, variable_get('janrain_capture_ui2', array()));
  $janrain_capture_optional = variable_get('janrain_capture_federate2', array());
  $janrain_capture_optional = array_merge($janrain_capture_optional, variable_get('janrain_capture_backplane2', array()));

  if (!empty($janrain_capture_optional['capture_sso_address'])) {
    $settings['janrainCapture']['sso_address'] = $janrain_capture_optional['capture_sso_address'];
  }

  if (isset($janrain_capture_optional['backplane_enabled'])
      && !empty($janrain_capture_optional['backplane_bus_name'])) {
    $settings['janrainCapture']['backplane_enabled'] = $janrain_capture_optional['backplane_enabled'];
    $settings['janrainCapture']['backplane_bus_name'] = $janrain_capture_optional['backplane_bus_name'];
  }

  // Add settings array into a JS variable
  drupal_add_js($settings, array('type' => 'setting', 'every_page' => TRUE, 'preprocess' => FALSE, 'weight' => 0, 'scope' => 'header',));
}

/**
 * Adds widget JS scripts to the page.
 */
function janrain_capture_widget_add_scripts() {
  global $base_url;

  // File scripts
  drupal_add_js(drupal_get_path('module', 'janrain_capture') . '/janrain_capture.js', array(
    'type' => 'file',
    'every_page' => TRUE,
    'weight' => 1,
    'preprocess' => FALSE,
    'scope' => 'header',));

  $widget = array(
    '#type' => 'markup',
    '#prefix' => '<script type="text/javascript">',
    '#suffix' => '</script>',
    '#markup' => janrain_capture_widget_js(),
    '#weight' => 3,
  );
  $capture_client = array(
    '#type' => 'markup',
    '#prefix' => '<script type="text/javascript" src="https://d7v0k4dt27zlp.cloudfront.net/assets/capture_client.js">',
    '#suffix' => '</script>',
    '#markup' => '',
    '#weight' => 2,
  );
  $capture_js = array(
    '#type' => 'markup',
    '#prefix' => '<script type="text/javascript" src="'. $base_url . '/' . drupal_get_path('module', 'janrain_capture') . '/janrain_capture.js">',
    '#suffix' => '</script>',
    '#markup' => '',
    '#weight' => 1,
  );

  drupal_add_html_head($widget, 'janrain_capture_widget_js');
  drupal_add_html_head($capture_client, 'janrain_capture_client_js');
}

/**
 * Returns Capture widget js.
 */
function janrain_capture_widget_js() {
  global $base_url;
  global $base_path;
  global $language;

  $janrain_settings = variable_get('janrain_capture_fields2', array());
  $janrain_settings = array_merge($janrain_settings, variable_get('janrain_capture_main2', array()));
  $janrain_settings = array_merge($janrain_settings, variable_get('janrain_capture_ui2', array()));
  $janrain_settings = array_merge($janrain_settings, variable_get('janrain_capture_federate2', array()));
  $janrain_settings = array_merge($janrain_settings, variable_get('janrain_capture_backplane2', array()));

  // module
  $settings["plex.moduleVersion"] = JANRAIN_CAPTURE_MODULE_VERSION;

  // capture
  $settings["capture.redirectUri"]        = url('janrain_capture/oauth', array('absolute' => TRUE));
  $settings["capture.appId"]              = $janrain_settings['capture_app_id'];
  $settings["capture.clientId"]           = $janrain_settings['capture_client_id'];
  $settings["capture.responseType"]       = "code";
  $settings["capture.captureServer"]      = $janrain_settings['capture_address'];
  $settings["capture.loadJsUrl"]          = $janrain_settings['load_js'];

  $share_settings = variable_get('janrain_capture_share', array());
  if (isset($share_settings["enabled"]) && $share_settings["enabled"]) {
    $settings["packages"]                 = '["login","capture","share"]';
  }
  else {
    $settings["packages"]                 = '["login","capture"]';
  }

  // engage
  $settings["appUrl"]                     = $janrain_settings['engage_address'];

  // federate
  $settings["capture.federate"]           = $janrain_settings['capture_sso_enabled'];
  $settings["capture.federateServer"]     = 'https://' . $janrain_settings['capture_sso_address'];
  $settings["capture.federateXdReceiver"] = $base_url . base_path() . drupal_get_path('module', 'janrain_capture') . '/xdcomm.html';
  $settings["capture.federateLogoutUri"]  = url('janrain_capture/simple_logout', array('absolute' => TRUE));
  $settings["capture.federateSegment"]    = isset($janrain_settings['capture_sso_segment_name']) ? $janrain_settings['capture_sso_segment_name'] : '';

  if (isset($janrain_settings['capture_sso_supported_segment_names'])) {
    $segment_names = explode(',', $janrain_settings['capture_sso_supported_segment_names']);

    if ($segment_names) {
      $settings["capture.federateSupportedSegments"] = json_encode($segment_names);
    }
  }

  // backplane
  $settings["capture.backplane"]          = $janrain_settings['backplane_enabled'];
  $settings["capture.backplaneServerBaseUrl"] = isset($janrain_settings['backplane_server_base_url']) ? $janrain_settings['backplane_server_base_url'] : '';
  $settings["capture.backplaneBusName"]   = $janrain_settings['backplane_bus_name'];
  $settings["capture.backplaneVersion"]   = $janrain_settings['backplane_version'];

  // miscellaneous
  $settings["capture.language"]           = $language->language;
  $settings["mobileFriendly"] = empty($janrain_settings['mobile_friendly']) ? FALSE : (bool) $janrain_settings['mobile_friendly'];

  if (module_exists('janrain_capture_screens')) {
    $settings["capture.stylesheets"] = "'" . file_create_url(_janrain_capture_get_screen_file('stylesheets/styles.css')) . "'";
    if ($mobile_stylesheet = _janrain_capture_get_screen_file('stylesheets/mobile-styles.css')) {
      $settings["capture.mobileStylesheets"] = "'" . file_create_url($mobile_stylesheet) . "'";
    }
    if ($ie_stylesheet = _janrain_capture_get_screen_file('stylesheets/ie-styles.css')) {
      $settings["capture.conditionalIEStylesheets"] = "'" . file_create_url($ie_stylesheet) . "'";
    }
  }
  else {
    $folder_url = variable_get('janrain_capture_screens_folder', 'file:///sites/all/themes/janrain-capture-screens/');

    // If path is local, search for user agent-specific stylesheets in the file system.
    if (strpos($folder_url, 'file:///', 0) === 0) {
      // Example of $folder_url: file:///sites/all/themes/janrain-capture-screens/
      $web_path = str_replace('file://', '', $folder_url);
      // Example of $web_path: /sites/all/themes/janrain-capture-screens/
      $fs_path = DRUPAL_ROOT . $web_path;
      // Example of $fs_path: /var/www/d7/sites/all/themes/janrain-capture-screens/
      $web_url = $base_url . $web_path;

      $base_stylesheet_path = "'{$web_url}stylesheets/janrain.css'";
      $mobile_stylesheet_path = "'{$web_url}stylesheets/janrain_mobile.css'";
      $ie_stylesheet_path = "'{$web_url}stylesheets/janrain_ie.css'";

      $settings["capture.stylesheets"] = $base_stylesheet_path;
      $settings["capture.mobileStylesheets"] = $mobile_stylesheet_path;
      $settings["capture.conditionalIEStylesheets"] = $ie_stylesheet_path;
    }
    else {
      // Remote stylesheets
      $settings["capture.stylesheets"] = "'{$folder_url}stylesheets/janrain.css'";
      $settings["capture.mobileStylesheets"] = "'{$folder_url}stylesheets/janrain_mobile.css'";
      $settings["capture.conditionalIEStylesheets"] = "'{$folder_url}stylesheets/janrain_ie.css'";
    }

    // Log a warning if directories are setup properly but no stylesheets were found
    if (!count($settings["capture.stylesheets"]) && !count($settings["capture.mobileStylesheets"]) && !count($settings["capture.conditionalIEStylesheets"])) {
      watchdog('janrain_capture',
                 'No stylesheets were found in the screens folder (@path). Please check the Janrain Capture module settings.',
                 array('@path' => $fs_path ?: $folder_url),
                 WATCHDOG_WARNING,
                 l(t('Janrain Capture module settings'), 'admin/config/people/janrain_capture'));
    }
  }



  $output = <<<EOD
function janrainSignOut(){
  janrain.capture.ui.endCaptureSession();
}
(function() {
  if (typeof window.janrain !== 'object') window.janrain = {};
  window.janrain.plex = {};
  window.janrain.settings = {};
  window.janrain.settings.capture = {};

  // module settings
  janrain.plex.moduleVersion = '{$settings["plex.moduleVersion"]}';

  // capture settings
  janrain.settings.capture.redirectUri = '{$settings["capture.redirectUri"]}';
  janrain.settings.capture.appId= '{$settings["capture.appId"]}';
  janrain.settings.capture.clientId = '{$settings["capture.clientId"]}';
  janrain.settings.capture.responseType = '{$settings["capture.responseType"]}';
  janrain.settings.capture.captureServer = '{$settings["capture.captureServer"]}';
  janrain.settings.capture.registerFlow = 'socialRegistration';
  janrain.settings.packages = {$settings['packages']};

  janrain.settings.capture.setProfileCookie = true;
  janrain.settings.capture.keepProfileCookieAfterLogout = true;
  janrain.settings.capture.setProfileData = true;

  janrain.settings.capture.federateEnableSafari = true;

  // styles
  janrain.settings.capture.stylesheets = [{$settings["capture.stylesheets"]}];\n
EOD;

  // mobile styles
  if (isset($settings["capture.mobileStylesheets"]) && $settings["capture.mobileStylesheets"] != '') {
    $output .= "janrain.settings.capture.mobileStylesheets = [{$settings['capture.mobileStylesheets']}];\n";
  }

  //IE styles
  if (isset($settings["capture.conditionalIEStylesheets"]) && $settings["capture.conditionalIEStylesheets"] != '') {
    $output .= "janrain.settings.capture.conditionalIEStylesheets = [{$settings['capture.conditionalIEStylesheets']}];\n";
  }

  // captcha
  $output .= "janrain.settings.capture.recaptchaPublicKey = '6LeVKb4SAAAAAGv-hg5i6gtiOV4XrLuCDsJOnYoP';\n";

  $output .= <<<EOD
  // engage settings
  janrain.settings.appUrl = '{$settings["appUrl"]}';
  janrain.settings.tokenAction = 'event';\n
EOD;

  // Backplane
  if ($settings["capture.backplane"]) {
    $output .= <<<EOD
  // backplane settings
  janrain.settings.capture.backplane = '{$settings["capture.backplane"]}';
  janrain.settings.capture.backplaneBusName = '{$settings["capture.backplaneBusName"]}';
  janrain.settings.capture.backplaneVersion = '{$settings["capture.backplaneVersion"]}';\n
EOD;
    if ($settings['capture.backplaneServerBaseUrl']) {
      $output .= "janrain.settings.capture.backplaneServerBaseUrl = 'https://{$settings['capture.backplaneServerBaseUrl']}';\n";
    }
  }

  if ($settings["capture.federate"]) {
    $output .= <<<EOD
  // federate settings
  janrain.settings.capture.federate = '{$settings["capture.federate"]}';
  janrain.settings.capture.federateServer = '{$settings["capture.federateServer"]}';
  janrain.settings.capture.federateXdReceiver = '{$settings["capture.federateXdReceiver"]}';
  janrain.settings.capture.federateLogoutUri = '{$settings["capture.federateLogoutUri"]}';\n
EOD;

    if (isset($settings["capture.federateSegment"])) {
      $output .= "janrain.settings.capture.federateSegment = '{$settings["capture.federateSegment"]}';\n";
    }

    if (isset($settings["capture.federateSupportedSegments"])) {
      $output .= "janrain.settings.capture.federateSupportedSegments = {$settings["capture.federateSupportedSegments"]};\n";
    }
  }

  if ($settings["capture.language"]) {
    $output .= "janrain.settings.language = '{$settings["capture.language"]}';\n";
  }

  if ($settings["mobileFriendly"]) {
    global $base_root;
    $current_url = $base_root . request_uri();
    $_SESSION['janrain_capture_redirect_uri'] = $current_url;

    $output .=
      "\n// mobile-specific settings
      janrain.settings.tokenAction = 'url';
      janrain.settings.popup = false;
      janrain.settings.tokenUrl = janrain.settings.capture.captureServer;
      janrain.settings.capture.redirectUri = '{$current_url}';
      janrain.settings.capture.redirectFlow = true;\n";
  }
  else {
    if (isset($_SESSION['janrain_capture_redirect_uri'])) {
      unset($_SESSION['janrain_capture_redirect_uri']);
    }
  }

  if (!isset($_SESSION['janrain_capture_access_token'])) {
    $api = new JanrainCaptureApi();
    $api->refreshAccessToken();
  }
  $access_token = "var access_token = '";
  $access_token .= isset($_SESSION['janrain_capture_access_token']) ? $_SESSION['janrain_capture_access_token'] : "";
  $access_token .= "';";

  $output .= <<<EOD
  function isReady() { janrain.ready = true; };
  if (document.addEventListener) {
    document.addEventListener("DOMContentLoaded", isReady, false);
  } else {
    window.attachEvent('onload', isReady);
  }

  var e = document.createElement('script');
  e.type = 'text/javascript';
  e.id = 'janrainAuthWidget';
  var url = document.location.protocol === 'https:' ? 'https://' : 'http://';
  url += '{$settings["capture.loadJsUrl"]}';
  e.src = url;
  var s = document.getElementsByTagName('script')[0];
  s.parentNode.insertBefore(e, s);
})();
{$access_token}

EOD;

  return $output;
}

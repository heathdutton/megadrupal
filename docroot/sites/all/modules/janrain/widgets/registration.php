<?php
/**
 * @file
 * Implementation of Janrain Registration Widget integration.
 */

/**
 * Implements hook_block_view().
 *
 * When Registration is in play, this file defines the necessary hooks and
 * alterations to Drupal.
 * Render login form in blocks for submit in js and login.php.
 */
function janrain_widgets_block_view($delta = '') {
  // Parse out widget file ID.
  list($screen, $fid) = explode('_', $delta);
  $file = file_load($fid);
  if (!$file) {
    return array(
      'content' => array(
        '#type' => 'markup',
        '#markup' => 'Janrain Registration Widget not found.',
      ),
    );
  }

  // Render a widget with a legit file.
  $block = array();
  // Filter editProfiles for users without them.
  if ('editProfile' == $screen && !_janrain_user_has_capture_uuid()) {
    // Different message for admins trying to view other profiles.
    if (arg(0) == 'user' && (arg(1) != $GLOBALS['user']->uid)) {
      $block['content']['#markup'] = 'Janrain profiles are only viewable by their users.';
    }
    else {
      $block['content']['#markup'] = 'You have no capture profile to show.';
    }
    return $block;
  }

  $sdk = JanrainSdk::instance();

  $block['content']['#attached']['js'] = array();
  $js = file_get_contents(drupal_get_path('module', 'janrain_widgets') . '/registration.js');
  $js .= $sdk->renderJs();

  $widget_folder_uri = "public://janrain_widgets/" . basename($file->filename, '.zip');
  $widget_folder = drupal_realpath($widget_folder_uri);
  if (file_exists("$widget_folder/janrain.css")) {
    $js .= "janrain.settings.capture.stylesheets = ['" . file_create_url("$widget_folder_uri/janrain.css") . "'];\n";
  }
  if (file_exists("$widget_folder/janrain-mobile.css")) {
    $js .= "janrain.settings.capture.mobileStylesheets = ['" . file_create_url("$widget_folder_uri/janrain-mobile.css") . "'];\n";
  }
  if (file_exists("$widget_folder/janrain-ie.css")) {
    $js .= "janrain.settings.capture.conditionalIEStylesheets = ['" . file_create_url("$widget_folder_uri/janrain-ie.css") . "'];\n";
  }
  $js .= sprintf("janrain.settings.capture.screenToRender = '%s';\n", check_plain($screen));
  if (user_is_logged_in()) {
    $js .= "janrain.settings.capture.federate = false;\n";
  }
  $block['content']['#attached']['js'][] = array(
    'data' => $js,
    'type' => 'inline');
  $block['content']['#markup'] = '';
  $block['content']['#attached']['js'][] = array('data' => "$widget_folder_uri/janrain-init.js", 'type' => 'file');
  $block['content']['#attached']['js'][] = array('data' => "$widget_folder_uri/janrain-utils.js", 'type' => 'file');
  $block['content']['#markup'] .= file_get_contents("$widget_folder_uri/screens.html");

  $clean_url = variable_get('clean_url', FALSE);
  if ($clean_url) {
    $form_action = $GLOBALS['base_url'] . "/user/login";
  }
  else {
    $form_action = $GLOBALS['base_url'] . "/?q=user/login";
  }
  // @todo generate form from Forms API to gain security and stuff
  $block['content']['#markup'] .= '<form action="' . check_url($form_action) . '"  style="display:none;" method="post" id="user_login"><input name="form_id" value="user_login"/></form>';
  return $block;
}

/**
 * Implements hook_block_info().
 */
function janrain_widgets_block_info() {
  $blocks = array();

  // Load packages.
  foreach (_janrain_widgets_list_pkgs() as $uri => $fid) {
    $tag = basename($uri, '.zip');
    $blocks["signIn_{$fid}"] = array(
      'info' => "Janrain Login ({$tag})",
      'cache' => DRUPAL_CACHE_PER_ROLE | DRUPAL_CACHE_PER_PAGE,
    );
    $blocks["verifyEmail_{$fid}"] = array(
      'info' => "Janrain Email Verify ({$tag})",
      'cache' => DRUPAL_CACHE_PER_ROLE | DRUPAL_CACHE_PER_PAGE,
    );
    $blocks["editProfile_{$fid}"] = array(
      'info' => "Janrain Profile ({$tag})",
      'cache' => DRUPAL_CACHE_PER_ROLE | DRUPAL_CACHE_PER_PAGE,
    );
    $blocks["resetPasswordRequestCode_{$fid}"] = array(
      'info' => "Janrain Password Recover ({$tag})",
      'cache' => DRUPAL_CACHE_PER_ROLE | DRUPAL_CACHE_PER_PAGE,
    );
  }
  return $blocks;
}

/**
 * Implements hook_preprocess_link().
 *
 * Make sure logout links also end capture sessions.
 */
function janrain_widgets_preprocess_link(&$items) {
  if ($items['path'] == 'user/logout' && JanrainSdk::instance()->FederateWidget) {
    $items['options']['attributes']['onclick'][] = 'janrain.plex.ssoLogout();';
  }
}

<?php

/**
 * @file
 *
 * This is file does a minimal drupal bootstrap and returns the current
 * recent supporter data.
 */

function campaignion_recent_supporters_bootstrap_inc() {
  # set base_url explicitly as SCRIPT_NAME would lead to
  # a 'wrong' base_url for the site
  global $base_url;
  $is_https = isset($_SERVER['HTTPS']) && strtolower($_SERVER['HTTPS']) == 'on';
  $http_protocol = $is_https ? 'https' : 'http';
  $base_root = $http_protocol . '://' . $_SERVER['HTTP_HOST'];
  # base_url gets stripped to it's correct location below
  $base_url = dirname($base_root . $_SERVER['SCRIPT_NAME']);

  $dir = dirname($_SERVER['DOCUMENT_ROOT'] . $_SERVER['SCRIPT_NAME']);
  while ($dir != '/') {
    $bootstrap = $dir . '/includes/bootstrap.inc';
    if (file_exists($bootstrap)) {
      define('DRUPAL_ROOT', $dir);
      return $bootstrap;
    }
    $dir = dirname($dir);
    $base_url = dirname($base_url);
  }
}

if ($bootstrap = campaignion_recent_supporters_bootstrap_inc()) {
  require_once $bootstrap;
}

_drupal_bootstrap_configuration();
_drupal_bootstrap_database();
require_once DRUPAL_ROOT . '/includes/common.inc';
require_once DRUPAL_ROOT . '/includes/cache.inc';
spl_autoload_unregister('drupal_autoload_class');
spl_autoload_unregister('drupal_autoload_interface');
foreach (variable_get('cache_backends', array()) as $include) {
  require_once DRUPAL_ROOT . '/' . $include;
}
drupal_load('module', 'psr0');
spl_autoload_register('drupal_autoload_class');
spl_autoload_register('drupal_autoload_interface');
require_once dirname(__FILE__) . '/campaignion_recent_supporters.module';

use \Drupal\campaignion_recent_supporters\RequestParams;
use \Drupal\campaignion_recent_supporters\ActivityBackend;

$params = new RequestParams($_GET);
if (!$params->isValid()) {
  drupal_add_http_header("Access-Control-Allow-Origin", "*");
  drupal_add_http_header("Access-Control-Allow-Headers", "Content-Type");
  drupal_add_http_header('Status', '403 Forbidden');
  // no wtachlog log possible due to minimal bootstrap
  drupal_json_output(array());
  exit;
}

$backend = $params->getBackend('activity');

if (isset($_GET['nid'])) {
  $backend->recentOnOneActionJson($params);
}
elseif (isset($_GET['types'])) {
  $backend->recentOnAllActionsJson($params);
}
else {
  $backend->emptyJson();
}


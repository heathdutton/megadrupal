<?php

/**
 * BAWstats drupal module
 *
 * BAWstats is a drupal module written in php for integration of
 * the BetterAWstats presentation and processing of AWstats data into drupal.
 * Copyright (C) 2008  Andrew Gillies (anaru@equivocation.org)
 *
 * For more information about BetterAWstats see:
 * http://betterawstats.com/
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or, at
 * your option, any later version.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 *
 */

$bawstats_path = drupal_get_path('module', 'bawstats');

/**
 * beterawstats globals and libraries
 *
 */

// global vars
global $BAW_LOGTYPE, $BAW_CURR, $BAW_MES, $BAW_LIB, $BAW_CONF_DEF, $BAW_CONF_DIS_DEF, $BAW_D;
global $BAW_CONF, $BAW_CONF_DIS, $BAW_DFILES, $BAW_SERVERS;

$BAW_CONF = array(); // holds all the configuration info

include_once DRUPAL_ROOT . '/' . $bawstats_path . '/config.php';
// NAME:    Script path
// INFO:    The path of BetterAWstats, No trailing slash
// DEFAULT: '/path/to/betterawstats/'
$BAW_CONF['site_path'] = $bawstats_path;
$BAW_CONF['max_percent'] = 'layout_percent';
$BAW_CONF['max_bytes'] = 'layout_bytes';
// NAME:    Web Configuration
// INFO:    Enable the online configuration editor? WARNING: Your config.php has
//          to be writable in order to enable this. This is a BAD idea to use
//          unless the folder is password-protected with a .htaccess file or
//          similar.
// DEFAULT: true
// POSSIBLE VALUES: true, false
$BAW_CONF['online_config'] = false;
$BAW_CONF['drupal_base'] = 'admin/reports/bawstats';

// get configuration from drupal variables (not from the config.php) for key locations
// NAME:    Path to AWStats Data
// INFO:    Set this value to the directory where AWStats saves its database
//          files into. ATTENTION: If you read those files on windows but have
//          them created on linux or the other way round, make sure you transfer
//          them 'BINARY'. Otherwise they cannot be read properly. No trailing
//          slash
// DEFAULT: '/path/to/betterawstats/awstats/data'
$BAW_CONF['path_data'] = variable_get('bawstats_data', '');
// NAME:    Path to AWStats Libraries
// INFO:    Set this value to the directory where AWStats saves its library
//          files into. No trailing slash
// DEFAULT: '/path/to/betterawstats/awstats/lib'
$BAW_CONF['path_lib'] = variable_get('bawstats_lib', '');
// NAME:    Path to AWStats Language files
// INFO:    Set this value to the directory where AWStats saves its language
//          files into. No trailing slash
// DEFAULT: '/path/to/betterawstats/awstats/lang'
$BAW_CONF['path_lang'] = variable_get('bawstats_lang', '');
// NAME:    URL to AWStats Icons
// INFO:    The url to the awstats icons, should include the whole http://...,
//          no trailing slash.
// DEFAULT: 'http://awstats.local/awstats/icon'
$BAW_CONF['icons_url'] = libraries_get_path('bawstats', TRUE) . '/icon';

// drupal map
$BAW_CONF_DIS['domains']['map'] = true;

$BAW_LOGTYPE = 'W';
$BAW_D = array();
$BAW_CURR = array();
$BAW_CURR['thismonth'] = array('hits' => 0);
$BAW_MES = array();
$BAW_DFILES = array(); // data files
$BAW_SERVERS = array(); // this will be populates with the servers for the config

if (!defined('XHTML')) {
  if ($BAW_CONF['xhtml']) {
    define('XHTML', ' /');
  }
  else {
    define('XHTML', '');
  }
}
if (!defined('BR')) {
  define('BR', '<br' . XHTML . '>');
}

// check we are configured...
if ((strlen($BAW_CONF['path_data']) > 0) && (strlen($BAW_CONF['path_lib']) > 0) && (strlen($BAW_CONF['path_lang']) > 0)) {
  include_once DRUPAL_ROOT . '/' . $bawstats_path . '/engine/helpers.inc.php';
  include_once DRUPAL_ROOT . '/' . $bawstats_path . '/modules/render_htmlchart.inc.php';
  include_once DRUPAL_ROOT . '/' . $bawstats_path . '/modules/render_table.inc.php';
  include_once DRUPAL_ROOT . '/' . $bawstats_path . '/modules/render_jpgraph.inc.php';
  include_once DRUPAL_ROOT . '/' . $bawstats_path . '/modules/render_map.inc.php';
  include_once DRUPAL_ROOT . '/' . $bawstats_path . '/engine/data.inc.php';
  include_once DRUPAL_ROOT . '/' . $bawstats_path . '/engine/language.inc.php';
  baw_language();
  include_once DRUPAL_ROOT . '/' . $bawstats_path . '/engine/display_helpers.php';

  if (!isset($BAW_MES['e'])) {
    $BAW_MES['e'] = 'utf-8';
  }

  //baw_check_config();
  include_once DRUPAL_ROOT . '/' . $bawstats_path . '/engine/library.inc.php';

  include_once DRUPAL_ROOT . '/' . $bawstats_path . '/engine/display.inc.php';
  include_once DRUPAL_ROOT . '/' . $bawstats_path . '/engine/extras.inc.php';

  // add language-dependent library values
  $BAW_LIB['domains']['unknown'] = $BAW_MES[0];
  $BAW_LIB['domains']['ip'] = $BAW_MES[0];
  $BAW_LIB['os']['list']['Unknown'] = $BAW_MES[0];
  $BAW_LIB['browser']['names']['Unknown'] = $BAW_MES[0];
  $BAW_LIB['browser']['icons']['Unknown'] = 'unknown';
  include_once DRUPAL_ROOT . '/' . $bawstats_path . '/engine/config_default.inc.php';
}

// NAME:    Script URL
// INFO:    The url of BetterAWstats' directory, No trailing slash
// DEFAULT: 'http://awstats.local'
$BAW_CONF['site_url'] = '/admin/bawstats';

// ensure the baw backend knows we are drupal
$BAW_CONF['module'] = 'drupal';

unset($bawstats_path);

/**
 * Return array of groups of statistics
 *
 * @param $BAW_CONF Configuration array
 * @param $BAW_LIB Library array containing information from AWStats config
 *
 * @return Array of groups of statistics
 */
function bawstats_groups($BAW_CONF, $BAW_LIB) {
  $statgroups = array();
  // Only get return non-empty array if the system is configured
  if ((strlen($BAW_CONF['path_data']) > 0) && (strlen($BAW_CONF['path_lib']) > 0) && (strlen($BAW_CONF['path_lang']) > 0)) {
    foreach ($BAW_LIB['item_groups'] as $group => $group_details) {
      $statgroups[check_plain($group)] = check_plain($group_details['title']);
    }
  }
  return($statgroups);
}

/**
 * Return array of sections
 *
 * @param $BAW_CONF Configuration array
 * @param $BAW_CONF_DIS_DEF
 *
 * @return Array of statistics sections
 */
function bawstats_sections($BAW_CONF, $BAW_CONF_DIS_DEF) {
  $statsections = array();
  // Only get return non-empty array if the system is configured
  if ((strlen($BAW_CONF['path_data']) > 0) && (strlen($BAW_CONF['path_lib']) > 0) && (strlen($BAW_CONF['path_lang']) > 0)) {
    foreach ($BAW_CONF_DIS_DEF as $key => $value) {
      $value['name'] = check_plain($value['name']);
      $value['help'] = check_plain($value['help']);
      $statsections[check_plain($key)] = $value;
    }
  }
  return($statsections);
}


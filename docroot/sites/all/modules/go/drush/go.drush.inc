<?php

/**
 * @file
 *   Go Drush commands
 */

require_once dirname(__FILE__) . '/../includes/go.autoload.inc';
require_once dirname(__FILE__) . '/go.cron.inc';
require_once dirname(__FILE__) . '/go.hipchat.inc';
require_once dirname(__FILE__) . '/go.dev.inc';
require_once dirname(__FILE__) . '/go.live.inc';
require_once dirname(__FILE__) . '/go.mail.inc';

if (version_compare(DRUSH_MAJOR_VERSION, 5) >= 0) {
  require_once dirname(__FILE__) . '/go.download.inc';
  require_once dirname(__FILE__) . '/go.require.inc';
}

/**
 * Implements hook_drush_command()
 *
 * @return array
 */
function go_drush_command() {
  $items['go-hipchat'] = array(
    'description' => 'Send message to Hipchat room.',
    'arguments' => array(
      'room_id' => 'ID of the chatroom.',
      'message' => 'Message to be sent to the room.',
    ),
    'options' => array(
      'from' => 'Sender name, default: Go Module',
      'token' => 'API Token, default: value of GO_HIPCHAT_API_TOKEN constant',
      'color' => 'Background color for the message (yellow, red, green, purple or random - default: yellow)',
      'notify' => 'Trigger notification for people in the room, default: 0',
    ),
    'aliases' => array('gohc'),
    'examples' => array(
      'drush gohc p.GoCMS "Hello room!"' => 'Send "Hello room!" to p.GoCMS chatroom.',
      'drush gohc Go1 "Log time please guys!" --from="Daniel Freshwater" --notify=1' => 'Freshy notify all people in Go1 room to log time.',
    ),
  );

  $items['go-cron'] = array(
    'description' => 'Run specific cron job',
    'arguments' => array(
      'module' => 'Machine name of the module.',
    ),
    'aliases' => array('goc'),
    'examples' => array(
      'drush goc system' => 'Fire system_cron() function.',
    ),
  );

  $items['go-dev'] = array(
    'description' => dt('Special drush commands, provided by Go1, execute some dev configurations.'),
    'options' => array(
      'devel'                 => '0 to disable devel.module. Default: 1',
      'environment_indicator' => '0 to disable environment_indicator.module. Default: 1',
      'update'                => '1 to enable update.module. Default: 0',
      'page_cache'            => 'Enable or disable page caching, default = 0',
      'block_cache'           => 'Enable or disable block caching, default = 0',
      'compress_js'           => 'Enable or disable js aggregation, default = 0',
      'compress_css'          => 'Enable or disable js aggregation, default = 0',
      'views-preview'         => 'Enable or disable "Automatically update preview on changes", default = 0',
      'views-sql'             => 'Enable or disable "Show the SQL query", default = 1',
      'views-queries'         => 'Enable or disable "Show other queries run during render during live preview", default = 1',
      'views-help'            => 'Enable or disable "Show advanced help warning", default = 0',
    ),
    'examples' => array(
      'drush godev --devel' => 'Enable devel module.',
      'drush godev --devel=0' => 'Disable devel module.',
    ),
    'aliases' => array('godev'),
    'bootstrap' => DRUSH_BOOTSTRAP_DRUPAL_FULL,
  );

  $items['go-mail'] = array(
    'description' => dt('Command to sent email.'),
    'options' => array(
      'subject' => dt('Mail subject'),
      'body' => dt('Mail body'),
      'from' => dt('Mail sender'),
      'to' => dt('Mail receiver'),
    ),
    'examples' => array(
      'drush gomail --subject="Test email" --body="Test content" --from=admin@example.com --to=tester@example.com' => 'Enable devel module.',
    ),
    'aliases' => array('gomail'),
    'bootstrap' => DRUSH_BOOTSTRAP_DRUSH,
  );

  $items['go-live'] = array(
    'description' => dt('Command to enable page/block caching and js/css aggregation Simultaneous Disable UI modules enable module Update!'),
    'option' => array(
      'cache'  => '0 to disable site caching. Default: 1',
      'js'     => '0 to disable JS aggregation. Default: 1',
      'css'    => '0 to disable CSS aggregation. Default: 1',
      'dev'    => '1 to enable devel module. Default: 0',
      'update' => '0 to disable update.module. Default: 1',
    ),
    'examples' => array(
      'drush golive --cache=1 --js=1 --update=1',
      'description' => "--cache=1 if enable page/block caching or --cache=0 not enable \n\r--js=1 if enable js/css aggregation --js=0 to vice versa \n\r--update=1 if enable module update or --update=0 if not enable \n\rNotes: Do not use option will to the default value is 1",
    ),
    'aliases' => array('golive'),
    'bootstrap' => DRUSH_BOOTSTRAP_DRUPAL_FULL,
  );

  if (version_compare(DRUSH_MAJOR_VERSION, 5) >= 0) {
    $items['go-download'] = array(
      'description' => 'Command to download libraries for you Drupal',
      'arguments' => array(
        'library' => 'Name of library to be downloaded.',
      ),
      'options' => array(
        'list' => 'List all supported libraries',
        'destination' => 'Directory you would like to download the library to.',
        'working-copy' => 'Preserves VCS directories, like .git, for projects downloaded using such methods.',
      ),
      'examples' => array(
        'drush godl --list' => 'List supported libraries.',
        'drush godl jquery.cycle' => 'Download the jquery.cycle library.',
      ),
      'aliases' => array('godl'),
      // No bootstrap is needed
      'bootstrap' => DRUSH_BOOTSTRAP_DRUSH,
    );

    $items['go-require'] = array(
      'description' => 'Download dependencies for Drupal — modules/themes/libraries/…',
      'examples' => array(
        'drush go-require' => 'Make sure all dependencies are downloaded.',
      ),
      'aliases' => array('gorequire', 'gor'),
      'bootstrap' => DRUSH_BOOTSTRAP_DRUSH,
      'engines' => array('release_info'),
    );
  }

  return $items;
}

/**
 * Request confirmation from the user
 *
 * @param string $msg message will print
 */
function go_drush_confirm($msg) {
  $y_option = drush_get_option('y');
  if ($y_option != 1) {
    return drush_confirm($msg);
  }

  return TRUE;
}

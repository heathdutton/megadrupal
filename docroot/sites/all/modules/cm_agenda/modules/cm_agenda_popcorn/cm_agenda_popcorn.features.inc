<?php
/**
 * @file
 * cm_agenda_popcorn.features.inc
 */

/**
 * Implements hook_mediafront_default_presets().
 */
function cm_agenda_popcorn_mediafront_default_presets() {
  $items = array(
    'popcorn' => array(
      'name' => 'popcorn',
      'description' => '',
      'player' => 'popcornplayer',
      'connect' => array(
        'iscontroller' => array(
          0 => 0,
        ),
        'isplaylist' => array(
          0 => 0,
        ),
      ),
      'settings' => array(
        'debug' => 1,
        'disptime' => 0,
        'duration' => 0,
        'volume' => 80,
        'wmode' => 'transparent',
        'preload' => TRUE,
        'autoplay' => 1,
        'autoload' => 1,
        'scrollMode' => 'auto',
        'scrollSpeed' => 20,
        'showPlaylist' => 1,
        'vertical' => 1,
        'node' => array(),
        'playlist' => '',
        'pageLimit' => 10,
        'preset' => '',
        'autoNext' => 1,
        'shuffle' => 0,
        'loop' => 0,
        'logo' => '/sites/all/modules/popcornplayer/player/logo.png',
        'swfplayer' => '/sites/all/modules/mediafront_popcornplayer/player/minplayer/flash/minplayer.swf',
        'link' => 'http://www.mediafront.org',
        'width' => '100%',
        'height' => '450px',
        'template' => 'default',
        'playlistOnly' => 0,
        'disablePlaylist' => 0,
        'controllerOnly' => 0,
        'volumeVertical' => 1,
        'plugins' => array(),
        'theme' => 'dark-hive',
        'showWhenEmpty' => 1,
        'showController' => 0,
        'prereel' => '',
        'postreel' => '',
        'player_settings__active_tab' => 'edit-player-settings-misc',
      ),
    ),
  );
  return $items;
}

<?php

function yp_cgi() {
  if (variable_get('yp_debug', 0)) {
    watchdog('yp', check_plain(print_r($_REQUEST, TRUE)), NULL, WATCHDOG_DEBUG);
  }
  $action = isset($_REQUEST['action']) ? trim($_REQUEST['action']) : '';
  $actions = array('add' => 'YPCGIAdd', 'touch' => 'YPCGITouch', 'remove' => 'YPCGIRemove');
  if (isset($actions[$action])) {
    new $actions[$action];
  }
  else {
    return MENU_ACCESS_DENIED;
  }
}

class YPCGI {

  function __construct() {
    $this->last_touch = REQUEST_TIME;
    foreach ($this->map as $key => $variables) {
      $this->$key = '';
      foreach ($variables as $variable) {
        if (isset($_REQUEST[$variable])) {
          $this->$key = trim($_REQUEST[$variable]);
        }
      }
    }
    $this->execute();
    drupal_add_http_header('YPMessage', $this->yp_message);
    drupal_add_http_header('YPResponse', $this->yp_response);
  }
}

class YPCGIAdd extends YPCGI {

  var $map = array(
    'server_name' => array('sn'),
    'server_type' => array('type'),
    'genre' => array('genre'),
    'bitrate' => array('audio_bitrate', 'b', 'bitrate', 'ice-bitrate'),
    'samplerate' => array('audio_samplerate', 'samplerate', 'ice-samplerate'),
    'channels' => array('audio_channels', 'channels', 'ice-channels'),
    'listen_url' => array('listenurl'),
    'description' => array('desc'),
    'url' => array('url'),
    'cluster_password' => array('cpswd'),
  );

  function execute() {
    $this->listing_ip = ip_address();
    drupal_write_record('yp_stream', $this);
    $this->yp_response = $this->sid ? 1 : 0;
    $this->yp_message = $this->yp_response ? 'Added' : 'Error';
    drupal_add_http_header('SID', $this->sid);
    drupal_add_http_header('TouchFreq', 200);
  }
}

class YPCGITouch extends YPCGI {

  var $map = array(
    'sid' => array('sid'),
    'listeners' => array('listeners'),
    'max_listeners' => array('max_listeners'),
    'server_subtype' => array('stype'),
    'current_song' => array('st'),
  );

  function execute() {
    $this->yp_response = drupal_write_record('yp_stream', $this, 'sid') ? 1 : 0;
    $this->yp_message = $this->yp_response ? 'Touched' : 'SID not found';
  }
}

class YPCGIRemove extends YPCGI {

  var $map = array(
    'sid' => array('sid'),
  );

  function execute() {
    db_delete('yp_stream')->condition('sid', $this->sid)->execute();
    $this->yp_response = 1;
    $this->yp_message = 'Removed';
  }
}

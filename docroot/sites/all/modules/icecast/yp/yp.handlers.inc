<?php

/**
 * Render stream bitrate with a "k" for kbps.
 */
class views_handler_field_yp_bitrate extends views_handler_field {
  function render($values) {
    $value = $values->{$this->field_alias};
    $value = $value ? $value . 'k' : t('VBR');
    // Integer field does not require check_plain().
    return $value;
  }
}

/**
 * Render "unknown" current song as empty string.
 */
class views_handler_field_yp_current_song extends views_handler_field {
  function render($values) {
    $value = $values->{$this->field_alias};
    $value = ($value == 'unknown - unknown') ? '' : $value;
    return check_plain($value);
  }
}

/**
 * Render listen URL as a .M3U link.
 */
class views_handler_field_yp_listen_url extends views_handler_field {
  function render($values) {
    $value = $values->{$this->field_alias} . '.m3u';
    return l(theme('image', array('path' => drupal_get_path('module', 'yp') . '/listen.png', 'alt' => t('Listen'), 'title' => t('Listen'), 'attributes' => array('width' => 16, 'height' => 16), 'getsize' => FALSE)), $value, array('html' => TRUE)) . l(t('Listen'), $value);
  }
}

/**
 * Render stream URL as a link.
 */
class views_handler_field_yp_server_name extends views_handler_field {
  function construct() {
    parent::construct();
    $this->additional_fields['url'] = 'url';
  }

  function render($values) {
    return l($values->{$this->field_alias}, $values->{$this->aliases['url']});
  }
}

/**
 * Render stream type as an abbreviation.
 */
class views_handler_field_yp_server_type extends views_handler_field {
  function render($values) {
    $value = $values->{$this->field_alias};
    switch ($value) {
      case 'application/ogg':
      case 'Ogg Vorbis':
        $value = 'Ogg';
        break;
      case 'audio/mpeg':
      case 'MP3 audio':
      case 'application/mp3':
        $value = 'MP3';
        break;
      case 'audio/aacp':
        $value = 'AAC+';
        break;
    }
    return check_plain($value);
  }
}

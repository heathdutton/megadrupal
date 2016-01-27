<?php

/**
 * @file
 * Irchwebchat helper/render functions
 */

/**
 * Generates output for block or page as convenient
 */
function _irchwebchat_generate_chat_form($type) {
  if ($type == IRCHWEBCHAT_DISPLAY_TYPE_FORM) {
    $modificadores = _irchwebchat_generar_modificadores_webchat();
    $host = 'www';
    $resource = 'webchat/webchat.php';
    $width = '210';
    $height = '173';
  }
  elseif ($type == IRCHWEBCHAT_DISPLAY_TYPE_EMBED) {
    $modificadores = '?canal=' . urlencode(str_replace('#', '', variable_get('irchwebchat_default_channel', '#irc-hispano')));
    $host = 'minichat';
    $resource = '';
    $width = '590';
    $height = '400';
  }
  else {
    // ERROR
    return '';
  }

  $output = '<center>';
  $output .= '<iframe marginwidth="0" marginheight="0" src="http://' . $host . '.irc-hispano.es/' . $resource . $modificadores . '" width="' . $width . '" height="' . $height . '" scrolling="no" frameborder="0"></iframe>';
  $output .= '</center>';

  return $output;
}

/**
 *  Helper function to generate the query part of the url
 */
function _irchwebchat_generar_modificadores_webchat() {
  global $user;
  $webchat_nick = $user->uid ? $user->name : variable_get('irchwebchat_nick', 'Guest');

  // aquí habrá que obtener las variables configuradas y generar la cadena de modificadores
  $modificadores = array();
  // modificador de nick
  $modificadores[] = 'nickinicial=' . urlencode($webchat_nick);
  // modificador para el canal por defecto
  $modificadores[] = 'canal=' . urlencode(str_replace('#', '', variable_get('irchwebchat_default_channel', '#irc-hispano')));
  // modificador para 'bloqueo' del canal por defecto
  $modificadores[] = 'locked=' . variable_get('irchwebchat_locked_channel', 0);
  //modificador para el canal provincial
  $modificadores[] = 'j=' . variable_get('irchwebchat_local_channel', '1');
  

  $cadena_modificadores = '?' . implode('&amp;', $modificadores);

  return $cadena_modificadores;
}

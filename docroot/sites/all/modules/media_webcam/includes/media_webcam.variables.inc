<?php

/**
 * @file Contains the variables and defaults used by the Media: Webcam module.
 */

/**
 * The variable namespace for Media: Webcam.
 */
define('MEDIA_WEBCAM_NAMESPACE', 'media_webcam__');

/**
 * Wrapper for variable_get() that uses the Media: Webcam variable registry.
 *
 * @param string $name
 *  The variable name to retrieve. Note that it will be namespaced by
 *  pre-pending MEDIA_WEBCAM_NAMESPACE, as to avoid variable collisions
 *  with other modules.
 * @param unknown $default
 *  An optional default variable to return if the variable hasn't been set
 *  yet. Note that within this module, all variables should already be set
 *  in the media_webcam_variable_default() function.
 * @return unknown
 *  Returns the stored variable or its default.
 *
 * @see media_webcam_variable_set()
 * @see media_webcam_variable_del()
 * @see media_webcam_variable_default()
 */
function media_webcam_variable_get($name, $default = NULL) {
  // Allow for an override of the default.
  // Useful when a variable is required (like $path), but namespacing still desired.
  if (!isset($default)) {
    $default = media_webcam_variable_default($name);
  }
  // Namespace all variables
  $variable_name = MEDIA_WEBCAM_NAMESPACE . $name;
  return variable_get($variable_name, $default);
}

/**
 * Wrapper for variable_set() that uses the Media: Webcam variable registry.
 *
 * @param string $name
 *  The variable name to set. Note that it will be namespaced by
 *  pre-pending MEDIA_WEBCAM_NAMESPACE, as to avoid variable collisions with
 *  other modules.
 * @param unknown $value
 *  The value for which to set the variable.
 * @return unknown
 *  Returns the stored variable after setting.
 *
 * @see media_webcam_variable_get()
 * @see media_webcam_variable_del()
 * @see media_webcam_variable_default()
 */
function media_webcam_variable_set($name, $value) {
  $variable_name = MEDIA_WEBCAM_NAMESPACE . $name;
  return variable_set($variable_name, $value);
}

/**
 * Wrapper for variable_del() that uses the Media: Webcam variable registry.
 *
 * @param string $name
 *  The variable name to delete. Note that it will be namespaced by
 *  pre-pending MEDIA_WEBCAM_NAMESPACE, as to avoid variable collisions with
 *  other modules.
 *
 * @see media_webcam_variable_get()
 * @see media_webcam_variable_set()
 * @see media_webcam_variable_default()
 */
function media_webcam_variable_del($name) {
  $variable_name = MEDIA_WEBCAM_NAMESPACE . $name;
  variable_del($variable_name);
}

/**
 * The default variables within the Media: Webcam namespace.
 *
 * @param string $name
 *  Optional variable name to retrieve the default. Note that it has not yet
 *  been pre-pended with the MEDIA_WEBCAM_NAMESPACE namespace at this time.
 * @return unknown
 *  The default value of this variable, if it's been set, or NULL, unless
 *  $name is NULL, in which case we return an array of all default values.
 *
 * @see media_webcam_variable_get()
 * @see media_webcam_variable_set()
 * @see media_webcam_variable_del()
 */
function media_webcam_variable_default($name = NULL) {
  static $defaults;

  if (!isset($defaults)) {
    $defaults = array(
      'width' => 637,
      'height' => 357,
      'no_flash' => <<<NO_FLASH
        <p>You must have Flash installed to use the webcam video recorder.</p>
        <p><a href="http://www.adobe.com/go/getflashplayer"><img src="http://www.adobe.com/images/shared/download_buttons/get_flash_player.gif" alt="" title=""></a></p>
NO_FLASH
,
      'connect_url' => 'rtmp://localhost/oflaDemo',
      'base_filename' => 'mediaWebcamVideo',
      'upload_folder' => '/usr/share/red5/webapps/oflaDemo/streams',
      'file_directory' => 'media-webcam',
      'max_duration' => 600,
    );
  }

  if (!isset($name)) {
    return $defaults;
  }

  if (isset($defaults[$name])) {
    return $defaults[$name];
  }
}

/**
 * Return the fully namespace variable name.
 *
 * @param string $name
 *  The variable name to retrieve the namespaced name.
 * @return string
 *  The fully namespace variable name, prepended with
 *  MEDIA_WEBCAM_NAMESPACE.
 */
function media_webcam_variable_name($name) {
  return MEDIA_WEBCAM_NAMESPACE . $name;
}

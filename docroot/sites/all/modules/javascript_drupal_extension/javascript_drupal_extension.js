/**
 * @file
 * Exposes Drupal functions to Javascript via ajax calls.
 */

/**
 * Returns the path to a system item (module, theme, etc.).
 *
 * @param string type
 *   The type of the item (i.e. theme, theme_engine, module, profile).
 * @param string name
 *   The name of the item for which the path is requested.
 * @param string callback_success
 *   (optional) The name of the function to call if successfull.
 * @param string callback_failure
 *   (optional) The name of the function to call if fail.
 *
 * @return string
 *   The path to the requested item.
 */
Drupal.drupal_get_path = function (type, name, callback_success, callback_failure) {
  return javascript_drupal_extension_ajax_call('drupal_get_path', { 'type': type, 'name': name }, callback_success, callback_failure);
}

/**
 * Logs a system message.
 *
 * @param $type
 *   The category to which this message belongs. Can be any string, but the
 *   general practice is to use the name of the module calling watchdog().
 * @param $message
 *   The message to store in the log. Keep $message translatable by not
 *   concatenating dynamic values into it! Variables in the message should be
 *   added by using placeholder strings alongside the variables argument to
 *   declare the value of the placeholders. See t() for documentation on how
 *   $message and $variables interact.
 * @param $variables
 *   (optional) Array of variables to replace in the message on display or
 *   NULL if message is already translated or not possible to translate.
 * @param $severity
 *   (optional) The severity of the message; one of the following values as
 *   defined in @link http://www.faqs.org/rfcs/rfc3164.html RFC 3164: @endlink
 *   - Drupal.settings.watchdog.WATCHDOG_EMERGENCY: Emergency, system is unusable.
 *   - Drupal.settings.watchdog.WATCHDOG_ALERT: Alert, action must be taken immediately.
 *   - Drupal.settings.watchdog.WATCHDOG_CRITICAL: Critical conditions.
 *   - Drupal.settings.watchdog.WATCHDOG_ERROR: Error conditions.
 *   - Drupal.settings.watchdog.WATCHDOG_WARNING: Warning conditions.
 *   - Drupal.settings.watchdog.WATCHDOG_NOTICE: (default) Normal but significant conditions.
 *   - Drupal.settings.watchdog.WATCHDOG_INFO: Informational messages.
 *   - Drupal.settings.watchdog.WATCHDOG_DEBUG: Debug-level messages.
 * @param $link
 *   (optional) A link to associate with the message.
 * @param string callback_success
 *   (optional) The name of the function to call if successfull.
 * @param string callback_failure
 *   (optional) The name of the function to call if fail.
 *
 * @see watchdog_severity_levels()
 * @see hook_watchdog()
 */
Drupal.watchdog = function (type, message, variables, severity, link, callback_success, callback_failure) {
  return javascript_drupal_extension_ajax_call('watchdog', { 'type': type, 'message': message, 'variables': variables, 'severity': severity, 'link': link }, callback_success, callback_failure)
}

/**
 * The actual AJAX-call to codebehind.
 *
 * @param string function_name
 *   The function to be executed in codebehind.
 * @param string function_params
 *   An array with variables to be sent along as params to the given function.
 *
 * @return string
 *   The output of the called function.
 */
function javascript_drupal_extension_ajax_call(function_name, function_params, callback_success, callback_failure) {
  var output = null;
  var async = false;

  if (typeof callback_success !== 'undefined') {
    callback_success = window[callback_success];

    if (typeof callback_success !== 'function') {
      callback_success = null;
    }
    else {
      async = true;
    }
  }
  else {
    callback_success = null;
  }

  if (typeof callback_failure !== 'undefined') {
    callback_failure = window[callback_failure];

    if (typeof callback_failure !== 'function') {
      callback_failure = null;
    }
  }
  else {
    callback_failure = null;
  }

  function_params = (typeof function_params !== 'undefined' ? function_params : array());

  jQuery.ajax({
    type: 'POST',
    url: Drupal.settings.basePath + Drupal.settings.javascript_drupal_extension.ajax_callback_url + '/' + function_name.toLowerCase().replace(/_/g, '-'),
    async: async,
    data: function_params,
    success: function (response) {
      if (callback_success !== null) {
        callback_success(response);
      }
      else {
        output = response;
      }
    },
    error: function (jqXHR, textStatus, errorThrown) {
      if (callback_failure !== null) {
        callback_failure(jqXHR, textStatus, errorThrown)
      }
      else {
        throw new Error(errorThrown);
      }
    }
  });

  if (async) {
    return true;
  }
  else {
    return output;
  }
}

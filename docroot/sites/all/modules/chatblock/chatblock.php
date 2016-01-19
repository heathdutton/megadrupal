<?php

/**
 * @file
 * Poll callback handler for chatblock.
 *
 * Serves as a callback handler for uncritical chatblock
 * operations. Reduces server load as it catches most
 * JS initiated requests and only bootstraps Drupal as
 * far as really necessary.
 */

/**
 * Used to avoid a misleading boolean function return value.
 */
define('CHATBLOCK_CACHE_READ_ERROR', -1);

/**
 * Some pre-checks. No service unless passed!
 */
if (
  @$_POST['maxId'] === NULL
  ||
  @$_POST['mp'] === NULL
  ||
  @$_POST['cid'] === NULL
  ||
  @$_POST['session'] === NULL
  ||
  $_POST['maxId'] < 0
  ||
  (int) $_POST['maxId'] != $_POST['maxId']
  ||

  /**
   * Some more paranoia.
   * Paths would never match one of these patterns unless they have been
   * manipulated.
   */

  preg_match('#(^[\\/]|\.+[\\/]*|:|[\\/]{2,})#', $_POST['mp'])

  ||

  // Overflow protection.

  // Bigint unsigned would never exceed this size.
  strlen($_POST['maxId']) > 20
  ||
  strlen($_POST['mp']) > 384
  ||
  strlen($_POST['cid']) > 256

  // Session name is alwas 36 byte in D6 (md5 + "SESS").
  // @todo: Probably overblocking? Losen?
  ||
  strlen($_POST['session']) != 36
) {
  exit;
}

$_chatblock_cache_path = $_POST['mp'] . '/cache';

if (is_dir($_chatblock_cache_path)) {
  $client_max_id = (int) $_POST['maxId'];
  require_once($_chatblock_cache_path . '/apc.inc');

  define('CHATBLOCK_CACHE_ID', preg_replace('/[^a-zA-Z0-9]/', '', $_POST['cid']));

  $server_max_id = chatblock_cache_get('max_id', TRUE);

  if ($server_max_id !== FALSE && $server_max_id > $client_max_id) {
    // The client is not up-to-date.

    if (chatblock_cache_get('mc_last_built')) {
      // Check if user is authenticated (valid token can only exist if user
      // has recently successfully authenticated vs. Drupal).
      if (
        @$_POST['token'] != ''
        &&
        @$_POST['tokentime'] != ''
        &&
        intval(@$_POST['tokentime']) > time() - CHATBLOCK_CACHE_TOKEN_TTL
      ) {

        // Check for server token.
        $server_token = chatblock_cache_get('servertoken');

        // Validate client token cs. server token.
        if ($server_token !== FALSE) {
          $client_token = md5($_COOKIE[$_POST['session']] + $server_token);
          if ($client_token == $_POST['token']) {
            $authenticated = TRUE;
          }
          else {
            // Server token times out regularly. As client can not know, when,
            // the challenge "how old is your token" is an additional barrier.
            // Revalidation will only happen if this test is passed.
            $server_tokentime = chatblock_cache_get('servertokentime');
            if (
              $server_tokentime === FALSE
              ||
              $server_tokentime > intval(abs($_POST['tokentime']))
            ) {
              // Server token seems to have timed out. Revalidation is needed.
              // Indicate that we need a new one (by unsetting) and use Drupal.
              unset($_POST['token']);
              $use_drupal = TRUE;
            }
            else {
              // Remember that we tell the client he is not welcome.
              // At least those who do not manipulate our script will accept.
              $not_welcome = TRUE;
            }
          }
        }
        else {
          // Server token seems to have timed out. Revalidation is needed.
          // Indicate that we need a new one (by unsetting) and use Drupal.
          unset($_POST['token']);
          $use_drupal = TRUE;
        }

        if ($authenticated) {
          // If there are cached messages.
          $use_drupal = chatblock_callback_process_cache($client_max_id) === CHATBLOCK_CACHE_READ_ERROR;
        }
      }
    }
    else {
      // No RAM cache available. Use Drupal instead.
      $use_drupal = TRUE;
    }
  }

  // We will only get here unless the above was run successfully
  // as it directly prints the json result and exits on success.
  if ($server_max_id === FALSE || @$use_drupal) {
    // The client is not up-to-date or no cache is available.
    // Prepare to run Drupal.
    $_POST['q'] = $_GET['q'] = 'js/chatblock/view';

    if (file_exists('./js.php')) {
      // Try with JS callback handler first.
      require_once './js.php';
    }
    else {
      // Start Drupal the standard way.
      require_once './index.php';
    }
  }
  else {
    // No further action is necessary. Send back an empty json object
    // to prevent client errors.
    header('Content-type: application/json');
    print @$not_welcome ? '{}' : '{"ok":true}';
  }
}

/**
 * Read RAM cache and, on success, print results directly to the client.
 *
 * @param $max_id
 *   Client's last received message ID.
 *
 * @return int
 *   Will return CHATBLOCK_CACHE_READ_ERROR in case of cache flaws.
 */
function chatblock_callback_process_cache($max_id) {
  // Inititalize.
  $last_timestamp = 0;

  // Never start from below the oldest cached message.
  $min_id = (int) chatblock_cache_get('min_id');
  // But also not from lower than one after the client's max.
  $start = max($min_id, $max_id + 1);
  $last = (int) chatblock_cache_get('max_id');
  if ($last < $start) {
    // This may happen if, whysoever, max_id is NULL.
    // (Which, of course, must never happen.)
    $last = $start;
  }
  if ($start > 0) {
    $cache_success = TRUE;
    $messages_cached = array();
    for ($i = $start; $i <= $last; $i++) {
      $row = chatblock_cache_get('msg_' . $i);
      if ($row !== FALSE) {
        $messages_cached[] = $row;

        // Remember latest timestamp.
        if ($i == $last) {
          $last_timestamp = $row->t;
        }
      }
      elseif (!empty($messages_cached)) {
        // If a message is not in the cache while within the
        // current id limits, this would indicate a cache problem
        // and the need to force a rebuild.
        $cache_success = FALSE;
        break;
      }
      else {
        // min_id cannot be smaller than the next higher id.
        // May happen by ttl timeout of messages.
        $new_start = $i+1;
      }
    }
  }
  if (isset($new_start) && $new_start > $min_id) {
    chatblock_cache_set('min_id', $new_start);
  }
  if ($cache_success) {
    $result = array(
      'maxId'    => $last,
      'lastTimestamp' => $last_timestamp,
      'messages' => $messages_cached,
      'ok' => TRUE,
    );
    header('Content-type: application/json');
    print chatblock_to_js($result);
    exit;
  }
  // If we get here, something went wrong.
  return CHATBLOCK_CACHE_READ_ERROR;
}

/**
 * Clones drupal_json_encode from common.inc.
 *
 * PHP's json_encode() was not available before 5.2.0.
 * Drupal 7 uses the version from PHP 5.3 or clones it.
 *
 * @param $var
 *   The data to encode.
 *
 * @return string
 *   A json string representation of $var.
 */
function chatblock_to_js($var) {

  if (version_compare(PHP_VERSION, '5.3.0', '>=')) {
    // Encode <, >, ', &, and " using the json_encode() options parameter.
    return json_encode($var, JSON_HEX_TAG | JSON_HEX_APOS | JSON_HEX_AMP | JSON_HEX_QUOT);
  }

  // json_encode() escapes <, >, ', &, and " using its options parameter, but
  // does not support this parameter prior to PHP 5.3.0.  Use a helper instead.
  include_once 'includes/json-encode.inc';
  return drupal_json_encode_helper($var);
}

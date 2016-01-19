<?php

/**
 * Alters the IRC message to be sent in response to an URL.
 *
 * @param string &$message
 *   The message that will be output. To not output any message, set this
 *   empty value or FALSE.
 * @param object $request
 *   The response from drupal_http_request() for the URL. The URL is included
 *   as $request->url.
 * @param object $data
 *   The IRC object from bot module.
 */
function hook_bot_url_message(&$message, $request, $data) {

}

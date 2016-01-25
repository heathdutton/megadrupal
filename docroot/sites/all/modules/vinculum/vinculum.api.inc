<?php
/**
 * @file
 * Hooks provided by the vinculum module.
 */

/**
 * @defgroup vinculum_api_hooks Vinculum API Hooks
 * @{
 * Functions to integrate with the Vinculum module.
 * @}
 */

/**
 * @addtogroup hooks
 * @{
 */

/**
 * Declare a vinculum handler.
 *
 * This is a hook used by vinculum handler modules. It allows a module to
 * define a handler which implements a vinculum protocol.
 * Modules implementing this should also implement hook_vinculum_send().
 *
 * @return
 *   An array containing the key 'protocol' which provides the human-readable
 *   name of the protocol implemented.  A vinculum module can only implement a
 *   single handler.
 *
 * @ingroup vinculum_api_hooks
 */
function hook_vinculum_handler() {
  return array(
    'protocol' => t('Pingback'),
  );
}

/**
 * Attempt to send a vinculum to a single URL for a single node.
 * Modules implementing this must also implement hook_vinculum_handler().
 *
 * @param String $target
 *    The URL of the external site. You can assume that the link will always be
 *    a FQDN.
 * @param String $source
 *    The canonical URL of the local node.
 * @param Object $node
 *    The node that links to the URL provided.
 *
 * @return Boolean
 * TRUE if the handler was successful in reporting the vinculum.
 */
function hook_vinculum_send($target, $source, $node) {

}

/**
 * Extend the vinculum module to check for external links in fields other than
 * the body field.
 */
function hook_vinculum_get_external_links($node) {

}

/**
 * Alter the list of external-links to be contacted by a vinculum handler.
 *
 * @param Array $links
 * An array of vinculum records: each record is an object with the properties:
 * - source
 * - target
 */
function hook_vinculum_link_send_alter(&$links) {
  // Don't attempt vinculums on example.com domains.
  foreach ($links as $key => $record) {
    if (preg_match('#\.example\.com$#', $record->target)) {
      unset($links[$key]);
    }
  }
}

/**
 * Actions to be fired when a vinculum is received.
 */
function hook_vinculum_received_action($record) {

}

/**
 * @} End of "addtogroup hooks".
 */

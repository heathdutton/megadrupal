<?php
/**
 * @file
 * API information for Seen.
 *
 * @copyright Copyright(c) 2014 Previous Next Pty Ltd
 * @license GPL v2 http://www.fsf.org/licensing/licenses/gpl.html
 * @author Chris Skene chris at previousnext dot com dot au
 */

/**
 * Alter the results for who has seen a node.
 *
 * This is called in seen_get_views(), which returns a list of who has
 * seen a node and when.
 *
 * @param array $results
 *   An array of view records for the node.
 * @param object $node
 *   The node results are retrieved for.
 */
function hook_seen_views_alter(&$results, $node) {

}

/**
 * Alter the results for who has seen a node.
 *
 * This is called after results have been sorted by user.
 *
 * @param array $results
 *   An array of view records for the node, sorted by user.
 * @param string $nid
 *   The node results are retrieved for.
 */
function hook_seen_sorted_views_alter(&$results, $nid) {

}

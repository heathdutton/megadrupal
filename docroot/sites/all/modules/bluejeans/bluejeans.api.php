<?php

/**
 * @file
 * API documentation for Blue Jeans module.
 */

/**
 * Notify modules of changes to a Blue Jeans conference.
 *
 * @param string $op
 *   A string representing the operation performed on the conference,
 *   one of "insert", "update", "delete".
 * @param object $conference
 *   The conference objet after modifications:
 *   - meeting_id: The unique conference identifier
 *   - nid: The node nid associated with the conference
 *   - title: The conference title
 *   - description: The conference description
 *   - start: Conference UNIX start time
 *   - end: Conference UNIX end time
 *   - metadata: JSON-encoded conference metadata as returned by Blue Jeans API
 *
 * @see bluejeans.module
 */
function hook_bluejeans_conference($op, $conference) {
}

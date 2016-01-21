<?php

/**
 * @file
 * Hooks provided by the merge_synonyms module.
 */

/**
 * Allow modules to act on term synonym merging.
 *
 * @param int $vid
 *   Vocabulary id.
 * @param int $tid
 *   Term id of term that synonym to be merged.
 * @param int $tid2
 *   Term id of term that is to be deleted.
 */
function hook_merge_synonyms_merge($vid, $tid, $tid2) {
  // Add logic to replace $tid2 reference to $tid from some place.
}

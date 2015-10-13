<?php

/**
 * Access function allow restrict access to reviews add form.
 *
 * @param string $type
 *  Entity type.
 * @param int $eid
 *  Entity id.
 *
 * @return bool
 *  TRUE if access is allowed, FALSE if access is denied.
 */
function hook_entity_review_add_access($type, $eid) {
  if ($type == 'taxonomy_term') {
    return TRUE;
  }
  else {
    return FALSE;
  }
}

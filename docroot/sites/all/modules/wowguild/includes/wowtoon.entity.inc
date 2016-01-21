<?php

/**
 * @file
 * Avatar editing UI.
 *
 * We make very little use of the EntityAPI interface for this - preferring instead to use
 * views. That offers more flexibility to change a UI that will, more often than not,
 * be end-user facing.
 */





function wowtoon_load_by_realm_name($fullrealm, $name) {
  $toon_key = wowguild_getkey($fullrealm, $name);
  $query = db_select('wowtoon_cached_data', 'w');
  $query->fields('w', array('toon_key'));
  $query->leftJoin('wowtoon_owners', 'o', 'o.toon_key = w.toon_key');
  $query->fields('o', array('aid'));
  $query->condition('w.toon_key', $toon_key);
  $result = $query->execute()->fetch();
  if (!empty($result->aid)) {
    return avatar_load($result->aid);
  }
  else {
    return FALSE;
  }
}



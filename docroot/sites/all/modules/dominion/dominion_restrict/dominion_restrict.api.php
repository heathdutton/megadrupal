<?php

/**
 * Check if a nodetype may be posted. Usually checked with domain id for current domain.
 *
 * @param string $type
 */
function hook_dominion_restrict_creation($type, $account){
  if ($type == 'forum'){
    // Restrict this nodetype, means that it cannot be posted
    return TRUE;
  }
  
  // Do not affect behaviour for this nodetype
  return FALSE;
}

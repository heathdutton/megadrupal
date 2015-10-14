<?php

/**
 * Main info hook that the predelete module uses to determine if the given
 * node could be deleted. 
 *
 * @param object $node
 *  The node object that is about to be deleted
 *
 * @return array
 *   An array of components, keyed by the component name. Each component can
 *   define several keys:
 *
 *   'result': This is a boolean value and controlls if the given node id may
 *     be deleted or not.
 *
 *   'reason': If the deletion is not allowed, the implementing module may
 *     provide a reason. The reason is presented as a message to the user only
 *     if the deletion is not allowed.
 */
function hook_predelete_node($node) {
  // Do something with the node object.
  return array(
    'result' => TRUE,
    'reason' => 'deletion is safe',
  );
}

/**
 * @}
 */

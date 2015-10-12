<?php

/**
 * Modules should implement this, if there is need to influence
 * DoubleRecall hiding decision.
 *
 * TRUE should be returned from this hook, if DoubleRecall should hide content
 * on this page. At least one module must return TRUE if DoubleRecall should be
 * used.
 */
function hook_doublerecall_should_hide() {
  $node = menu_get_object();
  return !empty($node) && $node->type == 'my_node_type';
}

/**
 * Alter decision about DoubleRecall initialization. Alters result of
 * hook_doublerecall_should_hide().
 */
function hook_doublerecall_should_hide_alter(&$should_hide) {

}
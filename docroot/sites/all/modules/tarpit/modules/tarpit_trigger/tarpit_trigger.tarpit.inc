<?php

/**
 * Implements hook_tarpit_view().
 */
function tarpit_trigger_tarpit_view() {
  $actions = (array) trigger_get_assigned_actions('tarpit_view');
  actions_do(array_keys($actions), NULL, array());
}

/**
 * Implements hook_tarpit_reaction().
 */
function tarpit_trigger_tarpit_reaction() {
  $actions = (array) trigger_get_assigned_actions('tarpit_reaction');
  actions_do(array_keys($actions), NULL, array());
}

<?php

/**
 * Implements hook_tarpit_view().
 */
function tarpit_rules_tarpit_view() {
  rules_invoke_event('tarpit_view');
}

/**
 * Implements hook_tarpit_reaction().
 */
function tarpit_rules_tarpit_reaction() {
  rules_invoke_event('tarpit_reaction');
}

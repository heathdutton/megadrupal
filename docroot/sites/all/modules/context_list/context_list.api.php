<?php

/**
 * Register a condition display class
 * @return array An array of display PHP classes that are used to display conditions
 */
function hook_context_list_register_condition_display() {
  return array(
    'all' => 'ContextListConditionDisplay',
    'defaultcontent' => 'ContextListConditionDisplay_defaultcontent',
    'path' => 'ContextListConditionDisplay_path',
  );
}

/**
 * Register a reaction display class
 * @return array An array of display PHP classes that are used to display conditions
 */
function hook_context_list_register_reaction_display() {
  return array(
    'all' => 'ContextListReactionDisplay',
    'block' => 'ContextListReactionDisplay_block',
  );
}
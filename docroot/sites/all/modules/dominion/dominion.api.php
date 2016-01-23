<?php

/**
 * @file
 * Dominion hooks file.
 */

/**
 * Alter the loaded subsite.
 * 
 * @param object $subsite
 */
function hook_dominion_load(&$subsite) {
  $subsite->my_var = 234;
}

/**
 * Notify other modules that we have created a new subsite.
 *
 * @param int $domain_id
 * @param $form_values
 */
function hook_dominion_insert($domain_id, $form_values = array()) {
  db_insert('mytable')
    ->fields(array(
      'domain_id' => $domain_id,
      'foo' => $form_values['bar'],
    ))
    ->execute();
}

/**
 * Notify other modules that we have updated a subsite.
 *
 * @param int $domain_id
 * @param array $form_values
 */
function hook_dominion_update($domain_id, $form_values = array()) {
  db_update('mytable')
    ->fields(array(
      'foo' => $form_values['bar'],
    ))
    ->condition('domain_id', $domain_id)
    ->execute();
}

/**
 * Notify other modules that we have deleted a subsite.
 *
 * @param object $subsite
 */
function hook_dominion_delete($subsite) {
  db_delete('mytable')
    ->condition('domain_id', $subsite->domain_id)
    ->execute();
}

/**
 * Hook for defining functions available in dominion subsites.
 *
 * @return array
 */
function hook_dominion_functions() {
  return array(
    'forum' => t('Forum'),
    'blog' => t('Blog'),
  );
}

/**
 * Alter the loaded blueprint.
 * 
 * @param object $blueprint
 */
function hook_dominion_blueprint_load(&$blueprint) {
  $blueprint->my_var = 234;
}

/**
 * Notify other modules that we have created a new blueprint.
 *
 * @param string $blueprint
 * @param $form_values
 */
function hook_dominion_blueprint_insert($blueprint, $form_values = array()) {
  db_insert('mytable')
    ->fields(array(
      'blueprint' => $blueprint,
      'foo' => $form_values['bar'],
    ))
    ->execute();
}

/**
 * Notify other modules that we have updated a blueprint.
 *
 * @param string $blueprint
 * @param array $form_values
 */
function hook_dominion_blueprint_update($blueprint, $form_values = array()) {
  db_update('mytable')
    ->fields(array(
      'foo' => $form_values['bar'],
    ))
    ->condition('blueprint', $blueprint)
    ->execute();
}

/**
 * Notify other modules that we have deleted a blueprint.
 *
 * @param object $subsite
 */
function hook_dominion_blueprint_delete($blueprint) {
  db_delete('mytable')
    ->condition('blueprint', $subsite->blueprint)
    ->execute();
}

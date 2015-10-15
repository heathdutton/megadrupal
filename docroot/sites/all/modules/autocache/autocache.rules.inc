<?php

/**
 * Implements hook_rules_action_info().
 */
function autocache_rules_action_info() {

  $items = array();

  $defaults = array(
    'group' => t('Autocache'),
    'access callback' => 'autocache_access_callback',
  );

  // Emit energy
  $items['autocache_flush'] = $defaults + array(
    'label' => t('Flush caches related to given entity'),
    'base' => 'autocache_rules_flush',
    'parameter' => array(
      'data' => array(
        'type' => 'entity',
        'label' => t('Entity'),
        'description' => t('The entity'),
        'restriction' => 'selector',
        'wrapped' => TRUE,
        'allow null' => TRUE,
      ),
    ),
  );

  return $items;
}


/**
 * Access callback.
 */
function autocache_access_callback() {
  return TRUE;
}

/**
 * Autocache flusher action
 */
function autocache_rules_flush($entity) {
  _autocache_act_on_update($entity->raw(), $entity->type());
}

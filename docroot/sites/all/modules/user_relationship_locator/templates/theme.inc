<?php
/**
 * @file
 * Pages callbacks.
 */

/**
 * Preprocessor for user_relationship_locator_default_block.
 */
function user_relationship_locator_preprocess_user_relationship_locator_default_block(&$vars) {
  $vars['items'] = array();

  foreach ($vars['matches'] as $requestee) {
    $vars['items'][] = theme('user_relationship_locator_default_block_item', array(
      'requester' => $vars['requester'],
      'requestee' => $requestee,
      'relationship' => $vars['relationship'],
    ));
  }

  $vars['load_buttons'] = theme('user_relationship_locator_load_buttons');
}

/**
 * Preprocessor for user_relationship_locator_default_block_item.
 */
function user_relationship_locator_preprocess_user_relationship_locator_default_block_item(&$vars) {
  $requestee = $vars['requestee'];

  $vars['user_link'] = l($requestee->name, 'user/' . $requestee->uid, array(
    'attributes' => array('class' => array('user-link')),
  ));

  $vars['relationship_link'] = theme('user_relationships_request_relationship_direct_link', array(
    'relate_to' => $requestee,
    'relationship_type' => $vars['relationship']
  ));

  $vars['ignore_link'] = l(t('Ignore'), "relationship_locator/$requestee->uid/ignore", array(
    'title' => array('title' => t('Ignore')),
    'query' => drupal_get_destination(),
  ));
}

/**
 * Preprocessor for user_relationship_locator_load_buttons.
 */
function user_relationship_locator_preprocess_user_relationship_locator_load_buttons(&$vars) {
  $buttons = array();

  foreach (module_implements('urlocator_load_buttons') as $module) {
    $buttons += module_invoke($module, 'urlocator_load_buttons');
  }

  drupal_alter('urlocator_load_buttons', $buttons);

  $vars['load_buttons'] = $buttons;
}

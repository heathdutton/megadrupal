<?php

/**
 * @file
 * Rules actions for the Word Link module.
 */

/**
 * Implements hook_rules_action_info().
 */
function word_link_rules_action_info() {
  return array(
    'add_word_link' => array(
      'label' => t('Create Word Link'),
      'group' => t('Word Link'),
      'parameter' => array(
        'text' => array(
          'type' => 'text',
          'label' => t('Word/Phrase'),
          'description' => t('The word or phrase you wish to convert to a link. This field is case sensitive.'),
        ),
        'weight' => array(
          'type' => 'integer',
          'label' => t('Weight'),
          'description' => t('Weight of the word. Lighter weights are higher up, heavier weights go down.'),
          'default value' => 0,
        ),
        'case_sensitive' => array(
          'type' => 'boolean',
          'label' => t('Case Sensitivity'),
          'description' => t('By default Word Link are case sensitive. Uncheck this checkbox if you want this particular Word Link to be case insensitive.'),
          'optional' => TRUE,
        ),
        'url' => array(
          'type' => 'text',
          'label' => t('URL'),
          'description' => t('The URL of the page to link to. External links must start with %http or %https and will be open in new window.', array('%http' => 'http://', '%https' => 'https://')),
        ),
        'url_label' => array(
          'type' => 'text',
          'label' => t('URL label'),
          'description' => t('label for the above URL. It will be embedded in the created link and appear as a tooltip when hovering the mouse over the link.'),
          'optional' => TRUE,
        ),
        'class' => array(
          'type' => 'text',
          'label' => t('Class'),
          'description' => t('Use this to add a class for the link. Default value is "word-link".'),
          'optional' => TRUE,
          'default value' => 'word-link',
        ),
        'rel' => array(
          'type' => 'text',
          'label' => t('Relationship'),
          'description' => t('Use this to add a rel attribute to the link.'),
          'optional' => TRUE,
        ),
        'visibility' => array(
          'type' => 'integer',
          'label' => t('Show links on specific pages'),
          'options list' => 'word_link_visibility_list',
          'default value' => 0,
        ),
        'except' => array(
          'type' => 'text',
          'label' => t('Pages'),
          'description' => t('Specify pages by using their paths. Enter one path per line. E.g. node/1.'),
          'optional' => TRUE,
        ),
      ),
      'base' => 'word_link_action_add_word_link',
      'access callback' => 'word_link_action_add_word_link_access',
    ),
  );
}

/**
 * Callback defining an options list for visibility properties.
 */
function word_link_visibility_list() {
  return array(
    t('All pages except those listed'),
    t('Only the listed pages'),
  );
}

/**
 * Add Word Link action.
 */
function word_link_action_add_word_link($text, $weight, $case_sensitive, $url, $url_label, $class, $rel, $visibility, $except) {
  $values = array(
    'text' => trim($text),
    'case_sensitive' => $case_sensitive,
    'url' => trim($url),
    'url_title' => trim($url_label),
    'class' => trim($class),
    'visibility' => $visibility,
    'except' => trim($except),
    'weight' => $weight,
    'rel' => $rel,
  );
  word_link_save($values);
  cache_clear_all('*', 'cache_filter', TRUE);
  field_cache_clear();
}

/**
 * Add Word Link access callback.
 */
function word_link_action_add_word_link_access() {
  return user_access('create word link');
}

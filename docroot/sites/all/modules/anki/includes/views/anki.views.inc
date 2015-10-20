<?php
/**
 * @file
 * Implementations of Views hooks for the Anki module.
 */

/**
 * Implements hook_views_plugins().
 */
function anki_views_plugins() {
  return array(
    'query' => array(
      'anki_query_cards' => array(
        'title' => t('Anki cards'),
        'help' => t('Query cards in an Anki collection'),
        'handler' => 'anki_views_plugin_query_cards',
      ),
    ),
  );
}

/**
 * Implements hook_views_data().
 */
function anki_views_data() {
  $data = array();

  $data['anki_cards'] = array(
    'table' => array(
      'group' => t('Anki'),
      'base' => array(
        'field' => 'id',
        'title' => t('Anki cards'),
        'query class' => 'anki_query_cards',
      ),
    ),

    'collection' => array(
      'title' => t('Collection'),
      'argument' => array(
        'handler' => 'anki_views_handler_argument_collection',
      ),
    ),

    'id' => array(
      'title' => t('Card ID'),
      'field' => array(
        'handler' => 'views_handler_field',
      ),
    ),

    'question' => array(
      'title' => t('Question'),
      'field' => array(
        'handler' => 'views_handler_field',
      ),
    ),

    'answer' => array(
      'title' => t('Answer'),
      'field' => array(
        'handler' => 'views_handler_field',
      ),
    ),
  );

  return $data;
}

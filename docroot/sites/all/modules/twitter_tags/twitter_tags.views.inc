<?php

/**
 * @file
 * Twitter Tags Views hooks.
 */

/**
 * Implements hook_views_data().
 */
function twitter_tags_views_data() {
  // Basic table information.
  $data['twitter_tags']['table']['group'] = t('Twitter');

  // Provide the implicit join.
  $data['twitter_tags']['table']['join']['twitter'] = array(
    'left_field' => 'twitter_id',
    'field' => 'twitter_id',
  );

  // Define our fields.
  $data['twitter_tags']['tag'] = array(
    'title' => t('Tag'),
    'help' => t('Hashtag found within a tweet.'),
    'field' => array(
      'handler' => 'views_handler_field',
      'click sortable' => TRUE,
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_string',
    ),
    'argument' => array(
      'handler' => 'views_handler_argument_string',
    ),
  );

  // If we're running taxonomy module, provide a relation to its data.
  if (module_exists('taxonomy')) {
    $data['twitter_tags']['tag']['relationship'] = array(
      'title' => t('Term'),
      'help' => t('Relate hashtag to a taxonomy term.'),
      'handler' => 'views_handler_relationship',
      'base' => 'taxonomy_term_data',
      'base field' => 'name',
      'label' => t('term'),
    );
  }

  return $data;
}

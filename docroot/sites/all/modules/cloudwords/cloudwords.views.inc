<?php

/**
 * @file
 * Cloudwords views integration.
 */

/**
 * Implements hook_views_data_alter().
 */
function cloudwords_views_data_alter(&$info) {
  $info['cloudwords_translatable']['label']['field']['handler'] = 'cloudwords_views_handler_label';

  $info['cloudwords_translatable']['content_type'] = array(
    'title' => t('node'),
    'help' => t('The content type (for example, "blog entry", "forum post", "story", etc).'),
    'real field' => 'objectid',
    'relationship' => array(
      'title' => t('Content type'),
      'help' => t('Relate a translatable to its content type.'),
      'handler' => 'cloudwords_handler_relationship',
      'base' => 'node',
      'base field' => 'nid',
      'field' => 'objectid',
      'label' => t('content type'),
      'filter key' => 'type',
      'filter value' => 'node_content',
      'filter remove' => 'type_1',
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_node_type',
    ),
  );

  $info['cloudwords_translatable']['vocabulary'] = array(
    'title' => t('Vocabulary'),
    'help' => t('The vocabulary for this taxonomy term.'),
    'real field' => 'objectid',
    'relationship' => array(
      'title' => t('Vocabulary'),
      'help' => t('Relate a translatable to its vocabulary.'),
      'handler' => 'cloudwords_handler_relationship',
      'base' => 'taxonomy_term_data',
      'base field' => 'tid',
      'field' => 'objectid',
      'label' => t('Vocabulary'),
      'filter key' => 'type',
      'filter value' => 'taxonomy_term',
      'filter remove' => 'vocabulary',
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_vocabulary_vid',
    ),
  );

  $info['cloudwords_translatable']['uid']['filter']['handler'] = 'cloudwords_handler_filter_project_user';

  $info['cloudwords_translatable']['in_translation'] = array(
    'help' => t('Whether this project is currently in translation.'),
    'title' => t('In translation'),
    'real field' => 'status',
    'filter' => array(
      'handler' => 'views_handler_filter_numeric',
    ),
  );

}

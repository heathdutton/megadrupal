<?php

/**
 * @file
 * Provide views data and handlers for word_link.module.
 */

/**
 * Implements hook_views_data().
 */
function word_link_views_data() {
  $data['word_link']['table']['group'] = t('Word Link');

  $data['word_link']['table']['base'] = array(
    'field' => 'id',
    'title' => t('Word Link'),
    'help' => t('Word Link'),
  );

  $data['word_link']['id'] = array(
    'title' => t('Word ID'),
    'help' => t('ID of the word'),
    'field' => array(
      'handler' => 'views_handler_field_numeric',
      'click sortable' => TRUE,
    ),
    'argument' => array(
      'handler' => 'views_handler_argument_numeric',
      'numeric' => TRUE,
      'name field' => 'id',
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_numeric',
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
  );

  $data['word_link']['text'] = array(
    'title' => t('Word'),
    'help' => t('Text of the word.'),
    'field' => array(
      'handler' => 'views_handler_field_markup',
      'format' => filter_fallback_format(),
      'click sortable' => TRUE,
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_string',
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
  );

  $data['word_link']['case_sensitive'] = array(
    'title' => t('Case sensitive'),
    'help' => t('Case sensitivity of the word link.'),
    'field' => array(
      'handler' => 'views_handler_field_boolean',
      'click sortable' => TRUE,
      'output formats' => array(
        'yes-no' => array(t('Yes'), t('No')),
      ),
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_boolean_operator',
      'label' => t('Case sensitive'),
      'type' => 'yes-no',
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
  );

  $data['word_link']['url'] = array(
    'title' => t('URL'),
    'help' => t('URL of the word link.'),
    'field' => array(
      'handler' => 'views_handler_field_url',
      'click sortable' => TRUE,
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_string',
    ),
  );

  $data['word_link']['url_title'] = array(
    'title' => t('URL title'),
    'help' => t('Title of the word link.'),
    'field' => array(
      'handler' => 'views_handler_field_markup',
      'format' => filter_fallback_format(),
      'click sortable' => TRUE,
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_string',
    ),
  );

  $data['word_link']['class'] = array(
    'title' => t('Class'),
    'help' => t('Class of the word link.'),
    'field' => array(
      'handler' => 'views_handler_field_markup',
      'format' => filter_fallback_format(),
      'click sortable' => TRUE,
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_string',
    ),
  );

  $data['word_link']['visibility'] = array(
    'field' => array(
      'title' => t('Visibility'),
      'help' => t('Visibility of the word link.'),
      'handler' => 'word_link_handler_word_link_visibility',
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
  );

  $data['word_link']['weight'] = array(
    'title' => t('Weight'),
    'help' => t('Weight of the word link.'),
    'field' => array(
      'handler' => 'views_handler_field_numeric',
      'click sortable' => TRUE,
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_numeric',
    ),
  );

  $data['word_link']['rel'] = array(
    'title' => t('Relationship'),
    'help' => t('Rel attribute of the word link.'),
    'field' => array(
      'handler' => 'views_handler_field_markup',
      'format' => filter_fallback_format(),
      'click sortable' => TRUE,
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_string',
    ),
  );

  $data['word_link']['status'] = array(
    'title' => t('Status'),
    'help' => t('Whether or not the word is enabled.'),
    'field' => array(
      'handler' => 'views_handler_field_boolean',
      'click sortable' => TRUE,
      'output formats' => array(
        'enabled-disabled' => array(t('Enabled'), t('Disabled')),
      ),
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_boolean_operator',
      'label' => t('Status'),
      'type' => 'enabled-disabled',
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
  );

  $data['word_link']['operations'] = array(
    'field' => array(
      'title' => t('Operations'),
      'help' => t('Provide an operations for editing/deleting a word.'),
      'handler' => 'word_link_handler_word_link_operations',
    ),
  );

  return $data;
}

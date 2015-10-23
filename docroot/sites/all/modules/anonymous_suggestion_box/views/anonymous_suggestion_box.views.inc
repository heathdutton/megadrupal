<?php
/**
 * @file
 * File for anonymous_suggestion_box.
 */

/**
 * Implements hook_views_data().
 */
function anonymous_suggestion_box_views_data() {
  $data = array();
  $data['anonymous_suggestion_box']['table']['group'] = t("Suggestions");

  $data['anonymous_suggestion_box']['table']['base'] = array(
    'field' => 'sid',
    'title' => 'Anonymous Suggestion Box Submissions',
    'weight' => 10,
    'access query tag' => 'view anonymous suggestion box',
  );
  $data['anonymous_suggestion_box']['sid'] = array(
    'title' => t('Suggestion ID'),
    'help' => t('ID of user submittion'),
    'field' => array(
      'handler' => 'views_handler_field',
      'click sortable' => TRUE,
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_numeric',
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
  );

  $data['anonymous_suggestion_box']['location'] = array(
    'title' => t('Suggestion Location'),
    'help' => t('Location submitted by user'),
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

  $data['anonymous_suggestion_box']['observation'] = array(
    'title' => t('Suggestion observation'),
    'help' => t('observation submitted by user'),
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

  $data['anonymous_suggestion_box']['suggested'] = array(
    'title' => t('Suggestion text'),
    'help' => t('suggestion submitted by user'),
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

  return $data;
}

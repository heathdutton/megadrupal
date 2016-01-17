<?php

/**
 * @file
 * Views integration for the media_theplatform_mpx module.
 */

/**
 * Implements hook_views_data().
 */
function media_theplatform_mpx_views_data() {
  $data = array();

  $data['mpx_video']['table'] = array(
    'group' => t('thePlatform mpx Video'),
    'join' => array(
      'file_managed' => array(
        'left_field' => 'fid',
        'field' => 'fid',
      ),
    ),
  );

  // @todo Do we even need this? Why not use file_managed.title?
  $data['mpx_video']['title'] = array(
    'title' => t('Title'),
    'help' => t('The title of the video.'),
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

  $data['mpx_video']['description'] = array(
    'title' => t('Description'),
    'help' => t('The description of the video.'),
    'field' => array(
      'handler' => 'views_handler_field',
      'click sortable' => TRUE,
    ),
  );

  $data['mpx_video']['id'] = array(
    'title' => t('ID'),
    'help' => t('The ID of the video.'),
    'field' => array(
      'handler' => 'views_handler_field',
      'click sortable' => TRUE,
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_numeric',
    ),
    'argument' => array(
      'handler' => 'views_handler_argument_numeric',
    ),
  );

  $data['mpx_video']['guid'] = array(
    'title' => t('Reference ID (GUID)'),
    'help' => t('The reference ID of the video.'),
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

  $data['mpx_video']['account'] = array(
    'title' => t('Account'),
    'help' => t('The mpx account associated with the video.'),
    'field' => array(
      'handler' => 'views_handler_field',
      'click sortable' => TRUE,
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_in_operator',
      'options callback' => 'media_theplatform_mpx_get_account_options',
    ),
    'argument' => array(
      'handler' => 'views_handler_argument_string',
    ),
  );

  $data['mpx_video']['created'] = array(
    'title' => t('Created date'),
    'help' => t('The date when the mpx video was initially ingested.'),
    'field' => array(
      'handler' => 'views_handler_field_date',
      'click sortable' => TRUE,
    ),
    'sort' => array(
      'handler' => 'views_handler_sort_date',
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_date',
    ),
  );

  $data['mpx_video']['updated'] = array(
    'title' => t('Updated date'),
    'help' => t('The date when the mpx video was most recently ingested or updated.'),
    'field' => array(
      'handler' => 'views_handler_field_date',
      'click sortable' => TRUE,
    ),
    'sort' => array(
      'handler' => 'views_handler_sort_date',
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_date',
    ),
  );

  // @todo If file_admin is enabled, should this read from file_managed.published instead?
  $data['mpx_video']['status'] = array(
    'title' => t('Published'),
    'help' => t('Whether or not the mpx video is published.'),
    'field' => array(
      'handler' => 'views_handler_field_boolean',
      'click sortable' => TRUE,
      'output formats' => array(
        'published-notpublished' => array(t('Published'), t('Not published')),
      ),
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_boolean_operator',
      'label' => t('Published'),
      'type' => 'yes-no',
      'use equal' => TRUE, // Use status = 1 instead of status <> 0 in WHERE statement
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
  );

  $data['mpx_video']['airdate'] = array(
    'title' => t('Air date'),
    'help' => t('The date when the video originally aired.'),
    'field' => array(
      'handler' => 'views_handler_field_date',
      'click sortable' => TRUE,
    ),
    'sort' => array(
      'handler' => 'views_handler_sort_date',
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_date',
    ),
  );

  $data['mpx_video']['available_date'] = array(
    'title' => t('Available date'),
    'help' => t('The date when the video will be available (sunrise).'),
    'field' => array(
      'handler' => 'views_handler_field_date',
      'click sortable' => TRUE,
    ),
    'sort' => array(
      'handler' => 'views_handler_sort_date',
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_date',
    ),
  );

  $data['mpx_video']['expiration_date'] = array(
    'title' => t('Expiration date'),
    'help' => t('The date when the video will expire (sunset).'),
    'field' => array(
      'handler' => 'views_handler_field_date',
      'click sortable' => TRUE,
    ),
    'sort' => array(
      'handler' => 'views_handler_sort_date',
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_date',
    ),
  );

  $data['mpx_video']['availability'] = array(
    'title' => t('Availability'),
    'help' => t('Whether the mpx video is unavailable, available, or expired.'),
    'field' => array(
      'field' => 'available_date',
      'handler' => 'MpxViewsFieldAvailability',
      // @todo Enable click sorting for availability.
      'click sortable' => FALSE,
      'additional fields' => array('expiration_date'),
    ),
    'filter' => array(
      'field' => 'video_id',
      'handler' => 'MpxViewsFilterAvailability',
    ),
  );

  $data['mpx_video']['author'] = array(
    'title' => t('Author'),
    'help' => t('The author of the video.'),
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

  $data['mpx_video']['keywords'] = array(
    'title' => t('Keywords'),
    'help' => t('The keywords of the video.'),
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

  $data['mpx_video']['copyright'] = array(
    'title' => t('Copyright'),
    'help' => t('The copyright of the video.'),
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

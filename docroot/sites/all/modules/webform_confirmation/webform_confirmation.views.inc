<?php

/**
 * @file
 * Views integration for the webform_confirmation custom module
 */

/**
 * Implements hook_views_data().
 */
function webform_confirmation_views_data() {

  $table = array(
    'webform_confirmation' => array(
      'table' => array(
        'group' => 'Webform Confirmation',
        'base' => array(
          'field' => 'index',
          'title' => 'Webform Confirmation Index',
          'help' => 'Webform Confirmation database table',
        ),
      ),
      // Index in serial form.
      'index' => array(
        'title' => t('Submission Index'),
        'help' => t('Webform Submission Index'),
        'field' => array(
          'click sortable' => TRUE,
        ),
      // Filter handler for filtering records by index.
        'filter' => array(
          'handler' => 'views_handler_filter_numeric',
        ),

        'sort' => array(
          'handler' => 'views_handler_sort',
        ),
      ),
      // Sid of the the webform.
      'sid' => array(
        'title' => t('Submission Id'),
        'help' => t('Webform Submission Id'),
        'field' => array(
          'click sortable' => TRUE,
        ),
        'filter' => array(
          'handler' => 'views_handler_filter_numeric',
        ),
        'sort' => array(
          'handler' => 'views_handler_sort',
        ),
      ),
      // Description of sender's mail.
      'sender_email' => array(
        'title' => t('Senders Mail ID'),
        'help' => t('Mail Id'),
        'field' => array(
          'click sortable' => TRUE,
        ),
        'filter' => array(
          'handler' => 'views_handler_filter_string',
        ),
        'sort' => array(
          'handler' => 'views_handler_sort',
        ),
      ),
      // Description of status.
      'status' => array(
        'title' => t('Status'),
        'help' => t('Status of the form'),
        'field' => array(
          'click sortable' => TRUE,
        ),
        'filter' => array(
          'handler' => 'views_handler_filter_numeric',
        ),
        'sort' => array(
          'handler' => 'views_handler_sort',
        ),
      ),
      // Description of Webform id.
      'webform_id' => array(
        'title' => t('ID of the webform'),
        'help' => t('Webform ID'),
        'field' => array(
          'click sortable' => TRUE,
        ),
        'filter' => array(
          'handler' => 'views_handler_filter_numeric',
        ),
        'sort' => array(
          'handler' => 'views_handler_sort',
        ),
      ),
      // Description of Webform name.
      'webform_name' => array(
        'title' => t('Name of the webform'),
        'help' => t('Webform Name'),
        'field' => array(
          'click sortable' => TRUE,
        ),
        'filter' => array(
          'handler' => 'views_handler_filter_string',
        ),
        'sort' => array(
          'handler' => 'views_handler_sort',
        ),
      ),
      // Description of hash field.
      'hash' => array(
        'title' => t('Hash Code'),
        'help' => t('Hash Code'),
        'field' => array(
          'click sortable' => TRUE,
        ),
        'filter' => array(
          'handler' => 'views_handler_filter_string',
        ),
        'sort' => array(
          'handler' => 'views_handler_sort',
        ),
      ),
    ),
  );
  return $table;
}

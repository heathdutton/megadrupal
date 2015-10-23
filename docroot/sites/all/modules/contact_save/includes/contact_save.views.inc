<?php

/**
 * @file
 * Provide views data and handlers for contact_save.module.
 */

/**
 * Implements hook_views_data().
 */
function contact_save_views_data() {
  $data['contact_save']['table']['group'] = t('Contact message');

  $data['contact_save']['table']['base'] = array(
    'field' => 'id',
    'title' => t('Contact messages'),
    'help' => t('Contact messages'),
  );

  $data['contact_save']['table']['join'] = array(
    'users' => array(
      'left_field' => 'uid',
      'field' => 'uid',
    ),
  );

  $data['contact_save']['table']['join'] = array(
    'contact' => array(
      'left_field' => 'cid',
      'field' => 'cid',
    ),
  );

  $data['contact_save']['id'] = array(
    'title' => t('Message ID'),
    'help' => t('Contact message ID'),
    'field' => array(
      'handler' => 'views_handler_field_numeric',
      'click sortable' => TRUE,
    ),
    'argument' => array(
      'handler' => 'views_handler_argument_numeric',
      'numeric' => TRUE,
      'name field' => 'cid',
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_numeric',
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
  );

  $data['contact_save']['cid'] = array(
    'title' => t('Category ID'),
    'help' => t('Contact category ID'),
    'field' => array(
      'handler' => 'views_handler_field_numeric',
      'click sortable' => TRUE,
    ),
    'argument' => array(
      'handler' => 'views_handler_argument_numeric',
      'numeric' => TRUE,
      'name field' => 'cid',
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_numeric',
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
    'relationship' => array(
      'title' => t('Contact category'),
      'help' => t('Relate message to a category.'),
      'handler' => 'views_handler_relationship',
      'base' => 'contact',
      'base field' => 'cid',
      'field' => 'cid',
      'label' => t('message_category'),
    ),
  );

  $data['contact_save']['uid'] = array(
    'title' => t('Uid'),
    'help' => t('User ID'),
    'field' => array(
      'handler' => 'views_handler_field_user',
      'click sortable' => TRUE,
    ),
    'argument' => array(
      'handler' => 'views_handler_argument_user_uid',
      'name field' => 'name',
    ),
    'filter' => array(
      'title' => t('Name'),
      'handler' => 'views_handler_filter_user_name',
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
    'relationship' => array(
      'title' => t('Author'),
      'help' => t('Relate message to the user who created it.'),
      'handler' => 'views_handler_relationship',
      'base' => 'users',
      'field' => 'uid',
      'label' => t('author'),
    ),
  );

  $data['contact_save']['mail'] = array(
    'title' => t('E-mail'),
    'help' => t('Email address for a given user. This field is normally not shown to users, so be cautious when using it.'),
    'field' => array(
      'handler' => 'views_handler_field_user_mail',
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

  $data['contact_save']['subject'] = array(
    'title' => t('Subject'),
    'help' => t('Message subject.'),
    'field' => array(
      'handler' => 'views_handler_field_markup',
      'format' => filter_fallback_format(),
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_string',
    ),
  );

  $data['contact_save']['message'] = array(
    'title' => t('Body'),
    'help' => t('Message body.'),
    'field' => array(
      'handler' => 'views_handler_field_markup',
      'format' => filter_fallback_format(),
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_string',
    ),
  );

  $data['contact_save']['created'] = array(
    'title' => t('Created'),
    'help' => t('Time when message was submitted.'),
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

  $data['contact_save']['read'] = array(
    'title' => t('Read?'),
    'help' => t('Flag indicating if the message has been read.'),
    'field' => array(
      'handler' => 'views_handler_field_boolean',
      'click sortable' => TRUE,
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_boolean_operator',
    ),
  );

  $data['contact_save']['view_link'] = array(
    'field' => array(
      'title' => t('View link'),
      'help' => t('Provide a simple link to view a message.'),
      'handler' => 'contact_save_handler_message_link_view',
    ),
  );

  $data['contact_save']['reply_link'] = array(
    'field' => array(
      'title' => t('Reply link'),
      'help' => t('Provide a simple link to reply to a message.'),
      'handler' => 'contact_save_handler_message_link_reply',
    ),
  );

  $data['contact_save']['delete_link'] = array(
    'field' => array(
      'title' => t('Delete link'),
      'help' => t('Provide a simple link to delete a message.'),
      'handler' => 'contact_save_handler_message_link_delete',
    ),
  );

  // Contact.
  $data['contact']['table']['group'] = t('Contact');

  $data['contact']['table']['base'] = array(
    'field' => 'cid',
    'title' => t('Contact'),
    'help' => t('Contact'),
  );

  $data['contact']['cid'] = array(
    'title' => t('Contact ID'),
    'help' => t('Contact category ID'),
    'field' => array(
      'handler' => 'views_handler_field_numeric',
      'click sortable' => TRUE,
    ),
    'argument' => array(
      'handler' => 'views_handler_argument_numeric',
      'numeric' => TRUE,
      'name field' => 'cid',
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_numeric',
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
  );

  $data['contact']['category'] = array(
    'title' => t('Category'),
    'help' => t('Category.'),
    'field' => array(
      'handler' => 'views_handler_field_markup',
      'format' => filter_fallback_format(),
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_string',
    ),
  );

  return $data;
}

<?php

/**
 * @file
 * Provide views data and handlers for contact_centre.module.
 */

/**
 * Implements hook_views_data().
 */
function contact_centre_views_data() {
  $data['contact_centre_messages']['table']['group'] = t('Contact centre');

  $data['contact_centre_messages']['table']['base'] = array(
    'field' => 'id',
    'title' => t('Contact centre'),
    'help' => t('Contact centre'),
  );

  $data['contact_centre_messages']['table']['join'] = array(
    'users' => array(
      'left_field' => 'uid',
      'field' => 'uid',
    ),
  );

  $data['contact_centre_messages']['table']['join'] = array(
    'contact' => array(
      'left_field' => 'cid',
      'field' => 'cid',
    ),
  );

  $data['contact_centre_messages']['id'] = array(
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

  $data['contact_centre_messages']['cid'] = array(
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

  $data['contact_centre_messages']['uid'] = array(
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

  $data['contact_centre_messages']['mail'] = array(
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

  $data['contact_centre_messages']['subject'] = array(
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

  $data['contact_centre_messages']['message'] = array(
    'title' => t('Message'),
    'help' => t('Message body.'),
    'field' => array(
      'handler' => 'views_handler_field_markup',
      'format' => filter_fallback_format(),
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_string',
    ),
  );

  $data['contact_centre_messages']['created'] = array(
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

  $data['contact_centre_messages']['opened'] = array(
    'title' => t('Opened'),
    'help' => t('Flag indicating if the message has been opened.'),
    'field' => array(
      'handler' => 'views_handler_field_boolean',
      'click sortable' => TRUE,
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_boolean_operator',
    ),
  );

  $data['contact_centre_messages']['view_link'] = array(
    'field' => array(
      'title' => t('View link'),
      'help' => t('Provide a link to view a message.'),
      'handler' => 'contact_centre_handler_view_link',
    ),
  );

  $data['contact_centre_messages']['addnote_link'] = array(
    'field' => array(
      'title' => t('Add note link'),
      'help' => t('Provide a link to add a note to a message.'),
      'handler' => 'contact_centre_handler_addnote_link',
    ),
  );

  // Contact centre actions
  $data['contact_centre_actions']['table']['group'] = t('Contact centre actions');

  $data['contact_centre_actions']['table']['base'] = array(
    'field' => 'id',
    'title' => t('Contact centre actions'),
    'help' => t('Actions and notes recorded against contact centre messages.'),
  );

  $data['contact_centre_actions']['table']['join'] = array(
    'users' => array(
      'left_field' => 'uid',
      'field' => 'uid',
    ),
  );

  $data['contact_centre_actions']['table']['join'] = array(
    'contact_centre_messages' => array(
      'left_field' => 'id',
      'field' => 'cid',
    ),
  );

  $data['contact_centre_actions']['id'] = array(
    'title' => t('Action ID'),
    'help' => t('Action/note ID'),
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

  $data['contact_centre_actions']['cid'] = array(
    'title' => t('CID'),
    'help' => t('Contact message ID'),
    'field' => array(
      'handler' => 'views_handler_field_numeric',
      'click sortable' => TRUE,
    ),
    'argument' => array(
      'handler' => 'views_handler_argument_numeric',
      'name field' => 'name',
    ),
    'filter' => array(
      'title' => t('Name'),
      'handler' => 'views_handler_filter_numeric',
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
    'relationship' => array(
      'title' => t('Contact Message'),
      'help' => t('Relate action/note to the message it is associated with.'),
      'handler' => 'views_handler_relationship',
      'base' => 'contact_centre_messages',
      'field' => 'cid',
      'label' => t('contact_message'),
    ),
  );

  $data['contact_centre_actions']['uid'] = array(
    'title' => t('UID'),
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

  $data['contact_centre_actions']['action'] = array(
    'title' => t('Action/note'),
    'help' => t('Action/note recorded against message.'),
    'field' => array(
      'handler' => 'views_handler_field_markup',
      'format' => filter_fallback_format(),
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_string',
    ),
  );

  $data['contact_centre_actions']['action_type'] = array(
    'title' => t('Action type'),
    'help' => t('Type of action (note/reply).'),
    'field' => array(
      'handler' => 'views_handler_field_markup',
      'format' => 'plain_text',
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_string',
    ),
  );

  $data['contact_centre_actions']['created'] = array(
    'title' => t('Created'),
    'help' => t('Time when action/note was recorded.'),
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

<?php

/**
 * @file
 * Views interface for newsletter.
 */

/**
 * Implements hook_views_data().
 */
function newsletter_views_data() {
  $data['newsletter_list']['table'] = array(
    'base' => array(
      'field' => 'nlid',
      'title' => t('Newsletter list'),
    ),
    'group' => t('Newsletter list'),
    'entity_type' => 'newsletter_list',
  );

  // nlid
  $data['newsletter_list']['nlid'] = array(
    'title' => t('Nlid'),
    'help' => t('The newsletter list ID.'),
    'field' => array(
      'handler' => 'views_handler_field_numeric',
      'click sortable' => TRUE,
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_numeric',
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
  );

  // title
  $data['newsletter_list']['title'] = array(
    'title' => t('Title'),
    'help' => t('The newsletter list name.'),
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

  // send_rate
  $data['newsletter_list']['send_rate'] = array(
    'title' => t('Send rate'),
    'help' => t('The newsletter list send rate.'),
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

  // last_sent
  $data['newsletter_list']['last_sent'] = array(
    'title' => t('Last sent'),
    'help' => t('The newsletter list last sent date.'),
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

  // send_again
  $data['newsletter_list']['send_again'] = array(
    'title' => t('Send again'),
    'help' => t('The newsletter list send again date.'),
    'field' => array(
      'handler' => 'newsletter_handler_field_datetime',
    ),
  );

  // created
  $data['newsletter_list']['created'] = array(
    'title' => t('Created date'),
    'help' => t('The date the newsletter list was created.'),
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

  $data['newsletter_newsletter']['table'] = array(
    'base' => array(
      'field' => 'nnid',
      'title' => t('Newsletter'),
    ),
    'group' => t('Newsletter'),
    'entity_type' => 'newsletter_newsletter',
  );

  // nnid
  $data['newsletter_newsletter']['nnid'] = array(
    'title' => t('Nnid'),
    'help' => t('The newsletter ID.'),
    'field' => array(
      'handler' => 'views_handler_field_numeric',
      'click sortable' => TRUE,
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_numeric',
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
  );

  // title
  $data['newsletter_newsletter']['title'] = array(
    'title' => t('Title'),
    'help' => t('The newsletter title.'),
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
  );

  // send_id
  $data['newsletter_newsletter']['send_id'] = array(
    'title' => t('Send ID'),
    'help' => t('The times a newsletter has been sent.'),
    'field' => array(
      'handler' => 'views_handler_field_numeric',
      'click sortable' => TRUE,
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_numeric',
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
  );

  // custom
  $data['newsletter_newsletter']['custom'] = array(
    'title' => t('Custom'),
    'help' => t('Whether this newsletter sent as a custom one.'),
    'field' => array(
      'handler' => 'views_handler_field_boolean',
      'click sortable' => TRUE,
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_boolean_operator',
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
  );

  // ntid
  $data['newsletter_newsletter']['ntid'] = array(
    'title' => t('Template ID'),
    'help' => t('The newsletter template ({newsletter_template}.ntid) this newsletter used.'),
    'field' => array(
      'handler' => 'views_handler_field_numeric',
      'click sortable' => TRUE,
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_numeric',
      'allow empty' => TRUE,
    ),
    'argument' => array(
      'handler' => 'views_handler_argument_numeric',
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
    'relationship' => array(
      'handler' => 'views_handler_relationship',
      'base' => 'newsletter_template',
      'base field' => 'ntid',
      'label' => t('Newsletter template'),
    ),
  );

  // subscribers_sent
  $data['newsletter_newsletter']['subscribers_sent'] = array(
    'title' => t('Subscribers sent'),
    'help' => t('The number of the subscribers a newsletter has been sent to.'),
    'field' => array(
      'handler' => 'views_handler_field_numeric',
      'click sortable' => TRUE,
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_numeric',
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
  );

  // clicks
  $data['newsletter_newsletter']['clicks'] = array(
    'title' => t('Clicks'),
    'help' => t('The number of clicks a newsletter gathered.'),
    'field' => array(
      'handler' => 'views_handler_field_numeric',
      'click sortable' => TRUE,
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_numeric',
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
  );

  // opens
  $data['newsletter_newsletter']['opens'] = array(
    'title' => t('Opens'),
    'help' => t('The number of opens a newsletter gathered.'),
    'field' => array(
      'handler' => 'views_handler_field_numeric',
      'click sortable' => TRUE,
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_numeric',
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
  );

  // last_sent
  $data['newsletter_newsletter']['last_sent'] = array(
    'title' => t('Last sent'),
    'help' => t('The newsletter last sent date.'),
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

  $data['newsletter_subscriber']['table'] = array(
    'base' => array(
      'field' => 'nsid',
      'title' => t('Newsletter subscriber'),
    ),
    'group' => t('Newsletter subscriber'),
    'entity_type' => 'newsletter_subscriber',
  );

  // nsid
  $data['newsletter_subscriber']['nsid'] = array(
    'title' => t('Nsid'),
    'help' => t('The newsletter subscriber ID.'),
    'field' => array(
      'handler' => 'views_handler_field_numeric',
      'click sortable' => TRUE,
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_numeric',
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
  );

  // uid
  $data['newsletter_subscriber']['uid'] = array(
    'title' => t('Uid'),
    'help' => t('The newsletter subscriber user ID.'),
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
      'handler' => 'views_handler_relationship',
      'base' => 'users',
      'base field' => 'uid',
      'label' => t('User'),
    ),
  );

  // email
  $data['newsletter_subscriber']['email'] = array(
    'title' => t('Email'),
    'help' => t('The newsletter subscriber email.'),
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
  );

  // firstname
  $data['newsletter_subscriber']['firstname'] = array(
    'title' => t('First name'),
    'help' => t('The newsletter subscriber first name.'),
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
  );

  // lastname
  $data['newsletter_subscriber']['lastname'] = array(
    'title' => t('Last name'),
    'help' => t('The newsletter subscriber last name.'),
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
  );

  // gender
  $data['newsletter_subscriber']['gender'] = array(
    'title' => t('Gender'),
    'help' => t('The newsletter subscriber gender.'),
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
  );

  // receive_format
  $data['newsletter_subscriber']['receive_format'] = array(
    'title' => t('Receive format'),
    'help' => t('The newsletter subscriber receive format.'),
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
  );

  // ip
  $data['newsletter_subscriber']['ip'] = array(
    'title' => t('IP'),
    'help' => t('The newsletter subscriber IP address.'),
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
  );

  // hash
  $data['newsletter_subscriber']['hash'] = array(
    'title' => t('Hash'),
    'help' => t('The newsletter subscriber unique hash.'),
    'field' => array(
      'handler' => 'views_handler_field',
      'click sortable' => TRUE,
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
  );

  // confirmed
  $data['newsletter_subscriber']['confirmed'] = array(
    'title' => t('Confirmed'),
    'help' => t('Whether this subscriber has confirmed his subscription.'),
    'field' => array(
      'handler' => 'views_handler_field_boolean',
      'click sortable' => TRUE,
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_boolean_operator',
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
  );

  // confirmation_timestamp
  $data['newsletter_subscriber']['confirmation_timestamp'] = array(
    'title' => t('Confirmation date'),
    'help' => t('The date the newsletter subscriber confirmed his subscription.'),
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

  // created
  $data['newsletter_subscriber']['created'] = array(
    'title' => t('Created date'),
    'help' => t('The date the newsletter subscriber was created.'),
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

  $data['newsletter_template']['table'] = array(
    'base' => array(
      'field' => 'ntid',
      'title' => t('Newsletter template'),
    ),
    'group' => t('Newsletter template'),
    'entity_type' => 'newsletter_template',
  );

  // ntid
  $data['newsletter_template']['ntid'] = array(
    'title' => t('Ntid'),
    'help' => t('The newsletter template ID.'),
    'field' => array(
      'handler' => 'views_handler_field_numeric',
      'click sortable' => TRUE,
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_numeric',
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
  );

  // subject
  $data['newsletter_template']['subject'] = array(
    'title' => t('Subject'),
    'help' => t('The newsletter template subject.'),
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
  );

  // basic
  $data['newsletter_template']['basic'] = array(
    'title' => t('Basic'),
    'help' => t('Whether this template is basic or not.'),
    'field' => array(
      'handler' => 'views_handler_field_boolean',
      'click sortable' => TRUE,
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_boolean_operator',
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
  );

  // exposed
  $data['newsletter_template']['exposed'] = array(
    'title' => t('Exposed'),
    'help' => t('Whether this list is exposed or not'),
    'field' => array(
      'handler' => 'views_handler_field_boolean',
      'click sortable' => TRUE,
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_boolean_operator',
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
  );

  // created
  $data['newsletter_template']['created'] = array(
    'title' => t('Created date'),
    'help' => t('The date the newsletter template was created.'),
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

  return $data;
}

<?php
function user_mailman_register_views_data() {

  // Mailing lists

  $data['mailman_lists']['table']['group'] = t('Mailman Lists');

  $data['mailman_lists']['table']['base'] = array(
    'field' => 'lid',
    'title' => t('Mailman lists'),
    'help' => t('Mailman mailing lists managed by User Mailman Register')
  );

  $data['mailman_lists']['table']['join']['users'] = array(
    'left_table' => 'mailman_users',
    'left_field' => 'lid',
    'field' => 'lid',
    'type' => 'INNER',
  );

  $data['mailman_lists']['lid'] = array(
    'title' => t('List ID'),
     'help' => t('List ID'),
    'field' => array(
      'handler' => 'views_handler_field',
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_string',
    ),
  );

  $data['mailman_lists']['name'] = array(
    'title' => t('Mailing list name'),
    'help' => t('Name of mailing list'),
    'field' => array(
      'handler' => 'views_handler_field',
    ),
    'sort' => array(
     'handler' => 'views_handler_sort',
     ),
    'filter' => array(
      'handler' => 'user_mailman_register_handler_filter_listname',
    ),
  );

  $data['mailman_lists']['command'] = array(
    'title' => t('Request email address'),
    'help' => t('Email address for user requests'),
    'field' => array(
      'handler' => 'user_mailman_register_handler_field_mail',
      'click sortable' => TRUE,
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_string',
    ),
  );

  $data['mailman_lists']['admin'] = array(
    'title' => t('Admin email address'),
    'help' => t('Email address for admin requests'),
    'field' => array(
      'handler' => 'user_mailman_register_handler_field_mail',
      'click sortable' => TRUE,
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_string',
    ),
  );

  $data['mailman_lists']['web'] = array(
    'title' => t('Web address'),
     'help' => t('Web address for users'),
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

  $data['mailman_lists']['webarch'] = array(
    'title' => t('Archive web address'),
    'help' => t("Web archive address for users"),
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

  $data['mailman_lists']['webadmin'] = array(
    'title' => t('Admin web page'),
    'help' => t('Web address for administration'),
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

  $data['mailman_lists']['user_invite'] = array(
    'title' => t('Only invite users'),
    'help' => t('Invite new users instead of subscribing them?'),
    'field' => array(
       'handler' => 'views_handler_field_boolean',
       'click sortable' => TRUE,
     ),
     'filter' => array(
       'handler' => 'views_handler_filter_boolean_operator',
       'label' => t('Published'),
       'type' => 'yes-no',
     ),
     'sort' => array(
       'handler' => 'views_handler_sort',
     ),
  );

  $data['mailman_lists']['user_notify'] = array(
    'title' => t('Notify users'),
    'help' => t('Send welcome messages to the new subscribee?'),
    'field' => array(
       'handler' => 'views_handler_field_boolean',
       'click sortable' => TRUE,
     ),
     'filter' => array(
       'handler' => 'views_handler_filter_boolean_operator',
       'label' => t('Published'),
       'type' => 'yes-no',
     ),
     'sort' => array(
       'handler' => 'views_handler_sort',
     ),
  );

  $data['mailman_lists']['user_admin_notify'] = array(
    'title' => t('Notify mailman admins'),
    'help' => t('Send notifications of new subscription to the list owner?'),
    'field' => array(
       'handler' => 'views_handler_field_boolean',
       'click sortable' => TRUE,
     ),
     'filter' => array(
       'handler' => 'views_handler_filter_boolean_operator',
       'label' => t('Published'),
       'type' => 'yes-no',
     ),
     'sort' => array(
       'handler' => 'views_handler_sort',
     ),
  );

   $data['mailman_lists']['allow_unsubscribe'] = array(
    'title' => t('Allow unsubscribe'),
    'help' => t('Allow users to unsubscribe?'),
    'field' => array(
       'handler' => 'views_handler_field_boolean',
       'click sortable' => TRUE,
     ),
     'filter' => array(
       'handler' => 'views_handler_filter_boolean_operator',
       'label' => t('Published'),
       'type' => 'yes-no',
     ),
     'sort' => array(
       'handler' => 'views_handler_sort',
     ),
  );

  $data['mailman_lists']['allow_temp_disable'] = array(
    'title' => t('Allow temporary disable'),
    'help' => t('Allow users to temporarily disable delivery?'),
    'field' => array(
       'handler' => 'views_handler_field_boolean',
       'click sortable' => TRUE,
     ),
     'filter' => array(
       'handler' => 'views_handler_filter_boolean_operator',
       'label' => t('Published'),
       'type' => 'yes-no',
     ),
     'sort' => array(
       'handler' => 'views_handler_sort',
     ),
  );

  $data['mailman_lists']['allow_digest'] = array(
    'title' => t('Allow digest'),
    'help' => t('Allow users to subscribe in digest mode?'),
    'field' => array(
       'handler' => 'views_handler_field_boolean',
       'click sortable' => TRUE,
     ),
     'filter' => array(
       'handler' => 'views_handler_filter_boolean_operator',
       'label' => t('Published'),
       'type' => 'yes-no',
     ),
     'sort' => array(
       'handler' => 'views_handler_sort',
     ),
  );

  $data['mailman_lists']['description'] = array(
    'title' => t('Description'),
    'help' => t('A brief description of this mailing list.'),
    'field' => array(
      'handler' => 'views_handler_field',
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_string',
    ),
  );

  $data['mailman_lists']['defaultnewreg'] = array(
    'title' => t('Default subscription'),
    'help' => t('Default subscription choice in the user subscription form.'),
    'field' => array(
       'handler' => 'views_handler_field_boolean',
       'click sortable' => TRUE,
     ),
     'filter' => array(
       'handler' => 'views_handler_filter_boolean_operator',
       'label' => t('Published'),
       'type' => 'yes-no',
     ),
     'sort' => array(
       'handler' => 'views_handler_sort',
     ),
  );

  $data['mailman_lists']['role_sync'] = array(
    'title' => t('Sync with roles'),
    'help' => t('Synchronize subscription with role/permissions?'),
    'field' => array(
       'handler' => 'user_mailman_register_handler_field_sync_status',
       'click sortable' => TRUE,
     ),
     'filter' => array(
       'handler' => 'user_mailman_register_handler_filter_sync_status',
     ),
     'sort' => array(
       'handler' => 'views_handler_sort',
     ),
  );

  // Mailman Subscribers

  $data['mailman_users']['table']['group'] = t('Mailman Subscribers');
  $data['mailman_users']['table']['join']['users'] = array(
    'left_field' => 'uid',
    'field' => 'uid',
    'type' => 'INNER',
  );

  $data['mailman_users']['uid'] = array(
    'title' => t('User Uid'),
    'help' => t('Users'),
    'field' => array(
      'handler' => 'user_mailman_register_handler_field_subscription_link',
      'click sortable' => TRUE,
    ),
    'relationship' => array(
      'base' => 'users',
      'field' => 'uid',
      'handler' => 'views_handler_relationship',
      'label' => t('User relationship'),
    ),
  );

  $data['mailman_users']['lid'] = array(
    'title' => t('List ID'),
    'help' => t('List ID'),
    'field' => array(
      'handler' => 'views_handler_field',
      'click sortable' => false,
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_string',
    ),
    'relationship' => array(
     'base' => 'mailman_lists',
     'field' => 'lid',
     'handler' => 'views_handler_relationship',
     'label' => t('List relationship'),
    ),
  );

  $data['mailman_users']['lstatus'] = array(
    'title' => t('Subscription status'),
    'help'  => t('Stored subscription status of the user'),
    'field' => array(
       'handler' => 'user_mailman_register_handler_field_subscription_status',
       'click sortable' => TRUE,
     ),
     'filter' => array(
       'handler' => 'user_mailman_register_handler_filter_subscription_status',
     ),
     'sort' => array(
       'handler' => 'views_handler_sort',
     ),
  );

  $data['mailman_users']['lmail'] = array(
    'title' => t('Subscribed email'),
    'help' => t('Email address at which the user is subscribed'),
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
  );

  $data['mailman_users']['lchecktime'] = array(
    'title' => t('Last sync'),
    'help' => t('Last time that subscription was synchronized with Mailman'),
    'field' => array(
        'handler' => 'views_handler_field_date',
    ),
    'filter' => array(
        'handler' => 'views_handler_filter_date',
    ),
    'sort' => array(
        'handler' => 'views_handler_sort_date',
    ),
  );

  $data['mailman_users']['lmod'] = array(
    'title' => t('Moderated'),
    'help' => t('Moderation status of user'),
    'field' => array(
       'handler' => 'views_handler_field_boolean',
       'click sortable' => TRUE,
     ),
     'filter' => array(
       'handler' => 'views_handler_filter_boolean_operator',
       'label' => t('Published'),
       'type' => 'yes-no',
     ),
     'sort' => array(
       'handler' => 'views_handler_sort',
     ),
  );

return $data;
}


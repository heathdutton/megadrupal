<?php
/**
 * @file
 * Provides Views integration for the Mail Tracking module.
 */

/**
 * Implements hook_views_data().
 */
function mail_tracking_views_data() {
  // Text field defaults.
  $text_default = array(
    'field' => array(
      'handler' => 'views_handler_field',
      'click sortable' => TRUE,
    ),
    'argument' => array(
      'handler' => 'views_handler_argument_string',
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_string',
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
  );

  // Numeric field defaults.
  $numeric_default = array(
    'field' => array(
      'handler' => 'views_handler_field_numeric',
      'click sortable' => TRUE,
    ),
    'argument' => array(
      'handler' => 'views_handler_argument_numeric',
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_numeric',
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
  );

  // Boolean field defaults.
  $boolean_default = array(
    'field' => array(
      'handler' => 'views_handler_field_boolean',
      'click sortable' => TRUE,
    ),
    'argument' => array(
      'handler' => 'views_handler_argument_numeric',
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_boolean_operator',
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
  );

  // Statistics table.
  $data['mail_tracking_statistics']['table'] = array(
    'base' => array(
      'field' => 'sid',
      'title' => t('Mail Tracking statistics'),
      'help' => t('Send, read, clicked and unsubscribed statistics per period for the different e-mails.'),
      'weight' => 15,
      'database' => 'default',
    ),
    'group' => t('Mail Tracking statistics'),
  );

  // Statistics id.
  $data['mail_tracking_statistics']['sid'] = array(
    'title' => t('Statistics id'),
    'help' => t('Primary identifier of the statistics record.'),
   ) + $numeric_default;

  // Module.
  $data['mail_tracking_statistics']['module'] = array(
    'title' => t('Module'),
    'help' => t('Module defining the e-mail message.'),
  ) + $text_default;

  // Message key.
  $data['mail_tracking_statistics']['message_key'] = array(
    'title' => t('Message key'),
    'help' => t('E-mail message key.'),
  ) + $text_default;

  // Period.
  $data['mail_tracking_statistics']['period'] = array(
    'title' => t('Period'),
    'help' => t('Statistics period.'),
    'field' => array(
      'handler' => 'mail_tracking_views_handler_statistics_period',
      'click sortable' => TRUE,
    ),
    'argument' => array(
      'handler' => 'views_handler_argument_string',
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_string',
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
  );

  // Node.
  $data['mail_tracking_statistics']['nid'] = array(
    'title' => t('Node id'),
    'help' => t('Simplenews newsletter node id.'),
    'field' => array(
      'handler' => 'views_handler_field_node',
      'click sortable' => TRUE,
    ),
    'argument' => array(
      'handler' => 'views_handler_argument_numeric',
      'numeric' => TRUE,
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_numeric',
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
  );

  // Send count.
  $data['mail_tracking_statistics']['send_count'] = array(
    'title' => t('Send count'),
    'help' => t('Number of e-mails send.'),
  ) + $numeric_default;

  // Read count.
  $data['mail_tracking_statistics']['read_count'] = array(
    'title' => t('Read count'),
    'help' => t('Number of e-mails read.'),
  ) + $numeric_default;

  // Clicked count.
  $data['mail_tracking_statistics']['clicked_count'] = array(
    'title' => t('Clicked count'),
    'help' => t('Number of recipients that clicked on a link.'),
  ) + $numeric_default;

  // Send count.
  $data['mail_tracking_statistics']['unsubscribed_count'] = array(
    'title' => t('Unsubscribed count'),
    'help' => t('Number recipients that unsubscribed from the newsletter.'),
  ) + $numeric_default;

  $data['mail_tracking_statistics']['mail_tracking_statistics_to_node'] = array(
    'title' => t('Node'),
    'help' => t('Simplenews newsletter node for this Mail Tracking statistics.'),
    'relationship' => array(
      'title' => t('Simplenews newsletter node'),
      'label' => t('Node'),
      'base' => 'node',
      'base field' => 'nid',
      'relationship field' => 'nid',
    ),
  );

  $data['mail_tracking_statistics']['mail_tracking_statistics_to_mail_tracking_token'] = array(
    'title' => t('Mail Tracking token'),
    'help' => t('Mail Tracking tokens for this statistics.'),
    'relationship' => array(
      'title' => t('Mail Tracking token'),
      'label' => t('mail_tracking_token'),
      'base' => 'mail_tracking_token',
      'base field' => 'sid',
      'relationship field' => 'sid',
    ),
  );

  // Token table.
  $data['mail_tracking_token']['table'] = array(
    'base' => array(
      'field' => 'token',
      'title' => t('Mail Tracking token'),
      'help' => t('Tokens and statistics per e-mail receipient.'),
      'weight' => 16,
      'database' => 'default',
    ),
    'group' => t('Mail Tracking token'),
  );

  // Token.
  $data['mail_tracking_token']['token'] = array(
    'title' => t('Token'),
    'help' => t('Token that identifies the recipient.'),
  ) + $text_default;

  // Recipient.
  $data['mail_tracking_token']['recipient'] = array(
    'title' => t('Recipient'),
    'help' => t('The recipients e-mail address.'),
  ) + $text_default;

  // Is read.
  $data['mail_tracking_token']['is_read'] = array(
    'title' => t('Is read'),
    'help' => t('Indicates if the message was read.'),
  ) + $boolean_default;

  // Is clicked.
  $data['mail_tracking_token']['is_clicked'] = array(
    'title' => t('Is clicked'),
    'help' => t('Indicates if the recipient clicked on a link.'),
  ) + $boolean_default;

  // Is unsubscribed.
  $data['mail_tracking_token']['is_unsubscribed'] = array(
    'title' => t('Is unsubscribed'),
    'help' => t('Indicates if the recipient unsubscribed from the newsletter.'),
  ) + $boolean_default;

  // Timestamp.
  $data['mail_tracking_token']['timestamp'] = array(
    'title' => t('Timestamp'),
    'help' => t('Timestamp the token was created.'),
    'field' => array(
      'handler' => 'views_handler_field_date',
      'click sortable' => TRUE,
    ),
    'argument' => array(
      'handler' => 'views_handler_argument_date',
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_date',
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
  );

  // Sender.
  $data['mail_tracking_token']['sender'] = array(
    'title' => t('Sender'),
    'help' => t('E-mail address (and name in most cases) of the message sender.'),
  ) + $text_default;

  // Subject.
  $data['mail_tracking_token']['subject'] = array(
    'title' => t('Subject'),
    'help' => t('The message subject.'),
  ) + $text_default;

  // Statistics id.
  $data['mail_tracking_token']['sid'] = array(
    'title' => t('Statistics id'),
    'help' => t('Primary identifier of the grouping statistics record.'),
  ) + $numeric_default;

  // Source table.
  $data['mail_tracking_source']['table']['group'] = t('Mail Tracking source');

  // Node.
  $data['mail_tracking_source']['nid'] = array(
    'title' => t('Node id'),
    'help' => t('Simplenews newsletter node id.'),
    'field' => array(
      'handler' => 'views_handler_field_node',
      'click sortable' => TRUE,
    ),
    'argument' => array(
      'handler' => 'views_handler_argument_numeric',
      'numeric' => TRUE,
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_numeric',
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
  );

  // Source.
  $data['mail_tracking_source']['source'] = array(
    'title' => t('Source'),
    'help' => t('Text used as Google Analytics source parameter.'),
    'field' => array(
      'handler' => 'views_handler_field_machine_name',
      'click sortable' => TRUE,
    ),
    'argument' => array(
      'handler' => 'views_handler_argument_string',
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_string',
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
  );

  // Joins.
  $data['mail_tracking_source']['table']['join'] = array(
    'node' => array(
      'field' => 'nid',
      'left_field' => 'nid',
    ),
  );

  return $data;
}

/**
 * Implements hook_views_data_alter().
 */
function mail_tracking_views_data_alter(&$data) {
  if (isset($data['node'])) {
    $data['node']['node_to_mail_tracking_statistics'] = array(
      'title' => t('Mail Tracking statistics'),
      'help' => t('Mail Tracking statistics for this Simplenews newsletter node.'),
      'relationship' => array(
        'title' => t('Mail Tracking statistics'),
        'label' => t('mail_tracking_statistics'),
        'base' => 'mail_tracking_statistics',
        'base field' => 'nid',
        'relationship field' => 'nid',
      ),
    );

    $data['node']['mail_tracking_source'] = array(
      'title' => t('Mail Tracking source'),
      'help' => t('Mail Tracking source for this Simplenews newsletter node.'),
      'relationship' => array(
        'title' => t('Mail Tracking source'),
        'label' => t('mail_tracking_source'),
        'base' => 'mail_tracking_source',
        'base field' => 'nid',
        'relationship field' => 'nid',
      ),
    );
  }
}

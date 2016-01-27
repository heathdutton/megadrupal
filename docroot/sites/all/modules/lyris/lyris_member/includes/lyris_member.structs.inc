<?php
/**
 * @file
 * Struct and enum data for member member structs.
 */
/**
 * Implements hook_struct_data().
 */
function lyris_member_struct_data() {
  $list_ops = array();
  $lists = lyris_get_lists();
  $list_ops = array();
  foreach ($lists as $ListName => $list) {
    $list_ops[$ListName] = $list['title'];
  }

  /**
   * Member Struct
   */
  $form_fields['MemberStruct'] = array(
    // Vertical Tab Groups
    'vertical_tabs' => array(
      '#type' => 'vertical_tabs',
      '#groups' => array(
        'admin'         => array('#weight' => 1, '#title' => t('Administrative Settings')),
        'basic'         => array('#weight' => 2, '#title' => t('Basic Settings')),
        'advanced'      => array('#weight' => 3, '#title' => t('Advanced Settings')),
        'notifications' => array('#weight' => 4, '#title' => t('Notifications')),
        'meta'          => array('#weight' => 5, '#title' => t('Meta Information')),
      ),
      '#permanent' => TRUE,
    ),
    'MemberID' => array(
      '#type' => 'textfield',
      '#title' => t('Member ID'),
      '#description' => t('Unique member ID.'),
      '#default_value' => 0,
      '#api' => array('10','11','12'),
      '#group' => 'admin',
      '#schema' => array(
        'type' => 'int',
        'not null' => TRUE,
      ),
      '#defaultable' => FALSE,
    ),
    'Additional' => array(
      '#type' => 'textfield',
      '#title' => t('Additional Information'),
      '#description' => t('Additional information about this member (not used by ListManager).'),
      '#default_value' => NULL,
      '#maxlength' => 255,
      '#api' => array('1.5.2a', '10', '11'),
      '#group' => 'admin',
      '#schema' => array(
        'type' => 'text',
        'size' => 'big',
      ),
    ),
    'ApprovalNeeded' => array(
      '#type' => 'checkbox',
      '#title' => t('Approval Needed'),
      '#description' => t('Can this member bypass approval to send messages?'),
      '#default_value' => 0,
      '#api' => array('10','11','12'),
      '#schema' => array(
        'type' => 'int',
        'length' => 1,
        'unsigned' => TRUE,
      ),
      '#group' => 'advanced',
    ),
    'CanApprovePending' => array(
      '#type' => 'checkbox',
      '#title' => t('Can Approve Pending Messages'),
      '#description' => t('If true, and if this member is an admin, can this member approve pending (moderated) messages?'),
      '#default_value' => 0,
      '#api' => array('10','11','12'),
      '#schema' => array(
        'type' => 'int',
        'length' => 1,
        'unsigned' => TRUE,
      ),
      '#group' => 'advanced',
    ),
    'CleanAuto' => array(
      '#type' => 'checkbox',
      '#title' => t('Don\'t Hold Member'),
      '#description' => t('If true, do not mark as \'held\' this member if they bounce too much email (i.e.: let them bounce all they want)'),
      '#default_value' => 0,
      '#api' => array('10','11','12'),
      '#schema' => array(
        'type' => 'int',
        'length' => 1,
        'unsigned' => TRUE,
      ),
      '#group' => 'advanced',
    ),
    'Comment' => array(
      '#type' => 'textfield',
      '#title' => t('Comments'),
      '#description' => t('Additional information about this member (not used by ListManager).'),
      '#default_value' => NULL,
      '#maxlength' => 255,
      '#api' => array('1.5.2a', '10', '11'),
      '#group' => 'admin',
      '#schema' => array(
        'type' => 'text',
        'size' => 'big',
      ),
      '#group' => 'admin',
    ),
    'DateBounce' => array(
      '#type' => 'date',
      '#title' => t('Last Bounced Date'),
      '#description' => t('The date of the most recent bounce.'),
      '#default_value' => 0,
      '#api' => array('10','11','12'),
      '#group' => 'admin',
      '#schema' => array(
        'type' => 'int',
        'length' => 10,
        'unsigned' => TRUE,
      ),
      '#group' => 'meta',
      '#defaultable' => FALSE,
    ),
    'DateConfirm' => array(
      '#type' => 'date',
      '#title' => t('Last Confirm Date'),
      '#description' => t('Date the member was last sent a \'confirm\' message.'),
      '#default_value' => 0,
      '#api' => array('10','11','12'),
      '#group' => 'admin',
      '#schema' => array(
        'type' => 'int',
        'length' => 10,
        'unsigned' => TRUE,
      ),
      '#group' => 'meta',
      '#defaultable' => FALSE,
    ),
    'DateHeld' => array(
      '#type' => 'date',
      '#title' => t('Held Date'),
      '#description' => t('Date the member was held.'),
      '#default_value' => 0,
      '#api' => array('10','11','12'),
      '#group' => 'admin',
      '#schema' => array(
        'type' => 'int',
        'length' => 10,
        'unsigned' => TRUE,
      ),
      '#group' => 'meta',
      '#defaultable' => FALSE,
    ),
    'DateJoined' => array(
      '#type' => 'date',
      '#title' => t('Joined Date'),
      '#description' => t('Date when member joined this list.'),
      '#default_value' => 0,
      '#api' => array('10','11','12'),
      '#group' => 'admin',
      '#schema' => array(
        'type' => 'int',
        'length' => 10,
        'unsigned' => TRUE,
      ),
      '#group' => 'meta',
      '#defaultable' => FALSE,
    ),
    'DateUnsubscribed' => array(
      '#type' => 'date',
      '#title' => t('Unsubscribed Date'),
      '#description' => t('Date when member unsubscribed from this list.'),
      '#default_value' => 0,
      '#api' => array('10','11','12'),
      '#group' => 'admin',
      '#schema' => array(
        'type' => 'int',
        'length' => 10,
        'unsigned' => TRUE,
      ),
      '#group' => 'meta',
      '#defaultable' => FALSE,
    ),
    'Demographics' => array(
      '#type' => 'textarea',
      '#title' => t('Demographics'),
      '#description' => t('An array of KeyValueType(s) as defined by your database.  Separate keys from values with a pipe "|" and place one entry per line.'),
      '#default_value' => NULL,
      '#api' => array('10','11','12'),
      '#schema' => array(
        'type' => 'text',
        'size' => 'big',
      ),
      '#group' => 'advanced',
    ),
    'EmailAddress' => array(
      '#type' => 'textfield',
      '#title' => t('E-mail Address'),
      '#description' => t('The email address of the member.'),
      '#default_value' => NULL,
      '#api' => array('10','11','12'),
      '#schema' => array(
        'type' => 'varchar',
        'length' => 255,
      ),
      '#element_validate' => array('_lyris_element_validate_email'),
      '#group' => 'basic',
    ),
    'ExpireDate' => array(
      '#type' => 'date',
      '#title' => t('Expired Date'),
      '#description' => t('The date the membership expired.'),
      '#default_value' => 0,
      '#api' => array('10','11','12'),
      '#group' => 'admin',
      '#schema' => array(
        'type' => 'int',
        'length' => 10,
        'unsigned' => TRUE,
      ),
      '#group' => 'meta',
      '#defaultable' => FALSE,
    ),
    'FullName' => array(
      '#type' => 'textfield',
      '#title' => t('Member Name'),
      '#description' => t('The full name of the member.'),
      '#default_value' => NULL,
      '#api' => array('10','11','12'),
      '#schema' => array(
        'type' => 'varchar',
        'length' => 255,
      ),
      '#group' => 'basic',
    ),
    'IsListAdmin' => array(
      '#type' => 'checkbox',
      '#title' => t('List Admin'),
      '#description' => t('If true, the member is a list administrator.'),
      '#default_value' => 1,
      '#api' => array('1.5.2a', '10', '11'),
      '#schema' => array(
        'type' => 'int',
        'length' => 1,
        'unsigned' => TRUE,
      ),
      '#group' => 'advanced',
    ),
    'ListName' => array(
      '#type' => 'select',
      '#title' => t('List Name'),
      '#description' => t('The name of the list this member belongs to.'),
      '#options' => $list_ops,
      '#default_value' => NULL,
      '#api' => array('10','11','12'),
      '#schema' => array(
        'type' => 'varchar',
        'length' => 255,
      ),
      '#defaultable' => FALSE,
      '#group' => 'basic',
    ),
    'MailFormat' => array(
      '#type' => 'select',
      '#title' => t('Mail Format'),
      '#description' => t('What format does the user want to receive mail.'),
      '#options' => lyris_member_enum('MailFormatEnum'),
      '#default_value' => 'H',
      '#api' => array('10','11','12'),
      '#schema' => array(
        'type' => 'varchar',
        'length' => 1,
      ),
      '#group' => 'basic',
    ),
    'MembershipKind' => array(
      '#type' => 'select',
      '#title' => t('Membership Type'),
      '#description' => t('The kind of subscription this member has to this list.'),
      '#options' => lyris_member_enum('MemberKindEnum'),
      '#default_value' => 'mail',
      '#api' => array('10','11','12'),
      '#schema' => array(
        'type' => 'varchar',
        'length' => 10,
      ),
      '#group' => 'advanced',
    ),
    'MemberStatus' => array(
      '#type' => 'select',
      '#title' => t('Member Status'),
      '#description' => t('What is the status of this member? Only standard recipients will typically receive mail from a list.'),
      '#options' => lyris_member_enum('MemberStatusEnum'),
      '#default_value' => 'normal',
      '#api' => array('10','11','12'),
      '#schema' => array(
        'type' => 'varchar',
        'length' => 13,
      ),
      '#group' => 'advanced',
    ),
    'NoRepro' => array(
      '#type' => 'checkbox',
      '#title' => t('CC Self'),
      '#description' => t('If true, the member should receive a copy of their own postings.'),
      '#default_value' => 0,
      '#api' => array('10','11','12'),
      '#schema' => array(
        'type' => 'int',
        'length' => 1,
        'unsigned' => TRUE,
      ),
      '#group' => 'notifications',
    ),
    'NotifyError' => array(
      '#type' => 'checkbox',
      '#title' => t('Error Notification'),
      '#description' => t('If true, the member (if a list administrator) should receive list error mail.'),
      '#default_value' => 0,
      '#api' => array('10','11','12'),
      '#schema' => array(
        'type' => 'int',
        'length' => 1,
        'unsigned' => TRUE,
      ),
      '#group' => 'notifications',
    ),
    'NotifySubmission' => array(
      '#type' => 'checkbox',
      '#title' => t('Moderation Notification'),
      '#description' => t('If true, the member (if a list administrator) should receive notification of messages pending moderation.'),
      '#default_value' => 0,
      '#api' => array('10','11','12'),
      '#schema' => array(
        'type' => 'int',
        'length' => 1,
        'unsigned' => TRUE,
      ),
      '#group' => 'notifications',
    ),
    'NumApprovalsNeeded' => array(
      '#type' => 'textfield',
      '#title' => t('Number of Approvals Needed'),
      '#description' => t('The number of approvals this member needs before posting to the list unapproved.'),
      '#size' => 10,
      '#maxlength' => 3,
      '#default_value' => 0,
      '#api' => array('10','11','12'),
      '#element_validate' => array('_lyris_element_validate_integer'),
      '#schema' => array(
        'type' => 'int',
      ),
      '#group' => 'advanced',
    ),
    'NumberOfBounces' => array(
      '#type' => 'textfield',
      '#title' => t('Number of Bounces'),
      '#description' => t('The number of bounces this member has had.'),
      '#size' => 10,
      '#maxlength' => 10,
      '#default_value' => 0,
      '#api' => array('10','11','12'),
      '#element_validate' => array('_lyris_element_validate_integer'),
      '#schema' => array(
        'type' => 'int',
      ),
      '#group' => 'meta',
      '#defaultable' => FALSE,
    ),
    'Password' => array(
      '#type' => 'textfield',
      '#title' => t('Password'),
      '#description' => t('The member\'s password.'),
      '#default_value' => NULL,
      '#api' => array('10','11','12'),
      '#schema' => array(
        'type' => 'varchar',
        'length' => 255,
      ),
      '#group' => 'basic',
    ),
    'ReadsHtml' => array(
      '#type' => 'checkbox',
      '#title' => t('Reads HTML'),
      '#description' => t('If true, this member has been detected to read HTML.'),
      '#default_value' => 0,
      '#api' => array('10','11','12'),
      '#schema' => array(
        'type' => 'int',
        'length' => 1,
        'unsigned' => TRUE,
      ),
      '#group' => 'meta',
      '#defaultable' => FALSE,
    ),
    'ReceiveAcknowlegment' => array(
      '#type' => 'checkbox',
      '#title' => t('Receive Acknowledgement'),
      '#description' => t('If true, this member receives a message acknowledging a posting to the list.'),
      '#default_value' => 0,
      '#api' => array('10','11','12'),
      '#schema' => array(
        'type' => 'int',
        'length' => 1,
        'unsigned' => TRUE,
      ),
      '#group' => 'notifications',
    ),
    'ReceiveAdminEmail' => array(
      '#type' => 'checkbox',
      '#title' => t('Receive Admin E-mails'),
      '#description' => t('If true, this member (if a list administrator) should receive messages for list administrators.'),
      '#default_value' => 0,
      '#api' => array('10','11','12'),
      '#schema' => array(
        'type' => 'int',
        'length' => 1,
        'unsigned' => TRUE,
      ),
      '#group' => 'notifications',
    ),
  );

  return $form_fields;
}

/**
 * Lyris Enumerations
 */
function lyris_member_enum($type) {
  $enums = array(
    'MailFormatEnum' => array(
      'T' => t('Text Only'),
      'H' => t('HTML'),
      'M' => t('Multipart')
    ),
    'MemberKindEnum' => array(
      'digest' => t('A digest'),
      'mimedigest' => t('A MIME digest'),
      'index' => t('Subject line index'),
      'nomail' => t('No mail'),
      'mail' => t('Normal list mail'),
    ),
    'MemberStatusEnum' => array(
      'normal' => t('Subscribed'),
      'confirm' => t('Awaiting Member Confirmation'),
      'private' => t('Awaiting Admin Approval'),
      'expired' => t('Expired'),
      'held' => t('Held (Invalid or Bounced)'),
      'unsub' => t('Unsubscribed'),
      'referred' => t('Invited, Not Joined'),
      'needs-confirm' => t('Needs Confirmation E-mail'),
      'needs-hello' => t('Needs Welcome E-mail'),
      'needs-goodbye' => t('Needs Goodbye E-mail'),
    ),
  );

  return $enums[$type];
}

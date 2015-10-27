<?php
/**
 * @file
 * Mailchimp user list module table of user mailing lists.
 */

/**
 * Table of the current users lists.
 *
 * @return string
 *   A table of users with user ID, name, and lists.
 */
function mailchimp_user_list_user_table() {
  $user_lists_details = mailchimp_user_list_retrieve_users_lists();
  $rows = array();
  if ($user_lists_details) {
    if (!is_array($user_lists_details['data'])) {
      $user_lists_details['data'][0] = $user_lists_details['data'];
    }
    foreach ($user_lists_details['data'] as $list_detail) {
      $rows[] = array(
        $list_detail['name'],
        $list_detail['default_from_name'],
        t(
          '<a href="@url">@member_count</a>',
          array(
            '@url' => url('admin/mailchimp/user_lists/' . $list_detail['id'] . '/members'),
            '@member_count' => $list_detail['stats']['member_count'],
          )
        ),
        $list_detail['stats']['unsubscribe_count'],
        $list_detail['stats']['open_rate'],
        $list_detail['stats']['click_rate'],
        t(
          '<a href="@view_campaign">View Campaigns</a>
          <br/>
          <a href="@new_campaign">New Campaign</a>
          <br/>
          <a href="@list_members">List Members</a>',
          array(
            '@view_campaign' => url('admin/mailchimp/user_lists/' . $list_detail['id'] . '/campaigns'),
            '@new_campaign' => url('admin/mailchimp/user_lists/' . $list_detail['id'] . '/edit_campaign'),
            '@list_members' => url('admin/mailchimp/user_lists/' . $list_detail['id'] . '/members'),
          )
        ),
      );
    }
  }
  $output = array(
    'header' => array(
      t('List Name'),
      t('Emails from'),
      t('Members'),
      t('Unsubscribed'),
      t('Open Rate'),
      t('Click Rate'),
      t('Campaigns'),
    ),
    'rows' => $rows,
  );
  return theme('table', $output);
}

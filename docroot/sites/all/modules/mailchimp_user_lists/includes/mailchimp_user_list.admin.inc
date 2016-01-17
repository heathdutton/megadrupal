<?php
/**
 * @file
 * Mailchimp user list module admin settings.
 */

/**
 * Creates a table for mailchimp user lists.
 *
 * @return string
 *   A table of users with user ID, name, and lists.
 */
function mailchimp_user_list_all_users_table() {
  $results = mailchimp_user_list_query();
  $rows = array();
  foreach ($results as $record) {
    $exp_list_ids = explode('|', $record->list_ids);
    foreach ($exp_list_ids as $key => $list_id) {
      if (isset($list_id)) {
        $exp_list_ids[$key] = mailchimp_user_list_list_title($list_id);
      }
    }
    $list_names = implode('<br/>', $exp_list_ids);
    $user_info = user_load($record->uid);
    $rows[] = array(
      $record->uid,
      t('<a href="@url">@name</a>',
        array(
          '@url' => 'user/' . $record->uid . '/edit',
          '@name' => $user_info->name,
        )
      ),
      $list_names,
    );
  }
  $output = array(
    'header' => array(
      t('User ID'),
      t('User Name'),
      t('List IDs'),
    ),
    'rows' => $rows,
  );
  return theme('table', $output);
}

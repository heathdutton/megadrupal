<?php
/**
 * @file
 * Mailchimp user list module campaign tables.
 */

/**
 * Wrapper function to make page with table and form.
 *
 * @return string
 *   html content
 */
function mailchimp_user_list_campaign_table_make_page($lid) {
  $page = mailchimp_user_list_campaign_table($lid);
  if (mailchimp_user_list_new_campaign_access()) {
    $form = drupal_get_form('mailchimp_user_list_campaign_table_form', $lid);
    $page .= drupal_render($form);
  }
  return $page;
}

/**
 * Function to generate table of campaigns available under mailing list.
 *
 * @return string
 *   html table
 */
function mailchimp_user_list_campaign_table($lid) {
  global $user;
  $q = mailchimp_get_api_object();
  $filters['list_id'] = $lid;
  // Retrieve all campaigns based on list id.
  $results = $q->campaigns($filters);
  if ($q->errorCode) {
    drupal_set_message(t('There was a problem retrieving your lists
      please contact your system administrator.'));
    watchdog('mailchimp',
      'Could not retrieve campaigns for user @user on list @list_name ID number @list_id error @error',
      array(
        '@user' => $user->name,
        '@list_name' => mailchimp_user_list_list_title(),
        '@list_id' => $lid,
        '@error' => $q->errorMessage,
      ),
      WATCHDOG_ERROR
    );
  }
  else {
    if ($results) {
      foreach ($results['data'] as $campaign) {
        $disable_stats = array('save', 'sending');
        $disable_edit = array('sent', 'sending');
        // Cannot view stats if the campaign has not been sent.
        if (in_array($campaign['status'], $disable_stats)) {
          $stats = $campaign['status'];
        }
        else {
          $stats = t('<a href="@url">View</a>',
            array(
              '@url' => url('admin/mailchimp/user_lists/' . $lid . '/campaign_stats/' . $campaign['id']),
            )
          );
        }
        // Cannot edit campaign after it has been sent.
        if (in_array($campaign['status'], $disable_edit)) {
          $edit = 'View';
        }
        else {
          $edit = 'Edit';
        }
        $rows[] = array(
          $campaign['title'],
          $stats,
          t(
            '<a href="@edit">@viewedit</a> |
              <a href="@delete">Delete</a> |
              <a href="@test">Test Send</a> | 
              <a href="@send">Send</a>',
              array(
                '@edit' => url('admin/mailchimp/user_lists/' . $lid . '/edit_campaign/' . $campaign['id']),
                '@delete' => url('admin/mailchimp/user_lists/' . $lid . '/delete_campaign/' . $campaign['id']),
                '@test' => url('admin/mailchimp/user_lists/' . $lid . '/test_send_campaign/' . $campaign['id']),
                '@send' => url('admin/mailchimp/user_lists/' . $lid . '/send_campaign/' . $campaign['id']),
                '@viewedit' => $edit,
              )
          ),
        );
      }
    }
  }
  if (isset($rows)) {
    $output = array(
      'header' => array(
        t('Campaign Name'),
        t('Campaign Status'),
        t('Actions'),
      ),
      'rows' => $rows,
    );
    return theme('table', $output);
  }
  else {
    return '<h2>No campaigns exist</h2>';
  }
}
/**
 * Function to generate form button to create new campaign on campaign table.
 *
 * @return array
 *   form data
 */
function mailchimp_user_list_campaign_table_form($form, &$form_state, $lid) {
  $form['mailchimp_create_campaign'] = array(
    '#type' => 'submit',
    '#value' => t('Create New Campaign'),
    '#submit' => array('mailchimp_user_list_new_campaign'),
  );
  $form['mailchimp_create_cid'] = array(
    '#type' => 'hidden',
    '#value' => $lid,
  );
  return $form;
}

/**
 * Wrapper function to redirect edit page for new campaigns.
 */
function mailchimp_user_list_new_campaign($form, &$form_state) {
  drupal_goto('admin/mailchimp/user_lists/' . $form_state['values']['mailchimp_create_cid'] . '/edit_campaign');
}

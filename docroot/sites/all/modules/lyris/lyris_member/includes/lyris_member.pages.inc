<?php
/**
 * @file
 * Data displays and basic output.
 */
/**
 * Overview page for subscription settings.
 */
function lyris_member_list_member_overview_page($list) {
  lyris_include('structs', 'lyris_member');
  lyris_include('forms', 'lyris_member');
  $out = theme('lyris_helper_tools', array('list' => $list));

  drupal_set_title(t('Members of @list', array('@list' => $list->title)));
  drupal_add_css(LYRIS_MOD_PATH . '/lyris.css');

  $lyris = lyris();
  if (!$lyris->isConnected()) {
    $out .= t('A connection to Lyris is required in order to manage members.');
    return $out;
  }

  // Get a list of local members for this list.
  $local = lyris_member_local_members($list->localVal('ListName'));

  // Get a list of remote members.
  $remote = $lyris->loadListMembers($list->localVal('ListName'),
                                    array('MemberID', 'EmailAddress', 'MemberType'));

  if ($remote === FALSE) {
    drupal_set_message(t('Unable to retrive remote members for this list.'), 'warning');
    $remote = array();
  }

  // @todo Check for a lyris connection before rendering this page.  We're
  // getting long requests when lyris is not available.  Ask IS to change the
  // response code instead of letting it timeout.

  // Add a count for total unsynced records.
  $unsynced = lyris_get_unsynced(array('entity_type' => 'lyris_member', llf('ListName') => $list->localVal('ListName')));
  $cnt = count($unsynced);

  if ($cnt == 0) {
    drupal_set_message(t('All imported members are in sync with Lyris.'), 'status');
  }
  else {
    drupal_set_message(t('There are %cnt members with local changes that should by synced to Lyris before delivering and mailings to this list.', array('%cnt' => $cnt)), 'warning');

    $out .= l(t('Sync List Members'), "lyris/sync/{$list->eid}", array('query' => drupal_get_destination()));
  }

  // Get a list of the local members who are waiting to be synced.
  $local_unsynced = array();

  // Count member summaries for each member status.
  $counts = array();
  foreach (lyris_member_enum('MemberStatusEnum') as $key => $label) {
    $counts[$key]['local'] = $counts[$key]['remote'] = 0;
    $counts[$key]['label'] = $label;
  }
  $total['local'] = $total['remote'] = 0;

  foreach ($local as $email => $record) {
    $counts[$record[llf('MemberStatus')]]['local']++;
    $total['local']++;
  }
  foreach ($remote as $email => $record) {
    $counts[$record['membertype']]['remote']++;
    $total['remote']++;
  }

  // Build table of member counts
  $rows = array();
  foreach ($counts as $status => $data) {
    if ($data['local'] == 0 && $data['remote'] == 0) {
      continue;
    }
    $row = array($data['label'], $data['local'], $data['remote']);
    $rows[] = $row;
  }

  // Add totals
  $rows[] = array(
    'data' => array(
      t('Total'),
      $total['local'],
      $total['remote'],
    ),
    'class' => array('totals'),
  );

  // Set table headers
  $header = array(t('Member Status'), t('Imported Members'), t('In Lyris'));

  /**
   * Build the output.
   */
  // Theme the summary table.
  $out .= theme('table', array('header' => $header, 'rows' => $rows, 'attributes' => array('id' => 'member-summary-table', 'class' => array('lyris-summary-table'))));

  // Add a mass-subscribe form
  $form = drupal_get_form('lyris_member_bulk_subscribe_form', $list->localVal('ListName'));
  $out .= drupal_render($form);

  return $out;
}

/**
 * Query and list members from a Lyris mailing list.
 *
 * If we ever move to a format where all members are imported into Drupal, this
 * could be replaced with a View of member entities.
 */
function lyris_member_list_member_list_page($list) {
  drupal_set_title(t('Members of @list', array('@list' => $list->title)));
  $out = theme('lyris_helper_tools', array('list' => $list));

  // Add a count for total unsynced records.
  $unsynced = lyris_get_unsynced(array('entity_type' => 'lyris_member', llf('ListName') => $list->localVal('ListName')));
  $cnt = count($unsynced);

  if ($cnt == 0) {
    drupal_set_message(t('All imported members are in sync with Lyris.'), 'status');
  }
  else {
    drupal_set_message(t('There are %cnt members with local changes that should by synced to Lyris before delivering and mailings to this list.', array('%cnt' => $cnt)), 'warning');

    $out .= l(t('Sync List Members'), "lyris/sync/{$list->eid}", array('query' => drupal_get_destination()));
  }

  lyris_include('forms', 'lyris_member');
  lyris_include('structs', 'lyris_member');
  $status  = lyris_member_enum('MemberStatusEnum');

  $lyris   = lyris('admin');
  $fields  = array('FullName', 'EmailAddress', 'MemberStatus', 'MemberID');
  $filters = array('ListName' => $list->localVal('ListName'));
  $range   = array(0, 50);

  // Filter by Member Status
  if (isset($_GET['MemberStatus']) && in_array($_GET['MemberStatus'], array_keys($status))) {
    $filters['MemberStatus'] = $_GET['MemberStatus'];
  }
  // Set the limit
  if (isset($_GET['limit']) && is_numeric($_GET['limit'])) {
    $range[1] = $_GET['limit'];
  }

  // Build form fields for filtering
  $form = drupal_get_form('lyris_member_detailed_member_list_filter_form');
  $out .= drupal_render($form);

  // Fetch results from Lyris
  $remotes = $lyris->loadListMembersRange($fields, $filters, $range);

  // Get the local results for comparison
  $locals = lyris_member_local_status($list->localVal('ListName'));

  // Merge local and lyris members into one array
  $members = array();
  foreach ($remotes as $remote) {
    $members[$remote['EmailAddress']] = array(
      'FullName' => $remote['FullName'],
      'EmailAddress' => $remote['EmailAddress'],
      'MemberID' => $remote['MemberID'],
      'remote_status' => $remote['MemberStatus'],
    );
  }

  // Allow local values to overwrite remote values if they are set.
  foreach ($locals as $EmailAddress => $local) {
    $members[$EmailAddress]['FullName'] = $local->FullName;
    $members[$EmailAddress]['EmailAddress'] = $local->EmailAddress;

    if (!isset($members[$EmailAddress]['MemberID'])) {
      $members[$EmailAddress]['MemberID'] = $local->MemberID;
    }

    $members[$EmailAddress]['local_status'] = $local->MemberStatus;
  }

  $form = drupal_get_form('lyris_member_detailed_member_list_form', $list, $members);
  $out .= drupal_render($form);

  return $out;
}

/**
 * User subscription management page.
 */
function lyris_member_user_page($account) {
  lyris_include('forms', 'lyris_member');
  $out = '';

  // @todo: Load an administrator's syncing button
  if (user_access('administer lyris subscriptions')) {

  }

  // Get member's memberships
  $open_lists = lyris_member_get_open_lists();

  // Load the lists
  $lists = array();
  foreach ($open_lists as $open_list) {
    $lists[$open_list[llf('ListName')]] = lyris_list_load($open_list[llf('ListName')]);
  }

  $form = drupal_get_form('lyris_member_list_opt_form', $lists, $account);
  $out .= drupal_render($form);

  return $out;
}

/**
 * Landing page for unsusbcribe links.
 *
 * @param $hash
 *   A string containing encoded information about the list and member to be
 *   unsubscribed.
 *
 * This page is publicly accessible so we need to consider various user states
 * when they arrive here.
 *
 * 1. Anonymous with a bad hash.
 * 2. Anonymous with a valid list but invalid memberid.
 * 3. Anonymous with a valid list and memberid.
 * 4. Authenticated user.
 */
function lyris_member_unsubscribe_page($hash) {
  global $user;
  $member = NULL;
  $list = NULL;

  // If the user is logged in, redirect them to their subscriptions page.
  if (user_is_logged_in()) {
    drupal_goto("user/{$user->uid}/lyris");
    exit;
  }

  // Decipher the hash
  // Just to be safe, make sure our hash is only letters and numbers.
  $hash = preg_replace('/[^a-z0-9]/i', '', strip_tags($hash));
  $listhash = substr($hash, 0, 32);
  $MemberID = substr($hash, 32);

  // Validate the list.  We're only unsubscribing from lists that we manage
  // from this site.
  $select = db_select(LYRIS_LIST_TABLE, 'll');
  $select->fields('ll', array('eid', llf('ListName')));
  $select->where('MD5(CONCAT(eid, ' . llf('ListName') . ')) = :hash', array(':hash' => $listhash));
  $res = $select->execute()->fetchAssoc();

  // We found a matching list.
  if ($res) {
    lyris_include('entities', 'lyris_member');

    $ListName = $res[llf('ListName')];
    $list_eid = $res['eid'];
    $list = lyris_list_load($list_eid);

    // Look for a local membership matching this ListName and MemberID.
    if ($MemberID) {
      if ($member = lyris_member_load_entity(array(llf('MemberID') => $MemberID))) {
        $member->updateStatus('unsub', FALSE);
        $status = LYRIS_MEMBER_UNSUB_SUCCESS;
      }
      else {
        // In order to push to Lyris we need at least a SimpleMemberStruct, which
        // includes an email address, so we'll need to import a membership to
        // properly process this request.
        $lyris = lyris('admin');
        $members = $lyris->loadListMembers($ListName, array(), array('MemberID' => $MemberID));

        if ($members) {
          foreach ($members as $membership) {
            $member = lyris_member();
            $member->mergeObjectData($membership);
            if ($member->save()) {
              $member->updateStatus('unsub', FALSE);
              $status = LYRIS_MEMBER_UNSUB_SUCCESS;
            }
            else {
              $status = LYRIS_MEMBER_UNSUB_ERROR;
            }
          }
        }
        else {
          $status = LYRIS_MEMBER_UNSUB_NOMEMBER;
        }
      }
    }
    else {
      $status = LYRIS_MEMBER_UNSUB_NOMEMBER;
    }
  }
  // List was not found.
  else {
    $status = LYRIS_MEMBER_UNSUB_NOLIST;
  }

  return theme('lyris_member_unsubscribe_page', array('status' => $status, 'list' => $list, 'member' => $member));
}

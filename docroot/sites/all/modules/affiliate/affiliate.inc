<?php

/**
 * Set the time of the current "clicks" update so the next time cron runs, only clicks newer than this
 * time will be added to each affiliate's running total.
 */
function affiliate_set_last_click_update() {
  $time = time();
  variable_del('affiliate_module_last_clicks_update');
  variable_set('affiliate_module_last_clicks_update', $time);
}

/**
 * Set the time of the current "referrals" update so the next time cron runs, only referrals newer than this
 * time will be added to each affiliate's running total.
 */
function affiliate_set_last_referrals_update() {
  $time = time();
  variable_del('affiliate_module_last_referrals_update');
  variable_set('affiliate_module_last_referrals_update', $time);
}

/**
 * Fetch the number of new referrals an affiliate has acquired since the last time cron was run.
 *
 * @param int $affid
 *   Affiliate (user) uid
 *
 * @return int
 *   The number of new referrals an affiliate has acquired since the last time cron was run.
 *   Will always return 0 if the Referral module is not installed.
 */
function affiliate_get_new_referrals_count_raw($affid) {
  $affid = (int) $affid;
  $num_referrals = 0;
  if (module_exists('referral')) {
    $lastupdate = variable_get('affiliate_module_last_referrals_update', 0);
    $num_referrals = db_query("SELECT COUNT(DISTINCT(r.uid)) AS referrals FROM {referral} r INNER JOIN {users} u ON r.uid = u.uid WHERE r.referral_uid = :affid AND r.created > :lastupdate AND u.status = 1", array(":affid" => $affid, ":lastupdate" => $lastupdate))->fetchField();
  }
  $out = (int) $num_referrals;
  return $out;
}

/**
 * Fetch the number of new clicks an affiliate has acquired since the last time cron was run.
 *
 * @param int $affid
 *   Affiliate (user) uid
 *
 * @return int
 *   The number of new clicks an affiliate has acquired since the last time cron was run.
 */
function affiliate_get_new_clicks_count_raw($affid) {
  $affid = (int) $affid;
  $lastupdate = variable_get('affiliate_module_last_clicks_update', 0);
  // Count cookie IDs? Or alter schema to have "click_id" as primary (serial) index?
  $newclicks = db_query("SELECT COUNT(DISTINCT(clickid)) AS newclicks FROM {affiliate_clicks} WHERE uid = :uid AND click_time > :lastupdate", array(":uid" => $affid, ":lastupdate" => $lastupdate))->fetchField();
  $out = (int) $newclicks;
  return $out;
}

/**
 * Copy data returned from affiliate_get_new_referrals_count_raw() into the
 * affiliate table, and set the last update time to the current time. Runs during cron.
 */
function affiliate_update_affiliate_referrals() {
  if (module_exists('referral')) {
    $affiliates = affiliate_fetch_all();
    $time = time();
    $lastupdate = variable_get('affiliate_module_last_referrals_update', 0);
    $cpr = variable_get('affiliate_module_payouts_per_referral', 0.00);
    if (!empty($affiliates)) {
      foreach ($affiliates as $affiliate) {
        $affid = $affiliate->uid;
        $newreferrals = affiliate_get_new_referrals_count_raw($affid);
        if ($newreferrals) {
          $new_payouts_owed = $newreferrals * $cpr;
          db_query("UPDATE {affiliate} SET referrals = referrals + :newreferrals, payouts_owed = payouts_owed + :new_payouts_owed WHERE uid = :uid AND active = 1", array(":newreferrals" => $newreferrals, ":new_payouts_owed" => $new_payouts_owed, ":uid" => $affid));
        }
      }
    }
    affiliate_set_last_referrals_update();
  }
}

/**
 * Copy data from the affiliate_clicks table into the affiliate table, and set the last
 * update time to the current time. Runs during cron.
 */
function affiliate_update_affiliate_clicks() {
  $affiliates = affiliate_fetch_all();
  $time = time();
  $cpc = variable_get('affiliate_module_payouts_per_click', 0.00);
  if (!empty($affiliates)) {
    foreach ($affiliates as $affiliate) {
      $affid = $affiliate->uid;
      $newclicks = affiliate_get_new_clicks_count_raw($affid);
      if ($newclicks) {
        $new_payouts_owed = $newclicks * $cpc;
        db_query("UPDATE {affiliate} SET clicks = clicks + :newclicks, payouts_owed = payouts_owed + :new_payouts_owed WHERE uid = :uid AND active = 1", array(":newclicks" => $newclicks, ":new_payouts_owed" => $new_payouts_owed, ":uid" => $affid));
      }
    }
  }
  affiliate_set_last_click_update();
}

/**
 * Takes all existing nodes of the selected types configured at admin/config/people/affiliate/content
 * and makes them "valid" endpoints for which affiliates will receive click-thru credit.
 */
function affiliate_bulk_create_node_campaigns() {
  $types_to_add = variable_get('affiliate_module_allowed_node_types', array());
  if (!empty($types_to_add)) {
    foreach ($types_to_add as $type) {
      $nids = db_query("SELECT IFNULL(aff.eid, n.nid) as nid FROM {affiliate_campaigns} aff RIGHT JOIN {node} n ON aff.eid = n.nid ORDER BY n.nid ASC")->fetchAll();
      for ($i = 0; $i < count($nids); $i++) {
        $nodeids[] = $nids[$i]->nid;
      }
      foreach ($nodeids as $nid) {
        $nid = (int) $nid;
        $campaign_exists = db_query("SELECT cid FROM {affiliate_campaigns} WHERE {affiliate_campaigns}.eid = :eid AND {affiliate_campaigns}.etype = 'node'", array(":eid" => $nid))->fetchField();
        if (!$campaign_exists || empty($campaign_exists)) {
          $created = time();
          $redirect_path = drupal_get_normal_path('node/' . $nid);
          $data = array();
          $data = serialize($data);
          db_insert('affiliate_campaigns')
            ->fields(array(
              'redirect_path' => $redirect_path,
              'ctype' => 1,
              'eid' => $nid,
              'etype' => 'node',
              'status' => 1,
              'created' => $created,
              'expires' => 0,
              'data' => $data,
            ))
            ->execute();
        }
      }
    }
  }
}

/**
 * Fetch an affiliate object.
 *
 * @param int $affid
 *   Affiliate ID (users.uid)
 *
 * @return object
 *   If affiliate exists, affiliate info (uid, active, created, clicks, referrals,
 *   payouts_owed, payouts_paid). Otherwise, empty.
 *   
 */
function affiliate_load_affiliate($affid) {
  $affid = (int) $affid;
  $res = db_query("SELECT * FROM {affiliate} WHERE {affiliate}.uid = :affid", array(":affid" => $affid));
  $affiliate = $res->fetchObject();
  return $affiliate;
}

/**
 * Add a new affiliate.
 *
 * @param int $uid
 *   User ID of the affiliate being created.
 *
 * @param bool $status
 *   (Optional) If affiliate is to be active, set $status to 1 (default). Otherwise,
 *   setting $status to 0 will still create the affiliate but the affiliate will not be active.
 *
 */
function affiliate_insert_affiliate($uid, $status = 1) {
  $uid = (int) $uid;
  $active = 0;
  if ($status) {
    $active = 1;
  }
  $time = time();
  db_insert('affiliate')
    ->fields(array(
      'uid' => $uid,
      'active' => $active,
      'created' => $time,
    ))
    ->execute();
}

/**
 * Update an affiliate's status.
 *
 * @param int $uid
 *   User ID of the affiliate whose status is being changed.
 *
 * @param bool $status
 *   1 = Active, 0 = Inactive
 */
function affiliate_set_affiliate_status($uid, $status) {
  $uid = (int) $uid;
  $active = 0;
  if ($status) {
    $active = 1;
  }
  db_update('affiliate')
    ->condition('uid', $uid)
    ->fields(array(
      'active' => $active,
    ))
    ->execute();
}

/**
 * Fetch all info for every affiliate account.
 *
 *  @return array
 *    An associative array of all affiliate users and their affiliation details.
 */
function affiliate_fetch_all() {
  $out = array();
  if ($aff_ids = db_query("SELECT * FROM {affiliate} ORDER BY {affiliate}.uid ASC")->fetchAll()) {
    $out = $aff_ids;
  }
  return $out;
}

/**
 * Attempt to make sure an incoming click was made validly.
 *
 * @param int $affid
 *   Affiliate (user) ID of the affiliate who should receive the credit for this click.
 *
 * @param string $dest
 *   The destination, formatted as a relative path, that the user should be redirected to upon validation.
 *
 * @param bool $verify_node_type
 *   (Optional) Verify that the node type to which the user is being redirected is one
 *   of the types allowed as set at admin/config/people/affiliate/content. 
 *
 */
function affiliate_validate_click($affid, $dest, $verify_node_type = TRUE) {
  $affid = (int) $affid;
  $dest = check_url($dest);
  $node_path_pattern = '<^(node/)([0-9]+)$>';
  $is_node_path = preg_match($node_path_pattern, $dest, $node_path_matches);
  $nid = 0;
  if (isset($node_path_matches) && !empty($node_path_matches) && isset($node_path_matches[2]) && $node_path_matches[2] > 0) {
    // filter_xss() necessary?
    $nid = (int) filter_xss($node_path_matches[2]);
  }
  $allow_all_types = variable_get('affiliate_module_allow_all_node_types', 0);
  $allow_all_paths = variable_get('affiliate_module_allow_all_paths', 0);
  if ($allow_all_paths) {
    if (drupal_valid_path($dest) && !url_is_external($dest)) {
      return TRUE;
    }
  }
  if ($allow_all_types || $verify_node_type) {
    if ($is_node_path && $nid) {
      if ($allow_all_types && drupal_valid_path($dest) && !url_is_external($dest)) {
        return TRUE;
      }
      if ($verify_node_type) {
        $node = node_load($nid);
        $type = $node->type;
        $allowed_node_types = variable_get('affiliate_module_allowed_node_types', array());
        if (!in_array($type, (array) $allowed_node_types)) {
          return FALSE;
        }
      }
    }
  }
  if (!affiliate_valid_destination($dest)) {
    return FALSE;
  }
  if (!affiliate_referrer_exists()) {
    return FALSE;
  }
  if (!affiliate_validate_uid($affid)) {
    return FALSE;
  }
  return TRUE;
}

/**
 * Verify that an affiliate destination path exists, is not external, and is not administrative in nature.
 *
 * @param string $dest
 *   The destination, formatted as a relative path, that the user should be redirected to upon validation.
 *
 * @return bool
 *   TRUE if $dest is a valid affiliate path, otherwise FALSE.
 */
function affiliate_valid_destination($dest) {
  if (!drupal_valid_path($dest)) {
    return FALSE;
  }
  if (url_is_external($dest)) {
    return FALSE;
  }
  if (path_is_admin($dest)) {
    return FALSE;
  }
  return TRUE;
}


/**
 * Check to see if the $_SERVER['HTTP_REFERER'] requirements, if any, have been met,
 * insofar as existence is concerned.
 *
 * @return bool
 *   FALSE if referral source is not set, provided the system has not been told to ignore
 *   this setting. Otherwise TRUE.
 */
function affiliate_referrer_exists() {
  if (variable_get('affiliate_module_check_referrer', 1)) {
    if (!isset($_SERVER['HTTP_REFERER']) || empty($_SERVER['HTTP_REFERER'])) {
      return FALSE;
    }
  }
  return TRUE;
}

/**
 * Alias function for people who might forget that REFERER is not the correct spelling of
 * the word REFERRER. ;)
 */
function affiliate_referer_exists() {
  return affiliate_referrer_exists();
}

/**
 * Verify if a node should display an affiliate link below it.
 *
 * @param object $node
 *   The node.
 *
 * @return bool
 *   TRUE if node is eligible, otherwise FALSE.
 */
function affiliate_node_is_eligible($node) {
  if (!$node->status) {
    return FALSE;
  }
  $allow_all_types = variable_get('affiliate_module_allow_all_node_types', 0);
  $allow_all_paths = variable_get('affiliate_module_allow_all_paths', 0);
  if ($allow_all_paths || $allow_all_types) {
    return TRUE;
  }
  $nid = $node->nid;
  $type = $node->type;
  $allowed_node_types = variable_get('affiliate_module_allowed_node_types', array());
  if (in_array($type, (array) $allowed_node_types)) {
    return TRUE;
  }
  return FALSE;
}

/**
 * Generate a path for each affiliate to use based on the current path and their user id.
 *
 * @param int $uid
 *   The {users}.uid of the affiliate. If $uid is not provided or is set to NULL, the
 *   value will default to the current user's {users}.uid.
 *
 * @return string
 *   A path which can be used to generate a clickable URL with which an affiliate can
 *   receive credit.
 *
 * @see affiliate_fetch_url()
 * @see affiliate_validate_click()
 * @see affiliate_node_markup()
 */
function affiliate_fetch_path($uid = NULL) {
  if (!empty($uid) && !is_null($uid)) {
    $user_id = $uid;
  }
  else {
    $user_id = $GLOBALS['user']->uid;
  }
  $user_id = (int) $user_id;
  $current_path = current_path();
  $dest_path = str_replace('/', '|', $current_path);
  $menu_prefix = variable_get('affiliate_module_affiliate_menu_path', 'affiliate');
  $affiliate_path = $menu_prefix . '/' . $user_id . '/' . $dest_path;
  $out = check_url($affiliate_path);
  return $out;
}

/**
 * Generate a url for each affiliate to use based on the current path and their user id.
 *
 * @param int $uid
 *   The {users}.uid of the affiliate. If $uid is not provided or is set to NULL, the
 *   value will default to the current user's {users}.uid.
 *
 * @return string
 *   A URL which, when clicked by an outside party, will give the affiliate credit for
 *   a click-thru.
 *
 * @see affiliate_fetch_path()
 * @see affiliate_validate_click()
 * @see affiliate_node_markup()
 */
function affiliate_fetch_url($uid = NULL) {
  global $base_url;
  if (!empty($uid) && !is_null($uid)) {
    $user_id = $uid;
  }
  else {
    $user_id = $GLOBALS['user']->uid;
  }
  $user_id = (int) $user_id;
  $path = affiliate_fetch_path($user_id);
  $url_raw = $base_url . base_path() . $path;
  $url = check_url($url_raw);
  $out = url($url);
  return $out;
}

/**
 * Attempts to fetch a campaign ID based on the node nid. Will not always work when, for
 * instance, all paths are allowed as affiliate paths.
 *
 * @return
 *   {node}.nid if found, otherwise 0.
 */
function affiliate_get_campaign_id_by_nid($nid) {
  $nid = (int) $nid;
  $cid = db_query("SELECT cid FROM {affiliate_campaigns} WHERE eid = :nid AND etype = 'node'", array(":nid" => $nid))->fetchField();
  $out = ($cid && !empty($cid)) ? (int) $cid : 0;
  return $out;
}

/**
 * Attempts to fetch a campaign ID based on the visited path. Will not always work when, for
 * instance, all paths are allowed as affiliate paths.
 *
 * @return
 *   Campaign ID if found, otherwise 0.
 */
function affiliate_get_campaign_id($path) {
  $path = check_url(rawurldecode($path));
  $cid = db_query("SELECT ac.cid FROM {affiliate_campaigns} ac WHERE ac.redirect_path = :path", array(":path" => $path))->fetchField();
  $out = ($cid && !empty($cid)) ? (int) $cid : 0;
  return $out;
}

/**
 * Record a click-thru, giving credit to the specified affiliate ID.
 *
 * @param int $affid
 *   The {affiliate}.uid of the affiliate
 *
 * @param int $campaign_id
 *   The {affiliate_campaigns}.cid of the specified campaign, if any.
 *
 * @return void
 *   Credit for the click applied to the affiliate account if validation attempts are passed.
 *
 * @see affiliate_click_denied()
 */
function affiliate_add_click($affid, $campaign_id = 0) {
  $cookie_value = md5(microtime());
  $timeout_flag = true;
  $affid = (int) $affid;
  $campaign_id = (int) $campaign_id;
  $time = time();
  if ($affid) {
    $affiliate_click_denied = affiliate_click_denied($affid);
    if ($_COOKIE && array_key_exists('AFFILIATE', $_COOKIE) && !$_COOKIE['AFFILIATE']) {
      setcookie('AFFILIATE', $cookie_value, REQUEST_TIME + 86400);
    }
    // Only record the click-thru if it looks like this isn't a cheater and
    // if the user who clicked has the permission to have her click counted.
    if (!$affiliate_click_denied && user_access('track affiliate clicks for this role')) {
      db_insert('affiliate_clicks')
        ->fields(array(
          'uid' => $affid,
          'cid' => $campaign_id,
          'click_time' => $time,
          'cookie_id' => $cookie_value,
          'ip' => ip_address(),
          'referrer' => $_SERVER['HTTP_REFERER'],
      ))
      ->execute();
      // If the Userpoints module exists, add points as configured at admin/config/people/affiliate/payouts
      if (module_exists('userpoints')) {
        $points = (int) variable_get('affiliate_module_payouts_userpoints_clicks', 0);
        $params = array(
    	  'points' => $points,
          'uid' => $affid,
          'operation' => 'affiliate click',
          'tid' => variable_get('affiliate_module_payouts_userpoints_tid', 0),
          'reference' => 'affiliate'
        );
        userpoints_userpointsapi($params);
      }
      // Allow the Referral module to set its cookie
      if (module_exists('referral')) {
        referral_set_cookie($affid);
      }
    }
  }
}

function affiliate_validate_uid($affid = 0) {
  global $user;
  $affid = (int) $affid;
  $uid = (int) $user->uid;
  $valid = TRUE;
  // Fail if affiliate ID from the URL is the same as the currently logged-in user ID.
  if ($affid == $uid) {
    $valid = FALSE;
  }
  // Fail if affiliate ID is not set or is set to 0 (which, essentially, means that it's not set).
  if (!isset($affid) || empty($affid) || $affid < 1) {
    $valid = FALSE;
  }
  return $valid;
}

function affiliate_click_denied($uid) {
  $account = user_load($uid);
  $denied_users = explode("\n", variable_get('affiliate_module_ignored_users', ''));
  if (in_array($account->name, $denied_users)) {
    return TRUE;
  }
  $ip = ip_address();
  $denied_ips = explode("\n",  variable_get('affiliate_module_ignored_ips', ''));
  if (in_array($ip, $denied_ips)) {
    return TRUE;
  }
  if (!affiliate_referrer_exists()) {
    return TRUE;
  }
  $too_soon = _affiliate_check_timer();
  if ($too_soon) {
    return TRUE;
  }
  $denied_uri_referrers = explode("\n", variable_get('affiliate_module_ignored_uri_referrers', ''));
  if (count($denied_uri_referrers)) {
    foreach ($denied_uri_referrers as $referrer) {
      if (!empty($referrer) && strpos($_SERVER['HTTP_REFERER'], $referrer)) {
        return TRUE;
      }
    }
  }
  return FALSE;
}

function _affiliate_check_timer() {
  $enabled = variable_get('affiliate_module_click_ignore_enable', 0);
  if (!$enabled) {
    return FALSE;
  }
  $ip = ip_address();
  // Default time limit allows for one click per IP per day.
  $time = time();
  $interval = variable_get('affiliate_module_click_ignore_interval', 86400);
  $limit = $time - $interval;
  return (bool) db_query("SELECT COUNT(*) FROM {affiliate_clicks} WHERE ip = :ip AND click_time > :limit", array(":ip" => $ip, ":limit" => $limit))->fetchField();
}

/**
 * @todo
 *   Have this function checked by security team if possible, or if not, get a second opinion just to be sure.
 */
function affiliate_page() {
  $allow_all_paths = variable_get('affiliate_module_allow_all_paths', 0);
  $allow_all_node_types = variable_get('affiliate_module_allow_all_node_types', 0);

  $affid = (int) filter_xss(arg(1));
  $path_given = arg(2);
  $tracker = (int) filter_xss(arg(3));

  // Note to self: ask a security guru about this.
  $path_arg = check_url(implode('/', explode('|', rawurldecode($path_given))));

  $verify_node_type = FALSE;
  if (!$allow_all_node_types) {
    $verify_node_type = TRUE;
  }
  if ($allow_all_paths) {
    $verify_node_type = FALSE;
  }

  if (!isset($tracker) || empty($tracker)) {
    $tracker = 0;
  }
  if (!isset($affid) || empty($affid)) {
    $affid = 1;
  }

  // Send invalid clicks to the front page if no path argument is supplied.
  if (!isset($path_arg) || empty($path_arg)) {
    $path = base_path();
  }
  else {
    $path = $path_arg;
  }
  // Path aliases will fail validation... Convert to normal path before validating.
  $path_normal = drupal_get_normal_path($path);

  // This seems like it could be written better...?
  if (affiliate_validate_click($affid, $path_normal, $verify_node_type)) {
    $campaign_id = affiliate_get_campaign_id($path_normal);
    affiliate_add_click($affid, $campaign_id);
  }
  $destination_is_valid = affiliate_valid_destination($path_normal);

  // Empty will simply take us to the front page, in case the path doesn't validate.
  $destination_actual = '';
  if ($destination_is_valid) {
    $destination_actual = drupal_get_path_alias($path_normal);
  }

  // Unnecessary check_url()? Better safe than sorry...
  $destination_actual = check_url($destination_actual);

  // At minimum, if all validations fail, we'll be taken to the front page. If the path
  // given is valid though, we'll be taken to that path either way, regardless of whether
  // the affiliate was credited with a valid click-thru.
  drupal_goto($destination_actual);
}

function affiliate_user_page($account) {
  global $user;
  $uid = $user->uid;
  $acctid = $account->uid;
  if (($uid == $acctid && user_access('view own affiliate info')) || user_access('administer affiliate settings')) {
    $name = $account->name;
    $out = affiliate_user_page_output($acctid);
    drupal_set_title(t('Affiliate information for @name', array('@name' => $name)));
    return $out;
  }
  else {
    drupal_access_denied();
  }
}

function affiliate_admin_overviewpage() {
  $out = affiliate_admin_overviewpage_output();
  $css = '.affiliate-payout-inlineform-amount, .affiliate-payout-inlineform-submit {display: inline-block;}';
  drupal_add_css($css, 'inline');
  drupal_set_title(t('Affiliate management: overview'));
  return $out;
}

function affiliate_user_page_output($acctid) {
  $out = affiliate_user_page_output_top($acctid);
  $out += affiliate_user_page_output_details($acctid);
  return $out;
}

function affiliate_user_page_output_details($acctid) {
  $header = array(
    array('data' => t('Date'), 'field' => 'click_time', 'sort' => 'desc'),
    array('data' => t('Referrer'), 'field' => 'referrer'),
  );

  $count_query = db_select('affiliate_clicks')
    ->condition('uid', $acctid);
  $count_query->addExpression('COUNT(DISTINCT(clickid))');

  $query = db_select('affiliate_clicks', 'a')->extend('PagerDefault')->extend('TableSort')
    ->condition('uid', $acctid)
    ->fields('a', array('click_time', 'referrer'))
    ->limit(30)
    ->orderByHeader($header);
  $query->setCountQuery($count_query);
  $result = $query->execute();

  $rows = array();
  foreach ($result as $res) {
    $click_time = format_date($res->click_time);
    $referrer = url($res->referrer);
    $rows[] = array($click_time, $referrer);
  }

  $build['affiliate_bottom_table']  = array(
    '#theme' => 'table',
    '#header' => $header,
    '#rows' => $rows,
    '#empty' => t('No click information available.'),
  );

  return $build;
}

function affiliate_user_page_output_top($acctid) {
  $header = array(
    array('data' => t('Active')),
    array('data' => t('Affiliate since')),
    array('data' => t('Clicks')),
    array('data' => t('Referrals')),
    array('data' => t('Payouts owed')),
    array('data' => t('Payouts paid'))
  );
  $payouts_symbol = variable_get('affiliate_module_payouts_symbol', '$');
  $payouts_symbol_pos = variable_get('affiliate_module_payouts_symbol_placement', 1);
  $payouts_amount = variable_get('affiliate_module_payouts_per_click', 0.00);

  $query = db_select('affiliate', 'a')
    ->condition('uid', $acctid)
    ->fields('a', array('uid', 'active', 'created', 'clicks', 'referrals', 'payouts_owed', 'payouts_paid'));
  $result = $query->execute();

  $rows = array();
  foreach ($result as $affiliate) {
    $affiliate_uid = $affiliate->uid;
    $affiliate_name = user_load($affiliate_uid)->name;
    $affiliate_uid_display =  l(t('@uid - @name', array('@uid' => $affiliate_uid, '@name' => $affiliate_name)), drupal_get_path_alias('user/' . $affiliate_uid));
    $affiliate_active = $affiliate->active ? t('Yes') : t('No');
    $affiliate_created = format_date($affiliate->created);
    $payouts_owed_display = $payouts_symbol . $affiliate->payouts_owed;
    $payouts_paid_display = $payouts_symbol . $affiliate->payouts_paid;
    if ($payouts_symbol_pos == 2) {
      $payouts_owed_display = $affiliate->payouts_owed . $payouts_symbol;
      $payouts_paid_display = $affiliate->payouts_paid . $payouts_symbol;
    }
    $rows[] = array($affiliate_active, $affiliate_created, $affiliate->clicks, $affiliate->referrals, $payouts_owed_display, $payouts_paid_display);
  }

  $build['affiliate_top_table']  = array(
    '#theme' => 'table',
    '#header' => $header,
    '#rows' => $rows,
    '#empty' => t('No affiliate information available.'),
  );

  return $build;
}

function affiliate_admin_overviewpage_output() {
  $header = array(
    array('data' => t('ID'), 'field' => 'uid'),
    array('data' => t('Name'), 'field' => 'name'),
    array('data' => t('Active'), 'field' => 'active'),
    array('data' => t('Created'), 'field' => 'created', 'sort' => 'desc'),
    array('data' => t('Clicks'), 'field' => 'clicks'),
    array('data' => t('Referrals'), 'field' => 'referrals'),
    array('data' => t('Payouts owed'), 'field' => 'payouts_owed'),
    array('data' => t('Payouts paid'), 'field' => 'payouts_paid'),
    array('data' => '', 'field' => NULL),
  );
  $payouts_symbol = variable_get('affiliate_module_payouts_symbol', '$');
  $payouts_symbol_pos = variable_get('affiliate_module_payouts_symbol_placement', 1);
  $payouts_amount = variable_get('affiliate_module_payouts_per_click', 0.00);
  $count_query = db_select('affiliate');
  $count_query->addExpression('COUNT(DISTINCT(uid))');

  $query = db_select('affiliate', 'a')->extend('PagerDefault')->extend('TableSort');
  $query->innerjoin('users', 'u', 'a.uid = u.uid');
  $query = $query
    ->fields('a', array('uid', 'active', 'created', 'clicks', 'referrals', 'payouts_owed', 'payouts_paid'))
    ->fields('u', array('name'))
    ->groupBy('a.uid')
    ->limit(30)
    ->orderByHeader($header);
  $query->setCountQuery($count_query);
  $result = $query->execute();

  $rows = array();
  foreach ($result as $affiliate) {
    $affiliate_uid = $affiliate->uid;
    $affiliate_name = check_plain($affiliate->name);
    $affiliate_uid_display =  l($affiliate_uid, drupal_get_path_alias('user/' . $affiliate_uid));
    $affiliate_name_display =  l($affiliate_name, drupal_get_path_alias('user/' . $affiliate_uid));
    $affiliate_active = $affiliate->active ? t('Yes') : t('No');
    $affiliate_created = format_date($affiliate->created);
    $payouts_owed_display = $payouts_symbol . $affiliate->payouts_owed;
    $payouts_paid_display = $payouts_symbol . $affiliate->payouts_paid;
    $payouts_form = '';
    if ($payouts_symbol_pos == 2) {
      $payouts_owed_display = $affiliate->payouts_owed . $payouts_symbol;
      $payouts_paid_display = $affiliate->payouts_paid . $payouts_symbol;
    }
    if (user_access('administer affiliate settings') || user_access('administer affiliate payouts')) {
      $payouts_form .= drupal_render(drupal_get_form('affiliate_payout_inlineform', $affiliate_uid));
    }
    $rows[] = array($affiliate_uid_display, $affiliate_name_display, $affiliate_active, $affiliate_created, $affiliate->clicks, $affiliate->referrals, $payouts_owed_display, $payouts_paid_display, $payouts_form);
  }

  $build['affiliate_top_table']  = array(
    '#theme' => 'table',
    '#header' => $header,
    '#rows' => $rows,
    '#empty' => t('No affiliate information available.'),
  );
  $build['affiliate_top_pager'] = array('#theme' => 'pager');

  return $build;
}

function affiliate_admin_userspage() {
  $out = drupal_get_form('affiliate_admin_usersform');
  drupal_set_title(t('Affiliate management: users'));
  return $out;
}

function affiliate_admin_contentpage() {
  $out = drupal_get_form('affiliate_admin_contentform');
  drupal_set_title(t('Affiliate management: content'));
  return $out;
}

function affiliate_admin_payoutspage() {
  $out = drupal_get_form('affiliate_admin_payoutsform');
  drupal_set_title(t('Affiliate management: payouts'));
  return $out;
}

function affiliate_admin_usersform() {
   $form['affiliate_module_settings']['configuration']['limits'] = array(
    '#type' => 'fieldset',
    '#title' => t('Restrictions and limitations'),
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
  );
  $form['affiliate_module_settings']['configuration']['limits']['affiliate_module_allow_all_users'] = array(
    '#type' => 'radios',
    '#title' => t('All users are affiliates'),
    '#default_value' => variable_get('affiliate_module_allow_all_users', 0),
    '#options' => array(0 => t('No'), 1 => t('Yes')),
    '#description' => t('If selected, anyone with an active account on this site will receive credit for referring an outside user to any path as configured on the <a href="@content_settings_form">content settings form</a>.', array('@content_settings_form' => base_path() . 'admin/config/people/affiliate/content')),
  );
  $form['affiliate_module_settings']['configuration']['limits']['affiliate_module_check_referrer'] = array(
    '#type' => 'radios',
    '#title' => t('Always check for referring URL'),
    '#default_value' => variable_get('affiliate_module_check_referrer', 1),
    '#options' => array(0 => t('No'), 1 => t('Yes')),
    '#description' => t('If enabled, affiliates will not receive credit unless the user who visits the link first clicks on a link from another website. In other words, if the visitor types the URL directly into their browser instead of clicking on a link somewhere, they will still be taken to the destination page but the affiliate whose link they visited will not receive credit for that visit. Enabled by default.'),
  );
  $form['affiliate_module_settings']['configuration']['limits']['affiliate_module_click_ignore_enable'] = array(
    '#type' => 'radios',
    '#title' => t('Enable IP click ignore'),
    '#default_value' => variable_get('affiliate_module_click_ignore_enable', 0),
    '#options' => array(0 => t('Disable'), 1 => t('Enable')),
    '#description' => t('If you wish to ignore clicks from the same IP address within a given time period, enable this option.'),
  );
  $form['affiliate_module_settings']['configuration']['limits']['affiliate_module_click_ignore_interval'] = array(
    '#type' => 'select',
    '#title' => t('IP click ignore time limit'),
    '#default_value' => variable_get('affiliate_module_click_ignore_interval', 86400),
    '#options' => drupal_map_assoc(array(3600, 10800, 21600, 32400, 43200, 86400, 172800, 259200, 604800, 1209600, 2419200), 'format_interval'),
    '#description' => t('Ignore clicks from the same IP address for this time interval.'),
    '#states' => array(
      'invisible' => array(
        ':input[name="affiliate_module_click_ignore_enable"]' => array('value' => 0),
      ),
    ),
  );
  $form['affiliate_module_settings']['configuration']['limits']['affiliate_module_ignored_uri_referrers'] = array(
    '#type' => 'textarea',
    '#title' => t('Ignore clicks referred by these URLs'),
    '#default_value' => variable_get('affiliate_module_ignored_uri_referrers', ''),
    '#size' => 70,
    '#rows' => 4,
    '#description' => t('List all referring URL sources, each on a new line, that will be ignored by the affiliate system if a click-thru originates from that source.'),
  );
  $form['affiliate_module_settings']['configuration']['limits']['affiliate_module_ignored_ips'] = array(
    '#type' => 'textarea',
    '#title' => t('Ignore clicks originating from these IP addresses'),
    '#default_value' => variable_get('affiliate_module_ignored_ips', ''),
    '#size' => 70,
    '#rows' => 4,
    '#description' => t('List all IP addresses, each on a new line, that will be ignored by the affiliate system when the IP address of the user who has clicked through matches any of the IP addresses on this list.'),
  );
  $form['affiliate_module_settings']['configuration']['limits']['affiliate_module_ignored_users'] = array(
    '#type' => 'textarea',
    '#title' => t('Denied by User'),
    '#default_value' => variable_get('affiliate_module_ignored_users', ''),
    '#size' => 70,
    '#rows' => 4,
    '#description' => t('List all user names, each on a new line, that will be ignored by the affiliate system if a click-thru originates from a user with that same user name. NOTE: currently logged-in users who click on their own links will be ignored automatically, regardless of whether their name appears in this or not.'),
  );
  $form['affiliate_module_settings']['configuration']['top_users'] = array(
    '#type' => 'fieldset',
    '#title' => t('Top Users'),
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
  );
  $form['affiliate_module_settings']['configuration']['top_users']['affiliate_module_top_users_count_page'] = array(
    '#type' => 'select',
    '#title' => t('Top Users Count For Page'),
    '#default_value' => variable_get('affiliate_module_top_users_count_page', 5),
    '#options' => array(5 => 5, 10 => 10, 25 => 25, 50 => 50, 100 => 100),
    '#description' => t('Number of users to show in top users page.'),
  );
  $form['affiliate_module_settings']['configuration']['top_users']['affiliate_module_top_users_count_block'] = array(
    '#type' => 'select',
    '#title' => t('Top Users Count For Block'),
    '#default_value' => variable_get('affiliate_module_top_users_count_block', 5),
    '#options' => array(5 => 5, 10 => 10, 25 => 25, 50 => 50, 100 => 100),
    '#description' => t('Number of users to show in top users block.'),
  );
  $form['affiliate_module_settings']['configuration']['top_users']['affiliate_module_top_users_period_interval'] = array(
    '#type' => 'select',
    '#title' => t('Period for top users block and page'),
    '#default_value' => variable_get('affiliate_module_top_users_period_interval', 259200),
    '#options' => drupal_map_assoc(array(86400, 172800, 259200, 259200, 432000, 604800, 1209600, 2592000), 'format_interval'),
    '#description' => t('How long of a timespan will be used as the limit for what is shown in the top users page?'),
  );
  return system_settings_form($form['affiliate_module_settings']);
}

function affiliate_admin_contentform() {
  $node_types = node_type_get_names();
  $allow_all_options = array(0 => t('No'), 1 => t('Yes'));
  $form['affiliate_module_affiliate_menu_path'] = array(
    '#type' => 'textfield',
    '#title' => t('Click-thru menu path'),
    '#default_value' => variable_get('affiliate_module_affiliate_menu_path', 'affiliate'),
    '#description' => t('Many pieces of (good) software exist that will essentially block anything from being displayed inside a browser if the thing being viewed comes from a link that contains words like <em>affiliate</em>, <em>click</em>, <em>partner</em>, <em>ad</em>, etc. This setting allows the menu paths for affiliate click-thru links to be set to something other than the word <em>affiliate</em>.'),
    '#required' => TRUE,
  );
  $form['affiliate_module_allow_all_paths'] = array(
    '#type' => 'radios',
    '#title' => t('Allow all paths'),
    '#default_value' => variable_get('affiliate_module_allow_all_paths', 0),
    '#options' => $allow_all_options,
    '#description' => t('Select whether affiliates will be credited for referring anyone to any URL from this site (i.e. if an affiliate refers someone to the contact page, they will receive credit for that).'),
  );
  $form['affiliate_module_allow_all_node_types'] = array(
    '#type' => 'radios',
    '#title' => t('Allow all content types'),
    '#default_value' => variable_get('affiliate_module_allow_all_node_types', 0),
    '#options' => $allow_all_options,
    '#description' => t('Select whether affiliates will be credited for referring anyone to any piece of content, regardless of its type. Note: a "piece of content", in this case and most others, is a node (i.e. the contact page, any custom views, etc, are not nodes).'),
    '#states' => array(
      'invisible' => array(
        ':input[name="affiliate_module_allow_all_paths"]' => array('value' => 1),
      ),
    ),
  );
  $form['affiliate_module_allowed_node_types'] = array(
    '#type' => 'checkboxes',
    '#title' => t('Allowed content types'),
    '#default_value' => variable_get('affiliate_module_allowed_node_types', array()),
    '#options' => $node_types,
    '#description' => t('Select which content types should be automatically available to all affiliates. Each new node will be available when cron is run.'),
    '#states' => array(
      'visible' => array(
        ':input[name="affiliate_module_allow_all_paths"]' => array('value' => 0),
        ':input[name="affiliate_module_allow_all_node_types"]' => array('value' => 0),
      ),
    ),
  );
  $form['affiliate_module_affiliate_link_text'] = array(
    '#type' => 'textfield',
    '#title' => t('Instructional text'),
    '#default_value' => variable_get('affiliate_module_affiliate_link_text', t('Copy the code below and paste it into your website')),
    '#description' => t('Text instructing users to copy the text in the affiliate link box and paste it into their website in order to earn affiliate credit.'),
  );
  $form['affiliate_module_flush_all_caches_on_admin_content_form_submit'] = array(
    '#type' => 'radios',
    '#title' => t('Flush all caches and rebuild menus after submitting this form'),
    '#default_value' => 0,
    '#options' => array(0 => t('No'), 1 => t('Yes')),
    '#description' => t('If you changed the click-thru menu path above, it is highly advised that you flush all the caches and rebuild all the menus after submitting this form. Otherwise there is a very good chance that your affiliate links will break because the system will not know that the click-thru menu path has changed.'),
    '#required' => TRUE,
  );
  $form['#submit'][] = 'affiliate_admin_contentform_submit';
  return system_settings_form($form);
}

function affiliate_admin_contentform_validate($form, &$form_state) {
  $menu_path_submitted = $form_state['values']['affiliate_module_affiliate_menu_path'];
  $menu_path_current = variable_get('affiliate_module_affiliate_menu_path', 'affiliate');
  $menu_path_is_dir_or_file = (is_dir(DRUPAL_ROOT . '/' . $menu_path_submitted) || file_exists(DRUPAL_ROOT . '/' . $menu_path_submitted)) ? TRUE : FALSE;
  $pattern = '~^([a-zA-Z0-9_-]+)$~';
  // Remove? Change?
  if (!preg_match($pattern, $menu_path_submitted)) {
    form_set_error('affiliate_module_affiliate_menu_path', t('The affiliate menu path must only contain alphanumeric characters, underscores, and/or dashes.'));
  }
  // Make sure the submitted path does not already exist
  if (drupal_valid_path($menu_path_submitted) && $menu_path_submitted != $menu_path_current) {
    form_set_error('affiliate_module_affiliate_menu_path', t('This path already exists and cannot be used as the affiliate menu path. Please choose a different path.'));
  }
  if ($menu_path_is_dir_or_file) {
    form_set_error('affiliate_module_affiliate_menu_path', t('This path cannot be used as the affiliate menu path because there is either a directory or a file that exists on your server with the same name. Please choose a different path.'));
  }
}

function affiliate_admin_contentform_submit($form, $form_state) {
  $menu_path_submitted = $form_state['values']['affiliate_module_affiliate_menu_path'];
  $menu_path_current = variable_get('affiliate_module_affiliate_menu_path', 'affiliate');
  $rebuild_caches = (int) $form_state['values']['affiliate_module_flush_all_caches_on_admin_content_form_submit'];
  if ($rebuild_caches) {
    drupal_flush_all_caches();
    drupal_set_message(t('All caches have been cleared and the menu router has been rebuilt to reflect your updated affiliate menu path.'));
  }
  // Remind the admin to rebuild the site's menus if the affiliate menu path has changed.
  else if ($menu_path_submitted != $menu_path_current) {
    drupal_set_message(t('The click-thru menu path appears to have changed, but no caches have been flushed and the menu router has not been updated. If you encounter "Page not found" errors, please consider re-submitting this form but selecting <em>Yes</em> under the <em>Flush all caches</em> option. Doing so will help to ensure that any stale data is removed from the database.'), 'warning');
  }
  else if (!drupal_valid_path($menu_path_submitted)) {
    drupal_set_message(t('Warning: it appears as though your site requires its caches to be flushed and its menu router to be updated. Until such time, your affiliates will likely encounter "Page not found" errors due to stale information inside the database. To remove the stale information, please consider re-submitting this form but selecting <em>Yes</em> under the <em>Flush all caches</em> option.'), 'warning');
  }
}


function affiliate_admin_payoutsform() {
  $userpoints_form_collapse = module_exists('userpoints') ? FALSE : TRUE;
  $userpoints_disabled = module_exists('userpoints') ? FALSE : TRUE;
  $userpoints_options = function_exists('userpoints_get_categories') ? userpoints_get_categories() : array(0 => t('No categories available'));
  $referral_form_collapse = module_exists('referral') ? FALSE : TRUE;
  $form['affiliate_module_settings']['payouts'] = array(
    '#type' => 'fieldset',
    '#title' => t('Payouts'),
    '#collapsible' => FALSE,
    '#collapsed' => FALSE,
  );
  $form['affiliate_module_settings']['payouts']['affiliate_module_payouts_symbol'] = array(
   '#type' => 'textfield',
   '#title' => t('Currency symbol'),
   '#default_value' => variable_get('affiliate_module_payouts_symbol', '$'),
   '#description' => t('Enter the symbol used to denote money in your locale. Example: in the US, France, and the UK, the symbols are typically <b>&dollar;</b>, <b>&euro;</b> and <b>&pound;</b>, respectively.'),
   '#size' => 1,
  );
   $form['affiliate_module_settings']['payouts']['affiliate_module_payouts_symbol_placement'] = array(
    '#type' => 'select',
    '#title' => t('Currency symbol placement'),
    '#default_value' => variable_get('affiliate_module_payouts_symbol_placement', 1),
    '#options' => array(1 => t('Before'), 2 => t('After')),
    '#description' => t('Select whether the currency symbol should be displayed before or after the amount.'),
  );
   $form['affiliate_module_settings']['payouts']['affiliate_module_payouts_per_click'] = array(
    '#type' => 'textfield',
    '#title' => t('Payouts per click'),
    '#default_value' => variable_get('affiliate_module_payouts_per_click', 0.00),
    '#description' => t('Enter the amount, using a decimal point as the separator, that each affiliate will receive per click. For example, if the amount is &dollar;0.50 enter <b>0.50</b> in the box. Or, if the amount is 1,07&euro; enter <b>1.07</b> in the box.'),
  );
  $form['affiliate_module_settings']['payouts']['referral'] = array(
    '#type' => 'fieldset',
    '#title' => t('Referrals'),
    '#collapsible' => TRUE,
    '#collapsed' => $referral_form_collapse,
  );
   $form['affiliate_module_settings']['payouts']['referral']['affiliate_module_payouts_per_referral'] = array(
    '#type' => 'textfield',
    '#title' => t('Payouts per referral'),
    '#default_value' => variable_get('affiliate_module_payouts_per_referral', 0.00),
    '#description' => t('Enter the amount, using a decimal point as the separator, that each affiliate will receive per referral. For example, if the amount is &dollar;0.50 enter <b>0.50</b> in the box. Or, if the amount is 1,07&euro; enter <b>1.07</b> in the box. <b>Note: this setting has no effect if the <a href="http://drupal.org/project/referral">Referral</a> module is not installed</b>.'),
  );
  $form['affiliate_module_settings']['payouts']['userpoints'] = array(
    '#type' => 'fieldset',
    '#title' => t('Userpoints'),
    '#collapsible' => TRUE,
    '#collapsed' => $userpoints_form_collapse,
  );
  $form['affiliate_module_settings']['payouts']['userpoints']['affiliate_module_payouts_userpoints_clicks'] = array(
   '#type' => 'textfield',
   '#title' => t('Points per click'),
   '#default_value' => variable_get('affiliate_module_payouts_userpoints_clicks', 0),
   '#description' => t('Enter the number of points each affiliate will receive per click. <b>Note: this setting has no effect if the <a href="http://drupal.org/project/userpoints">Userpoints</a> module is not installed</b>.'),
   '#size' => 1,
   '#disabled' => $userpoints_disabled,
  );
  $form['affiliate_module_settings']['payouts']['userpoints']['affiliate_module_payouts_userpoints_tid'] = array(
   '#type' => 'select',
   '#title' => t('Category'),
   '#default_value' => variable_get('affiliate_module_payouts_userpoints_tid', 0),
   '#options' => $userpoints_options,
   '#description' => t('Points for affilate click-thrus will be assigned to this category. <b>Note: this setting has no effect if the <a href="http://drupal.org/project/userpoints">Userpoints</a> module is not installed</b>.'),
   '#disabled' => $userpoints_disabled,
  );
  return system_settings_form($form['affiliate_module_settings']);
}

function affiliate_top_users_page() {
  $limit = variable_get('affiliate_module_top_users_count_page', 10);
  $time = time();
  $time_interval = variable_get('affiliate_module_top_users_period_interval', 259200);
  $time_limit = $time - $time_interval;
  $results = db_query("SELECT COUNT(DISTINCT(ac.clickid)) AS clicks, u.uid AS uid, u.name AS name, u.data AS data, aa.payouts_owed AS payouts_owed FROM {affiliate_clicks} ac INNER JOIN {users} u ON ac.uid = u.uid INNER JOIN {affiliate} aa ON u.uid = aa.uid WHERE aa.active = 1 AND ac.click_time > :time_limit GROUP BY uid ORDER BY clicks DESC", array(":time_limit" => $time_limit));
  $rows = array();
  $rank = 1;
  if ($results && !empty($results)) {
    foreach ($results as $key => $result) {
      $row_class = ($key % 2) ? 'affiliate-row-even' : 'affiliate-row-odd';
      $userdata = unserialize($result->data);
      $homepage = (isset($userdata['affiliate_homepage']) && !empty($userdata['affiliate_homepage'])) ? check_url($userdata['affiliate_homepage']) : '';
      $username = check_plain($result->name);
      $name = (!empty($homepage)) ? l($username, $homepage) : l($username, drupal_get_path_alias('user/' . $result->uid));
      $rows[] = array(
        array('class' => array($row_class, 'affiliate-row-rank'), 'data' => $rank++),
        array('class' => array($row_class, 'affiliate-row-name'), 'data' => $name),
        array('class' => array($row_class, 'affiliate-row-clicks'), 'data' => $result->clicks),
        array('class' => array($row_class, 'affiliate-row-payouts-owed'), 'data' => $result->payouts_owed),
      );
    }
  }
  if (empty($rows)) {
    $rows[] = array(array('data' => t('No affiliate activity found for this time period.'), 'colspan' => '4'));
  }
  $header = array('Rank', 'User', 'Clicks', 'Payouts');
  $output = theme('table', array('header' => $header, 'rows' => $rows));
  drupal_set_title(t('Top affiliates from @from through @to', array('@from' => date('Y-m-d', $time_limit), '@to' => date('Y-m-d', $time))));
  return $output;
}

function affiliate_top_users_block_content() {  
  $limit = variable_get('affiliate_module_top_users_count_block', 10);
  $time = time();
  $time_interval = variable_get('affiliate_module_top_users_period_interval', 259200);
  $time_limit = $time - $time_interval;
  $results = db_query("SELECT COUNT(DISTINCT(ac.clickid)) AS clicks, u.uid AS uid, u.name AS name, u.data AS data, aa.payouts_owed AS payouts_owed FROM {affiliate_clicks} ac INNER JOIN {users} u ON ac.uid = u.uid INNER JOIN {affiliate} aa ON u.uid = aa.uid WHERE aa.active = 1 AND ac.click_time > :time_limit GROUP BY uid ORDER BY clicks DESC", array(":time_limit" => $time_limit));
  $results_count = count($results);
  $rows = array();
  $rank = 1;
  if ($results && !empty($results)) {
    foreach ($results as $result) {
      $userdata = unserialize($result->data);
      $homepage = (isset($userdata['affiliate_homepage']) && !empty($userdata['affiliate_homepage'])) ? check_url($userdata['affiliate_homepage']) : '';
      $username = check_plain($result->name);
      $name = (!empty($homepage)) ? l($username, $homepage) : l($username, drupal_get_path_alias('user/' . $result->uid));
      $rows[] = array(
        array('data' => t('#@rank', array('@rank' => $rank++))),
        array('data' => $name),
      );
      /*
      $rows[] = $name;
      */
    }
  }
  if (empty($rows)) {
    $rows[] = array(array('data' => t('No affiliate activity found for this time period.'), 'colspan' => '2'));
  }
  $output = theme('table', array('rows' => $rows));
  //  $output = theme('item_list', array('items' => $rows, 'title' => '', 'type' => 'ol', 'attributes' => array('class' => 'affiliate-top-users-ol')));
  if ($results_count && $results_count > $limit) {
    $output .= theme('more_link', array('url' => drupal_get_path_alias('affiliate/top-users'), 'title' => t('More')));
  }
  return $output;
}


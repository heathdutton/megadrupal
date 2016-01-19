<?php
/**
 * @file
 * This file contains functions to display Overview pages for both
 * signed and pending agreements on nodes.
 */


/**
 * Menu callback. Create the main signed nodes report page, which gives an overview
 * of signed nodes report on users signing or not signing the nodes.
 *
 * @return
 *   All agreement in tabular form with edit/delete operations based on access attached.
 */
function signed_nodes_report_overview() {
  $signed_nodes = signed_nodes_get_all();
  //krumo($signed_nodes);
  $row = array();
  foreach ($signed_nodes as $snid => $snid_array) {
    $links = array(
      'signed_nodes_overview_edit' => array(
        'title' => t('Signed'),
        'href' => "signed_nodes/report/signed/$snid"),
      'signed_nodes_overview_delete' => array(
        'title' => t('Pending'),
        'href' => "signed_nodes/report/pending/$snid"),
    );
    $result = db_select('node', 'n')
      ->fields('n')
      ->condition('nid', $snid_array->nid, '=')
      ->execute()
      ->fetchAssoc();
    $nodetitle = $result['title'];
    $row[] = array(check_plain(t($nodetitle . " (" . $snid_array->year . ")")), theme('links', array('links' => $links, 'attributes' => array('class' => array('links', 'inline')))));
  }

  if ($row) {
    //$output = theme('table', array(t('Node Agreement'), t('Operations')), $row);
    $header = array(t('Node Agreement'), t('Operations'));
    $output = theme('table', array(
      'header' => $header,
      'rows' => $row,
      //'attributes' => array('class' => 'mytable')
    ));

  }
  else {
    $output = '<p>' . t('No signed nodes agreement have been added. Would you like to <a href="@link">add a new node agreement</a>?', array('@link' => url('admin/settings/signed_nodes/add'))) . '</p>';
  }

  return $output;
}

/**
 * Menu callback. Create the main signed nodes report page, which gives an overview
 * of signed nodes report on users signing or not signing the nodes.
 *
 * @param $snid
 *   The agreement id called signed_node id
 *
 * @return
 *   List of users who have signed an agreement, detailed report will be available
 *   if profile module is enabled, and configuration for the profile reporting is done in signed_nodes admin form .
 */
function signed_nodes_report_signed($snid) {
  $query = "SELECT sn.uid, u.name, u.mail FROM {signed_nodes_user} sn, {users} u WHERE sn.uid = u.uid and snid = :snid";
  return signed_nodes_report_all($snid, $query);
}

/**
 * Menu callback. Create the main signed nodes report page, which gives an overview
 * of signed nodes report on users signing or not signing the nodes.
 *
 * @param $snid
 *   The agreement id called signed_node id
 *
 * @return
 *   List of users who have signed an agreement, detailed report will be available
 *   if profile module is enabled, and configuration for the profile reporting is done in signed_nodes admin form .
 */
function signed_nodes_report_pending($snid) {
  $query = "SELECT u.uid, u.name, u.mail FROM {users} u WHERE u.status = 1 AND u.uid NOT IN (SELECT snu.uid FROM {signed_nodes_user} snu WHERE snu.snid = :snid) AND (SELECT 1 FROM {signed_nodes} sn where sn.snid = :snid)";
  return signed_nodes_report_all($snid, $query);
}

/**
 * Helper function to provide similar listing user functionality based on
 * different query and agreementid.
 *
 * @param $snid
 *   The agreement id called signed_node id
 * @param $query
 *   SQL query to get pending or signed status for all users
 *
 * @return
 *   List of users who have signed an agreement, detailed report will be available
 *   if profile module is enabled, and configuration for the profile reporting is done in signed_nodes admin form .
 *
 * @see signed_nodes_report_signed()
 * @see signed_nodes_report_pending()
 */
function signed_nodes_report_all($snid, $query) {
  $additional_header = array();
  $additional_row_elements = array();
  if (module_exists('profile')) {
    $parse_fields = explode(',', variable_get('reporting_profile_fields'));
    foreach ($parse_fields as $fields) {
      $fields_explode = explode('|', $fields);
      $additional_header[] = trim($fields_explode[0]);
      $additional_row_elements[] = trim($fields_explode[1]);
    }
  }
  $result = db_query($query, array(':snid' => $snid));

  $row = array();
  foreach ($result as $uids) {
    if ($uids->uid > 2) {
      $snuser = $uids;
      $links = array(
        'signed_nodes_overview_edit' => array(
          'title' => format_username($snuser),
          'href' => "user/$uids->uid",
          'attributes' => array('target' => '_blank')),
      );

    $r = array(check_plain(t($snuser->name)), theme('links', array('links' => $links, 'attributes' => array('class' => array('links', 'inline')))));
    foreach ($additional_row_elements as $re) {
      $r[] = db_query("SELECT pv.value FROM {profile_field} pf, {profile_value} pv WHERE  pf.name = :pfname and pf.fid = pv.fid and pv.uid = :uid", array(':pfname' => $re, ':uid' => $uids->uid))->fetchField();
    }
    $r[] = l($snuser->mail, 'mailto:' . $snuser->mail, array(absolute => TRUE));
    $row[] = $r;
    }
  }

  if ($row) {
    $header = array(t('EMP #'), t('Full Name'));
    foreach ($additional_header as $h) {
      $header[] = $h;
    }
    $header[] = 'E-Mail';
    $output = theme('table', array(
      'header' => $header,
      'rows' => $row,
      //'attributes' => array('class' => 'mytable')
    ));
  }
  else {
    $output = '<p>' . t('No user has signed this node agreement') . '</p>';
  }

  return $output;
}

/*
function _signed_nodes_get_department($snuser) {
  $sql = "SELECT td.name FROM {et_user_term} etut, {term_data} td, {term_hierarchy} th WHERE uid = ".$snuser->uid." AND td.tid = etut.tid AND td.tid = th.tid AND th.parent in (22,0) AND td.vid = (SELECT vid FROM {vocabulary} WHERE name= 'Organization')";
  $dept = db_result(db_query($sql));
  $sql = "SELECT td.name FROM {et_user_term} etut, {term_data} td WHERE uid = ".$snuser->uid." AND td.tid = etut.tid AND td.vid = (SELECT vid FROM {vocabulary} WHERE name = 'Location') AND td.name IN ('Jeddah','Riyadh','Khobar')";
  $loc = db_result(db_query($sql));
  return $dept."(".$loc.")";
}*/
<?php

/**
 * @file
 * PMB reader reading lists.
 */

require_once(drupal_get_path('module', 'pmb') . '/pmb.class_data.inc');

function pmb_reader_view_reading_lists($user) {
  drupal_set_title(t('My reader account: Reading lists'));
  $breadcrumb = array();
  $breadcrumb[] = l(t('Home'), NULL);
  $breadcrumb[] = l(t('My reader account: @name', array('@name' => $user->name)), _pmb_p('reader/') . $user->uid);
  $breadcrumb[] = t('Reading lists');
  drupal_set_breadcrumb($breadcrumb);

  $pmb_data = new pmb_data();
  if (!$pmb_data->set_reader($user)) {
    return FALSE;
  }

  $reading_lists = $pmb_data->reader_get_reading_lists();

  return theme('pmb_reader_reading_lists', array(
    'reader' => $user,
    'reading_lists' => $reading_lists,
    'parameters' => array(),
  ));
}

function pmb_reader_view_reading_lists_public($user) {
  drupal_set_title(t('My reader account: Public reading lists'));
  $breadcrumb = array();
  $breadcrumb[] = l(t('Home'), NULL);
  $breadcrumb[] = l(t('My reader account: @name', array('@name' => $user->name)), _pmb_p('reader/') . $user->uid);
  $breadcrumb[] = t('Public reading lists');
  drupal_set_breadcrumb($breadcrumb);

  $pmb_data = new pmb_data();
  if (!$pmb_data->set_reader($user)) {
    return FALSE;
  }

  $reading_lists = $pmb_data->reader_get_reading_lists_public();

  return theme('pmb_reader_reading_lists_public', array(
    'reader' => $user,
    'reading_lists' => $reading_lists,
    'parameters' => array(),
  ));
}

function pmb_reader_view_reading_list($user, $list_id) {
  $pmb_data = new pmb_data();
  if (!$pmb_data->set_reader($user)) {
    return FALSE;
  }

  $reading_lists = $pmb_data->reader_get_reading_lists();

  $the_list = FALSE;
  foreach ($reading_lists as $alist) {
    if ($alist->reading_list_id == $list_id) {
      $the_list = $alist;
      break;
    }
  }
  if (!$the_list) {
    drupal_set_title(t('Reading list not found!'));
    drupal_set_message(t('Reading list not found!'), 'error');
    return FALSE;
  }

  drupal_set_title(t('My reader account: Reading list: !item', array('!item' => $the_list->reading_list_name)));
  $breadcrumb = array();
  $breadcrumb[] = l(t('Home'), NULL);
  $breadcrumb[] = l(t('My reader account: @name', array('@name' => $user->name)), _pmb_p('reader/') . $user->uid);
  $breadcrumb[] = l(t('Reading lists'), _pmb_p('reader/') . $user->uid . _pmb_p('/reading_list'));
  $breadcrumb[] = check_plain($the_list->reading_list_name);
  drupal_set_breadcrumb($breadcrumb);

  $notices = $pmb_data->get_notices($the_list->reading_list_notices_ids);

  return drupal_get_form('pmb_reader_reading_list_form', $the_list, $notices);
}

function pmb_reader_view_reading_list_public($user, $list_id) {
  $pmb_data = new pmb_data();
  if (!$pmb_data->set_reader($user)) {
    return FALSE;
  }

  $reading_lists = $pmb_data->reader_get_reading_lists_public();

  $the_list = FALSE;
  foreach ($reading_lists as $alist) {
    if ($alist->reading_list_id == $list_id) {
      $the_list = $alist;
      break;
    }
  }
  if (!$the_list) {
    drupal_set_title(t('Reading list not found!'));
    drupal_set_message(t('Reading list not found!'), 'error');
    return FALSE;
  }

  drupal_set_title(t('My reader account: Public reading list: !item', array('!item' => $the_list->reading_list_name)));
  $breadcrumb = array();
  $breadcrumb[] = l(t('Home'), NULL);
  $breadcrumb[] = l(t('My reader account'), _pmb_p('reader/') . $user->uid);
  $breadcrumb[] = l(t('Reading lists'), _pmb_p('reader/') . $user->uid . _pmb_p('/reading_list_public'));
  $breadcrumb[] = check_plain($the_list->reading_list_name);
  drupal_set_breadcrumb($breadcrumb);

  $notices = $pmb_data->get_notices($the_list->reading_list_notices_ids);

  return theme('pmb_reader_reading_list_public', array(
    'reader' => $user,
    'reading_lists' => $the_list,
    'notices' => $notices,
    'parameters' => array(),
  ));
}

function pmb_reader_add_notices_to_reading_list($user, $list_id, $notices_ids) {
  $pmb_data = new pmb_data();
  if (!$pmb_data->set_reader($user)) {
    return FALSE;
  }

  $pmb_data->reader_add_notices_to_reading_list($list_id, $notices_ids);
}


function pmb_reader_remove_notices_from_reading_list($user, $list_id, $notices_ids) {
  $pmb_data = new pmb_data();
  if (!$pmb_data->set_reader($user)) {
    return FALSE;
  }

  $pmb_data->reader_remove_notices_to_reading_list($list_id, $notices_ids);
}

function pmb_reader_empty_reading_list($user, $list_id) {
  $pmb_data = new pmb_data();
  if (!$pmb_data->set_reader($user)) {
    return FALSE;
  }

  $pmb_data->reader_empty_reading_list($list_id);
}

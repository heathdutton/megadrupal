<?php

/**
 * @file
 * PMB reader reservations.
 */

require_once(drupal_get_path('module', 'pmb') . '/pmb.class_data.inc');

function pmb_reader_reservations($user) {
  drupal_set_title(t('My reader account: Reservations'));
  $breadcrumb = array();
  $breadcrumb[] = l(t('Home'), NULL);
  $breadcrumb[] = l(t('My reader account'), _pmb_p('reader/') . $user->uid);
  $breadcrumb[] = t('Reservations');
  drupal_set_breadcrumb($breadcrumb);

  $pmb_data = new pmb_data();
  $pmb_data->set_reader($user);
  $reservations = $pmb_data->reader_get_reservations();

  $notices_ids = array();
  $bulletins_ids = array();
  if ($reservations) {
    foreach ($reservations as $areservation) {
      if ($areservation->notice_id)
        $notices_ids[] = $areservation->notice_id;
      if ($areservation->bulletin_id)
        $bulletins_ids[] = $areservation->bulletin_id;
    }
  }

  $notices = $pmb_data->get_notices($notices_ids);
  $bulletins = $pmb_data->get_bulletins($bulletins_ids);

  return theme('pmb_reader_reservations', array(
    'reader' => $user,
    'reservations' => $reservations,
    'parameters' => array(
      'notices' => $notices,
      'bulletins' => $bulletins,
  )));
}

function pmb_reader_reservation($user, $action = NULL, $item = NULL, $location_id = 0) {
  switch ($action) {
    case _pmb_p('add') :
      pmb_reader_reservation_add($user, $item, $location_id);
      break;
    case _pmb_p('delete') :
      pmb_reader_reservation_delete($user, $item);
      break;
    default :
      drupal_goto(_pmb_p('reader/') . $user->uid . _pmb_p('/reservation'));
  }
}

function pmb_reader_reservation_add($user, $notice_id, $location_id = 0) {
  $bulletin_id = 0;
  if ($notice_id && ($notice_id[0] == 'b')) {
    $bulletin_id = drupal_substr($notice_id, 1) + 0;
  }
  else {
    $notice_id += 0;
  }
  if (!($notice_id || $bulletin_id)) {
    drupal_set_title(t('Missing notice id!'));
    drupal_set_message(t('Missing notice id!'), 'error');
    return FALSE;
  }

  $pmb_data = new pmb_data();
  if (!$pmb_data->set_reader($user)) {
    return FALSE;
  }

  if (!$pmb_data->reader_add_reservation($notice_id, $bulletin_id, $location_id)) {
    drupal_set_message(t('Could not create reservation!'), 'error');
  }

  $notice_title = '';
  if ($bulletin_id) {
    $bulletin = $pmb_data->get_bulletin($bulletin_id);
    $notice_title = $bulletin['bulletin']->bulletin_title;
  }
  else {
    $notice = $pmb_data->get_notice($notice_id);
    if (isset($notice['notice']['f']['200'])) {
      foreach ($notice['notice']['f']['200'] as &$afield) {
        if (!isset($afield['a']))
          continue;
        $notice_title .= ' ' . $afield['a'];
      }
    }
    $notice_title = trim($notice_title);
  }

  drupal_set_title(t('Add a reservation'));
  $breadcrumb = array();
  $breadcrumb[] = l(t('Home'), NULL);
  $breadcrumb[] = l(t('Record: @item', array('@item' => $notice_title)), 'catalog/record/' . $notice_id);
  $breadcrumb[] = t('Add a reservation');

  drupal_goto(_pmb_p('reader/') . $user->uid . _pmb_p('/reservation'));
}

function pmb_reader_reservation_delete($user, $reservation_id) {
  $reservation_id += 0;
  if ($reservation_id) {
    $pmb_data = new pmb_data();
    $pmb_data->set_reader($user);
    $pmb_data->reader_delete_reservation($reservation_id);
  }
  drupal_goto(_pmb_p('reader/') . $user->uid . _pmb_p('/reservation'));
}

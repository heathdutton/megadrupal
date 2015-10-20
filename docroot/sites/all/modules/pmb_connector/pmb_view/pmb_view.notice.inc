<?php

/**
 * @file
 * PMB view notices.
 */

require_once(drupal_get_path('module', 'pmb') . '/pmb.class_data.inc');

function pmb_view_notice($notice_id = NULL) {
  if ($notice_id === NULL) {
    return t('ERROR!');
  }

  global $user;

  $pmb_notice = new pmb_data();
  $pmb_notice->set_reader($user);
  $notice = $pmb_notice->get_notice($notice_id);

  if (!$notice || $notice['notice'] == 0) {
    drupal_set_title(t('Record not found!'));
    drupal_set_message(t('Record not found!'), 'error');
    return '';
  }

  // Check if record is an issue.
  if ($notice['fields']['main_type'] == t('Bulletin')) {
    $bulletin_id = $pmb_notice->find_notice_bulletin($notice_id);
    if ($bulletin_id) {
      drupal_goto(_pmb_p('catalog/issue/') . $bulletin_id);
    }
    // Else it's an issue without id, so it's managed as a normal notice...
  }

  if (module_exists('pmb_reader')) {
    $can_add_to_cart = $pmb_notice->check_reader($user);
    $can_reserve_notice = $pmb_notice->reader_can_reserve_notice($notice_id, 0);
  }
  else {
    $can_add_to_cart = FALSE;
    $can_reserve_notice = FALSE;
  }

  $title = $notice['fields']['main_title'];
  drupal_set_title(t('Record: !item', array('!item' => $title)), PASS_THROUGH);

  $breadcrumb = array();
  $breadcrumb[] = l(t('Home'), NULL);
  $breadcrumb[] = l(t('Catalog'), _pmb_p('catalog'));
  $breadcrumb[] = t('Record: !item', array('!item' => $title));
  drupal_set_breadcrumb($breadcrumb);

  $nb_items = ($notice['fields']['main_type'] == t('Article')) ?
    0 :
    count($notice['items']);

  if ($notice['fields']['main_type'] == t('Serial')) {
    return theme('pmb_view_serial', array(
      'notice' => $notice,
      'parameters' => array(
        'can_reserve' => $can_reserve_notice,
        'nb_items' => $nb_items,
    )));
  }

  return theme('pmb_view_notice', array(
    'notice' => $notice,
    'parameters' => array(
      'can_reserve' => $can_reserve_notice,
      'nb_items' => $nb_items,
      'can_add_to_cart' => $can_add_to_cart,
  )));
}

function pmb_view_notice_external($notice_id = NULL) {
  if ($notice_id === NULL) {
    return t('ERROR!');
  }

  global $user;

  $pmb_data = new pmb_data();
  $pmb_data->set_reader($user);

  $notice = $pmb_data->get_notice_external($notice_id);

  if (!$notice || $notice['notice'] == 0) {
    drupal_set_title(t('Record not found!'));
    drupal_set_message(t('Record not found!'), 'error');
    return '';
  }

  $title = '';
  if (isset($notice['notice']['f']['200'][0]['a']))
    $title = $notice['notice']['f']['200'][0]['a'];
  if (isset($notice['notice']['f']['801'][0][9]))
    $source = $notice['notice']['f']['801'][0][9];
  drupal_set_title($source . ' : ' . $title);

  $breadcrumb = array();
  $breadcrumb[] = l(t('Home'), NULL);
  $breadcrumb[] = l(t('Catalog'), _pmb_p('catalog'));
  $breadcrumb[] = t('External Record: @item', array('@item' => $title));
  drupal_set_breadcrumb($breadcrumb);

  return theme('pmb_view_notice_external', array(
    'notice' => $notice,
    'parameters' => array(),
  ));
}

function pmb_view_serial($notice_id = NULL, $page = 1) {
  if ($notice_id === NULL) {
    return t('ERROR!');
  }

  global $user;

  $pmb_data = new pmb_data();
  $pmb_data->set_reader($user);

  $notice = $pmb_data->get_notice($notice_id);

  if (!$notice || $notice['notice'] == 0) {
    drupal_set_title(t('Serial not found!'));
    drupal_set_message(t('Serial not found!'), 'error');
    return '';
  }

  $title = '';
  if (isset($notice['notice']['f']['200'][0]['a']))
    $title = $notice['notice']['f']['200'][0]['a'];
  drupal_set_title(t('Serial: !item', array('!item' => $title)));

  $breadcrumb = array();
  $breadcrumb[] = l(t('Home'), NULL);
  $breadcrumb[] = l(t('Catalog'), _pmb_p('catalog'));
  $breadcrumb[] = l(t('Serials'), _pmb_p('catalog/serial'));
  $breadcrumb[] = check_plain($title);
  drupal_set_breadcrumb($breadcrumb);

  $page += 0;

  $bulletins_count = pmb_variable_get('pmb_bulletinsperpage_serials');
  $bulletins_pages = array_chunk($notice['bulletins'], $bulletins_count);
  $page = min($page, count($bulletins_pages));
  $page = max(1, $page);

  $bulletins = (count($bulletins_pages)) ?
    $bulletins_pages[$page - 1] :
    array();

  return theme('pmb_view_serial', array(
    'notice' => $notice,
    'parameters' => array(
      'bulletins' => $bulletins,
      'page_number' => $page,
      'bulletins_per_page' => $bulletins_count,
  )));
}

function pmb_view_bulletin($bulletin_id = NULL) {
  if ($bulletin_id === NULL) {
    return t('ERROR!');
  }

  global $user;

  $pmb_data = new pmb_data();
  $pmb_data->set_reader($user);

  $bulletin = $pmb_data->get_bulletin($bulletin_id);

  if (!$bulletin) {
    drupal_set_title(t('Issue not found!'));
    drupal_set_message(t('Issue not found!'), 'error');
    return '';
  }
  $serial = $pmb_data->get_notice($bulletin['bulletin']->serial_id);

  $serial_title = (isset($serial['notice']['f']['200'][0]['a'])) ?
      $serial['notice']['f']['200'][0]['a'] :
      '';

  if ($bulletin['analysis']) {
    $analysis = $pmb_data->get_notices($bulletin['analysis']);
  }
  else {
    $analysis = array();
  }

  if (module_exists('pmb_reader')) {
    $can_reserve_notice = $pmb_data->reader_can_reserve_notice(0, $bulletin_id);
  }
  else {
    $can_reserve_notice = FALSE;
  }

  $nb_items = count($bulletin['items']);

  $title = $bulletin['bulletin']->bulletin_numero;
  if ($bulletin['bulletin']->bulletin_date) {
    $title .= ($title ? ', ' : '') . pmb_convert_date($bulletin['bulletin']->bulletin_date);
  }
  if ($bulletin['bulletin']->bulletin_date_caption) {
    $title .= ($title ? ', ' : '') . $bulletin['bulletin']->bulletin_date_caption;
  }
  if ($bulletin['bulletin']->bulletin_title) {
    $title .= ($title ? ', ' : '') . $bulletin['bulletin']->bulletin_title;
  }

  drupal_set_title(t('Issue: !item', array('!item' => $title)));

  $breadcrumb = array();
  $breadcrumb[] = l(t('Home'), NULL);
  $breadcrumb[] = l(t('Catalog'), _pmb_p('catalog'));
  $breadcrumb[] = l(t('Serials'), _pmb_p('catalog/serial'));
  $breadcrumb[] = l($serial_title, _pmb_p('catalog/serial/') . $serial['id']);
  $breadcrumb[] = t('Issue: @item', array('@item' => $title));
  drupal_set_breadcrumb($breadcrumb);

  return theme('pmb_view_bulletin', array(
    'bulletin' => $bulletin,
    'serial' => $serial,
    'parameters' => array(
      'can_reserve' => $can_reserve_notice,
      'nb_items' => $nb_items,
      'analysis' => $analysis,
  )));
}

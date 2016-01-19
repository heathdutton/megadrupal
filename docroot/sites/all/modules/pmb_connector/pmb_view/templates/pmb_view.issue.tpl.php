<?php

/**
 * @file
 * PMB view issue (bulletin) template.
 */

global $user;

$bulletin_id = $bulletin['id'];
$notice_fields = &$bulletin['fields'];

// Prepare template.
$template .= '<div id="notice_' . $bulletin_id . '">';

// Display cover if any.
if (isset($notice_fields['cover_url'])) {
  $template .= '<div id="notice_' . $bulletin_id . '_cover" style="float: left; padding-right: 20px; padding-top: 20px">';
  $template .= '<img src="' . $notice_fields['cover_url'] . '" style="max-width: 130px;">';
  $template .= '</div>';
  $template .= '<br style="clear: both;"/>';
}

// Display main informations of bulletin.
$template .= '<h2>' . t('Information') . '</h2>';
$template .= '<div style="float: left;" id="notice_' . $bulletin_id . '_table">';

$header = array();
$rows = array();

$rows[] = array(
  t('Title'),
  ($bulletin['bulletin']->bulletin_title) ?
    check_plain($bulletin['bulletin']->bulletin_title) :
    '[' . t('No title mentionned') . ']',
);
$rows[] = array(
  t('Number'),
  check_plain($bulletin['bulletin']->bulletin_numero),
);
$rows[] = array(
  t('Period'),
  check_plain(pmb_convert_date($bulletin['bulletin']->bulletin_date) . ' ' . $bulletin['bulletin']->bulletin_date_caption),
);

$rows[] = array(
  t('Serial'),
  ($bulletin['bulletin']->serial_title) ?
    l($bulletin['bulletin']->serial_title, _pmb_p('catalog/serial/') . $bulletin['bulletin']->serial_id) :
    '[' . t('No title mentionned') . ']',
);

$rows_added = array(
  'author' => array(
    'full_name' => t('Author'),
  ),
  'publisher' => array(
    'full_name' => t('Publisher'),
  ),
  'serie' => array(
    'full_name' => t('Serie'),
  ),
  'subserie' => array(
    'full_name' => t('Subserie'),
  ),
  'size' => t('Size'),
  'pagination' => t('Pagination'),
);
$rows = array_merge($rows, pmb_get_fields_list($notice_fields, $rows_added, FALSE));
$template .= theme('table', array('header' => $header, 'rows' => $rows, 'empty' => t('No information.')));
$template .= '</div>';
$template .= '<br style="clear: both;"/>';

// Display description (abstract).
$template .= '<h2>' . t('Description') . '</h2>';
$template .= '<div id="notice_' . $bulletin_id . '_desc">';
if (isset($notice_fields['abstract'])) {
  $template .= $notice_fields['abstract'];
}
else {
  $template .= '<em>' . t('No description.') . '</em>';
}
$template .= '</div>';
$template .= '<br style="clear: both;"/>';

// Display table of items.
if ($parameters['nb_items']) {
  $template .= '<h2>' . t('Items') . '</h2>';
  $template .= '<div id="notice_' . $bulletin_id . '_items">';

  $header = array(
    t('Barcode'),
    t('Cote'),
    t('Media type'),
    t('Location'),
    t('Section'),
    t('Availability'),
//     t('Situation'),
  );
  $rows = array();
  foreach ($bulletin['items'] as $item) {
    $rows[] = array(
      check_plain($item->cb),
      check_plain($item->cote),
      check_plain($item->support),
      check_plain($item->location_caption),
      check_plain($item->section_caption),
      check_plain($item->statut),
//       check_plain($item->situation),
    );
  }
  $template .= theme('table', array('header' => $header, 'rows' => $rows, 'empty' => t('No item.')));
  $template .= '</div>';

  if ($parameters['can_reserve']) {
    $template .= '<h3>' . t('Holding') . '</h3>';
    $template .= l(t('Hold this document'), _pmb_p('reader/') . $user->uid . _pmb_p('/reservation/add/b') . $bulletin_id);
  }

  $template .= '<br style="clear: both;"/>';
}

// Display table of digital items.
if (count($bulletin['expl_nums'])) {
  $template .= '<h2>' . t('Digital items') . '</h2>';
  $template .= '<div id="notice_' . $bulletin_id . '_explnums">';

  // Base url is get from gateway normal PMB server url.
  $base_url = dirname(dirname(pmb_variable_get('pmb_link_serverurl'))) . '/';
  $header = array(
    t('Download URL'),
    t('Name'),
  );
  $rows = array();
  foreach ($bulletin['expl_nums'] as &$aexplnum) {
    $rows[] = array(
      l($base_url . $aexplnum->downloadUrl, $base_url . $aexplnum->downloadUrl, array('attributes' => array('target' => '_blank'))),
      l($aexplnum->name, $base_url . $aexplnum->downloadUrl, array('attributes' => array('target' => '_blank'))),
    );
  }
  $template .= theme('table', array('header' => $header, 'rows' => $rows));

  $template .= '</div>';
  $template .= '<br style="clear: both;"/>';
}

// Display table of analysis.
if (count($parameters['analysis'])) {
  $template .= '<h2>' . t('Analysis') . '</h2>';
  $template .= '<div id="analysis_' . $bulletin_id . '_explnums">';

  $header = array(
    t('Title'),
  );
  $rows = array();
  foreach ($parameters['analysis'] as &$anotice) {
    $text = $anotice['fields']['main_title'];
    $text .= ($text ? ', ' . $anotice['fields']['main_author'] : $anotice['fields']['main_author']);
    $rows[] = array(
      l($text, _pmb_p('catalog/record/') . $anotice['id'], array('html' => TRUE)),
    );
  }
  $template .= theme('table', array('header' => $header, 'rows' => $rows));

  $template .= '</div>';
  $template .= '<br style="clear: both;"/>';
}

$template .= '</div>';

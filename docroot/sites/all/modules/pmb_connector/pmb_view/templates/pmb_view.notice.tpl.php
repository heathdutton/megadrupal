<?php

/**
 * @file
 * PMB view notice template.
 */

global $user;

$notice_id = $notice['id'];
$notice_fields = &$notice['fields'];

// Prepare template.
$template .= '<div id="notice_' . $notice_id . '">';

// Display cover if any.
if (isset($notice_fields['cover_url'])) {
  $template .= '<div id="notice_' . $notice_id . '_cover" style="float: left; padding-right: 20px; padding-top: 20px">';
  $template .= '<img src="' . $notice_fields['cover_url'] . '" style="max-width: 130px;">';
  $template .= '</div>';
  $template .= '<br style="clear: both;"/>';
}

// Display main informations of notice.
$template .= '<h2>' . t('Information') . '</h2>';
$template .= '<div style="float: left;" id="notice_' . $notice_id . '_table">';
$header = array();
// See all available fields in BIBLINFO.txt.
$rows = array(
  'document_type' => t('Document type'),

  'collection' => array(
    'one_row' => array(
      'full' => t('Collection'),
    ),
  ),
  'serial' => array(
    'one_row' => array(
      'full' => t('Serial'),
    ),
  ),
  'issue' => array(
    'one_row' => array(
      'full' => t('Issue'),
    ),
  ),

  'title' => array(
    'name' => t('Title'),
    'other_info' => t('Other title information'),
    'other_author' => t('Other title'),
    'parallel' => t('Original title'),
  ),
  'uniform_title' => array(
    'one_row' => array(
      'full' => t('Uniform title'),
    ),
  ),

  'author' => array(
    'one_row' => array(
      'full' => t('Author'),
    ),
  ),

  'convention' => array(
    'one_row' => array(
      'full_name' => t('Convention'),
    ),
  ),

  'publisher' => array(
    'one_row' => array(
      'full' => t('Publisher'),
    ),
  ),
  'main_date' => t('Date'),
  'edition_statement' => t('Edition statement'),

  'serie' => array(
    'one_row' => array(
      'full' => t('Serie'),
    ),
  ),
  'subserie' => array(
    'one_row' => array(
      'full' => t('Subserie'),
    ),
  ),

  'piece' => array(
    'one_row' => array(
      'full' => t('Piece'),
    ),
  ),

  'isbn' => t('ISBN'),
  'issn' => t('ISSN'),
  'price' => t('Price'),
  'pagination' => t('Pagination'),
  'layout' => t('Layout'),
  'size' => t('Format'),
  'accompanying_material' => t('Accompanying material'),

  'language' => t('Language'),
  'language_original' => t('Original language'),
  'category' => t('Categories'),
  'keyword' => t('Keywords'),
  'class' => t('Classes'),

  'source' => t('Source'),

  'online_doc_url' => t('Online document'),
  'online_doc_format' => t('Format of the online document'),

  'note_general' => t('General note'),
  'note_content' => t('Content note'),

  'relation' => array(
    'one_row' => array(
      'full' => t('Relations'),
    ),
  ),
);
$rows = pmb_get_fields_list($notice_fields, $rows, FALSE);
$template .= theme('table', array('header' => $header, 'rows' => $rows, 'empty' => t('No information.')));
$template .= '</div>';
$template .= '<br style="clear: both;"/>';

// Display description (abstract).
$template .= '<h2>' . t('Description') . '</h2>';
$template .= '<div id="notice_' . $notice_id . '_desc">';
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
  $template .= '<div id="notice_' . $notice_id . '_items">';

  $header = array(
    t('Barcode'),
    t('Cote'),
    t('Media type'),
    t('Location'),
    t('Section'),
    t('Availability'),
    t('Situation'),
  );
  $rows = array();
  foreach ($notice['items'] as $item) {
    $rows[] = array(
      check_plain($item->cb),
      check_plain($item->cote),
      check_plain($item->support),
      check_plain($item->location_caption),
      check_plain($item->section_caption),
      check_plain($item->statut),
      check_plain($item->situation),
    );
  }
  $template .= theme('table', array('header' => $header, 'rows' => $rows, 'empty' => t('No item.')));
  $template .= '</div>';

  if ($parameters['can_reserve']) {
    $template .= '<h3>' . t('Holding') . '</h3>';
    $template .= l(t('Hold this document'), _pmb_p('reader/') . $user->uid . _pmb_p('/reservation/add/') . $notice_id);
  }

  $template .= '<br style="clear: both;"/>';
}

// Display table of digital items.
if (count($notice['expl_nums'])) {
  $template .= '<h2>' . t('Digital items') . '</h2>';
  $template .= '<div id="notice_' . $notice_id . '_explnums">';

  // Base url is get from gateway normal PMB server url.
  $base_url = dirname(dirname(pmb_variable_get('pmb_link_serverurl'))) . '/';
  $header = array(
    t('Download URL'),
    t('Name'),
  );
  $rows = array();
  foreach ($notice['expl_nums'] as &$aexplnum) {
    $rows[] = array(
      // @todo Currently, PMB webservice doesn't return good vignUrl.
      l('<img src="' . $base_url . $aexplnum->vignUrl . '"/>', $base_url . $aexplnum->downloadUrl, array('html' => TRUE, 'attributes' => array('target' => '_blank'))),
      l($aexplnum->name, $base_url . $aexplnum->downloadUrl, array('attributes' => array('target' => '_blank'))),
    );
  }
  $template .= theme('table', array('header' => $header, 'rows' => $rows));

  $template .= '</div>';
  $template .= '<br style="clear: both;"/>';
}

// Display a form if user can add a notice to cart.
if ($parameters['can_add_to_cart']) {
  $template .= '<h2>' . t('Add to cart') . '</h2>';
  $template .= drupal_render(drupal_get_form('pmb_reader_add_notice_to_cart_form', $notice_id));
}

$template .= '</div>';

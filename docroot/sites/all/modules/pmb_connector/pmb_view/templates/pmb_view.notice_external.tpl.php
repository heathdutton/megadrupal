<?php

/**
 * @file
 * PMB view external notice template.
 */

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

$template .= '</div>';

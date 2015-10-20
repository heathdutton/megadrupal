<?php

/**
 * @file
 * PMB view serial template.
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
$rows = array(
  'collection' => array(
    'full' => t('Collection'),
  ),
  'title' => array(
    'full' => t('Title'),
    'parallel' => t('Original title'),
    'other_author' => t('Other title'),
  ),

  'document_type' => t('Document type'),

  'publisher' => array(
    'one_row' => array(
      'full' => t('Publishers'),
    ),
  ),
  'edition_statement' => t('Edition statement'),

  'serie' => array(
    'full' => t('Serie'),
    'issn' => t('ISSN'),
  ),
  'subserie' => array(
    'full' => t('Subserie'),
    'issn' => t('ISSN'),
  ),

  'author' => array(
    'one_row' => array(
      'full_name' => t('Authors'),
    ),
  ),

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

  'online_doc_url' => t('Online document'),
  'online_doc_format' => t('Format of the online document'),

  'note_general' => t('General note'),
  'note_content' => t('Content note'),
  'abstract' => t('Abstract'),
);
$rows = pmb_get_fields_list($notice_fields, $rows, FALSE);
$template .= theme('table', array('header' => $header, 'rows' => $rows, 'empty' => t('No information.')));
$template .= '</div>';
$template .= '<br style="clear: both;"/>';

// Display issues.
$template .= '<h2>' . t('Issues') . '</h2>';
$template .= '<div id="notice_' . $notice_id . '_desc">';
$header = array(
  t('Number'),
  t('Date'),
  t('Title'),
  t('Barcode'),
);
$rows = array();
foreach ($parameters['bulletins'] as $abulletin) {
  $link = l(($abulletin->bulletin_numero ? $abulletin->bulletin_numero : t('Unknown')), _pmb_p('catalog/issue/') . $abulletin->bulletin_id);
  $rows[] = array(
    $link,
    pmb_convert_date($abulletin->bulletin_date) . ' ' . $abulletin->bulletin_date_caption,
    $abulletin->bulletin_title,
    $abulletin->bulletin_barcode,
  );
}
$template .= theme('table', array('header' => $header, 'rows' => $rows, 'empty' => t('No issue.')));

// Display pager.
$link_maker_function = create_function('$page_number', 'return "' . _pmb_p('catalog/serial/') . $notice_id . '/" . $page_number;');
$template .= theme('pmb_pager', array(
  'current_page' => $parameters['page_number'],
  'page_count' => ceil(count($notice['bulletins']) / $parameters['bulletins_per_page']),
  'tags' => array(),
  'quantity' => 7,
  'link_generator_callback' => $link_maker_function,
));
$template .= '</div>';

$template .= '</div>';

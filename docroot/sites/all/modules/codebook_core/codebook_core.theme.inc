<?php

/**
 * Default theme implementation for codebook_core_section_note.
 */
function theme_codebook_core_section_notes($variables) {

  $lines = array();

  foreach ($variables['items'] as $item) {

    $w = entity_metadata_wrapper('field_collection_item', $item['value']);

    $line = array();

    if ($ord = $w->field_codebook_ordinance->value()) {
      // Grab the ordinance number from the ordinance.
      $ow = entity_metadata_wrapper('node', $ord);
      $ord_num = $ow->field_codebook_ord_num->value();
      $line[] = l($ord_num, "node/{$ord->nid}");
    }

      // If the field collection item contained text, add it in.
    if ($text = $w->field_codebook_footnote_text->value()) {
      $line[] = check_plain($text);
    }

    $lines[] = implode(': ', $line);

  }

  if ($lines) {
    return '(' . implode('; ', $lines) . ')';
  }

}

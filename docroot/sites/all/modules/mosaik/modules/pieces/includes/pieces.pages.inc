<?php

/**
 * Menu callback; presents the piece editing form.
 */
function piece_page_edit($piece) {
  module_load_include('inc', 'pieces', 'pieces.admin');
  drupal_set_title(t('<em>Edit Piece</em> @title', array('@title' => $piece['title'])), PASS_THROUGH);
  return drupal_get_form('pieces_form', $piece);
}

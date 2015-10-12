<?php

/**
 * Airing view callback.
 */
function cm_airing_view($airing) {
  drupal_set_title(entity_label('airing', $airing));
  return entity_view('airing', array(entity_id('airing', $airing) => $airing), 'full');
}

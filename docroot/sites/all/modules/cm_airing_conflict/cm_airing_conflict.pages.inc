<?php

/**
 * Airing Conflict view callback.
 */
function cm_airing_conflict_view($airingconflict) {
  drupal_set_title(entity_label('airingconflicts', $airingconflict));
  return entity_view('airingconflicts', array(entity_id('airingconflict', $airingconflict) => $airingconflict), 'full');
}

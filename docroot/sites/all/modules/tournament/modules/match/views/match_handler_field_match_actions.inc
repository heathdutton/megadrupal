<?php

/**
 * @file
 * Views handler for Match module
 */

class match_handler_field_match_actions extends views_handler_field {
  function render($values) {
    if (!empty($values->matches_nid)) {
      $node = node_load($values->matches_nid);
      $links = _match_actions($node, array('destination' => 'matches'));
      if (empty($links)) {
        $links = array(
          'match-details' => array(
            'title' => t('Match page'),
            'href' => 'node/' . $node->nid,
          ),
        );
      }
      return theme('links', array('links' => $links, 'attributes' => array('class' => array('links', 'inline', 'match-actions'))));
    }
  }
}

<?php
/**
 * @file
 * Theme functions for page output for knockout module.
 */

/**
 * Menu callback for full screen knockout brackets
 */
function knockout_bracket_callback($node) {
  module_load_include('inc', 'knockout', 'knockout.bracket');
  $build = knockout_bracket_build($node);
  return knockout_bracket_view($node, $build, TRUE);
}

/*
 * Preprocess for knockout-round.tpl.php.
 */
function template_preprocess_knockout_round(&$vars) {
  $rid = key($vars['round']);
  $matches = $vars['round'][$rid];
  $match_count = count($matches);
  $count = 1;
  $vars['rid'] = $rid;

  $vars['connectors'] = $vars['matches'] = '';
  foreach ($matches as $match) {
    $match['round'] = $rid;
    $first = ($count == 1) ? " first" : "";

    // Make bracket connectors for every other match OR straight line for each match in LB odd or one match rounds
      if ($vars['type'] == 'main' && $match_count > 1 && $count % 2 ||
        $vars['type'] != 'main' && !$vars['last_round'] && ($rid % 2 || $count % 2 ) ) {

      if ($vars['type'] == 'other' && $rid % 2 || $match_count == 1) {
        $type = 'straight-line';
        $last = ($count == $match_count) ? " last" : "";
      }
      else {
        $type = 'bracket';
        $last = ($count == $match_count - 1 ) ? " last" : "";
      }
      $vars['connectors'] .= theme('knockout_connectors', array('type' => $type, 'first' => $first, 'last' => $last));
    }

    $last = ($count == $match_count) ? " last" : "";

    $match_vars = array(
      'match' => $match,
      'rid' => $rid,
      'first' => $first,
      'last' => $last
    );
    $vars['matches'] .= theme('knockout_match', $match_vars);
    $count++;
  }
}

/*
 * Preprocess for knockout-match.tpl.php
 */
function template_preprocess_knockout_match(&$vars) {
  $vars['match_class'] = array();

  // Add first and last classes to match
  foreach (array('first', 'last') as $order) {
    if (!empty($vars[$order])) {
      $vars['match_class'][] = $order;
    }
  }

  // Add match status to it's class
  if (isset($vars['match']['status'])) {
    $vars['match_class'][] = 'status-' . $vars['match']['status'];
  }

  if (empty($vars['match']['participants'][0])) {
    $vars['match']['participants'][0] = array(
      'id' => 0,
      'name' => NULL,
      'score' => '-',
    );
  }
  if (empty($vars['match']['participants'][1])) {
    $vars['match']['participants'][1] = array(
      'id' => 0,
      'name' => NULL,
      'score' => '-',
    );
  }

  if (!empty($vars['match']['nid'])) {
    // Add match node ID to it's class
    $vars['match_class'][] = 'match-' . $vars['match']['nid'];
    $vars['match_class'][] = 'node';

    // Show scores only for confirmed matches
    if (!_match_confirmed($vars['match']['node'])) {
      foreach ($vars['match']['participants'] as $pid => $participant)  {
        $vars['match']['participants'][$pid]['score'] = '-';
      }
    }

    // Add match action "Enter result" for eligible users
    $vars['score_link'] = FALSE;
    if (match_access('result_enter', $vars['match']['node'])) {
      $vars['score_link'] = l(t('Enter result'), 'node/' . $vars['match']['nid'] . '/action/result_enter',
        array(
          'query' => array('destination' => 'node/' . $vars['match']['node']->tid . '/bracket'),
          'attributes' => array('class' => array('score-link', 'match-link')),
        )
      );

      // Add "show-score-link" class to match to help alignment with "Match details" link
      $vars['match_class'][] = 'show-score-link';
    }

    // Add detail link "Match details" for eligible matches
    $vars['details_link'] = l(t('Match details'), 'node/' . $vars['match']['nid'],
      array('attributes' => array('class' => array('match-link', 'details-link')))
    );
  }

  // When name is too wide to fit in match, shave it down and add an ellipsis
  foreach ($vars['match']['participants'] as $weight => $participant) {
    if (mb_strwidth($participant['name']) > 21 || isset($participant['entity']->name_trimmed)) {
      // Entity is not set in preview before starting tournament
      if (!isset($participant['entity'])) {
        $participant['entity']->name = $participant['name'];
      }

      while (mb_strwidth($participant['entity']->name) > 19) {
        $participant['entity']->name = mb_substr($participant['entity']->name, 0, -1);
      }
      $participant['entity']->name .= "...";
      $label = match_participant_label('user', $participant['entity']);
      $vars['match']['participants'][$weight]['display_name'] = $label;
      $participant['entity']->name_trimmed = TRUE;
    }
  }

  $vars['match']['participants'][0]['class'][] = 'participant-' . $vars['match']['participants'][0]['id'];
  $vars['match']['participants'][1]['class'][] = 'participant-' . $vars['match']['participants'][1]['id'];

  if (isset($vars['match']['status']) && $vars['match']['status'] == MATCH_STATUS_FINISHED) {
    if ($vars['match']['participants'][0]['score'] > $vars['match']['participants'][1]['score']) {
      $vars['match']['participants'][0]['class'][] = 'winner';
      $vars['match']['participants'][1]['class'][] = 'loser';
    }
    elseif ($vars['match']['participants'][0]['score'] < $vars['match']['participants'][1]['score']) {
      $vars['match']['participants'][1]['class'][] = 'winner';
      $vars['match']['participants'][0]['class'][] = 'loser';
    }
  }

  // Seeds not required after the first round
  if ($vars['rid'] > 1) {
    unset($vars['match']['participants'][0]['seed']);
    unset($vars['match']['participants'][1]['seed']);
  }

  foreach ($vars['match']['participants'] as $weight => $participant) {
    $side = $weight == 0 ? 'top' : 'bottom';
    $vars[$side] = $participant;
  }
}

/*
 * Preprocess for knockout-rankings.tpl.php.
 */
function template_preprocess_knockout_rankings(&$vars) {
  $node = $vars['node'];
  
  $participants = knockout_tournament_participant_ranks($node);
  $ids = array_keys($participants);
  $entities = entity_load($node->entity_type, $ids);
  
  foreach ($participants as &$participant) {
    $entity = $entities[$participant['id']];
    $participant['entity'] = $entity;
    
    // Handle user pictures in this module
    if ($node->entity_type == 'user' && variable_get('tournament_user_picture', 0)) {
      $participant['picture'] = theme('user_picture', array('account' => $entity));
    }

    $participant['label'] = match_participant_label($node->entity_type, $entity);
    $participant['rank_ordinal'] = tournament_ordinal_suffix($participant['rank']);
    
    // Tournament stats
    $matches = tournament_match_count($node->entity_type, $participant['id']);
    $tournaments = tournament_tournament_count($node->entity_type, $participant['id']);
    
    $participant['matches'] = t('@count matches', array('@count' => $matches));
    $participant['tournaments'] = t('@count tournaments', array('@count' => $tournaments));
    
    if (!empty($matches)) {
      $ratio = match_win_ratio($node->entity_type, $participant['id']);
      $participant['matches'] .= ' (' . t('@ratio% win', array('@ratio' => $ratio)) . ')';
    }
  }
  
  $vars['participants'] = $participants;
  $vars['close_link'] = array(
    '#theme' => 'link',
    '#text' => t('Close'),
    '#options' => array(
      'attributes' => array('id' => 'knockout-close-rankings'),
      'fragment' => 'close-rankings',
      'html' => FALSE,
    ),
  );
}

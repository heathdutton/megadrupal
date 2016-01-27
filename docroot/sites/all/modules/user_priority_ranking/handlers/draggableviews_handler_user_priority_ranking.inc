<?php

/**
 * @file
 * User Priority Ranking implementation for draggableviews.
 */

$plugin = array(
  'label' => 'User Priority Ranking',
  'handler' => array(
    'class' => 'draggableviews_handler_user_priority_ranking',
  ),
);

/*
 * Implements using the User Priority Ranking module.
 * This module should extends draggableviews_handler_flag_weights to use its get
 * function.
 */
class draggableviews_handler_user_priority_ranking extends draggableviews_handler {

  /**
   * Get the weight value.
   *
   * @param object $field
   *   Draggableviews field handler. View is $field->view,
   *   to get a row $field->view->result[$index].
   * @param int $index
   *   Index of the row.
   *
   * @return int
   *   Weight value.
   */
  function get($field, $index) {
    $row = $field->view->result[$index];
    if ($flag = flag_get_flag($field->view->relationship['flag_content_rel']->options['flag'])) {

      $limit = variable_get('flag_limit_' . $flag->name . '_value', 10);
      // Returns a reverse order offset by flag limit.
      return (isset($row->{'flag_content_' . $flag->content_type . '_priority'})) ? ($limit - $row->{'flag_content_' . $flag->content_type . '_priority'}) : 0;
    }
    else {
      // @TODO remove the else. Pretty sure I don't need.
      return $index;
    }
  }
  /**
   * Save weight and priority value.
   *
   * @param array $form_state
   *   Attay of form state of the form.
   *   View object $form_state['values']['view'].
   */
  function set($form_state) {
    global $user;

    $fv = $form_state['values'];
    $view = $form_state['build_info']['args'][0];
    $relationship = $view->relationship['flag_content_rel'];
    $fid = $relationship->definition['extra'][0]['value'];
    $flag = flag_get_flag($relationship->options['flag']);

    // For global flags, use uid 0.
    $uid = $flag->global ? 0 : $user->uid;
    // Need to sort weights.  The values do not matter, its the order.
    uasort($fv['draggableviews'], '_sort_user_priority_ranking');
    $i = 0;
    // Save records to our flags table.
    foreach ($fv['draggableviews'] as $item) {
      // Make sure id is available.
      if (!isset($item['id'])) {
        continue;
      }

      $limit = variable_get('flag_limit_' . $flag->name . '_value', FALSE);
      // Set the priority.
      user_priority_ranking_set_priority($fid, $flag->content_type, $item['id'], $uid, $i++, $limit);
    }
  }
}

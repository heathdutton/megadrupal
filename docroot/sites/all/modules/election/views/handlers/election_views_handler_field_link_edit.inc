<?php
/**
 * @file
 * Field handler to present a link to edit an election.
 */

class election_views_handler_field_link_edit extends election_views_handler_field_link {
  /**
   * Overrides parent::construct().
   */
  public function construct() {
    parent::construct();
    $this->additional_fields['type'] = 'type';
    $this->additional_fields['uid'] = 'uid';
  }

  /**
   * Overrides parent::render_link().
   */
  public function render_link($data, $values) {
    // Ensure user has access to edit this election.
    $election = new stdClass();
    $election->election_id = $this->get_value($values, 'election_id');
    $election->type = $this->get_value($values, 'type');
    $election->uid = $this->get_value($values, 'uid');
    if (!entity_access('update', 'election', $election)) {
      return;
    }

    $this->options['alter']['make_link'] = TRUE;
    $this->options['alter']['path'] = "election/{$election->election_id}/edit";
    $this->options['alter']['query'] = drupal_get_destination();

    $text = !empty($this->options['text']) ? $this->options['text'] : t('Edit');
    return $text;
  }
}

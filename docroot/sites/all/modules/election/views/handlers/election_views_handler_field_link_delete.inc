<?php
/**
 * @file
 * Field handler to present a link to delete an election.
 */

class election_views_handler_field_link_delete extends election_views_handler_field_link {
  /**
   * Overrides parent::construct().
   */
  public function construct() {
    parent::construct();
    $this->additional_fields['type'] = 'type';
  }

  /**
   * Overrides parent::render_link().
   */
  public function render_link($data, $values) {
    // Ensure user has access to delete this election.
    $election = election_load($this->get_value($values, 'election_id'));
    $election->election_id = $this->get_value($values, 'election_id');
    $election->type = $this->get_value($values, 'type');
    if (!election_access('delete', $election)) {
      return;
    }

    $this->options['alter']['make_link'] = TRUE;
    $this->options['alter']['path'] = election_uri_path($election) . '/delete';
    $this->options['alter']['query'] = drupal_get_destination();

    $text = !empty($this->options['text']) ? $this->options['text'] : t('Delete');
    return $text;
  }
}

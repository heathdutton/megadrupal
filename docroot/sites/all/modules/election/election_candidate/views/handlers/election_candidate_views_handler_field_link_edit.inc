<?php
/**
 * @file
 * Field handler to present a link to edit the candidate.
 */

class election_candidate_views_handler_field_link_edit extends election_candidate_views_handler_field_link {

  /**
   * Overrides parent::construct().
   */
  public function construct() {
    parent::construct();
    $this->additional_fields['election_id'] = 'election_id';
    $this->additional_fields['type'] = 'type';
    $this->additional_fields['uid'] = 'uid';
  }

  /**
   * Overrides parent::render_link().
   */
  public function render_link($data, $values) {
    // Ensure user has access to edit this candidate.
    $candidate = election_candidate_load($this->get_value($values, 'candidate_id'));
    if (!entity_access('update', 'election_candidate', $candidate)) {
      return;
    }
    $path = election_candidate_uri_path($candidate);
    $this->options['alter']['make_link'] = TRUE;
    $this->options['alter']['path'] = $path . '/edit';
    $this->options['alter']['query'] = drupal_get_destination();
    $text = !empty($this->options['text']) ? $this->options['text'] : t('Edit');
    return $text;
  }

}

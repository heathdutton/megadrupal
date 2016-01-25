<?php
/**
 * @file
 * Field handler to present a link to edit the post.
 */

class election_post_views_handler_field_link_edit extends election_post_views_handler_field_link {

  /**
   * Overrides parent::construct().
   */
  public function construct() {
    parent::construct();
    $this->additional_fields['election_id'] = 'election_id';
    $this->additional_fields['type'] = 'type';
    $this->additional_fields['vstatus_inheritance'] = 'vstatus_inheritance';
  }

  /**
   * Overrides parent::render_link().
   */
  public function render_link($data, $values) {
    // Ensure user has access to edit this post. The post does not need to be
    // loaded in full for this, but the election ($post->election) does.
    $post = new stdClass();
    $post->post_id = $this->get_value($values, 'post_id');
    $post->election_id = $this->get_value($values, 'election_id');
    $post->election = election_load($post->election_id);
    $post->type = $this->get_value($values, 'type');
    $post->vstatus_inheritance = $this->get_value($values, 'vstatus_inheritance');
    if (!entity_access('update', 'election_post', $post)) {
      return;
    }
    $this->options['alter']['make_link'] = TRUE;
    $this->options['alter']['path'] = "election-post/{$post->post_id}/edit";
    $this->options['alter']['query'] = drupal_get_destination();
    $text = !empty($this->options['text']) ? $this->options['text'] : t('Edit');
    return $text;
  }

}

<?php
/**
 * Field handler to provide simple renderer that allows using a themed team link.
 *
 * @ingroup views_field_handlers
 */
class team_handler_field_team_name extends team_handler_field_team {

  function render_link($data, $values) {
    if (!empty($this->options['link_to_team'])) {
      $team = new stdClass();
      $team->tid = $this->get_value($values, 'tid');
      $team->name = $this->get_value($values);
            
      return l($team->name, 'team/' . $team->tid);
    }

    // Otherwise, there's no special handling, so return the data directly.
    return $data;
  }
}

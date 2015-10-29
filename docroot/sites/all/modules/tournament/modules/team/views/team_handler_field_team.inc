<?php

/**
 * Field handler to provide simple renderer that allows linking to a team.
 *
 * @ingroup views_field_handlers
 */
class team_handler_field_team extends views_handler_field {
  /**
   * Override init function to provide generic option to link to team.
   */
  function init(&$view, &$data) {
    parent::init($view, $data);
    if (!empty($this->options['link_to_team'])) {
      $this->additional_fields['tid'] = 'tid';
    }
  }

  function option_definition() {
    $options = parent::option_definition();
    $options['link_to_team'] = array('default' => TRUE);
    return $options;
  }

  /**
   * Provide link to team option
   */
  function options_form(&$form, &$form_state) {
    $form['link_to_team'] = array(
      '#title' => t('Link this field to its team'),
      '#description' => t("Enable to override this field's links."),
      '#type' => 'checkbox',
      '#default_value' => $this->options['link_to_team'],
    );
    parent::options_form($form, $form_state);
  }

  function render_link($data, $values) {
    if (!empty($this->options['link_to_team']) && user_access('access team contents') && ($tid = $this->get_value($values, 'tid')) && $data !== NULL && $data !== '') {
      $this->options['alter']['make_link'] = TRUE;
      $this->options['alter']['path'] = "team/" . $tid;
    }
    return $data;
  }

  function render($values) {
    $value = $this->get_value($values);
    return $this->render_link($this->sanitize_value($value), $values);
  }
}

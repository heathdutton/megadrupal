<?php
// $Id;

/**
 * @file
 *   Views field handler for last.fm description fields.
 */

/**
 * Views field handler for last.fm description fields.
 *
 * Last.fm description fields can contain HTML, which is typically run through
 * check_plain() and thus not rendered. Here we give the user the option to
 * override this behavior and render the HTML.
 */
class lastfm_field_description extends lastfm_field_text {
  function option_definition() {
    $options = parent::option_definition();
    $options['render_html'] = array('default' => FALSE);
    return $options;
  }

  function options_form(&$form, $form_state) {
    $form['render_html'] = array(
      '#title' => t('Render HTML'),
      '#description' => t('By default, HTML returned from last.fm is escaped and will not render. Checking this option will disable this behavior and allow the HTML to render. Note that this could be a security problem if last.fm\'s filtering of dangerous HTML has problems.'),
      '#type' => 'checkbox',
      '#default_value' => $this->options['render_html'],
      '#weight' => 1,
    );
    parent::options_form($form, $form_state);
  }

  function render($values) {
    if ($this->options['render_html']) {
      $value = $values->{$this->field};
    }
    else {
      $value = check_plain($values->{$this->field});
    }
    
    return $value;
  }
}

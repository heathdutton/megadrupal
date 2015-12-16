<?php

/**
 * @file
 * Class kaltura_views_handler_field_kaltura_thumb.
 */

/**
 * Thumbnail field handler class.
 */
class kaltura_views_handler_field_kaltura_thumb extends views_handler_field {

  function option_definition() {
    $options = parent::option_definition();

    $options['thumbnail_width'] = array('default' => '80');
    $options['thumbnail_height'] = array('default' => '45');
    $options['with_preview'] = array('default' => '');

    return $options;
  }

  function options_form(&$form, &$form_state) {
    parent::options_form($form, $form_state);
    $form['thumbnail_width'] = array(
        '#type' => 'textfield',
        '#title' => t('Thumbnail Width'),
        '#size' => 5,
        '#default_value' => !empty($this->options['thumbnail_width']) ? $this->options['thumbnail_width'] : '80',
    );
    $form['thumbnail_height'] = array(
        '#type' => 'textfield',
        '#title' => t('Thumbnail Height'),
        '#size' => 5,
        '#default_value' => !empty($this->options['thumbnail_height']) ? $this->options['thumbnail_height'] : '45',
    );
    $form['with_preview'] = array(
      '#type' => 'checkbox',
      '#title' => t('with poping preview'),
      '#default_value' => !empty($this->options['with_preview']) ? $this->options['with_preview'] : 0,
    );
  }

  function render($values) {
    $value = $values->{$this->field_alias};
    $width = !empty($this->options['thumbnail_width']) ? $this->options['thumbnail_width'] : 0;
    $height = !empty($this->options['thumbnail_height']) ? $this->options['thumbnail_height'] : 0;

    $path = $value . '/width/' . $width . '/height/' . $height;
    $thumb = theme('image', array('path' => $path));

    if ($this->options['with_preview']) {
      $thumb = theme('image', array('path' => $path, 'attributes' => array('class' => 'k-prev-rotate')));

      $prev_width = $width * 1.5;
      $prev_height = $height * 1.5;
      $prev_vars['path'] = $value . '/width/' . $prev_width . '/height/' . $prev_height;
      $prev_vars['attributes'] = array('class' => 'k-preview', 'style' => 'display: none;');
      $prev = theme('image', $prev_vars);

      return '<div class="thumb-with-prev">' . $thumb . $prev . '</div>';
    }

    return $thumb;
  }
}

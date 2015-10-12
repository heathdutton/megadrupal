<?php

/**
 * @file
 * Class kaltura_views_handler_field_kaltura_media.
 */

/**
 * Helper function: Return an array of formatter options for a field type.
 *
 * Borrowed from field_ui.
 */
function kaltura_views_field_view_formatter_options($field_type = NULL) {
  $options = &drupal_static(__FUNCTION__);

  if (!isset($options)) {
    $field_types = field_info_field_types();
    $options = array();
    foreach (field_info_formatter_types() as $name => $formatter) {
      foreach ($formatter['field types'] as $formatter_field_type) {
        // Check that the field type exists.
        if (isset($field_types[$formatter_field_type])) {
          $options[$formatter_field_type][$name] = $formatter['label'];
        }
      }
    }
  }

  if ($field_type) {
    return !empty($options[$field_type]) ? $options[$field_type] : array();
  }
  return $options;
}

/**
 * Media field handler class.
 */
class kaltura_views_handler_field_kaltura_media extends views_handler_field {

  function option_definition() {
    $options = parent::option_definition();
    $options['height'] = array('default' => '');
    $options['width'] = array('default' => '');
    $options['uiconf'] = array('default' => '');
    $options['delivery'] = array('default' => '');
    return $options;
  }

  function options_form(&$form, &$form_state) {
    parent::options_form($form, $form_state);

    $form['height'] = array(
      '#type' => 'textfield',
      '#title' => t('Player Height'),
      '#size' => 5,
      '#default_value' => !empty($this->options['height']) ? $this->options['height'] : '364',
    );
    $form['width'] = array(
      '#type' => 'textfield',
      '#title' => t('Player Width'),
      '#size' => 5,
      '#default_value' => !empty($this->options['width']) ? $this->options['width'] : '410',
    );
    $form['uiconf'] = array(
      '#type' => 'textfield',
      '#title' => t('Custom player uiconf'),
      '#size' => 5,
      '#default_value' => !empty($this->options['uiconf']) ? $this->options['uiconf'] : '',
      '#description' => t('If left empty the default uiconf will be used'),
    );
    $form['delivery'] = array(
      '#type' => 'select',
      '#title' => t('Please select delivery type'),
      '#options' => array(
        'HTTP' => 'Progressive Download (HTTP)',
        'RTMP' => 'Adaptive Streaming (RTMP)',
      ),
      '#default_value' => !empty($this->options['delivery']) ? $this->options['delivery'] : '',
    );
  }

  function render($values) {
    // TODO: see if we can move some of this process to the pre render part.
    $value = $values->{$this->field_alias};
    if (isset($value)) {
      $entity = kaltura_entry_load($value);

      $type = $entity->kaltura_media_type;
      $path = $entity->kaltura_thumbnail_url;
      $settings['kaltura_entry_video_display']['player_width'] = $this->options['width'];
      $settings['kaltura_entry_video_display']['player_height'] = $this->options['height'];
      $settings['kaltura_entry_video_display']['custom_player'] = !empty($this->options['uiconf']) ? $this->options['uiconf'] : '48501'; // TODO: should be defined as a constant.
      $settings['delivery'] = $this->options['delivery'];
      $seometa['description'] = $entity->kaltura_description;
      $seometa['title'] = $entity->kaltura_title;

      $embed = field_kaltura_build_embed($value, $type, $settings, NULL, NULL, $path, $seometa);
      return theme('kaltura_player_default', array('embed' => $embed, 'title' => $entity->kaltura_title, 'desc' => $entity->kaltura_description));
    }
    else {
      return '';
    }
  }
}

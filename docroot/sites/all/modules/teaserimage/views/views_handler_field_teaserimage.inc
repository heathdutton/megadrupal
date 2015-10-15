<?php

/**
 * Teaser Image: handler for Views.
 */
class views_handler_field_teaserimage extends views_handler_field {

  /**
   * Constructor to provide additional fields to add.
   *
   */
  function construct() {
    parent::construct();

    // Node nid for teaserimage rendering.
    $this->additional_fields['teaserimage_node_nid'] = array(
      'table' => 'node',
      'field' => 'nid',
    );
    // Node title for teaserimage rendering.
    $this->additional_fields['teaserimage_node_title'] = array(
      'table' => 'node',
      'field' => 'title',
    );
    // Node type to verify it is an teaserimage node.
    $this->additional_fields['teaserimage_node_type'] = array(
      'table' => 'node',
      'field' => 'type',
    );
  }

  function option_definition() {
    $options = parent::option_definition();
    $options['link_to_node'] = array('default' => TRUE);
    $options['original_picture'] = array('default' => FALSE);
    $options['preset'] = array('default' => '');
    return $options;
  }

  function options_form(&$form, &$form_state) {
    parent::options_form($form, $form_state);

    $form['link_to_node'] = array(
      '#title' => t('Link the thumbnail to its node'),
      '#type' => 'checkbox',
      '#default_value' => !empty($this->options['link_to_node']),
    );

    $form['original_picture'] = array(
      '#title' => t('Display original'),
      '#type' => 'checkbox',
      '#default_value' => !empty($this->options['original_picture']),
      '#description' => t('If checked, the view will display the original picture used to build the thumbnail without applying the image style preset.'),
    );

    $form['preset'] = array(
      '#type' => 'select',
      '#title' => t('Thumbnail preset'),
      '#default_value' => $this->options['preset'],
      '#description' => t('This preset will override any other teaserimage setting.'),
      '#options' => image_style_options(TRUE),
    );
  }

  function render($values) {
    if ($thumbnail = $values->{$this->field_alias}) {
      $node = new stdClass();
      $node->teaserimage = $thumbnail;
      $node->type = $values->{$this->aliases['teaserimage_node_type']};
      $node->title = $values->{$this->aliases['teaserimage_node_title']};
      $preset = $this->options['preset'];

      if (empty($this->options['original_picture'])) {
        if (empty($preset)) {
          $thumbnail = theme('teaserimage_thumbnail', array('node' => $node));
        }
        else {
          $thumbnail_options = array(
            'style_name' => $preset,
            'path' => $thumbnail,
            'alt' => $node->title,
            'title' => $node->title,
          );
          if ($class = variable_get('teaserimage_css_classes_' . $node->type, variable_get('teaserimage_css_classes', 'teaserimage'))) {
            $thumbnail_options['attributes']['class'] = $class;
          }
          $thumbnail = theme('image_style', $thumbnail_options);
        }
      }
      else {
        $thumbnail = theme('image', array('path' => $thumbnail));
      }
      if (!empty($this->options['link_to_node'])) {
        return l($thumbnail, 'node/' . $values->nid, array('html' => TRUE));
      }
      else {
        return $thumbnail;
      }
    }
  }
}


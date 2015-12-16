<?php

/**
 * @file
 * Contains the entity gallery view row style plugin.
 */

/**
 * Plugin which performs a entity_gallery_view on the resulting object.
 *
 * Most of the code on this object is in the theme function.
 *
 * @ingroup views_row_plugins
 */
class views_plugin_row_entity_gallery_view extends views_plugin_row {
  // Basic properties that let the row style follow relationships.
  var $base_table = 'entity_gallery';
  var $base_field = 'egid';

  // Stores the entity galleries loaded with pre_render.
  var $entity_galleries = array();

  function init(&$view, &$display, $options = NULL) {
    parent::init($view, $display, $options);
    // Handle existing views with the deprecated 'teaser' option.
    if (isset($this->options['teaser'])) {
      $this->options['build_mode'] = $this->options['teaser'] ? 'teaser' : 'full';
    }
    // Handle existing views which has used build_mode instead of view_mode.
    if (isset($this->options['build_mode'])) {
      $this->options['view_mode'] = $this->options['build_mode'];
    }
  }

  function option_definition() {
    $options = parent::option_definition();

    $options['view_mode'] = array('default' => 'teaser');
    $options['links'] = array('default' => TRUE, 'bool' => TRUE);

    return $options;
  }

  function options_form(&$form, &$form_state) {
    parent::options_form($form, $form_state);

    $options = $this->options_form_summary_options();
    $form['view_mode'] = array(
      '#type' => 'select',
      '#options' => $options,
      '#title' => t('View mode'),
      '#default_value' => $this->options['view_mode'],
     );
    $form['links'] = array(
      '#type' => 'checkbox',
      '#title' => t('Display links'),
      '#default_value' => $this->options['links'],
    );
  }

  /**
   * Return the main options, which are shown in the summary title.
   */
  function options_form_summary_options() {
    $entity_info = entity_get_info('entity_gallery');
    $options = array();
    if (!empty($entity_info['view modes'])) {
      foreach ($entity_info['view modes'] as $mode => $settings) {
        $options[$mode] = $settings['label'];
      }
    }
    if (empty($options)) {
      $options = array(
        'teaser' => t('Teaser'),
        'full' => t('Full content')
      );
    }

    return $options;
  }

  function summary_title() {
    $options = $this->options_form_summary_options();
    return check_plain($options[$this->options['view_mode']]);
  }

  function pre_render($values) {
    $egids = array();
    foreach ($values as $row) {
      $egids[] = $row->{$this->field_alias};
    }
    $this->entity_galleries = entity_gallery_load_multiple($egids);
  }

  function render($row) {
    if (isset($this->entity_galleries[$row->{$this->field_alias}])) {
      $entity_gallery = $this->entity_galleries[$row->{$this->field_alias}];
      $entity_gallery->view = $this->view;
      $build = entity_gallery_view($entity_gallery, $this->options['view_mode']);

      return drupal_render($build);
    }
  }
}
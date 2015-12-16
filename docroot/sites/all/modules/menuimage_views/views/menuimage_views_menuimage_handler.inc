<?php

/**
 * @file
 * Definition of menuimage_views_menuimage_handler.
 */

/**
 * Description of what my handler does.
 */
class menuimage_views_menuimage_handler extends views_handler_field {
  /**
   * Add some required fields needed on render().
   */
  function construct() {
    parent::construct();
    $this->additional_fields['menuimage_field'] = array(
      'table' => 'menu_links',
      'field' => 'options',
    );
  }

  /**
   * Loads additional fields.
   */
  function query() {
    $this->ensure_my_table();
    $this->add_additional_fields();
  }

  /**
   * Default options form.
   */
  function option_definition() {
    $options = parent::option_definition();
    $options['image_style'] = array('default' => '');
    return $options;
  }

  /**
   * Creates the form item for the options added.
   */
  function options_form(&$form, &$form_state) {
    parent::options_form($form, $form_state);

    $image_styles = image_style_options(FALSE, PASS_THROUGH);
    $form['image_style'] = array(
      '#title' => t('Image style'),
      '#type' => 'select',
      '#default_value' => $this->options['image_style'],
      '#empty_option' => t('None (original image)'),
      '#options' => $image_styles,
    );
  }

  /**
   * Renders the field handler.
   */
  function render($values) {
    $alias = $this->aliases['menuimage_field'];
    if (empty($values->{$alias})) {
      return;
    }
    $menu_link_options = unserialize($values->{$alias});
    if (empty($menu_link_options['content']['image'])) {
      return;
    }
    $fid = $menu_link_options['content']['image'];
    $file_data = file_load($fid);
    if (empty($file_data)) {
      return;
    }

    $vars = array(
      'path' => $file_data->uri,
      'width' => $file_data->metadata['width'],
      'height' => $file_data->metadata['height'],
      'attributes' => array('class' => array('menuimage')),
      // @todo do we care about these?
      //      'title',
      //      'alt',
    );
    if (empty($this->options['image_style'])) {
      // Original
      return theme('image', $vars);
    }
    // image_style
    $vars['style_name'] = $this->options['image_style'];
    return theme('image_style', $vars);
  }
}

?>
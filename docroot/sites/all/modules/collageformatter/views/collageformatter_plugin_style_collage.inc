<?php
/**
 * @file
 * Definition of the Views style plugin.
 */

/**
 * Class collageformatter_plugin_style_collage.
 */
class collageformatter_plugin_style_collage extends views_plugin_style {
  public function option_definition() {
    $options = parent::option_definition();
    $options += _collageformatter_default_settings();
    return $options;
  }

  /**
   * Provide the form for setting options.
   */
  public function options_form(&$form, &$form_state) {
    parent::options_form($form, $form_state);
    $form['#collageformatter'] = TRUE;
    $form += _collageformatter_settings_form($this->options);
    $form['image_link_image_style']['#states'] = array(
      'visible' => array(
        ':input[name="style_options[image_link]"]' => array('value' => 'file'),
      )
    );
    $form['image_link_modal']['#states'] = array(
      'visible' => array(
        ':input[name="style_options[image_link]"]' => array('value' => 'file'),
      ),
    );
    $form['image_link_class']['#states'] = array(
      'invisible' => array(
        ':input[name="style_options[image_link]"]' => array('value' => ''),
      ),
    );
    $form['image_link_rel']['#states'] = array(
      'invisible' => array(
        ':input[name="style_options[image_link]"]' => array('value' => ''),
      ),
    );
  }

  public function render() {
    $collage_fields = array();

    foreach ($this->view->field as $field_name => $field) {
      if (isset($field->field_info) && $field->field_info['type'] == 'image') {
        $collage_fields[] = $field->handler_type . '_' . $field_name;
      }
    }

    $settings = $this->options;
    $settings['gallery'] = isset($this->view->dom_id) ? $this->view->dom_id : $this->view->name;

    $images = array();
    foreach ($this->view->result as $result) {
      foreach($collage_fields as $field_name) {
        foreach ($result->{$field_name} as $element) {
          $images[$element['raw']['fid']] = $element['raw'];
          if ($settings['image_link'] == 'content') {
            $content_uri = entity_uri($result->_field_data['nid']['entity_type'], $result->_field_data['nid']['entity']);
            $images[$element['raw']['fid']]['content_uri'] = $content_uri['path'];
          }
        }
      }
    }
    if (empty($images)) {
      return '';
    }

    $output = collageformatter_render_collage($images, $settings);

    return $output;
  }
}

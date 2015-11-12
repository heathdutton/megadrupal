<?php

/**
 * @file
 * Definition of addressfield_link_handler_field_address_link.
 */

/**
 * Provides diferent link display options for address entity(field).
 * @ingroup views_field_handlers
 */
class addressfield_link_handler_field_address_link extends views_handler_field {

  /**
   * Option to address link field.
   */
  public function option_definition() {
    $options = parent::option_definition();

    $options['selected_link_type'] = array('default' => 2, 'bool' => TRUE);

    return $options;
  }

  /**
   * Link to Addressfiels link fileld option form.
   */
  public function options_form(&$form, &$form_state) {
    $form['selected_link_type'] = array(
        '#type' => 'select',
        '#title' => t('Link type'),
        '#options' => array(
            1 => t('link_title'),
            2 => t('link_url'),
        ),
        '#default_value' => $this->options['selected_link_type'],
        '#description' => t("Address field contain link titles and URL's. Have to select one from them."),
    );

    parent::options_form($form, $form_state);
  }

  /**
   * Render the addresfields link field.
   */
  public function render($values) {
    $link = unserialize($this->get_value($values));
    $link_title = $link['link_title'];
    $link_url = $link['link_url'];
    switch ($this->options['selected_link_type']) {
      case 1:
        return $link_title;
        break;
      case 2:
        return $link_url;
        break;
    }
  }

}

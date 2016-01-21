<?php

/**
 * Field handler to present a card data's operations links.
 */
class commerce_cardonfile_handler_field_card_operations extends views_handler_field {
  function construct() {
    parent::construct();

    $this->additional_fields['card_id'] = 'card_id';
    $this->additional_fields['uid'] = 'uid';
  }

  function option_definition() {
    $options = parent::option_definition();

    $options['add_destination'] = TRUE;

    return $options;
  }

  function options_form(&$form, &$form_state) {
    parent::options_form($form, $form_state);

    $form['add_destination'] = array(
      '#type' => 'checkbox',
      '#title' => t('Add a destination parameter to operation links so users return to this View on form submission.'),
      '#default_value' => $this->options['add_destination'],
    );
  }

  function query() {
    $this->ensure_my_table();
    $this->add_additional_fields();
  }

  function render($values) {
    $card_id = $this->get_value($values, 'card_id');
    $uid = $this->get_value($values, 'uid');

    // Get the operations links.
    $links = menu_contextual_links('commerce-cardonfile', 'user/' . $uid . '/cards', array($card_id));

    if (!empty($links)) {
      // Add the destination to the links if specified.
      if ($this->options['add_destination']) {
        foreach ($links as $id => &$link) {
          // Only include the destination for the edit and delete forms.
          if (in_array($id, array('commerce-cardonfile-edit', 'commerce-cardonfile-delete'))) {
            $link['query'] = drupal_get_destination();
          }
        }
      }

      drupal_add_css(drupal_get_path('module', 'commerce_cardonfile') . '/theme/commerce_cardonfile.admin.css');
      return theme('links', array('links' => $links, 'attributes' => array('class' => array('links', 'inline', 'operations'))));
    }
  }
}

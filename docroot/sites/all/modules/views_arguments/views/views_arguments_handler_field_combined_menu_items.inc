<?php

class views_arguments_handler_field_combined_menu_items extends views_arguments_views_handler_field_links {

  /**
   * Overrides views_handler_field::option_definition().
   */
  function option_definition() {
    $options = parent::option_definition();

    $options['delimiter'] = array('default' => ' | ');

    return $options;
  }

  /**
   * Overrides views_handler_field::options_form().
   */
  function options_form(&$form, &$form_state) {
    parent::options_form($form, $form_state);

    $form['delimiter'] = array(
      '#type' => 'textfield',
      '#title' => t('Link delimiter'),
      '#default_value' => $this->options['delimiter'],
      '#description' => t('Can contain HTML. Will not be translated.'),
    );
  }

  /**
   * Overrides views_handler_field::query().
   */
  function query() {}

  /**
   * Overrides views_handler_field::render().
   */
  function render($values) {
    $links = $this->get_links();

    if ($this->options['check_access']) {
      $links = array_filter($links, function($link) {
        if ($link['href']) {
          $item = menu_get_item($link['href']);
          if ($item && $item['access']) {
            return TRUE;
          }
        }
      });
    }

    if ($links) {
      $links = array_map(function($link) {
        return $link['href'] ? l($link['title'], $link['href'], @$link['options'] ?: array()) : check_plain($link['title']);
      }, $links);
      return implode($this->options['delimiter'], $links);
    }

    return '';
  }

}

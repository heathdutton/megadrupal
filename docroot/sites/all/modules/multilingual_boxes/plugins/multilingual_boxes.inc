<?php

/**
 * @file
 * Provides a new box-type that provides multilingual box
 */

/**
 * Multilingual box.
 */
class multilingual_boxes extends boxes_box {

  /**
   * Implementation of boxes_box::options_defaults().
   */
  public function options_defaults() {

    $languages = language_list('language');
    $options_array = array();
    foreach ($languages as $langcode => $language) {
       $elemento = array();
       $elemento['value'] = '';
       $options_array['title_' . $langcode] = '';
       $options_array['text_' . $langcode] = $elemento;
    }

    return $options_array;
  }

  /**
   * Implementation of boxes_box::options_form().
   */
  public function options_form(&$form_state) {
    $format =  filter_default_format();
    $form = array();

    $form['additional_settings'] = array(
      '#type' => 'vertical_tabs',
      '#weight' => 99,
    );

    $languages = language_list('language');

    foreach ($languages as $langcode => $language) {
      $form[$language->language] = array(
        '#type' => 'fieldset',
        '#title' => $language->name,
        '#collapsible' => TRUE,
        '#collapsed' => TRUE,
        '#group' => 'additional_settings',
      );
      $form[$language->language]['title_' . $langcode] = array(
        '#type' => 'textfield',
        '#title' => t('Box title (@language)', array('@language' => $language->name)),
        '#default_value' => $this->options['title_' . $langcode],
      );

      $form[$language->language]['text_' . $langcode] = array(
        '#type' => 'text_format',
        '#base_type' => 'textarea',
        '#title' => t('Box title (@language)', array('@language' => $language->name)),
        '#format' => $format,
        '#default_value' => $this->options['text_' . $langcode]['value'],
      );
    }

    return $form;
  }

  /**
   * Implementation of boxes_box::render().
   */
  public function render() {
    global $language;
    $render_array = array();
    $title = $this->title;
    $content= '';
    if (isset($this->options['text_' . $language->language])) {
      $content = check_markup($this->options['text_' . $language->language]['value'], $this->options['text_' . $language->language]['format'], $langcode = $language->language, FALSE);
    }
    if (isset($this->options['title_' . $language->language])) {
      $title = filter_xss($this->options['title_' . $language->language]);
    }
    $box = array(
      'delta' => $this->delta,
      'title' => $title,
      'subject' => $title,
      'content' => $content,
      'is_empty' => FALSE,
    );

    return $box;
  }

}


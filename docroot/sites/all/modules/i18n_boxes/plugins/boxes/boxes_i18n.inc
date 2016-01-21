<?php

/**
 * Simple translatable custom text box.
 */
class boxes_i18n extends boxes_simple {

  /**
   * Implements boxes_content::options_defaults().
   */
  public function options_defaults() {
    $defaults = parent::options_defaults();
    return $defaults + array(
      'i18n_boxes' => array(
        'language' => I18N_BOXES_LOCALIZE,
      ),
    );
  }

  /**
   * Implements boxes_content::options_form().
   */
  public function options_form(&$form_state) {
    $form = parent::options_form($form_state);
    $form['i18n_boxes'] = array(
      '#type' => 'fieldset',
      '#title' => t('Multilingual settings'),
      '#collapsible' => TRUE,
      '#weight' => 0,
      '#tree' => TRUE, // To avoid conflict with i18n_blocks.
    );

    $options = array('' => t('All languages'));
    $options[I18N_BOXES_LOCALIZE] = t('All languages (Translatable)');
    $options += locale_language_list('name');

    $form['i18n_boxes']['language'] = array(
      '#type' => 'radios',
      '#title' => t('Language'),
      '#default_value' => $this->options['i18n_boxes']['language'],
      '#options' => $options,
    );

    return $form;
  }

  /**
   * Save a box.
   */
  public function save() {
    parent::save();
    // Make the box strings available to be translated.
    $this->locale_refresh();
  }

  /**
   * Refresh the translatable strings.
   */
  public function locale_refresh() {
    if ($this->options['i18n_boxes']['language'] == I18N_BOXES_LOCALIZE) {

      // Update the box's body field; if empty, remove the translation.
      if (!empty($this->options['body']['value'])) {
        i18n_string_update(array('boxes', $this->plugin_key, $this->delta, 'body'), $this->options['body']['value'], array('format' => $this->options['body']['format']));
      }
      else {
        i18n_string_remove(array('boxes',  $this->plugin_key, $this->delta, 'body'));
      }

      // Update the box's title field; if empty, remove the translation.
      if (!empty($this->title)) {
        i18n_string_update(array('boxes', $this->plugin_key, $this->delta, 'title'), $this->title);
      } else {
        i18n_string_remove(array('boxes',  $this->plugin_key, $this->delta, 'title'));
      }
    }
    return TRUE;
  }

  /**
   * Implements boxes_content::render().
   */
  public function render() {
    global $language;
    // Get the filtered rendered data from boxes_simple.
    $block = parent::render();
    // Handle any multilingual settings.
    if (!empty($this->options['i18n_boxes']['language'])) {
      if ($this->options['i18n_boxes']['language'] == I18N_BOXES_LOCALIZE) {
        // This box is available to translate.
        $block['content'] = i18n_string_text(array('boxes', $this->plugin_key, $this->delta, 'body'), $block['content'], array('format' => $this->options['body']['format']));
        // Also translate the title.
        $block['subject'] = i18n_string_plain(array('boxes', $this->plugin_key, $this->delta, 'title'), $block['subject']);
        $block['title'] = $block['subject'];
      }
      elseif ($this->options['i18n_boxes']['language'] != $language->language) {
        // This box does not share the language of the user, so do not display.
        $block = NULL;
      }
    }
    return $block;
  }

  public function cache_setting() {
    return DRUPAL_CACHE_CUSTOM;
  }
}

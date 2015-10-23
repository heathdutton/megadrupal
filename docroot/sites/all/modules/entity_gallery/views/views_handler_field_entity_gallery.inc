<?php

/**
 * @file
 * Contains the basic 'entity_gallery' field handler.
 */

/**
 * Field handler to provide simple renderer that allows linking to an entity
 * gallery.
 *
 * Definition terms:
 * - link_to_entity_gallery default: Should this field have the checkbox "link to entity gallery" enabled by default.
 *
 * @ingroup views_field_handlers
 */
class views_handler_field_entity_gallery extends views_handler_field {

  function init(&$view, &$options) {
    parent::init($view, $options);
    // Don't add the additional fields to groupby
    if (!empty($this->options['link_to_entity_gallery'])) {
      $this->additional_fields['egid'] = array('table' => 'entity_gallery', 'field' => 'egid');
      if (module_exists('translation')) {
        $this->additional_fields['language'] = array('table' => 'entity_gallery', 'field' => 'language');
      }
    }
  }

  function option_definition() {
    $options = parent::option_definition();
    $options['link_to_entity_gallery'] = array('default' => isset($this->definition['link_to_entity_gallery default']) ? $this->definition['link_to_entity_gallery default'] : FALSE, 'bool' => TRUE);
    return $options;
  }

  /**
   * Provide link to entity gallery option
   */
  function options_form(&$form, &$form_state) {
    $form['link_to_entity_gallery'] = array(
      '#title' => t('Link this field to the original gallery'),
      '#description' => t("Enable to override this field's links."),
      '#type' => 'checkbox',
      '#default_value' => !empty($this->options['link_to_entity_gallery']),
    );

    parent::options_form($form, $form_state);
  }

  /**
   * Render whatever the data is as a link to the entity gallery.
   *
   * Data should be made XSS safe prior to calling this function.
   */
  function render_link($data, $values) {
    if (!empty($this->options['link_to_entity_gallery']) && !empty($this->additional_fields['egid'])) {
      if ($data !== NULL && $data !== '') {
        $this->options['alter']['make_link'] = TRUE;
        $this->options['alter']['path'] = "gallery/" . $this->get_value($values, 'egid');
        if (isset($this->aliases['language'])) {
          $languages = language_list();
          $language = $this->get_value($values, 'language');
          if (isset($languages[$language])) {
            $this->options['alter']['language'] = $languages[$language];
          }
          else {
            unset($this->options['alter']['language']);
          }
        }
      }
      else {
        $this->options['alter']['make_link'] = FALSE;
      }
    }
    return $data;
  }

  function render($values) {
    $value = $this->get_value($values);
    return $this->render_link($this->sanitize_value($value), $values);
  }
}

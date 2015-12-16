<?php

/**
 * @file
 * Definition of views_handler_field_entity_gallery_revision.
 */

/**
 * Contains the basic 'entity_gallery_revision' field handler.
 */

/**
 * A basic entity_gallery_revision handler.
 *
 * @ingroup views_field_handlers
 */
class views_handler_field_entity_gallery_revision extends views_handler_field_entity_gallery {
  function init(&$view, &$options) {
    parent::init($view, $options);
    if (!empty($this->options['link_to_entity_gallery_revision'])) {
      $this->additional_fields['vid'] = 'vid';
      $this->additional_fields['egid'] = 'egid';
      if (module_exists('translation')) {
        $this->additional_fields['language'] = array('table' => 'entity_gallery', 'field' => 'language');
      }
    }
  }
  function option_definition() {
    $options = parent::option_definition();
    $options['link_to_entity_gallery_revision'] = array('default' => FALSE, 'bool' => TRUE);
    return $options;
  }

  /**
   * Provide link to revision option.
   */
  function options_form(&$form, &$form_state) {
    $form['link_to_entity_gallery_revision'] = array(
      '#title' => t('Link this field to its gallery revision'),
      '#description' => t('This will override any other link you have set.'),
      '#type' => 'checkbox',
      '#default_value' => !empty($this->options['link_to_entity_gallery_revision']),
    );
    parent::options_form($form, $form_state);
  }

  /**
   * Render whatever the data is as a link to the entity gallery.
   *
   * Data should be made XSS safe prior to calling this function.
   */
  function render_link($data, $values) {
    if (!empty($this->options['link_to_entity_gallery_revision']) && $data !== NULL && $data !== '') {
      $this->options['alter']['make_link'] = TRUE;
      $egid = $this->get_value($values, 'egid');
      $vid = $this->get_value($values, 'vid');
      $this->options['alter']['path'] = 'gallery/' . $egid;
      if ($egid != $vid) {
        $this->options['alter']['path'] .= "/revisions/$vid/view";
      }
      if (module_exists('translation')) {
        $language = $this->get_value($values, 'language');
        $languages = language_list();
        if (isset($languages[$language])) {
          $this->options['alter']['language'] = $languages[$language];
        }
      }
    }
    else {
      return parent::render_link($data, $values);
    }
    return $data;
  }
}

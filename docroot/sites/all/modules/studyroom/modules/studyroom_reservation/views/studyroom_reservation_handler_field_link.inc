<?php

/**
 * @file
 * Definition of studyroom_reservation_handler_field_link.
 */

/**
 * Contains a field handler to display links to entities as fields.
 */
class studyroom_reservation_handler_field_link extends views_handler_field_entity {
  /**
   * Add text to options list.
   */
  public function option_definition() {
    $options = parent::option_definition();
    $options['text'] = array('default' => '', 'translatable' => TRUE);
    return $options;
  }

  /**
   * Construct text form element.
   */
  public function options_form(&$form, &$form_state) {
    parent::options_form($form, $form_state);

    $form['text'] = array(
      '#type' => 'textfield',
      '#title' => t('Text to display'),
      '#default_value' => $this->options['text'],
    );

    // The path is set by render_link function so don't allow to set it.
    $form['alter']['path'] = array('#access' => FALSE);
    $form['alter']['external'] = array('#access' => FALSE);
  }

  /**
   * Pre-render.
   */
  public function pre_render(&$values) {
    $this->entity_info = entity_get_info($this->entity_type);
    parent::pre_render($values);
  }

  /**
   * Render.
   */
  public function render($values) {
    $entity = $this->get_value($values);
    if ($entity) {
      return $this->render_link($entity, $values);
    }
  }

  /**
   * Render a link to the reservation.
   */
  public function render_link($entity, $values) {
    if (entity_access('view', $this->entity_type, $entity)) {
      $this->options['alter']['make_link'] = TRUE;
      $uri = entity_uri($this->entity_type, $entity);
      $this->options['alter']['path'] = $uri['path'];
      $this->options['alter']['options'] = $uri['options'];
      $text = !empty($this->options['text']) ? $this->options['text'] : t('view');
      return $text;
    }
  }
}

<?php

/**
 * @file
 * Provides a new box-type that provides Views as boxes.
 */

/**
 * Views content listing box.
 */
class entity_boxes_entity extends boxes_box {
  /**
   * Implementation of boxes_box::options_defaults().
   */
  public function options_defaults() {
    return array();
  }

  /**
   * we use fancy ajax on our options form and this breaks with the context ui
   */
  function use_multistep_create() {
    return TRUE;
  }

  /**
   * Overrides boxes_box::cache_setting().
   *
   * This delegates each Views Box instance's cache setting to the value on the
   * View's Block display 'Block cache' value.
   */
  public function cache_setting() {
    return DRUPAL_CACHE_CUSTOM;
  }

  /**
   *  Implementation of boxes_box::options_form().
   *  @see boxes_form instead
   */
  public function options_form(&$form_state) {
    drupal_add_js(drupal_get_path('module', 'entity_boxes') . '/entity_boxes.js');
    $entity_boxes = entity_boxes_load_delta($form_state['box']->delta);
    $form = array();
    $form = entity_boxes_edit_form($form, $form_state, $entity_boxes);
    
    return $form;
  }

  public function control_form() {
    return TRUE;
  }

  /**
   *  Implements Boxes_Box::options_submit().
   *  Allows us to save our extra data that boxes won't know about
   */
  public function options_submit(&$form, &$form_state) {
    $form_state['values']['name'] = $form_state['values']['delta'];
    entity_boxes_edit_form_submit($form, $form_state);
    return $form;
  }
  
  /**
   * Implements Boxes_Box::save().
   * Allows us to prune entity fields from our box options.
   */
  public function save() {
    $fields = field_info_instances('entity_boxes', $this->plugin_key);
    foreach($fields as $field_id => $field_data) {
      unset($this->options[$field_id]);
    }
    // Save the box via the default implementation.
    parent::save();
  }

  /**
   * Implementation of boxes_box::render().
   */
  public function render() {
    $title = "";
    $content = "";
    $entity_boxes = entity_boxes_load_delta($this->delta);
    if (is_array($entity_boxes) && !empty($entity_boxes)) {
      $entity_boxes = array_pop($entity_boxes);
      $controller = entity_get_controller('entity_boxes');  
      $content = $controller->view(array($entity_boxes->entity_boxes_id => $entity_boxes));
    }
    
    $box = array(
      'delta' => $this->delta, // Crucial.
      'title' => $this->title,
      'subject' => $this->title,
      'content' => render($content),
      'is_empty' => FALSE,
    );

   
    return $box;
  }

  /**
   * Implements boxes_box::delete().
   *
   * Delete the entity associated with this box when the box is deleted.
   */
  public function delete() {
    $entity_boxes = entity_boxes_load_delta($this->delta);
    if (is_array($entity_boxes)) {
      foreach ($entity_boxes as $eb) {
        entity_boxes_delete_multiple(array($eb->entity_boxes_id));
      }
    }
    parent::delete();
  }
}

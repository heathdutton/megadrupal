<?php
/**
 * @file
 * Field handler to present the comment body with Comment Easy Reply tooltips.
 *
 * @ingroup views_field_handlers
 */

/**
 * Views field handler for comment body.
 */
class views_handler_field_comment_easy_reply_comment_body extends views_handler_field_field {

  /**
   * Defines an additional option definition for the Comment Easy Reply module.
   */
  function option_definition() {
    $options = parent::option_definition();
    $field = field_info_field($this->definition['field_name']);
    $field_type = field_info_field_types($field['type']);
    $column_names = array_keys($field['columns']);
    $default_column = 1;
    if (count($column_names) == 1) {
      $default_column = $column_names[0];
    }
    elseif (in_array('value', $column_names)) {
      $default_column = 'value';
    }
    // Defining a new option in comment_body field form.
    $options['comment_easy_reply_format'] = array(
      'default' => $default_column,
    );
    return $options;
  }

  /**
   * The comment_body field's option form.
   */
  function options_form(&$form, &$form_state) {
    parent::options_form($form, $form_state);
    $form['comment_easy_reply_format'] = array(
      '#title' => t('Process with Comment Easy Reply filter'),
      '#type' => 'checkbox',
      '#default_value' => $this->options['comment_easy_reply_format'],
      '#description' => t('If checked, comment body will be parsed by Comment Easy Reply module, turning every @#NUM into links.'),
      '#weight' => 20,
    );
  }

  /**
   * Replace comment reference occurrence with the appropriate tooltip.
   */
  function set_items($values, $row_id) {
    if (empty($values->_field_data[$this->field_alias]) || empty($values->_field_data[$this->field_alias]['entity'])) {
      return array();
    }
    $display = array(
      'type' => $this->options['type'],
      'settings' => $this->options['settings'],
      'label' => 'hidden',
      // Pass the View object in the display so that fields can act on it.
      'views_view' => $this->view,
      'views_field' => $this,
      'views_row_id' => $row_id,
    );
    $entity_type = $values->_field_data[$this->field_alias]['entity_type'];
    $entity = $this->get_value($values, 'entity');
    if (!$entity) {
      return array();
    }
    $langcode = $this->field_language($entity_type, $entity);
    $render_array = field_view_field($entity_type, $entity, $this->definition['field_name'], $display, $langcode);
    if (isset($this->options['comment_easy_reply_format']) && $this->options['comment_easy_reply_format']) {
      $render_array[0]['#markup'] = _comment_easy_reply_comment_body_replace($render_array[0]['#markup'], $entity->nid);
      $variables = array('comment' => $entity);
      comment_easy_reply_add_js($variables);
      comment_easy_reply_add_css($variables);
    }
    $items = array();
    if ($this->options['field_api_classes']) {
      // Make a copy.
      $array = $render_array;
      return array(array('rendered' => drupal_render($render_array)));
    }
    foreach (element_children($render_array) as $count) {
      $items[$count]['rendered'] = $render_array[$count];
      // field_view_field() adds an #access property to the render array that
      // determines whether or not the current user is allowed to view the
      // field in the context of the current entity. We need to respect this
      // parameter when we pull out the children of the field array for
      // rendering.
      if (isset($render_array['#access'])) {
        $items[$count]['rendered']['#access'] = $render_array['#access'];
      }
      // Only add the raw field items (for use in tokens) if the curent user
      // has access to view the field content.
      if ((!isset($items[$count]['rendered']['#access']) || $items[$count]['rendered']['#access']) && !empty($render_array['#items'][$count])) {
        $items[$count]['raw'] = $render_array['#items'][$count];
      }
    }
    return $items;
  }
}

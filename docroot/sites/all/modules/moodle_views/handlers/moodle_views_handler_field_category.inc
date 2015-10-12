<?php


/**
 * @file
 * Definition of a field handler for Moodle course categories.
 */

/**
 * Field handler that allows linking to a course category.
 *
 * Definition terms:
 * - link_to_category default: Should this field have the checkbox "Link to
 * the Moodle category page" enabled by default.
 *
 * @ingroup views_field_handlers
 */
class moodle_views_handler_field_category extends views_handler_field {

  /**
   * Object initialisation.
   */
  public function init(&$view, &$options) {
    parent::init($view, $options);
    if (!empty($this->options['link_to_category'])) {
      $this->additional_fields['id'] = array(
        'table' => 'course',
        'field' => 'category');
    }
  }

  /**
   * Define the "link to category" option.
   */
  public function option_definition() {
    $options = parent::option_definition();
    if (isset($this->definition['link_to_category default'])) {
      $default = $this->definition['link_to_category default'];
    }
    else {
      $default = FALSE;
    }
    $options['link_to_category'] = array(
      'default' => $default,
      'bool' => TRUE);
    return $options;
  }

  /**
   * Provide the form element for the "link to category" option.
   */
  public function options_form(&$form, &$form_state) {
    $form['link_to_category'] = array(
      '#title' => t('Link to the Moodle category page'),
      '#description' => t("Enable to override this field's links."),
      '#type' => 'checkbox',
      '#default_value' => !empty($this->options['link_to_category']));

    parent::options_form($form, $form_state);
  }

  /**
   * Render whatever the data is as a link to the category.
   *
   * Data should be made XSS safe prior to calling this function.
   */
  public function render_link($data, $values) {
    if (!empty($this->options['link_to_category']) && !empty($this->additional_fields['id'])) {
      if ($data !== NULL && $data !== '') {
        $path = variable_get('moodle_connector_url');
        $path .= "/course/category.php?id=" . $this->get_value($values, 'id');
        $this->options['alter']['make_link'] = TRUE;
        $this->options['alter']['path'] = $path;
      }
      else {
        $this->options['alter']['make_link'] = FALSE;
      }
    }
    return $data;
  }

  /**
   * Render the field.
   */
  public function render($values) {
    $value = $this->get_value($values);
    return $this->render_link($this->sanitize_value($value), $values);
  }
}

<?php


/**
 * @file
 * Definition of a field handler for Moodle courses.
 */

/**
 * Field handler that allows linking to a Moodle course.
 *
 * Definition terms:
 * - link_to_course default: Should this field have the checkbox "Link to
 *   the Moodle course page" enabled by default.
 *
 * @ingroup views_field_handlers
 */
class moodle_views_handler_field_course extends views_handler_field {

  /**
   * Overrides init().
   */
  public function init(&$view, &$options) {
    parent::init($view, $options);
    if (!empty($this->options['link_to_course'])) {
      $this->additional_fields['id'] = array(
        'table' => 'course',
        'field' => 'id');
    }
  }

  /**
   * Defines the "link to course" display option.
   */
  public function option_definition() {
    $options = parent::option_definition();
    $options['link_to_course'] = array(
      'default' => isset($this->definition['link_to_course default']) ? $this->definition['link_to_course default'] : FALSE,
      'bool' => TRUE);
    return $options;
  }

  /**
   * Provides form element for "link to course" display option.
   */
  public function options_form(&$form, &$form_state) {
    $form['link_to_course'] = array(
      '#title' => t('Link to the Moodle course page'),
      '#description' => t("Enable to override this field's links."),
      '#type' => 'checkbox',
      '#default_value' => !empty($this->options['link_to_course']));

    parent::options_form($form, $form_state);
  }

  /**
   * Renders whatever the data is as a link to the course.
   *
   * Data should be made XSS safe prior to calling this function.
   */
  protected function render_link($data, $values) {
    if (!empty($this->options['link_to_course']) && !empty($this->additional_fields['id'])) {
      if ($data !== NULL && $data !== '') {
        $this->options['alter']['make_link'] = TRUE;
        $this->options['alter']['path'] = variable_get('moodle_connector_url') . "/course/view.php?id=" . $this->get_value($values, 'id');
      }
      else {
        $this->options['alter']['make_link'] = FALSE;
      }
    }
    return $data;
  }

  /**
   * Renders the field.
   */
  public function render($values) {
    $value = $this->get_value($values);
    return $this->render_link($this->sanitize_value($value), $values);
  }
}

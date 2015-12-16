<?php

/**
 * @file
 * Main abstract class for Url formatter module.
 */

abstract class UrlFormatter {

  /**
   * The formatter settings.
   *
   * We need this information for creating correctly format of url.
   *
   * @var array
   */
  public $settings;

  /**
   * The human name of formatter.
   *
   * We need this for use in formatter summary.
   *
   * @var string
   */
  public $name;


  /**
   * Build form for formatter settings.
   *
   * All params passed as in hook_field_formatter_settings_form(),
   *
   * @return array
   *   empty array.
   *
   * @see url_formatter_field_formatter_settings_form()
   */
  public function form($field, $instance, $view_mode, $form, &$form_state) {
    $display = $instance['display'][$view_mode];
    $this->settings = $display['settings'][get_class($this)];
    return array();
  }


  /**
   * Add visibility for form elements in formatter settings form.
   *
   * Added for better usability of formatter,
   *
   * @param int $field_name
   *   name of field for which current formatter is added.
   *
   * @param int $name
   *   name of chekcbox that enables current formatter.
   *
   * @return array
   *   form #states.
   *
   * @see url_formatter_field_formatter_settings_form()
   */
  public function visibility($field_name, $name = 'enable') {
    return array(
      'visible' => array(
        ':input[name="fields[' . $field_name . '][settings_edit_form][settings][' . get_class($this) . '][' . $name . ']"]' => array('checked' => TRUE),
      ),
    );
  }


  /**
   * Build formatter summary.
   *
   * All params passed as in hook_field_formatter_settings_summary().
   *
   * @return array
   *   drupal render array.
   *
   * @see url_formatter_field_formatter_settings_summary()
   */
  public function summary($field, $instance, $view_mode) {
    $display = $instance['display'][$view_mode];
    $this->settings = $display['settings'][get_class($this)];
    $child_class = get_class($this);
    $items = array();
    if ($this->settings['enable']) {
      foreach ($child_class::defaultValue() as $name => $value) {
        if ($name == 'enable') {
          continue;
        }
        $array = array();
        $form = $this->form($field, $instance, $view_mode, array(), $array);
        $items[] = $form[$name]['#title'] . ': ' . $this->settings[$name];
      }
      return array(
        '#theme' => 'item_list',
        '#title' => $this->name,
        '#type'  => 'ul',
        '#items' => $items,
      );
    }
  }


  /**
   * Build a renderable array for a field value.
   *
   * All params passed as in hook_field_formatter_view().
   *
   * @return string
   *   output string.
   *
   * @see url_formatter_field_formatter_view()
   */
  public function view($entity_type, $entity, $field, $instance, $langcode, $items, $display, $delta, $output) {
    $this->settings = $display['settings'][get_class($this)];
    return $output;
  }


  /**
   * Regular expression for search url of current formatter.
   *
   * @return string
   *   Regular expression.
   */
  abstract public function regExp();


  /**
   * Build default values for settings in hook_field_formatter_info().
   *
   * @return array
   *   array with settings.
   *
   * @see url_formatter_field_formatter_info()
   */
  static public function defaultValue() {
    return array();
  }

}

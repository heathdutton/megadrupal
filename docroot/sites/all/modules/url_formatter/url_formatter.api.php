<?php

/**
 * @file
 * Url formatter API Documentation.
 */

/**
 * Example how to add your custom url formatter.
 */

/**
 * Implements hook_field_formatter_info_alter().
 */
function url_formatter_example_field_formatter_info_alter(&$info) {

  // Setting name must be as class name.
  $info['url_formatter']['settings']['UrlFormatterExample'] = UrlFormatterExample::defaultValue();
}


/**
 * Url formatter example class.
 */
class UrlFormatterExample extends UrlFormatter {

  /**
   * The human name of formatter.
   *
   * @see UrlFormatter::name.
   */
  public $name = 'Example';


  /**
   * Implements UrlFormatter::form().
   */
  public function form($field, $instance, $view_mode, $form, &$form_state) {
    $element = parent::form($field, $instance, $view_mode, $form, $form_state);

    $element['enable'] = array(
      '#title' => t('Example'),
      '#type' => 'checkbox',
      '#default_value' => $this->settings['enable'],
    );

    $element['some_settings'] = array(
      '#type' => 'textfield',
      '#title' => t('Some settings'),
      '#default_value' => $this->settings['some_settings'],
      '#states' => $this->visibility($field['field_name'])
    );

    return $element;
  }


  /**
   * Implements UrlFormatter::view().
   */
  public function view($entity_type, $entity, $field, $instance, $langcode, $items, $display, $delta, $output) {
    $output = parent::view($entity_type, $entity, $field, $instance, $langcode, $items, $display, $delta, $output);
    preg_match_all($this->regExp(), $output, $matches);
    if (!empty($matches[0])) {
      foreach (array_unique($matches[0]) as $url) {

        // One can change url to something else..
        $example = 'Foo';
        $output = str_replace($url, $example, $output);
      }
    }
    return $output;
  }


  /**
   * Implements UrlFormatter::defaultValue().
   */
  static public function defaultValue() {
    return array(
      'enable' => 1,
      'some_settings' => 'Yes',
    );
  }


  /**
   * Implements UrlFormatter::regExp().
   */
  public function regExp() {
    return 'https://(www\.)?drupal\.org/(\d+)';
  }

}

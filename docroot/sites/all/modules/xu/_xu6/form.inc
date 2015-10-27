<?php
/**
 * @file
 * Helpers for working with forms
 */
$plugin = array (
  'title' => t('Form'),
  'description' => t('Helper for manipulating Form API arrays.'),
  'xu_type' => 'helper',
  'class' => 'XuForm'
);

/**
 * The XuForm helper
 *
 * XuForm is useful for creating and working with form arrays. It aims to
 * simplify Drupal form creation.
 *
 *
 */
class XuForm {

  /**
   * The form being actioned
   *
   * @var Array
   */
  public $form = array();

  /**
   * Constructor
   *
   * @param array $form [optional]
   *  A Drupal form array
   */
  public function __construct($form = array()) {
    $this->form = array();
    if (!empty($form)) {
      $this->updateForm($form);
    }
  }

  /**
   * Merge forms
   *
   * This can be used to merge additional form arrays into your existing array.
   *
   * @param array $form
   *  A form array
   */
  public function updateForm($form) {
    $this->form = array_merge_recursive($form, $this->form);
  }

  /**
   * Basic create/update form element
   *
   * This is an extremely basic, long-form way to add an element, but its not
   * any less complicated that the current FAPI.
   *
   * Example:
   * <?php
   * $form->element('item_one', array('#type' => 'markup', '#value' => 'A bit of content'));
   *
   * $form->element('item_one, array('#value' => 'Overwritten with different content'));
   * ?>
   *
   * @param string $key
   *  They key to use for the element
   * @param array $args [optional]
   *  Array of arguments to append to the form element, keyed as per the Drupal
   *  FAPI
   *
   * @return bool|\XuForm
   */
  public function element($key, $args = array()) {

    // If $type isn't provided, and no element is set, exit early
    if (!isset($this->form[$key]) && (!isset($args['#type']) || empty($args['#type']))) {
        return FALSE;
    }

    // Iterate over remaining arguments and apply the element
    if (!empty($args)) {
      foreach ($args as $arg_key => $arg) {
        $this->form[$key][$arg_key] = $arg;
      }
    }

    return $this;
  }

  /**
   * Calculate a textfield length based on its content
   *
   * @param $form_field
   *  A form element which supports the #size property
   * @param int $padding
   * @param int $max
   * @param int $min
   *
   * @return bool
   */
  public function fieldAutosize(&$form_field, $padding = 4, $max = 120, $min = 4) {
    $supported = array(
      'file', 'password', 'password_confirm', 'select', 'textfield'
    );

    if (!isset($form_field['#type']) || !in_array($form_field['#type'], $supported)) {
      return FALSE;
    }

    if (isset($form_field['#value'])){
      $base_length = strlen($form_field['#value']);
      $padded = $base_length + 4;

      if (isset($max) && !is_null($max) && $padded > $max) {
        $padded = $max;
      }
      elseif (isset($min) && !is_null($min) && $padded < $min) {
        $padded = $min;
      }

      $form_field['#size'] = $padded;
    }
  }
}

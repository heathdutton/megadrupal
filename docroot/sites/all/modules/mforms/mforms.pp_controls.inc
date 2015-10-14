<?php

/**
 * @file
 * Single class that is a implementation of controls.
 * This implementation can be used for simple edit - submit/cancel workflow.
 */

/**
 * Controls - edit - submit, cancel
 */
class MformsPPControls extends MformsControls {

  private static $instances = array();

  private $label_submit;
  private $attributes_submit;

  private $ajaxified = FALSE;


  public static function getInstance(MformsIstore $store, MformsSteps $steps, $init = NULL) {
    if (empty(self::$instances[$store->getKey()])) {
      self::$instances[$store->getKey()] = new MformsPPControls($store, $steps);
    }

    return self::$instances[$store->getKey()];
  }

  /**
   * Instantiate controls object.
   *
   * @param MformsIstore $store
   *   Store object.
   * @param MformsSteps $steps
   *   Steps object.
   */
  protected function __construct(MformsIstore $store, MformsSteps $steps) {
    $this->store = $store;
    $this->steps = $steps;

    $this->label_submit = t('Edit');
  }

  function isReset() {
    // Does not apply for workflow defined by this controls implementation.
    return FALSE;
  }

  function getControls($form) {
    $form_controls = array();

    if ($this->isSingleStep()) {
      return $form_controls;
    }

    $wrapper_id = $form['mforms_info']['#value']['wrapper_id'];

    if ($this->steps->getPrev() != NULL) {
      $form_controls['cancel'] = array(
        '#value' => l(t('Cancel'), $this->cancel_path),
        '#weight' => 101,
      );
    }

    $form_controls['submit'] = array(
      '#type' => 'submit',
      '#value' => $this->label_submit,
      '#weight' => 100,
      '#attributes' => $this->attributes_submit,
    );

    if ($this->ajaxified) {
      $form_controls['submit']['#ajax'] = array(
        'callback' => '_mforms_ajax_get_form',
        'wrapper' => $wrapper_id,
        'method' => 'replace',
        'effect' => 'fade',
      );
    }

    return $form_controls;
  }

  /**
   * Sets custom labels for form controls.
   *
   * @param string $submit
   *   Submit label text.
   */
  function setControlsLabels($submit) {
    if ($submit != NULL && trim($submit) != "") {
      $this->label_submit = $submit;
    }
  }

  /**
   * Sets custom attributes for controls.
   *
   * @param array $submit
   *   Submit button attributes.
   */
  function setControlsAttributes($submit) {
    if (is_array($submit)) {
      $this->attributes_submit = $submit;
    }
  }
}

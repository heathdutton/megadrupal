<?php

/**
 * @file
 * Single class that is an implementation of controls.
 * This implementation is suitable for a few step forms.
 */

/**
 * Simple implementation of form controls
 */
class MformsSimpleControls extends MformsControls {

  private static $instances = array();

  protected $label_back;
  protected $label_continue;
  protected $label_submit;
  protected $label_delete;

  protected $weight_back = 11;
  protected $weight_continue = 10;
  protected $weight_submit = 10;
  protected $weight_delete = 12;

  protected $attributes_back = array();
  protected $attributes_continue = array();
  protected $attributes_submit = array();
  protected $attributes_delete = array();

  protected $ajaxified = FALSE;


  public static function getInstance(MformsIstore $store, MformsSteps $steps, $init = NULL) {
    if (empty(self::$instances[$store->getKey()])) {
      self::$instances[$store->getKey()] = new MformsSimpleControls($store, $steps);
    }

    return self::$instances[$store->getKey()];
  }

  protected function __construct(MformsIstore $store, MformsSteps $steps) {
    $this->store = $store;
    $this->steps = $steps;

    // Init default labels.
    $this->label_continue = t('Continue');
    $this->label_back = t('Back');
    $this->label_submit = t('Submit');
    $this->label_delete = t('Reset');

    $this->attributes_delete = array('onclick' => 'return confirm("' . t('This action will reset the form and all data you have entered will be lost. Do you whish to continue?') . '")');
  }

  function doSubmitOnClickedAction() {
    return FALSE;
  }

  function getControls($form) {
    $form_controls = array();

    if ($this->isSingleStep()) {
      return $form_controls;
    }

    $wrapper_id = $form['mforms_info']['#value']['wrapper_id'];

    $form_controls['delete-all'] = array(
      '#type' => 'submit',
      '#value' => $this->label_delete,
      '#weight' => $this->weight_delete,
      '#attributes' => $this->attributes_delete,
      '#limit_validation_errors' => array(),
      '#submit' => array('_mforms_cancel_submit'),
      '#store_key' => $this->store->getKey(),
    );

    if ($this->steps->getPrev() != NULL) {
      $form_controls['callback-' . $this->steps->getPrev()] = array(
        '#type' => 'submit',
        '#value' => $this->label_back,
        '#weight' => $this->weight_back,
        '#attributes' => $this->attributes_back,
        '#limit_validation_errors' => array(),
        '#submit' => array('_mforms_back_submit'),
        '#store_key' => $this->store->getKey(),
        '#mforms_step' => $this->steps->getPrev(),
      );

      if ($this->ajaxified) {
        $form_controls['callback-' . $this->steps->getPrev()]['#ajax'] = array(
          'callback' => '_mforms_ajax_get_form',
          'wrapper' => $wrapper_id,
          'method' => 'replace',
          'effect' => 'fade',
        );
      }
    }

    if ($this->steps->getNext() != NULL) {
      $form_controls['continue'] = array(
        '#type' => 'submit',
        '#value' => $this->label_continue,
        '#weight' => $this->weight_continue,
        '#attributes' => $this->attributes_continue,
      );

      if ($this->ajaxified) {
        $form_controls['continue']['#ajax'] = array(
          'callback' => '_mforms_ajax_get_form',
          'wrapper' => $wrapper_id,
          'method' => 'replace',
          'effect' => 'fade',
        );
      }
    }
    else {
      $form_controls['submit'] = array(
        '#type' => 'submit',
        '#value' => $this->label_submit,
        '#weight' => $this->weight_submit,
        '#attributes' => $this->attributes_submit,
      );
    }

    return $form_controls;
  }

  /**
   * Sets custom attributes for controls.
   *
   * @param array $back
   *   Back button attributes.
   * @param array $continue
   *   Continue button attributes.
   * @param array $submit
   *   Submit button attributes.
   * @param array $delete
   *   Delete/reset button attributes.
   */
  function setControlsAttributes($back, $continue, $submit, $delete) {
    if (is_array($back)) {
      $this->attributes_back = $back;
    }
    if (is_array($continue)) {
      $this->attributes_continue = $continue;
    }
    if (is_array($submit)) {
      $this->attributes_delete = $delete;
    }
  }

  /**
   * Sets custom labels for form controls.
   *
   * @param string $back
   *   Back button label.
   * @param string $continue
   *   Continue button label.
   * @param string $submit
   *   Submit button label.
   * @param string $delete
   *   Delete/reset button label.
   */
  function setControlsLabels($back, $continue, $submit, $delete) {
    if ($back != NULL && trim($back) != "") {
      $this->label_back = $back;
    }
    if ($continue != NULL && trim($continue) != "") {
      $this->label_continue = $continue;
    }
    if ($submit != NULL && trim($submit) != "") {
      $this->label_submit = $submit;
    }
    if ($delete != NULL && trim($delete) != "") {
      $this->label_delete = $delete;
    }
  }

  /**
   * Sets weights for form control elements.
   *
   * @param int $back
   *   Back button weight.
   * @param int $continue
   *   Continue button weight.
   * @param int $submit
   *   Submit button weight.
   * @param int $delete
   *   Delete/reset button weight.
   */
  function setControlsWeights($back, $continue, $submit, $delete) {
    $this->weight_back = $back;
    $this->weight_continue = $continue;
    $this->weight_submit = $submit;
    $this->weight_delete = $delete;
  }
}

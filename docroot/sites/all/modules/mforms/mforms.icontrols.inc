<?php

/**
 * @file
 * Defines basics of controls class. If you want fully custom
 * form controls, you need to implement this interface, so that
 * the instance of your implementation can be used internaly by mforms.
 *
 * In case you want to only customize existing controls
 * override any of the existing implementations of this interface or
 * you may use the abstract implementation MformsControls.
 */

/**
 * This interface defines form controls used by mforms.
 */
interface MformsIcontrols {

  /**
   * @static
   * @abstract
   *
   * Factory method to enforce singleton instance of controls for a specific
   * store key.
   *
   * @param MformsIstore $store
   *   Store instance.
   * @param MformsSteps $steps
   *   Steps instance.
   * @param array $init
   *   Optional init parameters.
   *
   * @return MformsIcontrols
   *   Instantiated MformsIcontrols object.
   */
  static function getInstance(MformsIstore $store, MformsSteps $steps, $init = array());

  /**
   * Gets form controls as defined by Drupal form API
   *
   * @param array $form_controls
   *   Drupal form.
   *
   * @return array
   *   Controls form part.
   */
  function getControls($form_controls);

  /**
   * Setter method to set clicked button.
   *
   * @param string $clicked_button_id
   *   Clicked button id as defined by Drupal FAPI.
   */
  function setClickedButton($clicked_button_id);

  /**
   * Gets clicked action
   *
   * @return string
   *   Clicked button id.
   */
  function getClickedAction();

  /**
   * Gets additional param from clicked action
   *
   * @return string
   *   Custom param set by clicked button.
   */
  function getClickedParam();

  /**
   * Determines if the form is single step.
   *
   * @return boolean
   *   Check status.
   */
  function isSingleStep();

  /**
   * Determines if user inserted values are going to be reset
   *
   * @return boolean
   *   Check status.
   */
  function isReset();

  /**
   * Tels the state machine if to call submit callback on user requested step.
   *
   * @return boolean
   *   Check status.
   */
  function doSubmitOnClickedAction();

  /**
   * Sets path to which header is sent after cancel
   *
   * @param string $cancel_path
   *   Path where to redirect user on cancel action.
   */
  function setCancelPath($cancel_path);

  /**
   * Gets cancel path.
   *
   * @return string
   *   The path where to redirect user on cancel action.
   */
  function getCancelPath();

  /**
   * Calling of this method will ajaxify some or all form operations.
   *
   * @param array $ajax
   *   Drupal #ajax form properties.
   */
  function ajaxify(array $ajax = array());
}

/**
 * Provides generic functionality to simplify creation of final
 * implementations. If considering own controls implementation
 * this abstract class is a good start point.
 */
abstract class MformsControls implements MformsIcontrols {

  /**
   * Reference to store object
   *
   * @var MformsIstore
   */
  protected $store;

  /**
   * Reference to steps object
   *
   * @var MformsSteps
   */
  protected $steps;

  /**
   * Single step flag.
   *
   * @var bool
   */
  protected $single_step = FALSE;

  /**
   * Cancel path.
   *
   * @var string
   */
  protected $cancel_path;

  /**
   * Flag to determine if to use ajaxified form controls.
   *
   * @var bool
   */
  protected $ajaxified = FALSE;

  /**
   * Sets single step - no form controls is displayed
   *
   */
  function setSingleStep() {
    $this->single_step = TRUE;
  }

  function isSingleStep() {
    return $this->single_step;
  }

  function setCancelPath($path) {
    $this->cancel_path = $path;
  }

  function getCancelPath() {
    return $this->cancel_path;
  }

  function setClickedButton($clicked_button) {
    $this->store->setStore('clicked_button', $clicked_button);
  }

  function getClickedAction() {
    $clicked_button = $this->store->getStore('clicked_button');

    if (empty($clicked_button)) {
      return NULL;
    }

    return (isset($clicked_button['#mforms_step']) ? $clicked_button['#mforms_step'] : NULL);
  }

  function getClickedParam() {
    $clicked_button = $this->store->getStore('clicked_button');

    if (empty($clicked_button)) {
      return NULL;
    }

    return (isset($clicked_button['#mforms_params']) ? $clicked_button['#mforms_params'] : NULL);
  }

  function doSubmitOnClickedAction() {
    return function_exists($this->getClickedAction());
  }

  function isReset() {
    $clicked_button = $this->store->getStore('clicked_button');
    return strpos($clicked_button['#id'], 'delete-all') !== FALSE;
  }

  /**
   * Default implementation - ignores $ajax parameter. If you need to
   * provide custom #ajax properties, create own controls implementation.
   *
   * @param array $ajax
   */
  function ajaxify(array $ajax = array()) {
    $this->ajaxified = TRUE;
  }
}

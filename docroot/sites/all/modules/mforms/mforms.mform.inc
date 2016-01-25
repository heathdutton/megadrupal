<?php

/**
 * @file
 * Single class - a utility object that provide state machine functionality.
 */

/**
 * State machine for multi step Drupal forms.
 */
class Mform {

  private static $instances = array();

  /**
   * Reference to store object.
   *
   * @var MformsIstore
   */
  private $store = NULL;

  /**
   * Reference to steps object.
   *
   * @var MformsSteps
   */
  private $steps = NULL;

  /**
   * Reference to controls object.
   *
   * @var MformsIcontrols
   */
  private $controls = NULL;

  /**
   * Drupal form array.
   *
   * @var array
   */
  private $form = array();


  /**
   * Private constructor to enforce singleton object.
   */
  private function __construct() {

  }

  /**
   * Init the state machine object and get its singleton.
   *
   * @param MformsIstore $store
   *   Storage where to store data across steps.
   * @param MformsSteps $steps
   *   Steps object.
   * @param MformsIcontrols $controls
   *   Controls object.
   * @param string $first_step
   *   The first step of the state machine chain.
   *
   * @return Mform
   *   Singleton object.
   * @throws Exception
   */
  public static function getInstance(MformsIstore $store, MformsSteps $steps, MformsIcontrols $controls = NULL, $first_step = NULL) {

    // If singleton instance is NULL, instantiate the Mform.
    if (empty(self::$instances[$store->getKey()])) {
      self::$instances[$store->getKey()] = new Mform();
    }

    // If store is provided, reset it.
    if ($store != NULL) {
      self::$instances[$store->getKey()]->store = $store;
    }

    // If $steps is provided, reset it.
    if ($steps != NULL) {
      self::$instances[$store->getKey()]->steps = $steps;
    }

    // If controls is provided, reset it.
    if ($controls != NULL) {
      self::$instances[$store->getKey()]->controls = $controls;
    }

    // Make sure we have all must-be objects.
    if (self::$instances[$store->getKey()]->store == NULL) {
      throw new Exception('Store not provided');
    }
    if (self::$instances[$store->getKey()]->steps == NULL) {
      throw new Exception('Steps not provided');
    }

    // If we do not have current step, init steps from provided args.
    if (self::$instances[$store->getKey()]->steps->getCurr() == NULL && $first_step != NULL) {
      self::$instances[$store->getKey()]->steps->addStep($first_step);
      self::$instances[$store->getKey()]->steps->setCurr($first_step);
    }

    // Return instantiated and set mfrom object.
    return self::$instances[$store->getKey()];
  }

  /**
   * Main caller method.
   *
   * @param string $type
   *   Callback method type.
   *    - 'build' - expects to call functions that build form definition array
   *    - 'validate' - validate actions
   *    - 'submit' - submit action that should process the submitted data.
   *      It also shifts the internal pointer to the next step.
   * @param array $form
   *   Form definition array as defined by Drupal.
   * @param array $form_state
   *   Form state Drupal array.
   * @param array $params
   *   Array of additional params/objects that should be passed in to
   *   the build callback.
   *
   * @return mixed
   *   On build action it returns Drupal's form array on other void
   * @throws Exception
   *    If: controls are not loaded; invalid callback is provided;
   */
  public function call($type, $form, &$form_state, $params = array()) {

    if ($this->controls == NULL) {
      throw new Exception('Unable to load controls object');
    }

    // If we have submit action indicate we go for form rebuild and
    // set submitted data into store.
    if ($type == 'submit') {
      $form_state['rebuild'] = TRUE;
      $this->setStore($form_state['values']);
    }

    // Set the clicked button into controls object.
    if (!empty($form_state['clicked_button'])) {
      $this->controls->setClickedButton($form_state['clicked_button']);
    }

    // If reset action was called, clear the store.
    if ($this->controls->isReset()) {
      $this->store->clearStore();
    }

    $clicked_action = $this->controls->getClickedAction();

    // If we have clicked action - user requested other then the next step.
    if (!empty($clicked_action)) {
      if (!in_array($clicked_action, $this->steps->getSteps())) {
        throw new Exception('Invalid callback ' . $clicked_action);
      }

      $this->callClickedAction($clicked_action, $type, $form, $form_state, $params);
    }
    // Running regular continue/next/submit action.
    else {
      $this->callNextAction($type, $form, $form_state, $params);
    }

    return $this->form;
  }

  /**
   * Handles form actions in case of custom callback.
   *
   * @param string $callback
   *   Custom callback name.
   * @param string $type
   *   Action type - build, validate, submit.
   * @param array $form
   *   Drupal form array.
   * @param array $form_state
   *   Drupal form_state array.
   * @param array $params
   *   Additional params passed into form.
   */
  private function callClickedAction($callback, $type, $form, &$form_state, $params = array()) {
    // We want to run validate and submit callback for the next step
    // but the build callback should be run for clickedAction.
    if ($this->controls->doSubmitOnClickedAction()) {
      if ($type == 'submit') {
        $this->callSubmit($form, $form_state);
      }
      elseif ($type == 'build') {
        $this->steps->setCurr($callback);
        $this->form = $this->callBuild($form_state, $params);
        // Unset the clicked button so that it is not used in following
        // regular lifecycle but not before build action is triggered so
        // that we have clicked_param available within the form build action.
        $this->controls->setClickedButton(NULL);
      }
    }
    // Just run the build callback as defined by clickedAction but only for
    // build type.
    elseif (!$this->controls->doSubmitOnClickedAction() && $type == 'build') {
      $this->steps->setCurr($callback);
      $this->form = $this->callBuild($form_state, $params);
    }
  }

  /**
   * Handles form actions for regular next action.
   *
   * @param string $type
   *   Action type - build, validate, submit.
   * @param array $form
   *   Drupal form array.
   * @param array $form_state
   *   Drupal form_state array.
   * @param array $params
   *   Additional params passed into form.
   */
  private function callNextAction($type, $form, &$form_state, $params = array()) {
    if ($type == 'build') {
      $this->form = $this->callBuild($form_state, $params);
    }
    elseif ($type == 'validate' && !$this->controls->isSingleStep()) {
      $this->callValidate($form, $form_state);
    }
    elseif ($type == 'submit' && !$this->controls->isSingleStep()) {
      $this->callSubmit($form, $form_state);
    }
  }

  /**
   * Calling form render callbacks.
   *
   * @param array $form_state
   *   Drupal form_state array.
   * @param array $params
   *   Additional params passed into form.
   *
   * @return array
   *   Should be form definition.
   * @throws Exception
   *    Throws an exception if form definition could not be retrieved.
   */
  private function callBuild(&$form_state, $params) {
    $next_step = NULL;

    // Call the callback function.
    $step = $this->steps->getCurr();
    if (!function_exists($step)) {
      return NULL;
    }

    $form = $step($form_state, $next_step, $params);

    if (!is_array($form)) {
      throw new Exception('Error retrieving form definition from step ' . $step);
    }

    // Add next step to the stack, however only if it is not already added.
    if ($next_step != NULL && array_search($next_step, $this->steps->getSteps()) === FALSE) {
      $this->steps->addStep($next_step);
    }
    $form['curr_step'] = array(
      '#type' => 'value', '#value' => $step,
    );
    $form['prev_step'] = array(
      '#type' => 'value', '#value' => $this->steps->getPrev(),
    );
    $form['next_step'] = array(
      '#type' => 'value', '#value' => $this->steps->getNext(),
    );
    $form['steps'] = array(
      '#type' => 'value', '#value' => serialize($this->steps->getSteps()),
    );

    return $form;
  }

  /**
   * Calling validate callbacks.
   *
   * @param array $form
   *   Drupal form array.
   * @param array $form_state
   *   Drupal form_state array.
   */
  private function callValidate($form, &$form_state) {
    $step = $this->steps->getCurr() . '_validate';

    if (!function_exists($step)) {
      return;
    }

    $step($form, $form_state);
  }

  /**
   * Calling submit callbacks.
   *
   * @param array $form
   *   Drupal form array.
   * @param array $form_state
   *   Drupal form_state array.
   */
  private function callSubmit($form, &$form_state) {
    $step = $this->steps->getCurr() . '_submit';

    if (function_exists($step)) {
      $step($form, $form_state);
    }

    // If we have next step - set it as current.
    if ($this->steps->getNext() != NULL) {
      $this->steps->setCurr($this->steps->getNext());
    }
  }

  /**
   * Set store used to store data between steps.
   *
   * @param mixed $data
   *   Data to be set into store.
   */
  private function setStore($data) {
    if ($this->steps->getCurr()) {
      $this->store->setStore($this->steps->getCurr(), $data);
    }
  }

  /**
   * Gets data from store.
   *
   * @param string $step
   *   Step name from which to retrieve data.
   *
   * @return mixed
   *   Data stored.
   */
  function getStore($step = NULL) {
    if ($step == NULL) {
      return $this->store->getStore($this->steps->getCurr());
    }
    return $this->store->getStore($step);
  }
}

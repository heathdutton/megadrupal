<?php
/**
 * @file
 * Contains the Qui_Query_Builder_Controller class.
 */

/**
 * Class Qui_Query_Builder_Controller
 */
class Qui_Query_Builder_Controller {

  public $form;
  public $form_state;
  public $action;
  public $report;

  private $handlers = array();

  /**
   * Instantiates the handlers.
   *
   * @param array $form
   *   Drupal form.
   * @param array $form_state
   *   Drupal form_state.
   */
  public function __construct(&$form, &$form_state) {
    $this->setVars($form, $form_state);
    $this->loadHandlers();
    $this->buildHandlers();
  }

  public function setVars(&$form, &$form_state) {
    $this->form = & $form;
    $this->form_state = & $form_state;
    $this->report = & $this->form_state['#report'];
    $this->action = & $this->form_state['#action'];
  }


  /**
   * Sets up form elements before rendering.
   */
  public function preRender() {
    $this->runHandlerMethods('setDefaults');
    $this->runHandlerMethods('preBuild');
  }

  /**
   * Build and render the handlers.
   */
  public function render() {
    $this->runHandlerMethods('executeButtonAction');
    $this->runHandlerMethods('build', TRUE);
    $this->runHandlerMethods('render', TRUE);
    // Reset database connection.
    db_set_active();
  }

  /**
   * Runs a method on all handlers.
   *
   * @param string $method
   *   Handler method to call.
   * @param bool $check_enabled
   *   Whether the handler must be enabled to call the method.
   */
  public function runHandlerMethods($method, $check_enabled = FALSE) {
    foreach ($this->getHandlers() as $handler) {
      if (!$check_enabled || $handler->enabled) {
        $handler->$method();
      }
    }
  }

  /**
   * Run validate method on all handlers.
   */
  public function validate() {
    $this->runHandlerMethods('validate');
  }

  /**
   * Run submit method on all enabled handlers.
   */
  public function submit() {
    if (!($this->report instanceof QuiReport)) {
      $this->report = new QuiReport();
    }
    $this->runHandlerMethods('setDefaults');
    $this->runHandlerMethods('preBuild');
    $this->runHandlerMethods('submit', TRUE);
    if ($this->report->status == ENTITY_IN_CODE) {
      // Update the status to be overridden.
      $this->report->status = ENTITY_OVERRIDDEN;
    }
    if (!$this->report->status) {
      $this->report->status = ENTITY_CUSTOM;
    }
    try {
      qui_report_save($this->report);
      drupal_set_message(t('Report saved.'));
      if ($this->report->identifier()) {
        drupal_goto("admin/reports/qui/manage/{$this->report->identifier()}/edit");
      }
    }
    catch (Exception $e) {
      drupal_set_message(t('Error saving report.'), 'error');
    }
  }

  /**
   * Instantiate handlers.
   */
  private function buildHandlers() {
    foreach ($this->handlers as $name => $opt) {
      if (isset($opt['class']) && !isset($opt['instance'])) {
        $this->handlers[$name]['instance'] = new $opt['class']($this);
      }
    }
  }

  /**
   * Return a specific handler instance.
   *
   * @param string $key
   *   Handler key.
   * @return mixed
   *   Handler object.
   */
  public function getHandler($key) {
    return $this->handlers[$key]['instance'];
  }

  /**
   * Get all handler objects.
   *
   * @return array
   *   Array of handler objects.
   */
  private function getHandlers() {
    $active = array();
    foreach ($this->handlers as $name => $opt) {
      $handler = $opt['instance'];
      $active[$name] = $handler;
    }
    return $active;
  }

  /**
   * Loads handlers from module hook.
   */
  private function loadHandlers() {
    foreach (module_implements('qui_query_builder_handlers_info') as $module) {
      $hook = "{$module}_qui_query_builder_handlers_info";
      $this->handlers = array_merge($this->handlers, $hook());
    }
  }

}

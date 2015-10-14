<?php

/**
 * @file
 * Provides a batched rule set component to rules.
 *
 * This is based upon rules core plugin 'rule set'.
 * @See rules.plugins.inc
 */

/**
 * Implements hook_rules_plugin_info().
 */
function rules_batch_rules_plugin_info() {
  return array(
    'batch loop' => array(
      'class' => 'RulesBatchLoop',
      'embeddable' => 'RulesActionContainer',
      'extenders' => array(
        'RulesPluginUIInterface' => array(
          'class' => 'RulesBatchLoopUI',
        ),
      ),
    ),
  );
}

/**
 * Loop plugin for using batch API.
 */
class RulesBatchLoop extends RulesLoop {

  protected $itemName = 'batch loop';

  public function evaluate(RulesState $state) {

    try {
      $batch = array(
        'operations' => array(array('rules_batch_batch_loop_process', array($this, $state))),
        'finished' => 'rules_batch_batch_loop_finished',
        'title' => t('Rules Batch'),
        'init_message' => t('Starting rules batch'),
        'progress_message' => t('Processed @current out of @total.'),
        'error_message' => t('Rules Batch has encountered an error.'),
      );

      batch_set($batch);
      rules_log('Batch set for %name.', array('%name' => $this->getPluginName()), RulesLog::INFO, $this);
    }
    catch (RulesEvaluationException $e) {
      rules_log($e->msg, $e->args, $e->severity);
      rules_log('Unable to set %name.', array('%name' => $this->getPluginName()), RulesLog::WARN, $this);
    }
  }

  public function process(RulesState $state) {
    foreach ($this->children as $action) {
      $action->evaluate($state);
    }
  }

  public function getPublicArgument($name, $info, $state) {
    return $this->getArgument($name, $info, $state);
  }

  public function label() {
    return !empty($this->label) ? $this->label : t('Batch Loop');
  }

}

/**
 * Batch Loop plugin UI
 */
class RulesBatchLoopUI extends RulesLoopUI {

}

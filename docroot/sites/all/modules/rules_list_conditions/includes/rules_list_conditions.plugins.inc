<?php

/**
 * @file
 * The Rules plugins for Any and All loops.
 */

if (!class_exists('RulesListConditionsConditionalLoop')) {
  module_load_include('inc', 'rules_list_conditions', 'includes/rules_list_conditions.core');
}

/**
 * An any loop.
 */
class RulesListConditionsAny extends RulesListConditionsConditionalLoop {
  protected $itemName = 'any';

  /**
   * {@inheritdoc}
   */
  public function __construct($settings = array(), $variables = NULL) {
    parent::__construct($settings, $variables);
  }

  /**
   * {@inheritdoc}
   */
  public function evaluate(RulesState $state) {
    try {
      $param_info = $this->pluginParameterInfo();
      $list = $this->getArgument('list', $param_info['list'], $state);

      if (isset($this->settings['list:select'])) {
        rules_log('Looping over the list items of %sel (ANY)', array('%sel' => $this->settings['list:select']), RulesLog::INFO, $this);
      }

      foreach ($list as $key => $item) {
        $local = $this->setListVar($state, $item);

        foreach ($this->children as $condition) {
          if ($condition->evaluate($local)) {
            rules_log($this->label() . ' evaluated to TRUE.');
            return !$this->negate;
          }
        }
      }
      rules_log($this->label() . 'ANY evaluated to FALSE.');
      return $this->negate;
    }
    catch (RulesEvaluationException $e) {
      $this->logExn($e);
    }
  }

  /**
   * {@inheritdoc}
   */
  protected function exportChildren($key = 'ANY') {
    return parent::exportChildren($key);
  }

  /**
   * {@inheritdoc}
   */
  protected function importChildern($export, $key = 'ANY') {
    parent::importChildren($export, $key);
  }

  /**
   * {@inheritdoc}
   */
  public function label() {
    return !empty($this->label) ? $this->label : ($this->negate ? t('NOT ANY') : t('ANY'));
  }
}

/**
 * An all loop.
 */
class RulesListConditionsAll extends RulesListConditionsConditionalLoop {
  protected $itemName = 'all';

  /**
   * {@inheritdoc}
   */
  public function __construct($settings = array(), $variables = NULL) {
    parent::__construct($settings, $variables);
  }

  /**
   * {@inheritdoc}
   */
  public function evaluate(RulesState $state) {
    try {
      $param_info = $this->pluginParameterInfo();
      $list = $this->getArgument('list', $param_info['list'], $state);

      if (isset($this->settings['list:select'])) {
        rules_log('Looping over the list items of %sel (ALL)', array('%sel' => $this->settings['list:select']), RulesLog::INFO, $this);
      }

      foreach ($list as $key => $item) {
        $local = $this->setListVar($state, $item);

        foreach ($this->children as $condition) {
          if (!$condition->evaluate($local)) {
            rules_log($this->label() . ' evaluated to TRUE.');
            return $this->negate;
          }
        }
      }
      rules_log($this->label() . ' evaluated to FALSE.');
      return !$this->negate;
    }
    catch (RulesEvaluationException $e) {
      $this->logExn($e);
    }
  }

  /**
   * {@inheritdoc}
   */
  protected function exportChildren($key = 'ALL') {
    return parent::exportChildren($key);
  }

  /**
   * {@inheritdoc}
   */
  protected function importChildern($export, $key = 'ALL') {
    parent::importChildren($export, $key);
  }

  /**
   * {@inheritdoc}
   */
  public function label() {
    return !empty($this->label) ? $this->label : ($this->negate ? t('NOT ALL') : t('ALL'));
  }
}

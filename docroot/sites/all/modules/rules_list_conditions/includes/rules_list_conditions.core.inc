<?php

/**
 * @file
 * Contains the base class for condition loops.
 */

/**
 * Base class for condition loops.
 */
abstract class RulesListConditionsConditionalLoop extends RulesConditionContainer {
  protected $listItemInfo;

  /**
   * {@inheritdoc}
   */
  public function __construct($settings = array(), $variables = NULL) {
    parent::__construct();
    $this->settings = (array) $settings + array(
      'item:var' => t('list_item'),
      'item:label' => t('Current list item'),
    );
    if (!empty($variables)) {
      $this->info['variables'] = $variables;
    }
  }

  /**
   * {@inheritdoc}
   */
  public function pluginParameterInfo() {
    return array(
      'list' => array(
        'type' => 'list',
        'restriction' => 'selector',
        'label' => t('List'),
        'description' => t('The list to loop over. The loop will check against each item in the list.'),
      ),
    );
  }

  /**
   * {@inheritdoc}
   */
  public function dependencies() {
    $dependencies = parent::dependencies();
    $dependencies[] = 'rules_list_conditions';
    return $dependencies;
  }

  /**
   * {@inheritdoc}
   */
  public function integrityCheck() {
    parent::integrityCheck();
    $this->checkVarName($this->settings['item:var']);
  }

  /**
   * {@inheritdoc}
   */
  protected function exportSettings() {
    $export = parent::exportSettings();
    $export['ITEM'][$this->settings['item:var']] = $this->settings['item:label'];
    return $export;
  }

  /**
   * {@inheritdoc}
   */
  protected function importSettings($export) {
    parent::importSettings($export);
    if (isset($export['ITEM'])) {
      $this->settings['item:var'] = rules_array_key($export['ITEM']);
      $this->settings['item:label'] = reset($export['ITEM']);
    }
  }

  /**
   * {@inheritdoc}
   */
  protected function importChildren($export, $key = NULL) {
    // Up to children to set $key.
    if (!isset($export[$key]) && isset($export['NOT ' . $key])) {
      $this->negate = TRUE;
      $key = 'NOT ' . $key;
    }
    parent::importChildren($export, $key);
  }

  /**
   * Return information about the list being iterated over.
   *
   * @return array
   *   An associate array containing
   *     'type': The type of the list's elements (or unknown if not set).
   *     'label': The human readable label, if set.
   */
  public function listItemInfo() {
    if (!isset($this->listItemInfo)) {
      if ($info = $this->getArgumentInfo('list')) {
        // Pass through the variable info keys like property info.
        $iks = array_flip(array('type', 'property info', 'bundle'));
        $this->listItemInfo = array_intersect_key($info, $iks);
        $this->listItemInfo['type'] = isset($info['type']) ? entity_property_list_extract_type($info['type']) : 'unknown';
      }
      else {
        $this->listItemInfo = array('type' => 'unknown');
      }
      $this->listItemInfo['label'] = $this->settings['item:label'];
    }
    return $this->listItemInfo;
  }

  /**
   * {@inheritdoc}
   */
  protected function stateVariables($element = NULL) {
    return array(
      $this->settings['item:var'] => $this->listItemInfo(),
    ) + parent::stateVariables($element);
  }

  /**
   * Internal helper for child loops to log exceptions.
   */
  protected function logExn($e) {
    rules_log($e->msg, $e->args, $e->severity);
    rules_log('Unable to evaluate %name.', array('%name' => $this->getPluginName()), RulesLog::WARN, $this);
  }

  /**
   * Internal helper for child loops to set current item in loop body.
   * @return RulesState
   *   A copy of the rules execution context with the loop variable for
   *   the current item in the list being iterated over.
   */
  protected function setListVar($state, $item) {
    $local = clone $state;
    $info = $this->listItemInfo();
    $name = $this->settings['item:var'];
    $local->addVariable($name, $item, $info);
    return $local;
  }
}

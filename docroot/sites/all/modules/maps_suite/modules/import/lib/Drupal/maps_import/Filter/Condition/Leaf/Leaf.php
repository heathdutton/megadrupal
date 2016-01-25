<?php

namespace Drupal\maps_import\Filter\Condition\Leaf;

use Drupal\maps_import\Filter\Condition\Condition;
use Drupal\maps_import\Converter\Converter;

/**
 * Leaf conditions.
 */
abstract class Leaf extends Condition {

  /**
   * The criteria used for filtering.
   *
   * @var array
   */
  public $criteria = array();

  /**
   * Flag indicating if the condition is negative.
   *
   * @var int
   */
  public $negate;

  /**
   * @inheritdoc
   */
  public function __construct(Converter $converter, array $properties = array()) {
    parent::__construct($converter, $properties);

    if (array_key_exists('extra', $properties)) {
      $extra = $properties['extra'];

      if (array_key_exists('criteria', $extra)) {
        if (!is_array($extra['criteria'])) {
          $extra['criteria'] = array($extra['criteria']);
        }

        $this->criteria = array_filter($extra['criteria']);
      }

      if (array_key_exists('negate', $extra)) {
        $this->negate = $extra['negate'];
      }
    }
  }

  /**
   * @inheritdoc
   */
  public function isContainer() {
    return FALSE;
  }

  /**
   * Get the condition criteria.
   *
   * @return array
   */
  public function getCriteria() {
    return $this->criteria;
  }

  /**
   * Set the condition criteria.
   *
   * @return array
   */
  public function setCriteria($criteria) {
    $this->criteria = $criteria;
  }

  /**
   * Whether the condition has multiple criteria or not.
   *
   * @return bool
   */
  public function isMultiple() {
    return FALSE;
  }

  /**
   * @inheritdoc
   */
  public function getLabel() {
    $settings = $this->getConverter()->getProfile()->getConfigurationTypes($this->getType(), 1);
    $criteria = $this->getCriteria();
    $count = $this->isMultiple() ? count($criteria) : 1;

    if (!$this->isMultiple()) {
      if ($settings) {
        $criteria = maps_suite_reduce_array(array_intersect_key($settings, array_flip($criteria)), 'title');
      }
    }

    $t_args = array(
      '%condition' => t($this->getTitle()),
      '%$criteria' => implode(', ', $criteria),
    );

    return $this->negate ?
      format_plural($count, '%condition is not %$criteria.', '%condition do not belongs to %$criteria.', $t_args) :
      format_plural($count, '%condition is %$criteria.', '%condition belongs to %$criteria.', $t_args);
  }

  /**
   * Check if the given entity match the condition.
   *
   * @param $entity
   *   The MaPS System® entity to check.
   *
   * @return bool
   */
  public abstract function match(array $entity);

  /**
  * Apply the negate status on the match result.
  */
  protected function checkNegate($match) {
    return !($match xor !$this->negate);
  }

  /**
   * @inheritdoc
   */
  public function save() {
    $this->setExtra(array(
      'criteria' => $this->getCriteria(),
      'negate' => $this->negate,
    ));

    return parent::save();
  }

  /**
   * Generate array for form page.
   */
  public function generateForm($form, &$form_state) {
    $form_state['condition'] = $this;

    $form['id'] = array('#type' => 'value', '#value' => $this->getId());
    $form['cid'] = array('#type' => 'value', '#value' => $this->getConverter()->getCid());
    $form['type'] = array('#type' => 'value', '#value' => $this->getType());

    $form['parent_id'] = array(
      '#type' => 'select',
      '#title' => t('Parent container'),
      '#options' => array('' => t('None'))  + $this->getConverter()->getConditionContainers(),
      '#default_value' => $this->getParentId(),
    );

    $form['weight'] = array(
      '#type' => 'weight',
      '#title' => t('Weight'),
      '#delta' => 10,
      '#default_value' => $this->getWeight(),
    );

    $form['type'] = array('#type' => 'value', '#value' => $this->getType());
    return $this->form($form, $form_state);
  }

  /**
   * Build the filter condition form element.
   *
   * @param $form
   *   An associative array containing the structure of the form.
   * @param $form_state
   *   A keyed array containing the current state of the form.
   */
  public function form($form, &$form_state) {
    if (!$config = $this->getConverter()->getConfigSettings($this->getType())) {
      drupal_set_message(t('The configuration does not contain any data related to the type %type. Please ensure that the profile configuration was correctly imported from MaPS System®.', array('%type' => $this->getType())), 'error');
    }

    $form['criteria'] = array(
      '#type' => 'select',
      '#title' => $this->getTitle(),
      '#options' => maps_suite_reduce_array($config, 'title'),
      '#multiple' => $this->isMultiple(),
      '#required' => !empty($config),
      '#default_value' => $this->getCriteria(),
    );

    $form['negate'] = array(
      '#type' => 'checkbox',
      '#title' => t('Negate'),
      '#default_value' => $this->negate,
    );

    return $form;
  }

  /**
   * Save the add form.
   * Necessary for multistep forms
   */
  public function formSave($form, &$form_state) {
    // Necessary for multistep forms.
    $values = array_merge($form_state['values'], $form_state['input']);

    $classes = $this->getConverter()->getFilter()->getAvailableConditions();
    $condition = new $classes[$values['type']]['class']($this->getConverter(), $values);
    $condition->setCriteria($values['criteria']);
    $condition->save();
  }

}

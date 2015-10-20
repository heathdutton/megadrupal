<?php

namespace Drupal\maps_import\Filter\Condition\Leaf\Object;

use Drupal\maps_import\Filter\Condition\Leaf\Leaf;

/**
 * Condition on object class.
 */
class ObjectClass extends Leaf {

  /**
   * @inheritdoc
   */
  public function getTitle() {
    return 'Object class';
  }

  /**
   * @inheritdoc
   */
  public function getType() {
    return 'class';
  }

  /**
   * @inheritdoc
   */
  public function isMultiple() {
    return TRUE;
  }

  /**
   * @inheritdoc
   */
  public function getLabel() {
    $settings = $this->getConverter()->getProfile()->getConfigurationTypes($this->getType(), 1);
    $criteria = $this->getCriteria();
    $count = $this->isMultiple() ? count($criteria) : 1;

    if ($this->isMultiple()) {
      $criteriaLabel = '';

      if (isset($criteria['at_least'])) {
        $criteriaLabel .= ' at least ' . implode(', ', maps_suite_reduce_array(array_intersect_key($settings, array_flip($criteria['at_least'])), 'title'));
      }
      if (isset($criteria['at_most'])) {
        $criteriaLabel .= ' at most ' . implode(', ', maps_suite_reduce_array(array_intersect_key($settings, array_flip($criteria['at_most'])), 'title'));
      }
    }
    else {
      $criteriaLabel = implode(', ', $criteria);
    }

    $t_args = array(
      '%condition' => t($this->getTitle()),
      '%$criteria' => $criteriaLabel,
    );

    return $this->negate ?
      format_plural($count, '%condition is not %$criteria.', '%condition do not belongs to %$criteria.', $t_args) :
      format_plural($count, '%condition is %$criteria.', '%condition belongs to %$criteria.', $t_args);
  }

  /**
   * @inheritdoc
   */
  public function match(array $entity) {
    $criteria = $this->getCriteria();
    if (empty($criteria)) {
      return !$this->negate;
    }

    $bool = TRUE;

    if (!isset($criteria['at_least'])) {
      $criteria['at_least'] = array();
    }

    if (!isset($criteria['at_most'])) {
      $criteria['at_most'] = array();
    }

    $entity['classes'] = unserialize($entity['classes']);
    $classes = array();
    foreach ($entity['classes'] as $class) {
      $classes[] = $class['value'];
    }

    sort($criteria['at_least']);
    sort($criteria['at_most']);
    sort($classes);

    if ($criteria['at_least'] == $criteria['at_most']) {
      $bool = ($classes == $criteria['at_least']);
    }
    else {
      if (!empty($criteria['at_least'])) {
        $at_least = array_intersect($classes, $criteria['at_least']);
        sort($at_least);
        $bool = ($bool AND ($at_least == $criteria['at_least']));
      }

      if (!empty($criteria['at_most'])) {
        $at_most = array_diff($classes, $criteria['at_most']);
        sort($at_most);
        $bool = ($bool AND empty($at_most));
      }
    }

    return $this->checkNegate($bool);
  }

  /**
   * @inheritdoc
   */
  public function form($form, &$form_state) {
    if (!$config = $this->getConverter()->getConfigSettings($this->getType())) {
      drupal_set_message(t('The configuration does not contain any data related to the type %type. Please ensure that the profile configuration was correctly imported from MaPS System®.', array('%type' => $this->getType())), 'error');
    }

    drupal_add_js(drupal_get_path('module', 'maps_import') . '/js/maps_import.js');

    $criteria = $this->getCriteria() + array(
      'at_least' => NULL,
      'at_most' => NULL,
    );

    $form['disable_at_least'] = array(
      '#type' => 'checkbox',
      '#title' => 'Disable at least',
      '#default_value' => FALSE,
    );

    $form['at_least'] = array(
      '#type' => 'select',
      '#title' => 'At least',
      '#options' => maps_suite_reduce_array($config, 'title'),
      '#multiple' => $this->isMultiple(),
      '#default_value' => $criteria['at_least'],
    );

    $form['disable_at_most'] = array(
      '#type' => 'checkbox',
      '#title' => 'Disable at most',
      '#default_value' => FALSE,
    );

    $form['at_most'] = array(
      '#type' => 'select',
      '#title' => 'At most',
      '#options' => maps_suite_reduce_array($config, 'title'),
      '#multiple' => $this->isMultiple(),
      '#default_value' => $criteria['at_most'],
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
    $criteria = array();

    if (!empty($values['at_least'])) {
      $criteria['at_least'] = $values['at_least'];
    }

    if (!empty($values['at_most'])) {
      $criteria['at_most'] = $values['at_most'];
    }

    $condition->setCriteria($criteria);
    $condition->save();
  }

}

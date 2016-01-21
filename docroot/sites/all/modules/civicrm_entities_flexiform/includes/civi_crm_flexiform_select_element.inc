<?php

/**
 * Created by PhpStorm.
 * User: cravecode
 * Date: 4/28/15
 * Time: 1:56 PM
 */
class CiviCrmFlexiformSelectElement extends CiviCrmEntitiesFlexiformElementBase {


  /**
   * Return the form element for this FlexiformElement.
   */
  public function form($form, &$form_state, $entity, $language = LANGUAGE_NONE) {
    $parents = $form['#parents'];
    $parents[] = $this->element_info['name'];

    // Work out the default value.
    $default = '';
    if (!empty($this->settings['default_value']['default_value'])) {
      $default = $this->settings['default_value']['default_value'];
    }
    if (!empty($this->settings['default_value']['use_tokens'])) {
      $default = $this->replaceCtoolsSubstitutions($default, $form['#flexiform_entities']);
    }

    // Should we load an initial value from an existing entity?
    if ($entity->id && isset($entity->{$this->element_info['name']})) {
      $default = $entity->{$this->element_info['name']};
    }

    $defaul_option = array("" => t('- Select -'));

    $form[$this->element_namespace] = array(
      '#type' => 'select',
      '#parents' => $parents,
      '#options' => ($defaul_option + $this->element_info['options']),
      '#title' => $this->label(),
      '#required' => TRUE,
      '#default_value' => $default,
      '#maxlength' => 255,
      '#attached' => array(
        'library' => array(array('join_renew', 'selection-other')),
      ),
    );

    $form = parent::form($form, $form_state, $entity);

    return $form;
  }

  /**
   * Validate the form element.
   */
  public function formValidate($form, &$form_state, $entity, $language = LANGUAGE_NONE) {
    parent::formValidate($form, $form_state, $entity, $language);
  }

  /**
   * Submit the form element.
   */
  public function formSubmit($form, &$form_state, $entity, $language = LANGUAGE_NONE) {
    parent::formSubmit($form, $form_state, $entity, $language);
  }

  /**
   * Extract the submitted values for this form element.
   */
  public function formExtractValues($form, &$form_state, $entity) {
    $value = parent::formExtractValues($form, $form_state, $entity);
    return $value;
  }

  /**
   * {@inheritdoc}
   */
  public function configureForm($form, &$form_state, $flexiform) {
    $form = parent::configureForm($form, $form_state, $flexiform);

    return $form;
  }

  /**
   * {@inheritdoc}
   */
  public function configureFormSubmit($form, &$form_state, $flexiform) {

    parent::configureFormSubmit($form, $form_state, $flexiform);
  }

}

<?php

/**
 * Created by PhpStorm.
 * User: cravecode
 * Date: 4/28/15
 * Time: 1:56 PM
 */
class CiviCrmFlexiformTextElement extends CiviCrmEntitiesFlexiformElementBase {


  /**
   * Return the form element for this FlexiformElement.
   */
  public function form($form, &$form_state, $entity, $language = LANGUAGE_NONE) {
    $element = array(
      '#type' => 'textfield',
    );
    $element = self::defaultElementInfo($element, $form, $entity);

    $form[$this->element_namespace] = $element;
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

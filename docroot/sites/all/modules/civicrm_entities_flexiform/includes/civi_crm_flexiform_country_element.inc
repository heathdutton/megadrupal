<?php

class CiviCrmFlexiformCountryElement extends CiviCrmFlexiformSelectElement {


  /**
   * Return the form element for this FlexiformElement.
   */
  public function form($form, &$form_state, $entity, $language = LANGUAGE_NONE) {
    $form = parent::form($form, $form_state, $entity);
    $select = &$form[$this->element_namespace];
    $module_path = drupal_get_path('module', 'civicrm_entities_flexiform');

    $select['#attached']['js'][] = "$module_path/js/country-province.js";
    $select['#attributes']['class'][] = 'country-id';
    $select['#attributes']['data-value'] = $select['#default_value'];

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

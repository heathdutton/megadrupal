<?php
/**
 * @file
 * Provides factory interface.
 */

/**
 * Defines an extender interface for factory forms and factory methods.
 */
interface RulesFactoryUIInterface {
  /**
   * Builds the factory form for creating an instance of the plugin.
   *
   * @param $form
   *   The form array to add the factory form to.
   * @param $form_state
   *   The current form state.
   */
  public function factoryForm(&$form, &$form_state, $options = array());

  /**
   * Validates the factory form submission as built in
   * RulesFactoryUIInterface::factoryForm().
   */
  public function factoryFormValidate(&$form, &$form_state, $options = array());

  /**
   * Processes the factory form submission as built in
   * RulesFactoryUIInterface::factoryForm(). Set 'rules_factory_created' to a
   * created instance for configuration to begin.
   */
  public function factoryFormSubmit(&$form, &$form_state, $options = array());
}

/**
 * Implements a default factory UI for extending a plugin with a factory form.
 */
abstract class RulesFactoryUI extends FacesExtender implements RulesFactoryUIInterface {
  /**
   * The factory-enabled Rules plugin.
   * @var RulesPlugin
   */
  protected $element;

  /**
   * Constructs this extender object.
   */
  public function __construct(FacesExtendable $object) {
    parent::__construct($object);
    $this->element = $object;
  }

  /**
   * Implements form validation so that validation can be skipped by default.
   */
  public function factoryFormValidate(&$form, &$form_state, $options = array()) {}
}

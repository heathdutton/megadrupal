<?php
/**
 * @file
 * Test class implementations.
 */

/**
 * Testing plugin.
 */
class RulesFactoryUITestPlugin extends RulesPlugin {
  protected $itemName = 'rules_admin_factory_test';

  /**
   * Argument value.
   */
  public $foo;

  /**
   * Creates a plugin with an argument that only the factory UI knows how to use.
   */
  public function __construct($foo = NULL) {
    parent::__construct();
    $this->foo = $foo;
  }

  /**
   * Executes the configuration.
   */
  public function executeByArgs($args = array()) {
    // Not implemented.
  }

  /**
   * Evaluates the element.
   */
  public function evaluate(RulesState $state) {
    // Not implemented.
  }

  /**
   * Returns dummy array for the plugin UI.
   */
  public function &componentVariables() {
    $variables = array();
    return $variables;
  }
}

/**
 * Second testing plugin.
 */
class RulesFactoryUITestPlugin2 extends RulesFactoryUITestPlugin {
  protected $itemName = 'rules_admin_factory_test2';
}

/**
 * Embedded element plugin.
 */
class RulesFactoryUITestElementPlugin extends RulesFactoryUITestPlugin implements RulesActionInterface {
  protected $itemName = 'rules_admin_factory_test_element';
}

/**
 * Multi-step plugin.
 */
class RulesFactoryUITestMultiStepPlugin extends RulesFactoryUITestPlugin {
  protected $itemName = 'rules_admin_factory_test_multistep';
}

/**
 * Embedded element multi-step plugin.
 */
class RulesFactoryUITestElementMultiStepPlugin extends RulesFactoryUITestElementPlugin {
  protected $itemName = 'rules_admin_factory_test_element_multistep';
}

/**
 * Testing factory UI.
 */
class RulesFactoryUITestFactoryUI extends RulesFactoryUI {
  /**
   * Builds factory form for testing.
   */
  public function factoryForm(&$form, &$form_state, $options = array()) {
    // Add a hidden field for 'foo'.
    $form['foo'] = array(
      '#type' => 'hidden',
      '#value' => 'foo',
    );

    // Track form state.
    $form['init'] = array(
      '#type' => 'item',
      '#markup' => $this->getYesNoMarkup('init', !empty($options['init'])),
    );
    $form['validate'] = array(
      '#type' => 'item',
      '#markup' => $this->getYesNoMarkup('validate', !empty($form_state['form_validate'])),
    );

    // Track component.
    $form['rules_factory_created_foo'] = array(
      '#type' => 'item',
      '#markup' => $this->getMarkup('foo', isset($form_state['rules_factory_created']) ? (string) $form_state['rules_factory_created']->foo : ''),
    );
  }

  /**
   * Constructs the markup for an exposed form state value.
   */
  protected function getMarkup($name, $value) {
    return '[factory form ' . $name . ': ' . $value . ']';
  }

  /**
   * Constructs the form state markup for a boolean value.
   */
  protected function getYesNoMarkup($name, $value) {
    return $this->getMarkup($name, $value ? 'yes' : 'no');
  }

  /**
   * Validates the form.
   */
  public function factoryFormValidate(&$form, &$form_state, $options = array()) {
    $form_state['form_validate'] = TRUE;
  }

  /**
   * Creates an instance of RulesFactoryUITestPlugin.
   */
  public function factoryFormSubmit(&$form, &$form_state, $options = array()) {
    $foo = $form_state['values']['foo'];
    $form_state['rules_factory_created'] = new RulesFactoryUITestPlugin($foo);
  }
}

/**
 * Factory form for second test plugin.
 */
class RulesFactoryUITestFactoryUI2 extends RulesFactoryUITestFactoryUI {
  /**
   * Overrides the default value of 'foo'.
   */
  public function factoryForm(&$form, &$form_state, $options = array()) {
    parent::factoryForm($form, $form_state, $options);

    // Change value of 'foo'.
    $form['foo']['#value'] = 'foo2';

    // Show class of the plugin.
    $form['test2'] = array(
      '#type' => 'item',
      '#markup' => $this->getYesNoMarkup('test2', TRUE),
    );

    // Show class of the plugin.
    $form['rules_factory_created_class'] = array(
      '#type' => 'item',
      '#markup' => $this->getMarkup('class', isset($form_state['rules_factory_created']) ? get_class($form_state['rules_factory_created']) : ''),
    );
  }

  /**
   * Creates an instance of RulesFactoryUITestPlugin2.
   */
  public function factoryFormSubmit(&$form, &$form_state, $options = array()) {
    $foo = $form_state['values']['foo'];
    $form_state['rules_factory_created'] = new RulesFactoryUITestPlugin2($foo);
  }
}

/**
 * Factory form for an embeddable element plugin.
 */
class RulesFactoryUITestElementFactoryUI extends RulesFactoryUITestFactoryUI {
  /**
   * Creates an embedded plugin.
   */
  public function factoryFormSubmit(&$form, &$form_state, $options = array()) {
    $foo = $form_state['values']['foo'];
    $form_state['rules_factory_created'] = new RulesFactoryUITestElementPlugin($foo);
  }
}

/**
 * Multi-step factory form for multi-step plugin.
 */
class RulesFactoryUITestMultiStepFactoryUI extends RulesFactoryUITestFactoryUI {
  /**
   * Builds the form in two steps.
   */
  public function factoryForm(&$form, &$form_state, $options = array()) {
    // Build element for first step.
    if (empty($form_state['step'])) {
      $form['step1'] = array(
        '#type' => 'hidden',
        '#value' => 1,
      );
    }
    // Build element for second step.
    else {
      $form['foo'] = array(
        '#type' => 'hidden',
        '#value' => 'foo',
      );
    }

    // Show form step.
    $form['form_step'] = array(
      '#type' => 'item',
      '#markup' => $this->getMarkup('step', isset($form_state['step']) ? $form_state['step'] : ''),
    );

    // Show class of the plugin.
    $form['rules_factory_created_class'] = array(
      '#type' => 'item',
      '#markup' => $this->getMarkup('class', isset($form_state['rules_factory_created']) ? get_class($form_state['rules_factory_created']) : ''),
    );
  }

  /**
   * Creates an instance of RulesFactoryUITestMultiStepPlugin in the second round.
   */
  public function factoryFormSubmit(&$form, &$form_state, $options = array()) {
    // Trigger second step.
    if (!empty($form_state['values']['step1'])) {
      $form_state['step'] = 1;
      $form_state['rebuild'] = TRUE;
    }
    // Create plugin.
    elseif (!empty($form_state['values']['foo'])) {
      $form_state['step'] = 2;
      $foo = $form_state['values']['foo'];
      $form_state['rules_factory_created'] = new RulesFactoryUITestMultiStepPlugin($foo);
    }
  }
}

/**
 * Multi-step factory form for element plugin.
 */
class RulesFactoryUITestElementMultiStepFactoryUI extends RulesFactoryUITestMultiStepFactoryUI {
  /**
   * Creates an instance of RulesFactoryUITestPlugin in the second round.
   */
  public function factoryFormSubmit(&$form, &$form_state, $options = array()) {
    // Trigger second step.
    if (!empty($form_state['values']['step1'])) {
      $form_state['step'] = 1;
      $form_state['rebuild'] = TRUE;
    }
    // Create plugin.
    elseif (!empty($form_state['values']['foo'])) {
      $form_state['step'] = 2;
      $foo = $form_state['values']['foo'];
      $form_state['rules_factory_created'] = new RulesFactoryUITestElementMultiStepPlugin($foo);
    }
  }
}

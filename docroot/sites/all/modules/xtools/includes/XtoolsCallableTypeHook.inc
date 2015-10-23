<?php

/**
 * Describes hooks.
 *
 * XtoolsCallableTypeHook::name is assumed to be the name of the hook prefixed
 * with "hook_". The blueprint and signature names default to
 * XtoolsCallableType::name;
 */
class XtoolsCallableTypeHook extends XtoolsCallableType {

  /**
   * The name of the hook without the "hook_" prefix.
   *
   * @var string
   */
  public $hook = NULL;

  /**
   * Constructor.
   */
  function __construct($hook, array $properties = array()) {
    $this->hook = $hook;
    parent::__construct($properties);
  }

  /**
   * Implements XtoolsCallableType::getCallables().
   */
  function getCallables($module) {
    return module_hook($module, $this->hook) ? array($module . '_' . $this->hook) : array();
  }
}
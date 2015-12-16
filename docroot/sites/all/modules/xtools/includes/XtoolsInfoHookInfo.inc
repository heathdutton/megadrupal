<?php

class XtoolsInfoHookInfo {

  /**
   * The hook's name. Its blueprint name is assumed to be the hook name
   * prefixed with "hook_".
   *
   * @var string
   */
  public $hook = '';

  /**
   * The name of the module the hook belongs to. Use "drupal" to indicate the
   * hook is part of Drupal core.
   *
   * @var string
   */
  public $module = '';

  function __construct($hook, $module = '') {
    $this->hook = $hook;
    $this->module = $module;
  }
}
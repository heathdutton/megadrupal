<?php

/**
 * Describes renderable element callbacks.
 */
class XtoolsCallableTypeHookElementInfoCallbacks extends XtoolsCallableType {

  /**
   * Implements XtoolsCallableType::getCallables().
   */
  function getCallables($module) {
    $callback_types = array_flip(array(
      '#after_build',
      '#element_validate',
      '#pre_render',
      '#post_render',
    ));
    $callables = array();
    if ($elements_info = module_invoke($module, 'element_info')) {
      foreach ($elements_info as $element_info) {
        foreach (array_intersect_key($element_info, $callback_types) as $callbacks) {
          foreach ($callbacks as $callback) {
            // Callbacks can be located in include files we don't know about,
            // so discard unavailable callbacks.
            if (function_exists($callback)) {
              $callables[] = $callback;
            }
          }
        }
      }
    }

    return $callables;
  }
}
<?php
/**
 * @file
 * restmini_service_test: Hooks provided by this module.
 */


/**
 * Declares a list of operations to be called, including arguments and client options.
 *
 * A test may use service endpoint methods of more base routes, and more modules.
 *
 * Example of a hook_restmini_service_test() implementation.
 *
 * @see restmini_service_example_restmini_service_test()
 *
 * @return array
 */
function hook_restmini_service_test() {
  // See restmini_service_example_restmini_service_test().
  return restmini_service_example_restmini_service_test();
}

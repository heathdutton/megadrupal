<?php
/**
 * @file
 * Contains the payment form from URL argument default plugin.
 */

/**
 * Default argument plugin to extract a pay_form via menu_get_object
 */
class pay_handler_argument_default_form extends views_plugin_argument_default {
  function get_argument() {
    $item = menu_get_item();
    foreach ($item['page_arguments'] as $object) {
      // If there's a 'pfid' key in the current page argument, assume it's a
      // payment form (TODO this would conflict with anything else using pfid!)
      if (is_object($object) && isset($object->pfid)) {
        return $object->pfid;
      }

      // If there's a 'pay_form' key in the current page argument, assume it's a
      // payment form and return its pfid.
      elseif (isset($object->pay_form)) {
        return $object->pay_form->pfid;
      }
    }
  }
}

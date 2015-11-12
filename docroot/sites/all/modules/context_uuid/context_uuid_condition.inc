<?php
/**
 * @file
 * Context condition plugin implementation.
 */

/**
 * Implement condition based on a list of entities UUIDs.
 */
class context_uuid_condition extends context_condition {

  function condition_form($context) {
    //$defaults = $this->fetch_from_context($context, 'values');
    //return uuid_entity_autocomplete_form_multiple($defaults);

    // We just pass the context object here and do everything else in the
    // hook_form_alter() for having access to $form_state array.
    return array(
      '#context_uuid_context' => $context,
      '#context_uuid_context_condition' => $this,
    );
  }

  function condition_form_submit($values) {
    // If context form is submitted, return an array in a form that context
    // module is fine with.
    if (!empty($values['submitted'])) {
      if (empty($values['current_items'])) {
        return array();
      }
      $parsed = array();
      foreach ($values['current_items'] as $entity_type => $items) {
        foreach ($items as $item) {
          if (empty($item) || !is_array($item)) {
            continue;
          }
          // Context module doesn't like multi-dimensional arrays, so we need
          // to concatenate entity type and UUID like this.
          $item_string = $entity_type . ':' . $item['entity_uuid'];
          $parsed[$item_string] = $item_string;
        }
      }
      return $parsed;
    }

    return $values;
  }

  function execute() {
    if (!$this->condition_used()) {
      return;
    }
    foreach ($this->get_contexts() as $context) {
      $values = $this->fetch_from_context($context, 'values');
      foreach ($values as $item) {
        list($entity_type, $uuid) = explode(':', $item);
        if ($entity = menu_get_object($entity_type)) {
          if (!empty($entity->uuid) && $entity->uuid == $uuid) {
            $this->condition_met($context);
          }
        }
      }
    }
  }

}

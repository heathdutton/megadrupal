<?php
namespace entity_context\handler;

class EntityReference extends \entity_context\core\FieldHandler {
  /**
   * Returns value for EntityReference field.
   * @return [type] [description]
   */
  public function getValue() {
    $values_list = array();
    $wrapper = $this->getWrapper();
    $context = $this->getContext();

    // user might not have set the Entityreference field
    if (!isset($context['entityreference'])) {
      return FALSE;
    }

    $entity = $wrapper->value();
    if (!isset($entity->{$context['field']})) {
      return FALSE;
    }

    // get the wrapper for referenced entity,
    $ref_wrapper = $wrapper->{$context['field']};

    // make sure this wrapper is for an actual entity; an empty field will still return
    // a wrapper, so we must use this check
    if (!($ref_wrapper->value())) {
      return FALSE;
    }

    // make sure we're dealing with a list
    if (get_class($ref_wrapper) !== 'EntityListWrapper') {
      $ref_wrapper = array($ref_wrapper);
    }

    // use foreach to iterate the EntityListWrapper object (which implements the IteratorAggregate
    // interface)
    foreach ($ref_wrapper as $ref_field_wrapper) {
      // figure out field group and type for field chosen on referenced entity
      list($ref_field_group, $ref_field_type) = entity_context_get_field_info($context['entityreference'], $ref_field_wrapper);

      // make sure we can support this field type, and pass off the value generation
      // to the appropriate field handler
      if ($ref_field_group && $ref_field_type) {
        $types_info = entity_context_get_field_types_info();
        if (isset($types_info[$ref_field_type]) && $handler_class = entity_context_get_field_handler_class($ref_field_type)) {
          // build new context structure
          $new_context = array(
            'entity_type' => $ref_field_wrapper->type(),
            'bundle' => $ref_field_wrapper->getBundle(),
            'field' => $context['entityreference'],
          );

          $handler = new $handler_class($ref_field_wrapper, $ref_field_group, $new_context);
          $values_list[] = $handler->getValue();
        }
      }
    }

    return $values_list;
  }
}
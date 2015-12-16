<?php

/**
* Define variable blueprints.
*
* Variable blueprints define the structure certain variables should have. You
* can define a blueprint for any variable in any context, such as entities of a
* certain type, or hook implementation return values.
*
* @return array
*   Keys are variable names, values are XtoolsBlueprintInterface objects.
*/
function hook_xtools_blueprint_info() {
  // Class FooFoo's blueprint.
  $blueprints['FooFoo'] = new XtoolsBlueprintObject('FooFoo', array(
    new XtoolsBlueprintObjectProperty(new XtoolsBlueprintString, 'title'),
  ));
  // Class FooBar's blueprint.
  $blueprints['FooBar'] = new XtoolsBlueprintObject('FooBar', array(
    new XtoolsBlueprintObjectProperty(new XtoolsBlueprintString, 'title', TRUE),
    new XtoolsBlueprintObjectProperty(new XtoolsBlueprintString, 'description'),
  ));

  return $blueprints;
}

/**
* Alter variable blueprints.
*
* @param array $blueprints
*   Keys are variable names, values are XtoolsBlueprintInterface objects.
*
* @return NULL
*/
function hook_xtools_blueprint_info_alter(array &$blueprints) {
  $blueprints['FooFoo']->object_properties[] = new XtoolsBlueprintObjectProperty(new XtoolsBlueprintBoolean, 'enabled');
  $blueprints['FooBar']->object_properties['description']->required = TRUE;
}

/**
 * Expose and describe types of callables.
 *
 * @see http://php.net/manual/language.types.callable.php
 *
 * @return array
 *   An aray of XtoolsCallableType elements. Their "owner" properties will
 *   default to the name of the module implementing this hook.
 */
function hook_xtools_callable_type_info() {
  return array(
    new XtoolsCallableType('hook_foo_action_foo', array(
      'signature' => new XtoolsSignature(array(
        new XtoolsSignatureParameter(array(
          'name' => 'foo',
          'reference' => TRUE,
          'type' => 'array',
        )),
      )),
    )),
    new XtoolsCallableType('hook_foo_action_bar', array(
      'signature' => new XtoolsSignaturePlaceholder('hook_foo_action_bar'),
    )),
    new XtoolsCallableType('hook_foo_info', array(
      'blueprint' => new XtoolsBlueprintPlaceholder('hook_foo_info'),
      'signature' => new XtoolsSignaturePlaceholder('hook_foo_info'),
    )),
  );
}

/**
 * Define callable signatures.
 *
 * @see http://php.net/manual/language.types.callable.php
 *
 * @return array
 *   Keys are signature names, values are XtoolsSignature objects.
 */
function hook_xtools_signature_info() {
  return array(
    'hook_foo_action_bar' => new XtoolsSignature(array(
      new XtoolsSignatureParameter(array(
        'name' => 'foo',
        'type' => 'array',
        'default_value' => array(),
      )),
    )),
   );
 }
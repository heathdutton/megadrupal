<?php

/**
 * @file
 * Contains the root component class.
 */

namespace ModuleBuider\Generator;

/**
 * Abstract Generator for root components.
 *
 * Root components are those with which the generating process may begin, such
 * as Module and Theme.
 */
abstract class RootComponent extends BaseGenerator {

  /**
   * Get the Drupal name for this component, e.g. the module's name.
   *
   * @todo: standardize this in the different root generators' component data
   * so this is not needed.
   */
  abstract public function getComponentSystemName();

  /**
   * Define the component data this component needs to function.
   *
   * This returns an array of data that defines the component data that
   * this component should be given to perform its work. This includes:
   *  - data that must be specified by the user
   *  - data that may be specified by the user, but can be computed or take from
   *    defaults
   *  - data that should not be specified by the user, as it is computed from
   *    other input.
   *
   * This array must be processed in the order in which the properties are
   * given, so that the callables for defaults and options work properly.
   *
   * Note this can't be a class property due to use of closures.
   *
   * @return
   *  An array that defines the data this component needs to operate. The order
   *  is important, as callbacks may depend on component data that has been
   *  assembled so far, i.e., on data properties that are earlier in the array.
   *  Each key corresponds to a key for a property in the $component_data that
   *  should be passed to this class's __construct(). Each value is an array,
   *  with the following keys:
   *  - 'label': A human-readable label for the property.
   *  - 'format': (optional) Specifies the expected format for the property.
   *    One of 'string', 'array', or 'boolean'. Defaults to 'string'.
   *  - 'default': (optional) The default value for the property. This is either
   *    a static value, or a callable, in which case it must be called with the
   *    array of component data assembled so far. Depending on the value of
   *    'required', this represents either the value that may be presented as a
   *    default to the user in a UI for convenience, or the value that will be
   *    set if nothing is provided when instatiating the component.
   *  - 'required': (optional) Boolean indicating whether this property must be
   *    provided. Defaults to FALSE.
   *  - 'options': (optional) A callable which returns a list of options for the
   *    property. This receives the component data assembled so far.
   *  - 'processing': (optional) A callback to processComponentData() to use to
   *    process input values into the final format for the component data array.
   *  - 'component': (optional) The name of a generator class, relative to the
   *    namespace. If present, this results in child components of this class
   *    being added to the component tree. If the property format is 'boolean',
   *    a single child is added; if 'array', one child is added for each value
   *    in the array.
   *  - 'computed': (optional) If TRUE, indicates that this property is computed
   *    by the component, and should not be obtained from the user.
   *
   * @see getComponentDataInfo()
   */
  abstract protected function componentDataDefinition();

  /**
   * Get a list of the properties that are required in the component data.
   *
   * UIs may use this to present the options to the user. Each property should
   * be passed to prepareComponentDataProperty(), to set any option lists and
   * allow defaults to build up incrementally.
   *
   * After all data has been gathered from the user, the completed data array
   * should be passed to processComponentData().
   *
   * @param $include_computed
   *  (optional) Boolean indicating whether to include computed properties.
   *  Default value is FALSE, as UIs don't need to work with these.
   *
   * @return
   *  An array containing information about the properties this component needs
   *  in its $component_data array. Keys are the names of properties. Each value
   *  is an array of information for the property. Of interest to UIs calling
   *  this are:
   *  - 'label': A human-readable label for the property.
   *  - 'format': Specifies the expected format for the property. One of
   *    'string' or 'array'.
   *  - 'required': Boolean indicating whether this property must be provided.
   *
   * @see componentDataDefinition()
   * @see prepareComponentDataProperty()
   * @see processComponentData()
   */
  public function getComponentDataInfo($include_computed = FALSE) {
    $return = array();
    foreach ($this->componentDataDefinition() as $property_name => $property_info) {
      if (empty($property_info['computed'])) {
        $property_info += array(
          'required' => FALSE,
          'format' => 'string',
        );
      }
      else {
        if (!$include_computed) {
          continue;
        }
      }

      $return[$property_name] = $property_info;
    }

    return $return;
  }

  /**
   * Prepares a property in the component data with default value and options.
   *
   * This should be called for each property in the component data info that is
   * obtained from getComponentDataInfo(), in the order given in that array.
   * This allows UIs to present default values to the user in a progressive
   * manner. For example, the Drush interactive mode may present a default value
   * for the module human name based on the value the user has already entered
   * for the machine name.
   *
   * The default value is placed into the $component_data array; the options are
   * placed into $property_info['options'].
   *
   * @param $property_name
   *  The name of the property.
   * @param &$property_info
   *  The definition for this property, from getComponentDataInfo().
   *  If the property has options, this will have its 'options' key set, in the
   *  the format VALUE => LABEL.
   * @param &$component_data
   *  An array of component data that is being assembled. This should contain
   *  property data that has been obtained from the user so far. This will have
   *  its $property_name key set with the default value for the property,
   *  which may be calculated based on the existing user data.
   *
   * @see getComponentDataInfo()
   */
  public function prepareComponentDataProperty($property_name, &$property_info, &$component_data) {
    // Set options.
    // This is always a callable if set.
    if (isset($property_info['options'])) {
      $options_callback = $property_info['options'];
      $options = $options_callback($property_info);

      $property_info['options'] = $options;
    }

    // Set a default value, if one is available.
    if (isset($property_info['default'])) {
      // The default property is either an anonymous function, or
      // a plain value.
      if (is_callable($property_info['default'])) {
        $default_callback = $property_info['default'];
        $default_value = $default_callback($component_data);
      }
      else {
        $default_value = $property_info['default'];
      }
      $component_data[$property_name] = $default_value;
    }
    else {
      // Always set the property name, even if it's something basically empty.
      $component_data[$property_name] = $property_info['format'] == 'array' ? array() : NULL;
    }
  }

  /**
   * Process component data prior to passing it to the generator.
   *
   * Performs final processing for the component data:
   *  - sets default values on empty properties
   *  - performs additional processing that a property may require
   *  - expand properties that represent child components.
   *
   * @param $component_data_info
   *  The complete component data info.
   * @param &$component_data
   *  The component data array.
   */
  public function processComponentData($component_data_info, &$component_data) {
    // Set defaults for properties that don't have a value yet.
    // First, get the component data info again, with the computed properties
    // this time, so we can add them in.
    $component_data_info_original = $this->getComponentDataInfo(TRUE);
    foreach ($component_data_info_original as $property_name => $property_info) {
      if (!empty($property_info['computed'])) {
        $component_data_info[$property_name] = $property_info;
      }
    }

    // TODO: refactor this with code in prepareComponentDataProperty().
    foreach ($component_data_info as $property_name => $property_info) {
      if (!empty($component_data[$property_name])) {
        continue;
      }

      if (isset($property_info['default'])) {
        if (is_callable($property_info['default'])) {
          $default_callback = $property_info['default'];
          $default_value = $default_callback($component_data);
        }
        else {
          $default_value = $property_info['default'];
        }
        $component_data[$property_name] = $default_value;
      }
    }

    // Allow each property to apply its processing callback. Note that this may
    // set or alter other properties in the component data array.
    foreach ($component_data_info as $property_name => $property_info) {
      if (isset($property_info['processing']) && !empty($component_data[$property_name])) {
        $processing_callback = $property_info['processing'];

        $processing_callback($component_data[$property_name], $component_data, $property_info);
      }
    } // processing callback

    // Expand any properties that represent child components to add.
    // TODO: This is a fairly rough piece of functionality that needs more
    // thought.
    foreach ($component_data_info as $property_name => $property_info) {
      if (isset($property_info['component']) && !empty($component_data[$property_name])) {
        // Get the component type.
        $component_type = $property_info['component'];

        switch ($property_info['format']) {
          case 'array':
            // Each value in the array is the name of a component.
            foreach ($component_data[$property_name] as $requested_component_name) {
              $component_data['requested_components'][$requested_component_name] = $component_type;
            }
            break;
          case 'boolean':
            // The component type can only occur once and therefore the name is
            // the same as the type.
            $component_data['requested_components'][$component_type] = $component_type;
            break;
        }
      }
    } // expand components
  }

}

<?php
/**
 * @file
 * Hooks provided by the Compass Filter of Assetic.
 */

/**
 * Alter the Compass filter instance before it is used.
 *
 * @param Assetic\Filter\CompassFilter $filter
 *   A CompassFilter instance.
 */
function hook_assetic_filter_instance_compass_alter($filter) {
  // Add the susy plugin to the Compass filter,
  // like you normally would do in 'require.rb'.
  // You can now use the plugin in your stylesheet with '@import "susy"'.
  $filter->addPlugin('susy');
}

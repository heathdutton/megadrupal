<?php

/**
 * @file
 * API provided by Organic groups time frame and example usage.
 */

/**
 * @addtogroup hooks
 * @{
 */

/**
 * Provide time frame handlers for groups and group content.
 *
 * The info tree may be four levels deep, keyed by, in order:
 * - Group entity type
 * - Group entity bundle
 * - Group content entity type
 * - Group content entity bundle
 * At every level you may provide a handler using the '#class' property as key.
 * The shallow handlers will have the biggest impact, but the deepest handlers
 * will take precedence over the shallow ones.
 *
 * @return array
 *   Tree of pointers to implemented 'OgTimeframeHandlerInterface'. In practice
 *   you will most likely extend OgTimeFrameHandler or any of the variants in
 *   og_timeframe_datefield.module and provide this as handler.
 */
function hook_og_timeframe_handler_info() {
  $handlers = array(
    // ↓ The group entity type.
    'node' => array(
      // ↓ The group entity bundle.
      'example_group' => array(
        // ↓ The group content entity type.
        'node' => array(
          // ↓ The group content entity bundle. This is the content type that
          // will be restricted access by time frame.
          'example_content' => array(
            // This handler will always be chosen for this type, overriding the
            // more shallow ones.
            '#class' => 'ExampleContentTimeframeHandler',
            // In addition you can provide som other random properties
            // (expensively) collected during this hook to your class during
            // object instantiation (__construct($info)). Info returned during
            // this hook is cached and therefore run rarely. Only keyes starting
            // with '#' is treated as properties and handed over during
            // instantiation.
            '#foo' => 'bar',
          ),
          // You can provide handlers in every level of the info tree, This
          // handler will be invoked for all group content that are attached to
          // groups of type 'example_group', except for 'example_content' types.
          '#class' => 'ExampleTimeframeHandler',
        ),
      ),
    ),
    // This is the omnipotent handler.
    '#class' => 'ExampleEverythingHandler',
  );

  // Shorthand version.
  $handlers['node']['example_group']['node']['example_content_2'] = array(
    '#class' => 'ExampleTimeframeHandler',
    '#foo' => 'baz',
  );
  return $handlers;
}

/**
 * Alter organic group time frame handler info.
 *
 * Use this hook to replace, add or update data for existing time frame
 * handlers. In situations where several modules maps up the same target, this
 * is the place to clean up.
 *
 * @param array $info
 *   The tree of current handlers.
 */
function hook_og_timeframe_handler_info_alter(array &$info) {
  // Update information of an existing handler.
  if (!empty($info['node']['example_group']['node']['example_content_2']['#class'])) {
    $info['node']['example_group']['node']['example_content_2']['#foo'] = 'cafe';
  }
}

/**
 * @} End of "addtogroup hooks".
 */

/**
 * @addtogroup callbacks
 * @{
 */

/**
 * This class is instantiated when access to group content is requested.
 */
class ExampleContentTimeframeHandler extends OgTimeframeHandler {

  /**
   * Overrides OgTimeframeHandler::__construct().
   */
  public function __construct($info) {
    // $info contains the same info registered in hook_timeframe_handler_info(),
    // with the addition of group and group content type and bundle.
    parent::__construct();
    // Do something with the provided properties.
    drupal_set_message(t("Two developers walks into a @establishment", array('@establishment' => ($info['#foo']))));
  }

  /**
   * Overrides OgTimeframeHandler::setGroup().
   */
  public function setGroup($group) {
    // Use this to provide your class a context to work with.
    $this->fromDate = new DateObject(some_logic_to_get_date($group->nid));
    $this->toDate = new DateObject('2014-05-17 12:00');
    parent::setGroup($group);
  }

  /**
   * Overrides OgTimeframeHandler::getMeasure().
   */
  public function getMeasure() {
    // Use whole days as time frame.  The day used in $to_date will then be a
    // valid date to edit content in.
    return 'days';
  }

}

/**
 * @} End of "addtogroup callbacks"
 */

<?php

/**
 * @file
 * The empty_text empty behavior class.
 */

/**
 * Empty behavior plugin that displays markup, usually just some text.
 *
 * This plugin allows administrators to display markup filtered through text
 * formats as the contents of a facet when it has no items.
 */
class FacetapiEmptyBehaviorAjaxFacets extends FacetapiEmptyBehavior {

  /**
   * A boolean flagging whether the input format is set, FALSE means it is
   * being pulled from FacetapiEmptyBehavior::getDefaultSettings().
   *
   * @var bool
   */
  protected $formatSet = FALSE;

  /**
   * Overrides FacetapiEmptyBehavior::__construct().
   *
   * Checks if a format was selected, calls parent's constructor.
   */
  public function __construct(stdClass $settings) {
    if (isset($settings->settings['empty_text']['format'])) {
      $this->formatSet = TRUE;
    }
    parent::__construct($settings);
  }

  /**
   * Implements FacetapiEmptyBehavior::execute().
   */
  public function execute() {
    switch ($this->settings['widget']) {
      case 'facetapi_ajax_checkboxes':
        $class1 = 'facet-wrapper-ajax-checkboxes';
        break;
      case 'facetapi_ajax_select':
        $class1 = 'facet-wrapper-ajax-selectbox';
        break;
      case 'facetapi_ajax_links':
        $class1 = 'facet-wrapper-ajax-links';
        break;
      default:
        $class1 = '';
        break;
    }

    $class2 = str_replace(array('_', ':'), '-', $this->configName);
    $class2 = str_replace('@', '', $class2);
    $wrapper = '<div class="' . $class1 . ' facetapi-facet-' . $class2 . '" id="facetapi-facet-' . $class2 . '"></div>';
    return array('#markup' => $wrapper);
  }
}

<?php

/**
 * @file
 * Contains TetherStatsChartsSchema.
 */

require_once __DIR__ . '/TetherStatsChartsSchemaInterface.php';

/**
 * Abstract class for a chart schema.
 */
abstract class TetherStatsChartsSchema implements TetherStatsChartsSchemaInterface {

  /**
   * The unique chartId.
   *
   * Used on the containing html element and on iteration callbacks to
   * identify this chart.
   *
   * @var string
   */
  public $chartId;

  /**
   * The array of chartable item definitions.
   *
   * @var array
   */
  protected $chartableItems = array();

  /**
   * Constructs a new chart schema with the specified $chart_id.
   *
   * @param string $chart_id
   *   The unique id of the chart. Must be machine friendly. Used as a
   *   javascript variable.
   */
  public function __construct($chart_id) {

    $this->chartId = $chart_id;
  }

  /**
   * Gets the array specification for a chartable item.
   *
   * This is generally only useful for the Chart object which uses this
   * schema.
   *
   * @param int|null $index
   *   (Optional) The index of the chartable item spec to return.
   *
   * @return array
   *   The chartable item spec. If $index is NULL, the entire array of
   *   chartable items will be returned.
   */
  public function getChartableItemSpec($index = NULL) {

    if (isset($index)) {

      return $this->chartableItems[$index];
    }
    return $this->chartableItems;
  }

  /**
   * Gets the title for the chartable item.
   *
   * @param int $index
   *   (Optional) The index of the chartable item title to return.
   *
   * @return array
   *   The chartable item title. If $index is NULL, an array of titles
   *   for all chartable items will be returned.
   */
  public function getChartableItemTitle($index = NULL) {

    if (isset($index)) {

      return $this->chartableItems[$index]['title'];
    }
    else {

      $titles = array();

      foreach ($this->chartableItems as $item) {

        $titles[] = $item['title'];
      }
      return $titles;
    }
  }

  /**
   * Adds a chartable item for an event.
   *
   * Display an item for the total number of events of a specified type such
   * as a "hit" or a "click".
   *
   * @param string $type
   *   The type of activity for the event such as a 'hit' or a 'click'.
   * @param string $title
   *   The title of the item. This is used on the chart legend if applicable.
   *
   * @return $this
   */
  public function addEventItem($type, $title) {

    $this->chartableItems[] = array(
      'analytics callback' => 'getAllActivityCount',
      'callback arguments' => array($type),
      'title' => $title,
    );
    return $this;
  }

  /**
   * Adds a chartable item for an event on a particular element.
   *
   * Display a item of the number of events for a given element counting  of
   * a specified type such as a "hit" or a "click".
   *
   * @param int $elid
   *   The element Id.
   * @param string $type
   *   The type of activity for the event such as a 'hit' or a 'click'.
   * @param string $title
   *   The title of the item. This is used on the chart legend.
   *
   * @return $this
   */
  public function addElementItem($elid, $type, $title) {

    $this->chartableItems[] = array(
      'analytics callback' => 'getElementActivityCount',
      'callback arguments' => array($elid, $type),
      'title' => $title,
    );
    return $this;
  }

  /**
   * Adds a chartable item for an element impressed on an element.
   *
   * Display an item that counts how many times a given element was impressed
   * on another given element.
   *
   * @param int $elid_impressed
   *   The element Id which was impressed.
   * @param int $elid_impressed_on
   *   The element Id to which the $elid_impressed element was impressed upon.
   * @param string $title
   *   The title of the item. This is used on the chart legend.
   *
   * @return $this
   */
  public function addElementImpressedOnElementItem($elid_impressed, $elid_impressed_on, $title) {

    $this->chartableItems[] = array(
      'analytics callback' => 'getElementImpressedOnElementCount',
      'callback arguments' => array($elid_impressed, $elid_impressed_on),
      'title' => $title,
    );
    return $this;
  }

  /**
   * Adds a chartable item for an element impressed on nodes of a given type.
   *
   * Display an item that counts how many times a given element was impressed on
   * pages of a given node type.
   *
   * @param int $elid_impressed
   *   The element Id which was impressed.
   * @param string $type
   *   The node bundle type on which the $elid_impressed was impressed upon.
   * @param string $title
   *   The title of the item. This is used on the chart legend.
   *
   * @return $this
   */
  public function addElementImpressedOnNodeTypeItem($elid_impressed, $type, $title) {

    $this->chartableItems[] = array(
      'analytics callback' => 'getElementImpressedOnNodeBundleCount',
      'callback arguments' => array($elid_impressed, $type),
      'title' => $title,
    );
    return $this;
  }

  /**
   * Adds a chartable item for an element impressed on pages with a base url.
   *
   * Display an item that counts how many times was impressed on all pages given
   * a common base url.
   *
   * @param int $elid_impressed
   *   The element Id which was impressed.
   * @param string $base_url
   *   The base url of pages for which the $elid_impressed was impressed upon.
   * @param string $title
   *   The title of the item. This is used on the chart legend.
   *
   * @return $this
   */
  public function addElementImpressedOnPagesWithBaseUrlItem($elid_impressed, $base_url, $title) {

    $this->chartableItems[] = array(
      'analytics callback' => 'getElementImpressedOnBaseUrlCount',
      'callback arguments' => array($elid_impressed, $base_url),
      'title' => $title,
    );
    return $this;
  }

  /**
   * Adds a chartable item for an element impressed anywhere.
   *
   * Display an item that counts how many times a given element was impressed
   * regardless of where that impression occurred.
   *
   * @param int $elid_impressed
   *   The element Id which was impressed.
   * @param string $title
   *   The title of the item. This is used on the chart legend.
   *
   * @return $this
   */
  public function addElementImpressedAnywhereItem($elid_impressed, $title) {

    $this->chartableItems[] = array(
      'analytics callback' => 'getElementImpressedAnywhereCount',
      'callback arguments' => array($elid_impressed),
      'title' => $title,
    );
    return $this;
  }

  /**
   * Adds a chartable item for all elements impressed on an element.
   *
   * Display an item that counts how many times other elements were impressed on
   * a given element.
   *
   * @param int $elid_impressed_on
   *   The element Id of the element which had other elements impressed upon it.
   * @param string $title
   *   The title of the item. This is used on the chart legend.
   *
   * @return $this
   */
  public function addAllElementsImpressedByElementItem($elid_impressed_on, $title) {

    $this->chartableItems[] = array(
      'analytics callback' => 'getAllElementsImpressedOnElementCount',
      'callback arguments' => array($elid_impressed_on),
      'title' => $title,
    );
    return $this;
  }

  /**
   * Adds a chartable item for hits on an element by a referrer.
   *
   * Display an item that counts how many times a "hit" event occurred on a
   * given element where a given string matches somewhere in the referrer field
   * of the activity log.
   *
   * For example, a referrer of "google" will count how many times a user has
   * "hit" the given element from a google source page.
   *
   * @param int $elid
   *   The element Id.
   * @param string $referrer
   *   The referrer string to match with the referrer field of the activity.
   * @param string $title
   *   The title of the item. This is used on the chart legend.
   *
   * @return $this
   */
  public function addElementReferredByItem($elid, $referrer, $title) {

    $this->chartableItems[] = array(
      'analytics callback' => 'getElementHitActivityWithReferrerCount',
      'callback arguments' => array($elid, $referrer),
      'title' => $title,
    );
    return $this;
  }

  /**
   * Adds a chartable item for hits on all elements by a referrer.
   *
   * Display an item that counts how many times a "hit" event occurred on any
   * element where a given string matches somewhere in the referrer field
   * of the activity log.
   *
   * For example, a referrer of "google" will count how many times a user has
   * "hit" the given element from a google source page.
   *
   * @param string $referrer
   *   The referrer string to match with the referrer field of the activity.
   * @param string $title
   *   The title of the item. This is used on the chart legend.
   *
   * @return $this
   */
  public function addAllElementsReferredByItem($referrer, $title) {

    $this->chartableItems[] = array(
      'analytics callback' => 'getHitActivityWithReferrerCount',
      'callback arguments' => array($referrer),
      'title' => $title,
    );
    return $this;
  }

  /**
   * Adds a chartable item for hits on an element using a browser.
   *
   * Display an item that counts how many times a "hit" event occurred on a
   * given element where a given string matches somewhere in the browser field
   * of the activity log.
   *
   * For example, a browser of "firefox" will count how many times the given
   * element has been "hit" by users using Firefox.
   *
   * @param int $elid
   *   The element Id.
   * @param string $browser
   *   The string to match with the browser field of the activity.
   * @param string $title
   *   The title of the item. This is used on the chart legend.
   *
   * @return $this
   */
  public function addElementWithBrowserItem($elid, $browser, $title) {

    $this->chartableItems[] = array(
      'analytics callback' => 'getElementHitActivityWithBrowserCount',
      'callback arguments' => array($elid, $browser),
      'title' => $title,
    );
    return $this;
  }

  /**
   * Adds a chartable item for hits on all elements using a browser.
   *
   * Display an item that counts how many times a "hit" event occurred on any
   * element where a given string matches somewhere in the browser field
   * of the activity log.
   *
   * For example, a browser of "firefox" will count how many times the given
   * element has been "hit" by users using Firefox.
   *
   * @param string $browser
   *   The string to match with the browser field of the activity.
   * @param string $title
   *   The title of the item. This is used on the chart legend.
   *
   * @return $this
   */
  public function addAllElementsWithBrowserItem($browser, $title) {

    $this->chartableItems[] = array(
      'analytics callback' => 'getHitActivityWithBrowserCount',
      'callback arguments' => array($browser),
      'title' => $title,
    );
    return $this;
  }

}

<?php
/**
 * @file
 * Class for binding Tealium utag_data to a jQuery selector event.
 *
 * Binds the sending of a Tealium Universal Data Object (utag_data) to the firing of a jQuery element event.
 */

namespace Drupal\tealium\Data;

use Drupal\tealium\TealiumUtagData;

class TealiumJqueryEventBinding implements TealiumJqueryEventBindingInterface {

  private $jQuerySelector = '';

  private $domEvent = 'click';

  private $trackType = 'link';

  private $tealiumData = NULL;

  function __toString() {
    $utag_data = strval($this->getTealiumData());
    $selector = $this->getJQuerySelector();
    $event = $this->getDomEvent();
    $type = $this->getTrackType();

    return <<< "__END_OF_BOUND_VARIABLE_SCRIPT__"

    (function($, u) {
      $(document).ready(function () {
        $('{$selector}').bind('{$event}', function(event){
          u.{$type}({$utag_data});
        });
      });
    }(jQuery, utag));
__END_OF_BOUND_VARIABLE_SCRIPT__;
  }

  // @todo: Look at renaming this method to improve
  function getJqueryCodeToAttachBindings() {
    if ($this->getTealiumData() !== NULL
        && $this->getJQuerySelector()
        && $this->getDomEvent()
        && $this->getTrackType()
    ) {
      return strval($this);
    }
    else {
      return NULL;
    }
  }

  /**
   * Set a jQuery selector for an element to bind the Data to.
   * @param string $jquery_selector
   *  A string containing a selector expression
   * @return $this
   * @see jQuery
   * @link http://api.jquery.com/jQuery/#jQuery-selector-context jQuery
   */
  public function setJQuerySelector($jquery_selector) {
    $this->jQuerySelector = strval($jquery_selector);

    return $this;
  }

  public function getJQuerySelector() {
    return $this->jQuerySelector;
  }

  /**
   * @param string $event_name
   *  A string containing one or more DOM event types, such as "click" or "submit," or custom event names.
   * @return $this
   * @see jQuery.bind
   * @link http://api.jquery.com/bind/ jQuery.bind
   */
  public function setDomEvent($event_name) {
    $this->domEvent = strval($event_name);

    return $this;
  }

  public function getDomEvent() {
    return $this->domEvent;
  }

  /**
   * @param string $tracking_type
   *  Set the Tealium tracking type to use [link|view]
   * @return $this
   * @deprecated Use self::setTrackTypeToLink or self::setTrackTypeToView
   * @todo remove this method and switch usages to self::setTrackTypeToLink or self::setTrackTypeToView
   */
  public function setTrackType($tracking_type) {
    if ($tracking_type === 'link' || $tracking_type === 'view') {
      $this->trackType = $tracking_type;
    }

    return $this;
  }

  /**
   * Set the Tealium tracking type to 'link'
   * @return $this
   */
  public function setTrackTypeToLink() {
    $this->trackType = 'link';

    return $this;
  }

  /**
   * Set the Tealium tracking type to 'view'
   * @return $this
   */
  public function setTrackTypeToView() {
    $this->trackType = 'view';

    return $this;
  }

  public function getTrackType() {
    return $this->trackType;
  }

  /**
   * @param TealiumUtagData $tealium_data_source_values
   * @return $this
   */
  public function setTealiumData(TealiumUtagData $tealium_data_source_values) {
    $this->tealiumData = $tealium_data_source_values;

    return $this;
  }

  /**
   * @return TealiumUtagData|null
   */
  public function getTealiumData() {
    if ($this->tealiumData instanceof TealiumUtagData
        && count($this->tealiumData->getAllDataSourceValues()) > 0
    ) {
      return $this->tealiumData;
    }
    else {
      return NULL;
    }
  }

}
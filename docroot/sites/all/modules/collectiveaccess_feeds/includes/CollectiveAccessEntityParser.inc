<?php

/**
 * @file
 * Helper class to parse CollectiveAccessEntity attributes for easier access.
 * This is used by collectiveaccess_feeds module to provide a Feeds-ready entity to map
 */

class CollectiveAccessEntityParser extends CollectiveAccessBaseParser {

  public function __construct(CollectiveAccessEntity $ca_item, $config = array()) {
    parent::__construct($ca_item, $config, 'ca_entities');
  }

  /**
   * Parse the CollectiveAccessEntity to make each attribute more easily accessible
   */
  public function parse() {
    $this->parse_attributes();
    $this->parse_labels();
  }

  /**
   * Helper method for the parsing of preferred labels
   */
  protected function parse_labels() {
    if ($this->ca_item->preferred_labels) {
      $a_label = NULL;
      foreach ($this->ca_item->preferred_labels as $locale => $value) {
        $locale_key = ($locale != 'NONE') ? 'title-' . $locale : 'title';
        $this->ca_item->{$locale_key} = $value['displayname'];
        // set at least one label if we found one. Useful in cases where a name is only
        // entered in one locale, since a name normally doesn't change per locale...
        if (!empty($value['displayname'])) {
          $a_label = $value['displayname'];
        }
      }
    }
    $this->ca_item->title = $a_label;
  }
}
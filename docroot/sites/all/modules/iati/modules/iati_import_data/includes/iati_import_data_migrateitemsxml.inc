<?php

/**
 * @file.
 *
 * Override MigrateItemsXML to allow concatenation
 * of multiple itemIDXpath elements.
 */

 class OAMMigrateItemsXML extends MigrateItemsXML {
  /**
   * Call to the class constructor.
   */
  public function __construct($xml_url, $item_xpath='item', $itemID_xpath='id') {
    parent::__construct($xml_url, $item_xpath, $itemID_xpath);
  }

  /**
   * Override MigrateItemsXML::getItemID,
   * to allow concatenation of multiple itemIDXpath elements.
   *
   * @param int $itemXML
   *  The itemXML based on itemIDXpath.
   *  
   * @return string
   */
  protected function getItemID($itemXML) {
    $value = NULL;
    if ($itemXML) {
      $results = $itemXML->xpath($this->itemIDXpath);
      if ($results) {
        $value = implode('^', $results);
      }
    }
    return $value;
  }
}

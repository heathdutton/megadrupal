<?php

/**
 * @file
 * Contains class for Field Collection layout.
 */

namespace Drupal\dynamic_panes_fc_layout;

use Drupal\dynamic_panes\Layout;

/**
 * Class for Field Collection layout.
 */
class FieldCollectionLayout extends Layout {

  /**
   * @var \EntityDrupalWrapper
   */
  protected $data;

  /**
   * @var string
   */
  protected $regionName;

  /**
   * Implements Layout::initRegion().
   */
  protected function initRegion($region_name) {
    if (isset($this->data->{DYNAMIC_PANES_FC_LAYOUT_FIELD_LAYOUT_REGIONS})) {
      $this->regionName = $region_name;

      foreach ($this->data->{DYNAMIC_PANES_FC_LAYOUT_FIELD_LAYOUT_REGIONS} as $fc_region_wrapper) {
        $region = $this->wrapRegion($fc_region_wrapper);
        if ($region && $region->getRegionName() == $region_name) {
          $this->region = $region;
        }
      }

      // Init empty region if not exist.
      if (!isset($this->region)) {
        $region = $this->wrapRegion(NULL);
        if ($region && $region->getRegionName() == $region_name) {
          $this->region = $region;
        }
      }
    }
  }

  /**
   * Implements Layout::getLayoutType().
   */
  public function getLayoutType() {
    return DYNAMIC_PANES_FC_LAYOUT_ENTITY_TYPE_NAME;
  }

  /**
   * Implements Layout::getLayoutName().
   */
  public function getLayoutName() {
    return $this->data->label();
  }

  /**
   * Implements Layout::getLayoutID().
   */
  public function getLayoutID() {
    return $this->data->getIdentifier();
  }

  /**
   * Get region name attached to this layout.
   *
   * @return string
   *   The region name.
   */
  public function getRegionName() {
    return $this->regionName;
  }
}

<?php

/**
 * @file
 * Provides Entity metadata integration.
 */

class NodeTabMetadataController extends EntityDefaultMetadataController {

  public function entityPropertyInfo() {
    $info = parent::entityPropertyInfo();
    $properties = &$info[$this->type]['properties'];
    // Make the label translatable.
    $properties['label']['translatable'] = TRUE;
    $properties['label']['i18n string'] = TRUE;

    return $info;
  }
}

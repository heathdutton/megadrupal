<?php

/**
 * @file
 * Definition of WoW\item\ItemController.
 */

/**
 * Controller class for items.
 *
 * This extends the EntityAPIController class, adding required special handling
 * for item objects.
 */
class WoWItemController extends WoWRemoteEntityController {

  protected function processResult($entity, $result) {
    $language = entity_language('wow_item', $entity);
    $entity->wow_item[$language][0] = array(
      'name' => $entity->name,
      'description' => $entity->description,
    );
  }

}

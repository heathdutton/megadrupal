<?php
/**
* @file
* Contains CommercePricelistListController.
*
* Handles CRUD for Pricelists.
*/


/**
 * Handles CRUD for Pricelists.
 * {@inheritdoc}
 */
class CommercePricelistListController extends EntityAPIController {

  /**
   * {@inheritdoc}
   */
  public function create(array $values = array()) {
    $values += array(
      'list_id' => 0,
      'weight' => 0,
      'title' => '',
      'status' => 1
    );
    return parent::create($values);
  }

  /**
   * {@inheritdoc}
   */
  public function save($entity, DatabaseTransaction $transaction = NULL) {
    // If our entity has no id, then we need to give it a
    // time of creation.
    if (empty($entity->list_id)) {
      $entity->created = time();
    }

    return parent::save($entity, $transaction);
  }


  /**
   * {@inheritdoc}
   */
  public function delete($ids, DatabaseTransaction $transaction = NULL) {
    parent::delete($ids, $transaction);

    // Delete orphan pricelist items.
    $query = db_select('commerce_pricelist_list', 'l')
      ->fields('l', array('item_id'))
      ->condition('list_id', $ids, 'IN')
      ->execute();
    $pricelist_items = $query->fetchAllKeyed();

    entity_delete_multiple('commerce_pricelist_item', $pricelist_items);
  }
}

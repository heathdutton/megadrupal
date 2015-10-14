<?php
/**
 * @file
 *   Rel implementation for n_to_n.
 */

/**
 * Implements n_to_n rel interface.
 */
class SDBRelNtoN extends SDBRel {

  /**
   * For N to N relation it provides mapping table of
   * current object and rel object/s.
   *
   * @return string
   *   Mapping table name.
   */
  function getMappingTable() {
    $rel_definition = $this->getDefinition();

    if (empty($rel_definition)) {
      throw new SDBException('REL Definition for provided class does not exist.');
    }

    if (!isset($rel_definition[$this->rel_class]['mapping_table'])) {
      throw new SDBException('REL Definition does not contain mapping table.');
    }

    return $rel_definition[$this->rel_class]['mapping_table'];
  }

  /**
   * Gets a list of all rel object linked with current instance.
   *
   * @param array $order_by
   *   Defines sorting of results:
   *   array('field' => 'created', 'direction' => 'ASC')
   * @param string $key_by
   *   Field which value will be used as key for the result array.
   * @param boolean $reload
   *   Flag to reload internal object cache.
   * @return array
   *   Array of SDBObjectInterface objects.
   */
  function getAll($order_by = NULL, $key_by = NULL, $reload = FALSE) {

    if (isset($this->objects[$this->rel_class]) && !$reload) {
      return $this->objects[$this->rel_class];
    }

    $rel_instance = self::getUtilRelInstance($this->rel_class);
    $rel_table = $rel_instance->getTable();
    $rel_primary_key = $rel_instance->getPrimaryKey();
    $mapping_table = $this->getMappingTable($this->rel_class);
    $foreign_key = $this->getFK($this->rel_class);
    $my_key = $this->getMyKey($this->rel_class);

    $select = db_select($rel_table)->fields($rel_table);
    $select->join($mapping_table, $mapping_table,
                  $mapping_table . '.' . $foreign_key . ' = ' . $rel_table . '.' . $rel_primary_key);
    $select->condition($mapping_table . '.' . $my_key, $this->getBaseObject()->getPrimaryKeyValue());

    if (is_array($order_by)) {
      if (isset($order_by['field'])) {
        $select->orderBy($order_by['field'], isset($order_by['direction']) ? $order_by['direction'] : 'ASC');
      }
      elseif (isset($order_by[0]) && is_array($order_by[0])) {
        foreach ($order_by as $def) {
          $select->orderBy($def['field'], isset($def['direction']) ? $def['direction'] : 'ASC');
        }
      }
    }

    if (empty($key_by)) {
      $key_by = $rel_primary_key;
    }

    if (!property_exists($rel_instance, $key_by)) {
      throw new SDBException('The key for REL objects index is undefined.');
    }

    $res = $select->execute()->fetchAllAssoc($key_by);
    $this->objects[$this->rel_class] = SDBFactory::initInstancesOf($this->rel_class, $res);

    return $this->objects[$this->rel_class];
  }

  /**
   * Creates rel object and will link it to the current object.
   *
   * @param array $data
   *   Data to create the rel object.
   * @return SDBObjectInterface
   *   Newly created and linked rel object.
   */
  function add(array $data) {
    $rel_def = $this->getDefinition();

    if (!isset($rel_def[$this->rel_class]['foreign_key'])) {
      throw new SDBException('Foreign key of REL object is not defined.');
    }

    $transaction = db_transaction();

    $rel_object = SDBFactory::initInstanceOf($this->rel_class, $data);
    $rel_object->save();
    $foreign_key_value = $rel_object->getPrimaryKeyValue();

    if (empty($foreign_key_value)) {
      $transaction->rollback();
      throw new SDBException('Save action of the REL object produced an instance without primary key.');
    }

    try {
      $this->link($rel_object);
    }
    catch (SDBException $e) {
      $transaction->rollback();
      throw $e;
    }

    return $rel_object;
  }

  /**
    * Deletes object.
    *
    * @return int
    * 	Number of affected rows, or boolean false on error
    * @throws PDOException
    */
  function delAll() {
    $res = parent::delAll();
    $this->delMappings();

    return $res;
  }

  /**
   * Removes entries from mapping table.
   */
  function delMappings() {
    $delete = db_delete($this->getMappingTable());
    $delete->condition($this->getMyKey(), $this->getBaseObject()->getPrimaryKeyValue());
    $delete->execute();
  }

  /**
   * Will link existing rel object to current object.
   *
   * @param SDBObjectInterface $rel_object
   *   Rel object to be linked to current object.
   * @return SDBObjectInterface
   *   Linked rel object.
   */
  function link(SDBObjectInterface $rel_object) {
    $class = get_class($rel_object);
    $rel_def = $this->getDefinition();
    $foreign_key_value = $rel_object->getPrimaryKeyValue();

    if (!$rel_object->isPersisted() || empty($foreign_key_value)) {
      throw new SDBException('Cannot map not persisted object or with undefined primary key value.');
    }

    $data = array(
      $this->getMyKey() => $this->getBaseObject()->getPrimaryKeyValue(),
      $rel_def[$class]['foreign_key'] => $foreign_key_value,
    );

    $res = drupal_write_record($this->getMappingTable(), $data);

    if ($res != SAVED_NEW) {
      throw new SDBException('Error linking REL object.');
    }

    return $rel_object;
  }

  /**
   * Remove link from provided object to the current one.
   *
   * @param SDBObjectInterface $rel_object
   *   Rel object to unlink from current object.
   * @return SDBObjectInterface
   *   Unlinked rel object.
   */
  function unlink(SDBObjectInterface $rel_object) {
    $rel_class = get_class($rel_object);
    $foreign_key = $this->getFK($rel_class);
    db_delete($this->getMappingTable($rel_class))
        ->condition($this->getMyKey($rel_class), $this->getBaseObject()->getPrimaryKeyValue())
        ->condition($this->getFK($rel_class), $rel_object->{$foreign_key})
        ->execute();

    return $rel_object;
  }
}

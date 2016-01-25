<?php
/**
 * @file
 *   Rel implementation for 1_to_n.
 */

/**
 * Implements 1_to_n rel interface.
 */
class SDBRel1toN extends SDBRel {

  /**
   * For N to N relation it provides mapping table of
   * current object and rel object/s.
   *
   * @return string
   *   Mapping table name.
   */
  function getMappingTable() {
    throw new SDBException('This REL type does not provide mapping table.');
  }

  /**
   * Gets a list of all rel object linked with current instance.
   *
   * @param array $order_by
   *   Defines sorting of results:
   *   array('field' => 'created', 'direction' => 'ASC')
   * @param string $key_by
   *   Field which value will be used as key for the result array.
   *   If none provided the REL object primary key name will be used.
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
    $foreign_key = $this->getFK($this->rel_class);

    $select = db_select($rel_table)->fields($rel_table)
        ->condition($foreign_key, $this->getBaseObject()->getPrimaryKeyValue());

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
      throw new SDBException('The key for REL objects index is undefined');
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
    $rel_object = SDBFactory::initInstanceOf($this->rel_class, $data);
    $foreign_key = $this->getFK($this->rel_class);
    $rel_object->$foreign_key = $this->getBaseObject()->getPrimaryKeyValue();
    return $rel_object->save();
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
    $foreign_key = $this->getFK(get_class($rel_object));
    $rel_object->$foreign_key = $this->getBaseObject()->getPrimaryKeyValue();
    return $rel_object->save();
  }

  /**
   * Removes link from current object to the provided one.
   *
   * @param SDBObjectInterface $rel_object
   *   Rel object to unlink from current object.
   * @return SDBObjectInterface
   *   Unlinked rel object.
   */
  function unlink(SDBObjectInterface $rel_object) {
    $foreign_key = $this->getFK(get_class($rel_object));
    $rel_object->$foreign_key = NULL;
    return $rel_object->save();
  }
}

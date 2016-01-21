<?php

/**
 * @file
 * Definition of SalsaEntity.
 */

/*
 * Entity class for the Salsa entity.
 */
class SalsaEntity extends Entity {

  /**
   * Can be used to pass additional fields to the save callback.
   *
   * @var array
   */
  public $additional = array();

  /**
   * Overrides Entity::buildContent().
   */
  public function buildContent($view_mode = 'full', $langcode = NULL) {
    $wrapper = entity_metadata_wrapper($this->entityType, $this);
    foreach ($wrapper as $key => $property) {
      $info = $property->info();
      $content[$key] = array(
        '#type' => 'item',
        '#title' => $info['label'],
        '#markup' => $property->value(),
      );
    }
    return entity_get_controller($this->entityType)->buildContent($this, $view_mode, $langcode, $content);
  }

  public function defaultUri() {
    return array(
      'path' => 'salsa/' . str_replace('salsa_', '', $this->entityType) . '/' . $this->{$this->idKey},
      'options' => array(),
    );
  }

  /**
   * Defines the entity label if the 'entity_class_label' callback is used.
   */
  protected function defaultLabel() {
    // Add in the translated specified label property.
    if (!empty($this->entityInfo['entity keys']['label'])) {
      return $this->{$this->entityInfo['entity keys']['label']};
    }
  }

  /**
   * Getter for the related tags of a salsa entity.
   */
  public function getTags() {
    // @todo unfortunately using getLeftJoin doesn't work for some reason yet.
    // $result = salsa_api()->getLeftJoin(array('tag', 'tag_data', 'database_table'), array('table_name' =>  str_replace('salsa_', '', $this->entityType), 'table_key' =>  $this->{$this->idKey}), NULL, array('tag_KEY', 'tag'));

    // If object was not saved yet return.
    if (empty($this->{$this->idKey})) {
      return array();
    }

    // Load related tag objects.
    $entities = entity_load('salsa_database_table', FALSE, array('table_name' => str_replace('salsa_', '', $this->entityType)));
    if (empty($entities)) {
      return array();
    }
    $database_table = reset($entities);
    $tag_data = entity_load('salsa_tag_data', FALSE, array(
      'database_table_KEY' => $database_table->database_table_KEY,
      'table_key' => $this->{$this->idKey},
    ));

    $return = array();
    // Get the tag keys.
    foreach ($tag_data as $tag_data_key => $tag_data) {
      $tag_keys[] = $tag_data->tag_KEY;
    }
    if (!empty($tag_keys)) {
      // Load the tag labels.
      $tags = entity_load('salsa_tag', $tag_keys);
      foreach ($tags as $delta => $tag) {
        $return[$tag->tag_KEY] = $tag->tag;
      }
    }

    return $return;
  }
}

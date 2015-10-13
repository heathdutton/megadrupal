<?php

/**
 * @file
 * Redhen Campaign type classes.
 */


/**
 * The class used for model type entities
 */
class RedhenCampaignPageType extends Entity {

  // @codingStandardsIgnoreStart
  public
    $name,
    $label,
    $locked,
    $data,
    $page_type;
  // @codingStandardsIgnoreEnd

  /**
   * Override the default __construct method.
   */
  public function __construct($values = array()) {
    parent::__construct($values, 'redhen_campaign_page_type');
  }

  /**
   * Exports the entity.
   *
   * @see entity_export()
   */
  public function export($prefix = '') {
    if (!empty($this->group)) {
      $this->data = array(
        'group' => $this->group,
        'group_settings' => $this->group_settings,
      );
      unset($this->group, $this->group_settings);
    }
    return entity_get_controller($this->entityType)->export($this, $prefix);
  }

  /**
   * Override parent save method to ensure we have a goal field for each type.
   */
  public function save() {
    $ret = parent::save();

    // Associate goal field instance with campaign.
    if ($ret) {
      // Create goal field if it doesn't exist.
      redhen_campaign_create_fields();

      // Now create an instance of the goal field for this campaign type.
      redhen_campaign_create_goal_instance($this->name, 'redhen_campaign_page');
    }
  }

  /**
   * Permanently deletes the entity.
   *
   * @param bool $delete_entities
   *   Indicates if all campaigns of this type should be deleted as well.
   *
   * @see entity_delete()
   */
  public function delete($delete_entities = FALSE) {
    $id = $this->identifier();
    if (isset($id)) {
      entity_get_controller($this->entityType)->delete(array($id), NULL, $delete_entities);
    }
  }
}

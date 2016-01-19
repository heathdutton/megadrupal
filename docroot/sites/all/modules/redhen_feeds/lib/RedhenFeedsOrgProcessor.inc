<?php
/**
 * @file
 * Class definition of RedhenFeedsOrgProcessor.
 */

/**
 * Creates organizations from feed items.
 */
class RedhenFeedsOrgProcessor extends FeedsProcessor {
  /**
   * Define entity type.
   */
  public function entityType() {
    return 'redhen_org';
  }

  /**
   * Implements parent::entityInfo().
   */
  protected function entityInfo() {
    $info = parent::entityInfo();
    $info['label plural'] = t('RedHen Organizations');
    return $info;
  }

  /**
   * Creates a new organization in memory and returns it.
   */
  protected function newEntity(FeedsSource $source) {
    $values = array(
      'type' => $this->bundle(),
      'author_uid' => $this->config['author'],
    );
    return redhen_org_create($values);
  }

  /**
   * Loads an existing organization.
   */
  protected function entityLoad(FeedsSource $source, $id) {
    return redhen_org_load($id);
  }

  /**
   * Save a organization.
   */
  public function entitySave($entity) {
    return redhen_org_save($entity);
  }

  /**
   * Delete a series of organizations.
   */
  protected function entityDeleteMultiple($ids) {
    redhen_org_delete_multiple($ids);
  }

  /**
   * Override parent::configDefaults().
   */
  public function configDefaults() {
    $return = array(
      'author' => 0,
    ) + parent::configDefaults();

    return $return;
  }

  /**
   * Override parent:configForm() to add custom config FAPI elements.
   */
  public function configForm(&$form_state) {
    $form = parent::configForm($form_state);
    unset($form['input_format']);

    $author = user_load($this->config['author']);
    $form['author'] = array(
      '#type' => 'textfield',
      '#title' => t('Author'),
      '#description' => t('Select the author of the organizations to be created - leave empty to assign "anonymous".'),
      '#autocomplete_path' => 'user/autocomplete',
      '#default_value' => empty($author->name) ?  'anonymous' : check_plain($author->name),
    );

    return $form;
  }

  /**
   * Reschedule if expiry time changes.
   */
  public function configFormSubmit(&$values) {
    if ($author = user_load_by_name($values['author'])) {
      $values['author'] = $author->uid;
    }
    else {
      $values['author'] = 0;
    }
    parent::configFormSubmit($values);
  }

  /**
   * Return available mapping targets.
   */
  public function getMappingTargets() {
    $targets['organization_id'] = array(
      'name' => t('organization ID'),
      'description' => t('The id of the organization. NOTE: use this feature with care, organization ids are usually assigned by RedHen.'),
      'optional_unique' => TRUE,
    );
    $targets['label'] = array(
      'name' => t('Name'),
      'description' => t('The name of the organization.'),
    );
    $targets['author_uid'] = array(
      'name' => t('User UID'),
      'description' => t('The Drupal user ID of the organization author.'),
    );

    self::loadMappers();
    $entity_type = $this->entityType();
    $bundle = $this->bundle();
    drupal_alter('feeds_processor_targets', $targets, $entity_type, $bundle);

    return $targets;
  }

  /**
   * Override setTargetElement to operate on a target item.
   */
  public function setTargetElement(FeedsSource $source, $target_organization, $target_element, $value) {
    $wrapper = entity_metadata_wrapper('redhen_org', $target_organization);
    $wrapper->$target_element->set($value);
  }

  /**
   * Get org_id of an existing feed item node if available.
   */
  protected function existingEntityId(FeedsSource $source, FeedsParserResult $result) {
    if ($id = parent::existingEntityId($source, $result)) {
      return $id;
    }

    // Iterate through all unique targets and test whether they do already
    // exist in the database.
    foreach ($this->uniqueTargets($source, $result) as $target => $value) {
      switch ($target) {

        // @TODO - Check to see if organization is unique based upon it's label property.

        case 'organization_id':
          return $value;
      }
    }

    return 0;
  }
}

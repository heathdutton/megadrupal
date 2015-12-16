<?php
/**
 * @file
 * Class definition of RedhenFeedsContactProcessor.
 */

/**
 * Creates contacts from feed items.
 */
class RedhenFeedsContactProcessor extends FeedsProcessor {
  /**
   * Define entity type.
   */
  public function entityType() {
    return 'redhen_contact';
  }

  /**
   * Implements parent::entityInfo().
   */
  protected function entityInfo() {
    $info = parent::entityInfo();
    $info['label plural'] = t('RedHen Contacts');
    return $info;
  }

  /**
   * Creates a new contact in memory and returns it.
   */
  protected function newEntity(FeedsSource $source) {
    $values = array(
      'type' => $this->bundle(),
      'author_uid' => $this->config['author'],
    );
    return redhen_contact_create($values);
  }

  /**
   * Loads an existing contact.
   */
  protected function entityLoad(FeedsSource $source, $id) {
    return redhen_contact_load($id);
  }

  /**
   * Save a contact.
   */
  public function entitySave($entity) {
    return redhen_contact_save($entity);
  }

  /**
   * Delete a series of contacts.
   */
  protected function entityDeleteMultiple($ids) {
    redhen_contact_delete_multiple($ids);
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
      '#description' => t('Select the author of the contacts to be created - leave empty to assign "anonymous".'),
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
    $targets['contact_id'] = array(
      'name' => t('Contact ID'),
      'description' => t('The id of the contact. NOTE: use this feature with care, contact ids are usually assigned by RedHen.'),
      'optional_unique' => TRUE,
    );
    $targets['first_name'] = array(
      'name' => t('First name'),
      'description' => t('The first name of the contact.'),
    );
    $targets['middle_name'] = array(
      'name' => t('Middle name'),
      'description' => t('The middle name of the contact.'),
    );
    $targets['last_name'] = array(
      'name' => t('Last name'),
      'description' => t('The last name of the contact.'),
    );
    $targets['email'] = array(
      'name' => t('Email'),
      'description' => t('The email of the contact.'),
      'optional_unique' => TRUE,
    );
    $targets['redhen_state'] = array(
      'name' => t('State'),
      'description' => t('Indicates if the contact is active or inactive. Must be boolean value.'),
    );
    $targets['author_uid'] = array(
      'name' => t('User UID'),
      'description' => t('The Drupal user ID of the contact author.'),
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
  public function setTargetElement(FeedsSource $source, $target_contact, $target_element, $value) {
    $wrapper = entity_metadata_wrapper('redhen_contact', $target_contact);
    $wrapper->$target_element->set($value);
  }

  /**
   * Get contact_id of an existing feed item node if available.
   */
  protected function existingEntityId(FeedsSource $source, FeedsParserResult $result) {
    if ($id = parent::existingEntityId($source, $result)) {
      return $id;
    }

    // Iterate through all unique targets and test whether they do already
    // exist in the database.
    foreach ($this->uniqueTargets($source, $result) as $target => $value) {
      switch ($target) {
        case 'email':
          $contact = redhen_contact_load_by_mail($value);
          if ($contact) {
            reset($contact)->contact_id;
          }
          break;

        case 'contact_id':
          return $value;
      }
    }

    return 0;
  }
}

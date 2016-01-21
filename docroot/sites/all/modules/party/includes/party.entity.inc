<?php

/**
 * @file
 * Contains the controller classes for Party entities.
 */

/**
 * The API controller class for the Party entity.
 */
class PartyController extends EntityAPIController {

  /**
   * Flag to indicate base table properties have been updated.
   * @var int
   */
  const UPDATE_PROPERTY = 0x01;

  /**
   * Flag to indicate fields have been updated requiring field attach invokes.
   * @var int
   */
  const UPDATE_FIELDS = 0x02;

  /**
   * Flag to indicate other things been updated which may require a full save.
   * @var int
   */
  const UPDATE_OTHER = 0x04;

  /**
   * Overrides EntityAPIController::invoke().
   *
   * Add in our additional logic for party labels. This is in here rather than
   * PartyController::save() so that we can get the label set as early as
   * possible. The tricky case is when a plugin requires the party to be saved
   * or we fall back to pid which also requires the party to be saved. For
   * those cases, we need to update the label immediately after we write the
   * record but before we invoke modules' implementation of
   * hook_entity_insert(). However, in the more common use case, we ideally
   * want the label to be set before hook_entity_presave() is invoked.
   */
  public function invoke($hook, $party) {
    // In presave, trigger our label and primary fields update without a save.
    if ($hook == 'presave' && empty($party->primary_fields_updated)) {
      $this->setPrimaryFields($party, FALSE);
    }
    // If we are post save, clear our primary fields update flag.
    elseif (in_array($hook, array('insert', 'update')) && !empty($party->primary_fields_updated)) {
      unset($party->primary_fields_updated);
    }

    // In insert our primary fields ran before there was a pid, so we re-run
    // them in case we were dependant on a pid.
    if ($hook == 'insert') {
      // Set primary fields, storing any properties but deferring other stores.
      $needs_store = $this->setPrimaryFields($party, FALSE);
      if ($needs_store & PartyController::UPDATE_PROPERTY) {
        drupal_write_record($this->entityInfo['base table'], $party, $this->idKey);
      }
    }

    // Trigger the rest of the invoke action.
    parent::invoke($hook, $party);

    if ($hook == 'insert' && $needs_store & PartyController::UPDATE_OTHER) {
      unset($party->is_new);
      $party->save();
    }
  }

  /**
   * Set the primary fields for the party.
   *
   * @param $party
   *   A party object.
   * @param bool|int $store
   *   Whether we should store any changes. TRUE means yes, FALSE means no. An
   *   integer comprised of the update flags (see return) can be used to specify
   *   whether to run specific stores.
   *
   * @return int
   *   Whether changes requiring a store were made. This is a flag which can
   *   contain:
   *   - PartyController::UPDATE_PROPERTY
   *   - PartyController::UPDATE_FIELDS
   *   - PartyController::UPDATE_OTHER
   */
  public function setPrimaryFields($party, $store = TRUE) {
    $needs_store = 0;

    // Convert a TRUE into all our flags.
    if ($store === TRUE) {
      $store = PartyController::UPDATE_PROPERTY |
        PartyController::UPDATE_FIELDS |
        PartyController::UPDATE_OTHER;
    }

    // Find all primary fields and get their values.
    $party_wrapper = entity_metadata_wrapper('party', $party);
    foreach(PartyPrimaryFields::getFields() as $target => $sources) {
      // Find the first one with a valid value.
      foreach ($sources as $source) {
        // Get hold of our source wrapper.
        $wrapper = NULL;
        // The party data set is a special case.
        if ($source['data_set'] == 'party') {
          $wrapper = $party_wrapper;
        }
        // Otherwise get hold of the data set entity wrapper.
        else {
          $controller = $party->getDataSetController($source['data_set']);
          if ($entity = $controller->getEntity()) {
            $wrapper = entity_metadata_wrapper($controller->getDataInfo('entity type'), $entity);
          }
        }

        // If we have a wrapper, get hold of the value.
        if ($wrapper && isset($wrapper->{$source['property']})) {
          // Get the original value.
          $original_value = $party_wrapper->{$target}->value();

          // Copy the value.
          if (!empty($source['value'])) {
            $new_value = $wrapper->{$source['property']}->value() ? $wrapper->{$source['property']}->{$source['value']}->value(): NULL;
          }
          else {
            $new_value = $wrapper->{$source['property']}->value();
          }

          // Pass on for any callbacks.
          $new_value = PartyPrimaryFields::executeCallback($new_value, $source, $target);

          if ($new_value !== NULL) {
            if ($new_value !== $original_value) {
              $party_wrapper->{$target} = $new_value;

              // Check what kind of save we need.
              $target_info = $party_wrapper->getPropertyInfo($target);
              if (!empty($target_info['field'])) {
                $needs_store = $needs_store | PartyController::UPDATE_FIELDS;
              }
              elseif (!empty($target_info['schema field'])) {
                $needs_store = $needs_store | PartyController::UPDATE_PROPERTY;
              }
              else {
                $needs_store = $needs_store | PartyController::UPDATE_OTHER;
              }
            }

            // We don't need to check anything else.
            break;
          }
        }
      }
    }

    // Allow modules to respond in case they need to do anything.
    drupal_alter('party_primary_fields', $party, $needs_store);

    // Check whether we need to store the changes.
    if ($store && $needs_store) {
      // If we need to do a full save, set a flag to prevent recursion and run.
      if ($store & $needs_store & PartyController::UPDATE_OTHER) {
        $party->primary_fields_updated = TRUE;
        $party->save();
      }
      // Otherwise we will deal with anything individually.
      else {
        // Load up the original party.
        $party->original = entity_load_unchanged('party', $party->pid);

        // If we need to do a write record, do that.
        if ($store & $needs_store & PartyController::UPDATE_PROPERTY) {
          drupal_write_record($this->entityInfo['base table'], $party, $this->idKey);
        }

        // If we need to invoke a field update, do that.
        if ($store & $needs_store & PartyController::UPDATE_FIELDS) {
          $this->invoke('update', $party);
        }
        // Otherwise invoke hook_entity_update() directly.
        else {
          module_invoke_all('entity_update', $party, 'party');
        }
      }
    }
    // Although no primary fields were changed, Search API Indexes can pull
    // information from attached entities, so any time this is called, we may
    // need to update the indexed item.
    elseif ($store && module_exists('search_api')) {
      search_api_track_item_change('party', array($party->pid));
    }

    return $needs_store;
  }

  /**
   * Overriding the buildContent function to add entity specific fields.
   */
  public function buildContent($entity, $view_mode = 'full', $langcode = NULL, $content = array()) {
    $content = parent::buildContent($entity, $view_mode, $langcode, $content);
    return $content;
  }

  /**
   * Get the data set controller for a given entity and data set.
   *
   * @param Party $party
   *  The party to get the data set for.
   * @param $data_set_name
   *  The name of a data set.
   *
   * @return
   *  The controller class, without attached entities loaded yet.
   */
  public function getDataSetController($entity, $data_set_name) {
    // If we don't already have our data set, load it.
    if (!isset($entity->data_set_controllers[$data_set_name])) {
      $sets = party_get_data_set_info();

      if (empty($sets[$data_set_name])) {
        throw new Exception(t("Party data set '@data_set_name' does not exist.", array(
          '@data_set_name' => $data_set_name,
        )));
      }
      else if (!($class = $sets[$data_set_name]['class']) || !class_exists($class)) {
        throw new Exception(t("Party data controller '@class' for data set '@data_set_name' does not exist.", array(
          '@class' => $class,
          '@data_set_name' => $data_set_name,
        )));
      }
      $entity->data_set_controllers[$data_set_name] = new $class($data_set_name, $entity);
    }

    return $entity->data_set_controllers[$data_set_name];
  }
}

/**
 * The class used for party entities.
 */
class Party extends Entity {

  /**
   * The internal, numeric party-id.
   *
   * @var integer
   */
  public $pid;

  /**
   * The human-readable label for the party.
   *
   * @var string
   *
   * @see PartyController::setLabel()
   */
  public $label;

  /**
   * Boolean indicating whether this party has been archived.
   *
   * Archiving parties provides a way to ignore parties from certain features
   * or restrict access. An example of when you would archive a party is when
   * the parties is deceased (in the case of a person) or no longer operates
   * (in the case of an organisation).
   *
   * @var boolean
   */
  public $archived;

  /**
   * Boolean indicating whether this party has been hidden.
   *
   * Hiding a party provides a way to remove a party from the system without
   * deleting and losing the data. An example of when you would hide a party
   * is on merging, where you would hide the party that is no longer used. This
   * is then completely hidden from all normal system processes, but it still
   * exists and can be loaded, for example to find out what party it has been
   * merged with.
   *
   * @var boolean
   */
  public $hidden;

  /**
   * If $merged is TRUE, this is the pid of the party that this party has been
   * merged into.
   *
   * @var integer
   */
  public $merged_party;

  /**
   * The primary email for this party.
   *
   * @var string
   */
  public $email;

  /**
   * A cache of data set controllers.
   *
   * @var array
   */
  public $data_set_controllers = array();

  public function __construct($values = array()) {
    parent::__construct($values, 'party');
  }

  /**
   * Return the default label for this party object.
   *
   * @return string
   *   Human readable label for the party object.
   */
  protected function defaultLabel() {
    return $this->label;
  }

  /**
   * Return the default path for this party.
   *
   * @return array
   *   An array with the following key:
   *   - path - the drupal path for the party.
   */
  protected function defaultUri() {
    return array('path' => 'party/' . $this->pid);
  }

  /**
   * Get a data set controller.
   */
  public function getDataSetController($data_set_name) {
    return entity_get_controller('party')->getDataSetController($this, $data_set_name);
  }
}

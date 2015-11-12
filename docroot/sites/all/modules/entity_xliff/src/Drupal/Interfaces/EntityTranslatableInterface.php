<?php

/**
 * @file
 * Defines a superset of EggsCereal\Interfaces\TranslatableInterface which is
 * specific to Drupal entities.
 */

namespace EntityXliff\Drupal\Interfaces;

use EggsCereal\Interfaces\TranslatableInterface;

interface EntityTranslatableInterface extends TranslatableInterface {

  /**
   * Returns the language of the wrapped entity, in effect representing the
   * "source" language for any XLIFF generated or being processed.
   *
   * @return string
   */
  public function getSourceLanguage();

  /**
   * Returns the target entity used by TranslatableInterface::setData() to set
   * and save translated data for this entity.
   *
   * Note that this should make as best an effort as possible to return the same
   * entity wrapper instance no matter how many times it is called. To do so,
   * it's recommended that you statically cache the target entity wrapper before
   * returning it.
   *
   * @param string $targetLanguage
   *   The target language of the desired entity wrapper.
   *
   * @return \EntityDrupalWrapper
   *   Returns an Entity wrapper representing the target entity.
   */
  public function getTargetEntity($targetLanguage);

  /**
   * Returns an array of fields for this entity that are translatable.
   *
   * @return array
   *   An array of entity property names that represent translatable fields. The
   *   fields should be accessible on the entity via a metadata wrapper call
   *   like: $this->entity->{$field}->value().
   */
  public function getTranslatableFields();

  /**
   * Returns whether or not the wrapped entity is translatable.
   *
   * @return bool
   *   TRUE if the wrapped entity is translatable. FALSE otherwise.
   */
  public function isTranslatable();

  /**
   * Takes the wrapped entity from a non-translated state to a ready-to-be
   * translated state (or does nothing if it is already in the latter state).
   *
   * In most cases, this will update the wrapped entity's language from language
   * neutral (und) to a specific language and perform any entity saves that it
   * needs.
   */
  public function initializeTranslation();

  /**
   * Saves a given Entity wrapper. This is the final step in saving translated
   * data on a given Entity; you may wish to override this method in your
   * custom entity translatable implementation for special needs.
   *
   * @param \EntityDrupalWrapper $wrapper
   *   The Entity wrapper to be saved.
   *
   * @param string $targetLanguage
   *   The language in which this entity is intended to be saved.
   *
   * @see EntityTranslatableBase::setData()
   */
  public function saveWrapper(\EntityDrupalWrapper $wrapper, $targetLanguage);

}

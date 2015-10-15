<?php

/**
 * @file
 * Contains \Drupal\plug_config\Plugin\Config\ConfigInterface.
 */

namespace Drupal\plug_config\Plugin\Config;

interface ConfigInterface {

  /**
   * Retrieves the config entity database schema.
   *
   * @return array
   *   The config entity schema.
   */
  public static function schema();

  /**
   * Checks if there is access to the entity.
   *
   * @param string $op
   *   The operation to perform on the entity.
   * @param string $entity_type
   *   The type of the entity being accessed.
   * @param object $entity
   *   The entity checking access to. Defaults to NULL.
   * @param object $account
   *   The user account. Defaults to NULL.
   *
   * @return bool
   *   Boolean indicating if $account is granted to perform $op.
   */
  public static function accessCallback($op, $entity_type, $entity = NULL, $account = NULL);

  /**
   * Generates the entity creation/edition form.
   *
   * @param array $form
   *   The basic form array.
   * @param array $form_state
   *   The form state array.
   * @param string $op
   *   The operation to perform on the entity. Can be 'edit', 'add' or 'clone'.
   *
   * @return array
   *   Form structure to define a new entity.
   */
  public function form(array $form, array &$form_state, $op = 'edit');

  /**
   * Submit callback for the entity form.
   *
   * @param array $form
   *   The basic form array.
   * @param array $form_state
   *   The form state array.
   */
  public function formSubmit(array $form, array &$form_state);

}

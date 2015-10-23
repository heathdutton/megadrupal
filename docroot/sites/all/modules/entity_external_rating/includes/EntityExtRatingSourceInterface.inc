<?php

/**
 * @file
 *  Contains the interface for an External Ratign Source Plugin.
 */

interface EntityExtRatingSourceInterface {
  /**
   * Defines the configuration form for the plugin. It returns an array just
   * like the one that is used by the Drupal form API. See the example in
   * TwitterRating or FacebookRating class.
   */
  public function configForm(&$formState);

  /**
   * Validation handler for the configuration form.
   */
  public function configFormValidate(&$values);

  /**
   * Submit handler for the configuration form.
   */
  public function configFormSubmit(&$values);

  /**
   * Saves the plugin configuration settings.
   */
  public function save();

  /**
   * Loads the plugin configuration settings.
   */
  public function load();

  /**
   * Returns the configuration of the plugin. This configuration is the one that
   * is submited by the configFormSubmit function.
   */
  public function getConfig();

  /**
   * Sets the internal configuration. Usually this is the one that is submitted
   * by the configFormSubmit function.
   */
  public function setConfig($config);

  /**
   * Returns the name of the source plugin. This should be a unique string that
   * identifies the plugin.
   */
  public function sourceName();

  /**
   * Saves a rating in the database.
   */
  public function saveRating($rating_id, $entity_id, $entity_type, $rating);

  /**
   * Calls the external rating source API to get (and then save) the rating.
   */
  public function saveExternalRating($params);

  /**
   * Connects to the external rating source API to get the rating.
   */
  public function getExternalRating(&$params);

  /**
   * Returns a html output that usually allows the user to rate the entity.
   */
  public function getHtml($params);
}

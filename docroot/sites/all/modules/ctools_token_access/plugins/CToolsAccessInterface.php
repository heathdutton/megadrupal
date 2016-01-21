<?php

/**
 * @file
 * Contains CToolsAccessInterface.
 */

interface CToolsAccessInterface {

  /**
   * Decides if the request is granted.
   *
   * @return bool
   *   TRUE for access granted and FALSE for access denied.
   */
  public function access();

  /**
   * Gets the settings form for the plugin.
   *
   * @return array
   *   Form API array.
   */
  public function settingsForm();

  /**
   * Gets the summary explaining the plugin settings.
   *
   * @return string
   *   Summary string.
   */
  public function settingsSummary();
}
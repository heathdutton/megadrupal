<?php

/**
 * @file
 * Contains AbstractSearchService.
 */

namespace Drupal\fac\SearchService;

/**
 * Abstract class with generic implementation of some service methods.
 *
 * For creating your own service class extending this class, you only need to
 * implement search() from the SearchServiceInterface interface.
 */
abstract class AbstractSearchService implements SearchServiceInterface {
  /**
   * Implements FacServiceInterface::__construct().
   */
  public function __construct() {
  }

  /**
   * Implements SearchApiServiceInterface::configurationForm().
   *
   * Returns an empty form by default.
   */
  public function configurationForm(array $form, array &$form_state) {
    return array();
  }

  /**
   * Implements FacServiceInterface::configurationFormValidate().
   *
   * Does nothing by default.
   */
  public function configurationFormValidate(array $form, array &$values, array &$form_state) {
  }

  /**
   * Implements FacServiceInterface::configurationFormSubmit().
   *
   * Does nothing by default.
   */
  public function configurationFormSubmit(array $form, array &$values, array &$form_state) {
  }

  /**
   * Implements FacServiceInterface::search().
   *
   * Does nothing by default.
   */
  public function search($key, $language) {
    return array();
  }

}

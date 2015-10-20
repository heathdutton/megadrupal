<?php

/**
 * @file
 * Contains SesarchServiceInterface.
 */

namespace Drupal\fac\SearchService;

/**
 * Interface defining the methods backend search services have to implement.
 *
 * Before a service object is used, the corresponding server's data will be read
 * from the database (see FACbstractService for a list of fields).
 */
interface SearchServiceInterface {
  /**
   * Constructs a service object.
   */
  public function __construct();

  /**
   * Form constructor for the service configuration form.
   *
   *
   * @param array $form
   *   The service options part of the form.
   * @param array $form_state
   *   The current form state.
   *
   * @return array
   *   A form array for setting service-specific options.
   */
  public function configurationForm(array $form, array &$form_state);

  /**
   * Validation callback for the form returned by configurationForm().
   *
   * Use form_error() to flag errors on form elements.
   *
   * @param array $form
   *   The form returned by configurationForm().
   * @param array $values
   *   The part of the $form_state['values'] array corresponding to this form.
   * @param array $form_state
   *   The complete form state.
   */
  public function configurationFormValidate(array $form, array &$values, array &$form_state);

  /**
   * Submit callback for the form returned by configurationForm().
   *
   * This method should set the options of this service according to
   * $values.
   *
   * @param array $form
   *   The form returned by configurationForm().
   * @param array $values
   *   The part of the $form_state['values'] array corresponding to this form.
   * @param array $form_state
   *   The complete form state.
   */
  public function configurationFormSubmit(array $form, array &$values, array &$form_state);

  /**
   * Performs a search call.
   *
   * @param  string $key
   *   The key to use in the search query.
   * @param  string $language
   *   The language to use in the search query.
   */
  public function search($key, $language);

}

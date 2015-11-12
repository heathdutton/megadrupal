<?php

/**
 * @file
 * Contains BasicTitleSearchService.
 */

namespace Drupal\fac\SearchService;

/**
 * Fast Autocomplete backend service class providing basic title search on
 * nodes.
 */
class BasicTitleSearchService extends AbstractSearchService {
  /**
   * Implements SearchServiceInterface::search().
   *
   * param string $key
   *   The key to use in the search.
   * param string $language
   *   The language to use in the search.
   *
   *  return array
   *    The results array containing the resulting nids.
   */
  public function search($key, $language) {
    $result = array();
    $settings = variable_get('fac_backend_service_settings', array());

    $query = db_select('node', 'n')
      ->fields('n', array(
        'nid',
      ))
      ->condition('n.title', '%' . db_like($key) . '%', 'LIKE')
      ->condition('n.status', 1, '=')
      ->condition('n.language', array(
        LANGUAGE_NONE,
        $language,
      ), 'IN')
      ->orderBy('n.title')
      ->range(0, isset($settings['number_of_results']) ? $settings['number_of_results'] : 5)
      ->addTag('node_access');

    // Allow other modules to modify the query.
    drupal_alter('fac_basic_title_search_query', $query);

    $nids = $query->execute()->fetchCol();

    foreach ($nids as $nid) {
      $result[] = array(
        'entity_type' => 'node',
        'etid' => $nid,
      );
    }

    return $result;
  }

  /**
   * Implements SearchServiceInterface::configurationForm().
   */
  public function configurationForm(array $form, array &$form_state) {
    $form = array();

    $settings = variable_get('fac_backend_service_settings', array());

    $form['number_of_results'] = array(
      '#type' => 'textfield',
      '#title' => t('Number of results'),
      '#required' => TRUE,
      '#size' => 2,
      '#default_value' => isset($settings['number_of_results']) ? $settings['number_of_results'] : 5,
      '#description' => t('The maximum number of results the service returns.')
    );

    $form['submit'] = array(
      '#type' => 'submit',
      '#value' => t('Save'),
    );

    return $form;
  }

  /**
   * Implements SearchServiceInterface::configurationFormValidate().
   */
  public function configurationFormValidate(array $form, array &$values, array &$form_state) {
    if ($values['number_of_results'] < 1) {
      form_set_error('number_of_results', t('Please enter a maximum number of results of 1 or more.'));
    }
  }

  /**
   * Implements SearchServiceInterface::configurationFormSubmit().
   */
  public function configurationFormSubmit(array $form, array &$values, array &$form_state) {
    $settings = array(
      'number_of_results' => $values['number_of_results'],
    );

    variable_set('fac_backend_service_settings', $settings);
  }

}

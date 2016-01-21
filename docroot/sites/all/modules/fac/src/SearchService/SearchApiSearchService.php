<?php

/**
 * @file
 * Contains SearchAPISearchService.
 */

namespace Drupal\fac\SearchService;

/**
 * Fast Autocomplete backend service class providing Search API search.
 */
class SearchApiSearchService extends AbstractSearchService {
  /**
   * Implements SearchServiceInterface::search().
   *
   * @param string $key
   *   The key to use in the search.
   * @param string $language
   *   The language to use in the search.
   *
   * @return array
   *   The results array containing the resulting nids.
   */
  public function search($key, $language) {
    $result = array();
    $settings = variable_get('fac_backend_service_settings', array());

    // Load the index and get the limit on results.
    $index = search_api_index_load($settings['index_id']);
    $limit = isset($settings['number_of_results']) ? $settings['number_of_results'] : 5;

    // Build the Search API query. Use the 'direct' parse mode to be able to
    // search for partial words.
    $query = new \SearchApiQuery($index, array(
      'limit' => $limit,
      'conjunction' => isset($settings['conjunction']) ? $settings['conjunction'] : 'OR',
    ));
    // Add the keys preceded and followed by '*' to search for partial words and
    // sort descending by relevance.
    $query->keys($key)
      ->sort('search_api_relevance', 'DESC');

    $fields = isset($settings['full_text_fields']) ? $settings['full_text_fields'] : '';
    if (!empty($fields)) {
      $query->fields($fields);
    }

    // Allow other modules to modify the query.
    drupal_alter('fac_search_api_search_query', $query);

    $search_results = $query->execute();
    $etids = array_keys($search_results['results']);

    // Build up the result array.
    foreach ($etids as $etid) {
      $result['items'][] = array(
        'entity_type' => $index->item_type,
        'etid' => $etid,
      );
    }

    return $result;
  }

  /**
   * Implements SearchServiceInterface::configurationForm().
   */
  public function configurationForm(array $form, array &$form_state) {
    $form = array();

    $search_api_indexes = search_api_index_load_multiple(FALSE, array(
      'enabled' => 1,
    ));

    if (!empty($search_api_indexes)) {
      $settings = variable_get('fac_backend_service_settings', array());

      $indexes = array();
      foreach ($search_api_indexes as $machine_name => $sai) {
        $indexes[$machine_name] = check_plain($sai->name);
      }

      $form['index_id'] = array(
        '#type' => 'select',
        '#title' => t('Search API index'),
        '#options' => $indexes,
        '#required' => TRUE,
        '#default_value' => isset($settings['index_id']) ? $settings['index_id'] : '',
        '#description' => t('Select the Search API index to use.'),
      );

      // If an index is selected and it is an index on a Solr index using the
      // Search API Solr backend, then most of the Search API processors need to
      // be disabled to enable partial word searching using the direct parse
      // mode. Show a table with the processor statusses compatiblity.
      if (isset($settings['index_id'])) {
        $index = search_api_index_load($settings['index_id']);
        $server = search_api_server_load($index->server);
        if ($server->class == 'search_api_solr_service') {
          $processors = search_api_get_processors();
          $processors_disabled = array(
            'search_api_case_ignore',
            'search_api_tokenizer',
            'search_api_stopwords',
            'search_api_highlighting',
          );
          $rows = array();

          // Go through all the processors and check if they are enabled or not
          // and give an indication about the compatibility in a table.
          foreach ($index->options['processors'] as $id => $processor) {
            if (in_array($id, $processors_disabled)) {
              $rows[] = array(
                'data' => array(
                  $processors[$id]['name'],
                  $processor['status'] ? t('Enabled') : t('Disabled'),
                  $processor['status'] ? t('Not OK') . '</br/>' . t('Please disable this processor in the !settings_link.', array(
                    '!settings_link' => l(t('Search API index filter settings'), 'admin/config/search/search_api/index/' . $index->machine_name . '/workflow', array(
                      'fragment' => 'edit-processors',
                    )),
                  )) : t('OK'),
                ),
                'class' => array(
                  $processor['status'] ? 'error' : 'ok',
                ),
              );
            }
            else {
              $rows[] = array(
                'data' => array(
                  $processors[$id]['name'],
                  $processor['status'] ? t('Enabled') : t('Disabled'),
                  t('OK'),
                ),
                'class' => array(
                  'ok',
                ),
              );
            }
          }

          $form['processors_info'] = array(
            '#prefix' => '<p>' . t('The table below shows the Search API processors compatibility with the Fast Autocomplete functionality for the selected Search API index. Most Search API processors need to be disabled to enable partial word searching and Apache Solr already does what these processors do by default.') . '</p>',
            '#markup' => theme('table', array(
              'header' => array(
                t('Processor name'),
                t('Status'),
                t('Compatibility'),
              ),
              'rows' => $rows,
              'attributes' => array(
                'class' => array(
                  'system-status-report',
                ),
              ),
            )),
          );

          // Add a form field to configure in which full textfields to search
          // for matches.
          $ft_field_options = array();
          $indexed_ft_fields = $index->getFullTextFields();
          foreach ($indexed_ft_fields as $ft_field) {
            $ft_field_options[$ft_field] = $ft_field;
          }
          $form['full_text_fields'] = array(
            '#type' => 'select',
            '#title' => t('Full text fields to search'),
            '#description' => t('The full text fields to search. If you do not select any fields, the Fast Autocomplete search is searching for matches on all full text fields.'),
            '#multiple' => TRUE,
            '#default_value' => isset($settings['full_text_fields']) ? $settings['full_text_fields'] : '',
            '#options' => $ft_field_options,
          );
        }

        $form['conjunction'] = array(
          '#type' => 'select',
          '#title' => t('Select the conjunction to use for the search terms'),
          '#options' => array(
            'OR' => 'OR',
            'AND' => 'AND',
          ),
          '#required' => TRUE,
          '#default_value' => isset($settings['conjunction']) ? $settings['conjunction'] : 'OR',
          '#description' => t('The conjunction of the search terms determines if any one of the search terms need to be in the result or that all search terms need to be in the result (AND).'),
        );

        // For each bundle of the entity type check if the bundle can be
        // selected based on the configuration of the bundle filter that might
        // be enabled on the configured index.
        $bundle_options = array();
        $bundles = field_info_bundles($index->item_type);
        foreach ($bundles as $id => $bundle) {
          $add_bundle_option = FALSE;
          if (!$index->options['data_alter_callbacks']['search_api_alter_bundle_filter']['status']) {
            $add_bundle_option = TRUE;
          }
          else {
            if ($index->options['data_alter_callbacks']['search_api_alter_bundle_filter']['settings']['default']) {
              if (empty($index->options['data_alter_callbacks']['search_api_alter_bundle_filter']['settings']['bundles']) || !in_array($id, $index->options['data_alter_callbacks']['search_api_alter_bundle_filter']['settings']['bundles'])) {
                $add_bundle_option = TRUE;
              }
            }
            else {
              if (in_array($id, $index->options['data_alter_callbacks']['search_api_alter_bundle_filter']['settings']['bundles'])) {
                $add_bundle_option = TRUE;
              }
            }
          }

          if ($add_bundle_option) {
            $bundle_options[$id] = check_plain($bundle['label']);
          }
        }
        $form['bundles'] = array(
          '#type' => 'select',
          '#title' => t('Bundle filter'),
          '#multiple' => TRUE,
          '#default_value' => isset($settings['bundles']) ? $settings['bundles'] : '',
          '#options' => $bundle_options,
          '#description' => t('If you do not select any bundles, the Fast Autocomplete search is not filtered by bundle. Are there bundles missing from the list? Maybe there is a bundle filter enabled on the index in the !settings_link.', array(
            '!settings_link' => l(t('Search API index filter settings'), 'admin/config/search/search_api/index/' . $index->machine_name . '/workflow', array(
              'fragment' => 'edit-callbacks',
            )),
          )),
        );
      }

      $form['number_of_results'] = array(
        '#type' => 'textfield',
        '#title' => t('Number of results'),
        '#required' => TRUE,
        '#size' => 2,
        '#default_value' => isset($settings['number_of_results']) ? $settings['number_of_results'] : 5,
        '#description' => t('The maximum number of results the service returns.'),
      );

      $form['submit'] = array(
        '#type' => 'submit',
        '#value' => t('Save'),
      );
    }
    else {
      drupal_set_message(t('You have not created or enabled any Search API indexes yet!'), 'warning');
    }

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
      'index_id' => $values['index_id'],
      'number_of_results' => $values['number_of_results'],
      'conjunction' => $values['conjunction'],
      'bundles' => $values['bundles'],
    );

    if (isset($values['full_text_fields'])) {
      $settings['full_text_fields'] = $values['full_text_fields'];
    }

    variable_set('fac_backend_service_settings', $settings);
  }

}

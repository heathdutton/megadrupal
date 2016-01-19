<?php
/**
 * @file
 * Taxonomy Related Entities bean plugin.
 */

class TaxRelatedBean extends BeanPlugin {

  /**
   * Declares default block settings.
   */
  public function values() {
    return array(
      'view_mode' => 'default',
      'filters' => array(
        'vocabulary' => FALSE,
        'records_shown' => 5,
        'offset_results' => 0,
      ),
      'settings' => array(
        'related' => 'page',
        'entity_type' => 'node',
        'bundle_types' => FALSE,
        'entity_view_mode' => FALSE,
        'hide_empty' => FALSE,
        'unmatch_add' => FALSE,
        'cache_duration' => '5',
        'cache_auth_user' => FALSE,
        'cache_anon_user' => TRUE,
      ),
      'more_link' => array(
        'text' => '',
        'path' => '',
      ),
    );
  }

  /**
   * Builds extra settings for the block edit form.
   */
  public function form($bean, $form, &$form_state) {

    // Here we are defining the form.
    $form = array();

    // Settings fieldset is used for configuring the 'related' bean output.
    $form['settings'] = array(
      '#type' => 'fieldset',
      '#tree' => TRUE,
      '#title' => t('Output'),
      '#prefix' => '<div id="output-wrapper">',
      '#suffix' => '</div>',
    );

    // Instantiate entity info.
    $entity_info = entity_get_info();

    // Create a list of entity types that have view modes.
    $entity_types = array();
    foreach ($entity_info as $key => $value) {
      if (!empty($value['view modes'])) {
        $entity_types[$key] = $value['label'];
      }
    }

    // Set a cache duration for the block.
    $form['settings']['cache_duration'] = array(
      '#type' => 'textfield',
      '#title' => t('Cache duration (in minutes)'),
      '#size' => 5,
      '#default_value' => $bean->settings['cache_duration'],
      '#required' => TRUE,
    );

    // User select how entites are related.
    $form['settings']['related'] = array(
      '#type' => 'select',
      '#title' => t('Related To'),
      '#options' => array(
        'page' => 'Active Page',
        'user' => 'Logged-in User',
      ),
      '#default_value' => $bean->settings['related'],
      '#required' => TRUE,
      '#multiple' => FALSE,
    );

    // Determine if auth-user caching is appropriate.
    $form['settings']['cache_auth_user'] = array(
      '#type' => 'checkbox',
      '#title' => t('Cache this block for authenticated users.'),
      '#description' => t('Warning: This will create a cache record for every authenticated user that views this block.'),
      '#default_value' => $bean->settings['cache_auth_user'],
      '#states' => array(
        'visible' => array(
          ':input[name="settings[related]"]' => array('value' => 'user'),
        ),
      ),
    );
    
    // Determine if anon-user caching is appropriate.
    $form['settings']['cache_anon_user'] = array(
      '#type' => 'checkbox',
      '#title' => t('Cache this block for anonymous users.'),
      '#description' => t('This will create a cache record for anonymous users that view this block.'),
      '#default_value' => $bean->settings['cache_anon_user'],
      '#states' => array(
        'visible' => array(
          ':input[name="settings[related]"]' => array('value' => 'user'),
        ),
      ),
    );

    // User select which entity type to use for output.
    $form['settings']['entity_type'] = array(
      '#type' => 'select',
      '#title' => t('Entity Type'),
      '#options' => $entity_types,
      '#default_value' => $bean->settings['entity_type'],
      '#required' => TRUE,
      '#multiple' => FALSE,
      '#ajax' => array(
        'callback' => 'bean_tax_entity_type_callback',
        'wrapper' => 'output-wrapper',
        'method' => 'replace',
      ),
    );

    // Check for an ajax update and use new entity_type setting.
    if (!isset($form_state['values']['settings']['entity_type'])) {
      $entity_type = $bean->settings['entity_type'];
    }
    else {
      $entity_type = $form_state['values']['settings']['entity_type'];
    }

    // User select which view mode to use for the results inside the bean.
    $form['settings']['entity_view_mode'] = array(
      '#type' => 'select',
      '#title' => t('Entity View Mode'),
      '#options' => bean_tax_get_entity_view_modes($entity_info, $entity_type),
      '#default_value' => $bean->settings['entity_view_mode'],
      '#required' => TRUE,
      '#multiple' => FALSE,
    );

    // Determine what entity bundle types to display.
    $form['settings']['bundle_types'] = array(
      '#type' => 'select',
      '#title' => t('Entity Bundles'),
      '#options' => bean_tax_get_entity_bundles($entity_type),
      '#default_value' => $bean->settings['bundle_types'],
      '#required' => TRUE,
      '#multiple' => TRUE,
      '#size' => 5,
    );

    $form['settings']['hide_empty'] = array(
      '#type' => 'checkbox',
      '#title' => t('Do not display block if there are no results.'),
      '#default_value' => $bean->settings['hide_empty'],
    );
    
    $form['settings']['unmatch_add'] = array(
      '#type' => 'checkbox',
      '#title' => t('Append unrelated entities so there are more results.'),
      '#default_value' => $bean->settings['unmatch_add'],
    );
 
    // Select objects for the vocabularies that will be used.
    $vocabulary = taxonomy_get_vocabularies();
    $select_vocabulary_array = array();
    foreach ($vocabulary as $item) {
      $select_vocabulary_array[$item->machine_name] = $item->name;
    }

    // Define the filters fieldset.
    $form['filters'] = array(
      '#type' => 'fieldset',
      '#tree' => TRUE,
      '#title' => t('Filters'),
    );

    // User define a maximum number of entities to be shown.
    $form['filters']['records_shown'] = array(
      '#type' => 'textfield',
      '#title' => t('Records shown'),
      '#size' => 5,
      '#default_value' => $bean->filters['records_shown'],
      '#element_validate' => array('bean_tax_setting_is_numeric'),
    );
   
    // User define a maximum number of entities to be shown.
    $form['filters']['offset_results'] = array(
      '#type' => 'textfield',
      '#title' => t('Offset Results'),
      '#size' => 5,
      '#default_value' => $bean->filters['offset_results'],
      '#element_validate' => array('bean_tax_setting_is_numeric'),
    );

    // Determine related taxonomy term vocabularies.
    $form['filters']['vocabulary'] = array(
      '#type' => 'select',
      '#title' => t('Vocabularies'),
      '#options' => $select_vocabulary_array,
      '#default_value' => $bean->filters['vocabulary'],
      '#required' => TRUE,
      '#multiple' => TRUE,
      '#size' => 5,
    );

    // Define a "read more" style link to be shown at the bottom of the bean.
    $form['more_link'] = array(
      '#type' => 'fieldset',
      '#tree' => TRUE,
      '#title' => t('More link'),
    );

    // Link text shown on the 'more link' to be defined by user.
    $form['more_link']['text'] = array(
      '#type' => 'textfield',
      '#title' => t('Link text'),
      '#default_value' => $bean->more_link['text'],
    );

    // Actual URL path for the 'more link' defined by user.
    $form['more_link']['path'] = array(
      '#type' => 'textfield',
      '#title' => t('Link path'),
      '#default_value' => $bean->more_link['path'],
    );
    return $form;
  }

  /**
   * Define an array of all taxonomy terms in the defined vocabularies.
   */
  private function getPossibleTerms($bean) {
    $possible_tid = array();
    foreach ($bean->filters['vocabulary'] as $vm) {
      $query = new EntityFieldQuery();
      $result = $query->entityCondition('entity_type', 'taxonomy_vocabulary')->propertyCondition('machine_name', $vm)->execute();
      foreach ($result['taxonomy_vocabulary'] as $vocabulary) {
        $vid = $vocabulary->vid;
      }
      $tree = taxonomy_get_tree($vid);
      foreach ($tree as $term) {
        $possible_tid[$term->tid] = $term->tid;
      }
    }
    return $possible_tid;
  }

  /**
   * Compare possible terms to the actual terms assigned to an entity.
   */
  private function getValidTerms($terms, $possible_tid, &$valid_tid) {
    foreach ($terms as $term) {
      if (isset($possible_tid[$term->tid])) {
        $valid_tid[$term->tid] = $term->tid;
      }
    }
  }

  /**
   * Entity field query for entities of the defined bundle.
   */
  private function getAggregate($bean) {
    $type = $bean->settings['entity_type'];
    foreach ($bean->settings['bundle_types'] as $bundle) {
      $query = new EntityFieldQuery();
      $query->entityCondition('entity_type', $type);
      $query->entityCondition('bundle', $bundle);
      $query->propertyOrderBy('created', 'DESC');
      if ($type == 'node') {
        $query->propertyCondition('status', 1);
      }
      // Additional conditions for node based translations.
      global $language;
      if ($language->language != NULL && $type == 'node') {
        $query->propertyCondition('language', $language->language);
        $query->propertyCondition('tnid', 0, "<>");
      }
      $results[$bundle] = $query->execute();

      // For nodes using field based translation.
      if ($language->language != NULL && $type == 'node') {
        $query = new EntityFieldQuery();
        $query->entityCondition('entity_type', $type);
        $query->entityCondition('bundle', $bundle);
        $query->propertyOrderBy('created', 'DESC');
        $query->propertyCondition('tnid', 0);
        $query->propertyCondition('status', 1);
        $field_translated = $query->execute();

        // Reassign the result array or merge arrays if necessary
        if (empty($results[$bundle][$type]) && !empty($field_translated)) {
          $results[$bundle][$type] = $field_translated[$type];
        }
        elseif(!empty($results[$bundle][$type]) && !empty($field_translated[$type])) {
          $combined = $results[$bundle][$type] + $field_translated[$type];
          ksort($combined);
          $results[$bundle][$type] = $combined;
        }
      }
    }
    // Store the results in an aggregated array of entities.
    $aggregate = array();
    if (isset($results[$bundle][$bean->settings['entity_type']])) {
      foreach ($results[$bundle][$bean->settings['entity_type']] as $id => $result) {
        $aggregate[$bean->settings['entity_type']][$id] = $result;
      }
    }
    return $aggregate;
  }

  /**
   * Create a taxonomy related "score" for each result's matching terms.
   */
  private function scoreResults($bean, $aggregate, $valid_tid) {
    $result = array();
    if (isset($aggregate[$bean->settings['entity_type']])) {
      foreach ($aggregate[$bean->settings['entity_type']] as $key => $value) {
        $entity_terms = bean_tax_get_entity_terms($bean->settings['entity_type'], $key, $value->vid);
        $score = 0;
        // The actual scoring to determine valid taxonomy term matching.
        foreach ($entity_terms as $term) {
          if (isset($valid_tid[$term->tid])) {
            $score++;
          }
        }
        $item['id'] = $key;
        $item['score'] = $score;
        // A score of 1 or greater adds to the array of matching entities.
        if ($score != 0) {
          $result[] = $item;
        }
        elseif ($score == 0 && $bean->settings['unmatch_add'])  {
          $result[] = $item;
        }
      }
    }
    // Calculate an overall score.
    $all = 0;
    foreach ($result as $item) {
      $all = ($item['score'] + $all);
    }
    // If overall score is not zero, sort based on score.
    if ($all != 0) {
      // Invoke comparison function to determine highest ranked results.
      usort($result, "bean_tax_cmp");
    }
    return $this->removeActive($result, $bean->settings['entity_type']);
  }

  /**
   * Remove active page from results.
   */
  private function removeActive($result, $type) {
    foreach ($result as $key => $entity) {
      $active_page = bean_tax_active_entity_array('page');
      if (isset($active_page['ids']) && $active_page['ids'][0] == $entity['id'] && $active_page['type'] == $type) {
        unset($result[$key]);
      }
    }
    return $result;
  }

  /**
   * Return the related entities as rendered HTML markup.
   */
  private function returnMarkup($bean, $result) {
    // Start counting results at index of 0.
    $i = 0;
    // Set and index for actual results shown.
    $shown = 0;
    // Set markup array as empty.
    $markup = '';
    // Load and render the related entities.
    foreach ($result as $entity) {
      if (isset($entity['id']) && $shown < $bean->filters['records_shown'] && $i >= $bean->filters['offset_results']) {
        $entity = entity_load_single($bean->settings['entity_type'], $entity['id']);
        $entity_view = entity_view($bean->settings['entity_type'], array($entity), $bean->settings['entity_view_mode']);
        $markup .= drupal_render($entity_view);
        $shown++;
      }
      // Count continues along...
      $i++;
    }
    return $markup;
  }

  /**
   * Displays the bean.
   */
  public function view($bean, $content, $view_mode = 'default', $langcode = NULL) {
    // Return the active entity information.
    $active_entity = bean_tax_active_entity_array($bean->settings['related']);
    
    // Create a unique id to be used by the cache.
    if (isset($active_entity['type']) && !empty($active_entity['ids'])) {
      // Determine user role and append to cache.
      if ($active_entity['type'] != 'user') {
        // Grab the highest rid attached to the user.
        global $user;
        $key = max(array_keys($user->roles));
        // Use active entity type, entity id and max role to determine cache id.
        $cid = $active_entity['type'] . ':' . $active_entity['ids'][0] . ':' . $key;
      }
      else {
        // Use active entity type and entity id to determine cache id.
        $cid = $active_entity['type'] . ':' . $active_entity['ids'][0];
      }
    }
    else {
      // Create a generic cache id for use otherwise.
      $cid = date('Y:m:d:i');
    }
    // Append language prefix to end of cache id.
    global $language;
    if ($language->prefix != '') {
      $cid = $cid . ':' . $language->prefix;
    }
    // Set the cache name.
    $cache_name = 'bean_tax:related:' . $bean->delta . ':' . $cid;
    
    // Check for cached content.
    if ($cache = cache_get($cache_name)) {
      $content = $cache->data;
    }
    else {
      // We need to make sure that the bean is configured correctly.
      if (!empty($active_entity) && !empty($bean->filters['vocabulary']) && !empty($bean->settings['bundle_types'])) {

        // Determine a list of possible terms based on the set vocabulary.
        $possible_tid = $this->getPossibleTerms($bean);

        // Return a list of valid term ids based on the terms attached to the
        // active entity object.
        $valid_tid = array();
        if (isset($active_entity['terms'])) {
          $this->getValidTerms($active_entity['terms'], $possible_tid, $valid_tid);
        }

        // Use EFQ to return all possible related entites.
        $aggregate = $this->getAggregate($bean);

        // Score and sort any valid results.
        $result = $this->scoreResults($bean, $aggregate, $valid_tid);

        // Related entities initially set to none.
        if (empty($result)) {
          // Hide block when result is empty and 'hide_empty' option is checked.
          if (isset($bean->settings['hide_empty']) || !$active_entity['object']) return;
          // There are no related nodes. Set Empty array for theme output.
          $content['#markup'] = t('No Results');
        }
        // Return something when viewing the bean page callback.
        elseif (isset($active_entity['type']) && $active_entity['type'] == 'bean' && $bean->bid === $active_entity['object']->bid) {
          $content['#markup'] = '';
        }
        else {
          // If all else fails, we really must have something to show people.
          $content['#markup'] = $this->returnMarkup($bean, $result);

          // Cache the bean where appropriate.
          if (isset($bean->settings['cache_duration']) && isset($bean->settings['cache_auth_user']) && isset($bean->settings['cache_anon_user'])) {
            $cache_bean = TRUE;
            // Check if authenticated user caching is turned off.
            if ($bean->settings['related'] == 'user' && !$bean->settings['cache_auth_user']) {
              $cache_bean = FALSE;
            }
            // Anonymous user check.
            $anon = user_is_anonymous();
            // Check if anonymous user caching is turned off.
            if ($bean->settings['related'] == 'user' && !$bean->settings['cache_anon_user'] && $anon == TRUE) {
              $cache_bean = FALSE;
            }
            // Check if anonymous user caching is turned on.
            if ($bean->settings['related'] == 'user' && $bean->settings['cache_anon_user'] && $anon == TRUE) {
              $cache_bean = TRUE;
            }
            // Finally, set the cache after all checks pass.
            if ($cache_bean) {
              cache_set($cache_name, $content, 'cache', time() + 60 * $bean->settings['cache_duration']);
            }
          }
        }
      }

      // Render the optional "more link" if provided.
      if (!empty($bean->more_link['text']) && !empty($bean->more_link['path'])) {
        $content['#markup'] .= theme('bean_tax_more_link', array('text' => $bean->more_link['text'], 'path' => $bean->more_link['path']));
      }
    }
    return $content;
  }
}

<?php
/**
 * @file
 * Taxonomy Term List bean plugin.
 */

class TaxListingBean extends BeanPlugin {

  /**
   * Declares default block settings.
   */
  public function values() {
    return array(
      'view_mode' => 'default',
      'filters' => array(
        'vocabulary' => FALSE,
      ),
      'settings' => array(
        'term_view_mode' => 'full',
        'records_shown' => 5,
        'hide_empty' => FALSE,
        'cache_duration' => '5',
      ),
    );
  }

  /**
   * Builds extra settings for the block edit form.
   */
  public function form($bean, $form, &$form_state) {

    // Here we are defining the form.
    $form = array();

    // Select objects for the vocabularies that will be used.
    $vocabulary = taxonomy_get_vocabularies();
    $select_vocabulary_array = array();
    foreach ($vocabulary as $item) {
      $select_vocabulary_array[$item->machine_name] = $item->name;
    }

    // Building the basic form for individual 'related' bean configuration.
    $form['filters'] = array(
      '#type' => 'fieldset',
      '#tree' => TRUE,
      '#title' => t('Filters'),
    );

    // Determine related taxonomy term vocabularies.
    $form['filters']['vocabulary'] = array(
      '#type' => 'select',
      '#title' => t('Vocabulary'),
      '#options' => $select_vocabulary_array,
      '#default_value' => $bean->filters['vocabulary'],
      '#required' => TRUE,
      '#multiple' => TRUE,
      '#size' => 5,
    );

    // Settings fieldset is used for configuring the 'listing' bean output.
    $form['settings'] = array(
      '#type' => 'fieldset',
      '#tree' => TRUE,
      '#title' => t('Output'),
    );

    // Select objects for the taxonomy term view modes that will be used.
    $term_view_modes = array();
    $entity_info = entity_get_info();
    foreach($entity_info['taxonomy_term']['view modes'] as $key => $value) {
      $term_view_modes[$key] = $value['label'];
    }

    // User select which view mode to use for the term listing inside the bean.
    $form['settings']['term_view_mode'] = array(
      '#type' => 'select',
      '#title' => t('Taxonomy Term View Mode'),
      '#options' => $term_view_modes,
      '#default_value' => $bean->settings['term_view_mode'],
      '#required' => TRUE,
      '#multiple' => FALSE,
    );

    // User define a maximum number of terms to be shown.
    $form['settings']['records_shown'] = array(
      '#type' => 'textfield',
      '#title' => t('Records shown'),
      '#size' => 5,
      '#default_value' => $bean->settings['records_shown'],
      '#element_validate' => array('bean_tax_setting_is_numeric'),
    );

    $form['settings']['hide_empty'] = array(
      '#type' => 'checkbox',
      '#title' => t('Do not display block if there are no results.'),
      '#default_value' => $bean->settings['hide_empty'],
    );

    // Set a cache duration for the block.
    $form['settings']['cache_duration'] = array(
      '#type' => 'textfield',
      '#title' => t('Cache duration (in minutes)'),
      '#size' => 5,
      '#default_value' => $bean->settings['cache_duration'],
      '#required' => TRUE,
    );

    return $form;
  }

  /**
   * Displays the bean.
   */
  public function view($bean, $content, $view_mode = 'default', $langcode = NULL) {
    // Retrieve the terms from the loaded entity.
    $active_entity = bean_tax_active_entity_array();
    // Check for cached content on this block.
    $cache_name = 'bean_tax:listing:' . $bean->delta . ':' . $active_entity['type']  . ':' . $active_entity['ids'][0];
    if ($cache = cache_get($cache_name)) {
      $content = $cache->data;
    }
    else {
      // We need to make sure that the bean is configured correctly.
      if ($active_entity['type'] != 'bean' && !empty($bean->filters['vocabulary']) && (isset($active_entity['terms']) && count($active_entity['terms']))) {
        // Reformat vocabulary list from machine names to vocabulary vids.
        $vids = array();
        foreach ($bean->filters['vocabulary'] as $vm) {
          $query = new EntityFieldQuery();
          $result = $query->entityCondition('entity_type', 'taxonomy_vocabulary');
          $query->propertyCondition('machine_name', $vm);
          global $language;
          if ($language->language != NULL && db_field_exists('taxonomy_vocabulary', 'language')) {
            $query->propertyCondition('language', $language->language);
          }
          $result = $query->execute();
          foreach ($result['taxonomy_vocabulary'] as $vocabulary) {
            $vids[$vocabulary->vid] = $vocabulary->vid;
          }
        }

        $i = 0;
        $content['terms'] = array();
        // Parse terms from correct vocabularies, limit list to X results.
        foreach ($active_entity['terms'] as $term) {
          $term = entity_load_single('taxonomy_term', $term->tid);
          if (in_array($term->vid, $vids) && $i < $bean->settings['records_shown']) {
            $content['terms'][$term->tid] = entity_view('taxonomy_term', array($term->tid => $term), $bean->settings['term_view_mode']);
            $i++;
          }
        }
        cache_set($cache_name, $content, 'cache', time() + 60 * $bean->settings['cache_duration']);
      }
      // Return something when viewing the bean page callback.
      elseif (isset($active_entity['type']) && $active_entity['type'] == 'bean' && $bean->bid === $active_entity['object']->bid) {
        $content['#markup'] = '';
      }
      elseif ($bean->settings['hide_empty'] || !$active_entity['object']) {
        return;
      }
      else {
        $content['#markup'] = t('No terms.');
      }
    }
    return $content;
  }
}


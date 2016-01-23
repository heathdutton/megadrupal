<?php

/**
 * @file
 * Administration pages provided by entity_bundle_redirect module.
 *
 * @author Francisco J. Cruz Romanos <grisendo@gmail.com>
 */

/**
 * Menu callback for admin/config/search/entity-bundle-redirect.
 */
function entity_bundle_redirect_admin_page() {

  $form = array();

  $entities = entity_get_info();

  $special_tokens = array(
    'taxonomy_term' => 'term',
  );

  $lang_und = new stdClass();
  $lang_und->language = LANGUAGE_NONE;
  $lang_und->name = 'All languages';
  $lang_und->enabled = 1;

  $languages = array(
    LANGUAGE_NONE => $lang_und,
  );
  $languages += language_list();

  $token_exists = module_exists('token');

  if (!$token_exists) {
    drupal_set_message(
      t(
        'Install and enable !url in order to take advantage of advanced path configuration options.',
        array(
          '!url' => l(
            t('Token module'),
            'http://drupal.org/project/token',
            array(
              'attributes' => array(
                'target' => '_blank',
              ),
            )
          ),
        )
      ),
      'warning'
    );
  }

  // Used to avoid ventral review warnings.
  $t = get_t();

  foreach ($entities as $entity_id => $entity) {
    if (isset($entity['uri callback'])) {
      $form[$entity_id] = array(
        '#type' => 'fieldset',
        '#title' => $t($entity['label']),
        '#collapsible' => TRUE,
        '#collapsed' => TRUE,
        '#tree' => TRUE,
      );
      $token_type = isset($special_tokens[$entity_id]) ? $special_tokens[$entity_id] : $entity_id;
      foreach ($entity['bundles'] as $type => $bundle) {
        $form[$entity_id][$type] = array(
          '#type' => 'fieldset',
          '#title' => $bundle['label'],
          '#parents' => array($entity_id, $type),
        );
        foreach ($languages as $lang_key => $language) {
          $stored_val = entity_bundle_redirect_load_stored($entity_id, $type, $lang_key);
          $form[$entity_id][$type][$lang_key] = array(
            '#type' => 'textfield',
            '#title' => t(
              '%bundle (%language)',
              array(
                '%bundle' => $t($bundle['label']),
                '%language' => $t($language->name),
              )
            ),
            '#default_value' => $stored_val ? $stored_val : '',
            '#parents' => array($entity_id, $type, $lang_key),
            '#description' => t('Redirection URL, empty for no redirection. It accepts tokens.'),
            '#token_types' => array($token_type),
            '#element_validate' => array('token_element_validate'),
            '#after_build' => array('token_element_validate'),
          );
          if (!$token_exists) {
            $form[$entity_id][$type][$lang_key]['#description'] = t('Redirection URL, empty for no redirection.');
            unset($form[$entity_id][$type][$lang_key]['#token_types']);
            unset($form[$entity_id][$type][$lang_key]['#element_validate']);
            unset($form[$entity_id][$type][$lang_key]['#after_build']);
          }
        }
      }
      if ($token_exists) {
        $form[$entity_id]['token_help'] = array(
          '#title' => t('Replacement patterns'),
          '#type' => 'fieldset',
          '#collapsible' => TRUE,
          '#collapsed' => TRUE,
        );
        $form[$entity_id]['token_help']['help'] = array(
          '#theme' => 'token_tree',
          '#token_types' => array($token_type),
        );
      }
    }
  }

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save'),
  );

  $form['#submit'][] = 'entity_bundle_redirect_admin_page_submit';

  return $form;

}

/**
 * Submit handler for the entity_bundle_redirect_admin_page() form.
 */
function entity_bundle_redirect_admin_page_submit($form, &$form_state) {

  foreach ($form_state['values'] as $entity_type => $bundles) {
    if (is_array($bundles)) {
      foreach ($bundles as $bundle => $langs) {
        if (is_array($langs)) {
          foreach ($langs as $lang_key => $value) {
            entity_bundle_redirect_save_url($entity_type, $bundle, trim($value), $lang_key);
          }
        }
      }
    }
  }
  cache_clear_all('*', 'cache_entity_bundle_redirect', TRUE);
  drupal_set_message(t('Entity Bundle redirections saved.'));

}

<?php

/**
 * @file
 * For lingwo_entry with views
 */

/**
 * Implements hook_views_data().
 */
function lingwo_entry_views_data() {
  $language_defaults = array(
    'field' => array(
      'handler' => 'lingwo_entry_views_handler_field_language',
      'click sortable' => TRUE,
    ),
    'filter' => array(
      'handler' => 'lingwo_entry_views_handler_filter_language',
    ),
    'argument' => array(
      'handler' => 'lingwo_entry_views_handler_argument_language',
    ),
  );

  return array(
    'lingwo_entry_info' => array(
      'table' => array(
        'group' => t('Dictionary'),
        'join' => array(
          'node' => array(
            'left_field' => 'nid',
            'field' => 'nid',
          ),
        ),
      ),
      'nid' => array(
        'title' => t('Node'),
        'help' => t('The actual node that this entry refers to.'),
        'relationship' => array(
          'base' => 'node',
          'base field' => 'nid',
          'handler' => 'views_handler_relationship',
          'label' => t('entry node'),
        ),
      ),
      'pos' => array(
        'title' => t('Part of speech'),
        'help' => t('The part of speech if this is a dictionary entry'),
        'field' => array(
          'handler' => 'lingwo_entry_views_handler_field_pos',
          'click sortable' => TRUE,
        ),
        // TODO: This should be exposed as a select with the available POS options -- except
        // how do we deal with multiple languages and how they have different POS values?
        'filter' => array(
          'handler' => 'views_handler_filter_string',
        ),
        'argument' => array(
          'handler' => 'views_handler_argument_string',
        ),
      ),
      'language' => array(
        'title' => t('Language'),
        'help' => t('The language or language pair of this entry'),
        'field' => array(
          'handler' => 'lingwo_entry_views_handler_field_language_pair',
        ),
        // Use source_language and target_language if you want to filter!
        /*
        'filter' => array(
        'handler' => 'views_handler_filter_string',
        ),
        */
        'argument' => array(
          'handler' => 'lingwo_entry_views_handler_argument_language_pair',
        ),
      ),
      'source_language' => array(
        'title' => t('Source Language'),
        'help' => t('The source language of this entry.'),
      ) + $language_defaults,
      'target_language' => array(
        'title' => t('Target Language'),
        'help' => t('The target language of this entry.'),
      ) + $language_defaults,
      'translation' => array(
        'title' => t('Translation'),
        'help' => t('A boolean indicating if this is a translation or not'),
        'field' => array(
          'handler' => 'views_handler_field_boolean',
          'click sortable' => TRUE,
          'output formats' => array(
            'translation-source-entry' => array(t('Translation'), t('Source')),
          ),
        ),
        'filter' => array(
          'handler' => 'views_handler_filter_boolean_operator',
          'label' => t('Translation'),
          'type' => 'yes-no',
        ),
      ),
    ),
  );
}

/**
 * Implements hook_views_handlers().
 */
function lingwo_entry_views_handlers() {
  return array(
    'info' => array(
      'path' => drupal_get_path('module', 'lingwo_entry'),
    ),
    'handlers' => array(
      'lingwo_entry_views_handler_field_pos' => array(
        'parent' => 'views_handler_field',
      ),
      'lingwo_entry_views_handler_field_language_pair' => array(
        'parent' => 'views_handler_field',
      ),
      'lingwo_entry_views_handler_argument_language_pair' => array(
        'parent' => 'views_handler_argument',
      ),
      'lingwo_entry_views_handler_field_language' => array(
        'parent' => 'views_handler_field',
      ),
      'lingwo_entry_views_handler_argument_language' => array(
        'parent' => 'views_handler_argument',
      ),
      'lingwo_entry_views_handler_filter_language' => array(
        'parent' => 'views_handler_filter_in_operator',
      ),
    ),
  );
}


<?php

/**
 * @file
 * For integrating lingwo_senses with views.
 */

/**
 * Implements hook_views_data().
 */
function lingwo_senses_views_data() {
  $data = array();

  $data['lingwo_senses_revisions'] = array(
    'table' => array(
      'group' => t('Dictionary senses'),
    ),

    'delta' => array(
      'title' => t('Sense order'),
      'field' => array(
        'handler' => 'views_handler_field_numeric',
        'click sortable' => TRUE,
      ),
      'sort' => array(
        'handler' => 'views_handler_sort',
      ),
    ),

    /*
    'source_sense' => array(
      'title' => t('Sense source'),
      'help' => t('The source sense (untranslated).'),
      'relationship' => array(
        'handler' => 'lingwo_senses_views_handler_relationship_source',
        'label' => t('Source senses'),
      ),
    ),
    */

    'id' => array(
      'title' => t('Sense ID'),
      'field' => array(
        'handler' => 'lingwo_senses_views_handler_field_data',
        'data property' => 'id',
        'click sortable' => FALSE,
      ),
    ),

    'difference' => array(
      'title' => t('Sense difference or definition'),
      'field' => array(
        'handler' => 'lingwo_senses_views_handler_field_data',
        'data property' => 'data][difference',
        'click sortable' => FALSE,
      ),
    ),

    'example' => array(
      'title' => t('Sense example'),
      'field' => array(
        'handler' => 'lingwo_senses_views_handler_field_data',
        'data property' => 'data][example',
        'click sortable' => FALSE,
      ),
    ),

    'example_translation' => array(
      'title' => t('Sense example translation'),
      'field' => array(
        'handler' => 'lingwo_senses_views_handler_field_data',
        'data property' => 'data][example_translation',
        'click sortable' => FALSE,
      ),
    ),

    'translation_clue' => array(
      'title' => t('Sense translation clue'),
      'field' => array(
        'handler' => 'lingwo_senses_views_handler_field_data',
        'data property' => 'data][clue',
        'click sortable' => FALSE,
      ),
    ),

    'translation' => array(
      'title' => t('Sense translation'),
      'field' => array(
        'handler' => 'lingwo_senses_views_handler_field_data',
        'data property' => 'data][trans',
        'click sortable' => FALSE,
      ),
    ),

    'translation_no_equivalent' => array(
      'title' => t('Sense translation has no equivalent in this language'),
      'field' => array(
        'handler' => 'lingwo_senses_views_handler_field_data',
        'data property' => 'data][no_equivalent',
        'click sortable' => FALSE,
      ),
    ),

    'translation_same_as' => array(
      'title' => t('Sense translation same as'),
      'field' => array(
        'handler' => 'lingwo_senses_views_handler_field_data',
        'data property' => 'data][same_as',
        'click sortable' => FALSE,
      ),
    ),
  );

  return $data;
}

/**
 * Implements hook_views_data_alter().
 */
function lingwo_senses_views_data_alter(&$data) {
  $data['node']['lingwo_senses'] = array(
    'title' => t('Dictionary senses'),
    'group' => t('Dictionary'),
    'help' => t('The senses on each dictionary entry.'),
    'relationship' => array(
      'base' => 'lingwo_senses_revisions',
      'base field' => 'vid',
      'field' => 'vid',
      'handler' => 'views_handler_relationship',
      'label' => t('Senses'),
    ),
  );
}


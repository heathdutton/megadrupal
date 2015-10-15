<?php
/**
 * @file
 * codebook_core.strongarm.inc
 */

/**
 * Implements hook_strongarm().
 */
function codebook_core_strongarm() {
  $export = array();

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'auto_entitylabel_node_codebook_chapter';
  $strongarm->value = '1';
  $export['auto_entitylabel_node_codebook_chapter'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'auto_entitylabel_node_codebook_ordinance';
  $strongarm->value = '1';
  $export['auto_entitylabel_node_codebook_ordinance'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'auto_entitylabel_node_codebook_section';
  $strongarm->value = '1';
  $export['auto_entitylabel_node_codebook_section'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'auto_entitylabel_node_codebook_title';
  $strongarm->value = '1';
  $export['auto_entitylabel_node_codebook_title'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'auto_entitylabel_pattern_node_codebook_chapter';
  $strongarm->value = '[node:codebook_doc_num] – [node:field-codebook-doc-heading]';
  $export['auto_entitylabel_pattern_node_codebook_chapter'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'auto_entitylabel_pattern_node_codebook_ordinance';
  $strongarm->value = 'Ordinance No. [node:field-codebook-ord-num]';
  $export['auto_entitylabel_pattern_node_codebook_ordinance'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'auto_entitylabel_pattern_node_codebook_section';
  $strongarm->value = '[node:codebook_doc_num] – [node:field-codebook-doc-heading]';
  $export['auto_entitylabel_pattern_node_codebook_section'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'auto_entitylabel_pattern_node_codebook_title';
  $strongarm->value = 'Title [node:codebook_doc_num] - [node:field-codebook-doc-heading]';
  $export['auto_entitylabel_pattern_node_codebook_title'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'auto_entitylabel_php_node_codebook_chapter';
  $strongarm->value = 0;
  $export['auto_entitylabel_php_node_codebook_chapter'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'auto_entitylabel_php_node_codebook_ordinance';
  $strongarm->value = 0;
  $export['auto_entitylabel_php_node_codebook_ordinance'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'auto_entitylabel_php_node_codebook_section';
  $strongarm->value = 0;
  $export['auto_entitylabel_php_node_codebook_section'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'auto_entitylabel_php_node_codebook_title';
  $strongarm->value = 0;
  $export['auto_entitylabel_php_node_codebook_title'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'field_bundle_settings_field_collection_item__field_codebook_section_notes';
  $strongarm->value = array(
    'view_modes' => array(
      'full' => array(
        'custom_settings' => FALSE,
      ),
      'diff_standard' => array(
        'custom_settings' => FALSE,
      ),
      'token' => array(
        'custom_settings' => FALSE,
      ),
    ),
    'extra_fields' => array(
      'form' => array(),
      'display' => array(),
    ),
  );
  $export['field_bundle_settings_field_collection_item__field_codebook_section_notes'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'field_bundle_settings_field_collection_item__field_codebook_title_notes';
  $strongarm->value = array(
    'view_modes' => array(
      'full' => array(
        'custom_settings' => FALSE,
      ),
      'diff_standard' => array(
        'custom_settings' => FALSE,
      ),
      'token' => array(
        'custom_settings' => FALSE,
      ),
    ),
    'extra_fields' => array(
      'form' => array(),
      'display' => array(),
    ),
  );
  $export['field_bundle_settings_field_collection_item__field_codebook_title_notes'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'field_bundle_settings_node__codebook_chapter';
  $strongarm->value = array(
    'view_modes' => array(
      'teaser' => array(
        'custom_settings' => FALSE,
      ),
      'full' => array(
        'custom_settings' => FALSE,
      ),
      'rss' => array(
        'custom_settings' => FALSE,
      ),
      'token' => array(
        'custom_settings' => FALSE,
      ),
      'codebook_embed' => array(
        'custom_settings' => FALSE,
      ),
    ),
    'extra_fields' => array(
      'form' => array(
        'title' => array(
          'weight' => '3',
        ),
      ),
      'display' => array(
        'codebook_sub_docs_eva_sections' => array(
          'default' => array(
            'weight' => '0',
            'visible' => TRUE,
          ),
          'teaser' => array(
            'weight' => '10',
            'visible' => FALSE,
          ),
        ),
        'codebook_sub_docs_entity_view_1' => array(
          'default' => array(
            'weight' => '1',
            'visible' => TRUE,
          ),
        ),
        'codebook_sub_docs_eva_sections_summary' => array(
          'default' => array(
            'weight' => '0',
            'visible' => TRUE,
          ),
        ),
        'codebook_sub_docs_eva_sections_full' => array(
          'default' => array(
            'weight' => '1',
            'visible' => TRUE,
          ),
        ),
        'codebook_footnotes_eva_chapter_footnotes' => array(
          'default' => array(
            'weight' => '6',
            'visible' => FALSE,
          ),
        ),
        'codebook_sub_docs_pdf_eva_sections_full' => array(
          'default' => array(
            'weight' => 10,
            'visible' => FALSE,
          ),
        ),
        'codebook_sub_docs_pdf_eva_sections_summary' => array(
          'default' => array(
            'weight' => 10,
            'visible' => FALSE,
          ),
        ),
      ),
    ),
  );
  $export['field_bundle_settings_node__codebook_chapter'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'field_bundle_settings_node__codebook_ordinance';
  $strongarm->value = array(
    'view_modes' => array(
      'teaser' => array(
        'custom_settings' => TRUE,
      ),
      'full' => array(
        'custom_settings' => FALSE,
      ),
      'rss' => array(
        'custom_settings' => FALSE,
      ),
      'diff_standard' => array(
        'custom_settings' => FALSE,
      ),
      'token' => array(
        'custom_settings' => FALSE,
      ),
      'codebook_embed' => array(
        'custom_settings' => FALSE,
      ),
    ),
    'extra_fields' => array(
      'form' => array(
        'title' => array(
          'weight' => '0',
        ),
      ),
      'display' => array(),
    ),
  );
  $export['field_bundle_settings_node__codebook_ordinance'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'field_bundle_settings_node__codebook_resolution';
  $strongarm->value = array(
    'view_modes' => array(
      'teaser' => array(
        'custom_settings' => TRUE,
      ),
      'full' => array(
        'custom_settings' => FALSE,
      ),
      'rss' => array(
        'custom_settings' => FALSE,
      ),
      'diff_standard' => array(
        'custom_settings' => FALSE,
      ),
      'token' => array(
        'custom_settings' => FALSE,
      ),
    ),
    'extra_fields' => array(
      'form' => array(
        'title' => array(
          'weight' => '0',
        ),
      ),
      'display' => array(),
    ),
  );
  $export['field_bundle_settings_node__codebook_resolution'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'field_bundle_settings_node__codebook_section';
  $strongarm->value = array(
    'view_modes' => array(
      'teaser' => array(
        'custom_settings' => FALSE,
      ),
      'full' => array(
        'custom_settings' => FALSE,
      ),
      'rss' => array(
        'custom_settings' => FALSE,
      ),
      'token' => array(
        'custom_settings' => FALSE,
      ),
      'codebook_embed' => array(
        'custom_settings' => TRUE,
      ),
    ),
    'extra_fields' => array(
      'form' => array(
        'title' => array(
          'weight' => '3',
        ),
      ),
      'display' => array(),
    ),
  );
  $export['field_bundle_settings_node__codebook_section'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'field_bundle_settings_node__codebook_title';
  $strongarm->value = array(
    'view_modes' => array(
      'teaser' => array(
        'custom_settings' => FALSE,
      ),
      'full' => array(
        'custom_settings' => FALSE,
      ),
      'rss' => array(
        'custom_settings' => FALSE,
      ),
      'token' => array(
        'custom_settings' => FALSE,
      ),
      'codebook_embed' => array(
        'custom_settings' => FALSE,
      ),
    ),
    'extra_fields' => array(
      'form' => array(
        'title' => array(
          'weight' => '2',
        ),
      ),
      'display' => array(
        'codebook_sub_docs_eva_chapters' => array(
          'default' => array(
            'weight' => '0',
            'visible' => TRUE,
          ),
          'teaser' => array(
            'weight' => '10',
            'visible' => FALSE,
          ),
          'print' => array(
            'weight' => 3,
            'visible' => FALSE,
          ),
        ),
        'codebook_footnotes_eva_title_footnotes' => array(
          'default' => array(
            'weight' => '1',
            'visible' => TRUE,
          ),
          'print' => array(
            'weight' => 2,
            'visible' => TRUE,
          ),
        ),
        'codebook_sub_docs_pdf_entity_view_1' => array(
          'default' => array(
            'weight' => 10,
            'visible' => FALSE,
          ),
          'print' => array(
            'weight' => 1,
            'visible' => TRUE,
          ),
        ),
        'codebook_sub_docs_pdf_eva_chapters' => array(
          'default' => array(
            'weight' => 10,
            'visible' => FALSE,
          ),
          'print' => array(
            'weight' => 0,
            'visible' => TRUE,
          ),
        ),
      ),
    ),
  );
  $export['field_bundle_settings_node__codebook_title'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'node_options_codebook_chapter';
  $strongarm->value = array(
    0 => 'status',
  );
  $export['node_options_codebook_chapter'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'node_options_codebook_ordinance';
  $strongarm->value = array(
    0 => 'status',
  );
  $export['node_options_codebook_ordinance'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'node_options_codebook_resolution';
  $strongarm->value = array(
    0 => 'status',
  );
  $export['node_options_codebook_resolution'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'node_options_codebook_section';
  $strongarm->value = array(
    0 => 'status',
  );
  $export['node_options_codebook_section'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'node_options_codebook_title';
  $strongarm->value = array(
    0 => 'status',
  );
  $export['node_options_codebook_title'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'node_preview_codebook_chapter';
  $strongarm->value = '1';
  $export['node_preview_codebook_chapter'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'node_preview_codebook_ordinance';
  $strongarm->value = '1';
  $export['node_preview_codebook_ordinance'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'node_preview_codebook_resolution';
  $strongarm->value = '1';
  $export['node_preview_codebook_resolution'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'node_preview_codebook_section';
  $strongarm->value = '1';
  $export['node_preview_codebook_section'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'node_preview_codebook_title';
  $strongarm->value = '1';
  $export['node_preview_codebook_title'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'node_submitted_codebook_chapter';
  $strongarm->value = 0;
  $export['node_submitted_codebook_chapter'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'node_submitted_codebook_ordinance';
  $strongarm->value = 0;
  $export['node_submitted_codebook_ordinance'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'node_submitted_codebook_resolution';
  $strongarm->value = 0;
  $export['node_submitted_codebook_resolution'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'node_submitted_codebook_section';
  $strongarm->value = 0;
  $export['node_submitted_codebook_section'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'node_submitted_codebook_title';
  $strongarm->value = 0;
  $export['node_submitted_codebook_title'] = $strongarm;

  return $export;
}

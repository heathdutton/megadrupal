<?php
/**
 * @file
 * codebook_print_pdf.features.inc
 */

/**
 * Implements hook_ctools_plugin_api().
 */
function codebook_print_pdf_ctools_plugin_api($module = NULL, $api = NULL) {
  if ($module == "strongarm" && $api == "strongarm") {
    return array("version" => "1");
  }
}

/**
 * Implements hook_views_api().
 */
function codebook_print_pdf_views_api($module = NULL, $api = NULL) {
  return array("api" => "3.0");
}

/**
 * Implements hook_field_default_field_instances_alter().
 */
function codebook_print_pdf_field_default_field_instances_alter(&$data) {
  if (isset($data['node-codebook_chapter-field_codebook_doc_heading'])) {
    $data['node-codebook_chapter-field_codebook_doc_heading']['display']['codebook_embed_pdf'] = array(
      'label' => 'above',
      'settings' => array(),
      'type' => 'hidden',
      'weight' => 6,
    ); /* WAS: '' */
    $data['node-codebook_chapter-field_codebook_doc_heading']['display']['print'] = array(
      'label' => 'above',
      'settings' => array(),
      'type' => 'hidden',
      'weight' => 6,
    ); /* WAS: '' */
  }
  if (isset($data['node-codebook_chapter-field_codebook_doc_num'])) {
    $data['node-codebook_chapter-field_codebook_doc_num']['display']['codebook_embed_pdf'] = array(
      'label' => 'inline',
      'settings' => array(),
      'type' => 'hidden',
      'weight' => 5,
    ); /* WAS: '' */
    $data['node-codebook_chapter-field_codebook_doc_num']['display']['print'] = array(
      'label' => 'inline',
      'settings' => array(),
      'type' => 'hidden',
      'weight' => 5,
    ); /* WAS: '' */
  }
  if (isset($data['node-codebook_chapter-field_codebook_title'])) {
    $data['node-codebook_chapter-field_codebook_title']['display']['codebook_embed_pdf'] = array(
      'label' => 'hidden',
      'settings' => array(),
      'type' => 'hidden',
      'weight' => 4,
    ); /* WAS: '' */
    $data['node-codebook_chapter-field_codebook_title']['display']['print'] = array(
      'label' => 'hidden',
      'settings' => array(),
      'type' => 'hidden',
      'weight' => 4,
    ); /* WAS: '' */
  }
  if (isset($data['node-codebook_chapter-field_codebook_title_notes'])) {
    $data['node-codebook_chapter-field_codebook_title_notes']['display']['codebook_embed_pdf'] = array(
      'label' => 'above',
      'settings' => array(),
      'type' => 'hidden',
      'weight' => 7,
    ); /* WAS: '' */
    $data['node-codebook_chapter-field_codebook_title_notes']['display']['print'] = array(
      'label' => 'above',
      'settings' => array(),
      'type' => 'hidden',
      'weight' => 7,
    ); /* WAS: '' */
  }
  if (isset($data['node-codebook_section-body'])) {
    $data['node-codebook_section-body']['display']['codebook_embed_pdf'] = array(
      'label' => 'hidden',
      'module' => 'text',
      'settings' => array(),
      'type' => 'text_default',
      'weight' => 0,
    ); /* WAS: '' */
    $data['node-codebook_section-body']['display']['print'] = array(
      'label' => 'hidden',
      'module' => 'text',
      'settings' => array(),
      'type' => 'text_default',
      'weight' => 0,
    ); /* WAS: '' */
  }
  if (isset($data['node-codebook_section-field_codebook_chapter'])) {
    $data['node-codebook_section-field_codebook_chapter']['display']['codebook_embed_pdf'] = array(
      'label' => 'hidden',
      'settings' => array(),
      'type' => 'hidden',
      'weight' => 4,
    ); /* WAS: '' */
    $data['node-codebook_section-field_codebook_chapter']['display']['print'] = array(
      'label' => 'hidden',
      'settings' => array(),
      'type' => 'hidden',
      'weight' => 4,
    ); /* WAS: '' */
  }
  if (isset($data['node-codebook_section-field_codebook_doc_heading'])) {
    $data['node-codebook_section-field_codebook_doc_heading']['display']['codebook_embed_pdf'] = array(
      'label' => 'above',
      'settings' => array(),
      'type' => 'hidden',
      'weight' => 6,
    ); /* WAS: '' */
    $data['node-codebook_section-field_codebook_doc_heading']['display']['print'] = array(
      'label' => 'above',
      'settings' => array(),
      'type' => 'hidden',
      'weight' => 6,
    ); /* WAS: '' */
  }
  if (isset($data['node-codebook_section-field_codebook_doc_num'])) {
    $data['node-codebook_section-field_codebook_doc_num']['display']['codebook_embed_pdf'] = array(
      'label' => 'above',
      'settings' => array(),
      'type' => 'hidden',
      'weight' => 5,
    ); /* WAS: '' */
    $data['node-codebook_section-field_codebook_doc_num']['display']['print'] = array(
      'label' => 'above',
      'settings' => array(),
      'type' => 'hidden',
      'weight' => 5,
    ); /* WAS: '' */
  }
  if (isset($data['node-codebook_section-field_codebook_exhibits'])) {
    $data['node-codebook_section-field_codebook_exhibits']['display']['codebook_embed_pdf'] = array(
      'label' => 'above',
      'module' => 'file',
      'settings' => array(),
      'type' => 'file_default',
      'weight' => 2,
    ); /* WAS: '' */
    $data['node-codebook_section-field_codebook_exhibits']['display']['print'] = array(
      'label' => 'above',
      'module' => 'file',
      'settings' => array(),
      'type' => 'file_default',
      'weight' => 2,
    ); /* WAS: '' */
  }
  if (isset($data['node-codebook_section-field_codebook_ordinances'])) {
    $data['node-codebook_section-field_codebook_ordinances']['display']['codebook_embed_pdf'] = array(
      'label' => 'inline',
      'module' => 'entityreference',
      'settings' => array(
        'link' => 1,
      ),
      'type' => 'entityreference_label',
      'weight' => 1,
    ); /* WAS: '' */
    $data['node-codebook_section-field_codebook_ordinances']['display']['print'] = array(
      'label' => 'inline',
      'module' => 'entityreference',
      'settings' => array(
        'link' => 1,
      ),
      'type' => 'entityreference_label',
      'weight' => 1,
    ); /* WAS: '' */
  }
  if (isset($data['node-codebook_section-field_codebook_section_notes'])) {
    $data['node-codebook_section-field_codebook_section_notes']['display']['codebook_embed_pdf'] = array(
      'label' => 'hidden',
      'module' => 'codebook_core',
      'settings' => array(),
      'type' => 'codebook_core_section_note',
      'weight' => 3,
    ); /* WAS: '' */
    $data['node-codebook_section-field_codebook_section_notes']['display']['print'] = array(
      'label' => 'hidden',
      'module' => 'codebook_core',
      'settings' => array(),
      'type' => 'codebook_core_section_note',
      'weight' => 3,
    ); /* WAS: '' */
  }
}

/**
 * Implements hook_strongarm_alter().
 */
function codebook_print_pdf_strongarm_alter(&$data) {
  if (isset($data['field_bundle_settings_node__codebook_chapter'])) {
    $data['field_bundle_settings_node__codebook_chapter']->value['extra_fields']['display']['codebook_footnotes_eva_chapter_footnotes']['codebook_embed_pdf'] = array(
      'weight' => 8,
      'visible' => FALSE,
    ); /* WAS: '' */
    $data['field_bundle_settings_node__codebook_chapter']->value['extra_fields']['display']['codebook_footnotes_eva_chapter_footnotes']['default']['weight'] = 8; /* WAS: 6 */
    $data['field_bundle_settings_node__codebook_chapter']->value['extra_fields']['display']['codebook_footnotes_eva_chapter_footnotes']['print'] = array(
      'weight' => 8,
      'visible' => FALSE,
    ); /* WAS: '' */
    $data['field_bundle_settings_node__codebook_chapter']->value['extra_fields']['display']['codebook_pdf_eva_chapter_full'] = array(
      'print' => array(
        'weight' => 1,
        'visible' => TRUE,
      ),
      'codebook_embed_pdf' => array(
        'weight' => 1,
        'visible' => TRUE,
      ),
      'default' => array(
        'weight' => 5,
        'visible' => FALSE,
      ),
    ); /* WAS: '' */
    $data['field_bundle_settings_node__codebook_chapter']->value['extra_fields']['display']['codebook_pdf_eva_chapter_summary'] = array(
      'print' => array(
        'weight' => 0,
        'visible' => TRUE,
      ),
      'codebook_embed_pdf' => array(
        'weight' => 0,
        'visible' => TRUE,
      ),
      'default' => array(
        'weight' => 6,
        'visible' => FALSE,
      ),
    ); /* WAS: '' */
    $data['field_bundle_settings_node__codebook_chapter']->value['extra_fields']['display']['codebook_sub_docs_entity_view_1']['codebook_embed_pdf'] = array(
      'weight' => 1,
      'visible' => TRUE,
    ); /* WAS: '' */
    $data['field_bundle_settings_node__codebook_chapter']->value['extra_fields']['display']['codebook_sub_docs_entity_view_1']['print'] = array(
      'weight' => 1,
      'visible' => TRUE,
    ); /* WAS: '' */
    $data['field_bundle_settings_node__codebook_chapter']->value['extra_fields']['display']['codebook_sub_docs_eva_sections_full']['codebook_embed_pdf'] = array(
      'weight' => 3,
      'visible' => FALSE,
    ); /* WAS: '' */
    $data['field_bundle_settings_node__codebook_chapter']->value['extra_fields']['display']['codebook_sub_docs_eva_sections_full']['print'] = array(
      'weight' => 3,
      'visible' => FALSE,
    ); /* WAS: '' */
    $data['field_bundle_settings_node__codebook_chapter']->value['extra_fields']['display']['codebook_sub_docs_eva_sections_summary']['codebook_embed_pdf'] = array(
      'weight' => 2,
      'visible' => FALSE,
    ); /* WAS: '' */
    $data['field_bundle_settings_node__codebook_chapter']->value['extra_fields']['display']['codebook_sub_docs_eva_sections_summary']['print'] = array(
      'weight' => 2,
      'visible' => FALSE,
    ); /* WAS: '' */
    $data['field_bundle_settings_node__codebook_chapter']->value['extra_fields']['display']['codebook_sub_docs_eva_sections']['codebook_embed_pdf'] = array(
      'weight' => 0,
      'visible' => TRUE,
    ); /* WAS: '' */
    $data['field_bundle_settings_node__codebook_chapter']->value['extra_fields']['display']['codebook_sub_docs_eva_sections']['print'] = array(
      'weight' => 0,
      'visible' => TRUE,
    ); /* WAS: '' */
    $data['field_bundle_settings_node__codebook_chapter']->value['extra_fields']['display']['codebook_sub_docs_pdf_eva_sections_full']['codebook_embed_pdf'] = array(
      'weight' => 10,
      'visible' => FALSE,
    ); /* WAS: '' */
    $data['field_bundle_settings_node__codebook_chapter']->value['extra_fields']['display']['codebook_sub_docs_pdf_eva_sections_full']['print'] = array(
      'weight' => 10,
      'visible' => FALSE,
    ); /* WAS: '' */
    $data['field_bundle_settings_node__codebook_chapter']->value['extra_fields']['display']['codebook_sub_docs_pdf_eva_sections_summary']['codebook_embed_pdf'] = array(
      'weight' => 10,
      'visible' => FALSE,
    ); /* WAS: '' */
    $data['field_bundle_settings_node__codebook_chapter']->value['extra_fields']['display']['codebook_sub_docs_pdf_eva_sections_summary']['print'] = array(
      'weight' => 10,
      'visible' => FALSE,
    ); /* WAS: '' */
    $data['field_bundle_settings_node__codebook_chapter']->value['view_modes']['codebook_embed_pdf'] = array(
      'custom_settings' => TRUE,
    ); /* WAS: '' */
    $data['field_bundle_settings_node__codebook_chapter']->value['view_modes']['print'] = array(
      'custom_settings' => TRUE,
    ); /* WAS: '' */
  }
  if (isset($data['field_bundle_settings_node__codebook_section'])) {
    $data['field_bundle_settings_node__codebook_section']->value['view_modes']['codebook_embed_pdf'] = array(
      'custom_settings' => TRUE,
    ); /* WAS: '' */
    $data['field_bundle_settings_node__codebook_section']->value['view_modes']['print'] = array(
      'custom_settings' => TRUE,
    ); /* WAS: '' */
  }
  if (isset($data['field_bundle_settings_node__codebook_title'])) {
    $data['field_bundle_settings_node__codebook_title']->value['extra_fields']['display']['codebook_pdf_eva_title_full'] = array(
      'print' => array(
        'weight' => 1,
        'visible' => TRUE,
      ),
      'default' => array(
        'weight' => 10,
        'visible' => FALSE,
      ),
    ); /* WAS: '' */
    $data['field_bundle_settings_node__codebook_title']->value['extra_fields']['display']['codebook_pdf_eva_title_summary'] = array(
      'print' => array(
        'weight' => 0,
        'visible' => TRUE,
      ),
      'default' => array(
        'weight' => 10,
        'visible' => FALSE,
      ),
    ); /* WAS: '' */
    $data['field_bundle_settings_node__codebook_title']->value['view_modes']['codebook_embed_pdf'] = array(
      'custom_settings' => FALSE,
    ); /* WAS: '' */
    $data['field_bundle_settings_node__codebook_title']->value['view_modes']['print'] = array(
      'custom_settings' => TRUE,
    ); /* WAS: '' */
  }
}

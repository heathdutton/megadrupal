<?php
/**
 * @file
 * codebook_print_pdf.features.features_overrides.inc
 */

/**
 * Implements hook_features_override_default_overrides().
 */
function codebook_print_pdf_features_override_default_overrides() {
  // This code is only used for UI in features. Exported alters hooks do the magic.
  $overrides = array();

  // Exported overrides for: field_instance
  $overrides["field_instance.node-codebook_chapter-field_codebook_doc_heading.display|codebook_embed_pdf"] = array(
    'label' => 'above',
    'settings' => array(),
    'type' => 'hidden',
    'weight' => 6,
  );
  $overrides["field_instance.node-codebook_chapter-field_codebook_doc_heading.display|print"] = array(
    'label' => 'above',
    'settings' => array(),
    'type' => 'hidden',
    'weight' => 6,
  );
  $overrides["field_instance.node-codebook_chapter-field_codebook_doc_num.display|codebook_embed_pdf"] = array(
    'label' => 'inline',
    'settings' => array(),
    'type' => 'hidden',
    'weight' => 5,
  );
  $overrides["field_instance.node-codebook_chapter-field_codebook_doc_num.display|print"] = array(
    'label' => 'inline',
    'settings' => array(),
    'type' => 'hidden',
    'weight' => 5,
  );
  $overrides["field_instance.node-codebook_chapter-field_codebook_title.display|codebook_embed_pdf"] = array(
    'label' => 'hidden',
    'settings' => array(),
    'type' => 'hidden',
    'weight' => 4,
  );
  $overrides["field_instance.node-codebook_chapter-field_codebook_title.display|print"] = array(
    'label' => 'hidden',
    'settings' => array(),
    'type' => 'hidden',
    'weight' => 4,
  );
  $overrides["field_instance.node-codebook_chapter-field_codebook_title_notes.display|codebook_embed_pdf"] = array(
    'label' => 'above',
    'settings' => array(),
    'type' => 'hidden',
    'weight' => 7,
  );
  $overrides["field_instance.node-codebook_chapter-field_codebook_title_notes.display|print"] = array(
    'label' => 'above',
    'settings' => array(),
    'type' => 'hidden',
    'weight' => 7,
  );
  $overrides["field_instance.node-codebook_section-body.display|codebook_embed_pdf"] = array(
    'label' => 'hidden',
    'module' => 'text',
    'settings' => array(),
    'type' => 'text_default',
    'weight' => 0,
  );
  $overrides["field_instance.node-codebook_section-body.display|print"] = array(
    'label' => 'hidden',
    'module' => 'text',
    'settings' => array(),
    'type' => 'text_default',
    'weight' => 0,
  );
  $overrides["field_instance.node-codebook_section-field_codebook_chapter.display|codebook_embed_pdf"] = array(
    'label' => 'hidden',
    'settings' => array(),
    'type' => 'hidden',
    'weight' => 4,
  );
  $overrides["field_instance.node-codebook_section-field_codebook_chapter.display|print"] = array(
    'label' => 'hidden',
    'settings' => array(),
    'type' => 'hidden',
    'weight' => 4,
  );
  $overrides["field_instance.node-codebook_section-field_codebook_doc_heading.display|codebook_embed_pdf"] = array(
    'label' => 'above',
    'settings' => array(),
    'type' => 'hidden',
    'weight' => 6,
  );
  $overrides["field_instance.node-codebook_section-field_codebook_doc_heading.display|print"] = array(
    'label' => 'above',
    'settings' => array(),
    'type' => 'hidden',
    'weight' => 6,
  );
  $overrides["field_instance.node-codebook_section-field_codebook_doc_num.display|codebook_embed_pdf"] = array(
    'label' => 'above',
    'settings' => array(),
    'type' => 'hidden',
    'weight' => 5,
  );
  $overrides["field_instance.node-codebook_section-field_codebook_doc_num.display|print"] = array(
    'label' => 'above',
    'settings' => array(),
    'type' => 'hidden',
    'weight' => 5,
  );
  $overrides["field_instance.node-codebook_section-field_codebook_exhibits.display|codebook_embed_pdf"] = array(
    'label' => 'above',
    'module' => 'file',
    'settings' => array(),
    'type' => 'file_default',
    'weight' => 2,
  );
  $overrides["field_instance.node-codebook_section-field_codebook_exhibits.display|print"] = array(
    'label' => 'above',
    'module' => 'file',
    'settings' => array(),
    'type' => 'file_default',
    'weight' => 2,
  );
  $overrides["field_instance.node-codebook_section-field_codebook_ordinances.display|codebook_embed_pdf"] = array(
    'label' => 'inline',
    'module' => 'entityreference',
    'settings' => array(
      'link' => 1,
    ),
    'type' => 'entityreference_label',
    'weight' => 1,
  );
  $overrides["field_instance.node-codebook_section-field_codebook_ordinances.display|print"] = array(
    'label' => 'inline',
    'module' => 'entityreference',
    'settings' => array(
      'link' => 1,
    ),
    'type' => 'entityreference_label',
    'weight' => 1,
  );
  $overrides["field_instance.node-codebook_section-field_codebook_section_notes.display|codebook_embed_pdf"] = array(
    'label' => 'hidden',
    'module' => 'codebook_core',
    'settings' => array(),
    'type' => 'codebook_core_section_note',
    'weight' => 3,
  );
  $overrides["field_instance.node-codebook_section-field_codebook_section_notes.display|print"] = array(
    'label' => 'hidden',
    'module' => 'codebook_core',
    'settings' => array(),
    'type' => 'codebook_core_section_note',
    'weight' => 3,
  );

  // Exported overrides for: variable
  $overrides["variable.field_bundle_settings_node__codebook_chapter.value|extra_fields|display|codebook_footnotes_eva_chapter_footnotes|codebook_embed_pdf"] = array(
    'weight' => 8,
    'visible' => FALSE,
  );
  $overrides["variable.field_bundle_settings_node__codebook_chapter.value|extra_fields|display|codebook_footnotes_eva_chapter_footnotes|default|weight"] = 8;
  $overrides["variable.field_bundle_settings_node__codebook_chapter.value|extra_fields|display|codebook_footnotes_eva_chapter_footnotes|print"] = array(
    'weight' => 8,
    'visible' => FALSE,
  );
  $overrides["variable.field_bundle_settings_node__codebook_chapter.value|extra_fields|display|codebook_pdf_eva_chapter_full"] = array(
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
  );
  $overrides["variable.field_bundle_settings_node__codebook_chapter.value|extra_fields|display|codebook_pdf_eva_chapter_summary"] = array(
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
  );
  $overrides["variable.field_bundle_settings_node__codebook_chapter.value|extra_fields|display|codebook_sub_docs_entity_view_1|codebook_embed_pdf"] = array(
    'weight' => 1,
    'visible' => TRUE,
  );
  $overrides["variable.field_bundle_settings_node__codebook_chapter.value|extra_fields|display|codebook_sub_docs_entity_view_1|print"] = array(
    'weight' => 1,
    'visible' => TRUE,
  );
  $overrides["variable.field_bundle_settings_node__codebook_chapter.value|extra_fields|display|codebook_sub_docs_eva_sections_full|codebook_embed_pdf"] = array(
    'weight' => 3,
    'visible' => FALSE,
  );
  $overrides["variable.field_bundle_settings_node__codebook_chapter.value|extra_fields|display|codebook_sub_docs_eva_sections_full|print"] = array(
    'weight' => 3,
    'visible' => FALSE,
  );
  $overrides["variable.field_bundle_settings_node__codebook_chapter.value|extra_fields|display|codebook_sub_docs_eva_sections_summary|codebook_embed_pdf"] = array(
    'weight' => 2,
    'visible' => FALSE,
  );
  $overrides["variable.field_bundle_settings_node__codebook_chapter.value|extra_fields|display|codebook_sub_docs_eva_sections_summary|print"] = array(
    'weight' => 2,
    'visible' => FALSE,
  );
  $overrides["variable.field_bundle_settings_node__codebook_chapter.value|extra_fields|display|codebook_sub_docs_eva_sections|codebook_embed_pdf"] = array(
    'weight' => 0,
    'visible' => TRUE,
  );
  $overrides["variable.field_bundle_settings_node__codebook_chapter.value|extra_fields|display|codebook_sub_docs_eva_sections|print"] = array(
    'weight' => 0,
    'visible' => TRUE,
  );
  $overrides["variable.field_bundle_settings_node__codebook_chapter.value|extra_fields|display|codebook_sub_docs_pdf_eva_sections_full|codebook_embed_pdf"] = array(
    'weight' => 10,
    'visible' => FALSE,
  );
  $overrides["variable.field_bundle_settings_node__codebook_chapter.value|extra_fields|display|codebook_sub_docs_pdf_eva_sections_full|print"] = array(
    'weight' => 10,
    'visible' => FALSE,
  );
  $overrides["variable.field_bundle_settings_node__codebook_chapter.value|extra_fields|display|codebook_sub_docs_pdf_eva_sections_summary|codebook_embed_pdf"] = array(
    'weight' => 10,
    'visible' => FALSE,
  );
  $overrides["variable.field_bundle_settings_node__codebook_chapter.value|extra_fields|display|codebook_sub_docs_pdf_eva_sections_summary|print"] = array(
    'weight' => 10,
    'visible' => FALSE,
  );
  $overrides["variable.field_bundle_settings_node__codebook_chapter.value|view_modes|codebook_embed_pdf"] = array(
    'custom_settings' => TRUE,
  );
  $overrides["variable.field_bundle_settings_node__codebook_chapter.value|view_modes|print"] = array(
    'custom_settings' => TRUE,
  );
  $overrides["variable.field_bundle_settings_node__codebook_section.value|view_modes|codebook_embed_pdf"] = array(
    'custom_settings' => TRUE,
  );
  $overrides["variable.field_bundle_settings_node__codebook_section.value|view_modes|print"] = array(
    'custom_settings' => TRUE,
  );
  $overrides["variable.field_bundle_settings_node__codebook_title.value|extra_fields|display|codebook_pdf_eva_title_full"] = array(
    'print' => array(
      'weight' => 1,
      'visible' => TRUE,
    ),
    'default' => array(
      'weight' => 10,
      'visible' => FALSE,
    ),
  );
  $overrides["variable.field_bundle_settings_node__codebook_title.value|extra_fields|display|codebook_pdf_eva_title_summary"] = array(
    'print' => array(
      'weight' => 0,
      'visible' => TRUE,
    ),
    'default' => array(
      'weight' => 10,
      'visible' => FALSE,
    ),
  );
  $overrides["variable.field_bundle_settings_node__codebook_title.value|view_modes|codebook_embed_pdf"] = array(
    'custom_settings' => FALSE,
  );
  $overrides["variable.field_bundle_settings_node__codebook_title.value|view_modes|print"] = array(
    'custom_settings' => TRUE,
  );

 return $overrides;
}

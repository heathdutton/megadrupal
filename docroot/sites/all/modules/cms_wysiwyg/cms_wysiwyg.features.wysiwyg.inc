<?php
/**
 * @file
 * cms_wysiwyg.features.wysiwyg.inc
 */

/**
 * Implements hook_wysiwyg_default_profiles().
 */
function cms_wysiwyg_wysiwyg_default_profiles() {
  $profiles = array();

  // Exported profile: wysiwyg_full
  $profiles['wysiwyg_full'] = array(
    'format' => 'wysiwyg_full',
    'editor' => 'ckeditor',
    'settings' => array(
      'default' => 1,
      'user_choose' => 0,
      'show_toggle' => 1,
      'add_to_summaries' => 1,
      'theme' => '',
      'language' => 'en',
      'buttons' => array(
        'default' => array(
          'Bold' => 1,
          'Italic' => 1,
          'Underline' => 1,
          'Strike' => 1,
          'JustifyLeft' => 1,
          'JustifyCenter' => 1,
          'JustifyRight' => 1,
          'JustifyBlock' => 1,
          'BulletedList' => 1,
          'NumberedList' => 1,
          'Outdent' => 1,
          'Indent' => 1,
          'Undo' => 1,
          'Redo' => 1,
          'Link' => 1,
          'Unlink' => 1,
          'Image' => 1,
          'TextColor' => 1,
          'Superscript' => 1,
          'Subscript' => 1,
          'Blockquote' => 1,
          'Source' => 1,
          'HorizontalRule' => 1,
          'PasteText' => 1,
          'PasteFromWord' => 1,
          'ShowBlocks' => 1,
          'RemoveFormat' => 1,
          'SpecialChar' => 1,
          'Format' => 1,
          'Styles' => 1,
          'Table' => 1,
          'CreateDiv' => 1,
          'Maximize' => 1,
          'Scayt' => 1,
          'Templates' => 1,
        ),
        'drupal' => array(
          'media' => 1,
          'break' => 1,
        ),
      ),
      'toolbarLocation' => 'top',
      'resize_enabled' => 1,
      'default_toolbar_grouping' => 1,
      'simple_source_formatting' => 1,
      'acf_mode' => 0,
      'acf_allowed_content' => '',
      'css_setting' => 'self',
      'css_path' => 'http://cdn.jsdelivr.net/bootstrap/3.3.5/css/bootstrap.css,%b%t/css/glazed.css',
      'stylesSet' => 'Lead=p.lead
Muted=p.text-muted
Button Primary=div.btn.btn-primary
Button Default=div.btn.btn-default',
      'block_formats' => 'p,blockquote,h1,h2,h3,h4,h5,h6,div,address,pre',
      'advanced__active_tab' => 'edit-css',
      'forcePasteAsPlainText' => 0,
    ),
    'rdf_mapping' => array(),
  );

  // Exported profile: wysiwyg_simple
  $profiles['wysiwyg_simple'] = array(
    'format' => 'wysiwyg_simple',
    'editor' => 'ckeditor',
    'settings' => array(
      'default' => 1,
      'user_choose' => 0,
      'show_toggle' => 1,
      'add_to_summaries' => 1,
      'theme' => '',
      'language' => 'en',
      'buttons' => array(
        'default' => array(
          'Bold' => 1,
          'Italic' => 1,
          'Strike' => 1,
          'JustifyLeft' => 1,
          'JustifyCenter' => 1,
          'JustifyRight' => 1,
          'BulletedList' => 1,
          'NumberedList' => 1,
          'Link' => 1,
          'Unlink' => 1,
          'Blockquote' => 1,
          'Source' => 1,
          'RemoveFormat' => 1,
          'Maximize' => 1,
        ),
        'drupal' => array(
          'break' => 1,
        ),
      ),
      'toolbarLocation' => 'top',
      'resize_enabled' => 1,
      'default_toolbar_grouping' => 1,
      'simple_source_formatting' => 1,
      'acf_mode' => 0,
      'acf_allowed_content' => '',
      'css_setting' => 'self',
      'css_path' => 'http://cdn.jsdelivr.net/bootstrap/3.3.5/css/bootstrap.css,%b%t/css/glazed.css',
      'stylesSet' => 'Lead=p.lead
Muted=p.text-muted
Button Primary=div.btn.btn-primary
Button Default=div.btn.btn-default',
      'block_formats' => 'p,blockquote,h1,h2,h3,h4,h5,h6,div,address,pre',
      'advanced__active_tab' => 'edit-css',
      'forcePasteAsPlainText' => 0,
    ),
    'rdf_mapping' => array(),
  );

  return $profiles;
}

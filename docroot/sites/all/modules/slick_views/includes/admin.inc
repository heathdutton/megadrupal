<?php
/**
 * @file
 * Contains functions called only by admin pages.
 */

/**
 * Off-loaded SlickViews::option_definition().
 */
function _slick_views_option_definition() {
  $options = array(
    'editor_link'         => array('default' => ''),
    'slide_thumbnail'     => array('default' => ''),
    'slide_field_wrapper' => array('default' => FALSE),
    'id'                  => array('default' => ''),
  );

  foreach (slick_get_global_default_settings() as $key => $value) {
    $options[$key] = array('default' => $value);
  }

  drupal_alter('slick_views_options_info', $options);
  return $options;
}

/**
 * Off-loaded SlickViews::options_form().
 */
function _slick_views_options_form(&$form, &$form_state, $view, $settings) {
  $fields = $view->display_handler->get_handlers('field');
  $slick_fields = url(drupal_get_path('module', 'slick_fields') . '/README.txt');

  $elements = $image_options = $caption_options = $layout_options = $slide_classes = array();
  $editor_options = $title_options = $tn_caption_options = $link_options = array();

  foreach ($fields as $field => $handler) {
    if (isset($handler->field_info)) {
      $type = $handler->field_info['type'];

      switch ($type) {
        case 'file':
        case 'image':
          $image_options[$field] = $handler->ui_name();
          break;

        case 'list_text':
          $layout_options[$field] = $handler->ui_name();
          break;

        case 'entityreference':
        case 'text':
        case 'text_long':
        case 'text_with_summary':
        case 'link_field':
          $link_options[$field] = $handler->ui_name();
          $title_options[$field] = $handler->ui_name();
          if ($type != 'link_field') {
            $tn_caption_options[$field] = $handler->ui_name();
          }
          break;
      }
    }

    if (in_array($type, array('list_text', 'entityreference', 'taxonomy_term_reference', 'text'))) {
      $slide_classes[$field] = $handler->ui_name();
    }

    // Content: title is not really a field, unless title.module installed.
    if ($handler->field == 'title') {
      $slide_classes[$field] = $handler->ui_name();
      $title_options[$field] = $handler->ui_name();
      $tn_caption_options[$field] = $handler->ui_name();
    }

    if ($handler->field == 'view_node') {
      $link_options[$field] = $handler->ui_name();
    }

    if ($handler->field == 'edit_node' || $handler->field == 'delete_node') {
      $editor_options[$field] = $handler->ui_name();
    }

    // Caption can be anything to get custom works going.
    $caption_options[$field] = $handler->ui_name();
  }
  
  $layout_builtin = slick_layouts();
  $layout_options = $layout_options ? $layout_options : $layout_builtin;

  slick_get_top_elements($elements, $settings, $form_state);
  slick_get_grid_elements($elements, $settings, $form_state);

  $elements['slide_field_wrapper'] = array(
    '#type'        => 'checkbox',
    '#title'       => t('Slick markups'),
    '#description' => t('If checked, Slick will provide its markups for the following fields, ignoring Views row/field template suggestions. This is relevant for the provided skins as Slick needs to know what to style. Simply uncheck it to turn all Views row/field templates back on with your custom works. For more complex slide (asNavFor + nested/overlay + main BG altogether), be sure to apply filter criteria and skin properly.'),
  );

  $elements['slide_layout'] = array(
    '#type'        => 'select',
    '#title'       => t('Slide layout'),
    '#options'     => $layout_options + $layout_builtin,
    '#prefix'      => '<h3 class="form--slick__title">' . t('Fields') . '</h3>',
    '#description' => t('Two options: <ol><li>A custom list text field, to control each slide layout. Choose "key" under its Formatter.</li><li>Builtin layouts, to apply uniformly to all slides.</li></ol>Leave empty to DIY. <a href="@url">Read more</a> about layouts.', array('@url' => $slick_fields)),
  );

  $elements['slide_title'] = array(
    '#type'        => 'select',
    '#title'       => t('Slide title'),
    '#options'     => $title_options,
    '#description' => t('If provided, it will be wrapped with H2 and class .slide__title.'),
  );

  $elements['slide_image'] = array(
    '#type'        => 'select',
    '#title'       => t('Main image'),
    '#options'     => $image_options,
    '#description' => t('Main image, treated as background if overlay is provided. It will be lazy-loaded if the Formatter is <strong>Image</strong> and lazyLoad is enabled. Only one image is displayed per slide, even if it is a multi-value field.'),
  );

  $elements['slide_thumbnail'] = array(
    '#type'        => 'select',
    '#title'       => t('Thumbnail image'),
    '#options'     => $image_options,
    '#description' => t("Only needed if <em>Option set thumbnail</em> is provided. Maybe the same field as the main image, only different instance. Leave empty to not use thumbnail pager."),
  );

  $elements['thumbnail_caption'] = array(
    '#type'        => 'select',
    '#title'       => t('Thumbnail caption'),
    '#options'     => $tn_caption_options,
    '#description' => t('Thumbnail caption maybe just title/ plain text. If Thumbnail image is not provided, the thumbnail pagers will be just text like regular tabs.'),
  );

  $elements['slide_overlay'] = array(
    '#type'        => 'select',
    '#title'       => t('Overlay media/slicks'),
    '#options'     => $image_options,
    '#description' => t('For audio/video, be sure the display is not image. For nested slicks, use the Slick carousel formatter for this field. Zebra layout is reasonable for overlay and captions.'),
  );

  $elements['slide_link'] = array(
    '#type'        => 'select',
    '#title'       => t('Link'),
    '#options'     => $link_options,
    '#description' => t('Link to content: Read more, View Case Study, etc, wrapped with class .slide__link.'),
  );

  $elements['editor_link'] = array(
    '#type'        => 'select',
    '#title'       => t('Editor link'),
    '#options'     => $editor_options,
    '#description' => t('Link for editor: edit, delete. Leave empty if Cache is enabled.'),
  );

  $slide_captions = is_array($settings['slide_caption']) ? array_values($settings['slide_caption']) : $settings['slide_caption'];
  $elements['slide_caption'] = array(
    '#type'          => 'checkboxes',
    '#title'         => t('Caption fields'),
    '#description'   => t("Select fields for the caption, excluding those already selected above."),
    '#options'       => $caption_options,
    '#default_value' => $slide_captions,
  );

  $elements['slide_classes'] = array(
    '#title'       => t('Slide class'),
    '#type'        => 'select',
    '#options'     => $slide_classes,
    '#description' => t('If provided, individual slide will have this class, e.g.: to have different background with transparent images and skin Split, or Slick filtering. Be sure to have a Key, Label or Plain text under Formatter without links accordingly. Supported fields: Node title, Entityreference, Taxonomy term, List, Text.'),
  );

  $elements['id'] = array(
    '#type'         => 'textfield',
    '#title'        => t('Element ID'),
    '#size'         => 40,
    '#maxlength'    => 255,
    '#field_prefix' => '#',
    '#description'  => t("Manually define the Slick carousel container ID. <em>This ID is used for the cache identifier, so be sure it is unique</em>. Leave empty to have a guaranteed unique ID managed by the module."),
  );

  slick_get_elements($elements, $settings, $form_state);

  drupal_alter('slick_views_options_form', $elements, $settings);
  slick_get_admin_assets($elements, $settings);

  $form['slick'] = array(
    '#type' => 'fieldset',
    '#title' => t('Slick carousel'),
    '#attributes' => array('class' => array('form--slick form--compact form--field form--views has-tooltip')),
    '#description' => t('Leave anything empty/unchecked, except Optionset, if working with custom markups. <br /><small>Alternatively check <strong>Slick markups</strong> to get consistent markups with Field formatter, be sure to add supported fields below. Views preview works with jQuery > 1.7.</small>'),
  );

  $grids = array(
    'grid_medium',
    'grid_small',
    'visible_slides',
    'preserve_keys',
  );

  $no_markups = array(
    'optionset',
    'skin',
    'skin_arrows',
    'skin_dots',
    'slide_classes',
    'id',
    'asnavfor_main',
    'grid',
    'slide_field_wrapper',
    'override',
    'overridables',
    'cache',
  ) + $grids;

  foreach ($elements as $key => $element) {
    $form['slick'][$key] = $element;
    if (isset($elements[$key]['#states'])) {
      continue;
    }
    if (!in_array($key, $no_markups)) {
      $form['slick'][$key]['#states'] = array(
        'visible' => array(
          ':input[name*="[slide_field_wrapper]"]' => array('checked' => TRUE),
        ),
      );
    }

    if (in_array($key, array('thumbnail_caption', 'asnavfor_auto'))) {
      $form['slick'][$key]['#states'] = array(
        'invisible' => array(
          array('select[name*="[optionset_thumbnail]"]' => array('value' => '')),
          array(':input[name*="[slide_field_wrapper]"]' => array('checked' => FALSE)),
        ),
      );
    }

    if (in_array($key, array('asnavfor_main', 'asnavfor_thumbnail'))) {
      $form['slick'][$key]['#states'] = array(
        'invisible' => array(
          array('select[name*="[optionset_thumbnail]"]' => array('value' => '')),
          array(':input[name*="[slide_field_wrapper]"]' => array('checked' => FALSE)),
          array(':input[name*="[asnavfor_auto]"]' => array('checked' => TRUE)),
        ),
      );
    }

    if (in_array($key, $grids)) {
      $form['slick'][$key]['#states'] = array(
        'invisible' => array(
          array('select[name$="[grid]"]' => array('value' => '')),
        ),
      );
    }

    if ($key == 'overridables') {
      $form[$key]['#states'] = array(
        'visible' => array(
          ':input[name$="[override]"]' => array('checked' => TRUE),
        ),
      );
    }
  }
}

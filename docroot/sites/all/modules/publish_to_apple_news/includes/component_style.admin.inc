<?php

/**
 * @file
 * Administration pages and forms for Publish to Apple News component styles.
 */

/**
 * Admin list of a article type's styles.
 */
function publish_to_apple_news_component_styles_page($article_type) {
  $render = array();
  $query = new EntityFieldQuery();
  $result = $query->entityCondition('entity_type', 'publish_to_apple_news_component_style')
    ->propertyCondition('tid', $article_type->tid)
    ->execute();
  
  if (isset($result['publish_to_apple_news_component_style'])) {
    $styles = entity_load('publish_to_apple_news_component_style', array_keys($result['publish_to_apple_news_component_style']));
    $header = array('Style Name', 'Operations');
    $rows = array();
    
    foreach ($styles as $style) {
      $edit_link = l(t('Edit'), 'admin/structure/publish-to-apple-news/types/' . $article_type->tid . '/styles/' . $style->sid . '/edit');
      $delete_link = l(t('Delete'), 'admin/structure/publish-to-apple-news/types/' . $article_type->tid . '/styles/' . $style->sid . '/delete');
      
      $rows[] = array(
        $style->identifier,
        "$edit_link | $delete_link",
      );
    }
    
    $render['table'] = array(
      '#theme' => 'table',
      '#header' => $header,
      '#rows' => $rows,
    );
  } else {
    $render = array(
      '#markup' => '<p>No component styles found.</p>',
    );
  }
  
  return $render;
}

/**
 * Form for creating an Publish to Apple News component style.
 */
function publish_to_apple_news_component_style_form($form, &$form_state, $article_type, $component_style = NULL) {
  drupal_add_css(drupal_get_path('module', 'publish_to_apple_news') . '/css/forms.css');
  $module_directory = drupal_get_path('module', 'publish_to_apple_news');
  drupal_add_js($module_directory . '/js/modal/jquery.modal.js');
  drupal_add_css($module_directory . '/js/modal/jquery.modal.css');
  
  $form_state['publish_to_apple_news_article_type'] = $article_type;
  $form_state['publish_to_apple_news_component_style'] = isset($component_style) ? $component_style : entity_create('publish_to_apple_news_component_style', array());
  
  if (!isset($form_state['entity_type'])) {
    $form_state['entity_type'] = 'publish_to_apple_news_component_style';
  }
  $form['token_tree'] = array(
    '#type' => 'fieldset',
    '#title' => t('Replacement patterns'),
    '#attached' => array('js' => array('misc/collapse.js', 'misc/form.js')),
    '#value' => theme('token_tree', array('token_types' => array('node'), 'recursion_limit' => 2)),
    '#prefix' => '<div id="ui-replacement-patterns" style="display: none">',
    '#suffix' => '</div>',
  );
  
  $form['identifier'] = array(
    '#type' => 'textfield',
    '#title' => t('Identifier'),
    '#description' => t('The unique identifier for this style to be referenced by components.'),
    '#default_value' => isset($component_style) ? $component_style->identifier : '',
    '#required' => TRUE,
  );
  $form['background_color'] = array(
    '#type' => 'textfield',
    '#title' => t('Background color'),
    '#description' => t("The component's background color, defined as a 3- to 8-character RGBA hexidecimal string. Should include '#'."),
    '#default_value' => isset($component_style) ? $component_style->background_color : '',
  );
  $form['opacity'] = array(
    '#type' => 'textfield',
    '#title' => t('Opacity'),
    '#description' => t('The opacity of the component, set as a float value between 0 (completely transparent) and 1 (completely opaque).'),
    '#default_value' => isset($component_style) ? $component_style->opacity : 1,
  );
  $form['image_fill'] = array(
    '#type' => 'fieldset',
    '#title' => 'Image fill',
    '#description' => t('Fills a component with a background image.'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
    'image_fill_url' => array(
      '#type' => 'textfield',
      '#maxlength' => 255,
      '#title' => t('Image URL'),
      '#description' => t('The URL of the image file to use for filling the component. You may use <a href="#ui-replacement-patterns" rel="modal:open">replacement patterns</a>.'),
      '#default_value' => isset($component_style) ? $component_style->image_fill_url : '',
    ),
    'image_fill_mode' => array(
      '#type' => 'select',
      '#options' => array(
        '' => t('Select...'),
        'cover' => t('Cover'),
        'fit' => t('Fit'),
      ),
      '#title' => t('Fill mode'),
      '#description' => t('Indicates how the image fill should be displayed.'),
      '#default_value' => isset($component_style) ? $component_style->image_fill_mode : '',
    ),
    'image_fill_vertical_alignment' => array(
      '#type' => 'select',
      '#options' => array(
        '' => t('Select...'),
        'center' => t('Center'),
        'top' => t('Top'),
        'bottom' => t('Bottom'),
      ),
      '#title' => t('Vertical alignment'),
      '#description' => t('The vertical alignment of the image fill within its component.'),
      '#default_value' => isset($component_style) ? $component_style->image_fill_vertical_alignment : '',
    ),
    'image_fill_horizontal_alignment' => array(
      '#type' => 'select',
      '#options' => array(
        '' => t('Select...'),
        'center' => t('Center'),
        'left' => t('Left'),
        'right' => t('Right'),
      ),
      '#title' => t('Horizontal alignment'),
      '#description' => t('The horizontal alignment of the image fill within its component.'),
      '#default_value' => isset($component_style) ? $component_style->image_fill_horizontal_alignment : '',
    ),
    'image_fill_attachment' => array(
      '#type' => 'select',
      '#options' => array(
        '' => t('Select...'),
        'scroll' => t('Scroll'),
        'fixed' => t('Fixed'),
      ),
      '#title' => t('Attachment'),
      '#description' => t('Indicates how the fill should behave when a user scrolls.'),
      '#default_value' => isset($component_style) ? $component_style->image_fill_attachment : '',
    ),
  );
  $boolean_options = array(
    0 => 'False',
    1 => 'True',
  );
  $form['border'] = array(
    '#type' => 'fieldset',
    '#title' => 'Border',
    '#description' => t('The border for the component, which inside the component and therefor influences the size of the content within the component.'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
    'border_color' => array(
      '#type' => 'textfield',
      '#title' => t('Color'),
      '#description' => t("The stroke color, defined as a 3- to 8-character RGBA hexadecimal string. Should include '#'."),
      '#default_value' => isset($component_style) ? $component_style->border_color : '',
    ),
    'border_width' => array(
      '#type' => 'textfield',
      '#title' => t('Width'),
      '#description' => t('The width of the stroke line. Can be either an integer value (for points), or a string compliant with the supported units, e.g. 20vh.'),
      '#default_value' => isset($component_style) ? $component_style->border_width : '',
    ),
    'border_style' => array(
      '#type' => 'select',
      '#options' => array(
        '' => t('Select...'),
        'solid' => t('Solid'),
        'dashed' => t('Dashed'),
        'dotted' => t('Dotted'),
      ),
      '#title' => t('Style'),
      '#description' => t('Defines the style of the stroke.'),
      '#default_value' => isset($component_style) ? $component_style->border_style : '',
    ),
    'border_top' => array(
      '#type' => 'select',
      '#options' => $boolean_options,
      '#title' => t('Top'),
      '#description' => t('Indicates whether the border should be applied to the top.'),
      '#default_value' => isset($component_style) ? $component_style->border_top : 0,
    ),
    'border_bottom' => array(
      '#type' => 'select',
      '#options' => $boolean_options,
      '#title' => t('Bottom'),
      '#description' => t('Indicates whether the border should be applied to the bottom.'),
      '#default_value' => isset($component_style) ? $component_style->border_bottom : 0,
    ),
    'border_left' => array(
      '#type' => 'select',
      '#options' => $boolean_options,
      '#title' => t('Left'),
      '#description' => t('Indicates whether the border should be applied to the left.'),
      '#default_value' => isset($component_style) ? $component_style->border_left : 0,
    ),
    'border_right' => array(
      '#type' => 'select',
      '#options' => $boolean_options,
      '#title' => t('Right'),
      '#description' => t('Indicates whether the border should be applied to the right.'),
      '#default_value' => isset($component_style) ? $component_style->border_right : 0,
    ),
  );
  $form['actions'] = array(
    '#type' => 'actions',
    'submit' => array(
      '#type' => 'submit',
      '#value' => t('Save'),
    ),
  );
  
  return $form;
}

/**
 * Validation for creating/editing a component style.
 */
function publish_to_apple_news_component_style_form_validate($form, &$form_state) {
  // Make sure no other component style in this article type has the same identifier
  $query = new EntityFieldQuery();
  $query->entityCondition('entity_type', 'publish_to_apple_news_component_style')
    ->propertyCondition('tid', $form_state['publish_to_apple_news_article_type']->tid)
    ->propertyCondition('identifier', $form_state['values']['identifier']);
  
  if (!empty($form_state['publish_to_apple_news_component_style']->sid)) {
    $query->propertyCondition('sid', $form_state['publish_to_apple_news_component_style']->sid, '!=');
  }
  
  $result = $query->execute();
  
  if (isset($result['publish_to_apple_news_component_style'])) {
    form_set_error('identifier', t('Another component style in this article type has the same identifier. The identifier must be unique.'));
  }
}

/**
 * Submit handler for creating/editing a component style.
 */
function publish_to_apple_news_component_style_form_submit(&$form, &$form_state) {  
  $component_style = entity_ui_controller($form_state['entity_type'])->entityFormSubmitBuildEntity($form, $form_state);
  
  // Save article type relationship
  $component_style->tid = $form_state['publish_to_apple_news_article_type']->tid;
  
  if ($component_style->save()) {
    drupal_set_message(t('Your component style has been saved.'));
  }
  
  $form_state['redirect'] = 'admin/structure/publish-to-apple-news/types/' . $form_state['publish_to_apple_news_component_style']->tid . '/styles';
}

/**
 * Confirm form for deleting a component style.
 */
function publish_to_apple_news_component_style_delete_form($form, &$form_state, $article_type, $component_style) {
  $form_state['article_type'] = $article_type;
  $form_state['component_style'] = $component_style;
  
  return confirm_form(
    $form,
    t('Are you sure you want to delete this component style?'),
    'admin/structure/publish-to-apple-news/types/' . $article_type->tid . '/styles',
    t('This action cannot be undone.'),
    t('Delete'),
    t('Cancel')
  );
}

/**
 * Submit handler for deleting a component style.
 */
function publish_to_apple_news_component_style_delete_form_submit(&$form, &$form_state) {
  $article_type = $form_state['article_type'];
  $component_style = $form_state['component_style'];
  
  $component_style->delete();
  
  drupal_set_message(t('Your component style has been deleted.'));
  
  $form_state['redirect'] = 'admin/structure/publish-to-apple-news/types/' . $article_type->tid . '/styles';
}

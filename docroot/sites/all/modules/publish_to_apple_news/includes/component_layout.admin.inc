<?php

/**
 * @file
 * Administration pages and forms for Publish to Apple News component layouts.
 */

/**
 * Admin list of a article type's layouts.
 */
function publish_to_apple_news_component_layouts_page($article_type) {
  $render = array();
  $query = new EntityFieldQuery();
  $result = $query->entityCondition('entity_type', 'publish_to_apple_news_component_layout')
    ->propertyCondition('tid', $article_type->tid)
    ->execute();
  
  if (isset($result['publish_to_apple_news_component_layout'])) {
    $layouts = entity_load('publish_to_apple_news_component_layout', array_keys($result['publish_to_apple_news_component_layout']));
    $header = array('Layout Name', 'Operations');
    $rows = array();
    
    foreach ($layouts as $layout) {
      $edit_link = l(t('Edit'), 'admin/structure/publish-to-apple-news/types/' . $article_type->tid . '/layouts/' . $layout->lid . '/edit');
      $delete_link = l(t('Delete'), 'admin/structure/publish-to-apple-news/types/' . $article_type->tid . '/layouts/' . $layout->lid . '/delete');
      
      $rows[] = array(
        $layout->identifier,
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
      '#markup' => '<p>No component layouts found.</p>',
    );
  }
  
  return $render;
}

/**
 * Form for creating a Publish to Apple News component layout.
 */
function publish_to_apple_news_component_layout_form($form, &$form_state, $article_type, $component_layout = NULL) {
  drupal_add_css(drupal_get_path('module', 'publish_to_apple_news') . '/css/forms.css');
  $form_state['publish_to_apple_news_article_type'] = $article_type;
  $form_state['publish_to_apple_news_component_layout'] = isset($component_layout) ? $component_layout : entity_create('publish_to_apple_news_component_layout', array());
  
  if (!isset($form_state['entity_type'])) {
    $form_state['entity_type'] = 'publish_to_apple_news_component_layout';
  }
    
  $form['identifier'] = array(
    '#type' => 'textfield',
    '#title' => t('Identifier'),
    '#description' => t('The unique identifier for this layout to be referenced by components.'),
    '#default_value' => isset($component_layout) ? $component_layout->identifier : '',
    '#required' => TRUE,
  );
  $form['column_start'] = array(
    '#type' => 'textfield',
    '#size' => 10,
    '#title' => t('Column start'),
    '#description' => t("Indicates which column the component's start position is in, based on the number of columns in the document."),
    '#default_value' => isset($component_layout) ? $component_layout->column_start : 0,
    '#required' => TRUE,
  );
  $form['column_span'] = array(
    '#type' => 'textfield',
    '#size' => 10,
    '#title' => t('Column span'),
    '#description' => t('Indicates how many columns the component spans, based on the number of columns in the document.'),
    '#default_value' => isset($component_layout) ? $component_layout->column_span : 7,
    '#required' => TRUE,
  );
  $form['margin_top'] = array(
    '#type' => 'textfield',
    '#size' => 10,
    '#title' => t('Margin top'),
    '#description' => t('The top margin in points.'),
    '#default_value' => !empty($component_layout->margin_top) ? $component_layout->margin_top : '',
  );
  $form['margin_bottom'] = array(
    '#type' => 'textfield',
    '#size' => 10,
    '#title' => t('Margin bottom'),
    '#description' => t('The bottom margin in points.'),
    '#default_value' => !empty($component_layout->margin_bottom) ? $component_layout->margin_bottom : '',
  );
  
  $boolean_options = array(
    0 => t('False'),
    1 => t('True'),
  );
  $form['content_inset'] = array(
    '#type' => 'fieldset',
    '#title' => t('Content inset'),
    '#description' => t('Sets which side(s) of a component need to apply an inset for its content.'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
    'content_inset_top' => array(
      '#type' => 'select',
      '#options' => $boolean_options,
      '#title' => t('Top'),
      '#description' => t('Whether the top inset should be taken into account.'),
      '#default_value' => isset($component_layout) ? $component_layout->content_inset_top : 0,
    ),
    'content_inset_right' => array(
      '#type' => 'select',
      '#options' => $boolean_options,
      '#title' => t('Right'),
      '#description' => t('Whether the right inset should be taken into account.'),
      '#default_value' => isset($component_layout) ? $component_layout->content_inset_right : 0,
    ),
    'content_inset_bottom' => array(
      '#type' => 'select',
      '#options' => $boolean_options,
      '#title' => t('Bottom'),
      '#description' => t('Whether the bottom inset should be taken into account.'),
      '#default_value' => isset($component_layout) ? $component_layout->content_inset_bottom : 0,
    ),
    'content_inset_left' => array(
      '#type' => 'select',
      '#options' => $boolean_options,
      '#title' => t('Left'),
      '#description' => t('Whether the left inset should be taken into account.'),
      '#default_value' => isset($component_layout) ? $component_layout->content_inset_left : 0,
    ),
  );
  $form['ignore_document_margin'] = array(
    '#type' => 'select',
    '#options' => array(
      'none' => t('None'),
      'left' => t('Left'),
      'right' => t('Right'),
      'both' => t('Both'),
    ),
    '#title' => t('Ignore document margin'),
    '#description' => t("Indicates whether a document's margins should be respected or should be ignored by the component."),
    '#default_value' => isset($component_layout) ? $component_layout->ignore_document_margin : 'none',
  );
  $form['ignore_document_gutter'] = array(
    '#type' => 'select',
    '#options' => array(
      'none' => t('None'),
      'left' => t('Left'),
      'right' => t('Right'),
      'both' => t('Both'),
    ),
    '#title' => t('Ignore document gutter'),
    '#description' => t('Indicates whether the gutters (if any) to the left and right of the component should be ignored.'),
    '#default_value' => isset($component_layout) ? $component_layout->ignore_document_gutter : 'none',
  );
  $form['minimum_height'] = array(
    '#type' => 'textfield',
    '#size' => 10,
    '#title' => t('Minimum height'),
    '#description' => t('Describes the minimum height of the component.'),
    '#default_value' => !empty($component_layout->minimum_height) ? $component_layout->minimum_height : '',
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
 * Submit handler for selecting component type when adding a new layout.
 */
function publish_to_apple_news_component_layout_form_type_submit($form, &$form_state) {
  $form_state['new_component_type'] = $form_state['values']['component_type'];
  $form_state['rebuild'] = TRUE;
}

/**
 * Validation for creating/editing a component layout.
 */
function publish_to_apple_news_component_layout_form_validate($form, &$form_state) {
  // Make sure no other component layout in this article type has the same identifier
  $query = new EntityFieldQuery();
  $query->entityCondition('entity_type', 'publish_to_apple_news_component_layout')
    ->propertyCondition('tid', $form_state['publish_to_apple_news_article_type']->tid)
    ->propertyCondition('identifier', $form_state['values']['identifier']);
  
  if (!empty($form_state['publish_to_apple_news_component_layout']->lid)) {
    $query->propertyCondition('lid', $form_state['publish_to_apple_news_component_layout']->lid, '!=');
  }
  
  $result = $query->execute();
  
  if (isset($result['publish_to_apple_news_component_layout'])) {
    form_set_error('identifier', t('Another component layout in this article type has the same identifier. The identifier must be unique.'));
  }
}

/**
 * Submit handler for creating/editing a component layout.
 */
function publish_to_apple_news_component_layout_form_submit($form, &$form_state) {  
  $component_layout = entity_ui_controller($form_state['entity_type'])->entityFormSubmitBuildEntity($form, $form_state);
  
  // Save article type relationship
  $component_layout->tid = $form_state['publish_to_apple_news_article_type']->tid;
  
  if ($component_layout->save()) {
    drupal_set_message(t('Your component layout has been saved.'));
  }
  
  $form_state['redirect'] = 'admin/structure/publish-to-apple-news/types/' . $form_state['publish_to_apple_news_component_layout']->tid . '/layouts';
}

/**
 * Confirm form for deleting a component layout.
 */
function publish_to_apple_news_component_layout_delete_form($form, &$form_state, $article_type, $component_layout) {
  $form_state['article_type'] = $article_type;
  $form_state['component_layout'] = $component_layout;
  
  return confirm_form(
    $form,
    t('Are you sure you want to delete this component layout?'),
    'admin/structure/publish-to-apple-news/types/' . $article_type->tid . '/layouts',
    t('This action cannot be undone.'),
    t('Delete'),
    t('Cancel')
  );
}

/**
 * Submit handler for deleting a component layout.
 */
function publish_to_apple_news_component_layout_delete_form_submit($form, &$form_state) {
  $article_type = $form_state['article_type'];
  $component_layout = $form_state['component_layout'];
  
  $component_layout->delete();
  
  drupal_set_message(t('Your component layout has been deleted.'));
  
  $form_state['redirect'] = 'admin/structure/publish-to-apple-news/types/' . $article_type->tid . '/layouts';
}

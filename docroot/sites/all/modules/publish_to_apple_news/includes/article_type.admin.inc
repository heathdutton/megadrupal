<?php

/**
 * @file
 * Administration pages & forms for Publish to Apple News Article Types.
 */

/**
 * Admin list of Publish to Apple News article types.
 */
function publish_to_apple_news_article_types_page() {
  drupal_add_css(drupal_get_path('module', 'publish_to_apple_news') . '/css/article-types.css');
  
  $render = array();
  $query = new EntityFieldQuery();
  $result = $query->entityCondition('entity_type', 'publish_to_apple_news_article_type')
    ->execute();

  if (isset($result['publish_to_apple_news_article_type'])) {
    $article_types = entity_load('publish_to_apple_news_article_type', array_keys($result['publish_to_apple_news_article_type']));
    $header = array('Article Type', 'Operations');
    $rows = array();

    foreach ($article_types as $article_type) {
      $content_type = ucwords($article_type->content_type);
      $type_name = l($content_type, 'admin/structure/publish-to-apple-news/types/' . $article_type->tid);
      $configure_link = l(t('Configure'), 'admin/structure/publish-to-apple-news/types/' . $article_type->tid);
      $settings_link = l(t('Settings'), 'admin/structure/publish-to-apple-news/types/' . $article_type->tid . '/settings');
      $download_link = l(t('Preview'), '/admin/structure/publish-to-apple-news/types/' . $article_type->tid . '/preview');
      $delete_link = l(t('Delete'), 'admin/structure/publish-to-apple-news/types/' . $article_type->tid . '/delete');

      $rows[] = array(
        $type_name . '<br />' . $article_type->description,
        "$configure_link | $settings_link | $download_link | $delete_link"
      );
    }

    $render['table'] = array(
      '#theme' => 'table',
      '#header' => $header,
      '#rows' => $rows,
    );
  } else {
    $render = array(
      '#markup' => '<p>No article types found.</p>',
    );
  }

  return $render;
}

/**
 * Form for creating a Publish to Apple News article type.
 */
function publish_to_apple_news_article_type_form($form, &$form_state, $article_type = NULL) {
  $module_directory = drupal_get_path('module', 'publish_to_apple_news');
  drupal_add_css($module_directory . '/css/forms.css');
  drupal_add_js($module_directory . '/js/modal/jquery.modal.js');
  drupal_add_css($module_directory . '/js/modal/jquery.modal.css');
  
  $form_state['publish_to_apple_news_article_type'] = isset($article_type) ? $article_type : entity_create('publish_to_apple_news_article_type', array());

  // Handle json for authors and keywords fields
  if (!isset($form_state['authors_processed']) && !empty($article_type->metadata_authors)) {
    $form_state['authors'] = json_decode($article_type->metadata_authors);
    $form_state['num_authors'] = count($form_state['authors']);
    $form_state['authors_processed'] = TRUE;
  }

  if (!isset($form_state['keywords_processed']) && !empty($article_type->metadata_keywords)) {
    $form_state['keywords'] = json_decode($article_type->metadata_keywords);
    $form_state['num_keywords'] = count($form_state['keywords']);
    $form_state['keywords_processed'] = TRUE;
  }

  if (!isset($form_state['entity_type'])) {
    $form_state['entity_type'] = 'publish_to_apple_news_article_type';
  }

  $content_types_list = array(
    '' => 'Select...',
  );
  $node_types = node_type_get_types();
  foreach ($node_types as $type) {
    $content_types_list[$type->type] = $type->name;
  }

  $form['content_type'] = array(
    '#type' => 'select',
    '#title' => t('Content type'),
    '#description' => t('The node type that Apple News articles of this type will be generated from.'),
    '#options' => $content_types_list,
    '#default_value' => isset($article_type) ? $article_type->content_type : '',
    '#required' => TRUE,
  );
  $form['description'] = array(
    '#type' => 'textfield',
    '#title' => t('Description'),
    '#description' => t('A description for your article type. This is only shown in Drupal.'),
    '#default_value' => isset($article_type) ? $article_type->description : '',
  );
  $form['title'] = array(
    '#type' => 'textfield',
    '#title' => t('Title'),
    '#description' => t('The article title. You may use <a href="#ui-replacement-patterns" rel="modal:open">replacement patterns</a>.'),
    '#default_value' => isset($article_type) ? $article_type->title : '[node:title]',
    '#required' => TRUE,
  );
  $form['subtitle'] = array(
    '#type' => 'textfield',
    '#title' => t('Subtitle'),
    '#description' => t('An optional subtitle. You may use <a href="#ui-replacement-patterns" rel="modal:open">replacement patterns</a>.'),
    '#default_value' => isset($article_type) ? $article_type->subtitle : '',
  );
  $form['excerpt'] = array(
    '#type' => 'textfield',
    '#title' => t('Excerpt'),
    '#description' => t('A short summary of the contents of the article. You may use <a href="#ui-replacement-patterns" rel="modal:open">replacement patterns</a>.'),
    '#default_value' => isset($article_type) ? $article_type->excerpt : '[node:summary]',
  );
  $form['token_tree'] = array(
    '#type' => 'fieldset',
    '#title' => t('Replacement patterns'),
    '#attached' => array('js' => array('misc/collapse.js', 'misc/form.js')),
    '#value' => theme('token_tree', array('token_types' => array('node'), 'recursion_limit' => 2)),
    '#prefix' => '<div id="ui-replacement-patterns" style="display: none">',
    '#suffix' => '</div>',
  );
  $form['language'] = array(
    '#type' => 'textfield',
    '#title' => t('Language'),
    '#description' => t('The language code of the article. You may use the replacement patterns above.'),
    '#default_value' => isset($article_type) ? $article_type->language : '[node:language]',
    '#required' => TRUE,
  );
  $form['metadata_thumbnail_url'] = array(
    '#type' => 'textfield',
    '#title' => t('Thumbnail URL'),
    '#description' => 'The URL of an image that can represent this document in a list of other documents. Note: only JPEG and PNG image types are supported. You may use the replacement patterns above.',
    '#default_value' => isset($article_type) ? $article_type->metadata_thumbnail_url : '[node:field_image]',
  );
  $form['metadata_canonical_url'] = array(
    '#type' => 'textfield',
    '#title' => t('Canonical URL'),
    '#description' => 'The canonical URL of a web version of this document. If you are working locally, the default replacement pattern will render as "localhost" which will throw an error from Apple. Replace with an actual base URL plus [node:path]. You may use the replacement patterns above.',
    '#default_value' => isset($article_type) ? $article_type->metadata_canonical_url : '[node:url]',
  );

  $form['#tree'] = TRUE;
  $form['metadata_authors'] = array(
    '#type' => 'fieldset',
    '#title' => t('Authors'),
    '#description' => t("The names of the article's authors. You may use the replacement patterns above."),
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
    '#prefix' => '<div id="metadata-authors">',
    '#suffix' => '</div>',
  );

  if (empty($form_state['num_authors'])) {
    $form_state['num_authors'] = 1;
  }

  for ($i = 0; $i < $form_state['num_authors']; $i++) {
    $form['metadata_authors']['authors'][$i] = array(
      '#type' => 'textfield',
      '#title' => t('Name'),
      '#default_value' => isset($form_state['authors'][$i]) ? $form_state['authors'][$i] : '',
    );
  }

  $form['metadata_authors']['add_author'] = array(
    '#type' => 'submit',
    '#value' => t('Add author'),
    '#submit' => array('publish_to_apple_news_article_type_form_add_author'),
    '#ajax' => array(
      'callback' => 'publish_to_apple_news_article_type_form_author_callback',
      'wrapper' => 'metadata-authors',
    ),
  );
  $form['metadata_authors']['remove_author'] = array(
    '#type' => 'submit',
    '#value' => t('Remove author'),
    '#submit' => array('publish_to_apple_news_article_type_form_remove_author'),
    '#ajax' => array(
      'callback' => 'publish_to_apple_news_article_type_form_author_callback',
      'wrapper' => 'metadata-authors',
    ),
  );

  $form['metadata_keywords'] = array(
    '#type' => 'fieldset',
    '#title' => t('Keywords'),
    '#description' => t('Keywords that describe the article. You can set up to 50 keywords. You may use the replacement patterns above.'),
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
    '#prefix' => '<div id="metadata-keywords">',
    '#suffix' => '</div>',
  );

  if (empty($form_state['num_keywords'])) {
    $form_state['num_keywords'] = 1;
  }

  for ($i = 0; $i < $form_state['num_keywords']; $i++) {
    $form['metadata_keywords']['keywords'][$i] = array(
      '#type' => 'textfield',
      '#title' => t('Keyword'),
      '#default_value' => isset($form_state['keywords'][$i]) ? $form_state['keywords'][$i] : '',
    );
  }

  $form['metadata_keywords']['add_keyword'] = array(
    '#type' => 'submit',
    '#value' => t('Add keyword'),
    '#submit' => array('publish_to_apple_news_article_type_form_add_keyword'),
    '#ajax' => array(
      'callback' => 'publish_to_apple_news_article_type_form_keyword_callback',
      'wrapper' => 'metadata-keywords',
    ),
  );
  $form['metadata_keywords']['remove_keyword'] = array(
    '#type' => 'submit',
    '#value' => t('Remove keyword'),
    '#submit' => array('publish_to_apple_news_article_type_form_remove_keyword'),
    '#ajax' => array(
      'callback' => 'publish_to_apple_news_article_type_form_keyword_callback',
      'wrapper' => 'metadata-keywords',
    ),
  );

  $form['layout'] = array(
    '#type' => 'fieldset',
    '#title' => t('Layout configuration'),
    '#description' => t('Layout properties for the article.'),
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
    'layout_columns' => array(
      '#type' => 'textfield',
      '#title' => t('Columns'),
      '#description' => t('The number of columns the article was designed for. Apple highly recommends 7 columns.'),
      '#default_value' => isset($article_type) ? $article_type->layout_columns : 7,
      '#size' => 10,
      '#element_validate' => array('element_validate_number'),
      '#required' => TRUE,
    ),
    'layout_width' => array(
      '#type' => 'textfield',
      '#title' => t('Width (pt)'),
      '#description' => t('The width (in points) this article was designed for. Apple highly recommends 1024 points.'),
      '#default_value' => isset($article_type) ? $article_type->layout_width : 1024,
      '#size' => 10,
      '#element_validate' => array('element_validate_number'),
      '#required' => TRUE,
    ),
    'layout_margin' => array(
      '#type' => 'textfield',
      '#title' => t('Margin (pt)'),
      '#description' => t('The outer (left and right) margins (in points) for this article.'),
      '#default_value' => isset($article_type) ? $article_type->layout_margin : 100,
      '#size' => 10,
      '#element_validate' => array('element_validate_number'),
    ),
    'layout_gutter' => array(
      '#type' => 'textfield',
      '#title' => t('Gutter (pt)'),
      '#description' => t('The gutter size (in points) to use for spacing between columns for this article.'),
      '#default_value' => isset($article_type) ? $article_type->layout_gutter : 40,
      '#size' => 10,
      '#element_validate' => array('element_validate_number'),
    ),
  );
  $form['actions'] = array(
    '#type' => 'actions',
    'submit' => array(
      '#type' => 'submit',
      '#value' => t('Save'),
    ),
  );
  
  //close modal when replaceent pattern has been selected:
  $close_modal_js = "jQuery('#ui-replacement-patterns .token-key a').click(function() {jQuery('.close-modal').click();}); ";
  //focus on the input when opening the replacement patternt modal:
  $replacement_focus_js = "jQuery('a[href=\"#ui-replacement-patterns\"]').click(function() {jQuery(this).parent().parent().find('input[type=\"text\"]').focus();}); ";
  
  $form['#attached']['js'][] = array('data' => 'jQuery(document).ready(function () {' . $close_modal_js . $replacement_focus_js . '});', 'type' => 'inline');

  return $form;
}

/**
 * Submit handler for "Add" author button.
 */
function publish_to_apple_news_article_type_form_add_author($form, &$form_state) {
  $form_state['num_authors']++;
  $form_state['rebuild'] = TRUE;
}

/**
 * Submit handler for "Remove" author button.
 */
function publish_to_apple_news_article_type_form_remove_author($form, &$form_state) {
  $form_state['num_authors']--;
  array_pop($form_state['authors']);
  $form_state['rebuild'] = TRUE;
}

/**
 * Ajax callback for author add/remove buttons.
 */
function publish_to_apple_news_article_type_form_author_callback($form, $form_state) {
  return $form['metadata_authors'];
}

/**
 * Submit handler for "Add" keyword button.
 */
function publish_to_apple_news_article_type_form_add_keyword($form, &$form_state) {
  $form_state['num_keywords']++;
  $form_state['rebuild'] = TRUE;
}

/**
 * Submit handler for "Remove" keyword button.
 */
function publish_to_apple_news_article_type_form_remove_keyword($form, &$form_state) {
  $form_state['num_keywords']--;
  array_pop($form_state['keywords']);
  $form_state['rebuild'] = TRUE;
}

/**
 * Ajax callback for keyword add/remove buttons.
 */
function publish_to_apple_news_article_type_form_keyword_callback($form, $form_state) {
  return $form['metadata_keywords'];
}

/**
 * Validation when adding/editing an article type.
 */
function publish_to_apple_news_article_type_form_validate($form, &$form_state) {
  // Validate no other article type has the same content type selected
  $query = new EntityFieldQuery();
  $query->entityCondition('entity_type', 'publish_to_apple_news_article_type')
    ->propertyCondition('content_type', $form_state['values']['content_type']);

  if (!empty($form_state['publish_to_apple_news_article_type']->tid)) {
    $query->propertyCondition('tid', $form_state['publish_to_apple_news_article_type']->tid, '!=');
  }

  $result = $query->execute();

  if (!empty($result['publish_to_apple_news_article_type'])) {
    form_set_error('content_type', t('An article type with the selected content type already exists.'));
  }
}
/**
 * Submit handler for creating/editing an article type.
 */
function publish_to_apple_news_article_type_form_submit($form, &$form_state) {
  $article_type = entity_ui_controller($form_state['entity_type'])->entityFormSubmitBuildEntity($form, $form_state);

  foreach ($article_type->layout as $key => $value) {
    $article_type->$key = $value;
  }

  // Json encode authors and keywords
  if (!empty($form_state['values']['metadata_authors']['authors'][0])) {
    $authors = array();
    foreach ($form_state['values']['metadata_authors']['authors'] as $author_name) {
      $authors[] = $author_name;
    }

    if (!empty($authors)) {
      $authors = json_encode($authors);
      $article_type->metadata_authors = $authors;
    }
  } else {
    $article_type->metadata_authors = '';
  }

  if (!empty($form_state['values']['metadata_keywords']['keywords'][0])) {
    $keywords = array();
    foreach ($form_state['values']['metadata_keywords']['keywords'] as $keyword) {
      $keywords[] = $keyword;
    }

    if (!empty($keywords)) {
      $keywords = json_encode($keywords);
      $article_type->metadata_keywords = $keywords;
    }
  } else {
    $article_type->metadata_keywords = '';
  }
  
  $create_defaults = FALSE;
  if (empty($article_type->tid)) {
    $create_defaults = TRUE;
  }

  if ($article_type->save()) {
    if ($create_defaults) {
      // Create default layouts, styles, and text styles
      module_load_include('inc', 'publish_to_apple_news', 'includes/defaults');
      publish_to_apple_news_create_component_defaults($article_type->tid);
    }
    
    drupal_set_message(t('Your article type has been saved.'));
  }

  $form_state['redirect'] = 'admin/structure/publish-to-apple-news/types/' . $article_type->tid;
}

/**
 * Confirm form for deleting an article type.
 */
function publish_to_apple_news_article_type_delete_form($form, &$form_state, $article_type) {
  $form_state['article_type'] = $article_type;

  return confirm_form(
    $form,
    t('Are you sure you want to delete this article type?'),
    'admin/structure/publish-to-apple-news',
    t('This action cannot be undone.'),
    t('Delete'),
    t('Cancel')
  );
}

/**
 * Submit handler for deleting an article type.
 */
function publish_to_apple_news_article_type_delete_form_submit($form, &$form_state) {
  $article_type = $form_state['article_type'];

  // Delete associated entities
  // Components
  $component_types = publish_to_apple_news_component_types();
  $component_entity_types = array_keys($component_types);
  foreach ($component_entity_types as $component_type) {
    $query = new EntityFieldQuery();
    $result = $query->entityCondition('entity_type', $component_type)
      ->propertyCondition('tid', $article_type->tid)
      ->execute();

    if (!empty($result[$component_type])) {
      $cids = array_keys($result[$component_type]);
      entity_delete_multiple($component_type, $cids);
    }
  }

  // Component layouts
  db_delete('publish_to_apple_news_component_layouts')
    ->condition('tid', $article_type->tid)
    ->execute();
  // Component styles
  db_delete('publish_to_apple_news_component_styles')
    ->condition('tid', $article_type->tid)
    ->execute();
  // Component text styles
  db_delete('publish_to_apple_news_component_text_styles')
    ->condition('tid', $article_type->tid)
    ->execute();

  $article_type->delete();

  drupal_set_message(t('Your article type has been deleted.'));

  $form_state['redirect'] = 'admin/structure/publish-to-apple-news';
}

/**
 * Form for creating an article type from a template.
 */
function publish_to_apple_news_article_type_template_form($form, &$form_state) {
  $content_types_list = array();
  $node_types = node_type_get_types();
  foreach ($node_types as $type) {
    $content_types_list[$type->type] = $type->name;
  }
  
  $form['content_type'] = array(
    '#type' => 'select',
    '#title' => t('Content type'),
    '#options' => $content_types_list,
    '#required' => TRUE,
  );
  
  $form['template'] = array(
    '#title' => t('Template'),
    '#required' => TRUE,
    '#type' => 'radios',
    '#options' => publish_to_apple_news_article_type_get_example_articles(),
    '#theme' => array('publish_to_apple_news_example_article_radios'),
  );

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Create'),
  );
  
  $module_directory = drupal_get_path('module', 'publish_to_apple_news');
  
  $form['#attached']['css'] = array(
    $module_directory . '/css/publish_to_apple_news_example_article_radios.css',
    $module_directory . '/js/modal/jquery.modal.css',
  );
  
  $form['#attached']['js'] = array(
    $module_directory . '/js/modal/jquery.modal.js',
  );
  
  return $form;
}

/**
 * Helper function for scanning for example articles
 */
function publish_to_apple_news_article_type_get_example_articles() {
  $exmple_articles = file_scan_directory(drupal_get_path('module', 'publish_to_apple_news') . '/includes/example_articles', '/article.json/');
  $options = array();
  $i = 0;
  foreach ($exmple_articles as $uri => $example) {
    $options[] = $uri;
  }
  return $options;
}

function publish_to_apple_news_example_article_radios($variables) {
  
  $element = $variables['element'];

  foreach ($element['#options'] as $key => $value) {
    $element[$key]['article_path'] = $value;
    $element['#children'] .= theme('publish_to_apple_news_example_article_radio', $element[$key]);
  }   
  $attributes = array();
  if (isset($element['#id'])) {
    $attributes['id'] = $element['#id'];
  }
  $attributes['class'] = 'example-articles-radios-container';
  if (!empty($element['#attributes']['class'])) {
    $attributes['class'] .= ' ' . implode(' ', $element['#attributes']['class']);
  }

  return '<div' . drupal_attributes($attributes) . ' >' . (!empty($element['#children']) ? $element['#children'] : '') . '<div class="clear"></div></div>';
}

function publish_to_apple_news_example_article_radio($variables) {
  global $scales;
  global $base_url;

  $element = $variables['element'];
  $element['#attributes']['type'] = 'radio';
  element_set_attributes($element, array('id', 'name', '#return_value' => 'value'));


  if (isset($element['#return_value']) && $element['#value'] !== FALSE && $element['#value'] == $element['#return_value']) {
    $element['#attributes']['checked'] = 'checked';
  }
  _form_set_class($element, array('form-radio'));
  
  $article_path = $element['article_path'];
  $thumbnail = $base_url . '/' . str_replace('article.json', 'thumbnail.png', $article_path);
  $preview_image = $base_url . '/' . str_replace('article.json', 'preview.png', $article_path);
  $title = publish_to_apple_news_get_example_article_title($article_path);
  $id = strtolower(str_replace(' ', '-', $title));

  if (!is_null($element['#title'])) {
    return '<div class="example-articles-radio">'
    . '<div class="thubnail-container"><a href="#' . $id . '" rel="modal:open"><img src="' . $thumbnail . '" /></a></div>'
    . '<input' . drupal_attributes($element['#attributes']) . ' />' 
    . '<div class="title">' . $title . '</div>'
    . '<div id="' . $id . '" class="modal" style="display: none"><img src="' . $preview_image . '" /></div>'
    . '</div>';
  }
}

function publish_to_apple_news_get_example_article_title($article_path) {
  $data = explode('/', $article_path);
  $title = $data[sizeof($data) - 2];
  return str_replace('_', ' ', $title);
}

/**
 * Validation for create from template form.
 */
function publish_to_apple_news_article_type_template_form_validate($form, &$form_state) {
  // Make sure there's not an existing article type mapped to the selected content type
  $query = new EntityFieldQuery();
  $result = $query->entityCondition('entity_type', 'publish_to_apple_news_article_type')
    ->propertyCondition('content_type', $form_state['values']['content_type'])
    ->execute();
  
  if (isset($result['publish_to_apple_news_article_type'])) {
    form_set_error('content_type', t('There is already an article type mapped to the content type you selected.'));
  }
}

/**
 * Submit handler for create from template form.
 */
function publish_to_apple_news_article_type_template_form_submit($form, &$form_state) {
  $form_state['redirect'] = 'admin/structure/publish-to-apple-news/create-template/' . $form_state['values']['content_type'] . '/' . $form_state['values']['template'];
}


/**
  * Form to map fields from an example article.
  */
function publish_to_apple_news_article_type_template_example_form($form, &$form_state, $content_type, $example_id) {
  $module_directory = drupal_get_path('module', 'publish_to_apple_news');
  drupal_add_css($module_directory . '/css/create-from-template.css');
  
  $example_articles = publish_to_apple_news_article_type_get_example_articles();
  $article_path = $example_articles[$example_id];
  $article_title = publish_to_apple_news_get_example_article_title($article_path);
  $data = publish_to_apple_news_article_type_get_example_article_data($article_path);
  
  $form = array(
    '#prefix' => '<div id="create-from-template"><h1 id="create-title">' . t('Create article type from ') . $article_title . '</h1>',
    '#suffix' => '</div>',
  );
  
  $styles = $data['componentStyles'];
  
  foreach ($data['components'] as $index => $component) {
    $component_form_section = publish_to_apple_news_article_type_get_example_article_component_form_section($component, $index, 0, $styles);
    if (! empty($component_form_section)) {
      $form['component_' . $index] = $component_form_section;
    }
  }
  
  $form['token_tree'] = array(
    '#type' => 'fieldset',
    '#title' => t('Replacement patterns'),
    '#attached' => array('js' => array('misc/collapse.js', 'misc/form.js')),
    '#value' => theme('token_tree', array('token_types' => array('node'), 'recursion_limit' => 2)),
    '#prefix' => '<div id="ui-replacement-patterns" style="display: none">',
    '#suffix' => '</div>',
  );
  
  $form['content_type'] = array(
    '#type' => 'hidden',
    '#value' => $content_type,
  );
  
  $form['article_path'] = array(
    '#type' => 'hidden',
    '#value' => $article_path,
  );
  
  $form['include_ad'] = array(
    '#type' => 'checkbox',
    '#title' => t('Include an advertisement?'),
    '#default_value' => TRUE,
    '#weight' => 997,
  );
  
  $form['empty_message'] = array(
    '#markup' => '<p>Note: Components that aren\'t mapped to a value will not be created.</p>',
    '#weight' => 998,
  );

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Create'),
    '#weight' => 999,
  );
  
  $module_directory = drupal_get_path('module', 'publish_to_apple_news');
  
  $form['#attached']['css'] = array(
    $module_directory . '/css/publish_to_apple_news_example_article_radios.css',
    $module_directory . '/js/modal/jquery.modal.css',
  );
  
  $form['#attached']['js'][] = array('data' => $module_directory . '/js/modal/jquery.modal.js');
  
  //close modal when replaceent pattern has been selected:
  $close_modal_js = "jQuery('#ui-replacement-patterns .token-key a').click(function() {jQuery('.close-modal').click();}); ";
  //focus on the input when opening the replacement patternt modal:
  $replacement_focus_js = "jQuery('a[href=\"#ui-replacement-patterns\"]').click(function() {jQuery(this).prev().find('input[type=\"text\"]').focus();}); ";
  
  $form['#attached']['js'][] = array('data' => 'jQuery(document).ready(function () {' . $close_modal_js . $replacement_focus_js . '});', 'type' => 'inline');
  
  return $form;
}

/**
 * Validation for create from template example form.
 */
function publish_to_apple_news_article_type_template_example_form_validate($form, &$form_state) {
  
  // Make sure there's not an existing article type mapped to the selected content type
  $query = new EntityFieldQuery();
  $result = $query->entityCondition('entity_type', 'publish_to_apple_news_article_type')
    ->propertyCondition('content_type', $form_state['values']['content_type'])
    ->execute();
  
  if (isset($result['publish_to_apple_news_article_type'])) {
    form_set_error('content_type', t('There is already an article type mapped to the content type you selected.'));
  }
}

/**
 * Submit handler for create from template example form.
 */
function publish_to_apple_news_article_type_template_example_form_submit($form, &$form_state) {
  
  module_load_include('inc', 'publish_to_apple_news', 'includes/default_article_types');
  
  $values = $form_state['values'];
  $data = publish_to_apple_news_article_type_get_example_article_data($values['article_path']);
  $styles_inputs = array();
  
  foreach ($data['components'] as $index => $component) {
    $value_name = 'component_' . $index;
    
    if (isset($values[$value_name])) {
      $data['components'][$index] = publish_to_apple_news_article_type_template_example_populate_component($component, $values[$value_name], $styles_inputs);
    } elseif ($component['role'] == 'banner_advertisement' && !$values['include_ad']) {
      unset($data['components'][$index]);
    }
  }
  
  foreach ($data['componentStyles'] as $style_name => $style) {
    if (! empty($styles_inputs[$style_name]) && ! empty($style['fill'])) {
      $data['componentStyles'][$style_name]['fill']['URL'] = $styles_inputs[$style_name];
    }
    if (! empty($styles_inputs[$style_name]) && ! empty($style['backgroundColor'])) {
      $data['componentStyles'][$style_name]['backgroundColor'] = $styles_inputs[$style_name];
    }
  }
  
  $folder_path = drupal_realpath(str_replace('article.json', '', $values['article_path'])) . '/';
  publish_to_apple_news_create_article_type_from_example($form_state['values']['content_type'], $data, $folder_path);
  
  drupal_set_message(t('Article type successfully created.'));
  $form_state['redirect'] = 'admin/structure/publish-to-apple-news';
}

function publish_to_apple_news_article_type_get_example_article_component_form_section($component, $index=0, $depth = 0, $styles) {
  
  $element = array();
  $style_name = '';
  $style_form = array();
  
  if (! empty($component['style'])) {
    $style_name = $component['style'];
    $style = (! empty($styles[$style_name])) ? $styles[$style_name] : array();
    
    if (! empty($style) && ! empty($style['fill'])) {
      $title = t('Select node image field using replacement patterns (e.g. [node:field_image])');
      
      $style_form = array(
        '#type' => 'textfield',
        '#title' => t('Background Image'),
        '#maxlength' => 255,
        '#attributes' => array(
          'class' => array(
            'component-field',
            'component-depth-' . drupal_html_class($depth+1),
          ),
          'title' => $title,
        ),
        '#weight' => -1,
        '#suffix' => '<a href="#ui-replacement-patterns" class="replacement-patterns-link" rel="modal:open">Replacement patterns</a><span class="tooltip" title=\'' . $title . '\'>?</span>',
      );
    }
    
    if (! empty($style) && ! empty($style['backgroundColor'])) {
      
      $style_form = array(
        '#type' => 'textfield',
        '#title' => t('Background Color'),
        '#maxlength' => 255,
        '#attributes' => array(
          'class' => array(
            'component-field',
            'component-depth-' . drupal_html_class($depth+1),
          ),
        ),
        '#default_value' => $style['backgroundColor'],
        '#weight' => -1,
        '#suffix' => '<a href="#ui-replacement-patterns" class="replacement-patterns-link" rel="modal:open">Replacement patterns</a>',
      );
    }
  }
  
  if (! empty($component['components']) || ! empty($component['items']) || ! empty($style_form)) {
    $role_title = ucwords($component['role']);
    
    if (! empty($component['identifier'])) {
      $role_title .= " (" . $component['identifier'] . ")";
    }
    
    $element = array(
      '#type' => 'container',
      '#attributes' => array(
        'class' => array(
          'component-depth-' . drupal_html_class($depth),
          'container-depth',
        ),
      ),
      '#tree' => TRUE,
      '#weight' => $index,
      '#prefix' => '<div class="component-container"><h2>' . t($role_title) . '</h2>',
      '#suffix' => '</div>',
    );
    
    if (!empty($style_form)) {
      $element[$style_name] = $style_form;
    }
    
    if (empty($component['components'])) {
      $component['components'] = array();
    }
    
    foreach ($component['components'] as $index => $component) {
      $component_form_section = publish_to_apple_news_article_type_get_example_article_component_form_section($component, $index, $depth+1, $styles);
      if (! empty($component_form_section)) {
        $element['component_' . $index] = $component_form_section;
      }
    }
    
    if (empty($component['items'])) {
      $component['items'] = array();
    }
    
    if (!empty($component['items'])) {
      $field_title = t('Enter the replacement pattern for a multi-value image field. Images will be automatically added and their titles will be used as captions.');
      $element['images_field'] = array(
        '#type' => 'textfield',
        '#title' => t('Images field'),
        '#maxlength' => 255,
        '#attributes' => array(
          'class' => array(
            'component-field',
            'component-depth-' . drupal_html_class($depth),
          ),
          'title' => $field_title,
        ),
        '#suffix' => '<a href="#ui-replacement-patterns" class="replacement-patterns-link" rel="modal:open">Replacement patterns</a><span class="tooltip" title=\'' . $field_title . '\'>?</span>',        
      );
    }
    
  }
  elseif (! empty($component['text']) || ! empty($component['URL'])) {
    $placeholder = ! empty($component['text']) ? str_replace("\n", " ", $component['text']) : '';
    $placeholder = ! empty($component['URL']) ? str_replace('bundle://', '', $component['URL']) : $placeholder;
    $placeholder = strlen($placeholder) > 32 ? substr($placeholder, 0, 32) . '...' : $placeholder;
    $title = t("A component that corresponds to the \n\"") . $placeholder . t("\"\n part of the example.");
    $default_value = "";
    if ($component['role'] === 'title') {
      $default_value = '[node:title]';
    }
    if (! empty($component['identifier']) && $component['identifier'] === 'body1') {
      $default_value = '[node:body]';
    }
    
    $role_title = ucwords($component['role']);
    
    if ($role_title == 'Pullquote') {
      $role_title = 'Pull quote';
    }
    
    $element = array(
      '#type' => 'textfield',
      '#title' => t($role_title),
      '#maxlength' => 255,
      '#attributes' => array(
        'class' => array(
          'component-field',
          'component-depth-' . drupal_html_class($depth),
          'component-tooltip',
        ),
        'title' => $title,
        'placeholder' => $placeholder,
      ),
      '#default_value' => $default_value,
      '#weight' => $index,
      '#suffix' => '<a href="#ui-replacement-patterns" class="replacement-patterns-link" rel="modal:open">Replacement patterns</a><span class="tooltip" title=\'' . $title . '\'>?</span>',
    );
  }
  
  return $element;
}

function publish_to_apple_news_article_type_get_example_article_components($path) {
  $data = publish_to_apple_news_article_type_get_example_article_data($path);
  return $data['components'];
}

function publish_to_apple_news_article_type_get_example_article_data($path) {
  $filename = $path;
  $handle = fopen($filename, "r");
  $contents = fread($handle, filesize($filename));
  fclose($handle);
  $data = json_decode($contents, TRUE);
  return $data;
}

function publish_to_apple_news_article_type_template_example_populate_component($component, $value, &$styles_inputs) {
  
  if (! empty($component['style']) && ! empty($value[$component['style']])) {
    $styles_inputs[$component['style']] = $value[$component['style']];
  }
  
  if (! is_array($value)) {
    if (! empty($component['text'])) {
      $component['text'] = $value;
    }
    if (! empty($component['URL'])) {
      $component['URL'] = $value;
    }
    return $component;
  }
  
  if (! empty($component['components'])) {
    foreach ($component['components'] as $index => $child_component) {
      $value_name = 'component_' . $index;
      if (isset($value[$value_name])) {
        $component['components'][$index] = publish_to_apple_news_article_type_template_example_populate_component($child_component, $value[$value_name], $styles_inputs);
      }
    }
  }
  
  if (! empty($value['images_field'])) {
    $component['images_field'] = $value['images_field'];
  }
  
  return $component;
}

/**
 * Form that allows user to download a full article to preview as a zip file.
 */
function publish_to_apple_news_download_preview_form($form, &$form_state, $article_type) {
  $form_state['article_type'] = $article_type;
  
  $form['description'] = array(
    '#markup' => '<p>Below you can generate an Apple News article for download. Drag the zip file into the News Preview app to see what it will look like in Apple News.</p>',
  );
  
  $form['node'] = array(
    '#type' => 'textfield',
    '#title' => t('Select a node'),
    '#description' => t('Select a node to generate the Apple News article with. Start typing the node\'s title and click a result that appears.'),
    '#autocomplete_path' => 'publish-to-apple-news/node-autocomplete/' . $article_type->content_type,
  );
  
  $form['zip'] = array(
    '#type' => 'radios',
    '#title' => t('Generate full article?'),
    '#options' => array(
      1 => t('Yes - zip file containing article.json and all resources'),
      0 => t('No - article.json only'),
    ),
    '#default_value' => TRUE,
  );
  
  $form['download'] = array(
    '#type' => 'submit',
    '#value' => t('Download'),
  );
  
  return $form;
}

function publish_to_apple_news_download_preview_form_validate($form, &$form_state) {
  if (!_publish_to_apple_news_autocomplete_get_nid($form_state['values']['node'])) {
    form_set_error('node', t('The node you entered does not exist.'));
  }
}

/**
 * Submit handler for preview form.
 * Generate either a complete zip file or just article.json and trigger download.
 */
function publish_to_apple_news_download_preview_form_submit($form, $form_state) {
  module_load_include('inc', 'publish_to_apple_news', 'includes/generators');
  $nid = _publish_to_apple_news_autocomplete_get_nid($form_state['values']['node']);
  $node = node_load($nid);
  $article = publish_to_apple_news_generate_article($node);
  $files_dir = drupal_realpath(variable_get('file_public_path', conf_path() . '/files'));
  $publish_to_apple_news_dir = $files_dir . '/publish-to-apple-news';
  $article_folder = 'publish-to-apple-news/' . _publish_to_apple_news_string_to_slug($node->title);
  $article_folder_path = $files_dir . '/' . $article_folder;
  $article_folder_uri = 'public://' . $article_folder;
  $article_json = str_replace('\/', '/', json_encode($article['article']));
  $zip_filename = _publish_to_apple_news_string_to_slug($node->title) . '.zip';
  $zip_path = $publish_to_apple_news_dir . '/' . $zip_filename;
  
  if (!file_exists($publish_to_apple_news_dir))
    drupal_mkdir($publish_to_apple_news_dir);
  
  if (file_exists($article_folder_path))
    file_unmanaged_delete_recursive($article_folder_path);
  
  drupal_mkdir($article_folder_path);
  
  file_unmanaged_save_data($article_json, $article_folder_uri . '/article.json');
  
  if ($form_state['values']['zip']) {
    foreach ($article['files'] as $article_file) {
      $article_filename = basename(urldecode($article_file));
      $article_filename = str_replace(' ', '-', $article_filename);
      
      if (substr($article_file, 0, 1) == '/') {
        file_unmanaged_copy($article_file, $article_folder_path . '/' . $article_filename);
      } else {
        $ch = curl_init();
        curl_setopt($ch, CURLOPT_URL, $article_file);
        curl_setopt($ch, CURLOPT_FOLLOWLOCATION, 1);
        curl_setopt($ch, CURLOPT_RETURNTRANSFER, 1);
        curl_setopt($ch, CURLOPT_BINARYTRANSFER, 1);
        $data = curl_exec($ch);
        curl_close($ch);
        file_unmanaged_save_data($data, $article_folder_uri . '/' . $article_filename);
      }
    }
    
    $zip = new ZipArchive();
    $zip->open($zip_path, ZipArchive::CREATE | ZipArchive::OVERWRITE);
    $zip->addEmptyDir(_publish_to_apple_news_string_to_slug($node->title));
    $files = new RecursiveIteratorIterator(
      new RecursiveDirectoryIterator($article_folder_path),
      RecursiveIteratorIterator::LEAVES_ONLY
    );
    
    foreach ($files as $file) {
      if (!$file->isDir()) {
        $file_path = $file->getRealPath();
        $relative_path = _publish_to_apple_news_string_to_slug($node->title) . '/' . substr($file_path, strlen($article_folder_path) + 1);
        $zip->addFile($file_path, $relative_path);
      }
    }
    
    $zip->close();
    
    header("Content-type: application/zip"); 
    header("Content-Disposition: attachment; filename=$zip_filename");
    header("Content-length: " . filesize($zip_path));
    header("Pragma: no-cache"); 
    header("Expires: 0"); 
    readfile("$zip_path");
    
    file_unmanaged_delete_recursive($zip_path);
  } else {
    header("Content-type: application/json"); 
    header("Content-Disposition: attachment; filename=article.json");
    header("Content-length: " . filesize($article_folder_path . '/article.json'));
    header("Pragma: no-cache"); 
    header("Expires: 0"); 
    readfile($article_folder_path . '/article.json');
  }
  
  file_unmanaged_delete_recursive($article_folder_path);
  
  exit;
}

/**
 * Node autocomplete by type
 */
function _publish_to_apple_news_node_autocomplete($node_type, $string) {
  $query = new EntityFieldQuery();
  $result = $query->entityCondition('entity_type', 'node')
    ->entityCondition('bundle', $node_type)
    ->propertyCondition('title', '%' . db_like($string) . '%', 'LIKE')
    ->execute();
  
  if (isset($result['node'])) {
   $nodes = node_load_multiple(array_keys($result['node']));
   
   $matches = array();
   foreach ($nodes as $node) {
     $matches[$node->title . ' (' . $node->nid . ')'] = $node->title;
   }
   drupal_json_output($matches); 
  }
  
  exit;
}

/**
 * Given a string, return the substring between (), which should be a node id.
 */
function _publish_to_apple_news_autocomplete_get_nid($string) {
  if (empty($string))
    return FALSE;
  
  preg_match_all('/\(([A-Za-z0-9 ]+?)\)/', $string, $matches);
  
  $nid = (int)$matches[1][0];
  $node = node_load($nid);
  
  if (!$node)
    return FALSE;
  
  return $nid;
}

function _publish_to_apple_news_string_to_slug($string) {
  $string = str_replace(array('/', ' '), '-', $string);
  $accents = '/&([A-Za-z]{1,2})(tilde|grave|acute|circ|cedil|uml|lig);/';
  return urlencode(strtolower(preg_replace($accents, '$1', htmlentities(utf8_decode($string)))));
}

<?php

/**
 * @file
 * provides page callbacks for various admin pages used in this module.
 */

/**
 * Callback function for manage booklets page.
 */
function jquery_booklets_admin_page() {
  $output = "";

  $header = array(
    array(
      'data' => t('Title'),
      'field' => 'bt.booklet_title',
      'sort' => 'asc',
      'width' => '60%',
    ),
    array(
      'data' => t('Created'),
      'width' => '15%',
    ),
    array(
      'data' => t('Operations'),
      'colspan' => 4,
      'width' => '25%',
    ),
  );

  // Start constructing the individual rows.
  $rows = array();
  // Contruct the db call for the list.
  // Load all data fields and attach pager and sorter function in.
  $query = db_select('booklets', 'bt')->extend('PagerDefault')->extend('TableSort');
  $query->fields('bt')
      ->limit(20)
      ->orderByHeader($header);

  $result = $query->execute();

  // Start constructing the individual rows.
  $rows = array();
  foreach ($result as $data) {
    $rows[] = array(
      'data' =>
      array(
        $data->booklet_title,
        date('Y-m-d', $data->created),
        l(t('view'), "booklet/" . $data->bid),
        l(t('manage pages'), "admin/structure/jquery-booklets/manage_pages/" . $data->bid),
        l(t('edit'), "admin/structure/jquery-booklets/edit/" . $data->bid),
        l(t('delete'), "admin/structure/jquery-booklets/delete/" . $data->bid),
      ),
    );
  }
  // Construct the call for the theme function to run on this.
  $output['booklets'] = array(
    '#theme' => 'table',
    '#header' => $header,
    '#rows' => $rows,
    '#empty' => t('No booklets found.'),
  );

  $output['booklets_pager'] = array('#theme' => 'pager');
  return $output;
}

/**
 * Callback function for booklets settings page.
 */
function jquery_booklets_configure() {
  $content_types = node_type_get_names();
  $form['jquery_booklet_content_types'] = array(
    '#type' => 'checkboxes',
    '#options' => $content_types,
    '#title' => t('Select content types'),
    '#description' => t('Select content types to use as booklet pages. When you 
      add new pages in the booklets, only nodes of seleted content types will 
      be listed in autocomplete field.'),
    '#default_value' => variable_get('jquery_booklet_content_types', $content_types),
  );

  $entity_info = entity_get_info('node');
  $view_modes_to_skip = array('rss', 'search_index', 'search_result');

  $allowed_view_modes = array();
  foreach ($entity_info['view modes'] as $key => $value) {
    if (!in_array($key, $view_modes_to_skip)) {
      $allowed_view_modes[$key] = $value['label'];
    }
  }

  $form['jquery_booklet_view_mode'] = array(
    '#type' => 'select',
    '#title' => t('Select Node View Mode'),
    '#options' => $allowed_view_modes,
    '#default_value' => variable_get('jquery_booklet_view_mode', 'full'),
    '#description' => t('Select node display view mode for the booklet.'),
  );

  return system_settings_form($form);
}

/**
 * Callback function for add booklet page.
 */
function jquery_booklets_add_form($form, &$form_state) {
  $form['#tree'] = TRUE;
  $form['booklet_fieldset'] = array(
    '#type' => 'fieldset',
    '#title' => t('Select Booklet Pages'),
    '#description' => t('Add multiple pages to this booklet. You can later on change their order from order page.'),
    '#prefix' => '<div id="booklet-fieldset-wrapper">',
    '#suffix' => '</div>',
  );

  $form['booklet_title'] = array(
    '#type' => 'textfield',
    '#title' => t('Booklet Title'),
    '#required' => TRUE,
    '#weight' => -10,
  );

  if (empty($form_state['num_nodes'])) {
    $form_state['num_nodes'] = 1;
  }
  for ($i = 0; $i < $form_state['num_nodes']; $i++) {
    $form['booklet_fieldset']['book_page'][$i] = array(
      '#type' => 'textfield',
      '#title' => t('Booklet Page'),
      '#autocomplete_path' => 'jquery-booklet/searchpage',
    );
  }
  $form['booklet_fieldset']['add_page'] = array(
    '#type' => 'submit',
    '#value' => t('Add one more'),
    '#submit' => array('_jquery_booklets_add_more_page'),
    '#ajax' => array(
      'callback' => '_jquery_booklets_add_more_callback',
      'wrapper' => 'booklet-fieldset-wrapper',
    ),
  );
  if ($form_state['num_nodes'] > 1) {
    $form['booklet_fieldset']['remove_page'] = array(
      '#type' => 'submit',
      '#value' => t('Remove one'),
      '#submit' => array('_jquery_booklets_remove_one'),
      '#ajax' => array(
        'callback' => '_jquery_booklets_add_more_callback',
        'wrapper' => 'booklet-fieldset-wrapper',
      ),
    );
  }
  $form['front_cover'] = array(
    '#type' => 'file',
    '#title' => t('Front Cover Image'),
    '#maxlength' => 40,
    '#description' => t("Upload image for the front page cover. Please upload an image of such size which fits your front cover."),
  );
  $form['back_cover'] = array(
    '#type' => 'file',
    '#title' => t('Back Cover Image'),
    '#maxlength' => 40,
    '#description' => t("Upload image for the back page cover. Please upload an image of such size which fits your back cover."),
  );

  $form['booklet_width'] = array(
    '#type' => 'textfield',
    '#title' => t('Width'),
    '#description' => t('Enter booklet width in pixels. eg. 600.'),
    '#default_value' => 600,
    '#required' => TRUE,
    '#size' => 10,
    '#element_validate' => array('_element_validate_integer_positive'),
  );
  $form['booklet_height'] = array(
    '#type' => 'textfield',
    '#title' => t('Height'),
    '#description' => t('Enter booklet height in pixels. eg. 400.'),
    '#default_value' => 400,
    '#required' => TRUE,
    '#size' => 10,
    '#element_validate' => array('_element_validate_integer_positive'),
  );

  if (module_exists('path')) {

    $form['booklet_alias'] = array(
      '#type' => 'textfield',
      '#title' => t('Url Alias'),
      '#description' => t('Optionally specify an alternative URL by which this 
        content can be accessed. For example, type "about" when writing an about
        page. Use a relative path and don\'t add a trailing slash or the URL 
        alias won\'t work.'),
      '#required' => FALSE,
      '#size' => 60,
    );
  }

  $form['advance_setting'] = array(
    '#type' => 'fieldset',
    '#title' => t('Advance Settings'),
    '#description' => t('Select advance setting for the booklet.'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );

  $form['advance_setting']['speed'] = array(
    '#type' => 'textfield',
    '#title' => t('Speed'),
    '#description' => t('Enter speed to turn the pages. Default value is 1000'),
    '#default_value' => 1000,
    '#required' => TRUE,
    '#size' => 10,
    '#element_validate' => array('_element_validate_integer_positive'),
  );
  $form['advance_setting']['direction'] = array(
    '#type' => 'select',
    '#title' => t('Reading Direction'),
    '#description' => t('Select Reading Direction'),
    '#default_value' => 'LTR',
    '#options' => array('LTR' => t('LTR'), 'RTL' => t('RTL')),
  );
  $form['advance_setting']['page_numbers'] = array(
    '#type' => 'select',
    '#title' => t('Show Page Number'),
    '#description' => t('Select whether you want to display page number at bottom of each page'),
    '#default_value' => 'true',
    '#options' => array('false' => t('false'), 'true' => t('true')),
  );
  $form['advance_setting']['autoplay'] = array(
    '#type' => 'select',
    '#title' => t('Autoplay'),
    '#description' => t('Enable / disable autoplay mode of book '),
    '#default_value' => 'false',
    '#options' => array('false' => t('false'), 'true' => t('true')),
  );
  $form['advance_setting']['arrows'] = array(
    '#type' => 'select',
    '#title' => t('Arrows'),
    '#description' => t('Enable / disable arrows for page turning.'),
    '#default_value' => 'false',
    '#options' => array('false' => t('false'), 'true' => t('true')),
  );

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Submit'),
  );
  return $form;
}

/**
 * Callback for both ajax-enabled buttons.
 *
 * Selects and returns the fieldset with the names in it.
 */
function _jquery_booklets_add_more_callback($form, $form_state) {
  return $form['booklet_fieldset'];
}

/**
 * Submit handler for the "add-one-more" button.
 *
 * Increments the max counter and causes a rebuild.
 */
function _jquery_booklets_add_more_page($form, &$form_state) {
  $form_state['num_nodes']++;
  $form_state['rebuild'] = TRUE;
}

/**
 * Submit handler for the "remove one" button.
 *
 * Decrements the max counter and causes a form rebuild.
 */
function _jquery_booklets_remove_one($form, &$form_state) {
  if ($form_state['num_nodes'] > 1) {
    $form_state['num_nodes']--;
  }
  $form_state['rebuild'] = TRUE;
}

/**
 * Return autocomplete results for nodes.  
 */
function jquery_booklet_searchpage_autocomplete($string) {
  $content_types = node_type_get_names();
  $selected_content_types = variable_get('jquery_booklet_content_types', $content_types);
  $node_type_array = array();
  foreach ($selected_content_types as $type) {
    $node_type_array[] = $type;
  }
  $query = db_select('node  ', 'n');
  $query->fields('n', array('nid', 'title'));
  $query->condition('n.title', '%' . db_like($string) . '%', 'LIKE');
  $query->condition('n.type', $node_type_array, 'IN');
  $query->orderBy('n.title', 'ASC');
  $result = $query->execute();
  $suggestions = array();
  foreach ($result as $data) {
    $suggestions[$data->nid . "#" . $data->title] = $data->title;
  }

  drupal_json_output((object) $suggestions);
}

/**
 * Implements hook_validate().
 */
function jquery_booklets_add_form_validate($form, &$form_state) {
  if (count($form_state['values']['booklet_fieldset']['book_page']) > 0 && $form_state['values']['op'] != 'Remove one') {
    foreach ($form_state['values']['booklet_fieldset']['book_page'] as $key => $value) {
      if ($value == '') {
        form_set_error('booklet_fieldset][book_page][' . $key, t('Booklet Page can not be left empty.'));
      }
      else {
        $parsed_value = explode('#', $value);
        if (count($parsed_value) > 1) {
          $node_title = $parsed_value[1];
          $node_exists = db_select('node')
              ->fields('node', array('nid'))
              ->condition('title', $node_title)
              ->range(0, 1)
              ->execute()
              ->fetchField();
          if (!$node_exists) {
            form_set_error('booklet_fieldset][book_page][' . $key, t('Found no valid post with that title.'));
          }
        }
        else {
          form_set_error('booklet_fieldset][book_page][' . $key, t('Invalid entry.'));
        }
      }
    }
  }
  if (!empty($_FILES['files']['name']['front_cover'])) {
    $file = file_save_upload('front_cover', array(
      'file_validate_is_image' => array(),
      'file_validate_extensions' => array('png gif jpg jpeg'),
    ));

    if ($file) {
      if ($file = file_move($file, 'public://booklet_covers/')) {
        $form_state['storage']['front_cover'] = $file;
      }
      else {
        form_set_error('file', t("Failed to write the uploaded file to the site's file folder."));
      }
    }
    else {
      form_set_error('file', t('No file was uploaded.'));
    }
  }

  if (!empty($_FILES['files']['name']['back_cover'])) {
    $file = file_save_upload('back_cover', array(
      'file_validate_is_image' => array(),
      'file_validate_extensions' => array('png gif jpg jpeg'),
    ));

    if ($file) {
      if ($file = file_move($file, 'public://')) {
        $form_state['storage']['back_cover'] = $file;
      }
      else {
        form_set_error('file', t("Failed to write the uploaded file to the site's file folder."));
      }
    }
    else {
      form_set_error('file', t('No file was uploaded.'));
    }
  }
  if (module_exists('path')) {
    if (!empty($form_state['values']['booklet_alias'])) {
      $url_alias = db_select('url_alias')
          ->fields('url_alias', array('alias'))
          ->condition('alias', check_plain($form_state['values']['booklet_alias']))
          ->range(0, 1)
          ->execute()
          ->fetchField();
      if ($url_alias) {
        form_set_error('booklet_alias', t('This url alias already exists.'));
      }
    }
  }
}

/**
 * Implements hook_submit().
 */
function jquery_booklets_add_form_submit($form, &$form_state) {

  $front_cover_fid = 0;
  $back_cover_fid = 0;

  if (!empty($_FILES['files']['name']['front_cover'])) {
    $file = $form_state['storage']['front_cover'];
    unset($form_state['storage']['front_cover']);
    $file->status = FILE_STATUS_PERMANENT;
    file_save($file);
    $front_cover_fid = $file->fid;
  }
  if (!empty($_FILES['files']['name']['back_cover'])) {
    $file = $form_state['storage']['back_cover'];
    unset($form_state['storage']['back_cover']);
    $file->status = FILE_STATUS_PERMANENT;
    file_save($file);
    $back_cover_fid = $file->fid;
  }
  $settings = array();
  foreach ($form_state['values']['advance_setting'] as $setting_key => $setting_data) {
    $settings[$setting_key] = check_plain($setting_data);
  }
  $bid = db_insert('booklets')->fields(array(
        'booklet_title' => $form_state['values']['booklet_title'],
        'front_cover_fid' => $front_cover_fid,
        'back_cover_fid' => $back_cover_fid,
        'width' => $form_state['values']['booklet_width'],
        'height' => $form_state['values']['booklet_height'],
        'settings' => serialize($settings),
        'created' => time(),
      ))->execute();
  if (count($form_state['values']['booklet_fieldset']['book_page']) > 0) {
    foreach ($form_state['values']['booklet_fieldset']['book_page'] as $key => $value) {
      if ($value != '') {
        $parsed_value = explode('#', $value);
        $nid = $parsed_value[0];
        db_insert('booklet_pages')->fields(array(
          'bid' => $bid,
          'nid' => $nid,
          'weight' => $key,
        ))->execute();
      }
    }
  }

  if (module_exists('path')) {
    if (!empty($form_state['values']['booklet_alias'])) {
      $path = array();
      $path['source'] = 'booklet/' . $bid;
      $path['alias'] = check_plain($form_state['values']['booklet_alias']);
      path_save($path);
    }
  }

  drupal_set_message(t('The booklet has been successfully saved.'));
  $form_state['redirect'] = 'admin/structure/jquery-booklets/list';
}

/**
 * Callback function for manage booklet pages.
 */
function jquery_booklets_manage_pages($form, &$form_state, $bid) {
  $form['booklet_pages']['#tree'] = TRUE;

  $query = db_select('booklets', 'bt');
  $query->leftJoin('booklet_pages', 'bp', 'bt.bid = bp.bid');
  $query->leftJoin('node', 'n', 'bp.nid = n.nid');
  $query->fields('bt', array('booklet_title'));
  $query->fields('bp');
  $query->fields('n', array('title'));
  $query->condition('bt.bid', $bid);
  $query->orderBy('bp.weight', 'ASC');
  $booklets_pages = $query->execute()->fetchAll();
  $booklet_title = '';

  foreach ($booklets_pages as $row) {
    if ($row->nid) {
      $form['booklet_pages'][$row->bpid] = array(
        'page_title' => array(
          '#markup' => check_plain($row->title),
        ),
        'remove_link' => array(
          '#markup' => l(t('Remove'), 'admin/structure/jquery-booklets/manage_pages/' . $bid . '/delete/' . $row->bpid),
        ),
        'weight' => array(
          '#type' => 'weight',
          '#title' => t('Weight'),
          '#default_value' => $row->weight,
          '#delta' => 10,
          '#title-display' => 'invisible',
        ),
      );
      $booklet_title = $row->booklet_title;
    }
  }
  $form['bid'] = array(
    '#type' => 'hidden',
    '#value' => $bid,
  );
  $form['booklet_title'] = array(
    '#type' => 'hidden',
    '#value' => $booklet_title,
  );

  $form['actions']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save Changes'),
  );

  return $form;
}

/**
 * Returns HTML for a table.
 *
 * @param array $variables
 *   An associative array containing:
 *   - form: An array of form fields for reorder pages.
 */
function theme_jquery_booklets_manage_pages($variables) {
  $form = $variables['form'];

  $rows = array();

  foreach (element_children($form['booklet_pages']) as $id) {
    if ($id) {
      $form['booklet_pages'][$id]['weight']['#attributes']['class'] = array(
        'page-weight',
      );

      $rows[] = array(
        'data' => array(
          drupal_render($form['booklet_pages'][$id]['page_title']),
          drupal_render($form['booklet_pages'][$id]['remove_link']),
          drupal_render($form['booklet_pages'][$id]['weight']),
        ),
        'class' => array('draggable', 'page-row-' . $id),
      );
    }
  }

  $header = array(
    $variables['form']['booklet_title']['#value'] . t(' Pages'),
    t('Operation'),
    t('Weight'),
  );

  $table_id = 'booklet-pages-reorder-table';
  $output = '';
  $output .= theme('table', array(
    'header' => $header,
    'rows' => $rows,
    'caption' => t('Please drag and drop the pages to reorder.'),
    'empty' => t('No pages found!'),
    'attributes' => array('id' => $table_id),
      )
  );

  $output .= drupal_render_children($form);

  drupal_add_tabledrag($table_id, 'order', 'sibling', 'page-weight');

  return $output;
}

/**
 * Implements hook_submit().
 */
function jquery_booklets_manage_pages_submit($form, &$form_state) {
  foreach ($form_state['values']['booklet_pages'] as $bpid => $item) {
    db_update('booklet_pages')
        ->fields(array(
          'weight' => $item['weight'],
        ))
        ->condition('bpid', $bpid)
        ->execute();
  }
  drupal_set_message(t('The pages order has been successfully saved.'));
}

/**
 * Callback function for booklet edit page.
 */
function jquery_booklets_edit_form($form, &$form_state, $bid) {

  $booklet_data = db_select('booklets', 'bt')
      ->fields('bt')
      ->condition('bt.bid', $bid)
      ->execute()
      ->fetchObject();

  $query = db_select('booklet_pages', 'bp');
  $query->leftJoin('node', 'n', 'bp.nid = n.nid');
  $query->fields('n', array('title'));
  $query->fields('bp');
  $query->condition('bp.bid', $bid);
  $query->orderBy('bp.weight', 'ASC');
  $booklet_pages_result = $query->execute();
  $total_pages = $booklet_pages_result->rowCount();
  $booklets_pages = $booklet_pages_result->fetchAll();

  $form['#tree'] = TRUE;
  $form['booklet_fieldset'] = array(
    '#type' => 'fieldset',
    '#title' => t('Select Booklet Pages'),
    '#description' => t('Add multiple pages to this booklet. You can later on change their order from order page.'),
    '#prefix' => '<div id="booklet-fieldset-wrapper">',
    '#suffix' => '</div>',
  );

  $form['booklet_title'] = array(
    '#type' => 'textfield',
    '#title' => t('Booklet Title'),
    '#required' => TRUE,
    '#weight' => -10,
    '#default_value' => $booklet_data->booklet_title,
  );

  if (empty($form_state['num_nodes'])) {
    if ($total_pages) {
      $form_state['num_nodes'] = $total_pages;
    }
    else {
      $form_state['num_nodes'] = 1;
    }
  }

  for ($i = 0; $i < $form_state['num_nodes']; $i++) {
    $form['booklet_fieldset']['book_page'][$i] = array(
      '#type' => 'textfield',
      '#title' => t('Booklet Page'),
      '#autocomplete_path' => 'jquery-booklet/searchpage',
      '#default_value' => (array_key_exists($i, $booklets_pages)) ? $booklets_pages[$i]->nid . "#" . $booklets_pages[$i]->title : '',
    );
  }

  $form['booklet_fieldset']['add_page'] = array(
    '#type' => 'submit',
    '#value' => t('Add one more'),
    '#submit' => array('_jquery_booklets_add_more_page'),
    '#ajax' => array(
      'callback' => '_jquery_booklets_add_more_callback',
      'wrapper' => 'booklet-fieldset-wrapper',
    ),
  );
  if ($form_state['num_nodes'] > 1) {
    $form['booklet_fieldset']['remove_page'] = array(
      '#type' => 'submit',
      '#value' => t('Remove one'),
      '#submit' => array('_jquery_booklets_remove_one'),
      '#ajax' => array(
        'callback' => '_jquery_booklets_add_more_callback',
        'wrapper' => 'booklet-fieldset-wrapper',
      ),
    );
  }
  $form['front_cover_fieldset'] = array(
    '#type' => 'fieldset',
    '#title' => t('Front Cover'),
  );

  if ($booklet_data->front_cover_fid) {
    $front_cover = file_load($booklet_data->front_cover_fid);
    $form['front_cover_fieldset']['front_cover_picture'] = array(
      '#type' => 'value',
      '#value' => $front_cover,
    );
    $form['front_cover_fieldset']['front_cover_current'] = array(
      '#markup' => theme('image_style', array('style_name' => 'thumbnail', 'path' => $front_cover->uri)),
    );
    $form['front_cover_fieldset']['front_cover_delete'] = array(
      '#type' => 'checkbox',
      '#title' => t('Delete picture'),
      '#access' => !empty($booklet_data->front_cover_fid),
      '#description' => t('Check this box to delete current front cover.'),
    );
    $form['front_cover_fieldset']['current_front_cover_fid'] = array(
      '#type' => 'hidden',
      '#value' => $booklet_data->front_cover_fid,
    );
  }

  $form['front_cover_fieldset']['front_cover'] = array(
    '#type' => 'file',
    '#title' => t('Front Cover Image'),
    '#maxlength' => 40,
    '#description' => t("Upload image for the front page cover. Please upload an image of such size which fits your front cover."),
  );

  $form['back_cover_fieldset'] = array(
    '#type' => 'fieldset',
    '#title' => t('Back Cover'),
  );
  if ($booklet_data->back_cover_fid) {
    $back_cover = file_load($booklet_data->back_cover_fid);
    $form['back_cover_fieldset']['back_cover_picture'] = array(
      '#type' => 'value',
      '#value' => $back_cover,
    );
    $form['back_cover_fieldset']['back_cover_current'] = array(
      '#markup' => theme('image_style', array('style_name' => 'thumbnail', 'path' => $back_cover->uri)),
    );
    $form['back_cover_fieldset']['back_cover_delete'] = array(
      '#type' => 'checkbox',
      '#title' => t('Delete picture'),
      '#access' => !empty($booklet_data->back_cover_fid),
      '#description' => t('Check this box to delete current back cover.'),
    );
    $form['front_cover_fieldset']['current_back_cover_fid'] = array(
      '#type' => 'hidden',
      '#value' => $booklet_data->back_cover_fid,
    );
  }

  $form['back_cover_fieldset']['back_cover'] = array(
    '#type' => 'file',
    '#title' => t('Back Cover Image'),
    '#maxlength' => 40,
    '#description' => t("Upload image for the back page cover. Please upload an image of such size which fits your back cover."),
  );
  $form['booklet_width'] = array(
    '#type' => 'textfield',
    '#title' => t('Width'),
    '#description' => t('Enter booklet width in pixels. eg. 600.'),
    '#default_value' => $booklet_data->width,
    '#required' => TRUE,
    '#size' => 10,
    '#element_validate' => array('_element_validate_integer_positive'),
  );
  $form['booklet_height'] = array(
    '#type' => 'textfield',
    '#title' => t('Height'),
    '#description' => t('Enter booklet height in pixels. eg. 400.'),
    '#default_value' => $booklet_data->height,
    '#required' => TRUE,
    '#size' => 10,
    '#element_validate' => array('_element_validate_integer_positive'),
  );

  if (module_exists('path')) {
    $url_alias = db_select('url_alias')
        ->fields('url_alias', array('alias'))
        ->condition('source', 'booklet/' . $bid)
        ->range(0, 1)
        ->execute()
        ->fetchField();

    $form['booklet_alias'] = array(
      '#type' => 'textfield',
      '#title' => t('Url Alias'),
      '#description' => t('Optionally specify an alternative URL by which this 
        content can be accessed. For example, type "about" when writing an about
        page. Use a relative path and don\'t add a trailing slash or the URL 
        alias won\'t work.'),
      '#default_value' => isset($url_alias) ? $url_alias : '',
      '#required' => FALSE,
      '#size' => 60,
    );
  }

  $form['advance_setting'] = array(
    '#type' => 'fieldset',
    '#title' => t('Advance Settings'),
    '#description' => t('Select advance setting for the booklet.'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );
  $settings = unserialize($booklet_data->settings);
  $form['advance_setting']['speed'] = array(
    '#type' => 'textfield',
    '#title' => t('Speed'),
    '#description' => t('Enter speed to turn the pages. Default value is 1000'),
    '#default_value' => isset($settings['speed']) ? $settings['speed'] : 1000,
    '#required' => TRUE,
    '#size' => 10,
    '#element_validate' => array('_element_validate_integer_positive'),
  );
  $form['advance_setting']['direction'] = array(
    '#type' => 'select',
    '#title' => t('Reading Direction'),
    '#description' => t('Select Reading Direction'),
    '#default_value' => isset($settings['direction']) ? $settings['direction'] : 'LTR',
    '#options' => array('LTR' => t('LTR'), 'RTL' => t('RTL')),
  );
  $form['advance_setting']['page_numbers'] = array(
    '#type' => 'select',
    '#title' => t('Show Page Number'),
    '#description' => t('Select whether you want to display page number at bottom of each page'),
    '#default_value' => isset($settings['page_numbers']) ? $settings['page_numbers'] : 'true',
    '#options' => array('false' => t('false'), 'true' => t('true')),
  );
  $form['advance_setting']['autoplay'] = array(
    '#type' => 'select',
    '#title' => t('Autoplay'),
    '#description' => t('Enable / disable autoplay mode of book '),
    '#default_value' => isset($settings['autoplay']) ? $settings['autoplay'] : 'false',
    '#options' => array('false' => t('false'), 'true' => t('true')),
  );
  $form['advance_setting']['arrows'] = array(
    '#type' => 'select',
    '#title' => t('Arrows'),
    '#description' => t('Enable / disable arrows for page turning.'),
    '#default_value' => isset($settings['arrows']) ? $settings['arrows'] : 'false',
    '#options' => array('false' => t('false'), 'true' => t('true')),
  );

  $form['bid'] = array(
    '#type' => 'hidden',
    '#value' => $bid,
  );
  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Submit'),
  );
  return $form;
}

/**
 * Implements hook_validate().
 */
function jquery_booklets_edit_form_validate($form, &$form_state) {

  if (count($form_state['values']['booklet_fieldset']['book_page']) > 0 && $form_state['values']['op'] != 'Remove one') {
    foreach ($form_state['values']['booklet_fieldset']['book_page'] as $key => $value) {
      if ($value == '') {
        form_set_error('booklet_fieldset][book_page][' . $key, t('Booklet Page can not be left empty.'));
      }
      else {
        $parsed_value = explode('#', $value);
        if (isset($parsed_value[1])) {
          $node_title = $parsed_value[1];
        }
        else {
          $node_title = $value;
        }

        $node_exists = db_select('node')
            ->fields('node', array('nid'))
            ->condition('title', $node_title)
            ->range(0, 1)
            ->execute()
            ->fetchField();
        if (!$node_exists) {
          form_set_error('booklet_fieldset][book_page][' . $key, t('Found no valid post with that title.'));
        }
      }
    }
  }

  if (!empty($_FILES['files']['name']['front_cover_fieldset'])) {
    $file = file_save_upload('front_cover_fieldset', array(
      'file_validate_is_image' => array(),
      'file_validate_extensions' => array('png gif jpg jpeg'),
    ));

    if ($file) {
      if ($file = file_move($file, 'public://')) {
        $form_state['storage']['front_cover'] = $file;
      }
      else {
        form_set_error('file', t("Failed to write the uploaded file to the site's file folder."));
      }
    }
    else {
      form_set_error('file', t('No file was uploaded.'));
    }
  }

  if (!empty($_FILES['files']['name']['back_cover_fieldset'])) {
    $file = file_save_upload('back_cover_fieldset', array(
      'file_validate_is_image' => array(),
      'file_validate_extensions' => array('png gif jpg jpeg'),
    ));

    if ($file) {
      if ($file = file_move($file, 'public://')) {
        $form_state['storage']['back_cover'] = $file;
      }
      else {
        form_set_error('file', t("Failed to write the uploaded file to the site's file folder."));
      }
    }
    else {
      form_set_error('file', t('No file was uploaded.'));
    }
  }

  if (module_exists('path')) {
    if (!empty($form_state['values']['booklet_alias'])) {
      $url_alias = db_select('url_alias')
          ->fields('url_alias', array('alias'))
          ->condition('alias', check_plain($form_state['values']['booklet_alias']))
          ->condition('source', 'booklet/' . $form_state['values']['bid'], '<>')
          ->range(0, 1)
          ->execute()
          ->fetchField();
      if ($url_alias) {
        form_set_error('booklet_alias', t('This url alias already exists.'));
      }
    }
  }
}

/**
 * Implements hook_submit().
 */
function jquery_booklets_edit_form_submit($form, &$form_state) {

  if (isset($form_state['values']['front_cover_fieldset']['front_cover_picture']) && is_object($form_state['values']['front_cover_fieldset']['front_cover_picture'])) {
    $front_cover_fid = $form_state['values']['front_cover_fieldset']['front_cover_picture']->fid;
  }
  else {
    $front_cover_fid = 0;
  }
  if (isset($form_state['values']['back_cover_fieldset']['back_cover_picture']) && is_object($form_state['values']['back_cover_fieldset']['back_cover_picture'])) {
    $back_cover_fid = $form_state['values']['back_cover_fieldset']['back_cover_picture']->fid;
  }
  else {
    $back_cover_fid = 0;
  }
  if (isset($form_state['values']['front_cover_fieldset']['front_cover_delete']) &&
      $form_state['values']['front_cover_fieldset']['front_cover_delete']) {
    file_delete($form_state['values']['front_cover_fieldset']['front_cover_picture']);
    $front_cover_fid = 0;
  }
  if (isset($form_state['values']['back_cover_fieldset']['back_cover_delete']) &&
      $form_state['values']['back_cover_fieldset']['back_cover_delete']) {
    file_delete($form_state['values']['back_cover_fieldset']['back_cover_picture']);
    $back_cover_fid = 0;
  }

  if (!empty($_FILES['files']['name']['front_cover_fieldset'])) {
    $file = $form_state['storage']['front_cover'];
    unset($form_state['storage']['front_cover']);
    $file->status = FILE_STATUS_PERMANENT;
    file_save($file);
    $front_cover_fid = $file->fid;
  }
  if (!empty($_FILES['files']['name']['back_cover_fieldset'])) {
    $file = $form_state['storage']['back_cover'];
    unset($form_state['storage']['back_cover']);
    $file->status = FILE_STATUS_PERMANENT;
    file_save($file);
    $back_cover_fid = $file->fid;
  }

  $bid = $form_state['values']['bid'];
  $settings = array();
  foreach ($form_state['values']['advance_setting'] as $setting_key => $setting_data) {
    $settings[$setting_key] = check_plain($setting_data);
  }
  db_update('booklets')->fields(array(
        'booklet_title' => $form_state['values']['booklet_title'],
        'front_cover_fid' => $front_cover_fid,
        'back_cover_fid' => $back_cover_fid,
        'width' => $form_state['values']['booklet_width'],
        'height' => $form_state['values']['booklet_height'],
        'settings' => serialize($settings),
      ))
      ->condition('bid', $bid)
      ->execute();

  db_delete('booklet_pages')
      ->condition('bid', $bid)
      ->execute();
  if (count($form_state['values']['booklet_fieldset']['book_page']) > 0) {
    foreach ($form_state['values']['booklet_fieldset']['book_page'] as $key => $value) {
      if ($value != '') {
        $parsed_value = explode('#', $value);
        $nid = $parsed_value[0];
        db_insert('booklet_pages')->fields(array(
          'bid' => $bid,
          'nid' => $nid,
          'weight' => $key,
        ))->execute();
      }
    }
  }
  if (module_exists('path')) {
    if (!empty($form_state['values']['booklet_alias'])) {
      $pid = db_select('url_alias')
          ->fields('url_alias', array('pid'))
          ->condition('source', 'booklet/' . $bid)
          ->range(0, 1)
          ->execute()
          ->fetchField();
      if ($pid) {
        $path = array();
        $path['pid'] = $pid;
        $path['source'] = 'booklet/' . $bid;
        $path['alias'] = check_plain($form_state['values']['booklet_alias']);
        path_save($path);
      }
      else {
        $path = array();
        $path['source'] = 'booklet/' . $bid;
        $path['alias'] = check_plain($form_state['values']['booklet_alias']);
        path_save($path);
      }
    }
    else {
      path_delete(array('source' => 'booklet/' . $bid));
    }
  }
  drupal_set_message(t('The booklet has been successfully saved.'));
  $form_state['redirect'] = 'admin/structure/jquery-booklets/list';
}

/**
 * Callback function for delete booklet page.
 */
function jquery_booklets_delete_confirm($form, &$form_state, $bid) {

  $booklet_data = db_select('booklets', 'bt')
      ->fields('bt', array('booklet_title'))
      ->condition('bt.bid', $bid)
      ->execute()
      ->fetchObject();

  $form['bid'] = array('#type' => 'hidden', '#value' => $bid);
  return confirm_form($form, t('Are you sure you want to delete %title?', array('%title' => $booklet_data->booklet_title)), 'admin/structure/jquery-booklets/list', t('This action cannot be undone.'), t('Delete'), t('Cancel')
  );
}

/**
 * Implements hook_submit().
 */
function jquery_booklets_delete_confirm_submit($form, &$form_state) {
  if ($form_state['values']['confirm']) {
    $bid = $form_state['values']['bid'];

    db_delete('booklets')
        ->condition('bid', $bid)
        ->execute();

    db_delete('booklet_pages')
        ->condition('bid', $bid)
        ->execute();

    if (module_exists('path')) {
      path_delete(array('source' => 'booklet/' . $bid));
    }

    drupal_set_message(t('The booklet has been successfully deleted.'));
    $form_state['redirect'] = 'admin/structure/jquery-booklets/list';
  }
}

/**
 * Callback function for remove booklet pages.
 */
function jquery_booklets_remove_pages($form, &$form_state, $bid, $bpid) {
  $form_state['bpid'] = $bpid;
  $form_state['bid'] = $bid;
  $booklet_title = _booklet_page_title($bid);
  return confirm_form($form, t('Are you sure you want to remove this page from 
    @booklet_title?', array('@booklet_title' => $booklet_title)), 'admin/structure/jquery-booklets/manage_pages/' . $bid);
}

/**
 * Implements hook_submit().
 */
function jquery_booklets_remove_pages_submit($form, &$form_state) {
  db_delete('booklet_pages')
      ->condition('bid', $form_state['bid'])
      ->condition('bpid', $form_state['bpid'])
      ->execute();

  drupal_set_message(t('The page has been successfully removed.'));
  $form_state['redirect'] = 'admin/structure/jquery-booklets/manage_pages/' . $form_state['bid'];
}

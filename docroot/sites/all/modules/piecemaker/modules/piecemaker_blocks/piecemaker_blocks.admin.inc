<?php
/**
 * @file
 * 
 */

/**
 * Form builder for the add block form.
 *
 * @see piecemaker_add_block_form_validate()
 * @see piecemaker_add_block_form_submit()
 * @ingroup forms
 */
function piecemaker_add_block_form($form, &$form_state, $delta = 0) {
  if (!empty($form_state['submitted']) && !empty($form_state['rebuild'])) {
    unset($form_state['values']['file']['add'], $form_state['input']['file']['add']);
  }
  $form_state_clone = &drupal_static('piecemaker_blocks_block_form');  
  $form_state_clone = $form_state;
  module_load_include('inc', 'block', 'block.admin');
  return block_admin_configure($form, $form_state, 'piecemaker_blocks', $delta);

}

/**
 * Form validation handler for the add block form.
 *
 * @see piecemaker_add_block_form()
 * @see piecemaker_add_block_form_submit()
 */
function piecemaker_add_block_form_validate($form, &$form_state) {
  $form_state_clone = &drupal_static('piecemaker_blocks_block_form');  
  $form_state_clone = $form_state;
  $custom_block_exists = (bool) db_query_range('SELECT 1 FROM {piecemaker_blocks} WHERE info = :info AND bid != :bid', 0, 1, array(':info' => $form_state['values']['info'], ':bid' => $form_state['values']['delta']))->fetchField();

  if (empty($form_state['values']['info']) || $custom_block_exists) {
    form_set_error('info', t('Ensure that each block description is unique.'));
  }  
}

/**
 * Form submission handler for the add block form.
 *
 * Saves the new custom block.
 *
 * @see piecemaker_add_block_form()
 * @see piecemaker_add_block_form_validate()
 */
function piecemaker_add_block_form_submit($form, &$form_state) {
  $form_state_clone = &drupal_static('piecemaker_blocks_block_form');
  if (!empty($form_state['values']['delta'])) {
    module_load_include('inc', 'block', 'block.admin');
    block_admin_configure_submit($form, &$form_state);
  }
  else {
    piecemaker_add_new_block_form_submit($form, &$form_state);
  }
  $form_state_clone = $form_state;
}
/**
 * Form callback for adding a new block
 */
function piecemaker_add_new_block_form_submit($form, &$form_state) {

  $query = db_insert('piecemaker_blocks');
  $delta = $query
    ->fields(array(
      'info' => $form_state['values']['info'],
      'pid' => $form_state['values']['pid'],
    ))
    ->execute();

  $files = $form_state['values']['file'];
  unset($files['add']);
  uasort($files, 'drupal_sort_weight');
  $i = 0;
  foreach ($files as $weight => $file) {
    $values = array(
      'fid' => $file['media']['fid'],
      'bid' => $delta,
      'weight' => $i,
      'data' => $file['options'],
      'type' => $file['type'],
    );
    $bfid = drupal_write_record('piecemaker_blocks_files', $values);
    $i++;
  }
  
  // Store block delta to allow other modules to work with new block.
  $form_state['values']['delta'] = $delta;
  
  $query = db_insert('block')->fields(array('visibility', 'pages', 'custom', 'title', 'module', 'theme', 'status', 'weight', 'delta', 'cache'));
  
  foreach (list_themes() as $key => $theme) {
    if ($theme->status) {
      $query->values(array(
        'visibility' => (int) $form_state['values']['visibility'],
        'pages' => trim($form_state['values']['pages']),
        'custom' => (int) $form_state['values']['custom'],
        'title' => $form_state['values']['title'],
        'module' => $form_state['values']['module'],
        'theme' => $theme->name,
        'status' => 0,
        'weight' => 0,
        'delta' => $delta,
        'cache' => DRUPAL_NO_CACHE,
      ));
    }
  }
  $query->execute();

  $query = db_insert('block_role')->fields(array('rid', 'module', 'delta'));
  foreach (array_filter($form_state['values']['roles']) as $rid) {
    $query->values(array(
      'rid' => $rid,
      'module' => $form_state['values']['module'],
      'delta' => $delta,
    ));
  }
  $query->execute();

  // Store regions per theme for this block
  foreach ($form_state['values']['regions'] as $theme => $region) {
    db_merge('block')
      ->key(array('theme' => $theme, 'delta' => $delta, 'module' => $form_state['values']['module']))
      ->fields(array(
        'region' => ($region == BLOCK_REGION_NONE ? '' : $region),
        'pages' => trim($form_state['values']['pages']),
        'status' => (int) ($region != BLOCK_REGION_NONE),
      ))
      ->execute();
  }

  drupal_set_message(t('The block has been created.'));
  cache_clear_all();
  $form_state['redirect'] = 'admin/structure/block';
}

/**
 * Form Handler for saving existing blocks
 */
function piecemaker_save_block_form_submit($delta, $edit) {
  $query = db_update('piecemaker_blocks');
  $query->condition('bid', $delta);

  $result = $query
    ->fields(array(
      'info' => $edit['info'],
      'pid' => $edit['pid'],
    ))
    ->execute();
  $num_deleted = db_delete('piecemaker_blocks_files')
    ->condition('bid', $delta)
    ->execute();
  $files = $edit['file'];
  unset($files['add']);
  uasort($files, 'drupal_sort_weight');
  $i = 0;
  foreach ($files as $weight => $file) {
    $values = array(
      'fid' => $file['media']['fid'],
      'bid' => $delta,
      'weight' => $i,
      'data' => $file['options'],
      'type' => $file['type'],
    );
    $bfid = drupal_write_record('piecemaker_blocks_files', $values);
    $i++;
  }
}

/**
 * Define the custom block form.
 */
function piecemaker_blocks_block_form($edit = array()) {
  $form_state = &drupal_static(__FUNCTION__);
  $edit += array(
    'info' => '',
    'files' => array(),
    'pid' => '',
  );
  $form['#attached']['css'][] = drupal_get_path('module', 'piecemaker_blocks') . '/css/admin.css';
  
  $form['info'] = array(
    '#type' => 'textfield',
    '#title' => t('Block description'),
    '#default_value' => $edit['info'],
    '#maxlength' => 64,
    '#description' => t('A brief description of your block. Used on the <a href="@overview">Blocks administration page</a>.', array('@overview' => url('admin/structure/block'))),
    '#required' => TRUE,
    '#weight' => -19,
  );
  $form['pid'] = array(
    '#type' => 'select',
    '#title' => t('Piecemaker Profile'),
    '#options' => piecemaker_profile_options(),
    '#default_value' => array($edit['pid']),
    '#required' => TRUE,
  );
  $form['file'] = array(
    '#type' => 'fieldset',
    '#title' => t('Piecemaker Files'),
    '#theme' => 'piecemaker_blocks_files',
    '#tree' => TRUE,
    '#prefix' => '<div id="files-table">',
    '#suffix' => '</div>',
  );
  $types = array(
    'Image' => 'Image',
    'Video' => 'Video',
    'Flash' => 'Flash',
  );
  $weights = range(0, 50);
  foreach (image_styles() as $stid => $style) {
    $image_styles[$stid] = $style['name'];
  }
  if (isset($form_state['blocks_files'])) {
    $edit['files'] = $form_state['blocks_files'];
    drupal_set_message('Files and file order are not saved until you save the block.');
  }
  $i = 1;
  foreach($edit['files'] as $key => $file) {
    if (!empty($file['bfid'])) {
      $form['file'][$key]['bfid'] = array(
        '#type' => 'value',
        '#value' => $file['bfid'],
      );
    }
    $form['file'][$key]['type'] = array(
      '#type' => 'value',
      '#value' => $file['type'],
    );
    $form['file'][$key]['type']['markup']['#markup'] = $file['type'];
    $form['file'][$key]['weight'] = array(
      '#type' => 'select',
      '#options' => $weights,
      '#default_value' => array($key),
      '#attributes' => array('class' => array('file-weight')),
    );
    $media_file = file_load($file['media']['fid']);
    $form['file'][$key]['media'] = array(
      '#markup' => theme('media_admin_thumbnail', array('file' => $media_file, 'style' => 'thumbnail')),
    );
    $form['file'][$key]['media']['fid'] = array(
      '#type' => 'value',
      '#value' => $file['media']['fid'],
    );
    $opts = array();
    $opts = $file['options'];
    $form['file'][$key]['options']['Title'] = array(
      '#type' => 'value',
      '#value' => empty($opts['Title']) ? '' : $opts['Title'],
    );
    $form['file'][$key]['options']['Title']['markup']['#markup'] = empty($opts['Title']) ? 'No Title Provided' : $opts['Title'];
    $form['file'][$key]['options'][$file['type']] = array(
      '#type' => 'item',
      '#title' => t($file['type'] . ' Options'),
      '#prefix' => '<div class="piecemaker-opt-wrap">',
      '#suffix' => '</div>',
    );
    if ($file['type'] == 'Image') {
      $form['file'][$key]['options']['Image']['style'] = array(
        '#type' => 'value',
        '#value' => $opts['Image']['style'],
      );
      $form['file'][$key]['options']['Image']['style']['markup'] = array(
        '#type' => 'item',
        '#title' => t('Image Style'),
        '#markup' => $image_styles[$opts['Image']['style']]
      );
      $form['file'][$key]['options']['Image']['Text'] = array(
        '#type' => 'value',
        '#value' => empty($opts['Image']['Text']) ? '' : $opts['Image']['Text'],
      );
      $form['file'][$key]['options']['Image']['Text']['markup'] = array(
        '#type' => 'item',
        '#title' => t('Image Text'),
        '#markup' => check_markup($opts['Image']['Text']['value'], $opts['Image']['Text']['format']),
      );
      $form['file'][$key]['options']['Image']['Hyperlink'] = array(
        '#type' => 'value',
        '#value' => empty($opts['Image']['Hyperlink']) ? '' : $opts['Image']['Hyperlink'],
      );
      $target = array(
        '_self' => 'Same Window/Tab',
        '_blank' => 'New Window/Tab',  
      );
      $form['file'][$key]['options']['Image']['Target'] = array(
        '#type' => 'value',
        '#value' => $opts['Image']['Target'],
      );
      $form['file'][$key]['options']['Image']['Hyperlink']['markup'] = array(
        '#type' => 'item',
        '#title' => t('Image Hyperlink'),
        '#markup' => l($opts['Image']['Hyperlink'], $opts['Image']['Hyperlink'], array(
          'absolute' => TRUE,
          'external' => url_is_external($opts['Image']['Hyperlink']),
          'attributes' => array(
            'target' => $opts['Image']['Target'],
          ),
        )) . ' - Link opens in: ' . $target[$opts['Image']['Target']],
      );
    }
    if ($file['type'] == 'Video') {
      $form['file'][$key]['options']['Video']['Width'] = array(
        '#type' => 'value',
        '#value' => $opts['Video']['Width'],
      );
      $form['file'][$key]['options']['Video']['Width']['markup'] = array(
        '#type' => 'item',
        '#title' => t('Video Width'),
        '#markup' => $opts['Video']['Width'] . ' pixels',
      );
      $form['file'][$key]['options']['Video']['Height'] = array(
        '#type' => 'value',
        '#value' => $opts['Video']['Height'],
      );
      $form['file'][$key]['options']['Video']['Height']['markup'] = array(
        '#type' => 'item',
        '#title' => t('Video Height'),
        '#markup' => $opts['Video']['Height'] . ' pixels',
      );
      $autoplay = array('true' => 'On', 'false' => 'Off');
      $form['file'][$key]['options']['Video']['Autoplay'] = array(
        '#type' => 'value',
        '#value' => $opts['Video']['Autoplay'],
      );
      $form['file'][$key]['options']['Video']['Autoplay']['markup'] = array(
        '#type' => 'item',
        '#title' => t('Video Autoplay'),
        '#markup' => $autoplay[$opts['Video']['Autoplay']],
      );
    }
    if (in_array($file['type'], array('Video', 'Flash')) && $opts['fvimage']['fid'] > 0) {
      $bg_file = file_load($opts['fvimage']['fid']);
      $form['file'][$key]['options']['fvimage'] = array(
        '#type' => 'item',
        '#title' => t('Background Image'),
        '#markup' => theme('media_admin_thumbnail', array('file' => $bg_file, 'style' => 'thumbnail')),
      );
      $form['file'][$key]['options']['fvimage']['fid'] = array(
        '#type' => 'value',
        '#value' => $opts['fvimage']['fid'],
      );
    }
    
    $form['file'][$key]['action'] = array(
      '#type' => 'submit',
      '#value' => 'Delete File ' . $i,
      '#submit' => array('piecemaker_blocks_delete_file'),
      '#ajax' => array(
        'callback' => 'piecemaker_blocks_file_ajax',
        'wrapper' => 'files-table',
        'progress' => array(
          'type' => 'throbber',
          'message' => t('Deleting file'),  
        ),
        'method' => 'replace',
        'effect' => 'fade',
      ),  
    );
    $i++;
  }
  $form['file']['add']['type'] = array(
    '#type' => 'select',
    '#options' => $types,
    '#default_value' => array('Image'),
    '#attributes' => array('class' => array('file-type-select')),
  );
  $form['file']['add']['weight'] = array(
    '#type' => 'select',
    '#options' => $weights,
    '#default_value' => 50,
    '#attributes' => array('class' => array('file-weight')),
  );
  $form['file']['add']['media'] = array(
    '#type' => 'media',
    '#title' => '',
    '#description' => '',
    '#media_options' => array(
      'global' => array(
        'types' => array('image', 'video', 'other'),
      ),
    )
  );
  $form['file']['add']['options']['Title'] = array(
    '#type' => 'textfield',
    '#title' => t('Title'),
  );
  $form['file']['add']['options']['Image'] = array(
    '#type' => 'item',
    '#title' => t('Image Options'),
    '#states' => array(
      'visible' => array(
        'select.file-type-select' => array('value' => 'Image'),
      ),  
    ),
    '#prefix' => '<div class="piecemaker-opt-wrap">',
    '#suffix' => '</div>',
  );
  $form['file']['add']['options']['Image']['style'] = array(
    '#type' => 'select',
    '#options' => $image_styles,
    '#title' => t('Image Style'),
  );
  $form['file']['add']['options']['Image']['Text'] = array(
    '#type' => 'text_format',
    '#title' => t('Text'),
    '#cols' => 25,
    '#format' => isset($edit['format']) ? $edit['format'] : NULL,
  );
  $form['file']['add']['options']['Image']['Hyperlink'] = array(
    '#type' => 'textfield',
    '#title' => t('Hyperlink'),
  );
  $form['file']['add']['options']['Image']['Target'] = array(
    '#type' => 'select',
    '#title' => t('Hyperlink Target'),
    '#options' => array(
       '_self' => 'Same Window/Tab',
       '_blank' => 'New Window/Tab',  
     ),
  );
  $form['file']['add']['options']['Video'] = array(
    '#type' => 'item',
    '#title' => t('Video Options'),
    '#states' => array(
      'visible' => array(
        'select.file-type-select' => array('value' => 'Video'),
      ),  
    ),
    '#prefix' => '<div class="piecemaker-opt-wrap">',
    '#suffix' => '</div>',
  );
  
  $form['file']['add']['options']['Video']['Width'] = array(
    '#type' => 'textfield',
    '#size' => 10,
    '#title' => 'Width',
    '#field_suffix' => 'pixels',
  );
  
  $form['file']['add']['options']['Video']['Height'] = array(
    '#type' => 'textfield',
    '#size' => 10,
    '#title' => 'Height',
    '#field_suffix' => 'pixels',
  );
  
  $form['file']['add']['options']['Video']['Autoplay'] = array(
    '#type' => 'radios',
    '#title' => 'Autoplay',
    '#options' => array('true' => 'On', 'false' => 'Off'),
    '#default_value' => 'true',
    '#attributes' => array('class' => array('radios-left')),
  );
  $form['file']['add']['options']['Flash'] = array(
    '#type' => 'item',
    '#title' => t('Flash Options'),
    '#states' => array(
      'visible' => array(
        'select.file-type-select' => array('value' => 'Flash'),
      ),  
    ),
    '#prefix' => '<div class="piecemaker-opt-wrap">',
    '#suffix' => '</div>',
  );
  
  $form['file']['add']['options']['fvimage'] = array(
    '#type' => 'media',
    '#title' => t('Background Image'),
    '#description' => '',
    '#states' => array(
      'invisible' => array(
        'select.file-type-select' => array('value' => 'Image'),
      ),  
    ),
    '#prefix' => '<div class="fvimage">',
    '#suffix' => '</div>',
  );
  
  $form['file']['add']['action'] = array(
    '#type' => 'submit',
    '#value' => 'Add File',
    '#submit' => array('piecemaker_blocks_add_file'),
    '#validate' => array('piecemaker_blocks_add_file_validate'),
    '#ajax' => array(
      'callback' => 'piecemaker_blocks_file_ajax',
      'wrapper' => 'files-table',
      'progress' => array(
        'type' => 'throbber',
        'message' => t('Adding file'),  
      ),
      'method' => 'replace',
      'effect' => 'fade',
    ),
  );
 
  return $form;
}

/**
 * Theme Callback for piecemaker_blocks_files
 */
function theme_piecemaker_blocks_files($vars) {
  $form = $vars['form'];
  $head = array(
    t('Type'),
    t('Order'),
    t('File'),
    t('Options'),
    t('Add/Delete File'),
  );
  foreach(element_children($form) as $key) {
    $line = &$form[$key];
    $row = array();
    $row['data'][] = drupal_render($line['type']);
    $row['data'][] = drupal_render($line['weight']);
    $row['data'][] = drupal_render($line['media']);
    $row['data'][] = drupal_render($line['options']);
    $row['data'][] = drupal_render($line['action']);
    $row['class'][] = 'draggable';
    
    if ($key == 'add') {
      $row['id'][] = 'add-files-row';
    }
    $tvars['rows'][] = $row;
  }
  
  $tvars['header'] = $head;
  $tvars['attributes'] = array(
    'id' => 'piecemaker-files',
  );
  $tvars['sticky'] = FALSE;
  drupal_add_tabledrag('piecemaker-files', 'order', 'sibling', 'file-weight');
  return theme('table', $tvars);
}

/**
 * Delete File callback for the block add/edit form
 */
function piecemaker_blocks_delete_file($form, &$form_state) {
  $form_state_clone = &drupal_static('piecemaker_blocks_block_form');  
  $clicked = $form_state['clicked_button'];
  $key = $clicked['#parents'][1];
  $files = $form_state['values']['file'];
  unset($files[$key]);
  //Kill the add row since it's not needed;
  unset($files['add']);
  uasort($files, 'drupal_sort_weight');
  foreach ($files as $key => $file) {
    unset($file['weight']);
    unset($file['action']);
    $blocks_files[] = $file; 
  }
  $form_state['blocks_files'] = $blocks_files;
  $form_state['rebuild'] = TRUE;
  drupal_set_message('File Deleted');
  $form_state_clone = $form_state;
}
/**
 * Add file callback for the black add/edit form
 */
function piecemaker_blocks_add_file($form, &$form_state) {
  $form_state_clone = &drupal_static('piecemaker_blocks_block_form');  
  $files = $form_state['values']['file'];
  uasort($files, 'drupal_sort_weight');
  foreach ($files as $key => $file) {
    unset($file['weight']);
    unset($file['action']);
    $blocks_files[] = $file; 
  }
  $form_state['blocks_files'] = $blocks_files;
  $form_state['rebuild'] = TRUE;
  drupal_set_message('File Added');
  $form_state_clone = $form_state;
}

/**
 * Add file callback for the black add/edit form
 */
function piecemaker_blocks_add_file_validate($form, &$form_state) {
  $form_state_clone = &drupal_static('piecemaker_blocks_block_form');  
  $file = $form_state['values']['file']['add'];
  if (empty($file['media']['fid'])) {
    form_set_error('file][add', 'You must select a file');
  }
  switch ($file['type']) {
    case 'Video':
      $opt = $file['options']['Video'];
      if (empty($opt['Width']) || empty($opt['Height'])) {
        form_set_error('file][add][options][Video', 'You must set a Width and Height for all Videos');
      }
      break;
  }
  $form_state_clone = $form_state;
}
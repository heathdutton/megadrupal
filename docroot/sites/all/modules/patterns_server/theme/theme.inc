<?php
/**
 * @file
 * Theme related functions, hooks.
 */

/**
 * Implements hook_theme().
 */
function patterns_server_theme() {
  return array(
    'patterns_server_pattern_list' => array(
      'variables' => array('patterns' => array(),
                           'searchkey' => array()),
    ),
    'patterns_server_pattern_fieldset' => array(
      'render element' => 'form',
    ),
    'patterns_server_patterns_table' => array(
      'variables' => array('header' => array(),
                           'rows' => array()),
    ),

    'patterns_server_one_pattern_info' => array(
      'variables' => array('patterns' => array()),
    ),
  );
}

/**
 * Build a table row for the tables in the pattern list page.
 * @param mixed $pid the numeric id of the pattern as in the database
 * @param StdClass $pattern A pattern object as loaded from database
 * @param array $extra associative array of extra parameters. Not used now.
 */
function _patterns_server_pattern_build_row($pid, $pattern, $options = array()) {

  $public_link = variable_get('patterns_public_url', '');

  $form['category'] = array( '#markup' => $pattern->info['category'],
  );
  $form['title'] = array(
    '#markup' => l($pattern->title, $public_link . '/info/' . $pid, array('attributes' => array('class' => "info-link"))),
  );
  $form['author'] = array(
    '#markup' =>  l($pattern->author, $public_link . '/search/2_' . $pattern->author),
  );
  $form['upload time'] = array(
    '#markup' => '<i value="' . $pattern->updated . '" ' . 'class="upload-time">' . date('j-m-Y H:i:s (P)', $pattern->updated) . '</i>',
  );
  $download_origin_times = db_select('patterns_server', 'ps')
    ->fields('ps', array('downloadnum', 'liked'))
    ->condition('pid', $pattern->pid)
    ->execute()
    ->fetchAssoc();
  if (empty($download_origin_times['downloadnum'])) {
    $download_origin_times['downloadnum'] = 0;
  }
  $form['downloadnum'] = array(
    '#markup' => '<strong class="download-times">' . $download_origin_times['downloadnum'] . '</strong>',
  );
  if (empty($download_origin_times['liked'])) {
    $download_origin_times['liked'] = 0;
  }
  $form['likednum'] = array(
    '#markup' => '<strong id="' . "$pattern->pid" . '_liked' . '">' . $download_origin_times['liked'] . '</strong>',
  );
  $form['download'] = array(
    '#markup' => l(t('Download'), $public_link . '/download/' . $pid, array('attributes' => array('class' => 'download-link'))),
  );
  global $user;
  $liked_origin = db_select('patterns_liked', 'pr')
    ->fields('pr', array('pid', 'uid'))
    ->condition('pid', $pattern->pid)
    ->condition('uid', $user->uid)
    ->execute()
    ->fetchAssoc();
  if (empty($liked_origin['pid'])) {
    $form['liked'] = array(
      '#markup' => l(t('Like'), $public_link . '/like/' . $pid, array('attributes' => array('class' => 'liked-link'))),
    );
  }
  else {
    $form['liked'] = array(
      '#markup' => l(t('Unlike'), $public_link . '/like/' . $pid, array('attributes' => array('class' => 'liked-link'))),
    );
  }
  return $form;
}


/**
 * Returns a list of the patterns currently stored in the server.
 */
function theme_patterns_server_pattern_list($args) {
  drupal_add_js('misc/ajax.js');
  drupal_add_js(drupal_get_path('module', 'patterns_server') . '/js/patterns_server_page.js');
  drupal_add_js(drupal_get_path('module', 'patterns_server') . '/js/moment.min.js');
  drupal_add_css(drupal_get_path('module', 'patterns_server') . '/css/patterns_server_page.css');
  // Get the data needed to be theme.
  $patterns = $args['patterns'];
  // Search box.
  $form['search-box']=drupal_get_form('patterns_server_search_form', $args['searchkey']);
  $patterns_title = '<div id="all_patterns_div">';
  $form['patterns'] = array(
    '#prefix' => $patterns_title,
    '#suffix' => ' </div>',
    '#tree' => TRUE,
  );
  if (empty($args['searchkey'])) {
    $title = 'Latest Patterns';
  }
  else {
    $title = 'Serach';
  }
  if (empty($patterns)) {
    if (empty($args['searchkey'])) {
      $form['patterns']['#markup'] =  t('No patterns available.');
    }
    else{
      $form['patterns']['#markup'] =  t('No search result.');
    }
  }
  else {
    foreach ($patterns as $pid => $pattern) {
      $form['patterns'][] = _patterns_server_pattern_build_row($pid, $pattern);
    }
    $form['patterns'] += array(
      '#type' => 'fieldset',
      '#title' => $title,
      '#collapsible' => TRUE,
      '#collapsed' => TRUE,
      '#theme' => 'patterns_server_pattern_fieldset',
      '#header' => array(
          t('Category'),
          t('Title'),
          t('Author'),
          t('Uploaded'),
          t('Downloads'),
          t('Score'),
          t('File'),
          t('Like'),
      ),
    );
  }
  return drupal_render($form);
}

/**
 */
function theme_patterns_server_pattern_fieldset($variables) {
  $form = $variables['form'];
  $rows = array();

  foreach (element_children($form) as $key) {
    $pattern = $form[$key];
    // $row is a row with six elements.
    $row = array();
    $row[] =  drupal_render($pattern['category']);
    $row[] = '<strong>' . drupal_render($pattern['title']) . '</strong>';
    $row[] = drupal_render($pattern['author']);
    $row[] = drupal_render($pattern['upload time']);
    $row[] = drupal_render($pattern['downloadnum']);
    $row[] = drupal_render($pattern['likednum']);
    $row[] = drupal_render($pattern['download']);
    $row[] = drupal_render($pattern['liked']);
    // $rows is a set of 10 rows  in which every row has six elements.
    $rows[] = $row;
  }
  return theme('patterns_server_patterns_table', array('header' => $form['#header'], 'rows' => $rows));
}

/**
 * Returns a table with the patterns.
 */
function theme_patterns_server_patterns_table($variables) {
  $header = $variables['header'];
  $rows = $variables['rows'];
  $attributes['class'][] = 'table table-hover';
  $output = '<table' . drupal_attributes($attributes) . ">\n";
  // Format the table header:
  if (count($header)) {
    $ts = tablesort_init($header);
    // HTML requires that the thead tag has tr tags in it followed by tbody
    // tags. Using ternary operator to check and see if we have any rows.
    $output .= (count($rows) ? ' <thead><tr>' : ' <tr>');
    foreach ($header as $cell) {
      $cell = tablesort_header($cell, $header, $ts);
      $output .= _theme_table_cell($cell, TRUE);
    }
    // Using ternary operator to close the tags based on whether or not there are rows
    $output .= (count($rows) ? " </tr></thead>\n" : "</tr>\n");
  }
  else {
    $ts = array();
  }
  // Format the table rows:
  if (count($rows)) {
    $output .= "<tbody>\n";
    $flip = array('even' => 'odd', 'odd' => 'even');
    $class = 'even';
    $row_number = 0;
    foreach ($rows as $number => $row) {
      $attributes = array();
      $class = $flip[$class];
      $attributes['class'][] = "pattern_row";
      // Build row
      $output .= ' <tr' . drupal_attributes($attributes) . '>';
      $i = 0;
      foreach ($row as $cell) {
        $cell = tablesort_cell($cell, $header, $ts, $i++);
        $output .= _theme_table_cell($cell);
      }
      $output .= " </tr>\n";
    }
    $output .= "</tbody>\n";
  }
  $output .= "</table>\n";
  return $output;
}

/**
 * Returns the information about certain pattern.
 */
function theme_patterns_server_one_pattern_info($args) {
  drupal_add_css(drupal_get_path('module', 'patterns_server') . '/css/patterns_server_page.css');
  drupal_add_js(drupal_get_path('module', 'patterns_server') . '/js/patterns_server_page.js');
  $patterns = $args['patterns'];
  if (count($patterns) == 0) {
    return NULL;
  }
  $keys = array_keys($patterns);
  $pattern = $patterns[$keys[0]];
  $content = _patterns_server_one_pattern_content($pattern);
  $content .= _patterns_server_one_pattern_comment_list($pattern);
  $markup_begin = '<div id="one_pattern_div"><fieldset>';
  $markup_end = '</fieldset></div>';
  global $base_url;
  $public_link = variable_get('patterns_public_url', '');
  $info_link = $public_link . '/info/' . $pattern->pid;
  $absolute_link = $base_url . '/' . $public_link;
  $server_link = l(t('Detail'), $info_link, array('attributes' => array('class' => array('btn btn-success server_detail'), 'name' => array("$absolute_link"))));
  $header_link = t("$pattern->title");
  $markup_header = '<legend>' . $header_link . "<strong>$server_link </strong>" . '</legend>';
  $markup_content = '<div id="one_pattern_content">' . $content . '</div>';
  $page = $markup_begin . $markup_header . $markup_content . $markup_end;
  return $page;
}

/**
 * Yaml file presentation.
 */
function _patterns_server_parser_yaml_file($pattern_file)  {
  $content_this = NULL;
  foreach ($pattern_file as $key => $value) {
    if (is_numeric($key) && is_array($value)) {
              $value_keys = array_keys($value);
              foreach ($value_keys as $_key => $_value) {
                $content_this .= '<li >' . '- <span class="yaml_aa5500">' . $_value . '</span> :' . '</li><ul>';
                $content_this .= _patterns_server_parser_yaml_file($value[$_value]);
                $content_this .= '</ul>';
              }
    }
    elseif (!is_numeric($key) && is_array($value)) {
      $content_this .= '<li><span class="yaml_aa5500">' . $key . '</span> :' . '</li><ul>';
      $content_this .= _patterns_server_parser_yaml_file($value);
      $content_this .= '</ul>';
    }
    elseif (is_numeric($key) && !is_array($value)) {
      $content_this .= '<li>' . '- <span class="yaml_116644">' . $value . '</span></li>';
    }
    else {
      $content_this .= '<li><span class="yaml_aa5500">' . $key . '</span> : <span class="yaml_221199">' . $value . '</span></li>';
    }
  }
  return $content_this;
}

/**
 * Returns the content of the selected pattern.
 */
function _patterns_server_one_pattern_content($pattern) {
  unset($pattern->status);
  unset($pattern->public);
  unset($pattern->enabled);
  unset($pattern->updated);
  unset($pattern->file);
  $pattern_file = $pattern->pattern;
  // Div for descript of pattern.
  $content = '<div class="one_pattern_one_word hero-unit"><p>Pattern Description: </p><dl class="dl-horizontal">';
  foreach ($pattern as $key => $value) {
    if (!is_array($value)) {
      $content .= '<dt>' . $key . '</dt><dd>' . $value . '</dd>';
    }
  }
  // Close div for descript of pattern.
  $content .= '</dl></div>';
  // Div for pattern file content.
  $title = "Pattern file content";
  $content .= '<div class="one_pattern_file hero-unit"><p id="one_pattern_file_descript">' . $title . '</p>';
  $file_content = '<div id="one_pattern_file_content" class="hero-unit"><ul>';
  $content .= $file_content;
  $content .= _patterns_server_parser_yaml_file($pattern_file );
  // Close div for pattern file content.
  $content .= '</ul></div></div>';
  return $content;
}

/**
 * Displays list of comments.
 */
function _patterns_server_one_pattern_comment_list($pattern) {
  // Div for comment of this pattern.
  $title = "Comments for pattern $pattern->pid($pattern->title) : ";
  $content = NULL;
  $content .= '<div class="one_pattern_file_comment hero-unit"><p>' . $title . '</p>';
  $pid = $pattern->pid;
  $comment_list = db_select('patterns_comments', 'pc')
    ->fields('pc')
    ->condition('pid', $pattern->pid)
    ->orderBy('created', 'ASC')
    ->execute()
    ->fetchAllAssoc('cid');
  if (!empty($comment_list)) {
    foreach ($comment_list as $key => $value) {
      $user_name = db_select('users', 'uc')
        ->fields('uc', array('name'))
        ->condition('uc.uid', $value->uid)
        ->execute()
        ->fetchAssoc();
      $created_time = date('jS F Y h:i:s A (T)', $value->created);
      $content .= '<div class = "one_pattern_file_comment_item">';
      $content .= '<strong>' . $user_name['name'] . '</strong>';
      $content .= ' said on ' . "<em> $created_time </em>" . ': <br/>';
      $content .= '<pre>' . $value->comment_content . '</pre>' . '</br>';
      $content .= "</div>";
    }
  }
  global $user;
  $args = array(
    'uid' => $user,
    'pid' => $pattern->pid,
  );
  $form['comment-box']=drupal_get_form('patterns_server_comment_form', $args);
  $content .= drupal_render($form);
  // Close div for comment of this pattern.
  $content .= '</div>';
  return $content;
}

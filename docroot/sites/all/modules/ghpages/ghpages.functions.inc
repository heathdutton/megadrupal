<?php
/**
 * @file
 * Various helper functions.
 */

/**
 * This creates a gh-pages folder in sites/default/files with a repo sub-dir.
 */
function ghpages_initialize_repo() {

  $ghpages_path = drupal_realpath(file_default_scheme() . '://') . '/gh-pages';
  if (!is_dir($ghpages_path)) {
    mkdir($ghpages_path);
  }

  $repo_name = basename(variable_get('ghpages_repo', ''), '.git');
  $repo_path = $ghpages_path . '/' . $repo_name;

  if (!is_dir($repo_path)) {
    mkdir($repo_path);
    $repo = new GitRepo($repo_path, TRUE, FALSE);
    $repo->clone_remote(variable_get('ghpages_repo'));
    $repo->checkout('gh-pages');
  }

  if (!is_dir($repo_path . '/pages')) {
    mkdir($repo_path . '/pages');
  }
}

/**
 * Utility function to process the html of the node.
 *
 * @param bool $everything
 *   Set to true if want to pass all site files
 *
 * @param bool $links
 *   Set to true if want to use absolute links instead
 *
 * @param bool $anon
 *   Set to true if want html as if viewed anonymously
 *
 * @return string
 *   Location of the locally saved html file
 */
function ghpages_data_process($everything, $links, $anon, $dir) {
  $node = menu_get_object();
  $html_name_array = explode("/", drupal_get_path_alias('node/' . $node->nid));
  $html_name = end($html_name_array);
  $html_name = check_plain(str_replace(" ", "", $html_name)) . '.html';

  // Turn on CSS / JS aggregation to cut down hugely on # of file transfers.
  $css_aggregation = variable_get('preprocess_css');
  $js_aggregation = variable_get('preprocess_js');
  variable_set('preprocess_css', 1);
  variable_set('preprocess_js', 1);
  $html_headers = array();
  $stage_site = url('node/' . $node->nid, array('absolute' => TRUE));
  if ($anon) {
    global $user;
    $current_user = $user;
    drupal_save_session(FALSE);
    $user = user_load(0);
    $html_piece = drupal_http_request(
      $stage_site,
      array('headers' => $html_headers)
    );
    if (!$html_piece->data) {
      drupal_set_message(
        t('There was a problem getting the Drupal page HTML'),
        'error'
      );
    }
    $html_original = $html_piece->data;
    $user = $current_user;
    drupal_save_session(TRUE);
  }
  else {
    $html_piece = drupal_http_request(
      $stage_site,
      array(
        'headers' => $html_headers,
      )
    );

    if (!$html_piece->data) {
      drupal_set_message(
        t('There was a problem getting the Drupal page HTML'), 'error'
      );
    }
    $html_original = $html_piece->data;
  }
  if ($everything) {

    // Initialize arrays to list all of the files we need to copy.
    $files = array();

    // Use QueryPath to find the files we need.
    $qp_links = htmlqp($html_original, 'link');
    $qp_javascripts = htmlqp($html_original, 'script');
    $qp_images = htmlqp($html_original, 'img');
    foreach ($qp_links as $qp_link) {
      if ($qp_link->attr('rel') == 'stylesheet') {
        $files[] = parse_url($qp_link->attr('href'), PHP_URL_PATH);
      }
    }
    foreach ($qp_javascripts as $qp_js) {
      if ($qp_js->attr('src') != NULL) {
        $files[] = parse_url($qp_js->attr('src'), PHP_URL_PATH);
      }
    }
    foreach ($qp_images as $qp_img) {
      if ($qp_img->attr('src') != NULL) {
        $files[] = parse_url($qp_img->attr('src'), PHP_URL_PATH);
      }
    }
    $html_fixed = @qp($html_original, array('ignore_parser_warnings' => TRUE));

    // Make all css, js, and image paths relative in the HTML.
    htmlqp($html_fixed, 'link')->each('ghpages_qp_base_href');
    htmlqp($html_fixed, 'script')->each('ghpages_qp_base_src');
    htmlqp($html_fixed, 'img')->each('ghpages_qp_base_src');
    $html_fixed = $html_fixed->html();

    // Call function to copy all files over.
    $result = ghpages_copy_files($files, $dir);
  }
  elseif ($links) {
    $name = 'http://' . $_SERVER['SERVER_NAME'];
    $pattern_css = '/stylesheet=\"all\" href=\"/';
    $new_pattern_css = 'stylesheet="all" href="' . $name;
    $html_fixed = preg_replace($pattern_css, $new_pattern_css, $html_original);
    $pattern_js_and_images = '/src=\"/';
    $new_pattern_js_and_images = 'src="' . $name;
    $html_fixed = preg_replace(
        $pattern_js_and_images,
        $new_pattern_js_and_images,
        $html_original
    );
  }
  $repo_name = basename(variable_get('ghpages_repo', ''), '.git');
  $repo_path = drupal_realpath(
    file_default_scheme() . '://') . '/gh-pages/' . $repo_name;

  if (!is_dir($repo_path . '/pages/' . $dir)) {
    mkdir($repo_path . '/pages/' . $dir);
  }

  $file_place = $repo_path . '/pages/' . $dir . '/' . $html_name;
  $file = fopen($file_place, "w");
  if (!fwrite($file, $html_fixed)) {
    drupal_set_message(
      t('Unable to save the html file locally at @file_place. Check your files settings.',
        array('@file_place' => $file_place)
      )
    );
  }
  fclose($file);
  // Set CSS / JS aggregation back to their original states.
  variable_set('preprocess_css', $css_aggregation ? 1 : 0);
  variable_set('preprocess_js', $js_aggregation ? 1 : 0);

  // Return location of where we save the file.
  return $file_place;
}

/**
 * Utility function for setting href to basename only.
 *
 * @param int $index
 *   The index of the QP object
 *
 * @param object $item
 *   A QueryPath object
 *
 * @return bool
 *   True continues the flow
 */
function ghpages_qp_base_href($index, $item) {
  $src = $item->getAttribute('href');
  $item->setAttribute('href', basename($src));
  return TRUE;
}

/**
 * Utility function for setting src to basename only.
 *
 * @param int $index
 *   The index of the QP object
 *
 * @param object $item
 *   A QueryPath object
 *
 * @return bool
 *   true continues the flow
 */
function ghpages_qp_base_src($index, $item) {
  $src = $item->getAttribute('src');
  $item->setAttribute('src', basename($src));
  return TRUE;
}

/**
 * Copy files to the public files directory before sending to GitHub.
 *
 * @param array $files
 *   A listing of file paths relative to DRUPAL_ROOT.
 *
 * @param string $dir
 *   The directory to create files in.
 *
 * @return bool
 *   True on successful copy, false on error
 */
function ghpages_copy_files($files, $dir) {
  $repo_path = drupal_realpath(file_default_scheme() . '://') . '/gh-pages/' .
    basename(variable_get('ghpages_repo', ''), '.git') . '/pages/' . $dir;
  if (!is_dir($repo_path)) {
    mkdir($repo_path);
  }
  foreach ($files as $file) {
    $path = file_unmanaged_copy(
        DRUPAL_ROOT . $file, $repo_path,
        FILE_EXISTS_REPLACE
    );
    if (!$path) {
      drupal_set_message(
        t('Failed to copy @file to the default files directory @dir',
          array('@file' => $file, '@dir' => $repo_path)
        )
      );
    }
  }
}

/**
 * Utility function to push to GitHub.
 *
 * @param array $form_state
 *   The values from our form entries
 *
 * @return mixed
 *   On error returns boolean false otherwise a file pointer
 */
function ghpages_push_files($form_state) {
  $dir = $form_state['values']['github_directory'];
  $repo_name = basename(variable_get('ghpages_repo', ''), '.git');
  $ghpages_path = drupal_realpath(file_default_scheme() .
    '://') . '/gh-pages/' . $repo_name;
  $repo_path = $ghpages_path . '/pages/' . $dir;

  if (!is_dir($ghpages_path . '/.git')) {
    $repo = Git::create($ghpages_path);
  }
  else {
    $repo = Git::open($ghpages_path);
  }

  $repo->add($repo_path);
  $repo->commit('Adding ' . $ghpages_path);

  try {
    $repo->push('https://' . variable_get('ghpages_username') . ':' .
      variable_get('ghpages_pass') . '@github.com/' .
      variable_get('ghpages_username') . '/' . $repo_name, 'gh-pages');
  }
  catch (Exception $e) {
    $message = $e->getMessage();
    if (strstr($message, variable_get('ghpages_pass'))) {
      $message = str_replace(
        variable_get('ghpages_pass'),
        str_repeat('*', count(variable_get('ghpages_pass'))),
        $message
      );
    }
    drupal_set_message(
      t('Failed while pushing repo with error message: @error',
        array('@error' => $message)
      ),
      'error'
    );
  }

  drupal_set_message(
    t('Page pushed to <a href="@url">@url</a>',
      array(
        '@url' => 'http://' . variable_get('ghpages_username') .
        '.github.com/' . $repo_name . '/pages/' . $dir . '/' . $dir . '.html',
      )
    )
  );
}

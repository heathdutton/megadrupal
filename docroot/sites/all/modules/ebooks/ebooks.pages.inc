<?php

/**
 * @file
 * Non-admin page callbacks.
 */


/**
 * Additional pages of results. This gets pulled in via AJAX calls. This is basically duplicated code from the ebooks_get_overdrive_html() function.
 */
function ebooks_pages_overdrive() {
  $page_overdrive = check_plain($_POST['page_overdrive']);
  $only_available = check_plain($_POST['only_available']);
  $options = array('ajax' => TRUE, 'page_overdrive' => $page_overdrive, 'only_available' => $only_available);
  $ebooks_search = check_plain($_POST['ebooks_search']);
  echo ebooks_get_overdrive_html($ebooks_search, $options);
  exit; // Need to kill it or else the AJAX runs repeatedly.
}

function ebooks_pages_3m() {
  $url = variable_get('ebooks_3m_url') . html_entity_decode(check_url($_REQUEST['url']));

  // Initialize a cURL session
  $ebooks_3m = curl_init();
  curl_setopt($ebooks_3m, CURLOPT_URL, $url);
  curl_setopt($ebooks_3m, CURLOPT_HEADER, TRUE);
  curl_setopt($ebooks_3m, CURLOPT_NOBODY, FALSE);
  curl_setopt($ebooks_3m, CURLOPT_RETURNTRANSFER, TRUE);
  curl_setopt($ebooks_3m, CURLOPT_REFERER, $url);
  curl_setopt($ebooks_3m, CURLOPT_USERAGENT, "Mozilla/5.0 (Windows NT 6.1; WOW64; rv:14.0) Gecko/20100101 Firefox/14.0.1");
  $search_3m = curl_exec($ebooks_3m);

  // 3M links require us to already have a cookie in place or else the request to search won't work. Let's find the cookie values.
  $cookie = '';
  $headers = explode("\n", $search_3m);

  foreach ($headers as $index => $value) {
    $header_arr = explode(': ', $value);
    $hName = $header_arr[0];
    $hValue = $header_arr[1];
    if ($hName == 'Set-Cookie') {
      $hValue = substr($hValue, 0, strpos($hValue, ';'));

      if ($cookie) {
        $cookie .= '; ';
      }

      $cookie .= $hValue;
    }
  }


  curl_setopt($ebooks_3m, CURLOPT_URL, $url);
  curl_setopt($ebooks_3m, CURLOPT_COOKIE, $cookie);
  curl_setopt($ebooks_3m, CURLOPT_HEADER, FALSE);
  curl_setopt($ebooks_3m, CURLINFO_HEADER_OUT, TRUE);
  curl_setopt($ebooks_3m, CURLOPT_NOBODY, FALSE);
  curl_setopt($ebooks_3m, CURLOPT_HTTPHEADER, array(
    'Accept: text/html,application/xhtml+xml,application/xml;q=0.9,*/*;q=0.8',
  ));

  $search_3m = curl_exec($ebooks_3m);

  //getting only div that we need.
  //i-> where to start h-> how many char to go
  $search_3m = trim(preg_replace('/\s+/', ' ', $search_3m)); // Remove any whitespace characters (linebreaks, etc.)

  if (strpos($search_3m, 'class="doc-link"') !== FALSE) {
    $i = strpos($search_3m, 'channel-list-view') + 19;
    $h = strpos($search_3m, 'root-footer') - $i - 9;

    $search_3m = substr($search_3m, $i, $h);
    $search_3m = str_replace('href= "', 'href="', $search_3m);
    $search_3m = str_replace('href="/library/', 'href="http://ebook.3m.com/library/', $search_3m);
    $search_3m = str_replace('<form', '<div class="form" ', $search_3m);
    $search_3m = str_replace('</form', '</div', $search_3m);

    // Strip unecessary/problematic tags.
    $search_3m = preg_replace('/<script[^>]*>.*?<\/script>/is', '', $search_3m);
    $search_3m = preg_replace('/<fieldset[^>]*>.*?<\/fieldset>/is', '', $search_3m);
    $search_3m = preg_replace('/<b\s[^>]*>.*?<\/b>/is', '', $search_3m);
    $search_3m = preg_replace('/<span class="year"[^>]*>.*?<\/span>/is', '', $search_3m);
    $search_3m = str_replace('</div></div></div></div></div></div>', '</div></div></div>', $search_3m);

    if (strpos($search_3m, 'Sorry, no results found. Please try modifying your query.')) {
      exit;
    }
    // Check for unnecessary final tags from 3M.
    $final_tags = substr($search_3m, -18);
    if ($final_tags == '</div></div></div>') {
      // Chop off the final tags then.
      $search_3m = substr($search_3m, 0, -18);
    }
    echo $search_3m;
  }
  exit;
}

function ebooks_get_widgets_form($form, $form_state) {

  $module_url = drupal_get_path('module', 'ebooks');

  drupal_add_css($module_url . '/ebooks.css');
  drupal_add_js($module_url . '/loadingPages.js');

  $ebooks_overview = variable_get('ebooks_overview');
  if (!empty($ebooks_overview)) {
    $form['overview'] = array(
      '#type' => 'markup',
      '#markup' => $ebooks_overview,
    );
  }
  $searched = isset($form_state['storage']['searched']);

  $form['ebooks_block_form'] = array(
    '#type' => 'fieldset',
    '#title' => t('Find eBooks'),
    '#attributes' => array('id' => 'ebooks_form'),
  );

  $ebooks_search_default_value = '';
  if (isset($form_state['storage']['values']['ebooks_block_search'])) {
    $ebooks_search_default_value = $form_state['storage']['values']['ebooks_block_search'];
  }
  elseif (!empty($_POST['ebooks_block_search'])) {
    $ebooks_search_default_value = $_POST['ebooks_block_search'];
  }
  if (isset($form_state['storage']['values']['ebooks_last_search']) && $form_state['storage']['values']['op'] == t('Search In Your Results')) {
    $ebooks_search_default_value = $form_state['storage']['values']['ebooks_last_search'] . ' ' . $ebooks_search_default_value;
  }
  $form['ebooks_block_form']['ebooks_block_search'] = array(
    '#type' => 'textfield',
    '#title' => t('Search'),
    '#description' => t('Search a title, author, subject or ISBN'),
    '#default_value' => $ebooks_search_default_value,
  );

  $ebooks_available_default_value = 0;
  if (isset($form_state['storage']['values']['ebooks_available'])) {
    $ebooks_available_default_value = $form_state['storage']['values']['ebooks_available'];
  }
  elseif (!empty($_POST['ebooks_available'])) {
    $ebooks_available_default_value = $_POST['ebooks_available'];
  }
  $form['ebooks_block_form']['ebooks_available'] = array(
    '#type' => 'checkbox',
    '#title' => t('Only available books'),
    '#value' => $ebooks_available_default_value,
  );

  $form['ebooks_block_form']['ebooks_submit'] = array(
    '#type' => 'submit',
    '#default_value' => 'Search eBooks',
    '#attributes' => array('class' => array('show-loading')),
  );

  // If they searched from another page (navigation menu, etc.)...
  if (!empty($_POST['ebooks_block_search'])) {
    // Auto-submit the form and show the loading graphic.

    $form['overview'] = array(
      '#type' => 'markup',
      '#markup' => '<script type="text/javascript" src="//code.jquery.com/jquery-1.8.2.min.js"></script>' .
      '<script type="text/javascript">jQuery(document).ready(function($) { ' .
      '$(\'#ebooks-loading-panel\').css(\'display\', \'block\');' .
      '$(\'#ebooks-get-widgets-form\').submit();' .
      ' });</script>',
    );
  }
  // 3M links require us to already have a cookie in place or else the links to books won't work. Set the cookie by loading the basic 3M page in an invisible iframe.
  if (!$searched) {
    $form['ebooks_3m_hidden_cookie_frame'] = array(
      '#type' => 'markup',
      '#markup' => '<iframe src="' . variable_get('ebooks_3m_url') . '" width="0" height="0" tabindex="-1" title="empty" class="element-invisible"></iframe>',
    );
  }

  $form['ebooks_loading_panel'] = array(
    '#type' => 'markup',
    '#markup' => '<div id="ebooks-loading-panel">' . t('Searching eBooks') . ' <img src="/' . drupal_get_path('module', 'ebooks') . '/loading.gif" /></div>',
  );

  if ($searched) {
    $values = $form_state['storage']['values'];

    $ebooks_overview = variable_get('ebooks_overview');
    if (!empty($ebooks_overview)) {
      $form['overview'] = array(
        '#type' => 'markup',
        '#markup' => $ebooks_overview,
      );
    }

    $form['ebooks_block_form']['ebooks_within_results'] = array(
      '#type' => 'submit',
      '#default_value' => t('Search In Your Results'),
      '#attributes' => array('class' => array('show-loading')),
    );

    $options = array();

    if ($values['ebooks_available']) {
      $options['only-available'] = TRUE;
    }

    if ($values['op'] == t('Search In Your Results')) {
      $options['search-within-results'] = TRUE;
    }

    $form['ebooks_result_3m'] = array(
      '#type' => 'fieldset',
      '#title' => '3M',
      '#attributes' => array('id' => 'ebooks-3m'),
      '#value' => ebooks_get_3m_html($ebooks_search_default_value, $options),
    );

    if (isset($values['ebooks_last_search']) && $options['search-within-results'] == TRUE) {
      $options['last-search'] = $values['ebooks_last_search'] . ' ' . $ebooks_search_default_value;
    }
    else {
      $options['last-search'] = $ebooks_search_default_value;
    }

    $form['ebooks_result_overdrive'] = array(
      '#type' => 'fieldset',
      '#title' => 'Overdrive',
      '#attributes' => array('id' => 'ebooks-overdrive'),
      '#value' => ebooks_get_overdrive_html($ebooks_search_default_value, $options),
    );

    $form['ebooks_last_search'] = array(
      '#type' => 'hidden',
      '#value' => $options['last-search'],
      '#attributes' => array(
        'id' => array(
          'ebooks-last-search',
        ),
      ),
    );
  }

  return $form;
}

function ebooks_get_widgets_form_submit($form, &$form_state) {
  $form_state['storage']['values'] = $form_state['values'];
  if ($form_state['input']['ebooks_available']) {
    $form_state['storage']['values']['ebooks_available'] = 1;
  }
  $form_state['storage']['searched'] = TRUE;
  $form_state['rebuild'] = TRUE;
}

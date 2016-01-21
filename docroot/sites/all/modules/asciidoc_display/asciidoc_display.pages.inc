<?php

/**
 * @file
 * Page-generating functions for AsciiDoc display, including blocks.
 */

/**
 * Page callback: Displays an AsciiDoc page.
 */
function asciidoc_display_build_page() {
  // If a problem occurs, we'll want to return a message.
  $error = array(
    '#type' => 'markup',
    '#markup' => '',
  );

  $arg = arg();
  $page_info = _asciidoc_display_find_page($arg);

  if (isset($page_info['redirect']) && $page_info['redirect']) {
    drupal_goto($page_info['redirect']);
  }

  if (isset($page_info['image_file']) && $page_info['image_file']) {
    $path = drupal_realpath($page_info['image_file']);
    $handle = @fopen($path, 'rb');
    if (!$handle) {
      drupal_not_found();
    }

    drupal_add_http_header('Content-type', 'image');
    drupal_send_headers();
    // Transfer file in chunks, as in Drupal's file_transfer() function.
    // We cannot use that directly, because this is not in the standard
    // file system locations.
    while (!feof($handle)) {
      print fread($handle, 1024);
    }
    fclose($handle);
    drupal_exit();
  }

  if (!$page_info['book'] || $page_info['edit_file']) {
    $error['#markup'] = t('Could not load page');
    return $error;
  }

  $path = drupal_realpath($page_info['page_file']);
  $html = @file_get_contents($path);
  if (!$html) {
    $error['#markup'] = t('This page is currently empty.');
    return $error;
  }

  // Set the title based on the book/section title content.
  $html_info = _asciidoc_display_parse_bare_html($html, $page_info['is_index'], '', TRUE);
  if ($html_info['book_title'] != $html_info['section_title']) {
    $html_info['section_title'] .= ' - ' . $html_info['book_title'];
  }
  drupal_set_title($html_info['section_title']);

  // Add the CSS.
  drupal_add_css(drupal_get_path('module', 'asciidoc_display') . '/asciidoc-display.css');

  // Return the body for display on the page.
  if (empty($html_info['body'])) {
    $error['#markup'] = t('This page is currently empty.');
    return $error;
  }

  $copyright = _asciidoc_display_find_copyright($page_info);
  if ($copyright) {
    $copyright = '<div class="copyright">' . $copyright . '</div>';
  }

  return $html_info['body'] . $copyright;
}

/**
 * Finds the copyright notice.
 *
 * @param array $page_info
 *   Page information output from _asciidoc_display_find_page().
 *
 * @return string
 *   Copyright information to display, or an empty string if not found.
 */
function _asciidoc_display_find_copyright($page_info) {
  if (!isset($page_info['copyright_file'])) {
    return '';
  }

  $path = drupal_realpath($page_info['copyright_file']);
  $html = @file_get_contents($path);
  if (!$html) {
    return '';
  }

  $html_info = _asciidoc_display_parse_bare_html($html, FALSE, '', FALSE);
  return isset($html_info['copyright']) ? $html_info['copyright'] : '';
}

/**
 * Page callback: Puts up the AsciiDoc source editor.
 */
function asciidoc_display_edit_source() {
  // If a problem occurs, we'll want to return a message.
  $error = array(
    '#type' => 'markup',
    '#markup' => '',
  );

  $arg = arg();
  $page_info = _asciidoc_display_find_page($arg);
  if (!$page_info['book'] || !$page_info['edit_file']) {
    $error['#markup'] = t('Could not load page');
    return $error;
  }

  $file = $page_info['edit_file'];
  $config = $page_info['book'];
  if (!$config['source_path']) {
    $error['#markup'] = t('Could not load page');
    return $error;
  }

  $text = _asciidoc_display_load_source($config, $file, $page_info['language']);
  if (!$text) {
    $error['#markup'] = t('Could not read %file', array('%file' => $file));
    return $error;
  }

  // Display it in the editor.
  return drupal_get_form('asciidoc_display_edit_form', $text, $file, $config, $page_info['language']);
}

/**
 * Loads AsciiDoc source text.
 *
 * @param array $config
 *   The configuration for the book.
 * @param string $file
 *   The particular file name within the book.
 * @param string $language
 *   The language to load.
 *
 * @return string
 *   The text of the file, or FALSE if invalid language, invalid file, or
 *   not found.
 */
function _asciidoc_display_load_source($config, $file, $language) {
  if (!_asciidoc_display_validate($language, $file)) {
    return FALSE;
  }

  $language = $config['translated'] ? '/' . $language : '';
  $path = drupal_realpath($config['source_path'] . $language . '/' . $file);
  $text = @file_get_contents($path);

  return $text;
}

/**
 * Builds information for other languages.
 *
 * @param string $file
 *   The file being edited.
 * @param array $config
 *   The configuration for the book.
 * @param string $current_langcode
 *   The language code of the current editing page.
 *
 * @return
 *   Build array containing a fieldset containing the other language links,
 *   or NULL if there are no other languages. Also contains a text box with
 *   the source langauge in another fieldset, if it is different from the
 *   current language.
 */
function asciidoc_display_edit_other_languages($file, $config, $current_langcode) {

  if (!$config['translated']) {
    return NULL;
  }

  $build = array();

  // Make list of links to other languages.
  $languages = _asciidoc_display_list_languages($config);
  $links = array();
  foreach ($languages as $langcode => $name) {
    if ($langcode != $current_langcode) {
      $links[$langcode] = array(
        'title' => $name,
        'href' => 'edit/' . $config['path'] . '/' . $langcode . '/' . $file,
        'attributes' => array('target' => '_blank'),
      );
    }
  }
  if (count($links)) {
    $build['language'] = array(
      '#type' => 'fieldset',
      '#collapsible' => TRUE,
      '#collapsed' => TRUE,
      '#title' => t('Other languages'),
      '#description' => t('View/edit a different language. Opens in a new window.'),
    );

    $build['language']['links'] = array(
      '#theme' => 'links__asciidoc_display_languages',
      '#links' => $links,
      '#attributes' => array('class' => array('asciidoc_display_languages_edit')),
    );
  }

  // Make text box with source language, if different from current language.
  if ($config['source_language'] != $current_langcode) {
    $text = _asciidoc_display_load_source($config, $file, $config['source_language']);
    if ($text) {
      $build['original'] = array(
        '#type' => 'fieldset',
        '#collapsible' => TRUE,
        '#collapsed' => TRUE,
        '#title' => t('Original language'),
      );

      $build['original']['source_text'] = array(
        '#type' => 'textarea',
        '#title' => t('Text in @language', array('@language' => $languages[$config['source_language']])),
        '#value' => $text,
        '#rows' => 20,
        '#cols' => 80,
      );
    }
  }

  return $build;
}

/**
 * Form constructor for the AsciiDoc editing form.
 *
 * @param string $text
 *   Text to display in the editor.
 * @parm string $file
 *   File name being edited.
 * @param array $config
 *   Configuration for the book being edited in.
 * @param string $langcode
 *   Language code, if there is one.
 */
function asciidoc_display_edit_form($form, &$form_state, $text, $file, $config, $langcode) {
  $form['#asciidoc_source_file'] = $file;
  $form['#asciidoc_config'] = $config;

  $other = asciidoc_display_edit_other_languages($file, $config, $langcode);
  if ($other) {
    $form['other_language'] = $other;
  }

  $form['help'] = array(
    '#type' => 'markup',
    '#markup' => '<p>' . t('There is a Preview button in the toolbar. It will show you the formatting output -- mostly -- it does not work well with images and cross-links.') . '</p>',
  );

  $form['editor'] = array(
    '#type' => 'textarea',
    '#title' => t('Edit %file', array('%file' => $file)),
    '#default_value' => $text,
    '#rows' => 20,
    '#cols' => 80,
    '#attributes' => array('class' => array('mark-it-up')),
  );

  $form['#attached']['library'][] = array('asciidoc_display', 'asciidoc_display.edit');

  if (asciidoc_display_patches_allowed($config, FALSE)) {
    $label = t('Download patch file');
  }
  else {
    $label = t('Download new file');
  }

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => $label,
  );

  return $form;
}

/**
 * Form submission handler for asciidoc_display_edit_form().
 */
function asciidoc_display_edit_form_submit($form, &$form_state) {
  $config = $form['#asciidoc_config'];
  $file = $form['#asciidoc_source_file'];
  $text = $form_state['values']['editor'];

  // The text editor prefers DOS line endings. Convert back to Unix, if
  // configured.
  if ($config['line_endings'] == 'unix') {
    $text = str_replace("\r", '', $text);
  }

  if (!$text) {
    drupal_set_message(t('No text in editor'));
    return;
  }

  // Make a patch file, optionally.
  if (asciidoc_display_patches_allowed($config, FALSE)) {
    $path = drupal_realpath($config['source_path'] . '/' . $file);
    $old_text = @file_get_contents($path);
    if ($old_text) {
      $text = xdiff_string_diff($old_text, $text);
    }

    // If we made a diff, change the filename.
    $file = $file . '.patch';
  }

  // Send the patch or text file to the browser.
  drupal_add_http_header('Content-Type', 'text/plain');
  drupal_add_http_header('Content-Disposition', format_string('attachment; filename="@filename"', array('@filename' => $file)));
  print $text;
  exit();
}

/**
 * Parses a "bare" HTML output from AsciiDoc.
 *
 * Assumes the bare HTML file was made with the scripts/bare.xsl style sheet.
 *
 * @param string $html
 *   HTML to parse. Assumed not to be empty.
 * @param bool $is_home
 *   TRUE if this is the home page.
 * @param string $toc_active
 *   Add an "active" class to table of contents links pointing to this file,
 *   if it is provided. If not provided, the HTML for the table of contents
 *   is left empty.
 * @param bool $highlight_php
 *   Apply standard PHP syntax highlighting for PHP code, if TRUE.
 *
 * @return array
 *   Associative array containing:
 *   - book_title: The title of the book this is in.
 *   - section_title: The title of the section this is in.
 *   - body: The HTML of the page, without the titles and table of contents.
 *     Exception: On the home page, the table of contents is displayed.
 *   - toc: The HTML for the table of contents section, if any.
 *   - files: Array of the source files for the page, if any are noted. These
 *     are validated to contain only legal characters.
 *   - copyright: The HTML for the copyright notice, if any.
 */
function _asciidoc_display_parse_bare_html($html, $is_home, $toc_active = '', $highlight_php = FALSE) {

  // This function may get called several times on the same HTML if blocks
  // are in use, so put the output into drupal_static(), and check to see
  // if we have already calculated it.
  $found = &drupal_static(__FUNCTION__);
  $args = array($html, $is_home, $toc_active, $highlight_php);
  $key = md5(serialize($args));
  if (isset($found[$key])) {
    return $found[$key];
  }

  // Also, use the cache for longer-term storage.
  $cached = cache_get('asciidoc_display_parse_bare_html');
  if ($cached) {
    $cached = $cached->data;
  }
  else {
    $cached = array();
  }
  if (isset($cached[$key])) {
    $found[$key] = $cached[$key];
    return $cached[$key];
  }

  $dom = new \DomDocument();
  $dom->loadHTML(mb_convert_encoding($html, 'HTML-ENTITIES', 'UTF-8'));

  // Read off the book title.
  $elem = $dom->getElementById('asciidoc-display-book-title');
  $info['book_title'] = $elem->nodeValue;

  // Read off the section title.
  $elem = $dom->getElementById('asciidoc-display-section-title');
  $info['section_title'] = $elem->nodeValue;

  // Read out the source files. These look like:
  // <span class="remark">Source file: config.txt</span>
  // Also, read out and remove the copyright notice. This looks like:
  // <span class="remark">Copyright notice: ...</span>
  $info['files'] = array();
  $spans = $dom->getElementsByTagName('span');
  foreach ($spans as $span) {
    if ($span->getAttribute('class') == 'remark') {
      $val = $span->nodeValue;
      $matches = array();
      if (preg_match('/^Source file: ([a-zA-Z_.0-9\-]+)$/', $val, $matches)) {
        $info['files'][] = $matches[1];
      }
      else {
        $full_span = $dom->saveHTML($span);
        if (preg_match('/Copyright notice: /i', $full_span)) {
          $span->removeAttribute('class');
          $info['copyright'] = $dom->saveHTML($span);

          // This is enclosed in an EM in a P tag. Remove it. We will display
          // it on every page instead.
          $pparent = $span->parentNode->parentNode;
          $pparent->parentNode->removeChild($pparent);
        }
      }
    }
  }

  // Read off and remove the table of contents div, if there is one; it has
  // class "toc". Also locate the main content div, which has class
  // "asciidoc-display-main-content".
  $toc = NULL;
  $body = NULL;
  $divs = $dom->getElementsByTagName('div');
  foreach ($divs as $div) {
    $class = $div->getAttribute('class');
    if (!$class) {
      continue;
    }
    $classes = explode(' ', $class);
    // Pick out or remove the table of contents.
    if (in_array('toc', $classes)) {
      if ($is_home) {
        // The home page is empty except the table of contents, so we do not
        // want to remove it.
        $toc = $div;
      }
      else {
        $toc = $div->parentNode->removeChild($div);
      }
    }
    // Pick out the main body div.
    if (in_array('asciidoc-display-main-content', $classes)) {
      $body = $div;
    }
  }

  if ($is_home) {
    // Set a class on the table of contents div so we can avoid collapsing
    // the whole thing when on the index page.
    $class = $toc->getAttribute('class');
    $classes = array_filter(explode(' ', $class));
    $classes[] = 'asciidoc-home-toc';
    $toc->setAttribute('class', implode(' ', $classes));
  }
  elseif ($toc && $toc_active) {
    // Set any links in the table of contents to this URL to have class
    // "active", and their UL/LI parents to "active-trail". This is pointless
    // on the home page.
    $links = $toc->getElementsByTagName('a');
    foreach ($links as $link) {
      $href = $link->getAttribute('href');
      if (strpos($href, $toc_active) !== FALSE) {
        _asciidoc_display_link_set_active($link);
      }
    }
  }

  if ($highlight_php) {
    // Yes, we do need to call this several times. Sigh.
    _asciidoc_display_highlight_xml_once($body, $dom);
    _asciidoc_display_highlight_xml_once($body, $dom);
    _asciidoc_display_highlight_xml_once($body, $dom);
    _asciidoc_display_highlight_xml_once($body, $dom);
  }

  $body = _asciidoc_display_dom_element_to_html($body);
  $toc = _asciidoc_display_dom_element_to_html($toc);

  $info['body'] = $body;
  $info['toc'] = $toc;

  $found[$key] = $info;
  $cached[$key] = $info;
  cache_set('asciidoc_display_parse_bare_html', $cached);
  return $info;
}

/**
 * Renders a DOMElement into an HTML string.
 */
function _asciidoc_display_dom_element_to_html($elem) {
  if (!$elem) {
    return '';
  }

  $doc = new \DOMDocument;
  $cloned = $elem->cloneNode(TRUE);
  $doc->appendChild($doc->importNode($cloned, TRUE));
  $html = @$doc->saveHTML();
  if (!$html) {
    return '';
  }

  return $html;
}

/**
 * Sets a DOM node to have class "active", and parents to "active_trail".
 *
 * @param \DOMNode $link
 *   A link that is deemed to be active.
 */
function _asciidoc_display_link_set_active($link) {
  // Set the link itself to have class "active".
  $class = $link->getAttribute('class');
  $classes = array_filter(explode(' ', $class));
  if (!in_array('active', $classes)) {
    $classes[] = 'active';
  }
  $link->setAttribute('class', implode(' ', $classes));

  // Set all LI elements that are parents of this to have class "active-trail".
  $node = $link;
  while ($node = $node->parentNode) {
    if (is_a($node, 'DOMElement') && ($node->tagName == 'li' || $node->tagName == 'ul')) {
      $class = $node->getAttribute('class');
      $classes = array_filter(explode(' ', $class));
      if (!in_array('active-trail', $classes)) {
        $classes[] = 'active-trail';
      }
      $node->setAttribute('class', implode(' ', $classes));
    }
  }
}

/**
 * Finds and highlights PHP code.
 *
 * Note: Because of how DOM elements work, you need to call this several
 * times, because when you replace nodes the array gets screwed up.
 *
 * @param \DOMNode $node
 *   DOM node object to highlight.
 * @param \DOMDocument $doc
 *   DOM document object this is in.
 */
function _asciidoc_display_highlight_xml_once($node, $doc) {
  $pres = $node->getElementsByTagName('pre');
  foreach ($pres as $pre) {
    $class = $pre->getAttribute('class');
    $classes = array_filter(explode(' ', $class));
    if (in_array('programlisting-php', $classes)) {
      $newcode = _asciidoc_display_highlight_code($pre->nodeValue);
      $newdoc = new \DOMDocument();
      $newdoc->loadHTML($newcode);
      $el = $newdoc->getElementsByTagName('code')->item(0);
      $newnode = $doc->importNode($el, TRUE);
      // This is the part that screws up the array of $pres. I tried keeping
      // track of what needs to be replaced, and doing it all at the end,
      // but the act of doing replaceChild changes the parent and all of
      // its children, so that doesn't work well. You just need to call this
      // function several times to make sure you get everything.
      $pre->parentNode->replaceChild($newnode, $pre);
    }
  }
}

/**
 * Highlights PHP code.
 *
 * @param string $code
 *   Code to highlight.
 *
 * @return string
 *   Highlighted code.
 */
function _asciidoc_display_highlight_code($code) {
  // Get rid of the &gt; and &lt; entities.
  $code = str_replace(array('&gt;', '&lt;'), array('>', '<'), trim($code));

  // Add a <?php tag to the front.
  $strip = FALSE;
  if (strpos('<?php', $code) !== 0) {
    $code = "<?php " . $code;
    $strip = TRUE;
  }

  // Highlight using native PHP code highlighter.
  $code = highlight_string($code, TRUE);

  // Strip off the initial <?php tag.
  if ($strip) {
    $code = implode('', explode('&lt;?php&nbsp;', $code, 2));
  }

  return $code;
}

/**
 * Generates the navigation block, if we are on an AsciiDoc display page.
 */
function asciidoc_display_navigation_block() {
  $arg = arg();
  $page_info = _asciidoc_display_find_page($arg);

  if (isset($page_info['redirect']) || !$page_info['book'] || $page_info['edit_file']) {
    // Display nothing if this is not an AsciiDoc page.
    return array();
  }

  $path = drupal_realpath($page_info['index_file']);
  $html = @file_get_contents($path);
  if (!$html) {
    // Display nothing if we did not locate the index file.
    return array();
  }

  // Make a link to the front page.
  $config = $page_info['book'];
  $home_link = l(t('Cover page'), $config['path'], array('attributes' => array('class' => 'asciidoc_cover_link')));

  // Display the table of contents.
  module_load_include('inc', 'asciidoc_display', 'asciidoc_display.pages');
  $html_info = _asciidoc_display_parse_bare_html($html, $page_info['is_index'], $page_info['page_filename']);

  return array(
    'subject' => $html_info['book_title'],
    // @todo This should be made more flexible and themeable.
    'content' => array(
      '#markup' => $home_link . ' <div class="asciidoc-display-toc"> ' . $html_info['toc'] . '</div>',
      '#attached' => array(
        'library' => array(array('asciidoc_display', 'asciidoc_display.nav')),
      ),
    ),
  );
}

/**
 * Generates the language block, if we are on an AsciiDoc display page.
 */
function asciidoc_display_language_block() {
  $arg = arg();
  $page_info = _asciidoc_display_find_page($arg);

  if (isset($page_info['redirect']) || !$page_info['book'] || $page_info['edit_file']) {
    // Display nothing if this is not an AsciiDoc page.
    return array();
  }

  $languages = _asciidoc_display_list_languages($page_info['book'], 'links');
  if (!count($languages)) {
    // Display nothing if there are no languages.
    return array();
  }

  $build = array();
  $build['title'] = array(
    '#type' => 'markup',
    '#markup' => '<p>' . $page_info['book']['title'] . '</p>',
  );

  $build['links'] = array(
    '#theme' => 'links__asciidoc_display_languages',
    '#links' => $languages,
    '#attributes' => array('class' => array('asciidoc_display_languages')),
  );

  return array(
    'subject' => t('Languages'),
    'content' => $build,
  );
}

/**
 * Generates the edit block, if we are on an AsciiDoc display page.
 */
function asciidoc_display_edit_block() {
  $arg = arg();
  $page_info = _asciidoc_display_find_page($arg);

  if (isset($page_info['redirect']) || !$page_info['book']) {
    // Display nothing if this is not an AsciiDoc page.
    return array();
  }
  $config = $page_info['book'];

  // Build some links for this page.
  $items = array();

  // Add a link to the project page, if it is defined.
  if ($config['project_name'] && $config['project_url']) {
    $items[] = t('This page is generated from AsciiDoc source in the !link project',
      array('!link' => l($config['project_name'], $config['project_url'])));
  }

  // Add a file issue link, if it is defined.
  if ($config['issue']) {
    $issue_query = array(
      'component' => 'Documentation',
      // Bug reports are category = 1.
      'categories' => 1,
      // Do not translate the issue link.
      'body' => 'RE: ' . check_url(request_uri()),
    );
    $items[] = l(t('File an issue'), $config['issue'], array('query' => $issue_query));
  }

  // Add edit links for all of the files being used to generate this page, if
  // we are not already on an Edit page and the user can edit.
  if (!$page_info['edit_file'] && user_access('edit asciidoc source')) {
    $path = drupal_realpath($page_info['page_file']);
    $html = @file_get_contents($path);
    if ($html) {
      // Find all files marked as being used in the page.
      module_load_include('inc', 'asciidoc_display', 'asciidoc_display.pages');
      $html_info = _asciidoc_display_parse_bare_html($html, $page_info['is_index'], '', TRUE);

      foreach ($html_info['files'] as $file) {
        $language_part = ($page_info['language'] ? $page_info['language'] . '/' : '');
        $items[$file] = l(t('Edit source file @file', array('@file' => $file)), 'edit/' . $config['path'] . '/' . $language_part . $file);
      }
    }
  }

  // Add a link to the AsciiDoc user manual if we are editing, and a link back
  // to the book.
  if ($page_info['edit_file']) {
    $items[] = l(t('AsciiDoc User Manual'), 'http://asciidoc.org/userguide.html');
    $items[] = l(t('Return to book'), $config['path']);
  }

  // Put it all together and return it, if we have any links.
  if (!count($items)) {
    return array();
  }

  $build = array();
  $build['edits'] = array(
    '#theme' => 'item_list',
    '#items' => $items,
  );

  return array(
    'subject' => t('Source information'),
    'content' => $build,
  );
}


/**
 * Figures out which page of which AsciiDoc book we are on.
 *
 * The path looks something like this, where "(path)" is the path from the
 * individual book config, which might have multiple components:
 * @code
 * [edit/](path)/[langcode/][/filename]
 * @endcode
 * We need to match (path) and the edit/langcode parts with an existing config
 * and figure out which filename we are on (or redirect to index.html if none
 * is given).
 *
 * @param string[] $args
 *   Page arguments array, for a page display or source edit page.
 *
 * @return string[]
 *   Array of information, containing:
 *   - redirect: Path this page should be redirected to, if set. If set,
 *     nothing else will be set. The patch comes from the configured path for
 *     the book that is matched, plus the configured default language.
 *   - book: Configuration for the book we are in, or FALSE if we are not on a
 *     valid book page. If FALSE, the rest of the information will be
 *     omitted. This information comes straight from the configuration form and
 *     has not been further sanitized.
 *   - language: The language code for the page we are on, or '' if the book is
 *     not translated. This is checked to make sure it is a known language.
 *   - edit_file: File name we are editing, if on an edit page, or FALSE if we
 *     are not on an edit page. If set, the rest of the info will be as if we
 *     were on the index page. This is validated to make sure the file name is
 *     legal.
 *   - image_file: Full path to image file, if the request is for an image.
 *   - page_filename: File name of the HTML file of the current page. This is
 *     validated to make sure the file name is legal.
 *   - page_file: Full path to the HTML file of the current page, made up of
 *     the page_filename and language information, along with the configured
 *     file path for the book.
 *   - index_file: Full path to the HTML file containing the book index, made up
 *     of the language information along with the configured file path for the
 *     book.
 *   - copyright_file: Full path to the HTML file, which might or might not
 *     exist, containing the copyright notice. This is made up from the
 *     configured information for the book, plus language information.
 *   - is_index: TRUE if this is the index page.
 *
 * @see _asciidoc_display_validate()
 */
function _asciidoc_display_find_page($args) {
  $found = &drupal_static(__FUNCTION__);

  // See if we've already calculated this for this request.
  $key = implode('/', $args);
  if (isset($found[$key])) {
    return $found[$key];
  }

  // See if it's in the cache.
  $cached = cache_get('asciidoc_display_find_page');
  if ($cached) {
    $cached = $cached->data;
  }
  else {
    $cached = array();
  }
  if (isset($cached[$key])) {
    $found[$key] = $cached[$key];
    return $cached[$key];
  }

  // It's not in our static or the cache, so we need to calculate it.

  // See if we might be on an edit page.
  $could_be_edit = (count($args) > 0 && $args[0] == 'edit');

  // Pop off the (supposed) file name. If the last bit doesn't end in .html,
  // we will need to redirect (unless it is an edit or image page).
  $filename = array_pop($args);
  $extension = pathinfo($filename, PATHINFO_EXTENSION);
  $on_html = ($extension == 'html');
  $is_img = in_array(strtolower($extension), array('jpg', 'jpeg', 'png', 'gif'));

  // Figure out possible paths, with edit/ prefix, languages, etc.
  $possible_paths = array();

  if ($on_html) {
    $path = implode('/', $args);
    $possible_paths['plain'] = array(
      'path' => $path,
      'translated' => FALSE,
      'edit_file' => FALSE,
      'page_filename' => $filename,
      'image_file' => FALSE,
      'language' => '',
    );

    if (count($args) > 1) {
      $newargs = $args;
      $language = array_pop($newargs);
      $path = implode('/', $newargs);
      $possible_paths['plain_language'] = array(
        'path' => $path,
        'translated' => TRUE,
        'edit_file' => FALSE,
        'image_file' => FALSE,
        'language' => $language,
        'page_filename' => $filename,
      );
    }
  }
  if ($is_img) {
    $path = implode('/', $args);
    $possible_paths['image'] = array(
      'path' => $path,
      'translated' => FALSE,
      'edit_file' => FALSE,
      'page_filename' => FALSE,
      'image_file' => $filename,
      'image_dir' => '',
      'language' => '',
    );

    if (count($args) > 1) {
      // Could have a language.
      $langargs = $args;
      $language = array_pop($langargs);
      $path = implode('/', $langargs);
      $possible_paths['image_language'] = array(
        'path' => $path,
        'edit_file' => FALSE,
        'page_filename' => FALSE,
        'image_file' => $filename,
        'image_dir' => '',
        'translated' => TRUE,
        'language' => $language,
      );

      // Also support an image subdirectory.
      $subargs = $args;
      $subdir = array_pop($subargs);
      $path = implode('/', $subargs);
      $possible_paths['image_subdir'] = array(
        'path' => $path,
        'translated' => FALSE,
        'edit_file' => FALSE,
        'page_filename' => FALSE,
        'image_dir' => $subdir,
        'image_file' => $filename,
        'language' => '',
      );

      // And finally, and image subdirectory plus language.
      if (count($subargs) > 1) {
        $langargs = $subargs;
        $language = array_pop($langargs);
        $path = implode('/', $langargs);
        $possible_paths['image_subdir_language'] = array(
          'path' => $path,
          'edit_file' => FALSE,
          'page_filename' => FALSE,
          'image_dir' => $subdir,
          'image_file' => $filename,
          'translated' => TRUE,
          'language' => $language,
        );
      }
    }
  }
  elseif ($could_be_edit) {
    $newargs = $args;
    array_shift($newargs);
    $path = implode('/', $newargs);
    $possible_paths['edit'] = array(
      'path' => $path,
      'translated' => FALSE,
      'edit_file' => $filename,
      'image_file' => FALSE,
      'page_filename' => 'index.html',
      'language' => '',
    );

    if (count($newargs) > 1) {
      $language = array_pop($newargs);
      $path = implode('/', $newargs);
      $possible_paths['edit_language'] = array(
        'path' => $path,
        'translated' => TRUE,
        'edit_file' => $filename,
        'language' => $language,
        'image_file' => FALSE,
        'page_filename' => 'index.html',
      );
    }
  }
  else {
    // Make one more path with the filename assumed missing, as a redirect.
    $args[] = $filename;
    $path = implode('/', $args);
    $possible_paths['redirect'] = array(
      'path' => $path,
      'redirect' => TRUE,
    );
  }

  // Try to find a matching configuration for the paths we've found.
  $configs = asciidoc_display_load_all_configs();
  foreach ($configs as $config) {
    foreach ($possible_paths as $pathinfo) {
      if ($config['path'] == $pathinfo['path'] && isset($pathinfo['redirect'])) {
        $language = $config['translated'] ? '/' . $config['primary_language'] : '';
        $info = array(
          'redirect' => $config['path'] . $language . '/index.html',
        );
        $found[$key] = $info;
        $cached[$key] = $info;
        cache_set('asciidoc_display_find_page', $cached);
        return $info;
      }
      elseif ($config['path'] == $pathinfo['path'] && $config['translated'] == $pathinfo['translated']) {
        $filename = $pathinfo['page_filename'];
        $language = $config['translated'] ? '/' . $pathinfo['language'] : '';
        if (_asciidoc_display_validate($pathinfo['language'], $filename)) {
          $info = $pathinfo + array(
            'book' => $config,
            'page_file' => $config['dir'] . $language . '/' . $filename,
            'index_file' => $config['dir'] . $language . '/index.html',
            'copyright_file' => $config['dir'] . $language . '/' . $config['copyright_file'],
            'is_index' => ($filename == 'index.html'),
          );
          if ($info['image_file']) {
            if (_asciidoc_display_validate('', $info['image_dir'])) {
              $subdir = $info['image_dir'] ? '/' . $info['image_dir'] : '';
              $info['image_file'] = $config['dir'] . $language . $subdir . '/' . $info['image_file'];
            }
          }
          $found[$key] = $info;
          $cached[$key] = $info;
          cache_set('asciidoc_display_find_page', $cached);
          return $info;
        }
      }
    }
  }

  // If we got here, we never found a match.
  $info = array(
    'book' => 0,
  );
  $found[$key] = $info;
  $cached[$key] = $info;
  cache_set('asciidoc_display_find_page', $cached);

  return $info;
}

/**
 * Checks to see if a discovered filename and language are valid.
 *
 * @param string $language
 *   Language code, validated to be a known language code.
 * @param string $filename
 *   File name, validated to not contain any directory information or bad
 *   characters. The only characters allowed are letters, numbers, and the
 *   - _ and . characters.
 *
 * @return bool
 *   TRUE if the language and file name are valid, and FALSE if not.
 */
function _asciidoc_display_validate($language, $filename) {
  if ($language !== '') {
    include_once DRUPAL_ROOT . '/includes/iso.inc';
    $all_languages = _locale_get_predefined_list();
    if (!isset($all_languages[$language])) {
      return FALSE;
    }
  }

  if (preg_match('/[^a-zA-Z0-9.\-_]/', $filename)) {
    return FALSE;
  }

  return TRUE;
}

/**
 * Makes a list of the languages available in a book.
 *
 * @param array $config
 *   The configuration for the book to make the list for.
 * @param string $type
 *   What to return, one of the following:
 *   - options: (default) Make an options list for a select element.
 *   - links: Make a list sutiable for theme_links().
 *
 * @return array
 *   List showing the languages that actually exist for this book. Empty if it
 *   is not translated or there are no recognizable language subdirectories.
 */
function _asciidoc_display_list_languages($config, $type = 'options') {
  if (!$config['translated']) {
    return array();
  }

  // See if it's in the cache.
  $key = $config['config_id'] . '-' . $type;
  $cached = cache_get('asciidoc_display_list_languages');
  if ($cached) {
    $cached = $cached->data;
  }
  else {
    $cached = array();
  }
  if (isset($cached[$key])) {
    return $cached[$key];
  }

  // It's not in the cache, so calculate it.

  include_once DRUPAL_ROOT . '/includes/iso.inc';
  $all_languages = _locale_get_predefined_list();

  $found = array();
  $dir = drupal_realpath($config['dir']);
  if($handle = opendir($dir)) {
    while (($entry = readdir($handle)) !== FALSE) {
      if (isset($all_languages[$entry]) && is_dir($dir . '/' . $entry)) {
        $language = $all_languages[$entry];
        // This is an array with the English name followed optionally by
        // the name in the native language.
        $name = (count($language) > 1) ? $language[1] : $language[0];
        if ($type == 'options') {
          $found[$entry] = $name;
        }
        else {
          $found[$entry] = array(
            'title' => $name,
            'href' => $config['path'] . '/' . $entry . '/index.html',
          );
        }
      }
    }
  }

  $cached[$key] = $found;
  cache_set('asciidoc_display_list_languages', $cached);
  return $found;
}

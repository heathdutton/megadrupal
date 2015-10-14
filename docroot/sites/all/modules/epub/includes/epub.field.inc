<?php
/**
 * Implements hook_field_formatter_info().
 */
function epub_field_formatter_info() {
  return array(
    'epub_cover' => array(
      'label' => t('EPUB cover image'),
      'description' => t('Displays the cover image of ebook'),
      'field types' => array('file'),
      'settings' => array(
        'width' => '100%',
      ),
    ),
    'epub_default' => array(
      'label' => t('EPUB: Default'),
      'description' => t('Displays ebook pages in an iframe'),
      'field types' => array('file'),
      'settings' => array(
        'width' => '100%',
        'height' => 'auto',
      ),
    ),
    'epub_toc' => array(
      'label' => t('EPUB: Table of Contents'),
      'description' => t('Displays the table of contents of the ebook'),
      'field types' => array('file'),
      'settings' => array(
      ),
    ),
    'epub_js' => array(
      'label' => t('EPUB: epub.js'),
      'description' => t('Displays ebook with epub.js/reader/index.html'),
      'field types' => array('file'),
      'settings' => array(
        'unzipped' => TRUE,
        'width' => '100%',
        'height' => '600px',
      ),
    ),
    'epub_js_reader' => array(
      'label' => t('EPUB: epub.js reader'),
      'description' => t('Display a link to open the ebook online with Epub.js reader'),
      'field types' => array('file'),
      'settings' => array(
        'unzipped' => TRUE,
      ),
    ),
  );
}

function epub_field_formatter_settings_form($field, $instance, $view_mode, $form, &$form_state) {
  $element = array();
  $display = $instance['display'][$view_mode];
  $settings = $display['settings'];
  switch ($display['type']) {
    case 'epub_cover':
      $element['width'] = array(
        '#type' => 'textfield',
        '#title' => t('Width'),
        '#default_value' => $settings['width'] ? $settings['width'] : '100%',
        '#description' => t('The width of ebook cover image.'),
      );
      return $element;
    case 'epub_default':
      $element['width'] = array(
        '#type' => 'textfield',
        '#title' => t('Width'),
        '#default_value' => $settings['width'] ? $settings['width'] : 'auto',
        '#description' => t('The width of ebook viewer area.'),
      );
      $element['height'] = array(
        '#type' => 'textfield',
        '#title' => t('Height'),
        '#default_value' => $settings['height'] ? $settings['height'] : '100%',
        '#description' => t('The height of ebook viewer area. If "auto" is used, the iframe will auto-fit the epub document height and anchors in links will not work correctly.'),
      );
      return $element;
    case 'epub_js':
      $element['unzipped'] = array(
        '#type' => 'checkbox',
        '#title' => t('Use unzipped files from epub file in the reader'),
        '#default_value' => $settings['unzipped'],
        '#description' => t('If unchecked, zipped epub file will be used directly in the reader. Make sure your users\' browsers can handle zip file.'),
      );
      $element['width'] = array(
        '#type' => 'textfield',
        '#title' => t('Width'),
        '#default_value' => $settings['width'] ? $settings['width'] : '600px',
        '#description' => t('The width of ebook reader area.'),
      );
      $element['height'] = array(
        '#type' => 'textfield',
        '#title' => t('Height'),
        '#default_value' => $settings['height'] ? $settings['height'] : '100%',
        '#description' => t('The height of ebook reader area.'),
      );
      return $element;
    case 'epub_js_reader':
      $element['unzipped'] = array(
        '#disabled' => TRUE,
        '#type' => 'checkbox',
        '#title' => t('Use unzipped files from epub file in the reader'),
        '#default_value' => $settings['unzipped'],
        '#description' => t('If unchecked, zipped epub file will be used directly in the reader. Make sure your users\' browsers can handle zip file.'),
      );
      return $element;
  }
  return '';
}

/**
 * Implements hook_field_formatter_settings_summary().
 */
function epub_field_formatter_settings_summary($field, $instance, $view_mode) {
  $display = $instance['display'][$view_mode];
  $settings = $display['settings'];
  switch ($display['type']) {
    case 'epub_cover':
      return t('Width: @width', array('@width' => $settings['width'] ? $settings['width'] : '100%'));
    case 'epub_default':
      $args = array(
        '@width' => $settings['width'] ? $settings['width'] : '100%',
        '@height' => $settings['height'] ? $settings['height'] : 'auto',
      );
      return t('Width: @width, Height: @height', $args);
    case 'epub_js':
      $args = array(
        '@width' => $settings['width'] ? $settings['width'] : '100%',
        '@height' => $settings['height'] ? $settings['height'] : 'auto',
      );
      return t('Let epub.js reader use unzipped epub contents from server: @unzipped', array('@unzipped' => $settings['unzipped'] ? t('Yes') : t('No'))) . t('Width: @width, Height: @height', $args);
    case 'epub_js_reader':
      return t('Let epub.js reader use unzipped epub contents from server: @unzipped', array('@unzipped' => $settings['unzipped'] ? t('Yes') : t('No')));
  }
  return '';
}

/**
 * Implements hook_field_formatter_view().
 */
function epub_field_formatter_view($entity_type, $entity, $field, $instance, $langcode, $items, $display) {
  $element = array();
  $settings = $display['settings'];
  switch ($display['type']) {
    case 'epub_cover':
      foreach ($items as $delta => $item) {
        $element[$delta] = array(
          '#theme' => 'epub_formatter_cover',
          '#file' => (object)$item,
          '#width' => $settings['width'],
        );
      }
      break;
    case 'epub_default':
      foreach ($items as $delta => $item) {
        $element[$delta] = array(
          '#theme' => 'epub_formatter_default',
          '#file' => (object)$item,
          '#width' => $settings['width'],
          '#height' => $settings['height'],
        );
      }
      break;
    case 'epub_toc':
      foreach ($items as $delta => $item) {
        $element[$delta] = array(
          '#theme' => 'epub_formatter_toc',
          '#file' => (object)$item,
          '#width' => $settings['width'],
          '#height' => $settings['height'],
        );
      }
      break;
    case 'epub_js':
      foreach ($items as $delta => $item) {
        $element[$delta] = array(
          '#theme' => 'epub_formatter_epubjs',
          '#file' => (object)$item,
          '#unzipped' => $settings['unzipped'],
          '#width' => $settings['width'],
          '#height' => $settings['height'],
        );
      }
      break;
    case 'epub_js_reader':
      foreach ($items as $delta => $item) {
        $element[$delta] = array(
          '#theme' => 'epub_formatter_epubjs_reader',
          '#file' => (object)$item,
          '#unzipped' => $settings['unzipped'],
        );
      }
      break;
  }
  return $element;
}

function theme_epub_formatter_cover($variables) {
  $file_url = file_create_url($variables['file']->uri);
  $filename = $variables['file']->filename;
  $fid = 'fid-' . $variables['file']->fid;

  $dir = 'public://epub_content/' . $variables['file']->fid;
  $file = drupal_realpath($variables['file']->uri);
  $output = t('<p>This eBook can\'t be opened.</p>');

  $scan = file_scan_directory($dir, '/.*\.opf$/');
  $opf = array_shift($scan);
  if (isset($opf)) {
    $opfXML = simplexml_load_file($opf->uri);
    $namespaces = $opfXML->getDocNamespaces();
    $opfXML->registerXPathNamespace('opf', 'http://www.idpf.org/2007/opf');
    $element = $opfXML->xpath('/opf:package/opf:metadata/opf:meta[@name="cover"]');
    if (empty($element)) {
        $variables = array(
          'path' => $GLOBALS['base_url'] . '/misc/help.png',
          'alt' => 'Book cover',
          'title' => $variables['file']->filename,
          'width' => $variables['width'],
          'attributes' => array(
            'class' => 'ebook-cover',
            'id' => $fid,
            'style' => 'border:1px solid black;',
           ),
        );

        $content = '<div class="ebook" id="' . $fid . '">' . l(theme('image', $variables), $file_url, $options = array('html'=>true)) . '</div>';
        $content .= '<p>' . t('A cover image is not required according to the ePub standard') . '</p>';
      }
      else {
      $attributes = $element[0]->attributes();
      $cover = epub_get_item($dir, (string)$attributes['content']);
      $variables = array(
        'path' => $cover,
        'alt' => 'Book cover',
        'title' => $variables['file']->filename,
        'width' => $variables['width'],
        'attributes' => array('
          class' => 'ebook-cover',
          'id' => $fid,
          'style' => 'border:1px solid black;',
        ),
      );
      //$content = l($filename, $file_url);
      $content = '<div class="ebook" id="' . $fid . '">' . l(theme('image', $variables), $file_url, $options = array('html'=>true)) . '</div>';
    }
  }

  return $content;
}

function theme_epub_formatter_default($variables) {
  $file_url = file_create_url($variables['file']->uri);
  $filename = $variables['file']->filename;
  $fid = 'fid-' . $variables['file']->fid;
  if (file_entity_access('download', $variables['file'] )) {
    $download_link = t('Download: ') . l($filename, $file_url);
  }

  $dir = 'public://epub_content/' . $variables['file']->fid;

  $file = drupal_realpath($variables['file']->uri);
  $output = $download_link . t('<p>This eBook can\'t be opened.</p>');
  $scan = file_scan_directory($dir, '/.*\.opf$/');
  $opf = array_shift($scan);
  if (isset($opf)) {
    $opfXML = simplexml_load_file($opf->uri);
    $namespaces = $opfXML->getDocNamespaces();
    $opfXML->registerXPathNamespace('opf', 'http://www.idpf.org/2007/opf');
    $items = array();
    foreach ($opfXML->xpath('/opf:package/opf:spine/opf:itemref') as $key => $value) {
      $attributes = $value->attributes();
      $id = (string)$attributes['idref'];
      $items[$key] = file_create_url(epub_get_item($dir, $id));
    }
    $query = drupal_get_query_parameters();
    if (isset($query['page'])) {
      $current_page = $query['page'];
    }
    else {
      $current_page = 0;
    }
    $total = count($items);
    pager_default_initialize($total, 1);

    $iframe = array(
      '#type' => 'html_tag',
      '#tag' => 'iframe',
      '#value' => '',
      '#attributes' => array(
        'class' => array('epub', 'ebook'),
        'id' => array($fid),
        'webkitallowfullscreen' => '',
        'mozallowfullscreen' => '',
        'allowfullscreen' => '',
        'name' => 'epub',
        'width' => $variables['width'] ? $variables['width'] : '100%',
        'height' => $variables['height'] ? $variables['height'] : '100%',
        'src' => $items[$current_page],
      ),
    );
    $output =  $download_link . render($iframe) . theme('pager');
  }
  drupal_add_css(drupal_get_path('module', 'epub') . '/css/epub.css');
  drupal_add_js(drupal_get_path('module', 'epub') . '/js/epub.js');
  if ($variables['height'] == 'auto') {
    drupal_add_js('Drupal.behaviors.epub.resizeIframe();', array('type' => 'inline', 'scope' => 'footer'));
  }
  return '<div id="ebook" class="epub">' . $output . '</div>';
}

function theme_epub_formatter_toc($variables) {
  $file_url = file_create_url($variables['file']->uri);
  $filename = $variables['file']->filename;
  $dir = 'public://epub_content/' . $variables['file']->fid;
  if (file_entity_access('download', $variables['file'])) {
    $download_link = t('Download: ') . l($filename, $file_url);
  }
  return $download_link . epub_get_toc($dir, '_blank');
}

function theme_epub_formatter_epubjs($variables) {
  global $base_url;
  $filename = $variables['file']->filename;
  $file_url = file_create_url($variables['file']->uri);
  $path = file_create_url('public://epub_content/' . $variables['file']->fid);
  if ($variables['unzipped']) {
    $epub = $base_url . '/sites/all/libraries/epub.js/reader/dev.html?bookPath=' . $path . '/';
  }
  else {
    $epub = $base_url . '/sites/all/libraries/epub.js/reader/dev.html?bookPath=' . $file_url;
  }
  $iframe = array(
    '#type' => 'html_tag',
    '#tag' => 'iframe',
    '#value' => '',
    '#attributes' => array(
      'class' => array('epub', 'ebook'),
      'id' => array('fid-' . $variables['file']->fid),
      'webkitallowfullscreen' => '',
      'mozallowfullscreen' => '',
      'allowfullscreen' => '',
      'name' => 'epub',
      'width' => $variables['width'] ? $variables['width'] : '100%',
      'height' => $variables['height'] ? $variables['height'] : '600px',
      'frameborder' => '0',
      'src' => $epub,
    ),
  );
  $output = render($iframe);// . l(t('Read it online'), $epub , array('attributes' => array('target'=>'_blank')));
  if (file_entity_access('download', $variables['file'])) {
    $output .= t('Download: ') . l($filename, $file_url) . '</div>';
  }
  return $output;
}

function theme_epub_formatter_epubjs_reader($variables) {
  global $base_url;
  $filename = $variables['file']->filename;
  $file_url = file_create_url($variables['file']->uri);
  $path = file_create_url('public://epub_content/' . $variables['file']->fid);
  if ($variables['unzipped']) {
    $epub = $base_url . '/sites/all/libraries/epub.js/reader/index.html?file=' . $path . '/';
  }
  else {
    $epub = $base_url . '/sites/all/libraries/epub.js/reader/index.html?file=' . $file_url;
  }
  $parameters = drupal_get_query_parameters();
  $link = l(t('Read it online'), 'file/' . $variables['file']->fid , array('query' => array('fullscreen' => TRUE), 'attributes' => array('target'=>'_blank')));
  if (module_exists('visitors')) {
    global $user;
    if ($user->uid == 1 || $user->uid == $variables['file']->uid) {
      $query = 'SELECT COUNT(DISTINCT visitors_uid) FROM {visitors} WHERE visitors_uid <> 0 AND visitors_title <> \'Access denied\' AND visitors_path = \'file/' . $variables['file']->fid . '\' AND visitors_url LIKE \'%?fullscreen=1%\'';
      $unique_visitors = db_query($query)->fetchField();
      $link .= '<span> (' . $unique_visitors . ' readers)</span>';
    }
  }
  if (file_entity_access('download', $variables['file'])) {
    $link = '<div>' . $link . '</div><div>' . t('Download: ') . l($filename, $file_url) . '</div>';
  }
  return $link;
}

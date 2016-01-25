<?php
/**
 * @file
 * Contains filter integration for CommonMark.
 */

/**
 * Returns the list of incompatible filters.
 *
 * @return array
 *   A list of incompatible filters.
 */
function _commonmark_incompatible_filters() {
  return [
    'filter_autop',
    'filter_html',
    'filter_html_escape',
    'filter_url',
  ];
}

/**
 * Implements hook_form_FORM_ID_alter().
 */
function commonmark_form_filter_admin_format_form_alter(&$form, &$form_state, $form_id) {
  $form['#attached']['js'][] = drupal_get_path('module', 'commonmark') . '/js/filter.admin.js';
  $form['filters']['status']['commonmark']['#description'] = t('Some filters are not compatible with or implement the same functionality already provided by the CommonMark filter. They will be forcefully disabled. To re-enable any of these filters, you must first disable the CommonMark filter.');
  foreach (_commonmark_incompatible_filters() as $name) {
    $element = &$form['filters']['status'][$name];
    $element['#states']['disabled'][':input[name="filters[commonmark][status]"]'] = array('checked' => TRUE);
    $element['#states']['checked'][':input[name="filters[commonmark][status]"]'] = array('checked' => FALSE);
  }
}

/**
 * Implements hook_filter_info().
 */
function commonmark_filter_info() {
  $filters['commonmark'] = [
    'title' => t('CommonMark'),
    'process callback' => '_commonmark_filter_process',
    'settings callback' => '_commonmark_filter_settings',
    'default settings' => [
      'allowed_tags' => '<a> <abbr> <address> <blockquote> <br> <cite> <code> <dd> <del> <div> <dl> <dt> <em> <h1> <h2> <h3> <h4> <h5> <h6> <hr> <img> <kbd> <li> <ol> <p> <pre> <small> <span> <strong> <sub> <sup> <table> <tbody> <td> <tfoot> <th> <thead> <tr> <ul>',
      'show_examples' => 1,
      'use_global_settings' => 1,
    ],
    'tips callback' => '_commonmark_filter_tips',
    'weight' => -20,
  ];
  return $filters;
}

/**
 * Implements callback_filter_process().
 */
function _commonmark_filter_process($text, $filter, $format, $langcode, $cache, $cache_id) {
  // Convert to HTML.
  $text = commonmark_convert_to_html($text, $filter);

  // Filter anything not in the allowed tags.
  if ($allowed_tags = preg_split('/\s+|<|>/', $filter->settings['allowed_tags'], -1, PREG_SPLIT_NO_EMPTY)) {
    $text = filter_xss($text, $allowed_tags);
  }

  return trim($text);
}

/**
 * Implements callback_filter_settings().
 */
function _commonmark_filter_settings(&$form, &$form_state, $filter, $format, $defaults) {
  $filter->settings += $defaults;
  $form_state['#filter'] = $filter;

  $settings['allowed_tags'] = [
    '#type' => 'textfield',
    '#title' => t('Allowed HTML tags'),
    '#default_value' => $filter->settings['allowed_tags'],
    '#maxlength' => 1024,
    '#description' => t('<p>A list of HTML tags that will be permitted to remain after CommonMark has converted the Markdown to HTML. JavaScript event attributes, JavaScript URLs, and CSS are always stripped.</p><p><strong>WARNING: Modifying this list will have a severe impact on the final output from CommonMark, use with caution.</strong></p>'),
  ];

  $settings['show_examples'] = [
    '#type' => 'checkbox',
    '#title' => t('Show examples in the "long" filter tips.'),
    '#default_value' => $filter->settings['show_examples'],
  ];

  $settings['use_global_settings'] = [
    '#type' => 'checkbox',
    '#title' => t('Use global CommonMark settings'),
    '#description' => t('Uses existing global setting set in the <a href="!config">CommonMark administrative configuration</a> area.', [
      '!config' => url('admin/config/content/commonmark'),
    ]),
    '#default_value' => $filter->settings['use_global_settings'],
  ];

  // Add extension settings overrides to the filter settings.
  $settings = commonmark_extension_settings_form($settings, $form_state, $form);
  $settings['commonmark_extensions']['#states'] = [
    'invisible' => [
      ':input[name="filters[commonmark][settings][use_global_settings]"]' => ['checked' => TRUE],
    ],
  ];

  // Because the admin filter form doesn't explicitly set it's submit callback,
  // by adding our own, we have inadvertently canceled it. Add it back in.
  if (isset($form['#submit']) && !in_array('filter_admin_format_form_submit', $form['#submit'])) {
    $form['#submit'][] = 'filter_admin_format_form_submit';
  }

  return $settings;
}

/**
 * Implements callback_filter_tips().
 *
 * @see _commonmark_filter_tips_build()
 */
function _commonmark_filter_tips($filter, $format, $long = FALSE) {
  // On the "short" tooltips, we can simply call the build array normally.
  if (!$long) {
    $build = _commonmark_filter_tips_build(array(), array(), $filter, $format, $long);
    return drupal_render($build);
  }

  // On the long format, the render array must be retrieved as a "form" due to
  // the fact that vertical tabs require form processing to work properly.
  $build = drupal_get_form('_commonmark_filter_tips_build', $filter, $format, $long);

  // Since we're essentially "hacking" the FAPI and this isn't an actual "form",
  // remove everything that isn't needed to prevent anyone from actually using
  // our custom form and possibly "submitting" it.
  unset($build['form_build_id'], $build['form_id'], $build['form_token']);
  $children = element_children($build);
  foreach (array_keys($build) as $key) {
    if (!in_array($key, $children)) {
      unset($build[$key]);
    }
  }

  // Render the build array.
  return drupal_render($build);
}

/**
 * Builds the tips for CommonMark in vertical tabs.
 *
 * @param array $form
 *   Not used.
 * @param array $form_state
 *   Not used.
 * @param object $filter
 *   An object representing the filter.
 * @param object $format
 *   An object representing the text format the filter is contained in.
 * @param bool $long
 *   Whether this callback should return a short tip to display in a form
 *   (FALSE), or whether a more elaborate filter tips should be returned for
 *   theme_filter_tips() (TRUE).
 *
 * @return array
 *   The build array.
 *
 * @see _commonmark_filter_tips()
 */
function _commonmark_filter_tips_build(array $form, array $form_state, $filter, $format, $long = FALSE) {
  global $base_url;

  $build['description']['#markup'] = '<p>' . t('Converts <a href="!commonmark" target="_blank">CommonMark</a> Markdown into valid HTML.', [
      '!commonmark' => 'http://commonmark.org/',
    ]) . '</p>';

  $allowed_tags = array();
  if (preg_match_all('/<([a-z0-9]+)[^a-z0-9]/i', $filter->settings['allowed_tags'], $matches)) {
    $allowed_tags = $matches[1];
  }

  $build['allowed_tags'] = [
    '#markup' => $allowed_tags ? t('<strong>Allowed HTML tags:</strong><br/><code>@tags</code>', ['@tags' => '<' . implode('> <', $allowed_tags) . '>']) : t('No HTML tags are allowed. CommonMark Markdown will be severely limited.'),
    '#weight' => 10,
  ];

  if (!$long || !$filter->settings['show_examples']) {
    return $build;
  }

  $build['help']['#markup'] = '<p>' . t('This site renders CommonMark Markdown. While learning all of the Markdown syntax may feel intimidating at first, learning how to use a very small number of the most basic Markdown syntax is very easy. The following sections will provide examples for commonly used Markdown syntax, what HTML output it generates and how it will display on the site.') . '</p>';

  $build['groups'] = ['#type' => 'vertical_tabs'];

  // Define default groups.
  $groups = [
    'general' => ['title' => t('General'), 'items' => []],
    'blockquotes' => ['title' => t('Block Quotes'), 'items' => []],
    'code' => ['title' => t('Code'), 'items' => []],
    'headings' => ['title' => t('Headings'), 'items' => []],
    'images' => ['title' => t('Images'), 'items' => []],
    'links' => ['title' => t('Links'), 'items' => []],
    'lists' => ['title' => t('Lists'), 'items' => []],
  ];

// @codingStandardsIgnoreStart
// Ignore Drupal coding standards during this section of code. There are
// multiple concatenated t() strings that need to be ignored.

  // General.
  $groups['general']['items'][] = [
    'title' => t('Paragraphs'),
    'description' => t('Paragraphs are simply one or more consecutive lines of text, separated by one or more blank lines.'),
    'strip_p' => FALSE,
    'tags' => [
      'p' => [ t('Paragraph one.') . "\n\n" . t('Paragraph two.') ],
    ],
  ];
  $groups['general']['items'][] = [
    'title' => t('Line Breaks'),
    'description' => t('If you want to insert a <code>&lt;br /&gt;</code> break tag, end a line with two or more spaces, then type return.'),
    'strip_p' => FALSE,
    'tags' => [
      'br' => [ t("Text with  \nline break") ],
    ],
  ];
  $groups['general']['items'][] = [
    'title' => t('Horizontal Rule'),
    'tags' => [
      'hr' => [ '---', '___', '***' ],
    ],
  ];
  $groups['general']['items'][] = [
    'title' => t('Deleted text'),
    'description' => t('The CommonMark spec does not (yet) have syntax for <code>&lt;del&gt;</code> formatting. You must manually specify them.'),
    'tags' => [
      'del' => '<del>' . t('Deleted') . '</del>',
    ],
  ];
  $groups['general']['items'][] = [
    'title' => t('Emphasized text'),
    'tags' => [
      'em' => [
        '_' . t('Emphasized') . '_',
        '*' . t('Emphasized') . '*',
      ],
    ],
  ];
  $groups['general']['items'][] = [
    'title' => t('Strong text'),
    'tags' => [
      'strong' => [
        '__' . t('Strong', [], ['context' => 'Font weight']) . '__',
        '**' . t('Strong', [], ['context' => 'Font weight']) . '**',
      ],
    ],
  ];

  // Blockquotes.
  $groups['blockquotes']['items'][] = [
    'tags' => [
      'blockquote' => [
        '> ' . t("Block quoted") . "\n\n" . t("Normal text"),
        '> ' . t("Nested block quotes\n>> Nested block quotes\n>>> Nested block quotes\n>>>> Nested block quotes") . "\n\n" . t("Normal text"),
        '> ' . t("Lorem ipsum dolor sit amet, consectetur adipiscing elit. Lorem ipsum dolor sit amet, consectetur adipiscing elit. Lorem ipsum dolor sit amet, consectetur adipiscing elit.") . "\n\n" . t("Normal text"),
      ],
    ],
  ];

  // Code.
  $groups['code']['items'][] = [
    'title' => t('Inline code'),
    'tags' => [
      'code' => '`' . t('Coded') . '`',
    ],
  ];
  $groups['code']['items'][] = [
    'title' => t('Fenced code blocks'),
    'tags' => [
      'pre' => [
        "```\n" . t('Fenced code block') . "\n```",
        "~~~\n" . t('Fenced code block') . "\n~~~",
        "    " . t('Fenced code block - indented using 4+ spaces'),
        "\t" . t('Fenced code block - indented using tab'),
      ],
    ],
  ];
  $groups['code']['items'][] = [
    'title' => t('Fenced code blocks (using languages)'),
    'tags' => [
      'pre' => [
        "```css\n.selector {\n  color: #ff0;\n  font-size: 10px;\n  content: 'string';\n}\n```",
        "```js\nvar \$selector = \$('#id');\n\$selector.foo('bar', {\n  'baz': true,\n  'value': 1\n});\n```",
        "```php\n\$build['table'] = array(\n  '#theme' => 'table',\n  '#header' => \$header,\n  '#rows' => \$rows,\n  '#sticky' => FALSE,\n);\nprint drupal_render(\$build);\n```"
      ],
    ],
  ];

  // Headings.
  $groups['headings']['items'][] = [
    'tags' => [
      'h1' => '# ' . t('Heading 1'),
      'h2' => '## ' . t('Heading 2'),
      'h3' => '### ' . t('Heading 3'),
      'h4' => '#### ' . t('Heading 4'),
      'h5' => '##### ' . t('Heading 5'),
      'h6' => '###### ' . t('Heading 6'),
    ],
  ];

  // Images.
  $groups['images']['items'][] = [
    'title' => t('Images'),
    'tags' => [
      'img' => [
        '![' . t('Alt text') . '](http://lorempixel.com/400/200/ "' . t('Title text') . '")',
      ],
    ],
  ];
  $groups['images']['items'][] = [
    'title' => t('Referenced images'),
    'strip_p' => FALSE,
    'tags' => [
      'img' => [
        "Lorem ipsum dolor sit amet\n\n![" . t('Alt text') . "]\n\nLorem ipsum dolor sit amet\n\n[" . t('Alt text') . ']: http://lorempixel.com/400/200/ "' . t('Title text') . '"',
      ],
    ],
  ];

  // Links
  $site_name = variable_get('site_name', 'Drupal');
  $site_mail = variable_get('site_mail', ini_get('sendmail_from'));
  $groups['links']['items'][] = [
    'title' => t('Links'),
    'tags' => [
      'a' => [
        "<$base_url>",
        "[$site_name]($base_url)",
        "<$site_mail>",
        "[Email: $site_name](mailto:$site_mail)",
      ],
    ],
  ];
  $groups['links']['items'][] = [
    'title' => t('Referenced links'),
    'description' => t('Link references are very useful if you use the same words through out a document and wish to link them all to the same link.'),
    'tags' => [
      'a' => [
        "[$site_name]\n\n[$site_name]: $base_url \"" . t('My title') . '"',
        "Lorem ipsum [dolor] sit amet, consectetur adipiscing elit. Lorem ipsum [dolor] sit amet, consectetur adipiscing elit. Lorem ipsum [dolor] sit amet, consectetur adipiscing elit.\n\n[dolor]: $base_url \"" . t('My title') . '"',
      ],
    ],
  ];
  $groups['links']['items'][] = [
    'title' => t('Fragments (anchors)'),
    'tags' => [
      'a' => [
        "[$site_name]($base_url#fragment)",
        "[$site_name](#element-id)",
      ],
    ],
  ];

  // Lists.
  $groups['lists']['items'][] = [
    'title' => t('Ordered lists'),
    'tags' => [
      'ol' => [
        "1. " . t('First item') . "\n2. " . t('Second item') . "\n3. " . t('Third item') . "\n4. " . t('Fourth item'),
        "1) " . t('First item') . "\n2) " . t('Second item') . "\n3) " . t('Third item') . "\n4) " . t('Fourth item'),
        "1. " . t('All start with 1') . "\n1. " . t('All start with 1') . "\n1. " . t('All start with 1') . "\n1. " . t('Rendered with correct numbers'),
        "1. " . t('First item') . "\n2. " . t('Second item') . "\n   1. " . t('First nested item') . "\n   2. " . t('Second nested item') . "\n      1. " . t('Deep nested item'),
        "5. " . t('Start at fifth item') . "\n6. " . t('Sixth item') . "\n7. " . t('Seventh item') . "\n8. " . t('Eighth item'),
      ],
    ],
  ];
  $groups['lists']['items'][] = [
    'title' => t('Unordered lists'),
    'tags' => [
      'ul' => [
        "- " . t('First item') . "\n- " . t('Second item'),
        "- " . t('First item') . "\n- " . t('Second item') . "\n  - " . t('First nested item') . "\n  - " . t('Second nested item') . "\n    - " . t('Deep nested item'),
        "* " . t('First item') . "\n* " . t('Second item'),
        "+ " . t('First item') . "\n+ " . t('Second item'),
      ]
    ],
  ];

// @codingStandardsIgnoreEnd

  // Allow extensions to provide additional groups/group items.
  $filter_clone = clone $filter;
  $format_clone = $format;
  $long_clone = $long;
  foreach (commonmark_get_extensions($filter) as $extension) {
    if (module_hook($extension['module'], 'commonmark_extension_tips')) {
      $function = $extension['module'] . '_commonmark_extension_tips';
      if (function_exists($function)) {
        $function($extension, $groups, $build, $filter_clone, $format_clone, $long_clone);
      }
    }
  }

  // Iterate over all the items.
  $converter = commonmark_get_converter($filter);
  $header = [check_plain($format->name) . ' / ' . t('HTML Output'), t('Rendered')];
  foreach ($groups as $group_name => $group) {
    // Build the group.
    $build['groups'][$group_name] = [
      '#title' => $group['title'],
      '#type' => 'fieldset',
    ];

    // Build group items.
    foreach ($group['items'] as $key => $item) {
      // Build the group item title.
      if (!empty($item['title'])) {
        $build['groups'][$group_name][$key]['title'] = [
          '#type' => 'html_tag',
          '#theme' => ['html_tag__commonmark_tip_title', 'html_tag'],
          '#tag' => 'h4',
          '#attributes' => ['class' => ['title']],
          '#value' => $item['title'],
        ];
      }

      // Build the group item description.
      if (!empty($item['description'])) {
        if (!is_array($item['description'])) {
          $item['description'] = array($item['description']);
        }
        if (count($item['description']) === 1) {
          $build['groups'][$group_name][$key]['description'] = [
            '#type' => 'html_tag',
            '#theme' => ['html_tag__commonmark_tip_description', 'html_tag'],
            '#tag' => 'div',
            '#attributes' => ['class' => ['description']],
            '#value' => $item['description'][0],
          ];
        }
        else {
          $build['groups'][$group_name][$key]['description'] = [
            '#theme' => ['item_list__commonmark_tip_description', 'item_list'],
            '#attributes' => ['class' => ['description']],
            '#items' => $item['description'],
          ];
        }
      }

      // Only continue if there are tags.
      if (empty($item['tags'])) {
        continue;
      }

      // Skip item if none of the tags are allowed.
      $item_tags = array_keys($item['tags']);
      $item_tags_not_allowed = array_diff($item_tags, $allowed_tags);
      if (count($item_tags_not_allowed) === count($item_tags)) {
        continue;
      }

      // Remove any tags not allowed.
      foreach ($item_tags_not_allowed as $tag) {
        unset($item['tags'][$tag]);
        unset($item['titles'][$tag]);
        unset($item['descriptions'][$tag]);
      }

      $rows = [];
      foreach ($item['tags'] as $tag => $examples) {
        if (!is_array($examples)) {
          $examples = array($examples);
        }
        foreach ($examples as $markdown) {
          $row = [];
          $rendered = $converter->convertToHtml($markdown);
          if (!isset($item['strip_p']) || !empty($item['strip_p'])) {
            $rendered = preg_replace('/^<p>|<\/p>\n?$/', '', $rendered);
          }

          $row[] = [
            'data' => '<pre><code class="language-markdown show-whitespace">' . check_plain($markdown) . '</code></pre><hr/><pre><code class="language-html show-whitespace">' . trim(check_plain($rendered)) . '</code></pre>',
            'style' => 'padding-right: 2em; vertical-align: middle; width: 66.666%',
          ];
          $row[] = [
            'data' => $rendered,
            'style' => 'vertical-align: middle; width: 33.333%',
          ];

          $rows[] = $row;
        }
      }

      $build['groups'][$group_name][$key]['tags'] = [
        '#theme' => ['table__commonmark_tip', 'table'],
        '#header' => $header,
        '#rows' => $rows,
        '#sticky' => FALSE,
      ];
    }
  }

  // Remove empty groups.
  foreach (element_children($build['groups']) as $child) {
    if (!element_children($build['groups'][$child])) {
      unset($build['groups'][$child]);
    }
  }

  $entities = [
    '&amp;' => t('Ampersand'),
    '&bull;' => t('Bullet'),
    '&cent;' => t('Cent'),
    '&copy;' => t('Copyright sign'),
    '&dagger;' => t('Dagger'),
    '&Dagger;' => t('Dagger (double)'),
    '&mdash;' => t('Dash (em)'),
    '&ndash;' => t('Dash (en)'),
    '&euro;' => t('Euro sign'),
    '&hellip;' => t('Horizontal ellipsis'),
    '&gt;' => t('Greater than'),
    '&lt;' => t('Less than'),
    '&middot;' => t('Middle dot'),
    '&nbsp;' => t('Non-breaking space'),
    '&para;' => t('Paragraph'),
    '&permil;' => t('Per mille sign'),
    '&pound;' => t('Pound sterling sign (GBP)'),
    '&reg;' => t('Registered trademark'),
    '&quot;' => t('Quotation mark'),
    '&trade;' => t('Trademark'),
    '&yen;' => t('Yen sign'),
  ];
  $rows = [];
  foreach ($entities as $entity => $description) {
    $rows[] = [
      $description,
      '<code>' . check_plain($entity) . '</code>',
      $entity,
    ];
  }

  $build['groups']['entities'] = [
    '#title' => t('HTML Entities'),
    '#type' => 'fieldset',
    'description' => [
      '#markup' => '<p>' . t('Most unusual characters can be directly entered without any problems.') . '</p>' . '<p>' . t('If you do encounter problems, try using HTML character entities. A common example looks like &amp;amp; for an ampersand &amp; character. For a full list of entities see HTML\'s <a href="@html-entities">entities</a> page. Some of the available characters include:', ['@html-entities' => 'http://www.w3.org/TR/html4/sgml/entities.html']) . '</p>'
    ],
    'table' => [
      '#theme' => 'table',
      '#header' => [t('Entity'), t('HTML code'), t('Rendered')],
      '#rows' => $rows,
      '#sticky' => FALSE,
    ]
  ];

  $build['allowed_tags'] = [
    '#title' => t('Allowed HTML Tags'),
    '#type' => 'fieldset',
    '#group' => 'groups',
    '#weight' => 10,
    'tags' => $build['allowed_tags'],
  ];

  return $build;
}

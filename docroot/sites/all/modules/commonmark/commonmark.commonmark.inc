<?php
/**
 * @file
 * The "commonmark" group hook definitions.
 *
 * @see commonmark_hook_info()
 */

use Drupal\CommonMark\Environment;

/**
 * Implements hook_commonmark_extension_info().
 */
function commonmark_commonmark_extension_info() {
  global $base_url;
  $extensions['enhanced_links'] = [
    'title' => t('Enhanced Links'),
    'description' => t('Extends CommonMark to provide additional enhancements when rendering links.'),
    'class' => '\Drupal\CommonMark\Inline\LinkRenderer',
    'method arguments' => ['Link', Environment::EXTENSION_INSTANCE_PLACEHOLDER],
    'settings' => [
      'external_new_window' => TRUE,
      'internal_host_whitelist' => parse_url($base_url, PHP_URL_HOST),
      'no_follow' => 'external',
    ],
  ];
  $extensions['at_autolinker'] = [
    'title' => t('@ Autolinker'),
    'enabled' => FALSE,
    'description' => t('Automatically link commonly used references that come after an at character (@) without having to use the link syntax.'),
    'class' => '\Drupal\CommonMark\Inline\AtAutolinker',
    'settings' => [
      'type' => 'user',
      'format_username' => TRUE,
      'url' => 'https://www.drupal.org/u/[text]',
    ],
  ];
  $extensions['hash_autolinker'] = [
    'title' => t('# Autolinker'),
    'enabled' => FALSE,
    'description' => t('Automatically link commonly used references that come after a hash character (#) without having to use the link syntax.'),
    'class' => '\Drupal\CommonMark\Inline\HashAutolinker',
    'settings' => [
      'type' => 'node',
      'node_title' => TRUE,
      'url' => 'https://www.drupal.org/node/[text]',
      'url_title' => TRUE,
    ],
  ];
  return $extensions;
}

/**
 * Implements hook_commonmark_extension_settings().
 */
function commonmark_commonmark_extension_settings(array $extension, array $element, array &$form_state, array &$form) {
  global $base_url;

  $filter = isset($form_state['#filter']) ? $form_state['#filter'] : NULL;

  switch ($extension['name']) {
    case 'at_autolinker':
      $selector = _commonmark_get_states_selector($filter, $extension, 'type');
      $element['type'] = [
        '#type' => 'select',
        '#title' => t('Map text to'),
        '#default_value' => $extension['settings']['type'],
        '#options' => [
          'user' => t('User'),
          'url' => t('URL'),
        ],
      ];
      $element['format_username'] = [
        '#type' => 'checkbox',
        '#title' => t('Replace text with formatted username'),
        '#description' => t('If enabled, it will replace the matched text with the formatted username.'),
        '#default_value' => $extension['settings']['format_username'],
        '#states' => [
          'visible' => [
            $selector => ['value' => 'user'],
          ],
        ],
      ];
      $element['url'] = [
        '#type' => 'textfield',
        '#title' => t('URL'),
        '#description' => t('A URL to format text with. Use the token "[text]" where it is needed. If you need to include the @, use the URL encoded equivalent: <code>%40</code>. Example: <code>https://twitter.com/search?q=%40[text]</code>.'),
        '#default_value' => $extension['settings']['url'],
        '#states' => [
          'visible' => [
            $selector => ['value' => 'url'],
          ],
        ],
      ];
      break;

    case 'enhanced_links':
      $element['#description'] = '<p>' . $element['#description'] . '</p><p><strong>' . t('NOTE: These settings ONLY apply to CommonMark Markdown links, if a user manually enters an <code>&lt;a&gt;</code> tag, then these settings will not be processed on them.') . '</strong></p>';

      $element['internal_host_whitelist'] = [
        '#type' => 'textarea',
        '#title' => t('Internal Host Whitelist'),
        '#description' => t('Allows additional host names to be treated as "internal" when they would normally be considered as "external". This is useful in cases where a multi-site is using different sub-domains. The current host name, %host, will always be considered "internal" (even if removed from this list). Enter one host name per line. No regular expressions are allowed, just exact host name matches.', [
          '%host' => parse_url($base_url, PHP_URL_HOST),
        ]),
        '#default_value' => $extension['settings']['internal_host_whitelist'],
      ];
      $element['external_new_window'] = [
        '#type' => 'checkbox',
        '#title' => t('Open external links in new windows'),
        '#description' => t('When this setting is enabled, any link that does not contain one of the above internal whitelisted host names will automatically be considered as an "external" link. All external links will then have the <code>target="_blank"</code> attribute and value added to it.'),
        '#default_value' => $extension['settings']['external_new_window'],
      ];
      $element['no_follow'] = [
        '#type' => 'select',
        '#title' => t('Add <code>rel="nofollow"</code> to'),
        '#description' => t('The rel="nofollow" attribute and value instructs some search engines that the link should not influence the ranking of the link\'s target in the search engine\'s index.'),
        '#default_value' => $extension['settings']['no_follow'],
        '#options' => [
          '' => t('None of the links'),
          'all' => t('All of the links'),
          'external' => t('External links only'),
          'internal' => t('Internal links only'),
        ]
      ];
      break;

    case 'hash_autolinker':
      $selector = _commonmark_get_states_selector($filter, $extension, 'type');
      $element['type'] = [
        '#type' => 'select',
        '#title' => t('Map text to'),
        '#default_value' => $extension['settings']['type'],
        '#options' => [
          'node' => t('Node'),
          'url' => t('URL'),
        ],
      ];
      $element['node_title'] = [
        '#type' => 'checkbox',
        '#title' => t('Replace text with title of node'),
        '#description' => t('If enabled, it will replace the matched text with the title of the node.'),
        '#default_value' => $extension['settings']['node_title'],
        '#states' => [
          'visible' => [
            $selector => ['value' => 'node'],
          ],
        ],
      ];
      $element['url'] = [
        '#type' => 'textfield',
        '#title' => t('URL'),
        '#description' => t('A URL to format text with. Use the token "[text]" where it is needed. If you need to include the #, use the URL encoded equivalent: <code>%23</code>. Example: <code>https://twitter.com/search?q=%23[text]</code>.'),
        '#default_value' => $extension['settings']['url'],
        '#states' => [
          'visible' => [
            $selector => ['value' => 'url'],
          ],
        ],
      ];
      $element['url_title'] = [
        '#type' => 'checkbox',
        '#title' => t('Replace text with title of URL'),
        '#description' => t('If enabled, it will replace the matched text with the title of the URL.'),
        '#default_value' => $extension['settings']['url_title'],
        '#states' => [
          'visible' => [
            $selector => ['value' => 'url'],
          ],
        ],
      ];
      break;
  }

  return $element;
}

/**
 * Implements hook_commonmark_extension_settings().
 */
function commonmark_commonmark_extension_tips(array $extension, array &$groups, array &$build, $filter, $format, $long) {
  global $base_url;
  global $user;

  // @codingStandardsIgnoreStart Ignore coding standards during this section
  // of code. Concatenated t() strings are ok here.
  switch ($extension['name']) {
    case 'at_autolinker':
      if ($extension['settings']['type'] === 'user') {
        if (!$user || !$user->uid) {
          $user = user_load(1);
        }
        $description = [t('Text that starts with an at symbol (@) followed by any character other than a space will be automatically linked to users on this site.')];
        if ($extension['settings']['format_username']) {
          $description[] = t('The formatted user name will be used in place of the text.');
        }
        $description[] = t('If the user does not exist, it will not automatically link.');
        $groups['links']['items'][] = [
          'title' => t('@ Autolinker'),
          'description' => $description,
          'tags' => [
            'a' => "@$user->name",
          ],
        ];
      }
      elseif ($extension['settings']['type'] === 'url') {
        $groups['links']['items'][] = [
          'title' => t('@ Autolinker'),
          'description' => t('Text that starts with an at symbol (@) followed by any character other than a space will automatically be linked to the following URL: <code>@url</code>', [
            '@url' => $extension['settings']['url'],
          ]),
          'tags' => [
            'a' => [
              '@dries',
            ],
          ],
        ];
      }
      break;

    case 'enhanced_links':
      if (!isset($groups['links']['items'][0]['description'])) {
        $groups['links']['items'][0]['description'] = [];
      }
      elseif (isset($groups['links']['items'][0]['description']) && !is_array($groups['links']['items'][0]['description'])) {
        $groups['links']['items'][0]['description'] = [$groups['links']['items'][0]['description']];
      }

      if ($extension['settings']['external_new_window']) {
        $groups['links']['items'][0]['description'][] = '<p>' . t('All external links will open in a new window or tab.') . '</p>';
        $groups['links']['items'][0]['tags']['a'][] = '[' . t('External link opens in new window') . '](http://example.com)';
      }
      else {
        $groups['links']['items'][0]['tags']['a'][] = '[' . t('External link') . '](http://example.com)';
      }

      if ($extension['settings']['no_follow'] === 'all') {
        $groups['links']['items'][0]['description'][] = '<p>' . t('All links will have the <code>rel="nofollow"</code> attribute applied to it.') . '</p>';
      }
      elseif ($extension['settings']['no_follow'] === 'external') {
        $groups['links']['items'][0]['description'][] = '<p>' . t('All external links will have the <code>rel="nofollow"</code> attribute applied to it.') . '</p>';
      }
      elseif ($extension['settings']['no_follow'] === 'internal') {
        $groups['links']['items'][0]['description'][] = '<p>' . t('All internal links will have the <code>rel="nofollow"</code> attribute applied to it.') . '</p>';
      }

      // Determine the internal whitelist host names.
      $hosts = preg_split("/\r\n|\n/", $extension['settings']['internal_host_whitelist'], -1, PREG_SPLIT_NO_EMPTY);

      // Ensure that the site's base url host name is always in this whitelist.
      $base_host = parse_url($base_url, PHP_URL_HOST);
      $key = array_search($base_host, $hosts);
      if ($key === FALSE) {
        $hosts[] = $base_host;
      }

      // Only show this description if there are multiple host names.
      if (count($hosts) > 1) {
        foreach ($hosts as &$host) {
          $host = '<code>' . check_plain($host) . '</code>';
        }
        $groups['links']['items'][0]['description'][] = '<p>' . t('Links with the following URL host names will be treated as "internal" links: !hosts.', [
          '!hosts' => implode(', ', $hosts),
        ]) . '</p>';
      }

      // Manual implementations.
      $site_name = variable_get('site_name', 'Drupal');
      $groups['links']['items'][] = [
        'title' => t('Manual links (using raw HTML)'),
        'description' => t('The above examples are only for links using the Markdown syntax. Manually defined links using raw HTML are always processed "as is". You will be responsible for adding all attributes and values. Suffice it to say, it is always best to avoid using raw HTML and instead use the Markdown syntax whenever possible.'),
        'tags' => [
          'a' => [
            "<a href=\"$base_url\">$site_name</a>",
            "<a href=\"http://example.com\">External link</a>",
            "<a href=\"$base_url\" target=\"_blank\" rel=\"nofollow\">$site_name</a>",
            "<a href=\"http://example.com\" target=\"_blank\" rel=\"nofollow\">External link</a>",
          ],
        ],
      ];
      break;

    case 'hash_autolinker':
      if ($extension['settings']['type'] === 'node') {
        $description = [t('Text that starts with hash symbol (#) followed by numbers will be automatically be linked to a node on this site.')];
        if ($extension['settings']['node_title']) {
          $description[] = t('The node title will be used in place the text.');
        }
        $description[] = t('If the node does not exist, it will not automatically link.');
        $groups['links']['items'][] = [
          'title' => t('# Autolinker'),
          'description' => $description,
        ];
      }
      elseif ($extension['settings']['type'] === 'url') {
        $description = [t('Text that starts with a hash symbol (#) followed by any character other than a space will automatically be linked to the following URL: <code>@url</code>', [
          '@url' => $extension['settings']['url'],
        ])];
        if ($extension['settings']['url_title']) {
          $description[] = t('The URL title will be used in place of the original text.');
        }
        $groups['links']['items'][] = [
          'title' => t('@ Autolinker'),
          'description' => $description,
          'tags' => [
            'a' => [
              '#3060',
              '#2562913',
              '#259843',
            ],
          ],
        ];
      }
      break;
  }
  // @codingStandardsIgnoreEnd
}

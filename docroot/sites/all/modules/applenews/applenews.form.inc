<?php
/**
 * @file
 * Form hook implementations.
 */

/**
 * Implements hook_form_alter().
 */
function applenews_form_alter(&$form, &$form_state, $form_id) {

  if (preg_match('/_node_form$/', $form_id)) {
    _applenews_form_alter_node_form($form, $form_state);
    $form['#submit'][] = '_applenews_form_alter_node_form_submit';
  }

}

/**
 * Entity edit form alter callback.
 */
function _applenews_form_alter_node_form(&$form, &$form_state) {
  module_load_include('inc', 'applenews');
  $entity_type = 'node';
  $bundle = $form['#bundle'];

  // Check if all required libraries installed, otherwise
  // display message about missing PHP libraries.
  if ($info = applenews_libraries_installed()) {
    $form['applenews'] += $info;
    return;
  }

  // Entity object.
  if ($entity = menu_get_object()) {
    $entity = menu_get_object();
    $entity_id = $entity->nid;
    // Use entity ID value as revision ID.
    $revision_id = $entity_id;
  };

  // Exports.
  $exports = applenews_exports_enabled();
  $export_options = [];
  if (count($exports)) {
    foreach ($exports as $key => $info) {
      $export = applenews_export_load($key);
      $valid = $export->showOnEdit($entity_type, $bundle) && (
          !isset($entity_id) ||
          applenews_entity_export_valid($export, $entity_type, $entity_id)
        );
      if (!$valid) {
        continue;
      }
      if (empty($default_export)) {
        $default_export = $export;
      }
      $export_options[$key] = $export->title;
    }
    if (!empty($entity_id)) {
      $default_export = applenews_entity_get_export($entity_type, $entity_id);
    }
  }

  $api_channels = applenews_channels(TRUE);
  $channels_all = applenews_channels();

  // Load existing Apple News article.
  if (!empty($entity_id) && !empty($revision_id)) {
    $post = new ApplenewsDatabase();
    if (!empty($channels_all)) {
      $channel_ids = array_keys($channels_all);
      $post->setChannelId($channel_ids);
    }
    $post->setPostId($entity_type, $entity_id, $revision_id);
    $article = $post->getArticle();
    $article_id = !empty($article['article_id']) ? $article['article_id'] : NULL;
  }

  $exportable = isset($default_export) &&
    $default_export->showOnEdit($entity_type, $bundle) &&
    $default_export->enabled &&
    count($api_channels);

  // Entity form JS helper (hide/show Apple News entity form options).
  $form['#attached']['js'][] = drupal_get_path('module', 'applenews') . '/js/form.js';

  // Add Apple News vertical tab.
  $form['applenews'] = [
    '#type' => 'fieldset',
    '#title' => t('Apple News'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
    '#group' => 'additional_settings',
    '#weight' => -10,
    '#tree' => TRUE,
    '#attributes' => [
      'class' => ['applenews-options'],
    ]
  ];

  // Push.
  if ($exportable) {
    if (!empty($entity->changed) && !empty($article_id) && !empty($article['postdate'])) {
      $label = t('Publish updates to Apple News');
      if ($entity->created != $entity->changed && $entity->changed != $article['postdate']) {
        $description = t('The article has local changes that are not published to Apple News.');
      }
      else {
        $description = t('Apple News published article is in sync with the current content.');
      }
    }
    else {
      $label = t('Publish new content to Apple News');
      $description = '';
    }
    $form['applenews']['applenews_publish_flag'] = [
      '#type' => 'checkbox',
      '#title' => $label,
      '#prefix' => '<div><strong>' . $description . '</strong></div>',
      '#description' => t('Check this option if you would like to post this content to Apple News.'),
    ];
    $form['applenews']['applenews_publish_flag']['#default_value']
      = !empty($article_id) ? TRUE : FALSE;
  }

  // Pushed.
  if (!empty($article_id) && !empty($article) && !empty($entity_id)) {
    // Post date.
    $form['applenews']['post_date'] = [
      '#markup' => '<div class="applenews-post-date">' . format_date($article['postdate'], 'short') . '</div><br/>',
      '#prefix' => '<div><strong>' . t('Apple News post date') . '</strong></div>',
    ];
    $form['applenews']['share-url'] = [
      '#markup' => '<div class="applenews-share-url">' . l($article['share_url'], $article['share_url'],
          [
            'attributes' => [
              'target' => '_blank'
            ]
          ]
        )
        . '</div><br/>',
      '#prefix' => '<div><strong>' . t('Share URL') . '</strong></div>',
    ];
    // Delete.
    if (count($api_channels)) {
      $link = l(t('Delete'), 'admin/config/content/applenews/'
        . $entity_type . '/'
        . $entity_id . '/'
        . $article['article_id'] . '/'
        . reset($channel_ids)
        . '/delete');
      $form['applenews']['delete'] = [
        '#markup' => '<div class="applenews-delete-link">' . t('!link this article from Apple News.', ['!link' => $link]) . '</div><br/>',
        '#prefix' => '<div><strong>' . t('Delete') . '</strong></div>',
      ];
    }
  }

  // Exports, channels, preview.
  if (empty($default_export)) {
    if (count($exports)) {
      $msg = t('No exports are !link for this node.',
        ['!link' => l(t('configured'), 'admin/config/content/applenews')]);
    }
    else {
      $msg = t('No exports are !link.',
        ['!link' => l(t('enabled'), 'admin/config/content/applenews')]);
    }
    $form['applenews']['exports'] = [
      '#markup' => '<div class="applenews-preview">' . $msg . '</div><br/>',
    ];
  }
  else {
    $form['applenews']['exports'] = [
      '#type' => 'select',
      '#title' => t('Export'),
      '#description' => t('!link define how content is pushed to Apple News.', [
        '!link' => l(t('Exports'), 'admin/config/content/applenews')
      ]),
      '#options' => $export_options,
      '#default_value' => empty($default_export) ? NULL : $default_export->id,
    ];
    // When new content show export dropdown when the checkbox is selected.
    if (empty($article_id)) {
      $form['applenews']['exports']['#states'] = [
        'invisible' => [
          ':input[name="applenews[applenews_publish_flag]"]' => ['checked' => FALSE],
        ]
      ];
    }

    $export_config_form = $default_export->getConfigForm();

    // Get configuration form for the AppleNewsAPI exportable entity.
    $channels_element = $export_config_form['additional_settings']['metadata_tab']['channels'];
    // Set Apple News default values for current entity.
    if (!empty($article) && !empty($article_id) && count($api_channels)) {
      foreach (element_children($channels_element) as $key) {
        unset($channels_element[$key]['#default_value']);
      }
      foreach ($article['sections'] as $section_id => $channel_id) {
        $key = 'channel-' . $channel_id;
        if (!empty($channels_element[$key])) {
          $channels_element[$key]['#default_value'] = TRUE;
        }
        $key = 'section-' . $channel_id . '-' . $section_id;
        if (!empty($channels_element[$key])) {
          $channels_element[$key]['#default_value'] = TRUE;
        }
      }
    }
    $form['applenews']['channels'] = &$channels_element;
    unset($form['applenews']['channels']['#weight']);
    // If entity edit.
    if (!empty($entity_id) && !empty($revision_id)) {
      $form['applenews']['channels']['#attributes']['class'][] = 'applenews-channels-edit-form';
    }
    else {
      $form['applenews']['channels']['#attributes']['class'][] = 'applenews-channels-add-form';
    }
    $form['applenews']['channels'][0]['#markup'] = '<div class="description">' . t('Apple News channels and sections where this content will be published.') . '</div>';

    // Is Preview.
    $is_preview = $export_config_form['additional_settings']['metadata_tab']['is_preview'];
    unset($is_preview['#description']);
    $is_preview['#title'] = t('Make this content visbile to members of my channels only.');
    $is_preview['#description'] = t('Indicates whether this article should be public (live) or should be a preview that is only visible to members of your channel.');
    $form['applenews']['is_preview'] = &$is_preview;
    unset($form['applenews']['is_preview']['#weight']);

    // Preview (Downloadable atchives).
    if (@$default_export->enabled && !empty($entity_id) && !empty($revision_id)) {
      $link = l(t('Download'), 'admin/config/content/applenews/' . $entity_type . '/' . $entity_id . '/' . $revision_id . '/download');
      $msg = t('!link the Apple News generated document (use the News Preview app to preview the article).', ['!link' => $link]);
      $form['applenews']['preview'] = [
        '#markup' => '<div class="applenews-preview">' . $msg . '</div><br/>',
        '#prefix' => '<div><strong>' . t('Preview') . '</strong></div>',
      ];
    }
  }

}

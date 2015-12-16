<?php

/**
 * @file
 * Admin pages for Social Timeline.
 */

/**
 * Admin interface for the Social Time module.
 *
 * @return array
 *   Array form for the admin page.
 */
function social_timeline_admin_form($form, &$form_state) {
  // Set the limit drop down values.
  $limit = drupal_map_assoc(range(1, 100));

  $form['#prefix'] = '<div id="st-form">';
  $form['#suffix'] = '</div>';

  // General settings.
  $form['social_timeline_global']['#tree'] = TRUE;
  $form['social_timeline_global']['#weight'] = 1;
  $form['social_timeline_global']['#collapsible'] = TRUE;
  $form['social_timeline_global']['#type'] = 'fieldset';
  $form['social_timeline_global']['#title'] = t('General Settings');

  // Get the global settings.
  $st_global = variable_get('social_timeline_global', array());

  // Set the general settings.
  $form['social_timeline_global']['skin'] = array(
    '#type' => 'select',
    '#title' => t('Skin'),
    '#options' => array(
      'light' => t('Light'),
      'dark' => t('Dark'),
    ),
    '#description' => t('Select the skin style'),
    '#default_value' => (isset($st_global['skin'])) ? $st_global['skin'] : 'light',
  );

  $form['social_timeline_global']['layout'] = array(
    '#type' => 'select',
    '#title' => t('Layout Mode'),
    '#options' => array(
      'timeline' => t('Timeline'),
      'columns' => t('Columns'),
      'one_column' => t('One Column'),
    ),
    '#description' => t('Select the layout mode'),
    '#default_value' => (isset($st_global['layout'])) ? $st_global['layout'] : 'timeline',
  );

  $form['social_timeline_global']['total'] = array(
    '#type' => 'textfield',
    '#title' => t('Total'),
    '#description' => t('Total number of items to retrieve'),
    '#default_value' => (isset($st_global['total'])) ? $st_global['total'] : 10,
    '#size' => 4,
  );

  $form['social_timeline_global']['items_width'] = array(
    '#type' => 'textfield',
    '#title' => t('Items Width'),
    '#description' => t('Set the width of each item in the timeline (in pixels)'),
    '#default_value' => (isset($st_global['items_width'])) ? $st_global['items_width'] : 200,
    '#size' => 4,
  );

  $form['social_timeline_global']['magnific_popup'] = array(
    '#type' => 'checkbox',
    '#title' => t('Add MagnificPopup'),
    '#description' => t('Add MagnificPopup support for images and videos'),
    '#default_value' => (empty($st_global['magnific_popup'])) ? 0 : 1,
  );

  $form['social_timeline_global']['social_icons'] = array(
    '#type' => 'checkbox',
    '#title' => t('Show Social Icons'),
    '#description' => t('Set if you want to show the social icons'),
    '#default_value' => (empty($st_global['social_icons'])) ? 0 : 1,
  );

  $form['social_timeline_global']['show_filter'] = array(
    '#type' => 'checkbox',
    '#title' => t('Show Filter'),
    '#description' => t('Set if you want to show the filter buttons'),
    '#default_value' => (empty($st_global['show_filter'])) ? 0 : 1,
  );

  $form['social_timeline_global']['show_layout'] = array(
    '#type' => 'checkbox',
    '#title' => t('Show Layout'),
    '#description' => t('Set if you want to show the layout buttons'),
    '#default_value' => (empty($st_global['show_layout'])) ? 0 : 1,
  );

  $form['social_timeline_global']['show_share_buttons'] = array(
    '#type' => 'checkbox',
    '#title' => t('Show "Share" Buttons'),
    '#description' => t('Set if you want to show the share buttons'),
    '#default_value' => (empty($st_global['show_share_buttons'])) ? 0 : 1,
  );

  // Get the availalbe default feeds.
  $feeds = _social_timeline_get_feeds();

  // Add default feed form elements.
  $form['social_timeline']['#tree'] = TRUE;
  $form['social_timeline']['#weight'] = -2;
  $form['social_timeline']['#type'] = 'container';
  $st_feeds = variable_get('social_timeline', array());

  foreach ($feeds as $k => $v) {
    $form['social_timeline'][$k] = array(
      'feed' => array(
        '#markup' => $v['title'],
      ),
      'data' => array(
        '#type' => 'textfield',
        '#title' => check_plain($v['data']),
        '#default_value' => (isset($st_feeds[$k]['data'])) ? $st_feeds[$k]['data'] : NULL,
      ),
      'limit' => array(
        '#type' => 'select',
        '#title' => t('Limit'),
        '#options' => $limit,
        '#default_value' => (isset($st_feeds[$k]['limit'])) ? $st_feeds[$k]['limit'] : 5,
      ),
      'weight' => array(
        '#type' => 'weight',
        '#title' => t('Weight'),
        '#default_value' => (isset($st_feeds[$k]['weight'])) ? $st_feeds[$k]['weight'] : 0,
        '#delta' => 10,
        '#title-display' => 'invisible',
      ),
      'active' => array(
        '#type' => 'checkbox',
        '#default_value' => (isset($st_feeds[$k]['active'])) ? $st_feeds[$k]['active'] : 0,
      ),
    );
  }

  // Set the limit drop down values.
  $limit = array();
  for ($i = 1; $i <= 100; $i++) {
    $limit[$i] = $i;
  }

  if ($form_state['rebuild'] == TRUE) {
    // Get the submitted values.
    $values = $form_state['values']['add_feed'];

    // Add the custom feed to the form array.
    $k = drupal_strtolower(str_replace(' ', '_', $values['title']));
    $title = $values['title'];
    $icon = $values['icon'];
    $url = $values['data'];

    $form['social_timeline']['custom_' . $k] = array(
      'feed' => array(
        '#markup' => $title,
      ),
      'data' => array(
        '#type' => 'textfield',
        '#title' => t('URL'),
        '#default_value' => $url,
        '#value' => $url,
      ),
      'limit' => array(
        '#type' => 'select',
        '#title' => t('Limit'),
        '#options' => $limit,
        '#default_value' => 5,
        '#value' => 5,
      ),
      'icon' => array(
        '#type' => 'textfield',
        '#title' => t('Icon'),
        '#default_value' => $icon,
        '#value' => $icon,
      ),
      'weight' => array(
        '#type' => 'weight',
        '#title' => t('Weight'),
        '#default_value' => 0,
        '#value' => 0,
        '#delta' => 10,
        '#title-display' => 'invisible',
      ),
      'active' => array(
        '#type' => 'checkbox',
        '#default_value' => 1,
        '#value' => 1,
      ),
      'delete' => array(
        '#type' => 'checkbox',
      ),
    );
  }

  // Get the custom feeds.
  $custom_feeds = variable_get('social_timeline');

  if (isset($form_state['values'])) {
    foreach ($form_state['values']['social_timeline'] as $sk => $sv) {
      if (!array_key_exists($sk, $custom_feeds)) {
        // Title fix.
        $title_st = str_replace('custom_', '', $sk);
        $title_st = ucwords(str_replace('_', ' ', $title_st));

        $form['social_timeline'][$sk] = array(
          'feed' => array(
            '#markup' => $title_st,
          ),
          'data' => array(
            '#type' => 'textfield',
            '#title' => check_plain($k),
            '#default_value' => $sv['data'],
          ),
          'limit' => array(
            '#type' => 'select',
            '#title' => t('Limit'),
            '#options' => $limit,
            '#default_value' => $sv['limit'],
          ),
          'icon' => array(
            '#type' => 'textfield',
            '#title' => t('Icon URL'),
            '#default_value' => $sv['icon'],
          ),
          'weight' => array(
            '#type' => 'weight',
            '#title' => t('Weight'),
            '#default_value' => $sv['weight'],
            '#delta' => 10,
            '#title-display' => 'invisible',
          ),
          'active' => array(
            '#type' => 'checkbox',
            '#default_value' => $sv['active'],
          ),
          'delete' => array(
            '#type' => 'checkbox',
          ),
        );
      }
    }
  }

  // Add the custom feeds to the form.
  if ($custom_feeds != NULL) {
    foreach ($custom_feeds as $k => $v) {
      if (strpos($k, 'custom_') !== FALSE) {
        // Title fix.
        $title = str_replace('custom_', '', $k);
        $title = ucwords(str_replace('_', ' ', $title));

        // Add custom feeds to the form array.
        $form['social_timeline'][$k] = array(
          'feed' => array(
            '#markup' => $title,
          ),
          'data' => array(
            '#type' => 'textfield',
            '#title' => check_plain($k),
            '#default_value' => (isset($st_feeds[$k]['data'])) ? $st_feeds[$k]['data'] : NULL,
          ),
          'limit' => array(
            '#type' => 'select',
            '#title' => t('Limit'),
            '#options' => $limit,
            '#default_value' => (isset($st_feeds[$k]['limit'])) ? $st_feeds[$k]['limit'] : 5,
          ),
          'icon' => array(
            '#type' => 'textfield',
            '#title' => t('Icon URL'),
            '#default_value' => (isset($st_feeds[$k]['limit'])) ? $st_feeds[$k]['icon'] : 5,
          ),
          'weight' => array(
            '#type' => 'weight',
            '#title' => t('Weight'),
            '#default_value' => (isset($st_feeds[$k]['weight'])) ? $st_feeds[$k]['weight'] : 0,
            '#delta' => 10,
            '#title-display' => 'invisible',
          ),
          'active' => array(
            '#type' => 'checkbox',
            '#default_value' => (isset($st_feeds[$k]['active'])) ? $st_feeds[$k]['active'] : 0,
          ),
          'delete' => array(
            '#type' => 'checkbox',
          ),
        );
      }
    }
  }

  // Sort the array based on row weight.
  uasort($form['social_timeline'], 'social_timeline_array_sort');

  // Add new feed form.
  $form['add_feed']['#tree'] = TRUE;
  $form['add_feed']['#type'] = 'fieldset';
  $form['add_feed']['#collapsible'] = TRUE;
  $form['add_feed']['#collapsed'] = TRUE;
  $form['add_feed']['#title'] = t('Add Custom Feed');
  $form['add_feed']['#weight'] = 40;

  $form['add_feed']['title'] = array(
    '#type' => 'textfield',
    '#title' => t('Feed Name'),
    '#description' => t('The name of the feed'),
  );

  $form['add_feed']['data'] = array(
    '#type' => 'textfield',
    '#title' => t('URL'),
    '#description' => t('The URL to the custom feed'),
  );

  $form['add_feed']['icon'] = array(
    '#type' => 'textfield',
    '#title' => t('Feed Icon'),
    '#description' => t('The icon to represent the feed'),
  );

  $form['add_feed']['link'] = array(
    '#type' => 'submit',
    '#value' => t('Add New Feed'),
    '#submit' => array('social_timeline_ajax_submit'),
    '#ajax' => array(
      'callback' => 'social_timeline_add_feed_callback',
      'wrapper' => 'st-form',
    ),
  );

  // Add submit actions.
  $form['actions'] = array('#type' => 'actions');
  $form['actions']['submit'] = array('#type' => 'submit', '#value' => t('Save Changes'));

  return $form;
}

/**
 * Submit function for rebuilding the form.
 */
function social_timeline_ajax_submit($form, &$form_state) {
  $form_state['rebuild'] = TRUE;
}

/**
 * Submit callback for the social_timeline_admin_form.
 */
function social_timeline_admin_form_submit($form, &$form_state) {
  // Loop through the form state and add/update our social timeline variable.
  foreach ($form_state['values'] as $id => $item) {
    if (strpos($id, 'social_timeline') !== FALSE) {
      variable_set($id, $item);
      $new_value = variable_get('social_timeline');
      if ($id == 'social_timeline') {
        foreach ($item as $k => $v) {
          // If the delete checkbox is checked then delete the custom feed.
          if (isset($form_state['values'][$id][$k]['delete'])) {
            if ($form_state['values'][$id][$k]['delete'] == 1) {
              unset($new_value[$k]);
            }
          }
        }

        // Save the new social timeline variable.
        variable_set('social_timeline', $new_value);
      }
      else {
        // Save the social timeline global settings variable.
        variable_set($id, $item);
      }
    }
  }
}

/**
 * Add new feed AJAX callback.
 */
function social_timeline_add_feed_callback($form, &$form_state) {
  return $form;
}

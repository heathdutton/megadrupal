<?php

/**
 * @file
 * Functions used by MM Media Assist
 */

/**
 * Print the main mm_media_assist category browser.
 */
function mm_media_assist_loader() {
  module_load_include('inc', 'monster_menus', 'mm_browser');
  $term_ids = func_get_args();
  $dashed = array_shift($term_ids);
  if ($dashed) {
    array_shift($term_ids);
    $start = array_pop($term_ids);
    if (!mm_media_sanitize_termids($term_ids)) {
      // Sanitization failed.
      return;
    }
    if ($term_ids) {
      $top = $term_ids[0];
    }
    else {
      $dashed = explode('-', $dashed);
      $top = $dashed[0];
    }
    $output = mm_browser($start, $top, MM_BROWSER_MODE_GALLERY);
  }
  else {
    $location = isset($_GET['location']) ? $_GET['location'] : '';
    $end = 0;
    if (stripos($location, '/node/') > 1) {
      $end = stripos($location, '/node/');
    }
    if (stripos($location, '/contents/') > 1) {
      $end = stripos($location, '/contents/');
    }
    $myterm_ids = array();
    if ($end) {
      $start = stripos($location, base_path()) + strlen(base_path());
      $url = substr($location, $start, $end - $start);
      monster_menus_url_inbound_alter($result, $url, NULL);
      mm_parse_args($myterm_ids, $oarg_list, $this_tid, $result);
    }
    $top_mmtid = isset($myterm_ids[0]) ? $myterm_ids[0] : 1;
    if (isset($this_tid) && is_numeric($this_tid))
      $output = mm_browser($this_tid, $top_mmtid, MM_BROWSER_MODE_WYSIWYG);
    else
      $output = mm_browser(mm_home_mmtid(), $top_mmtid, MM_BROWSER_MODE_WYSIWYG);
  }

  print theme('mm_media_assist_page',
    array(
      'content' => $output,
      'attributes' => array(
        'id' => 'mm-media-assist-load',
        'class' => array('mm-media-assist'),
      )
    )
  );
  $GLOBALS['devel_shutdown'] = FALSE; // prevent the devel module from outputting
  exit;
}

function mm_media_assist_node_form($form, &$form_state, $term_id) {
  global $user;

  $node = array('uid' => $user->uid, 'name' => $user->name, 'type' => 'mm_media');
  module_load_include('inc', 'node', 'node.pages');
  drupal_add_css(drupal_get_path('module', 'mm_media') . '/css/mm_media_assist.css');

  $form = node_form($form, $form_state, $node);
  if ($term_id) {
    $tree = mm_content_get($term_id);
    $mmlist = array($term_id => mm_content_get_name($tree));
    mm_content_get_default_node_perms($term_id, $grouplist, $userlist, MM_UI_MAX_USERS_IN_GROUP);
    $uid = $user->uid;
    $everyone = in_array(MM_PERMS_APPLY, explode(',', $tree->default_mode));
    $form['mm_categories'] = array(
      '#type' => 'fieldset',
      '#title' => t('Pages'),
      '#collapsible' => TRUE,
      '#collapsed' => TRUE
    );
    $form['mm_categories']['mm_catlist'] = array(
      '#type' => 'mm_catlist',
      '#description' => t('Choose additional pages where this content will appear.'),
      '#mm_list_popup_start' => implode('/', mm_content_get_parents($term_id)),
      '#default_value' => $mmlist
    );
    $form['mm_categories']['mm_catlist_restricted'] = array(
      '#type' => 'value',
      '#value' => array()
    );

    _mm_ui_node_form_perms($form, $userlist, $grouplist, $everyone,
      node_type_get_name($node), $uid,
      t('The default settings are taken from the settings of the page.'));
    $form['#validate'][] = 'mm_media_assist_node_form_validate';
    $form['#submit'][] = 'mm_media_assist_node_form_submit';
    unset($form['buttons']['preview']);
  }

  return $form;
}

function mm_media_assist_node_form_validate($form, &$form_state) {
  node_validate((object)$form_state['values'], $form, $form_state);
}

function mm_media_assist_node_form_submit($form, &$form_state) {
  $node = node_submit($form_state['values']);
  if (node_access('create', $node->type)) {
    node_save($node);
    watchdog('content', '%type: added %title.', array('%type' => $node->type, '%title' => $node->title), WATCHDOG_NOTICE, l(t('view'), "node/$node->nid"));
    drupal_set_message(t('Your @post was created.', array('@post' => node_type_get_name($node))));
  }
  if (!empty($_COOKIE['mm_last_page'])) {
    // This is really not safe, but would not be very useful to exploit
    drupal_goto($_COOKIE['mm_last_page']);
  }
  drupal_goto('mm_media_assist/properties/' . $node->nid);
}

/**
 * Load the media properties pane
 */
function mm_media_assist_properties($file, $node) {
  if (!user_access('access content') || !mm_content_user_can_node($node, MM_PERMS_READ)) {
    return MENU_ACCESS_DENIED;
  }

  drupal_add_library('system', 'ui');

  $file->isimage = !strncmp($file->filemime, 'image/', 6);
  if (empty($file->nid)) $file->nid = $node->nid;
  $form = drupal_get_form('mm_media_assist_properties_form', $file, $node);
  $output = drupal_render($form);
  print theme('mm_media_assist_page',
    array(
      'content' => $output,
      'attributes' => array(
        'id' => 'mm-media-assist-properties',
        'class' => array('mm-media-assist'),
      )
    )
  );
  $GLOBALS['devel_shutdown'] = FALSE; // prevent the devel module from outputting
  exit;
}

function mm_media_assist_properties_form($form, $form_state, $file, $node) {
  $form[] = array('#markup' => '<div id="mm-media-assist"><div id="mm-media-assist-nav"><h2 class="mm-media-assist-title-nomenu">' . t('File Properties') . '</h2></div>');

  $form_title = array(
    '#type' => 'textfield',
    '#title' => t('Title (optional)'),
    '#default_value' => strip_tags($node->title),
    '#size' => 30,
    '#maxlength' => 255,
  );

  // Buttons
  $actions = array(
    // Can't use '#type' => 'actions' here, because it messes up the weights
    '#prefix' => '<div id="edit-actions" class="form-actions form-wrapper">',
    '#suffix' => '</div>',
  );
  $actions['insert'] = array(
    '#type' => 'button',
    '#value' => t('Insert'),
    '#button_type' => 'button',
  );
  $actions['cancel'] = array(
    '#type' => 'button',
    '#value' => t('Back'),
    '#button_type' => 'button',
  );
  $actions['abort'] = array(
    '#type' => 'button',
    '#value' => t('Close window'),
    '#button_type' => 'button',
  );

  $title = empty($node->title) ? $file->filename : $node->title;
  if ($file->isimage) {
    $entity_info = entity_get_info('file');
    $view_modes = $entity_info['view modes'];
    $options = $formats = array();
    foreach ($view_modes as $view_mode => $view_mode_info) {
      // Don't present the user with an option to choose a view mode in which the
      // file is hidden.
      $extra_fields = field_extra_fields_get_display('file', $file->type, $view_mode);
      if (!$extra_fields['file']['visible']) {
        continue;
      }

      $options[$view_mode] = $view_mode_info['label'];
      $element = media_get_file_without_label($file, $view_mode, array('wysiwyg' => TRUE));

      // Make a pretty name out of this.
      $formats[$view_mode] = drupal_render($element);
    }

    if (!$formats) {
      throw new Exception('Unable to continue, no available formats for displaying media.');
    }

    $default_view_mode = variable_get('media__wysiwyg_default_view_mode');
    if (!isset($formats[$default_view_mode])) {
      $default_view_mode = key($formats);
    }

    // Create the form
    $form[] = array(
      '#markup' => '<div id="mm-media-assist-form" class="mm-media-assist-table"><p>' . t('Click "Insert" to embed this image in your text.') . '</p>');

    $form['title'] = $form_title;
    $form['title']['#description'] = t('The title shows when the mouse hovers over the image.');
    $form['desc'] = array(
      '#type' => 'textfield',
      '#title' => t('Alternate text (optional)'),
      '#default_value' => strip_tags($node->title),
      '#size' => 30,
      '#maxlength' => 255,
      '#description' => t('Alternate text won\'t show onscreen, but will be read by assistive devices for the blind.'),
    );

    // Presentation Style
    $form['format'] = array(
      '#title' => t('Style'),
      '#type' => 'select',
      '#default_value' => $default_view_mode,
      '#options' => $options,
    );

    // Settings
    $form[] = array('#markup' => '<div class="form-item" id="link-group">', '#title' => t('Settings'));
    $form['mm_media_link'] = array('#type' => 'checkbox', '#title' => t('Clicking on the image shows it full-size'));
    $form['mm_media_outlined'] = array('#type' => 'checkbox', '#title' => t('Show a black border around the image'));
    $form['mm_media_margin'] = array('#type' => 'checkbox', '#title' => t('Include some empty space around the image'));
    $form['actions'] = $actions;
    $form[] = array('#markup' => '</div>');

    $form[] = array('#markup' => "</div><div id=\"mm-media-assist-preview\"><div class=\"mm-media-browse-imagesingle ui-state-default ui-widget-content\"><div class=\"ui-widget-header mm-media-browse-caption\">");
    $form[] = array('#markup' => t('Preview of %title', array('%title' => $title)));
    $form[] = array('#markup' => '</div><div id="mm-media-preview"></div>');
    $form[] = array('#markup' => '</div></div>');

    drupal_add_js(array('mm_media' => array('formatFormFormats' => $formats)), 'setting');
  }   // end isimage
  else {
    $form[] = array(
      '#markup' => '<div id="mm-media-assist-form" class="mm-media-assist-table"><p>' . t('Click "Insert" to embed this file upload in your text.') . '</p><p>' . t('The title will appear next to the link to the file.') . '</p>');
    $properties_image = theme('mm_browser_thumbnail', array('file' => $file, 'mode' => ''));

    $form['title'] = $form_title;
    $form['desc'] = array(
      '#type' => 'textfield',
      '#title' => t('Description (optional)'),
      '#default_value' => strip_tags($node->title),
      '#size' => 30,
      '#maxlength' => 255,
    );

    $type = file_get_type($file);
    if ($type == 'audio') {
      $form['video'] = array(
        '#type' => 'hidden',
        '#value' => $file->uri,
      );
      $form['vwidth'] = array(
        '#type' => 'hidden',
        '#value' => 300,
      );
      $form['vheight'] = array(
        '#type' => 'hidden',
        '#value' => 24,
      );
      $form['autoplay'] = array(
        '#type' => 'checkbox',
        '#default_value' => 0,
        '#title' => t('Auto start audio'),
        '#description' => NULL,
      );
    }
    elseif ($type == 'video' || $file->filemime == 'application/x-shockwave-flash') {
      $form['video'] = array(
        '#type' => 'hidden',
        '#value' => $file->uri,
      );
      if (module_exists('getid3')) {
        $info = getid3_analyze(drupal_realpath($file->uri));
      }
      $form['vwidth'] = array(
        '#type' => 'textfield',
        '#size' => 25,
        '#default_value' => (isset($info['video']['resolution_x']) ? $info['video']['resolution_x'] : ''),
        '#title' => 'Video width',
      );
      $form['vheight'] = array(
        '#type' => 'textfield',
        '#size' => 25,
        '#default_value' => (isset($info['video']['resolution_y']) ? $info['video']['resolution_y'] : ''),
        '#title' => 'Video height',
      );
      $form['bg'] = array(
        '#type' => 'textfield',
        '#size' => 45,
        '#maxlength' => 400,
        '#default_value' => '',
        '#title' => t('Full URL for the preview image (optional)'),
        '#description' => t('Provide node id for thumbnail e.g. http://hostname.com/system/files/styles/preview/private/filename.png'),
      );
      $form['autoplay'] = array(
        '#type' => 'checkbox',
        '#default_value' => 0,
        '#title' => t('Auto start movie'),
        '#description' => NULL,
      );
    }

    if ($file->filemime == 'application/x-director') {
      $form['sound'] = array(
        '#type' => 'checkbox',
        '#title' => t('Sound'),
        '#default_value' => 1,
        '#description' => NULL,
        '#prefix' => '<div class="container-inline">'
      );
      $form['autostart'] = array(
        '#type' => 'checkbox',
        '#title' => t('Autostart'),
        '#default_value' => 1,
        '#description' => NULL,
        '#prefix' => '<span class="rightalign">',
        '#suffix' => '</span></div>'
      );
      $form['progress'] = array(
        '#type' => 'checkbox',
        '#title' => t('Progress'),
        '#description' => NULL,
        '#prefix' => '<div class="container-inline">'
      );
      $form['swliveconnect'] = array(
        '#type' => 'checkbox',
        '#title' => t('SWLiveConnect'),
        '#prefix' => '<span class="rightalign">',
        '#suffix' => '</span></div>'
      );
    }

    $form['actions'] = $actions;
    $form[] = array('#markup' => "</div><div id=\"mm-media-assist-preview\"><div class=\"mm-media-browse-imagesingle ui-state-default ui-widget-content\"><div class=\"ui-widget-header mm-media-browse-caption\">");
    $form[] = array('#markup' => t('Preview of %title', array('%title' => $title)) . '</div>');
    $form[] = array('#markup' => '<div id="mm-media-preview">' . $properties_image . '</div>');
    $form[] = array('#markup' => '</div></div>');
  }
  // Hidden Fields
  $form['nid'] = array(
    '#type' => 'hidden',
    '#value' => $node->nid,
  );
  $form['fid'] = array(
    '#type' => 'hidden',
    '#value' => $file->fid,
  );
  $form['instanceId'] = array(
    '#type' => 'hidden',
    '#value' => empty($_GET['instanceId']) ? 0 : $_GET['instanceId'],
  );

  $form['#attributes'] = array('name' => 'mm_media_assist', 'class' => array('mm-media-assist'));
  return $form;
}

// --------------------------------------------------------------------------------
// Theme Functions
// --------------------------------------------------------------------------------

/**
 * Theme the media assist page
 *
 * @param $variables
 *   An associative array containing:
 *   - content: the selected node
 *   - attributes: any attributes
 * @return string
 *   HTML code
 * @ingroup themeable
 */
function theme_mm_media_assist_page($variables) {
  drupal_add_css(drupal_get_path('module', 'mm_media') . '/css/mm_media_assist.css');
  _mm_media_assist_add_custom_js();
  return theme('mm_page_wrapper',
    array(
      'title' => t('Select a file upload'),
      'body' => $variables['content'],
      'wrap_body_with_div' => FALSE,
      'attributes' => $variables['attributes']
    )
  );
}

function _mm_media_assist_add_custom_js() {
  $error = t('The media cannot be inserted because the page that opened the popup is no longer present.');
  drupal_add_js(mm_static('back_in_history') . '
(function ($) {

Drupal.mmMediaUpdateCaption = function() {
  var title = $("input[name=title]").val().trim();
  if (title != "") {
    $("#mm-media-preview img").attr("title", title);
  }
  else {
    $("#mm-media-preview img").removeAttr("title");
  }
  var alt = $("input[name=desc]").val().trim();
  if (alt != "") {
    $("#mm-media-preview img").attr("alt", alt);
  }
  else {
    $("#mm-media-preview img").removeAttr("alt");
  }
};

Drupal.mmMediaInsertImage = function() {
  if (window.opener) {
    // Get variables from the fields on the properties frame
    var media = {
      options: {},
      type: $("select#edit-format").val() || "media",
      html: $("#mm-media-preview").html() || $("<img />"),
    };
    $(":input:not(:submit)").each(function(i, field) {
      if (field.name != "instanceId" && field.name.substr(0, 5) != "form_" && ($(this).attr("type") != "checkbox" || $(this).attr("checked")))
        media.options[field.name] = $(this).val();
    });
    window.opener.Drupal.wysiwyg.plugins.mm_media.insertMediaFile(
      {fid: $("input[name=fid]").val()},
      media.type, media.html, media.options,
      window.opener.Drupal.wysiwyg.instances[$("input[name=instanceId]").val()]);
      window.opener.Drupal.wysiwyg.plugins.mm_media.closeChildWindow();
  }
  else {
    alert("' . $error . '");
  }
  return false;
};

Drupal.behaviors.mmMediaInit = {
  attach: function (context, settings) {
    $(".mm-media-assist-table", context).once("mm-media-assist-table", function () {
      $("input[name=title],input[name=desc]")
        .blur(Drupal.mmMediaUpdateCaption);
      Drupal.mmMediaUpdateCaption();
      $("input#edit-abort")
        .click(function() {window.opener.Drupal.wysiwyg.plugins.mm_media.closeChildWindow();return false;});
      $("input#edit-cancel")
        .click(function() {Drupal.mm_back_in_history();return false;});
      $("input#edit-insert")
        .click(function () {
          Drupal.mmMediaUpdateCaption();
          Drupal.mmMediaInsertImage();
        });
      $("select#edit-format")
        .change(function() {
          $("#mm-media-preview").html(Drupal.settings.mm_media.formatFormFormats[$(this).val()]);
        })
        .trigger("change");
      $("input#edit-mm-media-outlined")
        .change(function() {
          $("#mm-media-preview img").css("border", this.checked ? "1px solid black" : "none");
        });
      $("input#edit-mm-media-margin")
        .change(function() {
          $("#mm-media-preview img").css("margin", this.checked ? "10px" : "0");
        });
    });
  }
};
})(jQuery);', 'inline');
}

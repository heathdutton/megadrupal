<?php
// $Id$

/**
 * @file
 * Admin section for Signwriter.
 */

/**
 * adds the signwriter profiles to the settings form
 */
function _signwriter_add_profiles_to_form(&$form, $formtag = "profiles") {
  $profiles = signwriter_load_profiles();
  $rows = array();
  foreach ($profiles as $profile) {
    $links[] = l($profile->name, 'admin/config/signwriter/signwriter/profile/' . $profile->id);
    $variables = array(
      'text' => $profile->name,
      'profile' => $profile,
      'attributes' => array('style' => 'vertical-align:middle'),
    );
    $rows[] = array(
      'name' => l(theme('signwriter_text', $variables), 'admin/config/signwriter/signwriter/profile/'. $profile->id, array('html' => TRUE)),
      'edit' => l(t('edit'), 'admin/config/signwriter/signwriter/profile/' . $profile->id),
      'delete' => l(t('delete'), "admin/config/signwriter/signwriter/profile/$profile->id/delete"),
    );
  }
  if (empty($rows)) {
    $rows[] = array(
      array(
        'data' => t('No profiles'),
        'colspan' => '3',
        'class' => 'message',
      ),
    );
  }

  $rows[] = array(
    array(
      'data' => l(t('Add a profile'), 'admin/config/signwriter/signwriter/profile/add'),
      'colspan' => '3',
    ),
  );
  $header = array(
    array(
      'data' => t('Profiles'),
      'colspan' => '3',
    ),
  );
  $form[$formtag] = array(
    '#markup' => theme('table', array('header' => $header, 'rows' => $rows)),
  );
}
/**
 * Deletes a signwriter profile.
 *
 * @param $profile
 *   The profile object to delete.
 */
function _signwriter_delete_profile($profile) {
  // TODO Please review the conversion of this statement to the D7 database API syntax.
  /* db_query('DELETE FROM {signwriter} WHERE id = %d', $profile->id) */
  if (db_delete('signwriter')
  ->condition('id', $profile->id)
  ->execute()) {
    drupal_set_message(t("Deleted the '@name' profile.", array('@name' => $profile->name)));
  }
  else {
    drupal_set_message(t("Deleting the '@name' profile failed!", array('@name' => $profile->name)), 'warning');
  }
}

/**
 * Save a profile or update a profile in the database.
 *
 * @param $profile
 *   The profile object to save
 */
function signwriter_save_profile($profile, $update = FALSE) {
  drupal_write_record('signwriter', $profile, ($update ? 'id' : array()));
  return $profile->id;
}


/**
 * adds the signwriter menuprofile to the settings form
 */
function _signwriter_add_menuprofiles_to_form(&$form, $formtag = "menuprofiles") {
  $profiles = signwriter_load_menuprofiles();
  $rows = array();
  foreach ($profiles as $profile) {
    $rows[] = array(
      'name' => $profile->name,
      'sample' => $profile->name,
      'edit' => l(t('edit'), 'admin/config/signwriter/signwriter/menuprofile/' . $profile->id),
      'delete' => l(t('delete'), "admin/config/signwriter/signwriter/menuprofile/$profile->id/delete"),
    );
  }
  if (empty($rows)) {
    $rows[] = array(array(
      'data' => t('No profiles'),
      'colspan' => '4',
      'class' => 'message',
    ));
  }
  $rows[] = array(
    array(
      'data' => l(t('Add a menu profile'), 'admin/config/signwriter/signwriter/menuprofile/add'),
      'colspan' => '4',
    ),
  );
  $header = array(
    'name' => t('Profile'),
    'sample' => t('Sample'),
    'actions' => t('Actions'),
    //'actions2' => "",
  );
  $form[$formtag] = array(
    '#markup' => theme('table', array('header' => $header, 'rows' => $rows)),
  );
}

/**
 * Deletes a signwriter profile.
 *
 * @param $profile
 *   The profile object to delete.
 */
function _signwriter_delete_menuprofile($profile) {
  // TODO Please review the conversion of this statement to the D7 database API syntax.
  /* db_query("DELETE FROM {signwriter_menu} WHERE id = %d", $profile->id) */
  if (db_delete('signwriter_menu')->condition('id', $profile->id)->execute()) {
    drupal_set_message(t("Deleted the '@name' profile.", array('@name' => $profile->name)));
  }
  else {
    drupal_set_message(t("Deleting the '@name' profile failed!", array('@name' => $profile->name)), 'warning');
  }
}

/**
 * Save a profile or update a profile in the database.
 *
 * @param $profile
 *   The profile object to save
 */
function _signwriter_save_menuprofile($profile, $update) {
  $values = serialize($profile->data);
  if (empty($profile->id)) {
    // TODO Please review the conversion of this statement to the D7 database API syntax.
    /* db_query("INSERT INTO {signwriter_menu} (name,data) VALUES ('%s','%s')", $profile->name, $values) */
    $id = db_insert('signwriter_menu')->fields(array(
      'name' => $profile->name,
      'data' => $values,
    ))->execute();
    return db_query("SELECT LAST_INSERT_ID()")->fetchField();
  }
  else {
    // TODO Please review the conversion of this statement to the D7 database API syntax.
    /* db_query("UPDATE {signwriter_menu} SET name='%s', data='%s' WHERE id=%d", $profile->name, $values, $profile->id) */
    db_update('signwriter_menu')
    ->fields(array(
      'name' => $profile->name,
      'data' => $values,
    ))
    ->condition('id', $profile->id)
    ->execute();
    return $profile->id;
  }
}



/**
 * The main admin>>settings>>signwriter page
 */
function signwriter_settings_form($form, $form_state) {
  $form = array();
  if (!function_exists('imagetypes')) {
    // TODO Please change this theme call as discussed at http://drupal.org/node/224333#theme_page.
    // $form['error'] = array(
    return $form;
  }
  else {
    _signwriter_add_profiles_to_form($form);
    $form['paths'] = array(
      '#type' => 'fieldset',
      '#title' => t('Paths'),
      '#collapsible' => TRUE,
      '#collapsed' => FALSE,
    );
    $form['paths']['signwriter_cachedir'] = array(
      '#type' => 'textfield',
      '#title' => t('Cache Directory'),
      '#description' => t('This is the directory that signwriter will store its generated images in. It must be publicly accessible; the default is \'files/signwriter-cache\'. Make sure that your webserver process is able to create and write to this directory. Files can be deleted from this directory at any time.'),
      '#default_value' => variable_get('signwriter_cachedir', file_default_scheme()  . '/signwriter'),
    );
    $form['paths']['signwriter_cachecleanperiod'] = array(
      '#type' => 'radios',
      '#title' => t('Cache cleanup period'),
      '#description' => t('Choose how often the images in the signwriter cache directory should be purged.'),
      '#default_value' => variable_get('signwriter_cachecleanperiod', '100 years'),
      '#options' => array(
        '1 day' => t('Daily'),
        '1 week' => t('Weekly'),
        '100 years' => t('Never'),
      ),
    );
    $form['paths']['signwriter_cachecleannow'] = array(
      '#type' => 'submit',
      '#value' => t('Clean signwriter cache now'),
    );
    $form['paths']['signwriter_fontpath'] = array(
      '#type' => 'textfield',
      '#title' => t('Font Search Path'),
      '#description' => t('Add a : separated list of directories to search for your font files. Signwriter will automatically search the drupal directory, your files directory, and your current theme\'s directory.'),
      '#default_value' => variable_get('signwriter_fontpath', ''),
    );

    $form['pages'] = array(
      '#type' => 'fieldset',
      '#title' => t('Pages'),
      '#collapsible' => TRUE,
      '#collapsed' => FALSE,
    );
    $options = array(t('Use on every page except the listed pages.'), t('Use on only the listed pages.'));
    $description = t("Enter one page per line as Drupal paths. The '*' character is a wildcard. Example paths are %blog for the blog page and %blog-wildcard for every personal blog. %front is the front page.", array('%blog' => 'blog', '%blog-wildcard' => 'blog/*', '%front' => '<front>'));
    $access = user_access('use PHP for signwriter pages');
    if ($access) {
      $options[] = t('Use if the following PHP code returns <code>TRUE</code> (PHP-mode, experts only).');
      $description .= ' ' . t('If the PHP-mode is chosen, enter PHP code between %php. Note that executing incorrect PHP-code can break your Drupal site.', array('%php' => '<?php ?>'));
    }
    $form['pages']['signwriter_pages_type'] = array(
      '#type' => 'radios',
      '#title' => t('Use signwriter on specific pages'),
      '#options' => $options,
      '#default_value' => variable_get('signwriter_pages_type', 0),
    );
    $form['pages']['signwriter_pages_list'] = array(
      '#type' => 'textarea',
      '#title' => t('Pages'),
      '#default_value' => variable_get('signwriter_pages_list', ''),
      '#description' => $description,
    );

    $form['signwriter_specialcharnotice'] = array(
      '#type' => 'checkbox',
      '#title' => t('Special character notice'),
      '#description' => t('Each time time a string is to be rendered it is checked for special characters outside the ASCII character set. If any are found and the current user has the permission to configure signwriter, output a notice how to allow these characters.'),
      '#default_value' => variable_get('signwriter_specialcharnotice', TRUE),
    );
    $titleoptions = array('disabled' => t('Disabled (headers unchanged)'));
    $profiles = signwriter_load_profiles();
    foreach ($profiles as $profile) {
      $titleoptions[$profile->id] = theme('signwriter_text', array(
        'text' => $profile->name,
        'profile' => $profile,
        'attributes' => array('style' => 'vertical-align:middle'),
      ));
    }

    $form['hooks'] = array(
      '#type' => 'fieldset',
      '#title' => t('Template Hooks'),
      '#description' => t('Choose a profile for each specific signwriter_preprocess hook to replace normal /text) headers with signwritten versions.'),
      '#collapsible' => TRUE,
      '#collapsed' => FALSE,
    );
    $form['hooks']['signwriter_pagehookprofile'] = array(
      '#type' => 'radios',
      '#title' => t('Page titles'),
      '#default_value' => variable_get('signwriter_pagehookprofile', 'disabled'),
      '#options' => $titleoptions,
    );
    $form['hooks']['signwriter_blockhookprofile'] = array(
      '#type' => 'radios',
      '#title' => t('Block titles'),
      '#default_value' => variable_get('signwriter_blockhookprofile', 'disabled'),
      '#options' => $titleoptions,
    );
    if (module_exists('panels')) {
      $form['hooks']['signwriter_panelshooktitleprofile'] = array(
        '#type' => 'radios',
        '#title' => t('Panels titles'),
        '#default_value' => variable_get('signwriter_panelshooktitleprofile', 'disabled'),
        '#options' => $titleoptions,
      );
    }

    //.... Menu related settings
    _signwriter_add_menuprofiles_to_form($form);

    //.... Build profile popup menu
    $menuprofiles = signwriter_load_menuprofiles();
    $menuoptions = array(false => t("Standard (not signwriter enhanced)"));
    foreach ($menuprofiles as $menuprofile) {
      $menuoptions[$menuprofile->id] = $menuprofile->name;
    }

    //.... Build settings for earch menu
    $form['signwritermenu'] = array(
      '#type' => 'fieldset',
      '#title' => t('Menu Settings'),
      '#description' => t('Choose a profile for each menu that should use Signwritermenu for drawing menu items.'),
      '#collapsible' => TRUE,
      '#collapsed' => FALSE,
    );
    $form['signwritermenu']['signwriter_menulevels'] = array(
      '#type' => 'textfield',
      '#title' => t('Maximum Menu Levels'),
      '#description' => t('Number of menu levels handled by Signwriter Menu'),
      '#default_value' => variable_get('signwriter_menulevels', '3'),
      '#size' => 2,
    );

    $menusettings = variable_get("signwriter_menusettings", array());
    $menus = menu_get_menus();
    foreach ($menus as $key => $title) {
      $form['signwritermenu']['swm_' . $key] = array(
        '#type' => 'select',
        '#title' => $title,
        '#descripion' => t(""),
        '#default_value' => (isset($menusettings[$key]) ? $menusettings[$key] : ''),
        '#options' => $menuoptions,
      );
    }
    $form = system_settings_form($form);
    $form['#submit'] = array('signwriter_settings_form_submit');
    return $form;
  }
}

/**
 * @todo Please document this function.
 * @see http://drupal.org/node/1354
 */
function signwriter_settings_form_validate($form, &$form_state) {
  if ($form_state['clicked_button']['#value'] == t('Clean signwriter cache now')) {
    signwriter_cron(TRUE);
    $form_state['rebuild'] = TRUE;
  }
}

/**
 * Submit signwritermenu settings
 */
function signwriter_settings_form_submit($form, &$form_state) {
  $op = isset($form_state['values']['op']) ? $form_state['values']['op'] : '';

  $menus = menu_get_menus();
  $menusettings = array();
  foreach ($menus as $key => $title) {
    $value = $form_state['values']['swm_' . $key];
    if ($value) {
      $menusettings[$key] = intval($value);
    }
    unset($form_state['values']['swm_' . $key]);
  }
  unset($form_state['values']['signwriter_cachecleannow']);

  if ($op == t('Reset to defaults')) {
    variable_del('signwriter_menusettings');
  }
  else {
    variable_set('signwriter_menusettings', $menusettings);
  }

  system_settings_form_submit($form, $form_state);
  cache_clear_all();
  drupal_theme_rebuild();
}

/**
 * @todo Please document this function.
 * @see http://drupal.org/node/1354
 */
function _signwriter_profile_font($p, $fontoptions, $state = "") {
  $fieldset = array();
  $fieldset['fontfile'] = array(
    '#type' => 'select',
    '#title' => t('Font'),
    '#description' => t('These fonts have been found on the host system. The directories searched for fonts were @fontsearch. To change the font search path, go to !signwriter_admin.', array('@fontsearch' => implode(', ', _signwriter_get_fontpaths()), '!signwriter_admin' => l(t('the signwriter settings page'), 'admin/config/signwriter/signwriter'))),
    '#options' => $fontoptions,
    '#default_value' => _signwriter_get_val($p, $state . 'fontfile'),
    '#prefix' => '<td>',
  );
  $fieldset['fontsize'] = array(
    '#type' => 'textfield',
    '#title' => t('Font Size'),
    '#description' => t("If you define 'Max Width' below then this size may be overridden in order to fit the text within the given width. Defaults to 20."),
    '#default_value' => _signwriter_get_val($p, $state . 'fontsize'),
    '#size' => 4,
  );
  $fieldset['foreground'] = array(
    '#type' => 'textfield',
    '#title' => t('Font Colour'),
    '#description' => t('This should be six hexadecimal digits, such as ff0000 for red, 00ff00 for green, or 0000ff for blue.'),
    '#default_value' => _signwriter_get_val($p, $state . 'foreground'),
    '#size' => 6,
    '#maxlength' => 6,
  );
  $fieldset['foreground-farb'] = array(
    '#markup' => '<div id="' . $state . 'foreground-farb"></div>',
  );
  return $fieldset;
}

function _signwriter_profile_shadow_settings($p, $fontoptions, $state = "") {
  $fieldset = array();
  $fieldset['drop_shadow'] = array(
    '#type' => 'checkbox',
    '#title' => t('Drop Shadow'),
    '#description' => t('If enabled, the text will cast a shadow.'),
    '#default_value' => _signwriter_get_val($p, $state . 'drop_shadow', FALSE),
  );
  $fieldset['shadow_color'] = array(
    '#type' => 'textfield',
    '#title' => t('Shadow Colour'),
    '#description' => t('This should be six hexadecimal digits, such as ff0000 for red, 00ff00 for green, or 0000ff for blue.'),
    '#default_value' => _signwriter_get_val($p, $state . 'shadow_color'),
    '#size' => 6,
    '#maxlength' => 6,
  );
  $fieldset['shadow_color-farb'] = array(
    '#markup' => '<div id="' . $state . 'shadow-color-farb"></div>',
  );
  $fieldset['shadow_xoffset'] = array(
    '#type' => 'textfield',
    '#title' => t('Shadow X Offset'),
    '#description' => t('The horizontal distance of the shadow from the actual text. A negative value will put the shadow to the left of the text.'),
    '#default_value' => _signwriter_get_val($p, $state . 'shadow_xoffset', $state == "" ? 5 : NULL),
    '#size' => 4,
  );
  $fieldset['shadow_yoffset'] = array(
    '#type' => 'textfield',
    '#title' => t('Shadow Y Offset'),
    '#description' => t('The vertical distance of the shadow from the actual text. A negative value will put the shadow above the text.'),
    '#default_value' => _signwriter_get_val($p, $state . 'shadow_yoffset', $state == "" ? 5 : NULL),
    '#size' => 4,
  );
  return $fieldset;
}

/**
 * @todo Please document this function.
 * @see http://drupal.org/node/1354
 */
function _signwriter_profile_background_settings($p, $fontoptions, $state = "") {
  $fieldset = array();
  $fieldset['transparent'] = array(
    '#type' => 'checkbox',
    '#title' => t('Transparent'),
    '#description' => t('If enabled, then the background colour selected below will be made transparent in the generated image.'),
    '#default_value' => _signwriter_get_val($p, $state . 'transparent', TRUE),
    'single' => TRUE,
  );
  $fieldset['background'] = array(
    '#type' => 'textfield',
    '#title' => t('Background Colour'),
    '#description' => t('This should be six hexadecimal digits, such as ff0000 for red, 00ff00 for green, or 0000ff for blue. To avoid jagged fonts when using transparency, make sure that this colour is the same as the page background colour. If you are using a background image and transparency then this colour will be made transparent in the source background image.'),
    '#default_value' => _signwriter_get_val($p, $state . 'background'),
    '#size' => 6,
    '#maxlength' => 6,
  );
  $fieldset['background-farb'] = array(
    '#markup' => '<div id="' . $state . 'background-farb"></div>',
  );
  $fieldset['bgimage'] = array(
    '#type' => 'textfield',
    '#title' => t('Background Image'),
    '#description' => t('Path to the background image to use, relative to your drupal directory. Leave blank to not use a background image.'),
    '#default_value' => _signwriter_get_val($p, $state . 'bgimage'),
    '#size' => 40,
  );
  return $fieldset;
}

/**
 * @todo Please document this function.
 * @see http://drupal.org/node/1354
 */
function _signwriter_profile_border_settings($p, $fontoptions, $state = "") {
  $fieldset = array();
  $fieldset['border'] = array(
    '#type' => 'checkbox',
    '#title' => t('Border'),
    '#description' => t('If enabled, the text will be surrounded by a border.'),
    '#default_value' => _signwriter_get_val($p, $state . 'border', FALSE),
  );
  $fieldset['border_color'] = array(
    '#type' => 'textfield',
    '#title' => t('Border Colour'),
    '#description' => t('This should be six hexadecimal digits, such as ff0000 for red, 00ff00 for green, or 0000ff for blue.'),
    '#default_value' => _signwriter_get_val($p, $state . 'border_color'),
    '#size' => 6,
    '#maxlength' => 6,
  );
  $fieldset['border_color-farb'] = array(
    '#markup' => '<div id="' . $state . 'border-color-farb"></div>',
  );
  $fieldset['border_radius'] = array(
    '#type' => 'textfield',
    '#title' => t('Border Radius'),
    '#description' => t('The radius of the border. Use only small values.'),
    '#default_value' => _signwriter_get_val($p, $state . 'border_radius', 1),
    '#size' => 4,
  );
  return $fieldset;
}

/**
 * @todo Please document this function.
 * @see http://drupal.org/node/1354
 */
function _signwriter_profile_onestate(&$v, $prefix, $suffix) {
  $desc = NULL;
  if (isset($v['#description'])) {
    $desc = $v['#description'];
    unset($v['#description']);
  }
  $v['#prefix'] = $prefix;
  $v['#suffix'] = $suffix;
  return $desc;
}

/**
 * @todo Please document this function.
 * @see http://drupal.org/node/1354
 */
function _signwriter_profile_threestate(&$form, $p, $fontoptions, $fieldset, $title) {
  $function = '_signwriter_profile_' . $fieldset;
  $form[$fieldset] = array(
    '#type' => 'fieldset',
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
    '#title' => t($title),
  );

  $header = "";
  foreach (array(
    'Normal' => '',
    'Hover' => 'hov_',
    'Active' => 'act_',
  ) as $headinfo => $style) {
    $header .= '<th class="signwriter_' . $style . 'style">' . t($headinfo) . '</th>';
  }

  $form[$fieldset]['starttable'] = array(
    '#markup' => '<table class="signwriter-pref"><tbody><tr>' . $header . '</tr>',
  );

  $f1 = call_user_func_array($function, array($p, $fontoptions, ""));
  $f2 = call_user_func_array($function, array($p, $fontoptions, "hov_"));
  $f3 = call_user_func_array($function, array($p, $fontoptions, "act_"));

  foreach (array_keys($f1) as $key) {
    $v1 = $f1[$key];
    $v2 = $f2[$key];
    $v3 = $f3[$key];
    if ($single = isset($v1['single'])) {
      unset($v1['single']);
    }

    $desc = _signwriter_profile_onestate($v1, $single ? '<tr><td colspan="3">' : '<tr><td class="signwriter_style">', $single ? '</td></tr>' : '</td>');
    $form[$fieldset][$key] = $v1;

    if (!$single) {
      _signwriter_profile_onestate($v2, '<td class="signwriter_hov_style">', '</td>');
      _signwriter_profile_onestate($v3, '<td class="signwriter_act_style">', '</td></tr>');
      $form[$fieldset]['hov_' . $key] = $v2;
      $form[$fieldset]['act_' . $key] = $v3;
    }
    if ($desc) {
      $form[$fieldset][$key . '_desc'] = array(
        '#value' => $desc,
        '#prefix' => '<tr><td colspan="3">',
        '#suffix' => '</td></tr>',
      );
    }
  }
  $form[$fieldset]['endtable'] = array(
    '#markup' => '</tbody></table>',
  );
}

/**
 * @todo Please document this function.
 * @see http://drupal.org/node/1354
 */
function signwriter_profile_form($form, &$form_state, $p = NULL) {
  drupal_add_library('system', 'farbtastic');
  drupal_add_js(drupal_get_path('module', 'signwriter') . '/color.js');

  // TODO: add a preview,
  $profileid = _signwriter_get_val($p, 'id');
  $profilename = _signwriter_get_val($p, 'name');

  drupal_set_title(t('Editing signwriter profile @profilename', array('@profilename' => $profilename)), PASS_THROUGH);

  $form = array();
  $form['id'] = array(
    '#type' => 'value',
    '#value' => $profileid,
  );
  $form['name'] = array(
    '#type' => 'textfield',
    '#title' => t('Profile Name'),
    '#required' => TRUE,
    '#default_value' => $profilename,
    '#size' => 40,
  );

  //.... Build font list preview
  $preview_profile = new stdClass();
  $rows = array();
  $fontoptions = array();
  $all_fonts = _signwriter_available_fonts();
  foreach ($all_fonts as $fontfile => $fontname) {
    $fontoptions[$fontfile] = $fontname;
    $preview_profile->fontfile = $fontfile;
    $preview_profile->fontsize = 12;
    $preview_profile->maxwidth = 400;
    $preview_profile->imagetype = 'gif';

    $rows[] = array(
      'name' => $fontname,
      'sample' => theme('signwriter_text', array(
        'text' =>"$fontname - Sample text (0123456789)",
        'profile' => $preview_profile,
        'attributes' => array('style' => 'vertical-align:middle'),
      )),
    );
  }
  $header = array(
    'name' => t("Name"),
    'sample' => t("Sample"),
  );
  $form['fontpreview'] = array(
    '#markup' => theme('table', array('header' => $header, 'rows' => $rows)),
  );

  $form['general'] = array(
    '#type' => 'fieldset',
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
    '#title' => t('General settings'),
  );
  $form['general']['allowed_nonasciichars'] = array(
    '#type' => 'textfield',
    '#title' => t('Allowed non-ASCII characters'),
    '#description' => t('Most fonts support only the ASCII character set so by default so strings containing unicode characters will not be replaced by default. Paste here all unicode characters your font does include so strings which only contain characters from this list and the default ASCII charset will be converted as requested. Put \'all\' in this field to allow all unicode characters.'),
    '#default_value' => _signwriter_get_val($p, 'allowed_nonasciichars'),
  );
  $form['general']['disable_span'] = array(
    '#type' => 'checkbox',
    '#title' => t('Disable hidden span'),
    '#default_value' => _signwriter_get_val($p, 'disable_span', FALSE),
    '#description' => t('Signwriter normally prints the signwriter image along with the same text that is in the image in a span with the styling "display: none;".  Due to possible issues with search engine rankings some people will want this and some people will not so if you don\'t you can disable it here.'),
  );
  $form['general']['use_title_text'] = array(
    '#type' => 'checkbox',
    '#title' => t('Use image title text'),
    '#default_value' => _signwriter_get_val($p, 'use_title_text', FALSE),
    '#description' => t('If checked the text in the image will be used as the image\'s title text (displayed when hovering over the image).'),
  );
  $form['general']['multiline'] = array(
    '#type' => 'checkbox',
    '#title' => t('Multiline'),
    '#description' => t('If enabled and the max width is set, the text will roll over to a new line when max width is reached, otherwise the font size will be decreased to fit the text in one line.'),
    '#default_value' => _signwriter_get_val($p, 'multiline', TRUE),
  );
  $form['general']['threestate'] = array(
    '#type' => 'checkbox',
    '#title' => t('Three State'),
    '#description' => t('If enabled then images are build using hover and active state as well.'),
    '#default_value' => _signwriter_get_val($p, 'threestate', FALSE),
  );

  _signwriter_profile_threestate($form, $p, $fontoptions, 'font', 'Text');
  _signwriter_profile_threestate($form, $p, $fontoptions, 'shadow_settings', 'Drop Shadow');
  _signwriter_profile_threestate($form, $p, $fontoptions, 'background_settings', 'Background');
  _signwriter_profile_threestate($form, $p, $fontoptions, 'border_settings', 'Border');


  $form['layout'] = array(
    '#type' => 'fieldset',
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
    '#title' => t('Image Layout'),
  );
  /*
   $form['layout']['angle'] = array(
   '#type' => 'textfield',
   '#title' => t('Angle'),
   '#description' => t("Set the angle of the image in degrees."),
   '#default_value' => _signwriter_get_val($p, 'angle', 0),
   '#size' => 4,
   );
   */
  $form['layout']['width'] = array(
    '#type' => 'textfield',
    '#title' => t('Width'),
    '#description' => t("Set the width of the image in pixels. Leave blank to have the width automatically assigned, or if you're using a background image."),
    '#default_value' => _signwriter_get_val($p, 'width'),
    '#size' => 4,
  );
  $form['layout']['height'] = array(
    '#type' => 'textfield',
    '#title' => t('Height'),
    '#description' => t("Set the height of the image in pixels. Leave blank to have the height automatically assigned, or if you're using a background image."),
    '#default_value' => _signwriter_get_val($p, 'height'),
    '#size' => 4,
  );
  $form['layout']['maxwidth'] = array(
    '#type' => 'textfield',
    '#title' => t('Max Width'),
    '#description' => t('This value is in pixels. If it is set then the text size will be decreased so that the text fits within the given width. Leave this blank to have no maximum.'),
    '#default_value' => _signwriter_get_val($p, 'maxwidth'),
    '#size' => 4,
  );
  $form['layout']['textalign'] = array(
    '#type' => 'select',
    '#title' => t('Text Align'),
    '#description' => t('Text Align only makes sense if your image is wider than the text. To make this happen either assign a background image, or set the width.'),
    '#default_value' => _signwriter_get_val($p, 'textalign'),
    '#options' => array(
      'left' => 'left',
      'center' => 'center',
      'right' => 'right',
    ),
  );
  $form['layout']['xoffset'] = array(
    '#type' => 'textfield',
    '#title' => t('X Offset'),
    '#description' => t('Adds to the distance from the left of the image to the start of the text (or from the right of the image in the case of right-aligned text).'),
    '#default_value' => _signwriter_get_val($p, 'xoffset'),
    '#size' => 4,
  );
  $form['layout']['yoffset'] = array(
    '#type' => 'textfield',
    '#title' => t('Y Offset'),
    '#description' => t('Adds to the distance from the top of the image to the baseline of the text (or something like that).'),
    '#default_value' => _signwriter_get_val($p, 'yoffset'),
    '#size' => 4,
  );

  $form['imagetype'] = array(
    '#type' => 'select',
    '#title' => t('Image Type'),
    '#description' => t("For transparency png is best, but it doesn't work in IE without hacks (see http://webfx.eae.net/dhtml/pngbehavior/pngbehavior.html for one such hack), so gif is a good alternative."),
    '#default_value' => _signwriter_get_val($p, 'imagetype'),
    '#options' => _signwriter_available_image_types(),
  );

  $form['pattern'] = array(
    '#type' => 'textfield',
    '#title' => t('Input Filter Pattern'),
    '#default_value' => _signwriter_get_val($p, 'pattern'),
    '#description' => t("If this pattern is defined then this profile will be available as an input filter which you can enable on the <a href='@input-formats-url'>%inputformats</a> page. When the filter is enabled, anything matching this pattern will be replaced with a signwriter image. The pattern should be a perl regular expression. For example, to replace all headings, use: <code>/&lt;h.*?&gt;.*?&lt;\/h.*?&gt;/</code>. To replace only h1 headings, use <code>/&lt;h1.*&gt;.*?&lt;\/h1&gt;/</code>. To define a custom pseudo-html tag (such as &lt;signwriter&gt;), use: <code>/&lt;signwriter&gt;.*?&lt;\/signwriter&gt;/</code>", array('@input-formats-url' => url('admin/config/content/formats'), '%inputformats' => 'Administer >> Site configuration >> Input formats')),
    '#size' => 20,
  );

  $form['save'] = array(
    '#type' => 'submit',
    '#value' => t('Save'),
  );
  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save and edit'),
  );
  // Don't add the delete button if they are on the add new profile page
  // Otherwise clicking the button will return page not found due to there not being a profile to delete.
  if ($p) {
    $form['delete'] = array(
      '#type' => 'submit',
      '#value' => t('Delete'),
    );
  }

  return $form;
}

/**
 * @todo Please document this function.
 * @see http://drupal.org/node/1354
 */
function signwriter_profile_form_validate($form, $form_state) {
  $shadow_error = t('If using a drop shadow, the Shadow X Offset and Shadow Y Offset values must not both be zero.');
  if ($form_state['values']['drop_shadow']) {
    if ($form_state['values']['shadow_xoffset'] == 0 && $form_state['values']['shadow_yoffset'] == 0) {
      form_set_error('shadow_xoffset', $shadow_error);
      form_set_error('shadow_yoffset', $shadow_error);
    }
  }

  if (!is_numeric($form_state['values']['fontsize']) && $form_state['values']['fontsize']) {
    form_set_error('fontsize', 'Font size must be numeric.');
  }
  /*
  if (!is_numeric($form_state['values']['angle']) && $form_state['values']['angle']) {
     form_set_error('angle', 'Angle must be numeric.');
  }
  */
}

/**
 * @todo Please document this function.
 * @see http://drupal.org/node/1354
 */
function signwriter_profile_form_submit($form, &$form_state) {
  $profile = (object) $form_state['values'];

  // To allow for font size default of 20.
  if (!$profile->fontsize) {
    $profile->fontsize = 20;
  }
  /*
   if (!$profile->angle) {
   $profile->angle = 0;
   }
   */

  $clicked_button = $form_state['clicked_button']['#value'];
  if ($clicked_button == t('Delete')) {
    $form_state['redirect'] = 'admin/config/signwriter/signwriter/profile/' . $profile->id . '/delete';
  }
  else {
    $id = signwriter_save_profile($profile, $form_state['values']['id']);
    $message = t("Profile '@name' saved.", array('@name' => $profile->name));
    //drupal_set_message(theme('signwriter_text', $message, $profile));
    drupal_set_message($message);
    $form_state['redirect'] = ($clicked_button == 'Save and edit') ? 'admin/config/signwriter/signwriter/profile/' . $id : 'admin/config/signwriter/signwriter';
  }
}

/**
 * Ask for user confirmation before deleting a profile.
 *
 * @param $profile
 *   The profile object to delete.
 */
function signwriter_confirm_delete_profile_form($form, &$form_state, $profile) {
  $form = array();
  $form['id'] = array(
    '#type' => 'value',
    '#value' => $profile->id,
  );
  return confirm_form($form,
  t("Are you sure you want to delete the '%title' profile?", array('%title' => $profile->name)),
      'admin/config/signwriter/signwriter',
  t('Deleting a profile cannot be undone.'),
  t('Delete'),
  t('Cancel'));
}

/**
 * Form_submit function for profile deletion.
 * Executes deletion and returns the user to the main signwriter settings page.
 */
function signwriter_confirm_delete_profile_form_submit($form, &$form_state) {
  $profile = signwriter_load_profile($form_state['values']['id']);
  _signwriter_delete_profile($profile);
  $form_state['redirect'] = 'admin/config/signwriter/signwriter';
}

/**
 *	Edit signwritermenu profile
 */
function signwriter_menuprofile_form($form, &$form_state, $p = NULL) {
  //	drupal_add_css('misc/farbtastic/farbtastic.css', 'module', 'all', FALSE);
  //	drupal_add_js('misc/farbtastic/farbtastic.js');
  //	drupal_add_js(drupal_get_path('module', 'signwritermenu') .'/color.js');

  //.... Build signwriter popup menu and show overview
  $swprofiles = signwriter_load_profiles();
  $rows = array();
  $menuoptions = array(false => t("Unused"));
  foreach ($swprofiles as $swprofile) {
    $menuoptions[$swprofile->id] = $swprofile->name;
    $rows[] = array(
      'name' => theme('signwriter_text', array(
        'text' => $swprofile->name,
        'profile' => $swprofile,
        'attributes' => array('style' => 'vertical-align:middle'),
      )),
    );
  }
  $header = array(array('data' => t('Signwriter samples')));

  $profileid = _signwriter_get_val($p, 'id');
  $profilename = _signwriter_get_val($p, 'name');
  $values = _signwriter_get_val($p, 'data');

  drupal_set_title(t('Editing signwritermenu profile @profilename', array('@profilename' => $profilename)), PASS_THROUGH);

  $form = array();
  $form['samples'] = array(
    '#value' => theme('table', array('header' => $header, 'rows' => $rows)),
  );
  $form['id'] = array(
    '#type' => 'value',
    '#value' => $profileid,
  );
  $form['name'] = array(
    '#type' => 'textfield',
    '#title' => t('Profile Name'),
    '#required' => TRUE,
    '#default_value' => $profilename,
    '#size' => 40,
  );

  $levels = variable_get('signwriter_menulevels', '3');
  for ($i = 1; $i <= $levels; $i++) {
    $form['set' . $i] = array(
      '#type' => 'fieldset',
      '#collapsible' => TRUE,
      '#collapsed' => FALSE,
      '#title' => t('Settings for Menu Items level ' . $i),
    );
    $form['set' . $i]['item-' . $i] = array(
      '#type' => 'select',
      '#title' => t("Signwriter Profile"),
      '#descripion' => t("Signwriter profile to be used to display regular menu items"),
      '#default_value' => $values[$i],
      '#options' => $menuoptions,
    );
  }

  $form['save'] = array(
    '#type' => 'submit',
    '#value' => t('Save'),
  );
  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save and edit'),
  );
  // Don't add the delete button if they are on the add new profile page
  // Otherwise clicking the button will return page not found due to there not being a profile to delete.
  if ($p) {
    $form['delete'] = array(
      '#type' => 'submit',
      '#value' => t('Delete'),
    );
  }

  return $form;
}

/**
 * @todo Please document this function.
 * @see http://drupal.org/node/1354
 */
function signwriter_menuprofile_form_submit($form, &$form_state) {
  $profile->id = $form_state['values']['id'];
  $profile->name = $form_state['values']['name'];
  $profile->data = array();

  $levels = variable_get('signwriter_menulevels', '3');
  for ($i = 1; $i <= $levels; $i++) {
    $item = intval($form_state['values']['item-' . $i]);
    $profile->data[$i] = $item;
  }

  $clicked_button = $form_state['clicked_button']['#value'];
  if ($clicked_button == t('Delete')) {
    $form_state['redirect'] = 'admin/config/signwriter/signwriter/menuprofile/' . $profile->id . '/delete';
  }
  else {
    $id = _signwriter_save_menuprofile($profile, $form_state['values']['id']);
    $message = t("Profile '@name' saved.", array('@name' => $profile->name));
    $form_state['redirect'] = ($clicked_button == 'Save and edit') ? 'admin/config/signwriter/signwriter/menuprofile/' . $id : 'admin/config/signwriter/signwriter';
  }
}

/**
 * Ask for user confirmation before deleting a profile.
 *
 * @param $profile
 *   The profile object to delete.
 */
function signwriter_confirm_delete_menuprofile_form($form, &$form_state, $profile) {
  $form = array();
  $form['id'] = array(
    '#type' => 'value',
    '#value' => $profile->id,
  );
  return confirm_form($form,
  t("Are you sure you want to delete the '%title' profile?", array('%title' => $profile->name)),
            'admin/config/signwriter/signwriter',
  t('Deleting a profile cannot be undone.'),
  t('Delete'),
  t('Cancel'));
}

/**
 * Form_submit function for profile deletion.
 * Executes deletion and returns the user to the main signwriter settings page.
 */
function signwriter_confirm_delete_menuprofile_form_submit($form, &$form_state) {
  $profile = signwriter_load_menuprofile($form_state['values']['id']);
  _signwriter_delete_menuprofile($profile);
  $form_state['redirect'] = 'admin/config/signwriter/signwriter';
}

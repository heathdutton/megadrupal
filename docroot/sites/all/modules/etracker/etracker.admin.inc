<?php

// TODO: Include some information for segments, they need to be ordered separately.

function etracker_settings_form($form, &$form_state) {

  $form['basic'] = array(
    '#type' => 'fieldset',
    '#title' => t('Tracking code'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );
  $form['basic']['etracker_account_key_1'] = array(
    '#type' => 'textfield',
    '#title' => t('Account Key 1'),
    '#default_value' => variable_get('etracker_account_key_1', ''),
  );
  $form['basic']['etracker_account_key_1_info'] = array(
    '#type' => 'container',
    '#attributes' => array(
      'class' => array('etracker', 'account-setup'),
    ),
    'description' => array(
      '#markup' => t('To find your account key 1, go to your etracker account into Settings / Setup Tracking Code. The key is displayed at the very bottom of the page.'),
    ),
  );

  $form['basic']['etracker_scope_script'] = array(
    '#type' => 'select',
    '#title' => t('Include script into'),
    '#options' => array(
      'header' => t('Header') . ' (' . t('recommended') . ')',
      'footer' => t('Footer'),
    ),
    '#default_value' => variable_get('etracker_scope_script', 'header'),
    '#description' => t('The JavaScript is built in a way that it does <strong>not</strong> block or delay the loading of the page and to support all functionality of the etracker suite it is required to have the etracker tracking code in the header of the page. Only change this if really needed.'),
  );
  $form['basic']['etracker_scope_noscript'] = array(
    '#type' => 'select',
    '#title' => t('Include no-script into'),
    '#options' => array(
      'header' => t('Header'),
      'footer' => t('Footer') . ' (' . t('recommended') . ')',
    ),
    '#default_value' => variable_get('etracker_scope_noscript', 'footer'),
    '#description' => t('For visitor with javascript turned off there is a fall-back solution that doesn\'t require javascript and as this comes with less functionality, it is not required early in the page and therefore can easily go to the bottom of your page.'),
  );

  $form['settings'] = array(
    '#type' => 'fieldset',
    '#title' => t('Settings'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );

  // Tracking multi-lingual:
  $language_default = _etracker_get_language_settings();
  $tx_language_as_tag = t('Add a tag for the current language');
  $form['settings']['etracker_multilingual'] = array(
    '#type' => 'checkboxes',
    '#title' => t('Tracking of multi-lingual sites'),
    '#options' => array(
      'tag' => $tx_language_as_tag,
      'area prefix' => t('Add language as prefix to each area'),
      'area suffix' => t('Add language as suffix to each area'),
      'page title suffix' => t('Append language to the page name'),
    ),
    '#default_value' => $language_default,
    '#description' => t('On multi-lingual sites the ISO abbreviation of the current language can be added to several parts of the tracking code. Please enable those parts that you would like to support.')
  );
  $form['settings']['etracker_multilingual_info_tag'] = array(
    '#type' => 'container',
    '#states' => array(
      'invisible' => array(
        ':input[name="etracker_multilingual[tag]"]' => array('checked' => FALSE),
      ),
    ),
    '#attributes' => array(
      'class' => array('etracker', 'account-setup'),
    ),
    'description' => array(
      '#markup' => '<span class="field">' . $tx_language_as_tag . '</span>' . t('To use the current language as a tag you need to setup a segment for each language to be tracked by going to your etracker account into Settings / Segmentation. Add a segment of type "free" with the attribute "language" and the value of the language, e.g. "en" for English or "de" for German.'),
    ),
  );

  // Tracking per path:
  $visible_path_mode = variable_get('etracker_visible_path_mode', ETRACKER_VISIBLE_PATH_MODE_ALL);
  $php_access = user_access('use PHP for etracker settings');
  if ($visible_path_mode == ETRACKER_VISIBLE_PATH_MODE_PHP && !$php_access) {
    $form['settings']['etracker_visible_path_mode'] = array(
      '#type' => 'value',
      '#value' => ETRACKER_VISIBLE_PATH_MODE_PHP,
    );
    $form['settings']['etracker_visible_path_pages'] = array(
      '#type' => 'value',
      '#value' => variable_get('etracker_visible_path_pages', ETRACKER_VISIBLE_PATH_PAGES),
    );
  }
  else {
    $options = array(
      ETRACKER_VISIBLE_PATH_MODE_ALL => t('All pages except those listed'),
      ETRACKER_VISIBLE_PATH_MODE_SELECTED => t('Only the listed pages'),
    );
    $description = t("Specify pages by using their paths. Enter one path per line. The '*' character is a wildcard. Example paths are %blog for the blog page and %blog-wildcard for every personal blog. %front is the front page.", array('%blog' => 'blog', '%blog-wildcard' => 'blog/*', '%front' => '<front>'));

    if (module_exists('php') && $php_access) {
      $options += array(ETRACKER_VISIBLE_PATH_MODE_PHP => t('Pages on which this PHP code returns <code>TRUE</code> (experts only)'));
      $title = t('Pages or PHP code');
      $description .= ' ' . t('If the PHP option is chosen, enter PHP code between %php. Note that executing incorrect PHP code can break your Drupal site.', array('%php' => '<?php ?>'));
    }
    else {
      $title = t('Pages');
    }
    $form['settings']['etracker_visible_path_mode'] = array(
      '#type' => 'radios',
      '#title' => t('Tracking specific pages'),
      '#options' => $options,
      '#default_value' => $visible_path_mode,
    );
    $form['settings']['etracker_visible_path_pages'] = array(
      '#type' => 'textarea',
      '#title' => '<span class="element-invisible">' . $title . '</span>',
      '#default_value' => variable_get('etracker_visible_path_pages', ETRACKER_VISIBLE_PATH_PAGES),
      '#description' => $description,
    );
  }

  // Tracking per role:
  $role_options = array_map('check_plain', user_roles());
  $form['settings']['etracker_visible_roles'] = array(
    '#type' => 'checkboxes',
    '#title' => t('Tracking specific roles'),
    '#default_value' => variable_get('etracker_visible_roles', array()),
    '#options' => $role_options,
    '#description' => t('Track only the selected role(s). If you select no roles, tracking will be enabled for all users.'),
  );

  // Tracking per user:
  $t_permission = array('%permission' => t('opt-in or out of etracker tracking'));
  $form['settings']['etracker_visible_user'] = array(
    '#type' => 'radios',
    '#title' => t('Allow users to customize tracking on their account page'),
    '#options' => array(
      0 => t('No customization allowed'),
      1 => t('Tracking on by default, users with %permission permission can opt out', $t_permission),
      2 => t('Tracking off by default, users with %permission permission can opt in', $t_permission)
    ),
    '#default_value' => variable_get('etracker_visible_user', 0),
  );

  // Link tracking:
  $form['settings']['links'] = array(
    '#type' => 'item',
    '#title' => t('Tracking of links'),
  );
  $form['settings']['etracker_track_external'] = array(
    '#type' => 'checkbox',
    '#title' => t('Tracking clicks on external links'),
    '#default_value' => variable_get('etracker_track_external', TRUE),
  );
  $form['settings']['etracker_track_mailto'] = array(
    '#type' => 'checkbox',
    '#title' => t('Tracking clicks on mailto links'),
    '#default_value' => variable_get('etracker_track_mailto', TRUE),
  );
  $form['settings']['etracker_track_download'] = array(
    '#type' => 'checkbox',
    '#title' => t('Tracking clicks on downloads (clicks on file links) for the following extensions'),
    '#default_value' => variable_get('etracker_track_download', TRUE),
  );
  $form['settings']['etracker_track_download_extensions'] = array(
    '#title' => t('List of download file extensions'),
    '#title_display' => 'invisible',
    '#type' => 'textfield',
    '#default_value' => variable_get('etracker_track_download_extensions', ETRACKER_TRACK_DOWNLOAD_EXTENSIONS),
    '#description' => t('A file extension list separated by the | character that will be tracked when clicked. Regular expressions are supported. For example: !extensions', array('!extensions' => ETRACKER_TRACK_DOWNLOAD_EXTENSIONS)),
    '#maxlength' => 255,
    '#states' => array(
      'enabled' => array(
        ':input[name="etracker_track_download"]' => array('checked' => TRUE),
      ),
      # Note: Form required marker is not visible as title is invisible.
      'required' => array(
        ':input[name="etracker_track_download"]' => array('checked' => TRUE),
      ),
    ),
  );

  // Tracking messages:
  $form['settings']['etracker_track_messages'] = array(
    '#type' => 'checkboxes',
    '#title' => t('Tracking messages of type'),
    '#default_value' => variable_get('etracker_track_messages', array()),
    '#description' => t('This will track the selected message types shown to users. Tracking of form validation errors may help you identifying usability issues in your site. Every message is tracked as one individual event. Messages from excluded pages cannot be tracked.'),
    '#options' => array(
      'status' => t('Status message'),
      'warning' => t('Warning message'),
      'error' => t('Error message'),
    ),
  );

  // Tracking settings misc:
  $form['settings']['misc'] = array(
    '#type' => 'item',
    '#title' => t('Miscellaneous'),
  );

  // Tracking site internal search terms:
  $site_search_dependencies = '<div class="admin-requirements">';
  $site_search_dependencies .= t('Requires: !module-list', array('!module-list' => (module_exists('search') ? t('@module (<span class="admin-enabled">enabled</span>)', array('@module' => 'Search')) : t('@module (<span class="admin-disabled">disabled</span>)', array('@module' => 'Search')))));
  $site_search_dependencies .= '</div>';
  $form['settings']['etracker_track_site_search'] = array(
    '#type' => 'checkbox',
    '#title' => t('Tracking internal search'),
    '#description' => t('If checked, internal search keywords are tracked.') . $site_search_dependencies,
    '#default_value' => variable_get('etracker_track_site_search', FALSE),
    '#disabled' => (module_exists('search') ? FALSE : TRUE),
  );

  // Tracking when DNT is set:
  $form['settings']['etracker_visible_dnt'] = array(
    '#type' => 'checkbox',
    '#title' => t('Universal web tracking opt-out'),
    '#description' => t('If enabled and your server receives the <a href="@donottrack">Do-Not-Track</a> header from the client browser, the etracker module will not embed any tracking code into your site. Compliance with Do Not Track could be purely voluntary, enforced by industry self-regulation, or mandated by state or federal law. Please accept your visitors privacy. If they have opt-out from tracking and advertising, you should accept their personal decision.', array('@donottrack' => 'http://donottrack.us/')),
    '#default_value' => variable_get('etracker_visible_dnt', TRUE),
  );

  $form['defaults'] = array(
    '#type' => 'fieldset',
    '#title' => t('Default values'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );
  $vocabularies = array();
  if (module_exists('taxonomy')) {
    foreach (taxonomy_get_vocabularies() as $vocabulary) {
      $vocabularies[$vocabulary->vid] = $vocabulary->name;
    }
  }
  $form['defaults']['etracker_mode_taxonomy_as_area'] = array(
    '#type' => 'select',
    '#title' => t('Vocabularies as areas'),
    '#options' => $vocabularies,
    '#multiple' => TRUE,
    '#default_value' => variable_get('etracker_mode_taxonomy_as_area', array()),
    '#access' => !empty($vocabularies),
    '#description' => t('If you select one or more of your taxonomies, their terms will be tracked on every page that contains one or more entities with associated terms. If terms in a taxonomy are organised in hierarchy, that will be reflected in hierarchical areas in you tracking reports.'),
  );
  $breadcrumb_modes = array(
    ETRACKER_MODE_BREADCRUMB_OFF => t('Off'),
    ETRACKER_MODE_BREADCRUMB_ON => t('On'),
    ETRACKER_MODE_BREADCRUMB_ON_EXCLUDE_HOME => t('On, exclude home'),
  );
  $form['defaults']['etracker_mode_breadcrumb_as_area'] = array(
    '#type' => 'select',
    '#title' => t('Breadcrumb as area'),
    '#options' => $breadcrumb_modes,
    '#default_value' => variable_get('etracker_mode_breadcrumb_as_area', ETRACKER_MODE_BREADCRUMB_ON),
    '#description' => t('Select if and how you want to utilize your breadcrumb as an hierarchical area for tracking. If turned on you can select whether the root of your site will be used as the most top area of the hierarchy or not.'),
  );
  $form['defaults']['etracker_mode_userrole_as_tag'] = array(
    '#type' => 'select',
    '#title' => t('User role tracking mode'),
    '#options' => array(
      ETRACKER_MODE_USERROLE_TAG_OFF => t('Off, do not tag user roles'),
      ETRACKER_MODE_USERROLE_TAG_INDIVIDUAL => t('Tag each role individually'),
      ETRACKER_MODE_USERROLE_TAG_MERGE => t('Merge all user roles into a single tag'),
    ),
    '#default_value' => variable_get('etracker_mode_userrole_as_tag', ETRACKER_MODE_USERROLE_TAG_OFF),
  );
  $form['defaults']['etracker_mode_userrole_as_tag_details'] = array(
    '#type' => 'container',
    '#states' => array(
      'invisible' => array(
        ':input[name="etracker_mode_userrole_as_tag"]' => array('value' => ETRACKER_MODE_USERROLE_TAG_OFF),
      ),
    ),
  );
  foreach ($role_options as $rid => $name) {
    $role_options[$rid] .= ' [' . $rid . ']';
  }
  $form['defaults']['etracker_mode_userrole_as_tag_details']['etracker_userroles_as_tag'] = array(
    '#type' => 'checkboxes',
    '#title' => t('User roles to include tags for'),
    '#default_value' => variable_get('etracker_userroles_as_tag', array()),
    '#options' => $role_options,
  );
  $form['defaults']['etracker_mode_userrole_as_tag_details']['info'] = array(
    '#type' => 'container',
    '#attributes' => array(
      'class' => array('etracker', 'account-setup'),
    ),
    'description' => array(
      '#markup' => t('To use the user roles as tags you need to setup a segment for each user role to be tracked by going to your etracker account into Settings / Segmentation. Add a segment of type "free" with the attribute "userrole" and the value of the role id, e.g. "1" for anonymous or "2" for authenticated users. If you\'re merging multiple user roles into one tag, then separate those role ids by commas.'),
    ),
  );

  $form['#attached']['css'] = array(
    drupal_get_path('module', 'etracker') . '/etracker.admin.css',
  );

  return system_settings_form($form);
}

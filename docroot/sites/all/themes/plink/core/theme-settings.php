<?php

// ////////////////////////////////////////////////////////////////////////////////////////////////
// 
// Settings for the Plink theme
// 
// ////////////////////////////////////////////////////////////////////////////////////////////////


// Add the form's CSS & JS
// ----------------------------------------------------------------------------------------
drupal_add_css(drupal_get_path('theme', 'plink') . '/css/theme-settings.css', 'file');
drupal_add_js(drupal_get_path('theme','plink') . "/js/theme-settings.js", 'file');
// ----------------------------------------------------------------------------------------


/**
* Implementation of THEMENAME_form_system_theme_settings_alter() function.
*/
function plink_form_system_theme_settings_alter(&$form, &$form_state) {  
  // Retrieve the theme key for the theme being edited. 
  $theme_key = $form_state['build_info']['args'][0];
	$theme_data = list_themes();
  global $theme_path;

	// Get the default settings from the .info file
	$settings = ($theme_key && isset($theme_data[$theme_key]->info['settings'])) ? $theme_data[$theme_key]->info['settings'] : array();
	
	//Get the defined regions from the .info file
  $regions_unwanted = array('help', 'page_top', 'page_bottom', 'dashboard_main', 'dashboard_sidebar', 'dashboard_inactive');
  $regions = array();
  
  foreach($theme_data[$theme_key]->info['regions'] as $key => $value) {
    if (!in_array($key, $regions_unwanted)) {
       $regions[$key] = $value;
     }
  }

// ----------------------------------------------------------------------------------------
// DEFAULT FORM OPTIONS
// ----------------------------------------------------------------------------------------  

	// collapse the default form items
	$form['theme_settings']['#collapsible'] = TRUE;
	$form['theme_settings']['#collapsed'] = TRUE;
	$form['favicon']['#collapsible'] = TRUE;
	$form['favicon']['#collapsed'] = TRUE;

	// Drupal Core expects these values to be present. Lets kill any php notices by
	// giving core a couple of empty values
	
  $form['logo_path'] = array(
    '#type' => 'value',
    '#value' => '',
  );
  $form['logo_upload'] = array(
    '#type' => 'value',
    '#value' => '',
  );
  
// ----------------------------------------------------------------------------------------
// Node Meta Information Settings
// ----------------------------------------------------------------------------------------
  $meta_string = plink_theme_get_setting('meta_info_string', $theme_key);

  $form['meta_info'] = array(
	  '#type' => 'fieldset',
	  '#title' => t('Custom Node Post Meta Information'),
	  '#description' => t('Override the node post meta information string that is displayed for nodes.'),
	  '#collapsible' => TRUE,
	  '#collapsed' => TRUE,
	  '#access' => user_access('Administer themes'),
	);

	$form['meta_info']['enable_meta_info'] = array(
	  '#type' => 'checkbox',
	  '#title' => t('Enable custom node meta information.'),
	  '#default_value' => plink_theme_get_setting('enable_meta_info', $theme_key),
	);

  $form['meta_info']['meta_info_string'] = array(
    '#type' => 'textfield',
    '#title' => t('Custom string'),
    '#default_value' => $meta_string ? $meta_string : 'Published by [node:author] [node:created:since] ago.',
    '#description' => module_exists('token') ? '' : '<em>If you want to see a list of available tokens, please install the Token module.',
    '#states' => array(
      'visible' => array(
        ':input[name="enable_meta_info"]' => array('checked' => TRUE),
      ),
    ),
  );
  
  if (module_exists('token')) {
    // Add the token help to a collapsed fieldset at the end of the configuration page.
    $form['meta_info']['token_help'] = array(
      '#type' => 'fieldset',
      '#title' => t('Available Tokens List'),
      '#collapsible' => TRUE,
      '#collapsed' => TRUE,
      '#states' => array(
        'visible' => array(
          ':input[name="enable_meta_info"]' => array('checked' => TRUE),
        ),
      ),
    );
    $form['meta_info']['token_help']['content'] = array(
     '#theme' => 'token_tree',
     '#token_types' => array('node', 'comment', 'term', 'user'),
    );
  }
  
// ----------------------------------------------------------------------------------------
// Apple Touch Favicon Settings
// ----------------------------------------------------------------------------------------

  $form['apple_touch'] = array(
    '#type' => 'fieldset',
    '#title' => t('Apple Touch Favicon'),
    '#description' => t('When creating a bookmark on iPhone, iPad, etc. this icon will be displayed on the home screen. For best results in all devices use a 114x114px image in PNG format.'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
    '#access' => user_access('Administer themes'),
  );

  $form['apple_touch']['enable_apple_touch'] = array(
    '#type' => 'checkbox',
    '#title' => t('Enable Apple Touch Favicon'),
    '#default_value' => plink_theme_get_setting('enable_apple_touch', $theme_key),
  );

  $touch_path = plink_theme_get_setting('apple_touch_icon_path', $theme_key);
    
  $form['apple_touch']['apple_touch_icon_path'] = array(
    '#type' => 'textfield',
    '#title' => t('Path to apple touch favicon'),
    '#default_value' => (!empty($touch_path)) ? $touch_path : drupal_get_path('theme', $theme_key) . '/img/apple-touch.png',
    '#states' => array(
      'visible' => array(
        ':input[name="enable_apple_touch"]' => array('checked' => TRUE),
      ),
    ),
  );

  $form['apple_touch']['apple_touch_icon_upload'] = array(
    '#type' => 'file',
    '#title' => t('Upload apple touch favicon image'),
    '#description' => t("If you don't have direct file access to the server, use this field to upload your shortcut icon."),
    '#states' => array(
      'visible' => array(
        ':input[name="enable_apple_touch"]' => array('checked' => TRUE),
      ),
    ),
  );
  $form['#submit'][] = 'plink_theme_settings_submit';
  $form['apple_touch']['apple_touch_icon_upload']['#element_validate'][] = 'plink_theme_settings_submit';

// ----------------------------------------------------------------------------------------
// Breadcrumbs Settings
// ----------------------------------------------------------------------------------------

  $form['breadcrumbs'] = array(
	  '#type' => 'fieldset',
	  '#title' => t('Breadcrumbs Settings'),
	  '#description' => t(''),
	  '#collapsible' => TRUE,
	  '#collapsed' => TRUE,
	  '#access' => user_access('Administer themes'),
	);

	$form['breadcrumbs']['enable_breadcrumbs'] = array(
	  '#type' => 'checkbox',
	  '#title' => t('Enable breadcrumbs'),
	  '#default_value' => plink_theme_get_setting('enable_breadcrumbs', $theme_key),
	);
	

// ----------------------------------------------------------------------------------------
// Master Layout Settings
// ----------------------------------------------------------------------------------------
 	
		$form['master'] = array(
		  '#type' => 'fieldset',
		  '#title' => t('Master Layout Settings'),
		  '#description' => t('Below are the settings for your master layout and main content regions. Please choose a content layout from the right and set the widths of the sidebars below. The Primary [1] region will always take up the remaining space.'),
		  '#collapsible' => TRUE,
		  '#collapsed' => FALSE,
		  '#access' => user_access('Administer themes'),
		);
		
		
		$form['master']['left_column_wrapper'] = array(
  		'#markup' => '<div class="settings-left-column">',
  	);
		
		$form['master']['main_screen_min'] = array(
		  '#title' => 'Min',
      '#type' => 'textfield',
      '#size' => 10,
      '#maxlength' => 8,
      '#prefix' => "<h2>" . t('SCREEN SIZE') . "</h2>",
      '#field_suffix' => t(' px'),
      '#default_value' => plink_theme_get_setting('main_screen_min', $theme_key),
    );
		
		$form['master']['main_screen_max'] = array(
      '#type' => 'textfield',
      '#size' => 10,
      '#maxlength' => 8,
      '#title' => t('max'),
      '#field_suffix' => t(' px'),
      '#default_value' => plink_theme_get_setting('main_screen_max', $theme_key),
    );
    
    $form['master']['main_grid_width'] = array(
      '#title' => 'Grid Width',
      '#type' => 'textfield',
      '#size' => 10,
      '#maxlength' => 8,
      '#prefix' => '<br/><h2>GRID SETTINGS</h2>',
      '#field_suffix' => t(' px'),
      '#default_value' => plink_theme_get_setting('main_grid_width', $theme_key),
      '#attributes' => array('class'=>array('grid-width')),
    );
    
    $form['master']['main_grid_columns'] = array(
      '#title' => 'Grid Columns',
      '#type' => 'textfield',
      '#size' => 10,
      '#maxlength' => 8,
      '#default_value' => plink_theme_get_setting('main_grid_columns', $theme_key),
      '#attributes' => array('class'=>array('grid-columns')),
    );
		
		$form['master']['main_grid_gutter'] = array(
      '#title' => 'Grid Gutter',
      '#type' => 'textfield',
      '#size' => 10,
      '#maxlength' => 8,
      '#field_suffix' => t(' px'),
      '#default_value' => plink_theme_get_setting('main_grid_gutter', $theme_key),
      '#attributes' => array('class'=>array('grid-gutter')),
    );
    
	
	  $form['master']['main_secondary_width'] = array(
  	  '#type' => 'textfield',
  	  '#title' => t('Secondary [2] Width'),
  	  '#size' => 20,
  	  '#maxlength' => 8,
  		'#prefix' => '<strong class="setting-heading">' . t('Sidebar Width Settings') . '</strong>',
  		'#attributes' => array('class'=>array('manualsizeinput', 'secondary-width'),'rel'=>'secondary'),
  		'#default_value' => plink_theme_get_setting('main_secondary_width', $theme_key),
  	);

  	$form['master']['main_tertiary_width'] = array(
  	  '#type' => 'textfield',
  	  '#title' => t('Tertiary [3] Width'),
  	  '#size' => 20,
  	  '#maxlength' => 8,
  		'#attributes' => array('class'=>array('manualsizeinput', 'tertiary-width'), 'rel'=>'tertiary'),
  		'#default_value' => plink_theme_get_setting('main_tertiary_width', $theme_key),
  	);	

  	$form['master']['left_column_wrapper_closure'] = array(
  		'#markup' => '</div>',
  	);

  	//
  	// RIGHT COLUMN
  	//

  	$form['master']['right_column_wrapper'] = array(
  		'#markup' => '<div class="settings-right-column">',
  	);

  	$form['master']['heading'] = array(
  	  '#markup' => t('Content Layout Options'),
  		'#prefix' => '<strong class="setting-heading">',
  		'#suffix' => '</strong>',
  	);

  	$form['master']['main_layout'] = array(
  	  '#type' => 'radios',
  	  '#options' => plink_theme_get_layout_options('main'),
  	  '#default_value' => plink_theme_get_setting('main_layout',$theme_key),
  	  '#attributes' => array('class'=>array('fancy-radios')),
  	);



  	$form['master']['right_column_wrapper_closure'] = array(
  		'#markup' => '</div>',
  	);
	
	
	
// ----------------------------------------------------------------------------------------
// Page Layout Options
// ----------------------------------------------------------------------------------------	
	
	$form['page_layout_options'] = array(
	  '#type' => 'fieldset',
	  '#title' => t('Page layout options'),
	  '#collapsible' => TRUE,
	  '#collapsed' => FALSE,
	  '#access' => user_access('Administer themes'),
	);
	
	//
	// LEFT COLUMN
	//
	 
	$form['page_layout_options']['left_column_wrapper'] = array(
		'#markup' => '<div class="settings-left-column">',
	);
	
	$form['page_layout_options']['full_width_regions'] = array(
	  '#type' => 'checkboxes',
	  '#title' => t(''),
		'#prefix' => '<strong class="setting-heading">' . t('100% width regions') . '</strong>',
		'#options' => array(
			'header' => 'header',
			'preface_first' => 'preface_first',
			'preface' => 'preface',
			'preface_last' => 'preface_last',
			'postscript_first' => 'postscript_first',
			'postscript' => 'postscript',
			'postscript_last' => 'postscript_last',
			'footer' => 'footer',
			),
		'#default_value' => plink_theme_get_setting('full_width_regions', $theme_key),	
	);
	
	$form['page_layout_options']['left_column_wrapper_closure'] = array(
		'#markup' => '</div>',
	);
	
	//
	// RIGHT COLUMN
	//
	
	$page_layout_preview = file_get_contents( DRUPAL_ROOT . "/" . drupal_get_path('theme', 'plink') . '/engine/page_layout_preview.html');
	
	$form['page_layout_options']['right_column_wrapper'] = array(
		'#markup' => '<div class="settings-right-column">',
	);
	
	$form['page_layout_options']['heading'] = array(
	  '#markup' => t('Layout Preview'),
		'#prefix' => '<strong class="setting-heading">',
		'#suffix' => '</strong>',
	);

	$form['page_layout_options']['page_layout_preview'] = array(
		'#markup' => $page_layout_preview,
	);
	

	
	$form['page_layout_options']['right_column_wrapper_closure'] = array(
		'#markup' => '</div>',
	);
	


 // ----------------------------------------------------------------------------------------
 // MEDIA QUERY LAYOUT 1
 // ----------------------------------------------------------------------------------------

  $form['content_layouts_mq1'] = array(
      '#type' => 'fieldset',
      '#title' => t('Responsive Layout One'),
      '#description' => t('This is a responsive layout where you can set different configurations and layouts for specific screen sizes. If you are not familiar with media queries or responsive layouts I suggest you do some googling before playing around with these options.'),
      '#collapsible' => TRUE,
      '#collapsed' => FALSE,
      '#access' => user_access('Administer themes'),
    );
    
    //
    // LEFT COLUMN
    //
    
    $form['content_layouts_mq1']['left_column_wrapper'] = array(
     '#markup' => '<div class="settings-left-column">',
    );
    
    $form['content_layouts_mq1']['enable_responsive_mq1'] = array(
      '#type' => 'checkbox',
      '#title' => t('Enable Repsonsive Layout'),
      '#default_value' => plink_theme_get_setting('enable_responsive_mq1', $theme_key),
    );
    
    $form['content_layouts_mq1']['mq1_screen_min'] = array(
		  '#title' => 'Min',
      '#type' => 'textfield',
      '#size' => 10,
      '#maxlength' => 8,
      '#prefix' => "<h2>" . t('SCREEN SIZE') . "</h2>",
      '#field_suffix' => t(' px'),
      '#default_value' => plink_theme_get_setting('mq1_screen_min', $theme_key),
    );
		
		$form['content_layouts_mq1']['mq1_screen_max'] = array(
      '#type' => 'textfield',
      '#size' => 10,
      '#maxlength' => 8,
      '#title' => t('max'),
      '#field_suffix' => t(' px'),
      '#default_value' => plink_theme_get_setting('mq1_screen_max', $theme_key),
    );
    
    $form['content_layouts_mq1']['mq1_grid_width'] = array(
      '#title' => 'Grid Width',
      '#type' => 'textfield',
      '#size' => 10,
      '#maxlength' => 8,
      '#prefix' => '<br/><h2>GRID SETTINGS</h2>',
      '#field_suffix' => t(' px'),
      '#default_value' => plink_theme_get_setting('mq1_grid_width', $theme_key),
      '#attributes' => array('class'=>array('grid-width')),
    );
    
    $form['content_layouts_mq1']['mq1_grid_columns'] = array(
      '#title' => 'Grid Columns',
      '#type' => 'textfield',
      '#size' => 10,
      '#maxlength' => 8,
      '#default_value' => plink_theme_get_setting('mq1_grid_columns', $theme_key),
      '#attributes' => array('class'=>array('grid-columns')),
    );
		
		$form['content_layouts_mq1']['mq1_grid_gutter'] = array(
      '#title' => 'Grid Gutter',
      '#type' => 'textfield',
      '#size' => 10,
      '#maxlength' => 8,
      '#field_suffix' => t(' px'),
      '#default_value' => plink_theme_get_setting('mq1_grid_gutter', $theme_key),
      '#attributes' => array('class'=>array('grid-gutter')),
    );
    
    
    $form['content_layouts_mq1']['mq1_secondary_width'] = array(
      '#type' => 'textfield',
      '#title' => t('Secondary [2] Width'),
      '#size' => 20,
      '#maxlength' => 8,
      '#prefix' => '<strong class="setting-heading">' . t('Sidebar Width Settings') . '</strong>',
      '#attributes' => array('class'=>array('manualsizeinput', 'secondary-width'),'rel'=>'secondary'),
      '#default_value' => plink_theme_get_setting('mq1_secondary_width', $theme_key),
    );
    
    $form['content_layouts_mq1']['mq1_tertiary_width'] = array(
      '#type' => 'textfield',
      '#title' => t('Tertiary [3] Width'),
      '#size' => 20,
      '#maxlength' => 8,
      '#attributes' => array('class'=>array('manualsizeinput', 'tertiary-width'),'rel'=>'tertiary'),
      '#default_value' => plink_theme_get_setting('mq1_tertiary_width', $theme_key),
    ); 
    
    $form['content_layouts_mq1']['left_column_wrapper_closure'] = array(
     '#markup' => '</div>',
    );
    
    //
    // RIGHT COLUMN
    //
    
    $form['content_layouts_mq1']['right_column_wrapper'] = array(
     '#markup' => '<div class="settings-right-column">',
    );
    
    $form['content_layouts_mq1']['heading'] = array(
      '#markup' => t('Content Layout Options'),
     '#prefix' => '<strong class="setting-heading">',
     '#suffix' => '</strong>',
    );
    
    $form['content_layouts_mq1']['responsive_layouts_mq1'] = array(
  	  '#type' => 'radios',
  	  '#options' => plink_theme_get_layout_options('main'),
  	  '#default_value' => plink_theme_get_setting('responsive_layouts_mq1',$theme_key),
  	  '#attributes' => array('class'=>array('fancy-radios')),
    );
        
    $form['content_layouts_mq1']['right_column_wrapper_closure'] = array(
     '#markup' => '</div>',
    );
    
    
    
    
    
    
// ----------------------------------------------------------------------------------------
// MEDIA QUERY LAYOUT 2
// ----------------------------------------------------------------------------------------

$form['content_layouts_mq2'] = array(
  '#type' => 'fieldset',
  '#title' => t('Responsive Layout Two'),
  '#description' => t('This is a responsive layout where you can set different configurations and layouts for specific screen sizes. If you are not familiar with media queries or responsive layouts I suggest you do some googling before playing around with these options.'),
  '#collapsible' => TRUE,
  '#collapsed' => FALSE,
  '#access' => user_access('Administer themes'),
);

//
// LEFT COLUMN
//

$form['content_layouts_mq2']['left_column_wrapper'] = array(
 '#markup' => '<div class="settings-left-column">',
);

$form['content_layouts_mq2']['enable_responsive_mq2'] = array(
  '#type' => 'checkbox',
  '#title' => t('Enable Repsonsive Layout'),
  '#default_value' => plink_theme_get_setting('enable_responsive_mq2', $theme_key),
);

$form['content_layouts_mq2']['mq2_screen_min'] = array(
  '#title' => 'Min',
  '#type' => 'textfield',
  '#size' => 10,
  '#maxlength' => 8,
  '#prefix' => "<h2>" . t('SCREEN SIZE') . "</h2>",
  '#field_suffix' => t(' px'),
  '#default_value' => plink_theme_get_setting('mq2_screen_min', $theme_key),
);

$form['content_layouts_mq2']['mq2_screen_max'] = array(
  '#type' => 'textfield',
  '#size' => 10,
  '#maxlength' => 8,
  '#title' => t('max'),
  '#field_suffix' => t(' px'),
  '#default_value' => plink_theme_get_setting('mq2_screen_max', $theme_key),
);

$form['content_layouts_mq2']['mq2_grid_width'] = array(
  '#title' => 'Grid Width',
  '#type' => 'textfield',
  '#size' => 10,
  '#maxlength' => 8,
  '#prefix' => '<br/><h2>GRID SETTINGS</h2>',
  '#field_suffix' => t(' px'),
  '#default_value' => plink_theme_get_setting('mq2_grid_width', $theme_key),
  '#attributes' => array('class'=>array('grid-width')),
);

$form['content_layouts_mq2']['mq2_grid_columns'] = array(
  '#title' => 'Grid Columns',
  '#type' => 'textfield',
  '#size' => 10,
  '#maxlength' => 8,
  '#default_value' => plink_theme_get_setting('mq2_grid_columns', $theme_key),
  '#attributes' => array('class'=>array('grid-columns')),
);

$form['content_layouts_mq2']['mq2_grid_gutter'] = array(
  '#title' => 'Grid Gutter',
  '#type' => 'textfield',
  '#size' => 10,
  '#maxlength' => 8,
  '#field_suffix' => t(' px'),
  '#default_value' => plink_theme_get_setting('mq2_grid_gutter', $theme_key),
  '#attributes' => array('class'=>array('grid-gutter')),
);

$form['content_layouts_mq2']['mq2_secondary_width'] = array(
  '#type' => 'textfield',
  '#title' => t('Secondary [2] Width'),
  '#size' => 20,
  '#maxlength' => 8,
 '#prefix' => '<strong class="setting-heading">' . t('Sidebar Width Settings') . '</strong>',
 '#attributes' => array('class'=>array('manualsizeinput', 'secondary-width'),'rel'=>'secondary'),
 '#default_value' => plink_theme_get_setting('mq2_secondary_width', $theme_key),
);

$form['content_layouts_mq2']['mq2_tertiary_width'] = array(
  '#type' => 'textfield',
  '#title' => t('Tertiary [3] Width'),
  '#size' => 20,
  '#maxlength' => 8,
 '#attributes' => array('class'=>array('manualsizeinput', 'tertiary-width'),'rel'=>'tertiary'),
 '#default_value' => plink_theme_get_setting('mq2_tertiary_width', $theme_key),
); 

$form['content_layouts_mq2']['left_column_wrapper_closure'] = array(
 '#markup' => '</div>',
);

//
// RIGHT COLUMN
//

$form['content_layouts_mq2']['right_column_wrapper'] = array(
 '#markup' => '<div class="settings-right-column">',
);

$form['content_layouts_mq2']['heading'] = array(
  '#markup' => t('Content Layout Options'),
 '#prefix' => '<strong class="setting-heading">',
 '#suffix' => '</strong>',
);

$form['content_layouts_mq2']['responsive_layouts_mq2'] = array(
  '#type' => 'radios',
  '#options' => plink_theme_get_layout_options('main'),
  '#default_value' => plink_theme_get_setting('responsive_layouts_mq2',$theme_key),
  '#attributes' => array('class'=>array('fancy-radios')),
);

$form['content_layouts_mq2']['right_column_wrapper_closure'] = array(
 '#markup' => '</div>',
);
    
    



// ----------------------------------------------------------------------------------------
// MEDIA QUERY LAYOUT 3
// ----------------------------------------------------------------------------------------

$form['content_layouts_mq3'] = array(
  '#type' => 'fieldset',
  '#title' => t('Responsive Layout Three'),
  '#description' => t('This is a responsive layout where you can set different configurations and layouts for specific screen sizes. If you are not familiar with media queries or responsive layouts I suggest you do some googling before playing around with these options.'),
  '#collapsible' => TRUE,
  '#collapsed' => FALSE,
  '#access' => user_access('Administer themes'),
);

//
// LEFT COLUMN
//

$form['content_layouts_mq3']['left_column_wrapper'] = array(
 '#markup' => '<div class="settings-left-column">',
);

$form['content_layouts_mq3']['enable_responsive_mq3'] = array(
  '#type' => 'checkbox',
  '#title' => t('Enable Repsonsive Layout'),
  '#default_value' => plink_theme_get_setting('enable_responsive_mq3', $theme_key),
);

$form['content_layouts_mq3']['mq3_screen_min'] = array(
  '#title' => 'Min',
  '#type' => 'textfield',
  '#size' => 10,
  '#maxlength' => 8,
  '#prefix' => "<h2>" . t('SCREEN SIZE') . "</h2>",
  '#field_suffix' => t(' px'),
  '#default_value' => plink_theme_get_setting('mq3_screen_min', $theme_key),
);

$form['content_layouts_mq3']['mq3_screen_max'] = array(
  '#type' => 'textfield',
  '#size' => 10,
  '#maxlength' => 8,
  '#title' => t('max'),
  '#field_suffix' => t(' px'),
  '#default_value' => plink_theme_get_setting('mq3_screen_max', $theme_key),
);

$form['content_layouts_mq3']['mq3_grid_width'] = array(
  '#title' => 'Grid Width',
  '#type' => 'textfield',
  '#size' => 10,
  '#maxlength' => 8,
  '#prefix' => '<br/><h2>GRID SETTINGS</h2>',
  '#field_suffix' => t(' px'),
  '#default_value' => plink_theme_get_setting('mq3_grid_width', $theme_key),
  '#attributes' => array('class'=>array('grid-width')),
);

$form['content_layouts_mq3']['mq3_grid_columns'] = array(
  '#title' => 'Grid Columns',
  '#type' => 'textfield',
  '#size' => 10,
  '#maxlength' => 8,
  '#default_value' => plink_theme_get_setting('mq3_grid_columns', $theme_key),
  '#attributes' => array('class'=>array('grid-columns')),
);

$form['content_layouts_mq3']['mq3_grid_gutter'] = array(
  '#title' => 'Grid Gutter',
  '#type' => 'textfield',
  '#size' => 10,
  '#maxlength' => 8,
  '#field_suffix' => t(' px'),
  '#default_value' => plink_theme_get_setting('mq3_grid_gutter', $theme_key),
  '#attributes' => array('class'=>array('grid-gutter')),
);


$form['content_layouts_mq3']['mq3_secondary_width'] = array(
  '#type' => 'textfield',
  '#title' => t('Secondary [2] Width'),
  '#size' => 20,
  '#maxlength' => 8,
 '#prefix' => '<strong class="setting-heading">' . t('Sidebar Width Settings') . '</strong>',
 '#attributes' => array('class'=>array('manualsizeinput', 'secondary-width'),'rel'=>'secondary'),
 '#default_value' => plink_theme_get_setting('mq3_secondary_width', $theme_key),
);

$form['content_layouts_mq3']['mq3_tertiary_width'] = array(
  '#type' => 'textfield',
  '#title' => t('Tertiary [3] Width'),
  '#size' => 20,
  '#maxlength' => 8,
 '#attributes' => array('class'=>array('manualsizeinput', 'tertiary-width'),'rel'=>'tertiary'),
 '#default_value' => plink_theme_get_setting('mq3_tertiary_width', $theme_key),
); 

$form['content_layouts_mq3']['left_column_wrapper_closure'] = array(
 '#markup' => '</div>',
);

//
// RIGHT COLUMN
//

$form['content_layouts_mq3']['right_column_wrapper'] = array(
 '#markup' => '<div class="settings-right-column">',
);

$form['content_layouts_mq3']['heading'] = array(
  '#markup' => t('Content Layout Options'),
 '#prefix' => '<strong class="setting-heading">',
 '#suffix' => '</strong>',
);

$form['content_layouts_mq3']['responsive_layouts_mq3'] = array(
  '#type' => 'radios',
  '#options' => plink_theme_get_layout_options('main'),
  '#default_value' => plink_theme_get_setting('responsive_layouts_mq3',$theme_key),
  '#attributes' => array('class'=>array('fancy-radios')),
);

$form['content_layouts_mq3']['right_column_wrapper_closure'] = array(
 '#markup' => '</div>',
);  
    
    
    

	
	
}

/**
 * Helper function for intercepting variables when using plinko
 **/ 

function plink_theme_get_setting($var, $theme = NULL) {
  $ts = array();
  
  if(module_exists('plinko')) {
    $ts = plinko_theme_get_setting($var,$theme);
  } else {
    $ts = theme_get_setting($var,$theme);
  }
	return $ts;
}

/**
 * Handle the form values after submit
 * @param form Nested array of form elements that comprise the form
 * @param form_state A keyed array containing the current state of the form 
 */ 
function plink_theme_settings_submit($form, &$form_state) {
  // Check for an apple touch favicon upload, and use that if available.
  if ($file = file_save_upload('apple_touch_icon_upload')) {
    
    $icon_path = variable_get('file_public_path', conf_path() . '/files') . '/' . $file->filename;
    // The image was saved using file_save_upload() and was added to the
    // files table as a temporary file. We'll make a copy to the files directory and let the garbage
    // collector delete the original upload.
    if (file_copy($file, 'public://' . $file->filename, FILE_EXISTS_REPLACE)) {
      $_POST['enable_apple_touch'] = $form_state['values']['enable_apple_touch'] = TRUE;
      $_POST['apple_touch_icon_path'] = $form_state['values']['apple_touch_icon_path'] = $icon_path;
    }
  }
}


/**
 * Helper function that returns the layout options for Plink.
 **/

function plink_theme_get_layout_options($type = "main") {
	
	$options = array();
	
	switch($type) {
		case "main" :
			$options[1] = "<div class=\"layout-block option1\"><div class=\"sec\">2</div><div class=\"prim\">1</div><div class=\"ter\">3</div><div class=\"error-cell\"></div></div>\r\n";
			$options[2] = "<div class=\"layout-block option2\"><div class=\"ter\">3</div><div class=\"prim\">1</div><div class=\"sec\">2</div><div class=\"error-cell\"></div></div>\r\n";
			$options[3] = "<div class=\"layout-block option3\"><div class=\"prim\">1</div><div class=\"sec\">2</div><div class=\"ter\">3</div><div class=\"error-cell\"></div></div>\r\n";
			$options[4] = "<div class=\"layout-block option4\"><div class=\"prim\">1</div><div class=\"ter\">3</div><div class=\"sec\">2</div><div class=\"error-cell\"></div></div>\r\n";
			$options[5] = "<div class=\"layout-block option5\"><div class=\"sec\">2</div><div class=\"ter\">3</div><div class=\"prim\">1</div><div class=\"error-cell\"></div></div>\r\n";
			$options[6] = "<div class=\"layout-block option6\"><div class=\"ter\">3</div><div class=\"sec\">2</div><div class=\"prim\">1</div><div class=\"error-cell\"></div></div>\r\n";
			$options[7] = "<div class=\"layout-block option7\"><div class=\"prim\">1</div><div class=\"sec\">2</div><div class=\"ter\">3</div></div>\r\n";
			$options[8] = "<div class=\"layout-block option8\"><div class=\"prim\">1</div><div class=\"sec\">2</div><div class=\"ter\">3</div></div>\r\n";
			$options[9] = "<div class=\"layout-block option9\"><div class=\"prim\">1</div><div class=\"sec\">2</div><div class=\"ter\">3</div><div class=\"error-cell\"></div></div>\r\n";
			$options[10] = "<div class=\"layout-block option10\"><div class=\"prim\">1</div><div class=\"ter\">3</div><div class=\"sec\">2</div><div class=\"error-cell\"></div></div>\r\n";
			$options[11] = "<div class=\"layout-block option11\"><div class=\"prim\">1</div><div class=\"sec\">2</div><div class=\"ter\">3</div></div>\r\n";
			$options[12] = "<div class=\"layout-block option12\"><div class=\"prim\">1</div><div class=\"sec\">2</div><div class=\"ter\">3</div></div>\r\n";
			$options[13] = "<div class=\"layout-block option13\"><div class=\"prim\">1</div><div class=\"sec\">2</div><div class=\"ter\">3</div></div>\r\n";
// revisit these later
//			$options[14] = "<div class=\"layout-block option13\"><div class=\"ter\">3</div><div class=\"sec\">2</div><div class=\"prim\">1</div></div>\r\n";
//			$options[15] = "<div class=\"layout-block option14\"><div class=\"ter\">3</div><div class=\"sec\">2</div><div class=\"prim\">1</div></div>\r\n";
		break;
	}
	
	return $options;
}


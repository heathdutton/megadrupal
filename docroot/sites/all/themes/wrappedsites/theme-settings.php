<?php
// include custom functions
include_once(drupal_get_path('theme', "wrappedsites") .'/inc/template-functions.inc');

function wrappedsites_form_system_theme_settings_alter(&$form, &$form_state) {
 /**
   * Theme name
   */
        wrappedsites_get_grid_cols();
	$cms_theme = "wrappedsites";

         drupal_add_js(drupal_get_path('theme', $cms_theme) . '/colorpicker/js/colorpicker.js', array('group' => CSS_THEME, 'preprocess' => FALSE));
         drupal_add_js(drupal_get_path('theme', $cms_theme) . '/colorpicker/js/eye.js', array('group' => CSS_THEME, 'preprocess' => FALSE));
         drupal_add_js(drupal_get_path('theme', $cms_theme) . '/colorpicker/js/utils.js', array('group' => CSS_THEME, 'preprocess' => FALSE));
         drupal_add_js(drupal_get_path('theme', $cms_theme) . '/colorpicker/js/layout.js', array('group' => CSS_THEME, 'preprocess' => FALSE));
         drupal_add_js(drupal_get_path('theme', $cms_theme) . '/scripts/wrappedsites-utils.js', array('group' => CSS_THEME, 'preprocess' => FALSE));

         drupal_add_css(drupal_get_path('theme', $cms_theme) . '/colorpicker/css/colorpicker.css', array('group' => CSS_THEME, 'preprocess' => FALSE));
         drupal_add_css(drupal_get_path('theme', $cms_theme) . '/colorpicker/css/layout.css', array('group' => CSS_THEME, 'preprocess' => FALSE));
         drupal_add_css(drupal_get_path('theme', $cms_theme) . '/css/admin/admin.css', array('group' => CSS_THEME, 'preprocess' => FALSE));
       
  /**
   * Get alt styles
   */
   
    
  $form['#attributes'] = array('class' => 'cmspanel');
  $altheme=variable_get('theme_default','wrappedsites');    
  $alt_stylesheet_path =  drupal_get_path('theme', $altheme) . '/css/960_grid_system/custom';
	$alt_stylesheets = array();

	if ( is_dir($alt_stylesheet_path) ) {
	    if ($alt_stylesheet_dir = opendir($alt_stylesheet_path) ) {
	        while ( ($alt_stylesheet_file = readdir($alt_stylesheet_dir)) !== false ) {
	            if(stristr($alt_stylesheet_file, ".css") !== false) {
	                $alt_stylesheets[] = $alt_stylesheet_file;
	            }
	        }
	    }
	}

	// turn into associative array
	foreach ($alt_stylesheets as $alt_style)
		$alt_styles[$alt_style] = $alt_style;

  /**
   * The default values for the theme variables. Make sure $defaults exactly
   * matches the $defaults in the template.php file.
   */


  // Create theme settings form widgets using Forms API

  // General Settings

  
  $form['wrps_container']=array();
  $form['wrps_container']['#weight']=-120;   
  $form['wrps_container']['general_settings']['#weight']=-120;          
  

 if ( theme_get_setting('grid_form')>0) {
          $cols_options=wrappedsites_get_grid_cols(); 
          $grid_regions=(array)wrappedsites_grid_regions();
          
                
          
	   // Grid Settings
	  $form['wrps_container']['general_settings']['wrappedsites'] = array(
	    '#type' => 'fieldset',
	    '#title' => t('grid 960 setting'),
	    '#collapsible' => TRUE,
	    '#collapsed' => TRUE,
	  );
                       if(theme_get_setting('altgrid')==0){ $form['wrps_container']['general_settings']['wrappedsites']['grid_cols'] = array(
			    '#type'          => 'select',
                            '#title'         => t('theme base grid system'),
                            '#description'   => t('Select cols for theme'),
                            '#default_value' => theme_get_setting('grid_cols'),
                            '#options'       => $cols_options,
			  );
                       }
                          
                  if (isset($grid_regions[0])) {    
                          
                          
                            $form['wrps_container']['general_settings']['wrappedsites']['col1wrapper'] = array(
                            '#type' => 'fieldset',
                            '#title' => t('Column 1 settings'),
                            '#collapsible' => FALSE,
                            '#collapsed' => FALSE,
                          );
                          
                          
                          $form['wrps_container']['general_settings']['wrappedsites']['col1wrapper']['col1_sidebar'] = array(
                                '#type'          => 'checkbox',
                                '#title'	=>'Enable',
                                '#description'         => t(''),
                                '#default_value' =>wrappedsites_get_cols_theme_setting('col1_sidebar',$grid_regions[0]->enable),
                              );
                          
                           
                            $form['wrps_container']['general_settings']['wrappedsites']['col1wrapper']['col1_position'] = array(
			    '#type'          => 'select',
                            '#title'         => t('position :'),
                            '#description'   => t(''),
                            '#default_value' => wrappedsites_get_cols_theme_setting('col1_position',$grid_regions[0]->position),
                            '#options'       => array('left'=>'Before main column','right'=>'After main column'),
			  );


                            
                          $form['wrps_container']['general_settings']['wrappedsites']['col1wrapper']['col1_width'] = array(
			    '#type'          => 'select',
                            '#title'         => t('Width in grid units (grid_…) :'),
                            '#description'   => t(''),
                            '#default_value' => wrappedsites_get_cols_theme_setting('col1_width',$grid_regions[0]->width),
                            '#options'       => wrappedsites_get_grid_width_cols(),
			  );

                          


                          $form['wrps_container']['general_settings']['wrappedsites']['col1wrapper']['col1_weight'] = array(
			    '#type'          => 'select',
                            '#title'         => t('weight :'),
                            '#description'   => t(''),
                            '#default_value' => wrappedsites_get_cols_theme_setting('col1_weight',$grid_regions[0]->weight),
                            '#options'       => array('0'=>'0','1'=>'1','2'=>'2','3'=>'3','4'=>'4','5'=>'5','6'=>'6','7'=>'7','8'=>'8','9'=>'9','10'=>'10','11'=>'11','12'=>'12','13'=>'13','14'=>'14','15'=>'15','16'=>'16','17'=>'17','18'=>'18','19'=>'19','20'=>'20'),
			  );


                  }


                 if (isset($grid_regions[1])) {
                            $form['wrps_container']['general_settings']['wrappedsites']['col2wrapper'] = array(
                            '#type' => 'fieldset',
                            '#title' => t('Column 2 settings :'),
                            '#collapsible' => FALSE,
                            '#collapsed' => FALSE,
                          );



                           $form['wrps_container']['general_settings']['wrappedsites']['col2wrapper']['col2_sidebar'] = array(
                                '#type'          => 'checkbox',
                                '#title'	=>'Enable',
                                '#description'         => t(''),
                                '#default_value' => wrappedsites_get_cols_theme_setting('col2_sidebar',$grid_regions[1]->enable),
                              );


                            $form['wrps_container']['general_settings']['wrappedsites']['col2wrapper']['col2_position'] = array(
			    '#type'          => 'select',
                            '#title'         => t('position :'),
                            '#description'   => t(''),
                            '#default_value' => wrappedsites_get_cols_theme_setting('col2_position',$grid_regions[1]->position),
                            '#options'       => array('left'=>'Before main column','right'=>'After main column'),
			  );

                           
                            $form['wrps_container']['general_settings']['wrappedsites']['col2wrapper']['col2_width'] = array(
			    '#type'          => 'select',
                            '#title'         => t('Width in grid units (grid_…)'),
                            '#description'   => t(''),
                            '#default_value' => wrappedsites_get_cols_theme_setting('col2_width',$grid_regions[1]->width),
                            '#options'       => wrappedsites_get_grid_width_cols(),
			  );
                            
                            
                      
                          $form['wrps_container']['general_settings']['wrappedsites']['col2wrapper']['col2_weight'] = array(
			    '#type'          => 'select',
                            '#title'         => t('weight :'),
                            '#description'   => t(''),
                            '#default_value' => wrappedsites_get_cols_theme_setting('col2_weight',$grid_regions[1]->weight),
                            '#options'       => array('0'=>'0','1'=>'1','2'=>'2','3'=>'3','4'=>'4','5'=>'5','6'=>'6','7'=>'7','8'=>'8','9'=>'9','10'=>'10','11'=>'11','12'=>'12','13'=>'13','14'=>'14','15'=>'15','16'=>'16','17'=>'17','18'=>'18','19'=>'19','20'=>'20'),
			  );
                 
                    
                          
                          
                 }   
                 
                 if (isset($alt_styles)){  
                       $form['wrps_container']['general_settings']['altgridwrapper'] = array(
                            '#type' => 'fieldset',
                            '#title' => t('Custom 960 Grid '),
                            '#collapsible' => TRUE,
                            '#collapsed' => TRUE,
                          );
     
                          
                        
                        $form['wrps_container']['general_settings']['altgridwrapper']['altgrid'] = array(
                                '#type'          => 'checkbox',
                                '#title'	=>'enable custom grid',
                                '#description'         => t(''),
                                '#default_value' => theme_get_setting('altgrid'),

                                      ); 
                        
                        
                         $form['wrps_container']['general_settings']['altgridwrapper']['alt_stylesheet'] = array(
                            '#type'          => 'select',
                            '#title'         => t('Custom 960 Grid Stylesheet'),
                                '#description'   => t('Select your themes alternative grid file'),
                            '#default_value' => theme_get_setting('alt_stylesheet'),
                            '#options'       => $alt_styles,
                          );

                         
                         $form['wrps_container']['general_settings']['altgridwrapper']['maxcols'] = array(
			'#type' => 'textfield',
			'#title' => t('Number of Cols'),
                        '#size' => 3,     
			'#description' => t('Enter count of grid columns '),
			'#default_value' => theme_get_setting('maxcols'),
                             
			'#required' => FALSE
		); 
                          
            }
                 
                 
 }



if ( theme_get_setting('colors_form')>0) {
   
    
    $color_pickers=(array)theme_get_setting('color_picker');
    $ocpickers=wrappedsites_get_color_pickers();
    $colorfile=theme_get_setting('color_file');
    
    $form_state['wrappedsites_color_pickers']=$color_pickers;
    $form_state['wrappedsites_ocpickers']=$ocpickers;
    $form_state['wrappedsites_color_file']=$colorfile;

   
   // Colors Settings
  $form['wrps_container']['general_settings']['colors_settings'] = array(
    '#type' => 'fieldset',
    '#title' => t('Colors Settings'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
    '#weight' => -120     
  );

 //"To adjust the colours used throughout your website please use the settings below.  You may already have 'Hex' colour values from your designer which you can type straight into the field, if not please click on the value in the fields below and choose the desired colour from the 'Picker'
    
    $form['wrps_container']['general_settings']['colors_settings']['colors_intro'] = array(
    '#prefix' => '<div class="color-form-intro">',
    '#markup' => t("<b>To adjust the colours used throughout your website please use the settings below.  You may already have 'Hex' colour values from your designer which you can type straight into the field, if not please click on the value in the fields below and choose the desired colour from the 'Picker'
   </b>"),
    '#suffix' => '</div><br>',
    );

  
    $form['wrps_container']['general_settings']['colors_settings']['colors_status'] = array(
		    '#type'          => 'checkbox',
		    '#title'	=>	'Use Colors?',
		    '#description'         => t('Use custom  colors for theme'),
		    '#default_value' => theme_get_setting('colors_status'),

        );


    foreach ($ocpickers as $key => $value) {
       
     if (!theme_get_setting('color_sett'.$key)){ $defval=$value->value;} else {$defval=theme_get_setting('color_sett'.$key); }
         
     $form['wrps_container']['general_settings']['colors_settings']['color_sett'.$key] = array(
        '#type' => 'textfield',
	'#title' => t($value->title),
	'#description' => $value->subtitle,
	'#default_value' => $defval,
        '#attributes' => array('style' => 'background-color:#'.$defval),
	'#required' => FALSE
	);
      
  

    }


$form['#submit'][] = 'wrappedsites_colors_form_submit';
}





return $form;

}



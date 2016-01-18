<?php

/** Implementation of hook_preprocess_page */
function wrappedsites_preprocess_page(&$vars) {
  global $grid_regions;  
  $gridObj=wrappedsites_get_settings();
  $vars['wrappedsites']=$gridObj;
  $vars['grid_regions']=wrappedsites_get_noempty_regions($gridObj);
  $grid_regions=$vars['grid_regions'];
  // Prepare header.
  $site_fields = array();
  if (!empty($vars['site_name'])) {
    $site_fields[] = $vars['site_name'];
  }
  if (!empty($vars['site_slogan'])) {
    $site_fields[] = $vars['site_slogan'];
  }
  $vars['site_title'] = implode(' ', $site_fields);
  if (!empty($site_fields)) {
    $site_fields[0] = '<span class="site-title">' . $site_fields[0] . '</span>';
  }
  $vars['site_html'] = implode(' ', $site_fields);

  // Set a variable for the site name title and logo alt attributes text.
  $slogan_text = $vars['site_slogan'];
  $site_name_text = $vars['site_name'];
  $vars['site_name_and_slogan'] = $site_name_text . ' ' . $slogan_text;
 	
}


function wrappedsites_preprocess_html(&$vars) {
  /*$vars['page']['#children'].='<'.'di'.'v cla'.'ss="con'.'tain'.'er_'.theme_get_setting('grid_cols').'"><di'.'v cla'.'ss="wrapp'.'ed-si'.'tes" sty'.'le="di'.'spa'.'ly:bl'.'ock; float:right;">Them'.'e by '.
   ' <'.'a cl'.'ass'.'="wra'.'pped-s'.'ite'.'s-li'.'nk" st'.'yle='.'"dis'.'pla'.'y:in'.'li'.'ne"'.' hre'.'f="ht'.'tp:/'.'/ww'.'w.wra'.
    'ppe'.'dsi'.'tes.c'.'om">Wr'.'ap'.'ped'.' '.'Si'.'tes'.'</'.'a'.'><'.'/di'.'v><'.'/d'.'iv>';
  */
 
}



function  wrappedsites_css_alter(&$css) {
 
 $new_color_items=theme_get_setting('new_color_item');
 $ncols=intval(theme_get_setting('grid_cols'));
 $altgrid=theme_get_setting('altgrid');
 $theme_name=variable_get('theme_default','wrappedsites');
 $wpcfile=variable_get($theme_name.'_wscolor_file', FALSE);
 if ($wpcfile){
     $css[$wpcfile] = array(
    'weight' => 10000,
    'group' => CSS_THEME,
    'media' => 'all',
    'type' => 'file',
    'every_page' => '1',
    'preprocess' => TRUE,
    'browsers' => array( 'IE' => TRUE, '!IE' => TRUE ),
    'data' => $wpcfile,

  );


 }

    $i=0.01;
    foreach ($css as $key => $value) {

      if (strpos($key,'960_grid_system') !== FALSE) {
        $value['group']=-1000;
        $value['weight']=$i;
        $i=$i+0.01;
        $css[$key]=$value;
      }

      if (strpos($key,'css/960_grid_system/960.css') !== FALSE) {
          if ($altgrid){
              $theme_name=variable_get('theme_default','wrappedsites'); 
              $theme_path=drupal_get_path('theme',$theme_name);
              $gridfile=theme_get_setting('alt_stylesheet');
              $value['data']=$theme_path.'/css/960_grid_system/custom/'.$gridfile;
              $css[$key]=$value;
        
          }
      }

    }

    
}


function wrappedsites_preprocess_region (&$variables) {
    $cregion=$variables['region'];
    $gtid_items=theme_get_setting('grid_sidebar');
    global $grid_regions;
    
    if (isset($gtid_items[$cregion])){ //check if grid960 region
        
      if (!isset($grid_regions)){$grid_regions=wrappedsites_get_noempty_regions();}
    
    foreach ($grid_regions as $key => $value) {
        
        if ($value->region==$cregion) {
             $variables['classes_array'][]='sidebar';
             $variables['classes_array'][]='grid_'.$value->width;
             $variables['classes_array'][]='pull_'.$value->pull;
            
        }
        
    }
    
    }
   
}
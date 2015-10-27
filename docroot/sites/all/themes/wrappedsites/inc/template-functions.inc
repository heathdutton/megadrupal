<?php

function wrappedsites_get_settings(){
  $grid_regions=(array)wrappedsites_grid_regions();

  $resObj= new stdClass();
  $resObj->gridType=theme_get_setting('grid_cols');

  if (isset($grid_regions[0])) {
  $resObj->isCol1=wrappedsites_get_cols_theme_setting('col1_sidebar',$grid_regions[0]->enable);
  $resObj->region1=$grid_regions[0]->id;
  $resObj->widthCol1=wrappedsites_get_cols_theme_setting('col1_width',$grid_regions[0]->width);
  $resObj->positionCol1= wrappedsites_get_cols_theme_setting('col1_position',$grid_regions[0]->position);
  $resObj->weightCol1=wrappedsites_get_cols_theme_setting('col1_weight',$grid_regions[0]->weight);
  $resObj->totalWeightCol1=wrappedsites_get_total_col_weight($resObj->positionCol1, $resObj->weightCol1);

  }

  else { $resObj->isCol1=0;   }

  if (isset($grid_regions[1])) {
  $resObj->isCol2=wrappedsites_get_cols_theme_setting('col2_sidebar',$grid_regions[1]->enable);
  $resObj->region2=$grid_regions[1]->id;
  $resObj->widthCol2=wrappedsites_get_cols_theme_setting('col2_width',$grid_regions[1]->width);
  $resObj->positionCol2= wrappedsites_get_cols_theme_setting('col2_position',$grid_regions[1]->position);
  $resObj->weightCol2=wrappedsites_get_cols_theme_setting('col2_weight',$grid_regions[1]->weight);
  $resObj->totalWeightCol2=wrappedsites_get_total_col_weight($resObj->positionCol2, $resObj->weightCol2);
  }

   else { $resObj->isCol2=0;   }

  return  $resObj;
}


function wrappedsites_get_total_col_weight($pos,$weight){
    $res=0;
    if ($pos=='left'){
       $res=$weight-60;
      } else {
       $res=$weight+60;
      }


 return $res;


}

function wrappedsites_get_columns_arr($wrappedsites){

  $cols=array();
  $content_width=$wrappedsites->gridType;
  
  /*other columns objects*/
  if ($wrappedsites->isCol1>0) {
        $tobj= new stdClass();
        $tobj->region=$wrappedsites->region1;
        $tobj->name='col1';
        $tobj->number=1;
        $tobj->width=$wrappedsites->widthCol1;
        $tobj->weight=$wrappedsites->totalWeightCol1;

        $content_width=$content_width-intval($tobj->width);
        $cols[intval($wrappedsites->totalWeightCol1)]=$tobj;
  }

  if ($wrappedsites->isCol2>0) {
        $tobj= new stdClass();
        $tobj->name='col2';
        $tobj->region=$wrappedsites->region2;
        $tobj->number=2;
        $tobj->width=$wrappedsites->widthCol2;
        $tobj->weight=$wrappedsites->totalWeightCol2;
        $content_width=$content_width-intval($tobj->width);
        if (!isset($cols[intval($wrappedsites->totalWeightCol2)])){
        $cols[intval($wrappedsites->totalWeightCol2)]=$tobj;}
        else {$cols[intval($wrappedsites->totalWeightCol2+1)]=$tobj;}

      }

 /*main column object*/
  $tobj= new stdClass();
  $tobj->name='content';
  $tobj->region='content';
  $tobj->number=0;
  $tobj->width=$content_width;
  $tobj->weight=0;
  $cols[0]=$tobj;


 /*sorting and return regions array */
  ksort($cols);
  $result=array();
  foreach ($cols as $key => $value) {
     $result[$value->name]=$value;
  }
return $result;
}


function  wrappedsites_get_column_pull($column_name,$content_name,$regions_arr){
  $pull=0;  
 if (isset($regions_arr[$column_name])&&$regions_arr[$column_name]->weight < $regions_arr[$content_name]->weight){
  $pull=$regions_arr[$content_name]->width;}
    
  return $pull;   
}

function  wrappedsites_get_content_push($content_name,$regions_arr){
  $push=0;
 foreach ($regions_arr as $value){
    if ($value->name==$content_name){break;}
        else {$push=$push+$value->width;
        }
   }
  return $push;
}


function wrappedsites_get_regions($gridObj){
  $regions=array();
  $regions_arr=wrappedsites_get_columns_arr($gridObj);
   foreach ($regions_arr as $value){
        if ($value->name=='content'){
            $value->push=wrappedsites_get_content_push('content', $regions_arr);
            $regions[$value->name]=$value;
        }else{
            $value->pull=wrappedsites_get_column_pull($value->name, 'content', $regions_arr);
            $regions[$value->name]=$value;
        }
    }
 return $regions;

  }


function wrappedsites_get_noempty_regions($gridObj=NULL){
  if (!isset($gridObj)){ $gridObj=wrappedsites_get_settings();} 
  $grid_regions=wrappedsites_get_regions($gridObj);
  
   $l=false;
    foreach ($grid_regions as $key=>$value) {
        $count=0; 
        if ($key!='content') {$count = count(block_list($value->region));}
        
        if ($count<1){
             $pname='isCol'.$value->number;
             $gridObj->$pname=0;
             $l=true;
        }
    }
     if ($l) { $res=wrappedsites_get_regions($gridObj);} else {$res=$grid_regions;}

 return  $res;

 
}


function wrappedsites_get_color_pickers ($preset="default") {
    $result=array();
    $color_items=(array)theme_get_setting('color_picker');
   if (isset($color_items[$preset])) { foreach ($color_items[$preset] as $key1=>$value1) {
        
       $carray=wrappedsites_get_color_picker_params ($value1, $key1);
        
       $result = array_merge($result, $carray); 
        
        
    } 
   }
    
    return  $result;
    
}


function wrappedsites_get_color_picker_params ($array, $id) {
    
    $result=array();
    
    foreach ($array as $key=>$value){
    
      $result[$id]=wrappedsites_get_color_picker_object ($value, $key,  $id);
        
    }
    
    
    return $result;
}

function wrappedsites_get_color_picker_object ($array ,$title , $id) {
     $pval="";
     $plabel=""; 
    foreach ($array as $key=>$value){
        $pval=$value;
        $plabel=$key;
    }
  
   $robject= new stdClass();
   $robject->id=$id;
   $robject->title=$title;
   $robject->value=$pval; 
   $robject->subtitle=$plabel; 
    
  return   $robject;
    
} 




function wrappedsites_get_color_items ($array, $selector) {
    
    $result=array();
    
    foreach ($array as $key=>$value){
    
      $result[]=wrappedsites_get_color_object ($value, $key,  $selector);
        
    }
    
    
    return $result;
}

function wrappedsites_get_color_object ($value ,$property , $selector) {
   $pval=$value;
   $robject= new stdClass();
   $robject->selector=$selector;
   $robject->property=$property;
   $robject->value=$pval; 
  
    
  return   $robject;
    
} 



function wrappedsites_grid_regions () {

    $result=array();
    $gtid_items=theme_get_setting('grid_sidebar');
   

    foreach ($gtid_items as $key=>$value) {

       $grid_region= wrappedsites_grid_region ($value, $key);
       $result[] =$grid_region;


    }

    return  $result;

}


function wrappedsites_grid_region ($array, $region_id) {
  
    $res_obj= new stdClass();
    $res_obj->id=$region_id;
    
    foreach ($array as $key => $value) {
        $res_obj->$key=$value;
        
    }
    
    
    return $res_obj;
    
    
}

function wrappedsites_get_cols_theme_setting($form_name,$def_val){
    $result='';
    $first_run=theme_get_setting('col1_width');
    $form_value=theme_get_setting($form_name);
    if (!isset($first_run)){$result=$def_val;} else {$result=$form_value; }
    
    return $result;
}


function wrappedsites_get_grid_cols() {
$theme_name=variable_get('theme_default','wrappedsites');    
$dcols=intval(theme_get_setting('grid_cols'));  
$altgrid=theme_get_setting('altgrid');
$vars=variable_get('theme_'.$theme_name.'_settings',array());
$maxcols=intval(theme_get_setting('maxcols'));
$options= array('12'=>'12 cols','16'=>'16 cols');

if ($altgrid==1){
if (count($vars)>0&&$maxcols>0){
    
    $vars['grid_cols']=$maxcols;  
    variable_set('theme_'.$theme_name.'_settings',$vars);
    $options[$maxcols]=$maxcols.' cols';
    
}
    
 
}else {
   if (!isset($options[$dcols])){
   $vars['grid_cols']='12';  
   variable_set('theme_'.$theme_name.'_settings',$vars);
   }
    
    
}

  return $options;  
}


function wrappedsites_get_grid_width_cols() {
  $opt=array('1'=>'1','2'=>'2','3'=>'3','4'=>'4','5'=>'5','6'=>'6','7'=>'7','8'=>'8','9'=>'9','10'=>'10','11'=>'11','12'=>'12');  
  $theme_name=variable_get('theme_default','wrappedsites');    
  $vars=variable_get('theme_'.$theme_name.'_settings',array());  
   if (isset($vars['grid_cols'])&&isset($vars['altgrid'])){
    $dcols=intval($vars['grid_cols']);
    $altgrid=$vars['altgrid'];
   
      if ($dcols>12){
          for ($i = 13; $i <= $dcols; $i++) {

              $opt[$i]=$i;

          }      

      }
  
  }
  
    return $opt;
}

function wrappedsites_colors_form_submit($form, &$form_state) {
   
    $theme_name=variable_get('theme_default','wrappedsites');
    $path = 'public://wscolor/';
    $l=file_prepare_directory($path, FILE_CREATE_DIRECTORY);
    $source = drupal_get_path('theme', $theme_name) . '/';
    $tarr=explode('/',$form_state['wrappedsites_color_file']['default']);
    $stylesheet=$form_state['wrappedsites_color_file']['default'];
    $filename=end($tarr);
    $ocpickers=(array)$form_state['wrappedsites_ocpickers'];
   
    if ($stylesheet&&file_exists($source.$stylesheet)) {
        $style = drupal_load_stylesheet($source  . $stylesheet, FALSE);
        foreach ($ocpickers as $value) {
            $selector='color_sett'.$value->id;
            $rval='#'.$form_state['input'][$selector];
            $style=str_replace('%'.$value->id.'%', $rval, $style);
        }

        $filepath = file_unmanaged_save_data($style, $path.$filename, FILE_EXISTS_REPLACE);

          // Set standard file permissions for webserver-generated files.
         drupal_chmod($path.$filename);

         variable_set($theme_name.'_wscolor_file', $path.$filename);
         drupal_set_message('css config file was detected :'.$path.$filename, 'status');
         variable_set('wrp_cssfile_completed',$theme_name);

       

    } else {variable_del($theme_name.'_wscolor_file');}


}


  
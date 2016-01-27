<?php

function biz_form_system_theme_settings_alter(&$form, &$form_state) {
if(function_exists('imagettfbbox')) {
 $theme = 'biz';
 $theme_path = drupal_get_path('theme', $theme);
 if(theme_get_setting('font') && theme_get_setting('font_size') && theme_get_setting('font_msg') && theme_get_setting('use_font')){
  $settings = array('font' => theme_get_setting('font'), 'font_size' => theme_get_setting('font_size'), 'font_msg' => theme_get_setting('font_msg'), 'use_font' => theme_get_setting('use_font'));
 } else {
  $settings = array('font' => '0', 'font_size' => '50', 'font_msg' => 'BIZ', 'use_font' => '1');
 }
 if($settings['use_font'] == 1){
 drupal_add_css('#font-box { display: none; }', array('type' => 'inline'));
 }

 $font_dir = $theme_path.'/images/fonts';
 $get_fonts = file_scan_directory($font_dir, '/.*\.((t|T)(t|T)(f|F))$/');
 $font_array = "";
 foreach($get_fonts as $font){
 $font_array['name'][$font->filename] = $font->name;
  }
  $form['use_font'] = array(
  '#type' => 'radios',
  '#options' => array('1' => t('No'), '2' => t('Yes')),
  '#title' => t('Use TTF Font for create Logo'),
  '#default_value' => $settings['use_font'],
  '#attributes' => array('class' => array('use-font')),
  );
  // Create the form widgets using Forms API
  /**/$form['font'] = array(
    '#type' => 'select',
    '#title' => t('Font'),
    '#default_value' => $settings['font'],
    '#options' => $font_array['name'],
    '#prefix' => "<div id=\"font-box\">\n",
    '#suffix' => "<div id=\"fontpreview\" style=\"width: 100%; height: 200px; background-color: #5E6577;\"></div>\n",

  );
   $form['font_msg'] = array(
    '#type' => 'textfield',
    '#title' => t('Title'),
    '#default_value' => $settings['font_msg'],

   );
   $form['font_size'] = array(
    '#type' => 'textfield',
    '#title' => t('Size'),
    '#default_value' => $settings['font_size'],
    '#size' => '2',
    '#maxlength' => '2',
    '#suffix' => "<div style=\"width: 120px; height: 20px; cursor: pointer; background-color: #fff; padding: 0; border: 2px solid #5E6577; border-radius: 10px; -moz-border-radius: 10px;\"> \n <div class=\"slider\" style=\"width: 20px; height: 20px; left: ".$settings['font_size']."px; top: 0px; background: transparent url(".base_path().$theme_path."/images/slider.png) no-repeat ; padding: 0; position:relative;\"> \n  </div>\n </div>\n </div>\n
 <script type=\"text/javascript\">
 <!--//--><![CDATA[//><!--
 (function ($) {
   var sleepTimer;
   function fontpreview(){
   $('img#fontimg').remove();
   $('div#fontpreview').append('<img src=\"".base_path().$theme_path."/images/img.php?msg='+$('#edit-font-msg').val()+'&size='+$('#edit-font-size').val()+'&font='+$('select#edit-font option:selected').val()+'\" id=\"fontimg\">');
   }
   function sleepme(myfunction){
   clearTimeout(sleepTimer);
   sleepTimer = setTimeout(myfunction,500); //return function 0.5 sec after action
   }
   
   $('input#edit-font-msg').keyup(function() {
   sleepme(fontpreview);
   });
   $('select#edit-font').change(function() {
   fontpreview();
   });
$('input.use-font').change(function() {
    if($('input.use-font:checked').val() == '2'){
     $('div#font-box').css('display', 'block');
    } else {
     $('div#font-box').css('display', 'none');
     }
   });
  
 /*********************************************** 

 * Drag and Drop Script: © Dynamic Drive (http://www.dynamicdrive.com)
 * This notice MUST stay intact for legal use 
 * Visit http://www.dynamicdrive.com/ for this script and 100s more. 
 ***********************************************/ 
 var dragobject={ 
 z: 0, x: 0, y: 0, offsetx : null, offsety : null, targetobj : null, dragapproved : 0,
 initialize:function(){ 
 document.onmousedown=this.drag 
 document.onmouseup=function(){this.dragapproved=0} 
 }, 
 drag:function(e){ 
 var evtobj=window.event? window.event : e 
 this.targetobj=window.event? event.srcElement : e.target 
 if (this.targetobj.className==\"slider\"){ 
 this.dragapproved=1 
 if (isNaN(parseInt(this.targetobj.style.left))){this.targetobj.style.left=0} 
 if (isNaN(parseInt(this.targetobj.style.top))){this.targetobj.style.top=0} 
 this.offsetx=parseInt(this.targetobj.style.left) 
 this.offsety=parseInt(this.targetobj.style.top) 
 this.x=evtobj.clientX; 
 this.y=evtobj.clientY; 
 if (evtobj.preventDefault) 
 evtobj.preventDefault() 
 document.onmousemove=dragobject.moveit;} 
 }, moveit:function(e){ 
 var evtobj=window.event? window.event : e 
 if (this.dragapproved==1){ 
 this.targetobj.style.top= 0+\"px\" 
 if((this.offsetx+evtobj.clientX-this.x) > 0 && (this.offsetx+evtobj.clientX-this.x) < 100){ 
 this.targetobj.style.left=this.offsetx+evtobj.clientX-this.x+\"px\"; 
 $('#edit-font-size').val(this.offsetx+evtobj.clientX-this.x);
 sleepme(fontpreview);} 
 return false;} 
 } 
  }
 dragobject.initialize();
 sleepme(fontpreview);
   }(jQuery));
 //--><!]]></script>",
   );
  $form['#submit'][] = 'biz_form_system_theme_settings_submit';
  return $form;/**/
 }
}
function  biz_form_system_theme_settings_submit (&$form, &$form_state) {
  if(!empty($_POST["font"]) && !empty($_POST["font_msg"]) && function_exists('imagettfbbox')){
   $theme = 'biz';
   $theme_path = drupal_get_path('theme', $theme);
   include_once($theme_path.'/images/textPNG.class.php');
   $font_dir = $theme_path.'/images/fonts';
   $textPNG = new textPNG;
   $textPNG->msg = $_POST["font_msg"]; // text to display
   $textPNG->font = $font_dir.'/'.$_POST["font"]; // font to use (include directory if needed).
   if (!empty($_POST["font_size"])) $textPNG->size = $_POST["font_size"]; // size in points
   $textPNG->output = variable_get('file_public_path', conf_path() . '/files')."/fontlogo.png";
   $textPNG->draw();
   }
  }
?>

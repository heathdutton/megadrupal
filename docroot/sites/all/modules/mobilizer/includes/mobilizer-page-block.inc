<?php
/**
 * @file
 *   Webpage content block elements - video, socialicons and share.
 */

/**
 * Function to display content selection page.
 * @return themed html list of content to add.
 */
function mobilizer_menu_add_content_type(){
  global $base_url;
  $mlid = arg(4);
  $image_base_path = $base_url . '/' . drupal_get_path('module', 'mobilizer');
  $output = '<div class = "add-page-block" >';
  $output .= '<div class = "page-type">' . l(t('<img src = ' . $image_base_path . '/images/image_ico.png' . ' />'), "admin/mobilizer/select-menu-configure/page/$mlid/add/image", $options = array(
    'html' => TRUE,
    'attributes' => array(
      'title' => 'Image'
    )
  )) . t('Image') . '</div>';
  $output .= '<div class = "page-type">' . l(t('<img src = ' . $image_base_path . '/images/image_slider_ico.png' . ' />'), "admin/mobilizer/select-menu-configure/page/$mlid/add/image-slider", $options = array(
    'html' => TRUE,
    'attributes' => array(
      'title' => 'Image Slider'
    )
  )) . t('Image Slider') . '</div>';
  $output .= '<div class = "page-type">' . l(t('<img src = ' . $image_base_path . '/images/contact_us.png' . ' />'), "admin/mobilizer/select-menu-configure/page/$mlid/add/email", $options = array(
    'html' => TRUE,
    'attributes' => array(
      'title' => 'Email'
    )
  )) . t('Email') . '</div>';
  $output .= '<div class = "page-type">' . l(t('<img src = ' . $image_base_path . '/images/video_ico.png' . ' />'), "admin/mobilizer/select-menu-configure/page/$mlid/add/video", $options = array(
    'html' => TRUE,
    'attributes' => array(
      'title' => 'Video'
    )
  )) . t('Video') . '</div>';
  $output .= '<div class = "page-type">' . l(t('<img src = ' . $image_base_path . '/images/social_ico.png' . ' />'), "admin/mobilizer/select-menu-configure/page/$mlid/social-icons", $options = array(
    'html' => TRUE,
    'attributes' => array(
      'title' => 'Social Icons'
    )
  )) . t('Social Icons') . '</div>';
  $output .= '<div class = "page-type">' . l(t('<img src = ' . $image_base_path . '/images/share_ico.png' . ' />'), "admin/mobilizer/select-menu-configure/page/$mlid/share", $options = array(
    'html' => TRUE,
    'attributes' => array(
      'title' => 'Share'
    )
  )) . t('Share') . '</div>';
  
  $options_addpage = array(
    'attributes' => array(
      'class' => 'btn btn-primary add-content cancel topten'
    ),
    'html' => TRUE
  );
  $cancel = l(t('Cancel'), "admin/mobilizer/select-menu-configure/page/$mlid/edit", $options_addpage);
  $output .= '</div>' . $cancel;
  return _mobilizer_themer($output, _mobilizer_iframe_with_html($base_url));
}

/**
 * Page callback: add share content
 */
function mobilizer_menu_add_content_share(){
  global $base_url;
  $render_array = drupal_get_form("mobilizer_content_share_form");
  $output = drupal_render($render_array);
  return _mobilizer_themer($output, _mobilizer_iframe_with_html($base_url));
}

/**
 * Function to build share content add/edit form.
 */
function mobilizer_content_share_form(){
  $share_txt = '';
  $share_text_desc = '';
  $button_txt = "Proceed";
  
  if (is_numeric(arg(4)) && arg(5) == "edit" && is_numeric(arg(6))){
    $blockobj = db_select('mobilizer_content_item', 'mct')
    ->fields('mct', array('page_id', 'content_data', 'item_type'))
    ->condition('page_id', arg(4))
    ->condition('block_id', arg(6))
    ->execute()->fetchObject();

    $share = unserialize($blockobj->content_data);
    $share_txt = $share['share_desc'];
    $button_txt = "Update";
  }

  $form['share_desc'] = array(
    '#type' => 'textfield',
    '#title' => t('Text Description'),
    '#default_value' => $share_txt,
    '#description' => t("Leave blank to use 'Share'"),
  );
  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t($button_txt),
    '#attributes' => array('class' => array('btn btn-primary')),
  );
  if(arg(5) == 'edit'){
    $cancel_redirect = 'admin/mobilizer/select-menu-configure/page/'.arg(4).'/edit';
  }
  else{
    $cancel_redirect = 'admin/mobilizer/select-menu-configure/page/'.arg(4).'/add/content';  
  }
   $form ['cancel'] = array(
    '#markup' => l(t('Cancel'), $cancel_redirect, array(
      'attributes' => array(
        'class' => array('btn btn-primary','cancel')
      )
    )),
  );
  return $form;
}

/**
 * Implements share content form validation.
 */
function mobilizer_content_share_form_validate($form, &$form_state){
  $share_desc = $form_state['values']['share_desc'];
  if ($share_desc == ''){
    $form_state['values']['share_desc'] = 'Share';
  }
}

/**
 * Implements share content form submition.
 */
function mobilizer_content_share_form_submit($form, &$form_state){
  global $base_url;
  $mlid = arg(4);
  $share['share_desc'] = $form_state['values']['share_desc'];
  $share_page = menu_link_load($mlid);
  if ($share_page['link_path'] != '<front>')
    $share['share_url'] = $base_url . '/' . $share_page['link_path'];
  else 
    $share['share_url'] = $base_url;
  
  if(is_numeric(arg(4)) && arg(5) == "edit" && is_numeric(arg(6))){ //@condition to update video block
    mobilizer_page_block_save( arg(4), "mobilizer_share", $share , arg(6));
  }else{
    mobilizer_page_block_save( arg(4), "mobilizer_share", $share , $block_id = "");
  }
  mobilizer_set_messages('Changes Updated successfully');
  drupal_goto('admin/mobilizer/select-menu-configure/page/'.arg(4).'/edit');
}

/**
 * Page callback: add video content.
 */
function mobilizer_page_add_content_type_video(){
  global $base_url;
  $render_array = drupal_get_form("mobilizer_page_block_video_form");
  $output = drupal_render($render_array);
  return _mobilizer_themer($output, _mobilizer_iframe_with_html($base_url));
}

/**
 * Function to build video content add/edit form.
 */
function mobilizer_page_block_video_form($form, &$form_state){
  $video_url = "";
  $video_description = "";
  $show_controls = 1;
  $autoplay = 1;
  $loop = 1;
  $button_txt = "Proceed";

  if (is_numeric(arg(4)) && arg(5) == "edit" && is_numeric(arg(6))){
    $blockobj = db_select('mobilizer_content_item', 'mct')
    ->fields('mct', array('page_id', 'content_data', 'item_type'))
    ->condition('page_id', arg(4))
    ->condition('block_id', arg(6))
    ->execute()->fetchObject();

    $video = unserialize($blockobj->content_data);
    $video_url = $video['video_url'];
    $show_controls = $video['show_controls'];
    $autoplay = $video['autoplay'];
    $loop = $video['loop'];
    //$video_description = $video['video_description'];
    $button_txt = "Update";
  }

  $form['video_url'] = array(
    '#type' => 'textfield',
    '#title' => t('Video URL'),
    '#maxlength' => 250,
    '#default_value' => $video_url,
    '#description' => t("www.example.com/path/to/video/file <br/> Allowed extension ogv, webm"),
    '#required' => TRUE
  );
  $form['video_options'] = array(
    '#type' => 'fieldset',
    '#title' => t('Display Options'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE
  );
  $form['video_options']['show_controls'] = array(
    '#type' => 'radios',
    '#title' => t('Show video controls'),
    '#options' => array(1 => t('Yes'), 0 => t('No')),
    '#default_value' => $show_controls,
  );
  $form['video_options']['autoplay'] = array(
    '#type' => 'radios',
    '#title' => t('Autoplay video '),
    '#options' => array(1 => t('Yes'), 0 => t('No')),
    '#default_value' => $autoplay,
  );
  $form['video_options']['loop'] = array(
    '#type' => 'radios',
    '#title' => t('Loop video '),
    '#options' => array(1 => t('Yes'), 0 => t('No')),
    '#default_value' => $loop,
  );
  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t($button_txt),
    '#attributes' => array('class' => array('btn btn-primary')),
    
  );
  if(arg(5) == 'edit'){
    $cancel_redirect = 'admin/mobilizer/select-menu-configure/page/'.arg(4).'/edit';
  }
  else{
    $cancel_redirect = 'admin/mobilizer/select-menu-configure/page/'.arg(4).'/add/content';  
  }
  $form ['cancel'] = array(
    '#markup' => l(t('Cancel'), $cancel_redirect, array(
      'attributes' => array(
        'class' => array('btn btn-primary','cancel')
      )
    )),
  );
  return $form;
}

/**
 * Implements video content form validation.
 */
function mobilizer_page_block_video_form_validate($form, &$form_state){
  $video_url = $form_state['values']['video_url'];
  $valid_video_extensions = array('ogv', 'webm');
  $video_extension = '';
  if ($video_url != ''){
    $path_parts = pathinfo($video_url);
    $video_extension = strtolower($path_parts['extension']);
  }
  if (!in_array($video_extension, $valid_video_extensions)){
    form_set_error('video_url', t('Enter valid video file.'));
    mobilizer_set_messages('Enter valid video file.', 'error');
  }
}

/**
 * Implements form submit.
 */
function mobilizer_page_block_video_form_submit($form, &$form_state) {
  $video['video_url'] = $form_state['values']['video_url'];
  $video['show_controls'] = $form_state['values']['show_controls'];
  $video['autoplay'] = $form_state['values']['autoplay'];
  $video['loop'] = $form_state['values']['loop'];
  
  if(is_numeric(arg(4)) && arg(5) == "edit" && is_numeric(arg(6))){ //@condition to update video block
    mobilizer_page_block_save( arg(4), "mobilizer_video", $video , arg(6));
  }else{
    mobilizer_page_block_save( arg(4), "mobilizer_video", $video , $block_id = "");
  }
  mobilizer_set_messages('Changes Updated successfully');
  drupal_goto('admin/mobilizer/select-menu-configure/page/'.arg(4).'/edit');
}

/**
 * @param unknown $page_id
 * @param unknown $item_type
 * @param unknown $data_arr
 * @param string $block_id @if block id , update the block data
 */
function mobilizer_page_block_save( $page_id, $item_type, $data_arr , $block_id = ""){
  $html_for_block = "";
  if($item_type == "mobilizer_video"){
    $html_for_block = mobilizer_video_block_html($data_arr);
    $path_part = pathinfo($data_arr['video_url']);
    $block_description = $path_part['basename'];
  }elseif ( $item_type == 'mobilizer_share'){
  	$html_for_block = mobilizer_share_block_html($data_arr);
  	$block_description = $data_arr['share_desc'];
  }

  if($block_id == ""){
    $status_id = db_insert('mobilizer_node')
    ->fields(array(
      'mlid' => $page_id,
      'bid' => 0,
      'weight' => 0,
      'ebabled' => 1,
      'block_description' => $block_description,
      'block_title' => $block_description,
      'block_delta' => '',
      'block_module' =>'',
      'node_block_html' =>$html_for_block,
      'type' => $item_type,
      'time' => REQUEST_TIME,))->execute();
 
    $content_item_id = db_insert('mobilizer_content_item')
    ->fields(array(
      'page_id' => $page_id,
      'item_type' => $item_type,
      'block_id' => $status_id,
      'content_data' => serialize($data_arr),
    ))->execute();
  }else{
    $status_id =  db_update('mobilizer_node')
    ->fields(array(
      'block_description' => $block_description,
      'node_block_html' => $html_for_block,
      'time' => REQUEST_TIME,
    ))
    ->condition('mid', $block_id)
    ->condition('mlid', $page_id)
    ->execute();

    $content_item_id = db_update('mobilizer_content_item')
    ->fields(array(
      'item_type' => $item_type,
      'content_data' => serialize($data_arr),
    ))->condition('block_id', $block_id)
    ->condition('page_id', $page_id)->execute();
  }
}

/**
 * @param array $attributes video control attributes
 * @return string
 */
function mobilizer_video_block_html($attributes) {
  $video_url = $attributes['video_url'];
  $show_controls = $attributes['show_controls'];
  $autoplay = $attributes['autoplay'];
  $loop = $attributes['loop'];
  
  $video_html = "<video height='115px' width='222px' src='$video_url' ";
  if ($show_controls){
  	$video_html .= "controls='controls' ";
  }
  if ($autoplay) {
    $video_html .= "autoplay='autoplay' ";
  }
  if ($loop) {
    $video_html .= "loop='loop' ";
  }
  $video_html .= ">Your browser does not support the <code>video</code> element.</video>";
  return $video_html;
}

/**
 * @param array $attributes
 * @return string
 */
function mobilizer_share_block_html($data_arr){
  $share_desc = $data_arr['share_desc'];
  $share_url = $data_arr['share_url'];
  
  /*$share_html = l(t($share_desc), $share_url, $options = array(
    'html' => TRUE,
    'attributes' => array(
      'title' => $share_desc
    ))
  );*/
  $share_html = "<div class='mobilizer-share' wr='true' icon='true' surround='true' facebook='true' twitter='true' google_plus='true' email='true' linked_in='true' text='I wish to share this great mobile website with you' share_popup_title='Share by:'>
    <span class='mobilizer-share-iconBg'>
      <span class='share-icon'></span> 
    </span> 
    <span class='text' onclick='javascript:mobilizer_show_popup(this);'>$share_desc</span>
    <div class='popup-data-wrapper' id='mobilizer-popup-data-wrapper' style='display:none'>
      <div class='popup-data'>
        <div class='share-close-wrapper'><div class='share-popup-close' onclick='javascript:share_popup_close(this);'>Close</div></div>
        <div class='fb-div mobilizer-share-widget'>"
          . l(t('Facebook'), "http://m.facebook.com/sharer.php?u=$share_url", array('html' => true, 'attributes' => array('target' => '_blank', 'class' => 'social-icons'))) . " 
        </div>
        <div class='twitter-div mobilizer-share-widget'>" 
          . l(t('Twitter'), "http://twitter.com/intent/tweet?text=I wished to share this great mobile website with you; url=$share_url", array('html' => true, 'attributes' => array('target' => '_blank' , 'class' => 'social-icons'))) . "
        </div> 
        <div id='mobilizer-gplus-share' class='gplus-div mobilizer-share-widget'>"
         . l(t('Google +'), "https://plus.google.com/share?url=$share_url", array('html' => true, 'attributes' => array('target' => '_blank', 'class' => 'social-icons'))) . "
        </div> 
        <div class='mobilizer-linkedin-div mobilizer-share-widget'>"
         . l(t('LinkedIn'), "http://www.linkedin.com/shareArticle?mini=true&url=$share_url&title=I wanted to share this great mobile website with you", array('html' => true, 'attributes' => array('target' => '_blank', 'class' => 'social-icons'))) . "
        </div>
      </div>
    </div>
  </div>";
  return $share_html;
}

/**
 * @return Ambigous <An, string>
 */
function mobilizer_menu_edit_content_type_video(){
  global $base_url;
  $render_array = drupal_get_form("mobilizer_page_block_video_form");
  $output = drupal_render($render_array);
  return _mobilizer_themer($output, _mobilizer_iframe_with_html($base_url));
}

/**
 * @return Ambigous <An, string>
 */
function mobilizer_menu_edit_content_type_share(){
  global $base_url;
  $render_array = drupal_get_form("mobilizer_content_share_form");
  $output = drupal_render($render_array);
  return _mobilizer_themer($output, _mobilizer_iframe_with_html($base_url));
}
?>

<?php

/**
 * @file
 * Web block and page content list into mobilizer.
 */

/**
 * Page callback: block and page content list.
 */
function mobilizer_menu_block_list() {
  global $base_url;
  $render_array = drupal_get_form("mobilizer_menu_block_list_form");
  $output = drupal_render($render_array);
  return _mobilizer_themer($output, _mobilizer_iframe_with_html($base_url));
}

/**
 * Page callback: block and page content list.
 */
function mobilizer_menu_block_list_form($form, &$form_state) {
  $mlid = arg(4);
  global $base_url;
  $blockobj = db_select('mobilizer_node', 'nm')
      ->fields('nm', array('mid', 'mlid', 'bid', 'block_description', 'block_title', 'weight', 'ebabled', 'type'))
      ->condition('mlid', $mlid)->orderBy('weight')
      ->execute()->fetchAll();
  $rows = array();
  if (count($blockobj)) {
    foreach ($blockobj as $block) {
      $form_type = array('form_block', 'form_page');
      if (in_array($block->type, $form_type)) {
        $block_desc = t($block->block_description . '[FROM]');
        $edit = '';
      }
      else {
        $block_desc = t($block->block_description);
        if($block->type == "mobilizer_email_us"){
        	$edit = l(t('Edit'), 'admin/mobilizer/select-menu-configure/page/' . $mlid . '/edit/' . $block->mid . '/email', array('attributes' => array('class' => 'edit')));
        }else if($block->type == "mobilizer_image_item"){
        	$edit = l(t('Edit'), 'admin/mobilizer/select-menu-configure/page/' . $mlid . '/edit/' . $block->mid . '/image', array('attributes' => array('class' => 'edit')));
        }else if($block->type == "mobilizer_slider_item"){
        	$edit = l(t('Edit'), 'admin/mobilizer/select-menu-configure/page/' . $mlid . '/edit/' . $block->mid . '/image-slider', array('attributes' => array('class' => 'edit')));
        }else if($block->type == "mobilizer_social"){
        	$edit = l(t('Edit'), 'admin/mobilizer/select-menu-configure/page/' . $mlid . '/edit/' . $block->mid . '/social-icons', array('attributes' => array('class' => 'edit')));
	    }elseif ($block->type == 'mobilizer_video'){
          $edit = l(t('Edit'), 'admin/mobilizer/select-menu-configure/page/' . $mlid . '/edit/' . $block->mid . '/video', array('attributes' => array('class' => 'edit')));
        }elseif ($block->type == 'mobilizer_share'){
          $edit = l(t('Edit'), 'admin/mobilizer/select-menu-configure/page/' . $mlid . '/edit/' . $block->mid . '/share', array('attributes' => array('class' => 'edit')));
        }else{
        	$edit = l(t('Edit'), 'admin/mobilizer/select-menu-configure/page/' . $mlid . '/edit/' . $block->mid . '/blockedit', array('attributes' => array('class' => 'edit')));
        }
       }

      $rows[$block->mid] = array(
        'description' => $block_desc,
        'action' => $edit,
        'ebabled' => $block->ebabled,
        'weight' => $block->weight,
      );
    }
  }
  else {
    $form['#empty_text'] = t('There are no block under this menu links yet.');
  }
  $form['#prefix'] = '<div class="menu_confi">';
  $form['#suffix'] = '</div>';
  $form['rearrange'] = array(
    '#prefix' => '<div id="red-dwarf">',
    '#suffix' => '</div>',
    '#tree' => TRUE,
    '#theme' => 'mobilizer_form_menu_block_list_form'
  );

  foreach ($rows as $key => $dataobj) {
    $form['rearrange'][$key]['description'] = array(
      '#markup' => $dataobj['description'],
    );
    $form['rearrange'][$key]['action'] = array(
      '#markup' => $dataobj['action'],
    );
    $form['rearrange'][$key]['ebabled'] = array(
      '#type' => 'checkbox',
      '#default_value' => $dataobj['ebabled'],
    );
    $form['rearrange'][$key]['weight'] = array(
      '#type' => 'textfield',
      '#default_value' => $dataobj['weight'],
      '#size' => 3,
      '#attributes' => array('class' => array('rank-weight')), // needed for table dragging
    );
  }
  
  $options_addpage = array(
    'attributes' => array(
      'class' => 'btn btn-primary add-content'
    ),
    'html' => TRUE
  );
  
  $add_content = l(t('Add New Content'), "admin/mobilizer/select-menu-configure/page/$mlid/add/content", $options_addpage);
  
  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Proceed'),
    '#attributes' => array('class' => array('btn btn-primary right')),
    '#prefix' => $add_content,    
  );
  return $form;
}

/**
 * theme_form  hendler for menu_block_list_form.
 */
function theme_mobilizer_form_menu_block_list_form($vars) {
  $element = $vars['element'];
  drupal_add_tabledrag('menu_block_list_form', 'order', 'sibling', 'rank-weight'); // needed for table dragging
  $header = array(
    'description' => t('BLOCKS LIST'),
    'ebabled' => t('ENABLED'),
    'action' => t('EDIT'),
    'weight' => t('WEIGHT'),
  );

  $rows = array();
  foreach (element_children($element) as $key) {
    $row = array();

    $row['data'] = array();

    foreach ($header as $fieldname => $title) {
      $row['data'][] = drupal_render($element[$key][$fieldname]);
      $row['class'] = array('draggable'); // needed for table dragging
    }
    $rows[] = $row;
  }
  if (!count($rows)) {
    $header = array('description' => t('No PAGE'));
  }
  return theme('table', array(
      'header' => $header,
      'rows' => $rows,
      'attributes' => array('id' => 'menu_block_list_form'), // needed for table dragging
    ));
}

/**
 * Submit handler for the block list.
 */
function mobilizer_menu_block_list_form_submit($form, &$form_state) {

  if ($form_state['values']['op'] == 'Proceed') {
    if (isset($form_state['values']['rearrange'])) {
      foreach ($form_state['values']['rearrange'] as $mid => $obj) {
        $update = db_update('mobilizer_node')
            ->fields(array(
              'ebabled' => $obj['ebabled'],
              'weight' => $obj['weight'],
            ))->condition('mid', $mid)->execute();
      }
      mobilizer_set_messages('Update successfully');
    }
    drupal_goto('admin/mobilizer/featured-pages');
  }
  else {
    mobilizer_set_messages('Invalid opration', 'error');
  }
}

/**
 * @param $nid
 * $nid id contain menu mlid.
 * Page callback handler for delete block listmenu
 * Delete menu and it's under all blocks 
 */
function mobilizer_menu_block_list_delete_page($nid = NULL) {
  global $base_url;
  $render_array = drupal_get_form("mobilizer_menu_block_list_delete_form", $nid);
  $output = drupal_render($render_array);
  return _mobilizer_themer($output, _mobilizer_iframe_with_html($base_url));
}

/**
 * 
 * @param unknown $form
 * @param unknown $form_state
 * @param string $nid
 * @return multitype:string
 */
function mobilizer_menu_block_list_delete_form($form, &$form_state, $nid = NULL) {
  $form['confirm_message'] = array(
    '#markup' => t('Are you sure you want to delete this menu page?'),
  );
  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Confirm'),
    '#submit' => array('menu_block_list_delete_form_submit')
  );
  $form['cancel'] = array(
    '#markup' => l(t('Cancel'), 'admin/mobilizer/featured-pages', array('attributes' => array('class' => 'btn btn-primary'))),
  );
  $form['mlid'] = array(
    '#type' => 'hidden',
    '#value' => $nid,
  );
  return $form;
}

/**
 * Submit hadnler for menu delete block list
 */
function mobilizer_menu_block_list_delete_form_submit($form, &$form_state) {
  $mlid = $form_state['values']['mlid'];
  menu_link_delete($mlid, $path = NULL);
  db_delete('mobilizer_node')
    ->condition('mlid', $mlid)
    ->execute();
  drupal_goto('admin/mobilizer/featured-pages');
}


/**
 * @function : for add email us form implementation
 * @return Ambigous <An, string>
 */
function mobilizer_menu_add_content_type_email(){
  global $base_url;
  $render_array = drupal_get_form("mobilizer_menu_block_email_form");
  $output = drupal_render($render_array);
  return _mobilizer_themer($output, _mobilizer_iframe_with_html($base_url));
}

/**
 * @function : Update the email us data
 * @return Ambigous <An, string>
 */
function mobilizer_menu_edit_content_type_email(){
  global $base_url;
  $render_array = drupal_get_form("mobilizer_menu_block_email_form");
  $output = drupal_render($render_array);
  return _mobilizer_themer($output, _mobilizer_iframe_with_html($base_url));
}


/**
 * @function : email us form
 * Page callback: block and email content list.
 */
function mobilizer_menu_block_email_form($form, &$form_state) {
  $email_address = "";
  $email_description = "Email Us";
  $button_txt = "Proceed";
  $form['#attributes'] = array('class' => array('add-page-block'));
  //@condition : for update the email us details
  if(is_numeric(arg(4)) && arg(5) == "edit" && is_numeric(arg(6))){		
	$blockobj = db_select('mobilizer_content_item', 'mct')
	->fields('mct', array('page_id', 'content_data', 'item_type'))
	->condition('page_id', arg(4))
	->condition('block_id', arg(6))
	->execute()->fetchObject();
	
	$email = unserialize($blockobj->content_data);
	$email_address = $email['email_address'];
	$email_description = $email['email_description'];
	$button_txt = "Update";
  }

  $form ['email_address'] = array(
    '#type' => 'textfield',
    '#title' => t('Email Address'),
    '#default_value' => $email_address,
    '#required' => 1,
  );
	
  $form ['email_description'] = array(
	'#type' => 'textfield',
	'#title' => t('Text Description'),
	'#default_value' => $email_description,
	'#required' => 1,
  );

  $form['submit'] = array(
	'#type' => 'submit',
	'#value' => t($button_txt),
	'#attributes' => array('class' => array('btn btn-primary')),
  );
   $form ['cancel'] = array(
    '#markup' => l(t('Cancel'), 'admin/mobilizer/select-menu-configure/page/'.arg(4).'/add/content', array(
      'attributes' => array(
        'class' => array('btn btn-primary','cancel')
      )
    )),
  );
  
  return $form;
}

/**
 * @function : validation
 * @param unknown $form
 * @param unknown $form_state
 */
function mobilizer_menu_block_email_form_validate($form, &$form_state) {
  if (!valid_email_address($form_state['values']['email_address'])) {
	form_set_error('email_address', t('The email address appears to be invalid.'));
  }
}	


/**
 * @function: submit action
 * @param unknown $form
 * @param unknown $form_state
 */
function mobilizer_menu_block_email_form_submit($form, &$form_state) {	
  $email['email_address'] = $form_state['values']['email_address'];
  $email['email_description'] = $form_state['values']['email_description'];	
  
  if(is_numeric(arg(4)) && arg(5) == "edit" && is_numeric(arg(6))){ //@condition for update email as per block
  	mobilizer_save_block_values( arg(4), "mobilizer_email_us", $email , arg(6));
  }else{
  	mobilizer_save_block_values( arg(4), "mobilizer_email_us", $email , $block_id = "");	
  }	
  mobilizer_set_messages('Changes Update successfully');
  drupal_goto('admin/mobilizer/select-menu-configure/page/'.arg(4).'/edit');
}

/**
 * @function: email us html genration for saving on block.
 * @param unknown $email
 * @param unknown $content
 * @return string
 */
function mobilizer_email_us_html($email,$content){	
  $html = '<div class="email-us-main-cs"><a email="'.$email.'" description="'.$content.'" 
  href="mailto:'.$email.'" 
  class="mobilizer-email-us-cs"> 
  <span class="mobilizer-email-us-desc-cs">'.$content.'</span>
  </a></div>';	
  
  return $html;
}


/**
 * @function: callback for image content
 * @for image content item
 */
function mobilizer_menu_add_content_type_image(){	
	global $base_url;
	$render_ary = drupal_get_form("mobilizer_menu_add_image_form");
	$header_page = drupal_render($render_ary);
	return _mobilizer_themer($header_page, _mobilizer_iframe_with_html($base_url));
}

/**
 * @function: image form creation
 * @param unknown $form
 * @param unknown $form_state
 * @return multitype:string number multitype:multitype:string   Ambigous <The, string, A, Optional>
 */
function mobilizer_menu_add_image_form($form, &$form_state) {
  global $base_url;	
  drupal_add_library('system', 'ui.widget');
  drupal_add_library('system', 'ui.mouse');
  drupal_add_library('system', 'ui.slider'); 
  $button_txt = "Proceed";
  $upload_image = "";
  $resize = "100";
  //@condition: update the image data.  
  if(is_numeric(arg(4)) && arg(5) == "edit" && is_numeric(arg(6))){
  	$blockobj = db_select('mobilizer_content_item', 'mct')
  	->fields('mct', array('page_id', 'content_data', 'item_type'))
  	->condition('item_type', "mobilizer_image_item")
  	->condition('page_id', arg(4))
  	->condition('block_id', arg(6))
  	->execute()->fetchObject();
    
  	$image = unserialize($blockobj->content_data);
  	$upload_image = $image['upload_image'];
  	$resize_arr = explode("%",$image['resize']);
  	$resize = $resize_arr[0];
  	$button_txt = "Update";
  }
 
  drupal_add_js('
    jQuery(document).ready(function ($) { 
      $(function() {
        $("#slider").slider({
        value:' . $resize . ',
        min: 10,
        max: 100,
        animate: true,
        step: 1,
        slide: function( event, ui ) {
                $( "#amount" ).val(ui.value + "%");
                }
       });
       $( "#amount" ).val($( "#slider" ).slider( "value" ) + "%" );
      });
    });', 'inline');

  $form['#attributes']['enctype'] = "multipart/form-data";
  
  $form['upload_image'] = array(
    
    '#type' => 'managed_file',
    '#title' => 'Upload Image',
    '#size' => 10,
    '#default_value' => $upload_image,
    '#upload_location' => 'public://mobilizer_images/',
    '#theme' => 'preview_image_thumbnail',
    '#required' => 1,
  	'#description' => t('Attach your *.(png, gif, jpg, jpeg) file here to import.'),
  	'#upload_validators' => array(
  		'file_validate_extensions' => array('png gif jpg jpeg'),
  	),
    
  );

  $form['resize_title'] = array(
   
    '#markup' => '<label class = "resize">Resize</label>',
  );
  
  $form['resize'] = array(
    '#type' => 'textfield',
    '#title' => t('Width'),
    '#id' => 'amount',
    '#size' => 5,
    '#default_value' => $resize.'%',
  );

  $form['slider'] = array(
    '#markup' => '<br/><div id="slider"></div><div class="clear"></div>',
  );

  $form['proceed'] = array(
    '#type' => 'submit',
    '#value' => t($button_txt),
    '#attributes' => array('class' => array('btn btn-primary')),
    
  );

  $form ['cancel'] = array(
    '#markup' => l(t('Cancel'), 'admin/mobilizer/select-menu-configure/page/'.arg(4).'/add/content', array(
      'attributes' => array(
        'class' => array('btn btn-primary','cancel')
      )
    )),
  );

  return $form;	
}

/**
 * @function: submit action
 * @param unknown $form
 * @param unknown $form_state
 */
function mobilizer_menu_add_image_form_submit($form, &$form_state) {				
		$image['upload_image'] = $form_state['values']['upload_image'];
		$image['resize'] = $form_state['values']['resize'];	
		if(is_numeric(arg(4)) && arg(5) == "edit" && is_numeric(arg(6))){
			mobilizer_save_block_values( arg(4), "mobilizer_image_item", $image , arg(6));
		}else{
			mobilizer_save_block_values( arg(4), "mobilizer_image_item", $image , $block_id = "");
		}	
		
		mobilizer_set_messages('Changes Update successfully');
		drupal_goto('admin/mobilizer/select-menu-configure/page/'.arg(4).'/edit');
}


/**
 * @function: common method for saving the data in block table
 * @param unknown $page_id
 * @param unknown $item_type
 * @param unknown $data_arr
 * @param string $block_id @if block id , update the block data
 */
function mobilizer_save_block_values( $page_id, $item_type, $data_arr , $block_id = ""){
  $html_for_block = "";
  if($item_type == "mobilizer_email_us"){
    $html_for_block = mobilizer_email_us_html($data_arr['email_address'],$data_arr['email_description']);	
	$block_description = $data_arr['email_description'];
  }else if($item_type == "mobilizer_image_item"){  	
  	$html_for_block = mobilizer_image_item_html($data_arr['upload_image'],$data_arr['resize']);
  	$block_description = "Image Block";
  }else if($item_type == "mobilizer_slider_item"){
  	$html_for_block = mobilizer_slider_item_html($data_arr,$page_id,$block_id);
  	$block_description = "Slider Block";
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
 * @function: image html for block
 * @param unknown $fid
 * @param unknown $width_size
 * @return string
 */
function mobilizer_image_item_html($fid,$width_size){	
	$file_obj = file_load($fid);
	$imgpath = isset($file_obj->uri) ? $file_obj->uri : NULL;
	$image_path = file_create_url($imgpath);	
	$html = '<div class="image-block-main-cs"><img src="'.$image_path.'" width ="'.$width_size.'"/></div>';
	return $html;
}

/**
 * @function: slider html for block
 * @param unknown $data_arr
 * @param unknown $page_id
 * @param unknown $block_id
 * @return string
 */
function mobilizer_slider_item_html($data_arr,$page_id,$block_id){
  $res_total = db_query('SELECT count(id) as total
FROM {mobilizer_content_item} mci WHERE mci.item_type = :item_type and mci.page_id =:page_id',
	array(':item_type' => "mobilizer_slider_item",':page_id' => $page_id))->fetchField();
	
  if($block_id != ""){
	$res_data = $page_id.'_'.$block_id;
  }else{
	$res_data = $page_id.'_'.$res_total;
  }
	
  $html_data = '<script type="text/javascript">
	jQuery(document).ready(function(){	
  		jQuery(".slider_'.$res_data.'").bxSlider();
	});	
  </script>
  <div id="image-slider">	
  <ul class="slider_'.$res_data.'">';
  if(count($data_arr) > 0){
	  foreach($data_arr as $slider_key => $slider_value){
		$html_data .='<li><img src="'.$slider_value['url'].'"></li>';
	  }	
    $html_data .= '</ul></div>';
  }else{
  	$html_data = "<div class='no-image-cs'>No image Available.</div>";
  }
  return $html_data;
}

/**
 * @function: slider callback
 * @return Ambigous <An, string>
 */
function mobilizer_menu_add_content_type_image_slider(){
	global $base_url;
	$render_ary = drupal_get_form("mobilizer_menu_add_image_slider_form");
	$header_page = drupal_render($render_ary);
	return _mobilizer_themer($header_page, _mobilizer_iframe_with_html($base_url));
}

/**
 * @function: slider form
 * @param unknown $form
 * @param unknown $form_state
 * @return multitype:string number multitype:multitype:string   Ambigous <The, string, A, Optional>
 */
function mobilizer_menu_add_image_slider_form ($form, &$form_state) {	
  $button_txt = "Upload & Submit";
  $upload_image = "";
  drupal_add_js(drupal_get_path("module","mobilizer")."/js/mobilizer-slider.js");
  
  $mobilizer_slider_images_arr = NULL;
  if(is_numeric(arg(4)) && arg(5) == "edit" && is_numeric(arg(6))){
	$blockobj = db_select('mobilizer_content_item', 'mct')
	->fields('mct', array('page_id', 'content_data', 'item_type'))
	->condition('page_id', arg(4))
	->condition('block_id', arg(6))
	->execute()->fetchObject();				
	$mobilizer_slider_images_arr = unserialize($blockobj->content_data);
  }
  $form['#attributes']['enctype'] = "multipart/form-data";
  $form ['slider_container'] = array(
  		'#type' => 'fieldset',
  		'#title' => t('Slider Images'),
  		'#weight' => 5,
  		'#collapsible' => FALSE,
  		'#collapsed' => FALSE,
  		'#prefix' => '<div class="slider_container_main_cs add-page-block">',
  		'#suffix' => '</div>'
  );
  
  $header = $options = array();
  
  if($mobilizer_slider_images_arr != NULL){
  	$header = array(
  			'image' => t('Image'),
  	);
  	
    foreach ($mobilizer_slider_images_arr as $slider_key => $slider_value) {  
  	  $options[$slider_value['fid']] = array(
  			'image' => t('<div class="slider-image-border-cs"><img src = "'.$slider_value['url'].'"  width ="80px" height="80px"/></div>'),
  	 );
    }
    
  }
  
  $form ['slider_container'] ['image_slider_list'] = array(
  		'#type' => 'tableselect',
  		'#header' => $header,
  		'#options' => $options,
  		'#empty' => t('<div class="main-blank-message-cs">No Image selected.</div>'),
  );
  
  $form ['slider_container'] ['pager'] = array('#markup' => theme('pager'));

  $form ['slider_container'] ['delete'] = array(
  		'#type' => 'submit',
  		'#value' => t('Delete'),
  		'#id' => 'delete_images',
  		'#attributes' => array('class' => array('btn btn-primary right') ,'style' => array('display:none')),
  );
  

  $form ['slider_upload_container'] = array(
  		'#type' => 'fieldset',
  		'#title' => t('Upload Images'),
  		'#weight' => 5,
  		'#collapsible' => FALSE,
  		'#collapsed' => FALSE,
  		'#prefix' => '<div class="slider_upload_container_main_cs add-page-block">',
  		'#suffix' => '</div>'
  );
  
  $form ['slider_upload_container'] ['file'] = array(
	'#type' => 'file',
	'#name' => 'files[]',
	'#title' => t('Upload some Images'),		
	'#description' => t('JPG\'s, GIF\'s, and PNG\'s only, 2MB Max Size'),
	'#attributes' => array('multiple' => 'multiple'),
  );
	
  $form ['slider_upload_container'] ['proceed'] = array(
	'#type' => 'submit',
	'#value' => t($button_txt),
	'#attributes' => array('class' => array('btn btn-primary')),
  );
  
  $form ['slider_upload_container'] ['cancel'] = array(
  		'#markup' => l(t('Cancel'), 'admin/mobilizer/select-menu-configure/page/'.arg(4).'/add/content', array(
  				'attributes' => array(
  						'class' => array('btn btn-primary','cancel')
  				)
  		)),
  );
  return $form;
}

/**
 * @function: slider validation
 * @param unknown $form
 * @param unknown $form_state
 */
function mobilizer_menu_add_image_slider_form_validate($form, &$form_state) {
	//Save multiple files
  if($form_state['values']['op'] != "Delete"){	
	  $num_files = count($_FILES['files']['name']);
	  if(!file_exists("public://mobilizer_slider_images")){
	    drupal_mkdir("public://mobilizer_slider_images");
	  }  
	  for ($i = 0; $i < $num_files; $i++) {
		$file = file_save_upload($i, array(
				'file_validate_is_image' => array(),
				'file_validate_extensions' => array('png gif jpg jpeg'),
		),'public://mobilizer_slider_images');
		
		
		if ($file) {
		  if ($file = file_move($file, 'public://mobilizer_slider_images')) {
			$form_state['values']['file'][$i] = $file;
		  }
		  else {
			form_set_error('file', t('Failed to write the uploaded file the site\'s file folder.'));
		  }
		}
		else {
		  form_set_error('file', t('No file was uploaded.'));
		}
	  }
  }else{
  	//print_r($form_state); die;
  }	  
}

/**
 * @function: slider submit action
 * @param unknown $form
 * @param unknown $form_state
 */
function mobilizer_menu_add_image_slider_form_submit($form, &$form_state) {
    $mobilizer_slider_images_arr = NULL;
	if(is_numeric(arg(4)) && arg(5) == "edit" && is_numeric(arg(6))){
		$blockobj = db_select('mobilizer_content_item', 'mct')
		->fields('mct', array('page_id', 'content_data', 'item_type'))
		->condition('page_id', arg(4))
		->condition('block_id', arg(6))
		->execute()->fetchObject();				
		$mobilizer_slider_images_arr = unserialize($blockobj->content_data);
	}
	
  if (is_array($form_state['values']['file'])) {
	//Make sure we keep our previously uploaded images
	foreach ($form_state['values']['file'] as $file) {
	  $new_file = file_load($file->fid);
	  $image_path = file_create_url($file->uri);
	  $mobilizer_slider_images_arr[] = array("fid" => $file->fid, "url" =>  $image_path);
	}
	
	if(is_numeric(arg(4)) && arg(5) == "edit" && is_numeric(arg(6))){
	  mobilizer_save_block_values( arg(4), "mobilizer_slider_item", $mobilizer_slider_images_arr , arg(6));
	}else{
	  mobilizer_save_block_values( arg(4), "mobilizer_slider_item", $mobilizer_slider_images_arr , $block_id = "");
	}	
  }else if($form_state['values']['op'] == "Delete"){  	
  	$new_mobilizer_arr = get_new_mobilizer_diff($form_state['values']['image_slider_list'],$mobilizer_slider_images_arr);
  	if(is_numeric(arg(4)) && arg(5) == "edit" && is_numeric(arg(6))){
  		mobilizer_save_block_values( arg(4), "mobilizer_slider_item", $new_mobilizer_arr , arg(6));
  	}
  	mobilizer_set_messages('Images Deleted successfully');
  	drupal_goto('admin/mobilizer/select-menu-configure/page/'.arg(4).'/edit/'.arg(6).'/image-slider');
  }
  mobilizer_set_messages('Images Uploaded successfully');
  drupal_goto('admin/mobilizer/select-menu-configure/page/'.arg(4).'/edit');
}

/**
 * @function: for genrate new array after delete image
 * @param unknown $deleted_arr
 * @param unknown $mobilizer_arr
 * @return multitype:multitype:unknown
 */
function get_new_mobilizer_diff($deleted_arr ,  $mobilizer_arr){
  $new_mobilizer_arr = array();
  foreach($mobilizer_arr as $key => $values){
	if($deleted_arr[$values['fid']] != $values['fid']){
		$new_mobilizer_arr[] = array('fid' => $values['fid'],'url' => $values['url']);
	}		 
  }	
  return $new_mobilizer_arr; 	
}


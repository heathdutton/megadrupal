<?php

/**
 * @file
 * The theme system, which controls the output of mobile site.
 *
 * The theme system allows for the major items of the site.
 */
function mobilizer_add_social_icons() {
  global $base_url;
  $render_array = drupal_get_form("mobilizer_add_social_icons_block");
  $html_color = drupal_render($render_array);
  return _mobilizer_themer($html_color, _mobilizer_iframe_with_html($base_url));
}

/**
 * Page callback: social block add/edit form.
 */
function mobilizer_add_social_icons_block($form, &$form_state) {
  $button_txt = 'Proceed';
  $social_facebook_icon = '';
  $social_facebook_url = '';
  $social_twitter_icon = '';
  $social_twitter_url = '';
  $social_youtube_url = '';
  $social_youtube_icon = '';
  $social_google_url = '';
  $social_google_icon = '';
  $social_linkedin_url = '';
  $social_linkedin_icon = '';
  if (is_numeric(arg(4)) && arg(5) == "edit" && is_numeric(arg(6))) {
    $blockobj = db_select('mobilizer_content_item', 'mct')
        ->fields('mct', array('page_id', 'content_data', 'item_type'))
        ->condition('page_id', arg(4))
        ->condition('block_id', arg(6))
        ->execute()->fetchObject();
    $social = unserialize($blockobj->content_data);
    

    $social_facebook_icon = $social['facebook_icon'];
    $social_facebook_url = $social['facebook_url'];
    $social_twitter_icon = $social['twitter_icon'];
    $social_twitter_url = $social['twitter_url'];
    $social_linkedin_icon = $social['linkedin_icon'];
    $social_linkedin_url = $social['linkedin_url'];
    $social_youtube_icon = $social['youtube_icon'];
    $social_youtube_url = $social['youtube_url'];
    $social_google_icon = $social['google_icon'];
    $social_google_url = $social['google_url'];
    $button_txt = 'Update';
  }
  $form['social_facebook_icon'] = array(
    '#type' => 'checkbox',
    '#title' => t('Facebook'),
    '#default_value' => $social_facebook_icon,
    '#attributes' => array('class' => array('social-facebook')),
    '#prefix' => '<div class = "clear">',
  );
  $form['social_facebook_url'] = array(
    '#type' => 'textfield',
    '#title' => t('Facebook URL'),
    '#default_value' => $social_facebook_url,
    '#suffix' => '</div>',
  );
  $form['social_twitter_icon'] = array(
    '#type' => 'checkbox',
    '#title' => t('Twitter'),
    '#default_value' => $social_twitter_icon,
    '#prefix' => '<div class = "clear">',
  );
  $form['social_twitter_url'] = array(
    '#type' => 'textfield',
    '#title' => t('Twitter URL'),
    '#default_value' => $social_twitter_url,
    '#suffix' => '</div>',
  );
  $form['social_google_icon'] = array(
    '#type' => 'checkbox',
    '#title' => t('Google Plus'),
    '#default_value' => $social_google_icon,
    '#prefix' => '<div class = "clear">',
  );
  $form['social_google_url'] = array(
    '#type' => 'textfield',
    '#title' => t('Google Plus URL'),
    '#default_value' => $social_google_url,
    '#suffix' => '</div>',
  );
  $form['social_youtube_icon'] = array(
    '#type' => 'checkbox',
    '#title' => t('Youtube'),
    '#default_value' => $social_youtube_icon,
    '#prefix' => '<div class = "clear">',
  );
  $form['social_youtube_url'] = array(
    '#type' => 'textfield',
    '#title' => t('Youtube URL'),
    '#default_value' => $social_youtube_url,
    '#suffix' => '</div>',
  );
  $form['social_linkedin_icon'] = array(
    '#type' => 'checkbox',
    '#title' => t('Linked In'),
    '#default_value' => $social_linkedin_icon,
    '#prefix' => '<div class = "clear">',
  );
  $form['social_linkedin_url'] = array(
    '#type' => 'textfield',
    '#title' => t('Linked In URL'),
    '#default_value' => $social_linkedin_url,
    '#suffix' => '</div>',
  );
  $form['save'] = array(
    '#type' => 'submit',
    '#value' => t($button_txt),
    '#attributes' => array('class' => array('btn btn-primary')),
    '#prefix' => '<div class = "submit_btn_bottom">',
  );
  $form['Cancel'] = array(
    '#type' => 'submit',
    '#value' => t('Cancel'),
    '#attributes' => array('class' => array('btn btn-primary','cancel')),
    '#suffix' => '</div>',
  );
  return $form;
}

/**
 * Implements form submit.
 */
function mobilizer_add_social_icons_block_submit($form, &$form_state) {
  $op = $form_state['input']['op'];
  $social['facebook_icon'] = $form_state['values']['social_facebook_icon'];
  $social['facebook_url'] = $form_state['values']['social_facebook_url'];
  $social['twitter_icon'] = $form_state['values']['social_twitter_icon'];
  $social['twitter_url'] = $form_state['values']['social_twitter_url'];
  $social['linkedin_icon'] = $form_state['values']['social_linkedin_icon'];
  $social['linkedin_url'] = $form_state['values']['social_linkedin_url'];
  $social['youtube_icon'] = $form_state['values']['social_youtube_icon'];
  $social['youtube_url'] = $form_state['values']['social_youtube_url'];
  $social['google_icon'] = $form_state['values']['social_google_icon'];
  $social['google_url'] = $form_state['values']['social_google_url'];
  if ($op == 'Proceed' || $op == 'Update') {
    if (is_numeric(arg(4)) && arg(5) == "edit" && is_numeric(arg(6))) { //@condition to update social block
      mobilizer_social_page_block_save(arg(4), "mobilizer_social", $social, arg(6));
    }
    else {
      mobilizer_social_page_block_save(arg(4), "mobilizer_social", $social, $block_id = "");
    }
    mobilizer_set_messages('Changes Updated successfully');
    drupal_goto('admin/mobilizer/select-menu-configure/page/' . arg(4) . '/edit');
  }
  else if(arg(5) == 'edit') {
    drupal_goto('admin/mobilizer/select-menu-configure/page/'.arg(4).'/edit');
  }
  else{
    drupal_goto('admin/mobilizer/select-menu-configure/page/'.arg(4).'/add/content');
  }
}

/**
 * @param unknown $page_id
 * @param unknown $item_type
 * @param unknown $data_arr
 * @param string $block_id @if block id , update the block data
 */
function mobilizer_social_page_block_save($page_id, $item_type, $data_arr, $block_id = "") {
  $html_for_block = "";
  if (arg(6)) {
    $blockhtml = db_select('mobilizer_node', 'mn')
        ->fields('mn', array('node_block_html'))
        ->condition('mid', arg(6))
        ->execute()->fetchAssoc();
    $html_for_block = $blockhtml['node_block_html'];
  }
  if ($item_type == "mobilizer_social") {
    $html_for_block = mobilizer_social_block_html($data_arr);
    $block_description = 'Social Block';
  }
  if ($block_id == "") {
    $status_id = db_insert('mobilizer_node')
        ->fields(array(
          'mlid' => $page_id,
          'bid' => 0,
          'weight' => 0,
          'ebabled' => 1,
          'block_description' => $block_description,
          'block_title' => $block_description,
          'block_delta' => '',
          'block_module' => '',
          'node_block_html' => $html_for_block,
          'type' => $item_type,
          'time' => REQUEST_TIME,))->execute();
    $content_item_id = db_insert('mobilizer_content_item')
        ->fields(array(
          'page_id' => $page_id,
          'item_type' => $item_type,
          'block_id' => $status_id,
          'content_data' => serialize($data_arr),
        ))->execute();
  } 
  else {
    $status_id = db_update('mobilizer_node')
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
 * @param array $attributes
 * @return string
 */
function mobilizer_social_block_html($attributes) {
  global $base_url;
  $image_base_path = $base_url . '/' . drupal_get_path('module', 'mobilizer');
  $social_facebook_icon = '';
  $social_facebook_url = '';
  $social_twitter_icon = '';
  $social_twitter_url = '';
  $social_youtube_url = '';
  $social_youtube_icon = '';
  $social_google_url = '';
  $social_google_icon = '';
  $social_linkedin_url = '';
  $social_linkedin_icon = '';
  $social_facebook_icon = $attributes['facebook_icon'];
  $social_facebook_url = $attributes['facebook_url'];
  $social_twitter_icon = $attributes['twitter_icon'];
  $social_twitter_url = $attributes['twitter_url'];
  $social_google_icon = $attributes['google_icon'];
  $social_google_url = $attributes['google_url'];
  $social_youtube_icon = $attributes['youtube_icon'];
  $social_youtube_url = $attributes['youtube_url'];
  $social_linkedin_icon = $attributes['linkedin_icon'];
  $social_linkedin_url = $attributes['linkedin_url'];
  $social_html = "";
  if ($social_facebook_icon) {
    $social_html .= '<a href =' . $attributes['facebook_url'] . ' target = "_blank" class = "social-icons" ><img src = ' . $image_base_path . '/images/socialmediaicons/f.png  width = "30px" height = "30px" style = "margin-right:2px;" /></a>';
  }
  if ($social_twitter_icon) {
    $social_html .= '<a href =' . $attributes['twitter_url'] . ' target = "_blank" class = "social-icons" ><img src = ' . $image_base_path . '/images/socialmediaicons/t.png  width = "30px" height = "30px" style = "margin-right:2px;" /></a>';
  }
  if ($social_google_icon) {
    $social_html .= '<a href =' . $attributes['google_url'] . ' target = "_blank" class = "social-icons" ><img src = ' . $image_base_path . '/images/socialmediaicons/g.png  width = "30px" height = "30px" style = "margin-right:2px;" /></a>';
  }
  if ($social_youtube_icon) {
    $social_html .= '<a href =' . $attributes['youtube_url'] . ' target = "_blank" class = "social-icons" ><img src = ' . $image_base_path . '/images/socialmediaicons/y.png  width = "30px" height = "30px" style = "margin-right:2px;" /></a>';
  }
  if ($social_linkedin_icon) {
    $social_html .= '<a href =' . $attributes['linkedin_url'] . ' target = "_blank" class = "social-icons" ><img src = ' . $image_base_path . '/images/socialmediaicons/i.png  width = "30px" height = "30px" style = "margin-right:2px;" /></a>';
  }
  return $social_html;
}


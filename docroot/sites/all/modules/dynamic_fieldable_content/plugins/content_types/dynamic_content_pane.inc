<?php
/**
 * This plugin array is more or less self documenting
 */
$plugin = array(
    'title' => t('Custom Fieldable content'),
    'no title override' => TRUE,
    'render callback' => 'dynamic_fieldable_content_type_render',
    'defaults' => array('admin_title' => '', 'title' => '', 'body' => '', 'repeat_body' => '', 'format' => 'addon_code', 'substitute' => TRUE),
    'js' => array('misc/autocomplete.js', 'misc/textarea.js', 'misc/collapse.js'),
    // Make sure the edit form is only used for some subtypes.
    'edit form' => 'dynamic_fieldable_content_type_edit_form',
    'add form' => '',
    'edit text' => t('Edit'),
    'all contexts' => TRUE,
);


/**
 * Return the custom content types with the specified $subtype_id.
*/
function dynamic_fieldable_content_type_content_type($subtype_id) {
  if ($subtype_id == 'dynamic_content_pane') {
    return _ctools_default_content_type_content_type();
  }
  elseif (module_exists('dynamic_fieldable_content')) {
    ctools_include('export');
    $content = ctools_export_crud_load('dynamic_fieldable_content', $subtype_id);
    if ($content) {
      return _dynamic_fieldable_content_type_content_type($content);
    }
  }
}

/**
 * Return all custom content types available.
 */
function dynamic_fieldable_content_dynamic_content_pane_content_type_content_types() {
  ctools_include('export');
  $types = array();
  $types['dynamic_content_pane'] = _dynamic_fieldable_content_default_content_type_content_type();

  if (module_exists('dynamic_fieldable_content')) {
    foreach (ctools_export_crud_load_all('dynamic_fieldable_content') as $name => $content) {
      $types[$name] = _dynamic_fieldable_content_type_content_type($content);
    }
  }
  return $types;
}

/**
 * Settings for the default custom content type.
 *
 * The default is the one that allows the user to actually create a type.
 */
function _dynamic_fieldable_content_default_content_type_content_type() {
  $info = array(
      'name' => 'dynamic',
      'title' => t('New dynamic fieldable content'),
      'top level' => TRUE,
      'category' => t('Dynamic'),
      'description' => t('Create a completely custom piece of HTML content.'),
      //  'edit form' => 'dynamic_fieldable_content_type_edit_form',
      'edit form' => '',
      'all contexts' => TRUE,
      'check editable' => 'dynamic_fieldable_content_type_editable',
  );

  return $info;
}

/**
 * Return an info array for a specific custom content type.
 */
function _dynamic_fieldable_content_type_content_type($content) {
  $info = array(
      'name' => $content->name,
      'title' => check_plain($content->admin_title),
      'description' => check_plain($content->admin_description),
      'category' => $content->category ? check_plain($content->category) : t('Dynamic'),
      'all contexts' => TRUE,
      'icon' => 'icon_block_custom.png',
      // Store this here to make it easy to access.
      'content' => $content,
  );

  return $info;
}

/**
 * Given a subtype and a $conf, return the actual settings to use.
 *
 * The actual settings may be stored directly in the pane or this may
 * be a pointer to re-usable content that may be in the database or in
 * an export. We have to determine from the subtype whether or not it
 * is local or shared custom content.
 */
function dynamic_fieldable_content_type_get_conf($subtype, $conf) {
  if ($subtype['name'] != 'dynamic_content_pane') {
    $settings = $subtype['content']->settings;
    $settings['custom_type'] = 'fixed';
    $settings['content'] = $subtype['content'];
  }
  else {
    // This means they created it as custom content and then set it as
    // reusable. Since we're not allowed to change the subtype, we're
    // still stored as though we are local, but are pointing off to
    // non-local.
    if (!empty($conf['name']) && module_exists('dynamic_fieldable_content')) {
      ctools_include('export');
      $content = ctools_export_crud_load('dynamic_fieldable_content', $conf['name']);
      if ($content) {
        $settings = $content->settings;
        $settings['custom_type'] = 'fixed';
        $settings['content'] = $content;
        $settings['admin_title'] = $content->admin_title;
      }
      else {
        $content = ctools_export_crud_new('dynamic_fieldable_content');
        $content->name = $conf['name'];
        $settings = array(
            'admin_title' => t('Missing/deleted content'),
            'title' => '',
            'body' => '',
            'repeat_body' => '',
            'format' => 'addon_code',
            'substitute' => TRUE,
            'custom_type' => 'fixed',
            'content' => $content,
        );
      }
    }
    // This means that it is created as custom and has not been set to
    // reusable.
    else {
      $settings = $conf;
      $settings['custom_type'] = 'local';
    }
  }

  // Correct for an error that came in because filter format changed.
  if (is_array($settings['body'])) {
    $settings['format'] = $settings['body']['format'];
    $settings['body'] = $settings['body']['value'];
  }

  return $settings;
}

function dynamic_fieldable_content_type_editable($content_type, $subtype, $conf) {
  if ($subtype['name'] == 'dynamic_content_pane' && !empty($conf['name'])) {
    return FALSE;
  }

  return TRUE;
}

/**
 * Output function for the 'custom' content type. Outputs a custom
 * based on the module and delta supplied in the configuration.
 */
function dynamic_fieldable_content_type_render($subtype, $conf, $args, $contexts) {
   //dsm($subtype);
  //dsm($conf);
  $settings = dynamic_fieldable_content_type_get_conf(dynamic_fieldable_content_type_content_type($subtype), $conf);
  static $delta = 0;

  $block          = new stdClass();
  $block->subtype = ++$delta;
  $block->title   = filter_xss_admin($settings['title']);
  
  $unique_block_id = $subtype.$delta;

  // Add keyword substitutions if we were configured to do so.
  $content = $settings['body'];
  $repeat_content = $settings['repeat_body'];
  if(is_array($repeat_content) && isset($repeat_content['value'])){
    $repeat_content = $repeat_content['value'];
  }
 // dsm($repeat_content);
  $settings_count = $settings['num_names'];
  $js_count = $settings['num_js'];
  $css_count = $settings['num_css'];
  $full_repeat_content = "";
  // dsm($repeat_content);
  // $content = str_replace("#dynamic:RepeatCode", $full_repeat_content, $content);
  for ($i = 0; $i < $settings_count; $i++) {
    $label = $settings['settings_fieldset_name_'.$i];
    $value = $settings['settings_fieldset_value_'.$i];
  //  dsm($label." -> ".$value);
    $repeat_content = str_replace("#dynamic:".$label."#", $value, $repeat_content);
    $content = str_replace("#dynamic:".$label."#", $value, $content);
  }
  $repeat_content = str_replace("#dynamic:WidgetID#", $unique_block_id, $repeat_content);
  $content = str_replace("#dynamic:WidgetID#", $unique_block_id, $content);
  for ($i = 0; $i < $js_count; $i++) {
    $js_url = isset($settings['js_fieldset_js_url_'.$i]) ? $settings['js_fieldset_js_url_'.$i] : "";
    if(empty($js_url)){
      continue;
    }
    $js_download = isset($settings['js_fieldset_js_download_'.$i]) ? $settings['js_fieldset_js_download_'.$i] : FALSE;
    //s3.amazonaws.com system_retrieve_file can't download from amazon s3. even curl doesn't work
    if($js_download && (strpos($js_url, "s3.amazonaws.com") < 1)){
      $dir_path = variable_get('file_public_path', conf_path() . '/files')."/dynamic_fieldable_content/{$subtype}/";
      $local_url = $dir_path.basename($js_url);
      if (file_exists($local_url)) {
        $js_url = $local_url;
      }else{
        file_prepare_directory($dir_path, $options = FILE_CREATE_DIRECTORY);
        $js_url = system_retrieve_file($js_url, $destination = $dir_path, $managed = FALSE, $replace = FILE_EXISTS_REPLACE);
      }
    }
    //if relative file, use relative path for aggregation. no caching.
    if(substr($js_url, 0, 4) != "http"){
      drupal_add_js($js_url);
    }else{
      drupal_add_js($js_url, 'external');
    }
  }
  
  for ($i = 0; $i < $css_count; $i++) {
    $css_url = isset($settings['css_fieldset_css_url_'.$i]) ? $settings['css_fieldset_css_url_'.$i] : "";
    if(empty($css_url)){
      continue;
    }
    $css_download = isset($settings['css_fieldset_css_download_'.$i]) ? $settings['css_fieldset_css_download_'.$i] : FALSE;
    //s3.amazonaws.com system_retrieve_file can't download from amazon s3. even curl doesn't work
    if($css_download && (strpos($css_url, "s3.amazonaws.com") < 1)){
      $dir_path = variable_get('file_public_path', conf_path() . '/files')."/dynamic_fieldable_content/{$subtype}/";
      $local_url = $dir_path.basename($css_url);
      if (file_exists($local_url)) {
        $css_url = $local_url;
      }else{
        file_prepare_directory($dir_path, $options = FILE_CREATE_DIRECTORY);
        $css_url = system_retrieve_file($css_url, $destination = $dir_path, $managed = FALSE, $replace = FILE_EXISTS_REPLACE);
      }
    }
    if(substr($css_url, 0, 4) != "http"){
      drupal_add_css($css_url);
    }else{
      drupal_add_css($css_url, 'external');
    }
  }
  
 // dsm($repeat_content);
  preg_match_all("/%node:schemaorg-.*:value:\?/i",
  $repeat_content,
  $out, PREG_PATTERN_ORDER);
  if(isset($out[0]) && isset($out[0][0])){
    $token_code = $out[0][0];
    $token_code = str_replace(":value:?", ":count", $token_code);
    $repeat_count = ctools_context_keyword_substitute($token_code, array(), $contexts);
    for ($i = 0; $i < $repeat_count; $i++) {
      $repeat_content_with_counter = str_replace(':value:?', ":value:$i", $repeat_content);
      $full_repeat_content .= $repeat_content_with_counter;
    }
  }
  /*
  $end_pos = strpos($repeat_content, ":value:?]");
  if($end_pos > 1){
  $start_pos = strrpos($repeat_content, "[node:", $end_pos);
  $token_code = substr($repeat_content,$start_pos,$end_pos);
  dsm($token_code);
  }
  */
 // dsm($content);
  if(!empty($full_repeat_content)){
    $content = str_replace("#dynamic:RepeatCode#", $full_repeat_content, $content);
  }
 // dsm($content);
  if (!empty($contexts) && !empty($settings['substitute'])) {
    // dsm($content);
    // dsm($contexts);
    $content = ctools_context_keyword_substitute($content, array(), $contexts);
    // dsm($content);
  }

  $block->content = check_markup($content, $settings['format']);
  $block->repeat_content = check_markup($repeat_content, $settings['format']);
  if ($settings['custom_type'] == 'fixed' && user_access('administer custom content')) {
    $block->admin_links = array(
        array(
            'title' => t('Configure content pane'),
            'alt' => t("Configure this pane in administer >> structure >> custom content panes"),
            'href' => 'admin/structure/dynamic-content/list/' . $settings['content']->name . '/edit',
            'query' => drupal_get_destination(),
        ),
    );
  }

  return $block;
}

/**
 * Callback to provide the administrative title of the custom content.
 */
function dynamic_fieldable_content_type_admin_title($subtype, $conf) {
  $settings = dynamic_fieldable_content_type_get_conf(dynamic_fieldable_content_type_content_type($subtype), $conf);

  $output = t('Custom');
  $title = !empty($settings['admin_title']) ? $settings['admin_title'] : $settings['title'];
  if ($title) {
    if ($settings['custom_type'] != 'fixed') {
      $output = t('Custom: @title', array('@title' => $title));
    }
    else {
      $output = $title;
    }
  }

  return $output;
}

/**
 * Callback to provide administrative info. In this case we'll render the
 * content as long as it's not PHP, which is too risky to render here.
 */
function dynamic_fieldable_content_type_admin_info($subtype, $conf) {
  $settings = dynamic_fieldable_content_type_get_conf(dynamic_fieldable_content_type_content_type($subtype), $conf);

  $block = new stdClass();
  $block->title = filter_xss_admin($settings['title']);
  // We don't want to render php output on preview here, because if something is
  // wrong the whole display will be borked. So we check to see if the php
  // evaluator filter is being used, and make a temporary change to the filter
  // so that we get the printed php, not the eval'ed php.
  $php_filter = FALSE;
  foreach (filter_list_format($settings['format']) as $filter) {
    if ($filter->module == 'php') {
      $php_filter = TRUE;
      break;
    }
  }
  // If a php filter is active, just print the source, but only if the current
  // user has access to the actual filter.
  if ($php_filter) {
    $filter = filter_format_load($settings['format']);
    if (!filter_access($filter)) {
      return NULL;
    }
    $block->content = '<pre>' . check_plain($settings['body']) . '</pre>';
  }
  else {
    // We also need to filter through XSS admin because <script> tags can
    // cause javascript which will interfere with our ajax.
    $block->content = filter_xss_admin(check_markup($settings['body'], $settings['format']));
  }
  return $block;
}

function dynamic_fieldable_content_settings_add_more($form, &$form_state, $settings, $no_js_use = FALSE) {
  drupal_add_library('system', 'ui.autocomplete');
  drupal_add_library('system', 'ui.button');
  drupal_add_css(drupal_get_path('module', 'dynamic_fieldable_content') . "/css/dynamic_fieldable_content_settings.css");
  
  $form['js_fieldset'] = array(
      '#type' => 'fieldset',
      '#title' => t('Javascript URL'),
      // Set up the wrapper so that AJAX will be able to replace the fieldset.
      '#prefix' => '<div id="js-fieldset-wrapper">',
      '#suffix' => '</div>',
      '#description' => t("Put valid URL. this will be added to drupal via drupal_add_js()."),
  );
  $form['js_fieldset']['#tree'] = TRUE;
  
  // Build the fieldset with the proper number of names. We'll use
  // $form_state['num_names'] to determine the number of textfields to build.
  if (empty($form_state['num_js'])) {
    $form_state['num_js'] = (isset($settings['num_js']) && !empty($settings['num_js']) ? $settings['num_js'] : 1);
  }
  
  for ($i = 0; $i < $form_state['num_js']; $i++) {
    $form['js_fieldset']['js_url'][$i] = array(
        '#type' => 'textfield',
        '#title' => t(''),
        '#size' => 15,
        '#default_value' => (isset($settings['js_fieldset_js_url_'.$i]) && !empty($settings['js_fieldset_js_url_'.$i]) ? $settings['js_fieldset_js_url_'.$i] : ""),
        '#weight' => $i*0,
    );
  }
  
  $form['js_fieldset']['add_js'] = array(
      '#type' => 'submit',
      '#value' => t('Add one more JS URL'),
      '#submit' => array('dynamic_fieldable_content_js_add_more_add_one'),
      // See the examples in dynamic_fieldable_content_settings.module for more details on the
      // properties of #ajax.
      '#ajax' => array(
          'callback' => 'dynamic_fieldable_content_js_add_more_callback',
          'wrapper' => 'js-fieldset-wrapper',
      ),
      '#prefix' => '<div style="clear:both"></div>',
  );
  
  if ($form_state['num_js'] > 1) {
    $form['js_fieldset']['remove_js'] = array(
        '#type' => 'submit',
        '#value' => t('Remove one JS'),
        '#submit' => array('dynamic_fieldable_content_js_add_more_remove_one'),
        '#ajax' => array(
            'callback' => 'dynamic_fieldable_content_js_add_more_callback',
            'wrapper' => 'js-fieldset-wrapper',
        ),
        '#prefix' => '<div class="clearfix">',
        '#suffix' => '</div>',
    );
  }
  
  if ($no_js_use) {
    // Remove the #ajax from the above, so ajax.js won't be loaded.
    if (!empty($form['js_fieldset']['remove_js']['#ajax'])) {
      unset($form['js_fieldset']['remove_js']['#ajax']);
    }
    unset($form['js_fieldset']['add_js']['#ajax']);
  }
  
  $form['css_fieldset'] = array(
      '#type' => 'fieldset',
      '#title' => t('CSS URL'),
      // Set up the wrapper so that AJAX will be able to replace the fieldset.
      '#prefix' => '<div id="css-fieldset-wrapper">',
      '#suffix' => '</div>',
      '#description' => t("Put valid URL. this will be added to drupal via drupal_add_css()."),
  );
  $form['css_fieldset']['#tree'] = TRUE;
  
  // Build the fieldset with the proper number of names. We'll use
  // $form_state['num_names'] to determine the number of textfields to build.
  if (empty($form_state['num_css'])) {
    $form_state['num_css'] = (isset($settings['num_css']) && !empty($settings['num_css']) ? $settings['num_css'] : 1);
  }
  
  for ($i = 0; $i < $form_state['num_css']; $i++) {
    $form['css_fieldset']['css_url'][$i] = array(
        '#type' => 'textfield',
        '#title' => t(''),
        '#size' => 15,
        '#default_value' => (isset($settings['css_fieldset_css_url_'.$i]) && !empty($settings['css_fieldset_css_url_'.$i]) ? $settings['css_fieldset_css_url_'.$i] : ""),
        '#weight' => $i*0,
    );
  }
  
  $form['css_fieldset']['add_css'] = array(
      '#type' => 'submit',
      '#value' => t('Add one more CSS URL'),
      '#submit' => array('dynamic_fieldable_content_css_add_more_add_one'),
      // See the examples in dynamic_fieldable_content_settings.module for more details on the
      // properties of #ajax.
      '#ajax' => array(
          'callback' => 'dynamic_fieldable_content_css_add_more_callback',
          'wrapper' => 'css-fieldset-wrapper',
      ),
      '#prefix' => '<div style="clear:both"></div>',
  );
  
  if ($form_state['num_css'] > 1) {
    $form['css_fieldset']['remove_css'] = array(
        '#type' => 'submit',
        '#value' => t('Remove one CSS'),
        '#submit' => array('dynamic_fieldable_content_css_add_more_remove_one'),
        '#ajax' => array(
            'callback' => 'dynamic_fieldable_content_css_add_more_callback',
            'wrapper' => 'css-fieldset-wrapper',
        ),
        '#prefix' => '<div class="clearfix">',
        '#suffix' => '</div>',
    );
  }
  
  if ($no_js_use) {
    // Remove the #ajax from the above, so ajax.js won't be loaded.
    if (!empty($form['css_fieldset']['remove_css']['#ajax'])) {
      unset($form['css_fieldset']['remove_css']['#ajax']);
    }
    unset($form['css_fieldset']['add_css']['#ajax']);
  }
  
  $form['settings_fieldset'] = array(
      '#type' => 'fieldset',
      '#title' => t('Widget Settings'),
      // Set up the wrapper so that AJAX will be able to replace the fieldset.
      '#prefix' => '<div id="settings-fieldset-wrapper">',
      '#suffix' => '</div>',
      '#description' => t("#dynamic:&lt;Label&gt#; in the body will be replaced by it's correesponding value. example: #dynamic:Label1#"),
  );
  $form['settings_fieldset']['#tree'] = TRUE;

  $form['settings_fieldset']['label_fieldset'] = array(
      '#type' => 'fieldset',
      '#title' => t('Label'),
      '#attributes' => array(
          'class' => array(
              'class-fieldcollection-label',
          )
      ),
  );

  $form['settings_fieldset']['value_fieldset'] = array(
      '#type' => 'fieldset',
      '#title' => t('Value'),
      '#attributes' => array(
          'class' => array(
              'class-fieldcollection-value',
          )
      ),
  );

  // Build the fieldset with the proper number of names. We'll use
  // $form_state['num_names'] to determine the number of textfields to build.
  if (empty($form_state['num_names'])) {
    $form_state['num_names'] = (isset($settings['num_names']) && !empty($settings['num_names']) ? $settings['num_names'] : 1);
  }


  for ($i = 0; $i < $form_state['num_names']; $i++) {
    $form['settings_fieldset']['label_fieldset']['name'][$i] = array(
        '#type' => 'textfield',
        '#title' => t(''),
        '#size' => 15,
        '#default_value' => (isset($settings['settings_fieldset_name_'.$i]) && !empty($settings['settings_fieldset_name_'.$i]) ? $settings['settings_fieldset_name_'.$i] : "Label".($i+1)),
        '#weight' => $i*0,
        '#attributes' => array(
            'class' => array(
                'class-fieldcollection-label'
            )
        ),
    );
    $form['settings_fieldset']['value_fieldset']['value'][$i] = array(
        '#type' => 'textfield',
        '#title' => t(''),
        '#size' => 25,
        '#default_value' => (isset($settings['settings_fieldset_value_'.$i]) ? $settings['settings_fieldset_value_'.$i] : "Value".($i+1)),
        '#weight' => ($i*0)+1,
        '#attributes' => array(
            'class' => array(
                'class-fieldcollection-type',
            )
        ),
    );
    if(isset($settings['s_f_options_'.$i]) && !empty($settings['s_f_options_'.$i])){
      $optArray = explode("\n", $settings['s_f_options_'.$i]);
      $opt_string = "";
      foreach ($optArray as $opt){
        $opt = trim($opt,"\r\n");
        $opt_string .= '"'.$opt.'",';
      }
      $opt_string = trim($opt_string,",");
      $dynamic_js_code = '(function($) {
        var availableTags'.$i.' = ['.$opt_string.'];

			                 var $input'.$i.' = $("#edit-settings-fieldset-value-fieldset-value-'.$i.'").autocomplete({
			                     source: availableTags'.$i.',
			                     minLength: 0
			                 }).addClass("ui-widget ui-widget-content ui-corner-left");

			                 $("<button type=\'button\'>&nbsp;</button>")                     
			                     .attr("tabIndex", -1)                     
			                     .attr("title", "Show All Items")                     
			                     .insertAfter($input'.$i.')                     
			                     .button({                         
			                         icons: {                             
			                             primary: "ui-icon-triangle-1-s"                         
			                         },                         
			                         text: false                     
			                     })                                      
			                     .click(function() {                         
			                         // close if already visible                         
			                         if ($input'.$i.'.autocomplete("widget").is(":visible")) {
			                              $input'.$i.'.autocomplete( "close" );
			                              return;                         
			                         }                                              
			                         $(this).blur();                                                 
			                         $input'.$i.'.autocomplete("search", "" );                         
			                         $input'.$i.'.focus();                     
			                     });
			                     })(jQuery);';
      drupal_add_js($dynamic_js_code, array('type' => 'inline', 'scope' => 'footer', 'weight' => 5));
    }
  }
  $form['settings_fieldset']['add_name'] = array(
      '#type' => 'submit',
      '#value' => t('Add one more'),
      '#submit' => array('dynamic_fieldable_content_settings_add_more_add_one'),
      // See the examples in dynamic_fieldable_content_settings.module for more details on the
      // properties of #ajax.
      '#ajax' => array(
          'callback' => 'dynamic_fieldable_content_settings_add_more_callback',
          'wrapper' => 'settings-fieldset-wrapper',
      ),
      '#prefix' => '<div style="clear:both"></div>',
  );

  $form['#attached']['css'] = array(
      drupal_get_path('module', 'dynamic_fieldable_content') . '/css/dynamic_fieldable_content_add_more.css',
  );

  if ($form_state['num_names'] > 1) {
    $form['settings_fieldset']['remove_name'] = array(
        '#type' => 'submit',
        '#value' => t('Remove one'),
        '#submit' => array('dynamic_fieldable_content_settings_add_more_remove_one'),
        '#ajax' => array(
            'callback' => 'dynamic_fieldable_content_settings_add_more_callback',
            'wrapper' => 'settings-fieldset-wrapper',
        ),
        '#prefix' => '<div class="clearfix">',
        '#suffix' => '</div>',
    );
  }

  // This simply allows us to demonstrate no-javascript use without
  // actually turning off javascript in the browser. Removing the #ajax
  // element turns off AJAX behaviors on that element and as a result
  // ajax.js doesn't get loaded.
  // For demonstration only! You don't need this.
  if ($no_js_use) {
    // Remove the #ajax from the above, so ajax.js won't be loaded.
    if (!empty($form['settings_fieldset']['remove_name']['#ajax'])) {
      unset($form['settings_fieldset']['remove_name']['#ajax']);
    }
    unset($form['settings_fieldset']['add_name']['#ajax']);
  }

  return $form;
}

/**
 * Returns an edit form for the custom type.
 */
function dynamic_fieldable_content_type_edit_form($form, &$form_state) {
//  dsm($form);
  $settings = dynamic_fieldable_content_type_get_conf($form_state['subtype'], $form_state['conf']);
  $form_state['settings'] = $settings;

  if ($settings['custom_type'] == 'fixed') {
    // return $form; // no form for this case.  TODO: why was this added earlier?
  }
  /*
   $form['admin_title'] = array(
       '#type' => 'textfield',
       '#default_value' => isset($settings['admin_title']) ? $settings['admin_title'] : '',
       '#title' => t('Administrative title'),
       '#description' => t('This title will be used administratively to identify this pane. If blank, the regular title will be used.'),
   );
  */
  $form['title'] = array(
      '#type' => 'textfield',
      '#default_value' => $settings['title'],
      '#title' => t('Title'),
  );
  $form = dynamic_fieldable_content_settings_add_more($form, $form_state, $settings);
  $form['body'] = array(
      '#type' => 'text_format',
      '#title' => t('Body'),
      '#default_value' => $settings['body'],
      '#format' => $settings['format'],
  );
  $form['repeat_body'] = array(
      '#type' => 'text_format',
      '#title' => t('Repeat Code'),
      '#default_value' => $settings['repeat_body'],
      '#format' => $settings['format'],
  );

  if (!empty($form_state['contexts'])) {
    // Set extended description if both CCK and Token modules are enabled, notifying of unlisted keywords
    if (module_exists('content') && module_exists('token')) {
      $description = t('If checked, context keywords will be substituted in this content. Note that CCK fields may be used as keywords using patterns like <em>%node:field_name-formatted</em>.');
    }
    elseif (!module_exists('token')) {
      $description = t('If checked, context keywords will be substituted in this content. More keywords will be available if you install the Token module, see http://drupal.org/project/token.');
    }
    else {
      $description = t('If checked, context keywords will be substituted in this content.');
    }

    $form['substitute'] = array(
        '#type' => 'checkbox',
        '#title' => t('Use context keywords'),
        '#description' => $description,
        '#default_value' => !empty($settings['substitute']),
    );
    $form['contexts'] = array(
        '#title' => t('Substitutions'),
        '#type' => 'fieldset',
        '#collapsible' => TRUE,
        '#collapsed' => TRUE,
    );

    $rows = array();
    foreach ($form_state['contexts'] as $context) {
      foreach (ctools_context_get_converters('%' . check_plain($context->keyword) . ':', $context) as $keyword => $title) {
        $rows[] = array(
            check_plain($keyword),
            t('@identifier: @title', array('@title' => $title, '@identifier' => $context->identifier)),
        );
      }
    }
    $header = array(t('Keyword'), t('Value'));
    $form['contexts']['context'] = array('#markup' => theme('table', array('header' => $header, 'rows' => $rows)));
  }

  if (!user_access('administer custom content') || !module_exists('dynamic_fieldable_content')) {
    return $form;
  }

  // Make the other form items dependent upon it.
  ctools_include('dependent');
  ctools_add_js('dependent');

  $form['reusable'] = array(
      '#type' => 'checkbox',
      '#title' => t('Make this content reusable'),
      '#default_value' => FALSE,
  );

  $form['name'] = array(
      '#type' => 'textfield',
      '#title' => t('Machine name'),
      '#description' => t('The machine readable name of this content. It must be unique, and it must contain only alphanumeric characters and underscores. Once created, you will not be able to change this value!'),
      '#dependency' => array('edit-reusable' => array(1)),
  );

  $form['category'] = array(
      '#type' => 'textfield',
      '#title' => t('Category'),
      '#description' => t('What category this content should appear in. If left blank the category will be "Miscellaneous".'),
      '#dependency' => array('edit-reusable' => array(1)),
  );

  $form['admin_description'] = array(
      '#type' => 'textarea',
      '#title' => t('Administrative description'),
      '#description' => t('A description of what this content is, does or is for, for administrative use.'),
      '#dependency' => array('edit-reusable' => array(1)),
  );
  return $form;
}

function _dynamic_fieldable_content_type_edit_save(&$content, $form_state) {
  // Apply updates to the content object.
  /*  $content->category = $form_state['values']['category'];
  $content->admin_title = $form_state['values']['admin_title'];
  $content->admin_description = $form_state['values']['admin_description'];
  */
  foreach (array_keys($form_state['plugin']['defaults']) as $key) {
    if (isset($form_state['values'][$key])) {
      $content->settings[$key] = $form_state['values'][$key];
    }
  }

  $ctr = 0;
  foreach ($form_state['values']['settings_fieldset']['label_fieldset']['name'] as $key => $value) {
    $content->settings['settings_fieldset_name_'.$key] = $form_state['values']['settings_fieldset']['label_fieldset']['name'][$key];
    $ctr++;
  }
  foreach ($form_state['values']['settings_fieldset']['value_fieldset']['value'] as $key => $value) {
    $content->settings['settings_fieldset_value_'.$key] = $form_state['values']['settings_fieldset']['value_fieldset']['value'][$key];
  }
  $content->settings['num_names'] = $ctr;
  
  $ctr = 0;
  foreach ($form_state['values']['css']['js_url'] as $key => $value) {
    $content->settings['js_fieldset_js_url_'.$key] = $form_state['values']['js_fieldset']['js_url'][$key];
    $ctr++;
  }
  $content->settings['num_js'] = $ctr;
  
  $ctr = 0;
  foreach ($form_state['values']['css_fieldset']['css_url'] as $key => $value) {
    $content->settings['css_fieldset_css_url_'.$key] = $form_state['values']['css_fieldset']['css_url'][$key];
    $ctr++;
  }
  $content->settings['num_css'] = $ctr;

  ctools_export_crud_save('dynamic_fieldable_content', $content);
}

/**
 * The validate form to ensure the custom content data is okay.
 */
function dynamic_fieldable_content_type_edit_form_validate(&$form, &$form_state) {
  if ($form_state['settings']['custom_type'] != 'fixed' && !empty($form_state['values']['reusable'])) {
    if (empty($form_state['values']['name'])) {
      form_error($form['name'], t('Name is required.'));
    }

    // Check for string identifier sanity
    if (!preg_match('!^[a-z0-9_]+$!', $form_state['values']['name'])) {
      form_error($form['name'], t('The name can only consist of lowercase letters, underscores, and numbers.'));
      return;
    }

    if (!module_exists('dynamic_fieldable_content')) {
      return;
    }

    // Check for name collision
    if ($form_state['values']['name'] == 'dynamic_content_pane' || (ctools_export_crud_load('dynamic_fieldable_content', $form_state['values']['name']))) {
      form_error($form['name'], t('Content with this name already exists. Please choose another name or delete the existing item before creating a new one.'));
    }
  }
}

/**
 * The submit form stores the data in $conf.
 */
function dynamic_fieldable_content_type_edit_form_submit($form, &$form_state) {
  // Because of changes in filter form, these two keys are out of position:
  $form_state['values']['format'] = $form_state['values']['body']['format'];
  $form_state['values']['body'] = $form_state['values']['body']['value'];

  if ($form_state['settings']['custom_type'] == 'fixed') {
    _dynamic_fieldable_content_type_edit_save($form_state['settings']['content'], $form_state);
  }
  // If the 'reusable' checkbox was checked, we will create a new
  // custom content and give it the proper values.
  else if (!empty($form_state['values']['reusable'])) {
    $content = ctools_export_crud_new('dynamic_fieldable_content');
    $content->name = $form_state['values']['name'];
    _dynamic_fieldable_content_type_edit_save($content, $form_state);
    $form_state['conf']['name'] = $content->name;
  }
  else {
    // Otherwise, just save values into $conf normally.

    foreach (array_keys($form_state['plugin']['defaults']) as $key) {
      $form_state['conf'][$key] = isset($form_state['values'][$key]) ? $form_state['values'][$key] : $form_state['plugin']['defaults'][$key];
    }
  }
}

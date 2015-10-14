<?php

/**
 * @file
 * Administrative page callbacks for the recommenderghost module.
 */

/**
 * Implements hook_admin_settings().
 */
function recommenderghost_admin_settings_form($form_state) {
  $form['account'] = array(
    '#type' => 'fieldset',
    '#title' => t('General settings'),
  );

  $form['account']['recommenderghost_apikey'] = array(
    '#title' => t('API key'),
    '#type' => 'textfield',
    '#default_value' => variable_get('recommenderghost_apikey', ''),
    '#size' => 35,
    '#maxlength' => 32,
    '#required' => TRUE,
    '#description' => t('This is your API key. To get an API key, <a href="@recommenderghost">register your site with RecommenderGhost</a>, or if you already have registered your site, go to your RecommenderGhost - My Account to see the API key next  to your profile. <a href="@apikey">Find more information in the documentation</a>.', array(
      '@recommenderghost' => 'http://www.recommenderghost.com',
      '@apikey' => url('http://www.recommenderghost.com/how-to',
      array('fragment' => 'apikey')))),
  );
  $form['account']['recommenderghost_websiteid'] = array(
    '#title' => t('Website ID'),
    '#type' => 'textfield',
    '#default_value' => variable_get('recommenderghost_websiteid', ''),
    '#size' => 35,
    '#maxlength' => 15,
    '#required' => TRUE,
    '#description' => t('This website ID is unique to each site you want to track separately. To get a website ID, <a href="@recommenderghost">register your site with RecommenderGhost</a>, or if you already have registered your site, go to your RecommenderGhost - My Websites and see the website ID next to every site profile. <a href="@apikey">Find more information in the documentation</a>.', array(
      '@recommenderghost' => 'http://www.recommenderghost.com',
      '@apikey' => url('http://www.recommenderghost.com/how-to',
      array('fragment' => 'websiteid')))),
  );

  $form['account']['recommenderghost_tracking_method'] = array(
    '#type' => 'radios',
    '#title' => t('Which API do you want to use?'),
    '#options' => array(
      "JS" => t('Javascript (default)') . '<div class="description">' .
      t('Use Javascript for tracking and receiving recommendations.') . '</div>',
      "REST" => t('REST') . '<div class="description">' .
      t('Use REST calls for tracking and receiving recommendations. The advantage is that every interaction with RecommenderGhost is invisible to the user. Please note that this method cannot be used in conjunction with caching like Varnish or Boost module.') . '</div>',
    ),
    '#default_value' => variable_get('recommenderghost_tracking_method', "JS"),
  );

  // Visibility settings.
  $form['tracking_title'] = array(
    '#type' => 'item',
    '#title' => t('Tracking scope'),
  );
  $form['tracking'] = array(
    '#type' => 'vertical_tabs',
  );

  // Render the content types overview.
  $form['tracking']['content_type_settings'] = array(
    '#type' => 'fieldset',
    '#title' => t('Content Types'),
  );

  $form['tracking']['content_type_settings']['recommenderghost_visibility_content_types'] = array(
    '#type' => 'radios',
    '#title' => t('Add tracking for specific content types'),
    '#options' => array(
      t('Add to the selected content types only'),
      t('Add to every content type except the selected ones'),
    ),
    '#default_value' => variable_get('recommenderghost_visibility_content_types', 0),
  );

  $content_type_checkbox_list_options = array();
  $content_types = node_type_get_types();
  foreach ($content_types as $id => $content_type) {
    $content_type_checkbox_list_options[$content_type->type] = $content_type->name;
  }
  $form['tracking']['content_type_settings']['recommenderghost_content_types'] = array(
    '#type' => 'checkboxes',
    '#options' => $content_type_checkbox_list_options,
    '#default_value' => variable_get('recommenderghost_content_types', array()),
    '#title' => t('Content types'),
    '#description' => t('If none of the content types are selected, all content types will be tracked. If one or more content types are checked, that content type will be tracked (or excluded, depending on the setting above).'),
  );

  // Render the item mapping overview.
  $form['tracking']['item_type_settings'] = array(
    '#type' => 'fieldset',
    '#title' => t('Item Types'),
    '#description' => t('You can map your content types to item types created in RecommenderGhost. Standard item type is ITEM.'),
  );
  $item_array = array("ITEM" => "ITEM");
  if (variable_get('recommenderghost_apikey', '')) {
    $item_obj = recommenderghost_call_api("itemtypes", array());
    if (isset($item_obj->error)) {
      watchdog('recommenderghost', 'Error: !error', array('!error' => '<pre>' . print_r($item_obj, TRUE) . '</pre>'));
    }
    elseif ($item_obj != FALSE && @$item_obj->itemTypes->itemType != "ITEM") {
      $items = $item_obj->itemTypes->itemType;
      $item_array = array();
      foreach ($items as $item) {
        $item_array[$item] = $item;
      }
    }
    $node_types = node_type_get_types();
    $node_types_array = array();
    foreach ($node_types as $node_type_obj) {
      $node_types_array[$node_type_obj->type] = $node_type_obj->name;
      $form['tracking']['item_type_settings']['recommenderghost_mapping_' . $node_type_obj->type] = array(
        '#type' => 'select',
        '#title' => t('Select item type for @node_type', array('@node_type' => $node_type_obj->type)
        ),
        '#default_value' => variable_get('recommenderghost_mapping_' . $node_type_obj->type, 'ITEM'),
        '#options' => $item_array,
      );
    }
  }
  else {
    $form['tracking']['item_type_settings']['#description'] .= "<br/>" . t('You need to add your API key and Website ID before you can do any item mapping.');
  }

  // Render the images overview.
  $form['tracking']['images'] = array(
    '#type' => 'fieldset',
    '#title' => t('Images'),
  );
  $form['tracking']['images']['recommenderghost_image_token_field'] = array(
    '#type' => 'textfield',
    '#title' => 'Pattern for the image',
    '#default_value' => variable_get('recommenderghost_image_token_field',
                                       '[node:field_image], [node:field-product:0:field_images], [node:uc_product_image]'),
    '#description' => t('Enter the tokens of the image fields you want to use in your recommendations. You can use multiple tokens. Only the first one that is filled with an image is taken. See below for a complete list of tokens.'),
  );
  // Add the token help to a collapsed fieldset at the end of the config page.
  $form['tracking']['images']['token_help'] = array(
    '#type' => 'fieldset',
    '#title' => t('Available Tokens List'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );
  $form['tracking']['images']['token_help']['content'] = array(
    '#theme' => 'token_tree',
    '#token_types' => array('node'),
  );

  // Page specific visibility configurations.
  $php_access = user_access('use PHP for tracking visibility');
  $visibility = variable_get('recommenderghost_visibility_pages', 0);
  $pages = variable_get('recommenderghost_pages', RECOMMENDERGHOST_PAGES);

  $form['tracking']['page_vis_settings'] = array(
    '#type' => 'fieldset',
    '#title' => t('Pages'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );

  if ($visibility == 2 && !$php_access) {
    $form['tracking']['page_vis_settings'] = array();
    $form['tracking']['page_vis_settings']['visibility'] = array(
      '#type' => 'value',
      '#value' => 2);
    $form['tracking']['page_vis_settings']['pages'] = array(
      '#type' => 'value',
      '#value' => $pages);
  }
  else {
    $options = array(
      t('Every page except the listed pages'),
      t('The listed pages only'),
    );
    $description = t("Specify pages by using their paths. Enter one path per line. The '*' character is a wildcard. Example paths are %blog for the blog page and %blog-wildcard for every personal blog. %front is the front page.", array(
          '%blog' => 'blog',
          '%blog-wildcard' => 'blog/*',
          '%front' => '<front>'));

    if (module_exists('php') && $php_access) {
      $options[] = t('Pages on which this PHP code returns <code>TRUE</code> (experts only)');
      $title = t('Pages or PHP code');
      $description .= ' ' . t('If the PHP option is chosen, enter PHP code between %php. Note that executing incorrect PHP code can break your Drupal site.', array('%php' => '<?php ?>'));
    }
    else {
      $title = t('Pages');
    }
    $form['tracking']['page_vis_settings']['recommenderghost_visibility_pages'] = array(
      '#type' => 'radios',
      '#title' => t('Add tracking to specific pages'),
      '#options' => $options,
      '#default_value' => $visibility,
    );
    $form['tracking']['page_vis_settings']['recommenderghost_pages'] = array(
      '#type' => 'textarea',
      '#title' => $title,
      '#title_display' => 'invisible',
      '#default_value' => $pages,
      '#description' => $description,
      '#rows' => 10,
    );
  }

  // Render the role overview.
  $form['tracking']['role_vis_settings'] = array(
    '#type' => 'fieldset',
    '#title' => t('Roles'),
  );

  $form['tracking']['role_vis_settings']['recommenderghost_visibility_roles'] = array(
    '#type' => 'radios',
    '#title' => t('Add tracking for specific roles'),
    '#options' => array(
      t('Add to the selected roles only'),
      t('Add to every role except the selected ones'),
    ),
    '#default_value' => variable_get('recommenderghost_visibility_roles', 0),
  );

  $role_options = array_map('check_plain', user_roles());
  $form['tracking']['role_vis_settings']['recommenderghost_roles'] = array(
    '#type' => 'checkboxes',
    '#title' => t('Roles'),
    '#default_value' => variable_get('recommenderghost_roles', array()),
    '#options' => $role_options,
    '#description' => t('If none of the roles are selected, all users will be tracked. If a user has any of the roles checked, that user will be tracked (or excluded, depending on the setting above).'),
  );

  // Advanced feature configurations.
  $form['advanced'] = array(
    '#type' => 'fieldset',
    '#title' => t('Advanced settings'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );

  // Code snippet custom callback.
  $custom_drawback = "function drawCustomCallbackToDiv(json,recommenderDiv){\n" .
                                "if(\"undefined\"==typeof(json.error) ) { // if no error show recommendations\n" .
                                " try{\n" .
                                "      var items = json.recommendeditems.item; \n" .
                                " }catch(e){\n" .
                                "     return; //no recommendations found\n  }\n" .
                                " /* when the object is already in array format, this block will not execute */\n" .
                                " if( \"undefined\" == typeof(items.length) ) {\n" .
                                "     items = new Array(items);\n }\n" .
                                " // display recommendations in the DIV layer specified in the parameter recommenderDiv\n" .
                                " if (items.length>0) {\n" .
                                "     for (x=0;x<15 && x <items.length;x++) {\n" .
                                "           document.getElementById(recommenderDiv).innerHTML +=\n" .
                                "               \"<div style='width:170px;padding:5px;float:left;text-align:center;\'>\"+\n" .
                                "               \"<a href='\"               + items[x].url + \"'>\" +" .
                                "               \"<img style='width:150px;border:0px;' alt='\" + items[x].description + \"'\"+\n" .
                                "               \"     src='\"+ items[x].imageUrl + \"'/><br/>\"\n" .
                                "                + items[x].description +\n" .
                                "               \"</a>\" +\n" .
                                "               \"</div>\";\n" .
                                "     }\n  }\n }\n}\n";
  $form['advanced']['codesnippet'] = array(
    '#type' => 'fieldset',
    '#title' => t('Custom JavaScript code'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
    '#description' => t('You can customize the layout of the recommendation blocks by defining your own drawing callback function. <strong>This function must be called drawCustomCallbackToDiv</strong>. Below Javascript code will only be used if API method is set to Javascript and the below method is explicitly defined in the block settings. <strong>Do not include the &lt;script&gt; tags</strong>, and always end your code with a semicolon (;).'), array());
  $form['advanced']['codesnippet']['recommenderghost_drawingcallback'] = array(
    '#type' => 'textarea',
    '#title' => t('Drawing callback function'),
    '#default_value' => variable_get('recommenderghost_drawingcallback', $custom_drawback),
    '#rows' => 30,
    '#wysiwyg' => FALSE,
    '#description' => t("You can customize the layout of the JS recommendation blocks by defining your own drawing callback function. This function will only be used if defined in the block settings."),
  );

  // Scope of JS.
  $form['advanced']['recommenderghost_js_scope'] = array(
    '#type' => 'select',
    '#title' => t('JavaScript scope'),
    '#description' => t('RecommenderGhost recommends adding the external JavaScript files to the header for performance reasons.'),
    '#options' => array(
      'footer' => t('Footer'),
      'header' => t('Header'),
    ),
    '#default_value' => variable_get('recommenderghost_js_scope', 'header'),
    '#disabled' => (variable_get('recommenderghost_domain_mode', 0) == 2) ? TRUE : FALSE,
  );

  return system_settings_form($form);
}

/**
 * Implements _form_validate().
 */
function recommenderghost_admin_settings_form_validate($form, &$form_state) {

  // Trim some text values.
  $form_state['values']['recommenderghost_apikey'] = trim($form_state['values']['recommenderghost_apikey']);
  $form_state['values']['recommenderghost_websiteid'] = trim($form_state['values']['recommenderghost_websiteid']);
  $form_state['values']['recommenderghost_pages'] = trim($form_state['values']['recommenderghost_pages']);
  $form_state['values']['recommenderghost_drawingcallback'] = trim($form_state['values']['recommenderghost_drawingcallback']);
  $form_state['values']['recommenderghost_image_token_field'] = trim($form_state['values']['recommenderghost_image_token_field']);

  if (preg_match('/(.*)<\/?script(.*)>(.*)/i', $form_state['values']['recommenderghost_drawingcallback'])) {
    form_set_error('recommenderghost_drawingcallback', t('Do not include the &lt;script&gt; tags in the javascript code snippet.'));
  }
  // Check API key and website ID.
  $item = recommenderghost_call_api("itemtypes", array(), $form_state['values']['recommenderghost_apikey'], $form_state['values']['recommenderghost_websiteid']);
  if (isset($item->error)) {
    form_set_error('recommenderghost_apikey', t('Wrong API key / Website ID combination!'));
  }
}

/**
 * Validate a form element that should have tokens in it.
 *
 * For example:
 * @code
 * $form['my_node_text_element'] = array(
 *   '#type' => 'textfield',
 *   '#title' => t('Some text to token-ize that has a node context.'),
 *   '#default_value' => 'The title of this node is [node:title].',
 *   '#element_validate' => array('recommenderghost_token_element_validate'),
 * );
 * @endcode
 */
function recommenderghost_token_element_validate(&$element, &$form_state) {
  $value = isset($element['#value']) ? $element['#value'] : $element['#default_value'];

  if (!drupal_strlen($value)) {
    // Empty value needs no further validation since the element should depend
    // on using the '#required' FAPI property.
    return $element;
  }

  $tokens = token_scan($value);
  $invalid_tokens = _recommenderghost_get_forbidden_tokens($tokens);
  if ($invalid_tokens) {
    form_error($element, t('The %element-title is using the following forbidden tokens with personal identifying information: @invalid-tokens.',
            array(
              '%element-title' => $element['#title'],
              '@invalid-tokens' => implode(', ', $invalid_tokens))));
  }

  return $element;
}

/**
 * Adds some privacy by filtering "forbidden" tokens.
 */
function _recommenderghost_get_forbidden_tokens($value) {
  $invalid_tokens = array();
  $value_tokens = is_string($value) ? token_scan($value) : $value;

  foreach ($value_tokens as $type => $tokens) {
    if (array_filter($tokens, '_recommenderghost_contains_forbidden_token')) {
      $invalid_tokens = array_merge($invalid_tokens, array_values($tokens));
    }
  }

  array_unique($invalid_tokens);
  return $invalid_tokens;
}

/**
 * Validate if a string contains forbidden tokens not allowed by privacy rules.
 *
 * @param string $token_string
 *   A string with one or more tokens to be validated.
 *
 * @return bool
 *   TRUE if blacklisted token has been found, otherwise FALSE.
 */
function _recommenderghost_contains_forbidden_token($token_string) {
  // List of strings in tokens with personal identifying information not allowed
  // for privacy reasons.
  //
  // This list can never ever be complete. For this reason it tries to use a
  // regex and may kill a few other valid tokens, but it's the only way to
  // protect users as much as possible from admins with illegal ideas.
  //
  // User tokens are not prefixed with colon to catch 'current-user' and 'user'.
  //
  // This list is comparably short as a recommendation engine can use user ids
  // to calculate recommendations. The collected data cannot be exported
  // nor analyzed.
  $token_blacklist = array(
    ':one-time-login-url]',
    // addressfield_tokens.module
    ':first-name]',
    ':last-name]',
    ':name-line]',
    // realname.module
    ':name-raw]',
    // token.module
    ':ip-address]',
  );

  return preg_match('/' . implode('|', array_map('preg_quote', $token_blacklist)) . '/i', $token_string);
}

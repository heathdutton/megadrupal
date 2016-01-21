<?php
/**
 * @file
 * Page to redirecting to LimeSurvey answer fill page.
 */

/**
 * Page making the LimeSurvey available, as an iframe, or a link.
 *
 * If the answer node is not provided, create a link for a new answer node. The
 * form button is handle by the 'toform' page.
 *
 * @param $type
 *   A string. The answer content type.
 * @param $node
 *   An answer node object or FALSE if not provided
 *
 * @return
 *   A string. The url to fill the survey.
 */
function ls_answ_fill_survey($type, $node = FALSE) {
  // $type has been checked into the ls_answ_fill_survey_access() function.

  // Set the correct display mode.
  $display_mode = ls_answ_fix_display_mode($type);

  // Set the node.
  if (empty($node)) {
    module_load_include('inc', 'ls_answ', 'ls_answ');
    $node = ls_answ_new_node($type);
  }
  // Load LS survey properties
  module_load_include('inc', 'limesurvey_sync', 'ls_api/limesurvey_sync_api_survey');
  $survey_properties = limesurvey_sync_survey_properties($node->ls_sid);

  return ls_answ_linktosurvey($node, $display_mode);
}

/**
 * Manage link to the limeSurvey website for completing or editing an answer.
 *
 * This submit handler is called after the node saving process : it manage
 * http redirection if the user wants to modify the answers on the LimeSurvey
 * website. Options can alter redirection.
 *
 * @param $node
 *   An object. The answer node.
 * @param $display_mode
 *   A string. Informs how the LimeSurvey website will be displayed. Available
 *   values are : 'form', 'redirect', 'iframe', 'colorbox', 'lightbox2',
 *   'modalframe', 'link'. If not provided, retireve the default display mode
 *   from the $node->type value.
 *
 * @return
 *   A string. The html code for displaying the LimeSurvey answer editing form.
 */
function ls_answ_linktosurvey($node, $display_mode = '') {
  $output = '';
  module_load_include('inc', 'ls_survey', 'ls_survey.type');
  $default_options = ls_survey_answ_default_options();
  $options = variable_get('ls_answ_options_' . $node->type, $default_options);
  if (empty($display_mode)) {
    $display_mode = ls_answ_fix_display_mode($node->type);
  }
  module_load_include('inc', 'limesurvey_sync', 'ls_api/limesurvey_sync_api_survey');
  $survey_properties = limesurvey_sync_survey_properties($node->ls_sid);

  if (empty($node->nid) && $survey_properties['tokens_table']) {
    // Create an invitation on the LS database but don't save it on Drupal.
    module_load_include('inc', 'limesurvey_sync', 'ls_api/limesurvey_sync_api_answ');
    $node->ls_token = limesurvey_sync_answ_set_invit($node->type, $node->ls_sid, $node->ls_lang, '', '');
  }

  module_load_include('inc', 'limesurvey_sync', 'ls_api/limesurvey_sync_api_answ');
  // Absolute url.
  $survey_url = limesurvey_sync_answ_surveyurl($node, TRUE);

  switch ($display_mode) {

    case 'form':
      $formated_nid = (!empty($node->nid)) ? '/' . $node->nid : '';
      drupal_goto('toform/' . $node->type . $formated_nid);
      break;

    case 'redirect':
      drupal_goto($survey_url);
      break;

    case 'colorbox':
      // Below only for debug purpose.
      $extra_query = array(
        'width' => $options['iframe_dim_width'],
        'height' => $options['iframe_dim_height'],
        'iframe' => 'true',
      );
      $attributes = array('attributes' => array(
          'id' => 'ls_lightframe',
          'class' => 'colorbox-load',
        ));
      $cb_survey_url = limesurvey_sync_answ_surveyurl($node, TRUE, $extra_query);
      // No break instruction !

    case 'lightbox2':
      // The lightbox2 custom module must be enabled.
      $attributes = array('attributes' => array(
          'rel' => 'lightframe[|width:200px;height:100px;]', //["' . $node->title . '"]',
          'id' => 'ls_lightframe',
        ));
      // No break instruction !

    case 'iframe':
    case 'modalframe':
      // This is the parent windows of the iframe.
      $custom_end_url = (!empty($options['ls_end_url'])) ? $options['ls_end_url'] : '<front>';
      // Skip the page loading the fresh answer if not needed.
      $end_url = ($options['ls_auto_import']) ? 'backfromsurvey/' . $node->type : $custom_end_url;
      $end_url = url($end_url, array('absolute' => TRUE));

      $alt_options = $options;
      // overwrite the display_mode in order to reflect the
      // ls_answ_fix_display_mode() function result.
      $alt_options['ls_redirect'] = $display_mode;
      ls_answ_load_js_parent_page($alt_options, $end_url, $survey_url);
      if (!empty($node->nid)) {
        ls_answ_delete_success_message($node);
      }
      $output .= theme('ls_answ_iframe_end.tpl', array('text' => l(t('Redirection in progress...'), $end_url)));
      // Do not print the iframe tag if display mode is colorbox, lightbox2 or
      // modalframe. The js function is set to not fail if the
      // 'ls_answ_iframe_container'  div id does not exists.
      if ($display_mode == 'iframe') {
        // Here, we can edit the iframe width and height properties.
        // Parameters may be overwritten by the ls_answ.iframe.js JS functions.
        $height = ($options['iframe_dim'] == 'auto' || $options['iframe_dim'] == 'invisible' || ($options['iframe_dim'] == 'custom' && empty($options['iframe_dim_height']))) ? $default_options['iframe_dim_height'] : $options['iframe_dim_height'];
        $width = ($options['iframe_dim'] == 'auto' || $options['iframe_dim'] == 'invisible' || ($options['iframe_dim'] == 'custom' && empty($options['iframe_dim_width']))) ? '100%' : $options['iframe_dim_width'];
        // Delete 'px'
        $height = str_replace('px', '', $height);
        $width = str_replace('px', '', $width);
        // Display the iframe.
        $output = theme('ls_answ_iframe.tpl', array('url' => $survey_url, 'width' => $width, 'height' => $height)) . $output;
      }
      return $output;
      break;

    case 'link':
    default:
      // Print a link to fill the form.
      module_load_include('inc', 'limesurvey_sync', 'ls_api/limesurvey_sync_api_answ');
      $output = '<h2 class="ls_answ_link_submit">';
      $output .= l(t('Go to the survey'), $survey_url);
      $output .= '</h2>';
      return $output;
  }
}

/**
 * Fire a JS fonction asking to the parent page to close the current child page.
 *
 * This page is going to be close thanks to its parent page.
 *
 * @param $type
 *   An string. The answer content type.
 *
 * @return
 *   A string. A waiting message.
 */
function ls_answ_close_frames($type) {
  // Set the correct display mode.
  $display_mode = ls_answ_fix_display_mode($type);

  // Load the associated survey node.
  module_load_include('inc', 'ls_answ', 'ls_answ');
  $node_survey = ls_answ_find_survey($type);

  // Insert javascrip stuff.
  $iframes = array('iframe', 'colorbox', 'lightbox2', 'modalframe');
  if (in_array($display_mode, $iframes)) {
    // This is the child iframe, asking to its parent iframe (form/%/%node) to
    // disapear and to redirect to the drupal correct page
    // (backfromsurvey/%type).
    ls_answ_load_js_child_page($display_mode);
    // The js will take care of the redirection.
    return t('Redirection in progress...');
  }
}

/**
 * Load javascrip on the parent page when iframe is used to fill the survey.
 *
 * This function may auto open the frame displaying the LimeSurvey site on the
 * answer filling page, and prepare javascript to automatically close the
 * iframe and to redirect the user to a specific Drupal page after the answer
 * has been completed.
 *
 * @param $options
 *   An array, provide the display mode value (the 'ls_redirect' key),
 *   availables values are 'iframe', 'colorbox', 'modalframe', 'lightbox2'. It
 *   provides to the dimensions of the frame.
 * @param $redirection_url
 *   A string. The url redirect after the survey has been completed. It may be
 *   the anser node view page.
 * @param $survey_url
 *   A string. LimeSurvey form url.
 * @param $redirection_timing
 *   A numeric. The duration before redirecting back to drupal. Default is
 *   100ms (=0.1s)
 */
function ls_answ_load_js_parent_page($options, $redirection_url, $survey_url = '', $redirection_timing = 100) {
  $display_mode = $options['ls_redirect'];
  $autoload_functions = array();
  $js_domain = $js_onresize = $js_custom = '';

  $js_redirect = "\nfunction ls_answ_answer_completed() {";
  $js_redirect .= "\n  if (document.getElementById('ls_answ_iframe_container')) {";
  $js_redirect .= "\n    document.getElementById('ls_answ_iframe_container').style.display='none';";
  $js_redirect .= "\n  }";
  $js_redirect .= "\n  window.document.getElementById('ls_form_end').style.display='inline';";
  $js_redirect .= "\n  setTimeout(\"window.location='$redirection_url'\", $redirection_timing);";
  $js_redirect .= "\n};";


  switch ($display_mode) {
    case 'iframe':
      // Js code to improve domain conflicts.
      $js_domain = ls_answ_js_host();
      // Once the survey is finished : the frame disapears and the js
      // redirection beguins within 100ms.
      // From http://forums.digitalpoint.com/showthread.php?t=582469#post5546667.
      // Automatic resize the frame, from
      // http://www.dyn-web.com/tutorials/iframes/fluid.html
      // @TODO : for cross-domain iframe resizing, I may use this instead :
      // http://consumer.easyxdm.net/current/example/resize_iframe.html
      $add_js_file = FALSE;
      if ($options['iframe_dim'] == 'invisible') {
        // Delay the JS in order to let the time to the LimeSurvey page to load
        // and set a correct height for the child page.
        // Note : 2s seems to be to short, the frame does not appear
        // (height = 0). As a workaround, call several times the function (at
        // 2s, 4s, 6s and 8s).
        $autoload_functions[] = "setTimeout(\"ls_answ_ContentHeightSetIframeHeight('ls_answ_iframe')\", 2000);";
        $autoload_functions[] = "setTimeout(\"ls_answ_ContentHeightSetIframeHeight('ls_answ_iframe')\", 4000);";
        $autoload_functions[] = "setTimeout(\"ls_answ_ContentHeightSetIframeHeight('ls_answ_iframe')\", 6000);";
        $autoload_functions[] = "setTimeout(\"ls_answ_ContentHeightSetIframeHeight('ls_answ_iframe')\", 8000);";
        $add_js_file = TRUE;
      }
      elseif ($options['iframe_dim'] == 'auto' || $options['iframe_dim_height'] == 0 ) {
        // Delay the JS in order to let the time to the LimeSurvey page to load
        // and set a correct height for the parent page. Delay set to 2s, 4s,
        // 6s and 8s.
        $autoload_functions[] = "setTimeout(\"ls_answ_fluidSetIframeHeight('ls_answ_iframe', .8)\", 2000);";
        $autoload_functions[] = "setTimeout(\"ls_answ_fluidSetIframeHeight('ls_answ_iframe', .8)\", 4000);";
        $autoload_functions[] = "setTimeout(\"ls_answ_fluidSetIframeHeight('ls_answ_iframe', .8)\", 6000);";
        $autoload_functions[] = "setTimeout(\"ls_answ_fluidSetIframeHeight('ls_answ_iframe', .8)\", 8000);";
        $js_onresize = ls_answ_fix_jquery("$(window).resize(function() { ls_answ_fluidSetIframeHeight('ls_answ_iframe', .8);});");
        $add_js_file = TRUE;
      }
      // Else : $options['iframe_dim'] == 'custom' : Do nothing.
      if ($add_js_file) {
        // preprocess is FALSE.
        $js_options = array(
          'type' => 'file',
          'group' => JS_DEFAULT,
          'preprocess' => FALSE,
        );
        drupal_add_js(file_create_url(drupal_get_path('module', 'ls_answ') . '/ls_answ.iframe.js'), $js_options);
      }
      break;

    case 'colorbox':
      $height = ($options['iframe_dim_height'] == 0 || $options['iframe_dim'] == 'auto') ? '85%' : $options['iframe_dim_height'];
      $width = ($options['iframe_dim_width'] == 0 || $options['iframe_dim'] == 'auto') ? '85%' : $options['iframe_dim_width'];
      $autoload_functions[] = "$.fn.colorbox({width:'$width', height:'$height', iframe:true, href:'$survey_url'});";
      // onClosed: ls_answ_answer_completed() -> is executed on the frame load !?
      break;

    case 'lightbox2':
      if ($options['iframe_dim'] == 'auto' || ($options['iframe_dim_width'] == 0 && $options['iframe_dim_height'] == 0)) {
        $dim = '';
      }
      else {
        $height = ($options['iframe_dim_height'] == 0) ? '' : 'height:' . $options['iframe_dim_height'] . ';';
        $width = ($options['iframe_dim_width'] == 0) ? '' : 'width:' . $options['iframe_dim_width'] . ';';
        $dim = "[|$width$height]";
      }
      $js_custom = "\nfunction ls_answ_load_lightbox2() {";
      $js_custom .=  $add_var;
      $js_custom .=  "\n  var lsOptions = {";
      $js_custom .=  "\n    href: '$survey_url',";
      $js_custom .=  "\n    title: '',";
      //$js_custom .=  "\n    height: '$height',"; // Ignored !
      //$js_custom .=  "\n    width: $width,"; // Ignored !
      $js_custom .=  "\n    rel: 'lightframe$dim',";
      $js_custom .=  "\n    id: '',";
      // Disallow to close the frame ! The user must complete the survey using
      // the LimeSurvey form buttons.
      // @TODO : comment line below, does not work.
      // $js_custom .=  "\n    disableCloseClick: true,";
      $js_custom .=  "\n  };";
      $js_custom .=  "\n  return lsOptions;";
      $js_custom .=  "\n};";
      $autoload_functions[] = 'Lightbox.start(ls_answ_load_lightbox2(), false, true, false, false);';
      break;

    case 'modalframe':
      // help : http://drupal.org/node/700752.
      // Send the Modal Frame javascript for parent windows to the page.
      // The modalframe js need to provide the width and height values as px
      // (not %) and numeric (not string).
      if ($options['iframe_dim'] == 'auto' || ($options['iframe_dim_height'] == 0 && $options['iframe_dim_width'] == 0)) {
        $width_js = 'autoFit: true';
        $height_js = '';
      }
      else {
        preg_match('#(^[0-9]{1,4})[ ]?(px)?$#', $options['iframe_dim_height'], $match_height);
        $height = $match_height[1]; // Eliminate the 'px'.
        preg_match('#(^[0-9]{1,4}\.?[0-9]{0,1})[ ]?(%|px)?$#', $options['iframe_dim_width'], $match_width);
        $width = $match_width[1]; // Eliminate the 'px'.
        $height_js = ($options['iframe_dim_height'] == 0) ? '' : "\n    height: $height";
        $width_js = ($options['iframe_dim_width'] == 0) ? '' : "\n    width: $width";
      }
      modalframe_parent_js();
      $js_custom =  "\nfunction ls_answ_load_modalframe() {";
      $js_custom .=  "\n  var lsOptions = {";
      $js_custom .=  "\n    url: '" . $survey_url . "',";
      $js_custom .=  $width_js;
      $js_custom .=  $height_js;
      // If the user manually close the frame before finsihing to fill the
      // survey, go to the end page.
      // Below : bug : onSubmit is executed on opening the frame ! (close the
      // frame immediatly after opening it) : I disable it.
      // $js_custom .=  "\n    onSubmit: ls_answ_answer_completed(),";
      $js_custom .=  "\n  };";
      $js_custom .=  "\n  Drupal.modalFrame.open(lsOptions);";
      $js_custom .=  "\n };";
      $autoload_functions[] = 'ls_answ_load_modalframe()';
      break;
  }

  // Autoload functions.
  $on_load = ls_answ_js_autoload_prepare($autoload_functions);

  // Load JS in the correct order.
  $js = array_diff(array($js_domain, $js_redirect, $js_custom, $on_load, $js_onresize), array());
  drupal_add_js(implode("\n", $js), array('type' => 'inline', 'scope' => 'header', 'group' => JS_DEFAULT, 'weight' => 5));
}

/**
 * Load javascrip on the child page when iframe is used to fill the survey.
 *
 * This function is loaded  after the answer has been completed on the
 * LimeSurvey site from an iframe. It asks to the parent page to close the
 * frame and to redirect the user to a specific page.
 *
 * @param $display_mode
 *   A string. Available values are 'iframe', 'colorbox', 'modalframe',
 *   'lightbox2'. Default is NULL : all js functions are loaded but they will be
 *   fired or not depending on the parent page, which is correctly set.
 */
function ls_answ_load_js_child_page($display_mode = NULL) {
  $js_domain = ls_answ_js_host();
  $autoload_functions = array();
  // This is the child iframe, asking to its parent iframe (form/%type/%node)
  // to disapear and to redirect to the drupal correct page (backfromsurvey/%type).
  $autoload_functions[] = 'window.parent.ls_answ_answer_completed();';

  // If display mode is unknown, call all close frame functions.
  if (module_exists('colorbox') && ($display_mode == 'colorbox' || empty($display_mode))) {
    $autoload_functions[] = 'parent.jQuery.fn.colorbox.close();';
  }
  if (module_exists('lightbox2') && ($display_mode == 'lightbox2' || empty($display_mode))) {
    $autoload_functions[] = 'window.parent.Lightbox.end();';
  }
  if (module_exists('modalframe') && ($display_mode == 'modalframe' || empty($display_mode))) {
    // Send the Modal Frame javascript for child windows to the page.
    modalframe_child_js();
    // Tell the parent window to close the modal frame dialog.
    modalframe_close_dialog();
  }

  // Autoload functions
  $on_load = ls_answ_js_autoload_prepare($autoload_functions);

  // Load JS in the correct order.
  $js = array_diff(array($js_domain, $on_load), array());
  drupal_add_js(implode("\n", $js), array('type' => 'inline', 'scope' => 'header', 'group' => JS_DEFAULT, 'weight' => 5));
}

/**
 * Autoload javascript functions on opening page.
 *
 * Load a javascript or a jquery script on loading the page.
 *
 * @param $autoload_functions
 *   An array. Values are names of the javascrip functions to load
 *   automatically when the page opening.
 *
 * @return
 *   A string. The javascript to be executed on page load.
 */
function ls_answ_js_autoload_prepare($autoload_functions = array()) {
  if (empty($autoload_functions)) {
    return '';
  }
  $js_custom = "\n  $(document).ready(function(){";
  foreach ($autoload_functions as $function) {
    $js_custom .= "\n    " . $function;
  }
  $js_custom .= "\n  });";
  $js_custom = ls_answ_fix_jquery($js_custom);
  return $js_custom;
}

/**
 * Provide the servor domain name.
 *
 * Used to improve javascript communication between the Drupal site and an
 * iframe. Unfortunally, it seems to work only on localhost, this piece of js
 * code can produce the error: 'Illegal document.domain value' code: '1009'.
 * That why this function is disabled by default. Help on :
 * http://www.leggetter.co.uk/2010/03/12/making-cross-domain-javascript-
 * requests-using-xmlhttprequest-or-xdomainrequest.html
 *
 * @param $enable
 *   A boolean. If TRUE : send the server domain name. Default is FALSE.
 *
 * @return
 *   A string. The servor domain name.
 */
function ls_answ_js_host($enable = FALSE) {
  if (!$enable) {
    return '';
  }
  $host = $_SERVER['HTTP_HOST'];
  if (strpos($host, '.') !== FALSE) {
    // For 'www.mysite1.example.com' and 'www.mysite2.example.com':
    $darray = explode('.', $_SERVER['HTTP_HOST']);
    $narray = array_reverse($darray);
    // Return 'example'.
    $short_host = $narray[1];
  }
  else {
    $short_host = $host;
  }
  return "\ndocument.domain = '" . $short_host . "';";
}

/**
 * Delete the success message when editing or creating a node for esthetic raison.
 *
 * When an answer node is create or updated and the user is redirect on the
 * iframe, the success message is displayed but the user has not fill the
 * survey yet! Not so sexy, we delete it.
 *
 * @param $node
 *   An answer node object.
 */
function ls_answ_delete_success_message($node) {
  if (empty($node) || !is_object($node)) {
    return;
  }
  $t_args = array(
    '@type' => node_type_get_name($node),
    '%title' => $node->title,
  );
  $succesfull_insert = array('status' => array(t('@type %title has been created.', $t_args)));
  $succesfull_update = array('status' => array(t('@type %title has been updated.', $t_args)));

  $messages = drupal_get_messages();
  if (in_array($messages, array($succesfull_insert, $succesfull_update))) {
    drupal_get_messages(NULL, TRUE);
  }
}

/**
 * Make sure the display mode is correct.
 *
 * Usefull if some custom modules have been disabled but the options variable
 * has not been updated to reflect thoses changes.

 * @param $type
 *   A string. The answer content type machine name.
 *
 * @return
 *   A string, the correct display mode.
 */
function ls_answ_fix_display_mode($type) {
  module_load_include('inc', 'ls_survey', 'ls_survey.type');
  $default_options = ls_survey_answ_default_options();
  $options = variable_get('ls_answ_options_' . $type, $default_options);
  $display_mode = $options['ls_redirect'];
  if (empty($display_mode) || ($display_mode == 'lightbox2' && !module_exists('lightbox2')) || ($display_mode == 'colorbox' && !module_exists('colorbox')) || ($display_mode == 'modalframe' && !module_exists('modalframe'))) {
    $display_mode = $default_options['ls_redirect']; // Default display mode.
  }
  return $display_mode;
}

/**
 * Make sure to not rely on $() being the jQuery function.
 *
 * @param $inline_js
 *   A string. JS code to add.
 *
 * @return
 *   A string, the wrapped javascript code.
 */
function ls_answ_fix_jquery($inline_js) {
  if (!empty($inline_js)) {
    $start = "\n" . '// start jQuery block' . "\n" . '(function ($) {';
    $end = "\n" . '}(jQuery));' . "\n" . '//End jQuery block';
    return $start . $inline_js . $end;
  }
  return '';
}

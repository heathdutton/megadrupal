<?php

/**
 * Use:
 * - create hook_menu() entry for a link
 * - display link, ensuring to call drupal_add_library('dialog', 'dialog'), and adding the
 *   'use-dialog' and 'use-ajax' classes to the link
 * - in your link callback, call dialog_show_form();
 * - in your form's FAPI declaration, call dialog_form_prepare();
 */

/**
 * Processes and renders the form in a dialog.
 * @param  [type] $form_id ID of the form to load.
 */
function dialog_show_form($form_id) {
  list($handler, $handler_data) = devtools_func_handler(__FUNCTION__, func_get_args());

  if ($handler) {
    return $handler_data;
  }
  else {
    // remove $form_id from the arguments.
      $args = func_get_args();
      array_shift($args);

    // save the arguments & dialog options for future use
      $form_state = array();
      $form_state['build_info']['args'] = $args;

    // build the form
      $form = drupal_build_form($form_id, $form_state);

    // set dialog options
      $dialog_options = array();
      if (isset($form_state['dialog_settings']['states'][$form_state['dialog_settings']['current_state']]['dialog'])) {
        $dialog_options = $form_state['dialog_settings']['states'][$form_state['dialog_settings']['current_state']]['dialog'];
      }

    // set dialog class
      $class = array();
      if (isset($dialog_options['dialogClass'])) {
        $class = explode(' ', $dialog_options['dialogClass']);
      }

      $stem = 'dialog';
      foreach (array($form_id, $args[1], $args[2]) as $part) {
        $stem .= '_' . $part;
        $class[] = $stem;
      }

      $dialog_options['dialogClass'] = implode(' ', $class);

    // return the form
      $commands = array();
      $commands[] = dialog_command_display(drupal_render($form), $dialog_options);
    
    return $commands;
  }
}

/**
 * Prepares the form for AJAX submissions within a dialog.
 * @param  [type] $form       FAPI array.
 * @param  [type] $form_state Current form state.
 */
function dialog_form_prepare(&$form, &$form_state) {
  list($handler, $handler_data) = devtools_func_handler(__FUNCTION__, func_get_args());

  if ($handler) {
    return $handler_data;
  }
  else {
    // remove form elements which are not part of the current state
      $settings = &$form_state['dialog_settings'];
      $state_class = '';
      if (isset($settings['current_state'])) {
        $state_class = 'state-' . $settings['current_state'];

        foreach ($form as $key => $item) {
          if ($key{0} != '#' && strpos($key, 'state_') === 0 && $key != 'state_' . $settings['current_state']) {
            unset($form[$key]);
          }
        }

        // does the current state ask for a close link?
          if (isset($settings['states'][$settings['current_state']]['close_link']) && $settings['states'][$settings['current_state']]['close_link']) {
            $close_link = '';
            dialog_add_close_link($close_link, $settings, TRUE);

            if (!empty($close_link)) {
              $form['state_' . $settings['current_state']]['close_link'] = array(
                '#markup' => $close_link,
              );
            }
          }
      }

    // wrap the form in a div which we can target for HTML replacement
      $wrapper_id = 'dialog-' . $form_state['build_info']['form_id'] . '-wrapper';

      // set prefix
        $new_prefix = '<div id="' . $wrapper_id . '" class="dialog-content-wrapper ' . $state_class . '">';
        if (isset($form['#prefix'])) {
          $new_prefix .= $form['#prefix'];
        }
        $form['#prefix'] = $new_prefix;

      // set suffix
        $new_suffix = '</div>';
        if (isset($form['#suffix'])) {
          $new_suffix = $form['#suffix'] . $new_suffix;
        }
        $form['#suffix'] = $new_suffix;

    // set AJAX requests on submit elements
      foreach ($form_state['dialog_settings']['states'] as $state_name => &$state) {
        if (isset($state['submit'])) {
          foreach ($state['submit'] as &$submit_item) {
            // the submit element key is a reference to the appropriate element in the form, so 
            // these changes actually set the AJAX data in the appropriate location within
            // the $form array
            $submit_item['element']['#ajax']['callback'] = 'dialog_form_submit';
            $submit_item['element']['#ajax']['wrapper'] = $wrapper_id;
          }
        }
      }
  }
}

/**
 * Processes the AJAX form submission within dialog forms.
 * @param  [type] $form       [description]
 * @param  [type] $form_state [description]
 * @return [type]             [description]
 */
function dialog_form_submit($form, &$form_state) {
  list($handler, $handler_data) = devtools_func_handler(__FUNCTION__, func_get_args());

  if ($handler) {
    return $handler_data;
  }
  else {  
    // check for validation errors
      if (form_get_errors()) {
        dialog_form_prepare($form, $form_state);
        return $form;
      }

    // start building the ajax commands
      $commands = array();

    // do basic prep
      $settings = $form_state['dialog_settings'];
      $current_state = $settings['current_state'];

    // do we need to redirect the user on state submission?
      if (isset($settings['states'][$current_state]['submit_redirect'])) {
        $redirect_js =  '
                          <script>
                            window.location.replace("' . $settings['states'][$current_state]['submit_redirect'] . '");
                          </script>
                        ';
        $commands[] = ajax_command_append('body', $redirect_js);
      }
    // no redirects necessary, so process the state settings normally
      else {
        // do we need to update the trigger?
          if (isset($settings['states'][$current_state]['trigger_callback']) && function_exists($settings['states'][$current_state]['trigger_callback'])) {
            $trigger_update = '';

            if (isset($settings['states'][$current_state]['trigger_args'])) {
              $trigger_update = call_user_func_array($settings['states'][$current_state]['trigger_callback'], $settings['states'][$current_state]['trigger_args']);
            }
            else {
              $trigger_update = $settings['states'][$current_state]['trigger_callback']();
            }

            $commands[] = ajax_command_replace('#' . $settings['trigger_id'], $trigger_update);
          }

        // find the triggering ('submit') element and check to see if there's a next state
          $trigger_parents_key = implode('/', $form_state['triggering_element']['#array_parents']);

          if (isset($settings['states'][$current_state]['submit'])) {
            foreach ($settings['states'][$current_state]['submit'] as $submit_item) {
              $item_parents_key = implode('/', $submit_item['element']['#array_parents']);
              
              if (($trigger_parents_key == $item_parents_key) && isset($submit_item['next_state'])) {
                // update the current state
                  $form_state['dialog_settings']['current_state'] = $submit_item['next_state'];
                  $form_state['build_info']['args'][0] = $submit_item['next_state'];
              }
            }
          } 
        
        // start building HTML output with the status messages
          $output = theme('status_messages');

        // close the dialog as it will be rebuilt with the new state
          $commands[] = dialog_command_dismiss();

        // check for a javascript callback when the dialog is submitted
          if (isset($settings['states'][$current_state]['submit_js_callback'])) {
            $callback = $settings['states'][$current_state]['submit_js_callback'];

            // data being passed to the callback is optional
            if (!isset($callback['data'])) {
              $callback['data'] = array();
            }

            $commands[] = ajax_command_callback($callback['behavior'], $callback['method'], $callback['data']);
          }

        // if autoclose is enabled we leave the dialog closed
          if (!(isset($settings['states'][$current_state]['submit_autoclose']) && $settings['states'][$current_state]['submit_autoclose'])) {
            // rebuild the form & render output
              $form = drupal_rebuild_form($form_state['build_info']['form_id'], $form_state, $form);
              $output .= drupal_render($form);

            // build the AJAX commands
              $commands[] = dialog_command_display($output, $form_state['dialog_settings']['states'][$form_state['dialog_settings']['current_state']]['dialog']);
          }
      }

    return array('#type' => 'ajax', '#commands' => $commands);
  }
}

/**
 * Helper to consistently wrap content in the same class.
 * TODO: move to a theme function?
 * @param  [type] $content          [description]
 * @param  string $additional_class [description]
 * @return [type]                   [description]
 */
function dialog_wrap_content($content, $additional_class = '') {
  list($handler, $handler_data) = devtools_func_handler(__FUNCTION__, func_get_args());

  if ($handler) {
    return $handler_data;
  }
  else {  
    return '<div class="devtools-dialog-content ' . $additional_class . '">' . $content . '</div>';
  }
}

/**
 * Helper for creating a link to close the dialog.
 * @param  [type] $output   Current output content.
 * @param  [type] $settings Dialog configuration settings.
 * @param  boolean $force   [description]
 * @return [type]           [description]
 */
function dialog_add_close_link(&$output, $settings, $force = FALSE) {
  $state = $settings['states'][$settings['current_state']];

  if ((!empty($output) || $force) && (!isset($state['close_link']) || $state['close_link'])) {
    $text = t('Close');

    // check for custom text 
    if (isset($state['close_link_text']) && !empty($state['close_link_text'])) {
      $text = $state['close_link_text'];
    }

    $class = array(
      'devtools-dialog-close',
    );

    // check for custom classes 
    if (isset($state['close_link_class']) && !empty($state['close_link_class'])) {
      $class = array_merge($class, $state['close_link_class']);
    }

    $link = l($text, '', array(
      'attributes' => array(
        'onclick' => 'Drupal.dialog.dialog("close"); return false;',
        'class' => $class,
      ),
    ));

    $output .= dialog_wrap_content($link, 'close-dialog');
  }
}


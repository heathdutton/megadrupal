<?php
/**
 * A handler to provide files or folders management toolbox.
 *
 * @ingroup views_field_handlers
 */
class oa_folders_handler_field_folder_toolbox extends views_handler_field {

  function allow_advanced_render() {
    return FALSE;
  }

  function query() {
  }

  function option_definition() {
    $options = parent::option_definition();
    return $options;
  }

  function options_form(&$form, &$form_state) {
    parent::options_form($form, $form_state);
    unset($form['alter']);
    unset($form['empty']);
    unset($form['hide_empty']);
    unset($form['empty_zero']);
  }

  function render($values) {
    static $done = FALSE;

    $output = '';
    $node = NULL;
    $nid  = NULL;
    $fid  = NULL;
    $type = 'folder';
    if (isset($values->nid)) {
      $nid = $values->nid;
      $node = node_load($values->nid);
    }
    else {
      $node = menu_get_object('node');
      $nid = $node->nid;
    }
    if (isset($values->file_managed_field_data_field_oa_media_fid)) {
      $fid = $values->file_managed_field_data_field_oa_media_fid;
      $type = 'file';
    }

    if ($node && $node->type == 'oa_folder') {
      foreach (oa_folders_toolbox_actions() as $toolbox) {
        if (isset($toolbox['access callback']) && is_callable($toolbox['access callback'])) {
          $callback = $toolbox['access callback'];
          $access = $callback($toolbox, $node, $fid);
        }
        else {
          $access_op = (isset($toolbox['node access']) ? $toolbox['node access'] : 'update');
          $access = node_access($access_op, $node);
        }
        if ($access) {
          if (!$done) {
            ctools_include('ajax');
            ctools_include('modal');
            ctools_modal_add_js();
            $done = TRUE;
          }

          if ($toolbox['type'] == $type) {
            $string = 'oa_folders/toolbox/!op/!nid/!fid/nojs';
            if (isset($toolbox['path'])) {
              $string = $toolbox['path'];
            }
            $path = strtr($string, array('!op' => $toolbox['op'], '!nid' => $nid, '!fid' => $fid));
            $options = array();
            if (!isset($toolbox['attributes'])) {
              $options['attributes'] = array('class' => 'ctools-use-modal');
            }
            else {
              $options['attributes'] = $toolbox['attributes'];
            }
            if (isset($toolbox['verify']) && $toolbox['verify']) {
              if (!drupal_valid_path($path)) {
                continue;
              }
            }
            
            if (isset($toolbox['glyphicon'])) {
              $options['html'] = TRUE;
              $output .= l('<i class="glyphicon ' . $toolbox['glyphicon'] . '" title="' . $toolbox['title'] . '"></i> ', $path, $options);
            }
            else {
              $output .= l($toolbox['title'], $path, $options) . ' ' ;
            }
            // TODO: make it pretty
            // $output .= l($toolbox['title'], $path, $options);
            //  , array('query' => array('destination' => 'node/' . $nid ))
            // $output .= theme('toolbox_link', $toolbox['title'], $path, _oa_folders_dom_id($toolbox['op'], $type, $id), $toolbox['op']);
          }
        }
      }
    }
    return $output;
  }
}


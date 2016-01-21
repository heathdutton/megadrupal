<?php
// $Id: myhookadmin.inc,v 1.4 2010/10/09 05:56:14 ufku Exp $

/**
 * Generates admin page.
 */
function myhookadmin_admin() {
  if (isset($_GET['token']) && isset($_GET['varname']) && drupal_valid_token($_GET['token'], 'myhookadmin')) {
    drupal_add_http_header('Content-Type', 'text/plain; charset=utf-8');
    print myhookadmin_var_get($_GET['varname'], '');
    exit();
  }
  $output = array();
  $output['form'] = drupal_get_form('myhookadmin_admin_form');
  $output['tips'] = array('#markup' => '
<h2>Tips & Links</h2>
<ul class="tips">
  <li>Example hook_node_insert() implementation that notifies site admin of new content.<br />
  <code>function myhook_node_insert($node) {<br />
&nbsp;  drupal_mail_system(\'myhook\', \'mail\')->mail(array(<br />
&nbsp; &nbsp; \'to\' => \'admin@example.com\',<br />
&nbsp; &nbsp; \'subject\' => \'New content\',<br />
&nbsp; &nbsp; \'body\' => \'New content entered at \' . url(\'node/\' . $node->nid, array(\'absolute\' => TRUE)),<br />
&nbsp; &nbsp; \'headers\' => array(\'From\' => variable_get(\'site_mail\', ini_get(\'sendmail_from\'))),<br />
&nbsp; ));<br />
}</code>
  </li>
  <li>If your code causes a fatal error or if you want to disable it for a single request, just append <code>?myhook=0</code> to the URL. Ex: ' . url('admin/config/development/myhook', array('query' => array('myhook' => '0'), 'absolute' => TRUE)) . '</li>
  <li>You may need to flush caches to make some hooks such as <code>myhook_menu()</code> or <code>myhook_theme()</code> to take effect.</li>
  <li><a href="http://api.drupal.org/api/group/hooks/7">Drupal core hooks</a></li>
  <li><a href="http://api.lullabot.com/group/views_hooks/7">Views hooks</a></li>
  <li><a href="http://api.lullabot.com/group/hooks/7">Various module hooks</a></li>
</ul>
<script type="text/javascript">
<!--//--><![CDATA[//><!--
window.jQuery && (function($) {$(function() {
  var url = "' . url('admin/config/development/myhook', array('query' => array('token' => drupal_get_token('myhookadmin')))) . '";
  var $name = $("#edit-varname"), $value = $("#edit-varvalue");
  if (!$name.size()) return;
  var getval = function() {
    var varname = $name.val();
    if (!varname) {
      $name.focus();
      return alert("Please enter a varible name");
    }
    var xurl = url + "&varname=" + escape(varname);
    $button.val("Loading...").attr("disabled", "disabled");
    $.ajax({
      url: xurl,
      dataType: "text",
      error: function(request) {alert(Drupal.ajaxError(request, xurl))},
      success: function(value) {$value.val(value).focus().select()},
      complete: function() {$button.removeAttr("disabled").val("Get value");}
    });
  };
  var $button = $(document.createElement("input")).attr({type: "button", value: "Get value"}).insertAfter($name[0]).click(getval);
  // Alter enter key for variable name
  $name.keydown(function(e) {
    if (e.keyCode == 13) {
      var val = this.value;
      $name.one("keyup", function(){
        this.value == val && getval();
      });
      return false;
    }
  });
})})(jQuery);
//--><!]]>
</script>');
  return $output;
}

/**
 * Returns admin form.
 */
function myhookadmin_admin_form($form, &$form_state) {
  $form = array();
  $form['code'] = array(
    '#title' => 'Content of myhook.module',
    '#type' => 'textarea',
    '#default_value' => myhookadmin_var_get('myhook_code', ''),
    '#rows' => 20,
    '#cols' => 60,
    '#description' => 'Define your custom hooks as if they are part of myhook.module. Do not include PHP opening and closing tags.',
  );
  $form['is_inc'] = array(
    '#title' => 'Save the code in a file and include it as a file',
    '#type' => 'checkbox',
    '#default_value' => variable_get('myhookadmin_is_inc', 0),
    '#description' => 'This may save a DB hit and an eval execution on each request as the code will be saved in a file whose path must be defined below.',
  );
  $form['inc_path'] = array(
    '#title' => 'File path to include',
    '#type' => 'textfield',
    '#size' => 60,
    '#default_value' => variable_get('myhookadmin_inc_path', ''),
    '#description' => 'This must be a PHP-writable file path such as public://myhook.inc. It will be made readonly automatically after writing.',
  );
  $weight = db_query("SELECT weight FROM {system} WHERE name = 'myhook'")->fetchField();
  $sql = "SELECT name, weight FROM {system} WHERE type = 'module' AND status = 1 ORDER BY weight";
  $min = db_query_range($sql, 0, 1)->fetchObject();
  $max = db_query_range($sql . ' DESC', 0, 1)->fetchObject();
  $form['weight'] = array(
    '#title' => 'Myhook module weight',
    '#type' => 'textfield',
    '#size' => 60,
    '#maxlength' => 4,
    '#default_value' => $weight,
    '#description' => 'Currently Min: ' . check_plain($min->name) . '(' . $min->weight . '), Max: ' . check_plain($max->name) . '(' . $max->weight . ')',
  );
  $form['vars'] = array(
    '#title' => 'Add/edit/delete a myhook variable',
    '#type' => 'fieldset',
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );
  $form['vars']['varname'] = array(
    '#title' => 'Variable name',
    '#type' => 'textfield',
    '#size' => 60,
    '#maxlength' => 128,
    '#default_value' => '',
  );
  $form['vars']['vardelete'] = array(
    '#title' => 'Delete this variable',
    '#type' => 'checkbox',
    '#default_value' => 0,
  );
  $form['vars']['varvalue'] = array(
    '#title' => 'Variable value',
    '#type' => 'textarea',
    '#default_value' => '',
    '#rows' => 10,
    '#cols' => 60,
    '#description' => 'The variables are stored in table {myhookadmin} and can be managed by <code>myhookadmin_var_get("variable_name", "optional_default_value")</code> and <code>myhookadmin_var_set("variable_name", "value")</code>.',
  );
  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => 'Save',
  );
  $form['#validate'][] = 'myhookadmin_admin_validate';
  $form['#submit'][] = 'myhookadmin_admin_submit';
  return $form;
}

/**
 * Validates admin form.
 */
function myhookadmin_admin_validate($form, &$form_state) {
  $is_inc = $form_state['values']['is_inc'];
  $inc_path = $form_state['values']['inc_path'];
  $code = $form_state['values']['code'];
  if (substr($code, 0, 5) == '<?php') {
    $code = $form_state['values']['code'] = substr($code, 5);
  }
  // Check if file is writable
  if ($is_inc) {
    if (empty($inc_path)) {
      return form_set_error('inc_path', 'Please specify a valid file path.');
    }
    if (file_exists($inc_path)) {
      @drupal_chmod($inc_path, 0666);
    }
    if (!$fp = fopen($inc_path, 'w')) {
      return form_set_error('inc_path', 'Unable to open the specified file for writing.');
    }
    fwrite($fp, "<?php\n\n" . $code);
    fclose($fp);
    @drupal_chmod($inc_path, 0444);
  }
}

/**
 * Submits admin form.
 */
function myhookadmin_admin_submit($form, &$form_state) {
  extract($form_state['values']);

  // Save code
  myhookadmin_var_set('myhook_code', $code);

  // Save variables
  variable_set('myhookadmin_is_inc', $is_inc);
  variable_set('myhookadmin_inc_path', $inc_path);

  // Save weight
  db_update('system')->fields(array('weight' => $weight))->condition('name', 'myhook')->execute();

  // Save/delete myhook variable
  if (!empty($varname) && $varname != 'myhook_code') {
    if ($vardelete) {
      $varvalue = NULL;
      drupal_set_message('The ' . check_plain($varname) . ' variable has been deleted.');
    }
    myhookadmin_var_set($varname, $varvalue);
  }

  drupal_set_message('The changes have been saved.');
}
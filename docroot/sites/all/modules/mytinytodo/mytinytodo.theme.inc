<?php

function mytinytodo_html_add_css($src) {
	   if ($src[0] == '/')
			   $src = substr($src, 1);
	   drupal_add_css($src, array('group' => CSS_DEFAULT));
}

function mytinytodo_html_add_js($src) {
	   drupal_add_js($src);
}

function mytinytodo_html_add_library($module, $name) {
	   global $base_path;

	   $output = '';
	   if ($library = drupal_get_library($module, $name)) {
			   if ($library['js']) {
					   foreach($library['js'] as $file => $info)
							   mytinytodo_html_add_js($base_path . $file);
			   }
			   if ($library['css']) {
					   foreach($library['css'] as $file => $info)
							   mytinytodo_html_add_css($base_path . $file);
			   }
	   }
}

function theme_mytinytodo_html($variables) {

	module_load_include('inc', 'mytinytodo', 'lib/init');

	$nid = $variables['nid'];
	$field_name = $variables['field'];

	$results = db_query("SELECT id, delta FROM {mytinytodo_fields} WHERE entity_id = :entity_id", array(':entity_id' => $nid));
	foreach($results as $result) {
		$info = field_info_field_by_id($result->delta);
		if ($info['field_name'] == $field_name) {
			$fid = $result->id;
			break;
		}
	}

	if (!$fid)
		return '';

	$output = '';
	$lang = Lang::instance();

	if ($lang->rtl()) 
		Config::set('rtl', 1);
	if (!is_int(Config::get('firstdayofweek')) || Config::get('firstdayofweek') < 0 || Config::get('firstdayofweek') > 6)
		Config::set('firstdayofweek', 1);

	if (!defined('TEMPLATEPATH'))
		define('TEMPLATEPATH', MTTPATH. 'themes/'.Config::get('template').'/');

	global $base_path;
	$path = $base_path . drupal_get_path('module', 'mytinytodo') . '/';
	mytinytodo_html_add_css($path . 'lib/themes/default/list-style2.css', array('group' => CSS_DEFAULT));

	if (isset($variables['pda']) && $variables['pda'] === true) {
		$viewport = array(
				'#tag' => 'meta',
				'#attributes' => array(
					'name' => 'viewport',
					'id' => 'viewport',
					'content' => 'width=device-width',
					),
				);
		mytinytodo_html_add_css($path . 'lib/themes/default/pda.css', array('group' => CSS_DEFAULT));
	}

	mytinytodo_html_add_library('system','jquery');
	mytinytodo_html_add_library('system','jquery.once');
	mytinytodo_html_add_js($base_path  . 'misc/ui/jquery.ui.core.min.js');
	mytinytodo_html_add_js($base_path . 'misc/jquery.ba-bbq.js');
	mytinytodo_html_add_library('system','jquery.cookie');
	mytinytodo_html_add_library('system','ui.widget');
	mytinytodo_html_add_library('system','ui.mouse');
	mytinytodo_html_add_js($path . 'lib/jquery/jquery.autocomplete-1.1.js');
	mytinytodo_html_add_library('system','effects');
	mytinytodo_html_add_library('system','effects.highlight');
	mytinytodo_html_add_library('system','ui.datepicker');
	mytinytodo_html_add_library('system','ui.sortable');

	$jsoutput = '';
	if (is_numeric($fid))
		$jsoutput .= "var field_id = {$fid};\n";

	$jsoutput .= "var base_url = '" . url(NULL, array('absolute' => TRUE)) . "';\n";

	drupal_add_js($jsoutput, 'inline');

	mytinytodo_html_add_js($path . 'lib/mytinytodo.js');
	mytinytodo_html_add_js($path . 'lib/mytinytodo_lang.php');
	mytinytodo_html_add_js($path . 'lib/mytinytodo_ajax_storage.js');
	mytinytodo_html_add_js($base_path . 'mytinytodo/init');

	$output .= '
	<div id="wrapper">
	<div id="container">
	<div id="mtt_body">

	<div id="loading"></div>

	<div id="bar">
	<div id="msg"><span class="msg-text"></span><div class="msg-details"></div></div>
	<div class="bar-menu">
	<span class="menu-owner" style="display:none">
	<a href="#settings" id="settings">' . t('Settings') . '</a>
	</span>
	</div>
	</div>

	<br clear="all" />

	<div id="page_tasks" style="display:none">

	<div id="lists">
	<ul class="mtt-tabs"></ul>
	<div class="mtt-tabs-add-button" title="' . t('New list') . '"><span></span></div>
	<div id="tabs_buttons">
	<div class="mtt-tabs-select-button mtt-tabs-button" title="' . t('Select list') . '"><span></span></div>
	</div>
	<div id="list_all" class="mtt-tab mtt-tabs-alltasks mtt-tabs-hidden"><a href="#alltasks"><span>' . t('All tasks') . '</span><div class="list-action"></div></a></div>
	</div>

	<div id="mtt_toolbar" class="mtt-htabs">

	<div id="htab_search">
	<table class="mtt-searchbox"><tr><td>
	<div class="mtt-searchbox-c">
	<input type="text" name="search" value="" maxlength="250" id="search" autocomplete="off" />
	<div class="mtt-searchbox-icon mtt-icon-search"></div>
	<div id="search_close" class="mtt-searchbox-icon mtt-icon-cancelsearch"></div>
	</div> 
	</td></tr></table>
	</div>

	<div id="htab_newtask">
	<table class="mtt-taskbox"><tr><td class="mtt-tb-cell">
	<div class="mtt-tb-c">
	<form id="newtask_form" method="post">
	<label id="task_placeholder" class="placeholding" for="task">
	 <input type="text" name="task" value="" maxlength="250" id="task" autocomplete="off" />
	 <span>' . t('New task') . '</span>
	</label>
	<div id="newtask_submit" class="mtt-taskbox-icon mtt-icon-submittask" title="' . t('Add') . '"></div>
	</form>
	</div>
	</td>
	<td><a href="#" id="newtask_adv" class="mtt-img-button" title="' . t('Advanced') . '"><span></span></a></td>
	</tr></table>
	</div>

	<div id="searchbar" style="display:none">' . t('Searching for') . ' <span id="searchbarkeyword"></span></div>

	<div style="clear:both"></div>

	</div>


	<h3>
	<span id="taskview" class="mtt-menu-button"><span class="btnstr">' . t('Tasks') . '</span> (<span id="total">0</span>) <span class="arrdown"></span></span>
	<span class="mtt-notes-showhide">' . t('Notes:') . ' <a href="#" id="mtt-notes-show">' . t('Show') . '</a> / <a href="#" id="mtt-notes-hide">' . t('Hide') . '</a></span>
	<span id="mtt_filters"></span>
	<span id="tagcloudbtn" class="mtt-menu-button">' . t('Tags') . ' <span class="arrdown2"></span></span>
	</h3>

	<div id="taskcontainer">
	<ol id="tasklist" class="sortable"></ol>
	</div>

	</div> <!-- end of page_tasks -->


	<div id="page_taskedit" style="display:none">

	<div><a href="#" class="mtt-back-button">&lt;&lt; ' . t('Back') . '</a></div>

	<h3 class="mtt-inadd">' . t('New task') . '</h3>
	<h3 class="mtt-inedit">' . t('Edit task') . '
	<div id="taskedit-date" class="mtt-inedit">
	(<span class="date-created" title="' . t('Created') . '"><span></span></span><span class="date-completed" title="' . t('Completed') . '"> &mdash; <span></span></span>)
	</div>
	</h3>

	<form id="taskedit_form" name="edittask" method="post">
	<input type="hidden" name="isadd" value="0" />
	<input type="hidden" name="id" value="" />
	<div class="form-row form-row-short">
	<span class="h">' . t('Priority') . '</span>
	<select name="prio">
	<option value="2">+2</option><option value="1">+1</option><option value="0" selected="selected">&plusmn;0</option><option value="-1">&minus;1</option>
	</select>
	</div>
	<div class="form-row form-row-short">
	<span class="h">' . t('Due') . ' </span>
	<input name="duedate" id="duedate" value="" class="in100" title="Y-M-D, M/D/Y, D.M.Y, M/D, D.M" autocomplete="off" />
	</div>
	<div class="form-row-short-end"></div>
	<div class="form-row"><div class="h">' . t('Tasks') . '</div> <input type="text" name="task" value="" class="in500" maxlength="250" /></div>
' . theme('mytinytodo_edit_note') . '
' . theme('mytinytodo_edit_tags') . '
	<input type="submit" value="' . t('Save') . '" /> 
	<input type="button" id="mtt_edit_cancel" class="mtt-back-button" value="' . t('Cancel') . '" />
	</div>
	</form>

	</div>  <!-- end of page_taskedit -->


	<div id="priopopup" style="display:none">
	<span class="prio-neg prio-neg-1">&minus;1</span>
	<span class="prio-zero">&plusmn;0</span>
	<span class="prio-pos prio-pos-1">+1</span>
	<span class="prio-pos prio-pos-2">+2</span>
	</div>

	<div id="taskviewcontainer" class="mtt-menu-container" style="display:none">
	<ul>
	<li id="view_tasks">' . t('Tasks') . ' (<span id="cnt_total">0</span>)</li>
	<li id="view_past">' . t('Overdue') . ' (<span id="cnt_past">0</span>)</li>
	<li id="view_today">' . t('Today and tomorrow') . ' (<span id="cnt_today">0</span>)</li>
	<li id="view_soon">' . t('Soon') . ' (<span id="cnt_soon">0</span>)</li>
	</ul>
	</div>

	<div id="tagcloud" style="display:none">
	<a id="tagcloudcancel" class="mtt-img-button"><span></span></a>
	<div id="tagcloudload"></div>
	<div id="tagcloudcontent"></div>
	</div>


	<div id="listmenucontainer" class="mtt-menu-container" style="display:none">
	<ul>
	<li class="mtt-need-list mtt-need-real-list" id="btnRenameList">' . t('Rename list') . '</li>
	<li class="mtt-need-list mtt-need-real-list" id="btnDeleteList">' . t('Delete list') . '</li>
	<li class="mtt-need-list mtt-need-real-list" id="btnClearCompleted">' . t('Clear completed tasks') . '</li>
	<li class="mtt-need-list mtt-need-real-list mtt-menu-indicator" submenu="listexportmenucontainer"><div class="submenu-icon"></div>' . t('Export') . '></li>
	<li class="mtt-menu-delimiter mtt-need-real-list"></li>
	<li class="mtt-need-list mtt-need-real-list" id="btnPublish"><div class="menu-icon"></div>' . t('Publish') . '</li>
	<li class="mtt-need-list mtt-need-real-list" id="btnRssFeed"><div class="menu-icon"></div>' . t('RSS Feed') . '</li>
	<li class="mtt-menu-delimiter mtt-need-real-list"></li>
	<li class="mtt-need-list mtt-need-real-list sort-item" id="sortByHand"><div class="menu-icon"></div>' . t('Sort by hand') . ' <span class="mtt-sort-direction"></span></li>
	<li class="mtt-need-list sort-item" id="sortByDateCreated"><div class="menu-icon"></div>' . t('Sort by date created') . ' <span class="mtt-sort-direction"></span></li>
	<li class="mtt-need-list sort-item" id="sortByPrio"><div class="menu-icon"></div>' . t('Sort by priority') . ' <span class="mtt-sort-direction"></span></li>
	<li class="mtt-need-list sort-item" id="sortByDueDate"><div class="menu-icon"></div>' . t('Sort by due date') . ' <span class="mtt-sort-direction"></span></li>
	<li class="mtt-need-list sort-item" id="sortByDateModified"><div class="menu-icon"></div>' . t('Sort by date modified') . ' <span class="mtt-sort-direction"></span></li>
	<li class="mtt-menu-delimiter"></li>
	<li class="mtt-need-list" id="btnShowCompleted"><div class="menu-icon"></div>' . t('Show completed tasks') . '</li>
	</ul>
	</div>

	<div id="listexportmenucontainer" class="mtt-menu-container" style="display:none">
	<ul>
	<li class="mtt-need-list mtt-need-real-list" id="btnExportCSV">' . t('CSV') . '</li>
	<li class="mtt-need-list mtt-need-real-list" id="btnExportICAL">' . t('iCalendar') . '</li>
	</ul>
	</div>

	<div id="taskcontextcontainer" class="mtt-menu-container" style="display:none">
	<ul>
	<li id="cmenu_edit"><b>' . t('Edit') . '</b></li>
	<li id="cmenu_note">' . t('Edit note') . '</li>
	<li id="cmenu_prio" class="mtt-menu-indicator" submenu="cmenupriocontainer"><div class="submenu-icon"></div>' . t('Priority') . '</li>
	<li id="cmenu_move" class="mtt-menu-indicator" submenu="cmenulistscontainer"><div class="submenu-icon"></div>' . t('Move to') . '</li>
	<li id="cmenu_delete">' . t('Delete') . '</li>
	</ul>
	</div>

	<div id="cmenupriocontainer" class="mtt-menu-container" style="display:none">
	<ul>
	<li id="cmenu_prio:2"><div class="menu-icon"></div>+2</li>
	<li id="cmenu_prio:1"><div class="menu-icon"></div>+1</li>
	<li id="cmenu_prio:0"><div class="menu-icon"></div>&plusmn;0</li>
	<li id="cmenu_prio:-1"><div class="menu-icon"></div>&minus;1</li>
	</ul>
	</div>

	<div id="cmenulistscontainer" class="mtt-menu-container" style="display:none">
	<ul>
	</ul>
	</div>

	<div id="slmenucontainer" class="mtt-menu-container" style="display:none">
	<ul>';

	if (is_readonly())
		$output .= '<li id="slmenu_list:-1" class="list-id--1 mtt-need-list" style="display:none">';
	else
		$output .= '<li id="slmenu_list:-1" class="list-id--1 mtt-need-list">';

	$output .= '<div class="menu-icon"></div><a href="#alltasks">' . t('All tasks') . '></a></li>';

	if (is_readonly())
		$output .= '<li class="mtt-menu-delimiter slmenu-lists-begin mtt-need-list" style="display:none"></li>';
	else
		$output .= '<li class="mtt-menu-delimiter slmenu-lists-begin mtt-need-list"></li>';

	$output .='
	</ul>
	</div>

	<div id="page_ajax" style="display:none"></div>

	</div>
	<div id="space"></div>
	</div>

	</div>';

	return $output;
}

function theme_mytinytodo_todo_menu_tree($variables) {
	$menu = $variables['menu'];
	$fid = $variables['fid'];

	$menus = array();
	$containers = array();
	$scripts = array();
	foreach ($menu as $name => $item) {
		if (isset($item['items']) && is_array($item['items'])) {
			// Get the main menu
			$menus[] = theme('mytinytodo_todo_submenu_item', array('item' => array('name' => $name, 'text' => $item['text']), 'fid' => $fid));

			// Get the sub menu containers and scripts
			$output = theme('mytinytodo_todo_menu_container', array('items' => $item['items'], 'item' => array('name' => $name, 'text' => $item['text']), 'fid' => $fid));
			if (!empty($output['containers']))
				$containers = array_merge($containers, $output['containers']);

			if (!empty($output['scripts']))
				$scripts = array_merge($scripts, $output['scripts']);
		} else {
			$menus[] = theme('mytinytodo_todo_menu_item', array('item' => array('name' => $name, 'text' => $item['text']), 'fid' => $fid));

			// Get the Javascript that will be executed when the menu is clicked
			if (isset($item['callback'])) {
				$scripts[] = theme('mytinytodo_todo_menu_item_callback', array('item' => array('name' => $name, 'text' => $item['text']), 'callback' => $item['callback'], 'arguments' => $item['arguments']));
			}
		}
	}

	return array('menus' => $menus, 'containers' => $containers, 'scripts' => $scripts);
}

function theme_mytinytodo_todo_menu_container($variables) {
	$item = $variables['item'];
	$items = $variables['items'];
	$fid = $variables['fid'];
	$item_id = 'cmenu_' . $variables['item']['name'];
	$container_id = $item_id . '_container';
	$containers = array();
	$scripts = array();

	$container = '<div id="' . $container_id . '" class="mtt-menu-container" style="display:none">' . "\n<ul>\n";
	foreach ($items as $submenu_name => $submenu_item) {
		$output = theme('mytinytodo_todo_menu_tree', array('menu' => array($submenu_name => $submenu_item), 'fid' => $fid));
		$container .= theme('mytinytodo_todo_menu_items', array('items' => $output['menus'], 'fid' => $fid));

		if (!empty($output['containers']))
			$containers = array_merge($containers, $output['containers']);
		if (!empty($output['scripts']))
			$scripts = array_merge($scripts, $output['scripts']);
	}
	$container .= "\n</ul>\n</div>";
	$containers[] = $container;

	return array('containers' => $containers, 'scripts' => $scripts);
}

function theme_mytinytodo_todo_menu_item($variables) {
	$item_id = 'cmenu_' . $variables['item']['name'];
	$text = $variables['item']['text'];

	return "<li id=\"{$item_id}\">{$text}</li>\n";
}

function theme_mytinytodo_todo_submenu_item($variables) {
	$item = $variables['item'];
	$item_id = 'cmenu_' . $item['name'];
	$container_id = $item_id . '_container';
	$text = $item['text'];

	return '<li id="' . $item_id . '" class="mtt-menu-indicator" submenu="' . $container_id . '"><div class="submenu-icon"></div>' . $text . "</li>\n";
}

function theme_mytinytodo_todo_menu_item_callback($variables) {
	$item = $variables['item'];
	$item_id = 'cmenu_' . $item['name'];
	$arguments = $variables['arguments'];
	$callback = $variables['callback'];
	$args = '';

	if (isset($arguments) && is_array($arguments)) {
		foreach ($arguments as $index => $argument) {
			if (is_string($argument)) {
				if (preg_match('/^javascript:(.*)$/i', $argument, $matches)) {
					$arguments[$index] = $matches[1];
				} else {
					$arguments[$index] = '"' . $argument . '"';
				}
			}
		}
		$args = join(',', $arguments);
	}

	return "$('#{$item_id}').click(function() {\n{$callback}({$args});\n});";
}

function theme_mytinytodo_todo_menu_items($variables) {
	return join("\n", $variables['items']);
}

function theme_mytinytodo_todo_menu_containers($variables) {
	return join("\n", $variables['containers']);
}

function theme_mytinytodo_todo_menu_scripts($variables) {
	return "\n<script type=\"text/javascript\">\n" . join("\n", $variables['scripts']) . "\n</script>\n";
}

function theme_mytinytodo_edit_note() {
	return '<div class="form-row"><div class="h">' . t('Note') . '</div> <textarea name="note" class="in500"></textarea></div>';
}

function theme_mytinytodo_edit_tags() {
	return '<div class="form-row"><div class="h">' . t('Tags') . '</div>
 <table cellspacing="0" cellpadding="0" width="100%"><tr>
  <td><input type="text" name="tags" id="edittags" value="" class="in500" maxlength="250" /></td>
  <td class="alltags-cell">
   <a href="#" id="alltags_show">' . t('Show all') . '</a>
   <a href="#" id="alltags_hide" style="display:none">' . t('Hide all') . '</a></td>
 </tr></table>
</div>
<div class="form-row" id="alltags" style="display:none;">' . t('All tags') . ': <span class="tags-list"></span></div>';
}

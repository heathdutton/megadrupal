<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Strict//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-strict.dtd">
<html xmlns="http://www.w3.org/1999/xhtml" lang="<?php print $GLOBALS['language']->language; ?>" xml:lang="<?php print $GLOBALS['language']->language; ?>">
<head>
<?php

    function mytinytodo_page_add_css($src, $absolute_url = false) {
        if ($absolute_url || variable_get('clean_url', 0)) {
            echo '<link rel="stylesheet" type="text/css" href="' . url($src, array('absolute' => TRUE)) . '" media="all" />' . "\n";
        } else {
            echo '<link rel="stylesheet" type="text/css" href="' . $src . '" media="all" />';
        }
    }

    function mytinytodo_page_add_js($src, $absolute_url = false) {
        if ($absolute_url || variable_get('clean_url', 0)) {
            echo '<script type="text/javascript" src="' . url($src, array('absolute' => TRUE)) . '"></script>' . "\n";
        } else {
            echo '<script type="text/javascript" src="' . $src . '"></script>';
        }
    }

    function mytinytodo_page_add_library($module, $name) {
        global $base_path;

        if ($library = drupal_get_library($module, $name)) {
            if ($library['js']) {
                foreach($library['js'] as $file => $info)
                    mytinytodo_page_add_js($file);
            }
            if ($library['css']) {
                foreach($library['css'] as $file => $info)
                    mytinytodo_page_add_css($file);
            }
        }
    }

    $lang = Lang::instance();

    if ($lang->rtl()) 
        Config::set('rtl', 1);
    if (!is_int(Config::get('firstdayofweek')) || Config::get('firstdayofweek') < 0 || Config::get('firstdayofweek') > 6)
        Config::set('firstdayofweek', 1);

    if (!defined('TEMPLATEPATH'))
        define('TEMPLATEPATH', MTTPATH. 'themes/'.Config::get('template').'/');

        global $base_path;
        $mytinytodo_path = drupal_get_path('module', 'mytinytodo') . '/';
        mytinytodo_page_add_css($mytinytodo_path . 'lib/themes/default/list-style.css');

    if (isset($_GET['pda'])) {
        $viewport = array(
            '#tag' => 'meta',
            '#attributes' => array(
                'name' => 'viewport',
                'id' => 'viewport',
                'content' => 'width=device-width',
            ),
        );
        //TODO: mytinytodo_page_add_html_head($viewport, 'pda-viewport');
        mytinytodo_page_add_css($mytinytodo_path . 'lib/themes/default/pda.css', array('group' => CSS_DEFAULT));
    } else {
        //mytinytodo_page_add_css($mytinytodo_path . 'lib/themes/default/print.css', array('group' => CSS_DEFAULT));
    }

    mytinytodo_page_add_library('system','jquery');
    mytinytodo_page_add_library('system','jquery.once');
    mytinytodo_page_add_js('misc/ui/jquery.ui.core.min.js');
    mytinytodo_page_add_js('misc/jquery.ba-bbq.js');
    mytinytodo_page_add_library('system','jquery.cookie');
    mytinytodo_page_add_library('system','ui.widget');
    mytinytodo_page_add_library('system','ui.mouse');
    mytinytodo_page_add_js($mytinytodo_path . 'lib/jquery/jquery.autocomplete-1.1.js');
    mytinytodo_page_add_library('system','effects');
    mytinytodo_page_add_library('system','effects.highlight');
    mytinytodo_page_add_library('system','ui.datepicker');
    mytinytodo_page_add_library('system','ui.sortable');

    $jsoutput = '';
    if (is_numeric($_GET['fid']))
        $jsoutput .= "var field_id = {$_GET['fid']};\n";

    $jsoutput .= "var base_url = '" . url(NULL, array('absolute' => TRUE)) . "';\n";

    if (variable_get('clean_url', 0)) {
        $jsoutput .= "var clean_url = true;\n";
    } else {
        $jsoutput .= "var clean_url = false;\n";
    }

    $jsoutput .= "var csrf_token = '" . drupal_get_token('mytinytodo') . "';\n";

    echo "<script type=\"text/javascript\">\n{$jsoutput}\n</script>\n";

    mytinytodo_page_add_js($mytinytodo_path . 'lib/mytinytodo.js');
    mytinytodo_page_add_js($mytinytodo_path . 'lib/mytinytodo_lang.php');
    mytinytodo_page_add_js($mytinytodo_path . 'lib/mytinytodo_ajax_storage.js');

    // Pass field id so that task list specific settings can be loaded.
    echo '<script type="text/javascript" src="' . url('mytinytodo/init', array('query' => array('fid' => $_GET['fid']), 'absolute' => TRUE)) . '" media="all"></script>' . "\n";
?>
    </head>
    <body>

    <div id="wrapper">
    <div id="container">
    <div id="mtt_body">

    <div id="loading"></div>

    <div id="bar">
     <div id="msg"><span class="msg-text"></span><div class="msg-details"></div></div>
     <div class="bar-menu">
     <span class="menu-owner" style="display:none">
       <a href="#settings" id="settings"><?php _e('a_settings');?></a>
     </span>
     </div>
    </div>

    <br clear="all" />

    <div id="page_tasks" style="display:none">

    <div id="lists">
     <ul class="mtt-tabs"></ul>
     <div class="mtt-tabs-add-button" title="<?php _e('list_new'); ?>"><span></span></div>
     <div id="tabs_buttons">
      <div class="mtt-tabs-select-button mtt-tabs-button" title="<?php _e('list_select'); ?>"><span></span></div>
     </div>
     <div id="list_all" class="mtt-tab mtt-tabs-alltasks mtt-tabs-hidden"><a href="#alltasks"><span><?php _e('alltasks'); ?></span><div class="list-action"></div></a></div>
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
         <span><?php _e('htab_newtask');?></span>
        </label>
        <div id="newtask_submit" class="mtt-taskbox-icon mtt-icon-submittask" title="<?php _e('btn_add');?>"></div>
       </form>
      </div>
     </td>
     <td><a href="#" id="newtask_adv" class="mtt-img-button" title="<?php _e('advanced_add');?>"><span></span></a></td>
     </tr></table>
    </div>
     
    <div id="searchbar" style="display:none"><?php _e('searching');?> <span id="searchbarkeyword"></span></div>

    <div style="clear:both"></div>

    </div>


    <h3>
    <span id="taskview" class="mtt-menu-button"><span class="btnstr"><?php _e('tasks');?></span> (<span id="total">0</span>) <span class="arrdown"></span></span>
    <span class="mtt-notes-showhide"><?php _e('notes');?> <a href="#" id="mtt-notes-show"><?php _e('notes_show');?></a> / <a href="#" id="mtt-notes-hide"><?php _e('notes_hide');?></a></span>
    <span id="mtt_filters"></span>
    <span id="tagcloudbtn" class="mtt-menu-button"><?php _e('tagcloud');?> <span class="arrdown2"></span></span>
    </h3>

    <div id="taskcontainer">
     <ol id="tasklist" class="sortable"></ol>
    </div>

    </div> <!-- end of page_tasks -->


    <div id="page_taskedit" style="display:none">

    <div><a href="#" class="mtt-back-button"><?php _e('go_back');?></a></div>

    <h3 class="mtt-inadd"><?php _e('add_task');?></h3>
    <h3 class="mtt-inedit"><?php _e('edit_task');?>
     <div id="taskedit-date" class="mtt-inedit">
      (<span class="date-created" title="<?php _e('taskdate_created');?>"><span></span></span><span class="date-completed" title="<?php _e('taskdate_completed');?>"> &mdash; <span></span></span>)
     </div>
    </h3>

    <form id="taskedit_form" name="edittask" method="post">
    <input type="hidden" name="isadd" value="0" />
    <input type="hidden" name="id" value="" />
    <div class="form-row form-row-short">
     <span class="h"><?php _e('priority');?></span>
     <select name="prio">
      <option value="2">+2</option><option value="1">+1</option><option value="0" selected="selected">&plusmn;0</option><option value="-1">&minus;1</option>
     </select>
    </div>
    <div class="form-row form-row-short">
     <span class="h"><?php _e('due');?> </span>
     <input name="duedate" id="duedate" value="" class="in100" title="Y-M-D, M/D/Y, D.M.Y, M/D, D.M" autocomplete="off" />
    </div>
    <div class="form-row-short-end"></div>
    <div class="form-row"><div class="h"><?php _e('task');?></div> <input type="text" name="task" value="" class="in500" maxlength="250" /></div>
    <?php echo theme('mytinytodo_edit_note') ?>
    <?php echo theme('mytinytodo_edit_tags') ?>
    <div class="form-row form-bottom-buttons">
     <input type="submit" value="<?php _e('save');?>" /> 
     <input type="button" id="mtt_edit_cancel" class="mtt-back-button" value="<?php _e('cancel');?>" />
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
     <li id="view_tasks"><?php _e('tasks');?> (<span id="cnt_total">0</span>)</li>
     <li id="view_past"><?php _e('f_past');?> (<span id="cnt_past">0</span>)</li>
     <li id="view_today"><?php _e('f_today');?> (<span id="cnt_today">0</span>)</li>
     <li id="view_soon"><?php _e('f_soon');?> (<span id="cnt_soon">0</span>)</li>
    </ul>
    </div>

    <div id="tagcloud" style="display:none">
     <a id="tagcloudcancel" class="mtt-img-button"><span></span></a>
     <div id="tagcloudload"></div>
     <div id="tagcloudcontent"></div>
    </div>


    <div id="listmenucontainer" class="mtt-menu-container" style="display:none">
    <ul>
     <li class="mtt-need-list mtt-need-real-list" id="btnRenameList"><?php _e('list_rename');?></li>
     <li class="mtt-need-list mtt-need-real-list" id="btnDeleteList"><?php _e('list_delete');?></li>
     <li class="mtt-need-list mtt-need-real-list" id="btnClearCompleted"><?php _e('list_clearcompleted');?></li>
     <li class="mtt-need-list mtt-need-real-list mtt-menu-indicator" submenu="listexportmenucontainer"><div class="submenu-icon"></div><?php _e('list_export'); ?></li>
     <li class="mtt-menu-delimiter mtt-need-real-list"></li>
     <li class="mtt-need-list mtt-need-real-list" id="btnPublish"><div class="menu-icon"></div><?php _e('list_publish');?></li>
     <li class="mtt-need-list mtt-need-real-list" id="btnRssFeed"><div class="menu-icon"></div><?php _e('list_rssfeed');?></li>
     <li class="mtt-menu-delimiter mtt-need-real-list"></li>
     <li class="mtt-need-list mtt-need-real-list sort-item" id="sortByHand"><div class="menu-icon"></div><?php _e('sortByHand');?> <span class="mtt-sort-direction"></span></li>
     <li class="mtt-need-list sort-item" id="sortByDateCreated"><div class="menu-icon"></div><?php _e('sortByDateCreated');?> <span class="mtt-sort-direction"></span></li>
     <li class="mtt-need-list sort-item" id="sortByPrio"><div class="menu-icon"></div><?php _e('sortByPriority');?> <span class="mtt-sort-direction"></span></li>
     <li class="mtt-need-list sort-item" id="sortByDueDate"><div class="menu-icon"></div><?php _e('sortByDueDate');?> <span class="mtt-sort-direction"></span></li>
     <li class="mtt-need-list sort-item" id="sortByDateModified"><div class="menu-icon"></div><?php _e('sortByDateModified');?> <span class="mtt-sort-direction"></span></li>
     <li class="mtt-menu-delimiter"></li>
     <li class="mtt-need-list" id="btnShowCompleted"><div class="menu-icon"></div><?php _e('list_showcompleted');?></li>
    </ul>
    </div>

    <div id="listexportmenucontainer" class="mtt-menu-container" style="display:none">
    <ul>
      <li class="mtt-need-list mtt-need-real-list" id="btnExportCSV"><?php _e('list_export_csv');?></li>
      <li class="mtt-need-list mtt-need-real-list" id="btnExportICAL"><?php _e('list_export_ical');?></li>
    </ul>
    </div>

    <div id="taskcontextcontainer" class="mtt-menu-container" style="display:none">
    <ul>
     <li id="cmenu_edit"><b><?php _e('action_edit');?></b></li>
     <li id="cmenu_note"><?php _e('action_note');?></li>
     <li id="cmenu_prio" class="mtt-menu-indicator" submenu="cmenupriocontainer"><div class="submenu-icon"></div><?php _e('action_priority');?></li>
     <li id="cmenu_move" class="mtt-menu-indicator" submenu="cmenulistscontainer"><div class="submenu-icon"></div><?php _e('action_move');?></li>
     <li id="cmenu_delete"><?php _e('action_delete');?></li>
<?php

    $fid = $_GET['fid'];
    $containers = '';
    $scripts = '';
    if (is_numeric($fid)) {
        $menus = module_invoke_all('mytinytodo_todo_menu', $fid);
        if (is_array($menus)) {
            foreach($menus as $name => $menu) {
                $output = theme('mytinytodo_todo_menu_tree', array('menu' => array($name => $menu), 'fid' => $fid));
                echo theme('mytinytodo_todo_menu_items', array('items' => $output['menus'], 'fid' => $fid));
                $containers .= theme('mytinytodo_todo_menu_containers', array('containers' => $output['containers'], 'fid' => $fid));
                $scripts .= theme('mytinytodo_todo_menu_scripts', array('scripts' => $output['scripts'], 'fid' => $fid));
            }
        }
    }
?>
</ul>
</div>

<?php echo $containers; ?>
<?php echo $scripts; ?>

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
<ul>
 <li id="slmenu_list:-1" class="list-id--1 mtt-need-list" <?php if(is_readonly()) echo 'style="display:none"' ?>><div class="menu-icon"></div><a href="#alltasks"><?php _e('alltasks'); ?></a></li>
 <li class="mtt-menu-delimiter slmenu-lists-begin mtt-need-list" <?php if(is_readonly()) echo 'style="display:none"' ?>></li>
</ul>
</div>

<div id="page_ajax" style="display:none"></div>

</div>
<div id="space"></div>
</div>

</div>
</body>
</html>

<?php 

require_once dirname(__FILE__) . '/theme.inc';

function trotoar_preprocess_page(&$variables) {

/**
 * Declare variables.
 *
 */
$variables['trp_attributes'] = '';$variables['trh_attributes'] = '';$variables['trs_attributes'] = '';$variables['trf_attributes'] = '';
$variables['atas'] = '';$variables['tengah'] = '';$variables['bawah'] = '';
$variables['top'] = array();$variables['middle'] = array();$variables['bottom'] = array();
$variables['branding']['top'] = array();$variables['branding']['middle'] = array();$variables['branding']['bottom'] = array();
$variables['menu']['top'] = array();$variables['menu']['middle'] = array();$variables['menu']['bottom'] = array();
$variables['header']['top'] = array();$variables['header']['middle'] = array();$variables['header']['bottom'] = array();
$variables['preface']['top'] = array();$variables['preface']['middle'] = array();$variables['preface']['bottom'] = array();
$variables['teaser']['top'] = array();$variables['teaser']['middle'] = array();$variables['teaser']['bottom'] = array();
$variables['content']['top'] = array();$variables['content']['middle'] = array();$variables['content']['bottom'] = array();
$variables['footer']['top'] = array();$variables['footer']['middle'] = array();$variables['footer']['bottom'] = array();
$a = '';$b = '';$c = '';$d = '';$k = '';$l = '';$output = '';$va = '';

/**
 * Attributes
 *
 */
	$variables['trp_attributes'] .= ' id="page" class="page clearfix container-' . theme_get_setting('container_grid') . ' "';
	
	$variables['trh_attributes'] .= ' id="header-global-section" class=" global-section header-global-section"';
	
	$variables['trs_attributes'] .= ' id="container-global-section" class=" global-section container-global-section"';
	
	$variables['trf_attributes'] .= ' id="footer-global-section" class=" global-section footer-global-section"';
	
/**
 * Header regions
 *
 */            
if(!(theme_get_setting('hdr_i') == 0)){
$output = '';
$va		= '';
$output .= '<div id="header-section" class="section header-section"><div id="header-zone" class="zone header-zone">';
for ($i = 1; $i <= theme_get_setting('hdr_i'); $i++) {
	
	$a = (theme_get_setting('container_grid_global') == 1) ? theme_get_setting('container_grid') : theme_get_setting('hdr_grid_' . $i . '');
	$b = theme_get_setting('hdr_grid_' . $a . '_width_' . $i . '');
	$c = theme_get_setting('hdr_grid_' . $a . '_prefix_' . $i . '');
	$d = theme_get_setting('hdr_grid_' . $a . '_suffix_' . $i . '');
	if($c != 0): $k = 'prefix-' . $c . ''; else: $k = '';endif;
	if($d != 0): $l = 'suffix-' . $d . ''; else: $d = '';endif;
	$output .= '<div id="header-wrap-' . $i . '" class="hover ' . theme_get_setting('layout_type') . ' grid-type-' . $a . ' grid-' . $b . ' ' . $k . ' ' . $l . ' header-wrap header-wrap-' . $i . '"><div id="header-' . $i . '" class="header header-' . $i . '">' . render($variables['page']['header-' . $i . '']) . '</div></div>';
	$a = '';$b = '';$c = '';$d = '';$k = '';$l = '';
	
}
$output .= '</div></div>';
$va = theme_get_setting('hdr_weight') . 'c';
$variables['header'][theme_get_setting('hdr_pos')] = array($va => $output);
}
/**
 * Branding regions
 *
 */

$output = '';
$va		= '';
$output .= '<div id="branding-section" class="section branding-section"><div id="branding-zone" class="zone branding-zone">';

	$a = (theme_get_setting('container_grid_global') == 1) ? theme_get_setting('container_grid') : theme_get_setting('brd_grid');
	$b = theme_get_setting('brd_grid_' . $a . '_width');
	$c = theme_get_setting('brd_grid_' . $a . '_prefix');
	$d = theme_get_setting('brd_grid_' . $a . '_suffix');
	if($c != 0): $k = 'prefix-' . $c . ''; else: $k = '';endif;
	if($d != 0): $l = 'suffix-' . $d . ''; else: $d = '';endif;
	if(filter_xss_admin(variable_get('site_name', 'Drupal'))){$variables['site_name']         = (theme_get_setting('toggle_name') ? '<h2 class="site-name">' . filter_xss_admin(variable_get('site_name', 'Drupal')) . '</h2>' : '');}
    if(filter_xss_admin(variable_get('site_slogan', ''))){$variables['site_slogan']       = (theme_get_setting('toggle_slogan') ? '<h6 class="site-slogan">' . filter_xss_admin(variable_get('site_slogan', '')) . '</h6>' : '');}
	$variables['logo']				= (theme_get_setting('logo') ? '<div class="logo-image"><a href="' . base_path() . '" title="' . variable_get('site_name', 'Drupal') . '"><img id="logo" alt="' . variable_get('site_name', 'Drupal') . '" src="' . theme_get_setting('logo') . '"/></a></div>' : '');
	$tr_site_name_slogan = '<hgroup class="site-name-slogan">' . $variables['site_name'] . $variables['site_slogan'] . '</hgroup>';
	
	if($tr_site_name_slogan || $variables['logo']){
	$output .= '<div id="branding-data" class="hover ' . theme_get_setting('layout_type') . ' grid-type-' . $a . ' grid-' . $b . ' ' . $k . ' ' . $l . ' branding-data branding-wrap"> ' . $variables['logo'] . $tr_site_name_slogan . '</div>';
	}
	$output .= '<div id="branding-region-wrap" class="hover ' . theme_get_setting('layout_type') . ' grid-type-' . $a . ' grid-' . $b . ' ' . $k . ' ' . $l . ' branding-region-wrap branding-wrap"><div id="branding-region" class="branding branding-region">' . render($variables['page']['branding']) . '</div></div>';
	$a = '';$b = '';$c = '';$d = '';$k = '';$l = '';
	
$output .= '</div></div>';
$va = theme_get_setting('brd_weight') . 'a';
$variables['branding'][theme_get_setting('brd_pos')] = array($va => $output);


/**
 * Menu regions
 *
 */

$output = '';
$va		= '';
$output .= '<div id="menu-section" class="section menu-section"><div id="menu-zone" class="zone menu-zone">';

	$a = (theme_get_setting('container_grid_global') == 1) ? theme_get_setting('container_grid') : theme_get_setting('menu_grid');
	$b = theme_get_setting('menu_grid_' . $a . '_width');
	$c = theme_get_setting('menu_grid_' . $a . '_prefix');
	$d = theme_get_setting('menu_grid_' . $a . '_suffix');
	if($c != 0): $k = 'prefix-' . $c . ''; else: $k = '';endif;
	if($d != 0): $l = 'suffix-' . $d . ''; else: $d = '';endif;
	$main_menu = theme_get_setting('toggle_main_menu') ? theme('links__system_main_menu', array('links' => menu_main_menu(),'attributes' => array('id' => 'main-menu','class' =>array('links', 'inline', 'main-menu')))) : '';
	$secondary_menu = theme_get_setting('toggle_secondary_menu') ? theme('links__system_secondary_menu',array('links' => menu_secondary_menu(),'attributes' => array('id' => 'subnav','class' =>array('links', 'inline', 'secondary-menu')))) : '';
	
	if($main_menu || $secondary_menu){
	$output .= '<nav id="navigation" class="hover ' . theme_get_setting('layout_type') . ' grid-type-' . $a . ' grid-' . $b . ' ' . $k . ' ' . $l . ' navigation">' . $main_menu . $secondary_menu . '</nav>';
	}
	$output .= '<div id="menu-region-wrap" class="hover ' . theme_get_setting('layout_type') . ' grid-type-' . $a . ' grid-' . $b . ' ' . $k . ' ' . $l . ' menu-region-wrap menu-wrap"><div id="menu-region" class="menu menu-region">' . render($variables['page']['menu']) . '</div></div>';
	$a = '';$b = '';$c = '';$d = '';$k = '';$l = '';
	
$output .= '</div></div>';
$va = theme_get_setting('menu_weight') . 'b';
$variables['menu'][theme_get_setting('menu_pos')] = array($va => $output);

/**
 * Preface regions
 *
 */
if(!(theme_get_setting('preface_i') == 0)){
$output = '';
$va		= '';
$output .= '<div id="preface-section" class="section preface-section"><div id="preface-zone" class="zone preface-zone">';
for ($i = 1; $i <= theme_get_setting('preface_i'); $i++) {

	$a = (theme_get_setting('container_grid_global') == 1) ? theme_get_setting('container_grid') : theme_get_setting('preface_grid_' . $i . '');
	$b = theme_get_setting('preface_grid_' . $a . '_width_' . $i . '');
	$c = theme_get_setting('preface_grid_' . $a . '_prefix_' . $i . '');
	$d = theme_get_setting('preface_grid_' . $a . '_suffix_' . $i . '');
	if($c != 0): $k = 'prefix-' . $c . ''; else: $k = '';endif;
	if($d != 0): $l = 'suffix-' . $d . ''; else: $d = '';endif;
	$output .= '<div id="preface-wrap-' . $i . '" class="hover ' . theme_get_setting('layout_type') . ' grid-type-' . $a . ' grid-' . $b . ' ' . $k . ' ' . $l . ' preface-wrap preface-wrap-' . $i . '"><div id="preface-' . $i . '" class="preface preface-' . $i . '">' . render($variables['page']['preface-' . $i . '']) . '</div></div>';
	$a = '';$b = '';$c = '';$d = '';$k = '';$l = '';
	
}
$output .= '</div></div>';
$va = theme_get_setting('preface_weight') . 'd';
$variables['preface'][theme_get_setting('preface_pos')] = array($va => $output);
}
/**
 * Bottom teaser regions
 *
 */
if(!(theme_get_setting('teaser_i') == 0)){
$output = '';
$va		= '';
$output .= '<div id="bottom-teaser-section" class="section bottom-teaser-section"><div id="bottom-teaser-zone" class="zone bottom-teaser-zone">';
for ($i = 1; $i <= theme_get_setting('teaser_i'); $i++) {

	$a = (theme_get_setting('container_grid_global') == 1) ? theme_get_setting('container_grid') : theme_get_setting('teaser_grid_' . $i . '');
	$b = theme_get_setting('teaser_grid_' . $a . '_width_' . $i . '');
	$c = theme_get_setting('teaser_grid_' . $a . '_prefix_' . $i . '');
	$d = theme_get_setting('teaser_grid_' . $a . '_suffix_' . $i . '');
	if($c != 0): $k = 'prefix-' . $c . ''; else: $k = '';endif;
	if($d != 0): $l = 'suffix-' . $d . ''; else: $d = '';endif;
	$output .= '<div id="bottom-teaser-wrap-' . $i . '" class="hover ' . theme_get_setting('layout_type') . ' grid-type-' . $a . ' grid-' . $b . ' ' . $k . ' ' . $l . ' bottom-teaser-wrap bottom-teaser-wrap-' . $i . '"><div id="bottom-teaser-' . $i . '" class="bottom-teaser bottom-teaser-' . $i . '">' . render($variables['page']['teaser-' . $i . '']) . '</div></div>';
	$a = '';$b = '';$c = '';$d = '';$k = '';$l = '';
	
}
$output .= '</div></div>';
$va = theme_get_setting('teaser_weight') . 'f';
$variables['teaser'][theme_get_setting('teaser_pos')] = array($va => $output);
}
/**
 * Footer regions
 *
 */
 if(!(theme_get_setting('ftr_i') == 0)){
$output = '';
$va		= '';
$output .= '<div id="footer-section" class="section footer-section"><div id="footer-zone" class="zone footer-zone">';
for ($i = 1; $i <= theme_get_setting('ftr_i'); $i++) {

	$a = (theme_get_setting('container_grid_global') == 1) ? theme_get_setting('container_grid') : theme_get_setting('ftr_grid_' . $i . '');
	$b = theme_get_setting('ftr_grid_' . $a . '_width_' . $i . '');	
	$c = theme_get_setting('ftr_grid_' . $a . '_prefix_' . $i . '');
	$d = theme_get_setting('ftr_grid_' . $a . '_suffix_' . $i . '');
	if($c != 0): $k = 'prefix-' . $c . ''; else: $k = '';endif;
	if($d != 0): $l = 'suffix-' . $d . ''; else: $d = '';endif;
	$output .= '<div id="footer-wrap-' . $i . '" class="hover ' . theme_get_setting('layout_type') . ' grid-type-' . $a . ' grid-' . $b . ' ' . $k . ' ' . $l . ' footer-wrap footer-wrap-' . $i . '"><div id="footer-' . $i . '" class="footer footer-' . $i . '">' . render($variables['page']['footer-' . $i . '']) . '</div></div>';
	$a = '';$b = '';$c = '';$d = '';$k = '';$l = '';
	
}
$output .= '</div></div>';
$va = theme_get_setting('ftr_weight') . 'g';
$variables['footer'][theme_get_setting('ftr_pos')] = array($va => $output);
}
/**
 * Content regions
 *
 */

$output = '';
$va		= '';
$go		= array();
$sdf	= array();
$ctn	= array();
$sds	= array();
$z='';$y='';$x='';$q='';$w='';$e='';
$z 		= theme_get_setting('ctn_pos');
$main_content_title = '';$main_content_tabs = '';$main_content_actions = '';

if($z == 'left'):
	$z='a';$y='b';$x='c';
elseif($z == 'center'):
	$z='b';$y='a';$x='c';
else:
	$z='c';$y='a';$x='b';
endif;

if(theme_get_setting('title_show')){
$tr_title = drupal_get_title();
if($tr_title){$main_content_title = '<h1 id="page-title" class="title">' . drupal_get_title() . '</h1>';}
}
if(theme_get_setting('tabs_show')){
$tr_tabs = render($variables['tabs']);
if ($tr_tabs){$main_content_tabs = '<div class="tabs">' . render($variables['tabs']) . '</div>';}
}
if(theme_get_setting('action_links_show')){
$tr_actions = render($variables['action_links']);
if ($tr_actions){$main_content_actions = '<div class="action-links">' . render($variables['action_links']) . '</div>';}
}
$output .= '<div id="content-section" class="section content-section"><div id="content-zone" class="zone content-zone">';
//-------- breadcrumb --------------------------------------------------------------
$bread = '';
$breadcrumb = drupal_get_breadcrumb();
if (!empty($breadcrumb)){
	
	if (!theme_get_setting('br_set_home')){
		array_shift($breadcrumb);
	}
	if (theme_get_setting('br_set_value')){
		$bread = '<h2 class="element-invisible">' . t('You are here') . '</h2>';
		$bread .= '<div class="hover breadcrumb grid-type-' . theme_get_setting('container_grid') . ' grid-' . theme_get_setting('container_grid') . '">' . implode(theme_get_setting('br_set_sprt'), $breadcrumb) . '</div>';
	}
}
$output .= $bread;
$bread = '';
$breadcrumb = '';
//-------- messages ----------------------------------------------------------------
$msg = drupal_set_message();
extract($msg,EXTR_PREFIX_ALL,"tr");

if(theme_get_setting('msg_show')){
if($tr_error || $tr_status || $tr_warning){
$output .= '<div id="messages" class=" grid-type-' . theme_get_setting('container_grid') . ' grid-' . theme_get_setting('container_grid') . '">';

if($tr_status){
	$tr_status_show = '<div class="hover messages status"><h2 class="element-invisible">Status message</h2><ul><li>' . implode('</li><li>',$tr_status) . '</li></ul></div>';
}
if($tr_warning){
	$tr_warning_show = '<div class="hover messages warning"><h2 class="element-invisible">Warning message</h2><ul><li>' . implode('</li><li>',$tr_warning) . '</li></ul></div>';
}
if($tr_error){
	$tr_error_show = '<div class="hover messages error"><h2 class="element-invisible">Error message</h2><ul><li>' . implode('</li><li>',$tr_error) . '</li></ul></div>';
}

$output .= $tr_status_show . $tr_warning_show . $tr_error_show . '</div>';
}
}
//-------- sidebar first -----------------------------------------------------------
	if(theme_get_setting('sdf_show')){
	$a = (theme_get_setting('container_grid_global') == 1) ? theme_get_setting('container_grid') : theme_get_setting('sdf_grid');
	$b = theme_get_setting('sdf_grid_' . $a . '_width');	
	$c = theme_get_setting('sdf_grid_' . $a . '_prefix');
	$d = theme_get_setting('sdf_grid_' . $a . '_suffix');
	if($c != 0): $k = 'prefix-' . $c . ''; else: $k = '';endif;
	if($d != 0): $l = 'suffix-' . $d . ''; else: $d = '';endif;
	$q .= '<aside id="sidebar-first-wrap" class="hover ' . theme_get_setting('layout_type') . ' grid-type-' . $a . ' grid-' . $b . ' ' . $k . ' ' . $l . ' content-wrapper-1-1 sidebar-first-wrap sidebar-wrap"><div id="sidebar-first" class="content-wrapper-1 sidebar-first sidebar">' . render($variables['page']['sidebar_first']) . '</div></aside>';
	$a = '';$b = '';$c = '';$d = '';$k = '';$l = '';$va	= '';
	$va = $y . 'a';
	$sdf = array($va => $q);}
//--------- main content ----------------------------------------------------------
	$a = '';$b = '';$c = '';$d = '';
	$a = (theme_get_setting('container_grid_global') == 1) ? theme_get_setting('container_grid') : theme_get_setting('ctn_grid');
	$b = theme_get_setting('ctn_grid_' . $a . '_width');	
	$c = theme_get_setting('ctn_grid_' . $a . '_prefix');
	$d = theme_get_setting('ctn_grid_' . $a . '_suffix');
	if($c != 0): $k = 'prefix-' . $c . ''; else: $k = '';endif;
	if($d != 0): $l = 'suffix-' . $d . ''; else: $d = '';endif;
	$w .= '<div id="content-wrap" class="hover ' . theme_get_setting('layout_type') . ' grid-type-' . $a . ' grid-' . $b . ' ' . $k . ' ' . $l . ' content-wrapper-1-1 content-wrap"><div id="main-content" class="content-wrapper-1 main-content">' . $main_content_title . $main_content_tabs . render($variables['page']['help']) . $main_content_actions . render($variables['page']['content']) . '</div></div>';
	$a = '';$b = '';$c = '';$d = '';$k = '';$l = '';$va	= '';
	$va =  $z . 'b';
	$ctn = array($va => $w);
//---------- sidebar second --------------------------------------------------------
	if(theme_get_setting('sds_show')){
	$a = (theme_get_setting('container_grid_global') == 1) ? theme_get_setting('container_grid') : theme_get_setting('sds_grid');
	$b = theme_get_setting('sds_grid_' . $a . '_width');	
	$c = theme_get_setting('sds_grid_' . $a . '_prefix');
	$d = theme_get_setting('sds_grid_' . $a . '_suffix');
	if($c != 0): $k = 'prefix-' . $c . ''; else: $k = '';endif;
	if($d != 0): $l = 'suffix-' . $d . ''; else: $d = '';endif;
	$e .= '<aside id="sidebar-second-wrap" class="hover ' . theme_get_setting('layout_type') . ' grid-type-' . $a . ' grid-' . $b . ' ' . $k . ' ' . $l . ' content-wrapper-1-1 sidebar-second-wrap sidebar-wrap"><div id="sidebar-second" class="content-wrapper-1 sidebar-second sidebar">' . render($variables['page']['sidebar_second']) . '</div></aside>';
	$a = '';$b = '';$c = '';$d = '';$k = '';$l = '';$va	= '';
	$va = $x . 'c';
	$sds = array($va => $e);}
//--------------------------------------------------------------------
$go = array_merge($sdf,$ctn,$sds);
ksort($go);
$a = implode(' ',$go);
$output .= $a;
$va	= '';
$z='';$y='';$x='';$q='';$w='';$e='';
$output .= '</div></div>';
$va = theme_get_setting('content_weight') . 'e';
$variables['content'][theme_get_setting('content_pos')] = array($va => $output);


 
 /**
 * Process before load
 *
 */

	$variables['top'] = array_merge($variables['branding']['top'],$variables['menu']['top'],$variables['header']['top'],$variables['preface']['top'],$variables['content']['top'],$variables['teaser']['top'],$variables['footer']['top']);
	$variables['middle'] = array_merge($variables['branding']['middle'],$variables['menu']['middle'],$variables['header']['middle'],$variables['preface']['middle'],$variables['content']['middle'],$variables['teaser']['middle'],$variables['footer']['middle']);
	$variables['bottom'] = array_merge($variables['branding']['bottom'],$variables['menu']['bottom'],$variables['header']['bottom'],$variables['preface']['bottom'],$variables['content']['bottom'],$variables['teaser']['bottom'],$variables['footer']['bottom']);
	ksort($variables['top']);
	ksort($variables['middle']);
	ksort($variables['bottom']);


	if($variables['top']){$variables['atas'] = implode(' ',$variables['top']);}
	if($variables['middle']){$variables['tengah'] = implode(' ',$variables['middle']);}
	if($variables['bottom']){$variables['bawah'] = implode(' ',$variables['bottom']);}

}

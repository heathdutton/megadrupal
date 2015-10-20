var loop_counter = 0;
jQuery(window).load(function(){
	var right_menu = jQuery('.sidebar .block-menu ul.menu');
	var counter = 0;
	if (right_menu) {
		var counter = 1;
		right_menu.find('li').each(function() {
			jQuery(this).addClass('menu-icon-' + counter);
			counter ++;
		});
	}
	check_loading_widgets();
}); 

function check_loading_widgets() {
	jQuery('#main-wrapper .left-group, #sidebar-first-wrapper').matchHeights();
	loop_counter ++;
	var loading = false;
	var has_widget = false;
	jQuery('.block.block-widgets .block-inner .block-content').each(function() {
		has_widget = true;
		if(jQuery(this).height() < 100) {
			loading = true;
		}
	});
	if(has_widget && loading && loop_counter < 10) {
		window.setTimeout(check_loading_widgets, 500);
	}
	jQuery('#main-wrapper .left-group, #sidebar-first-wrapper').matchHeights();
}

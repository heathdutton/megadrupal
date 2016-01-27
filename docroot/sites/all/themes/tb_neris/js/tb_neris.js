var tb_neris_sidebar_scrolling = false;
var tb_neris_sidebar = false;
var tb_neris_main_content = false;
var tb_neris_toolbar = false;

jQuery(window).load(function(){
  window.setTimeout(nucleus_set_equal_height, 100);
  jQuery('.block-user #user-login-form #edit-name').val(Drupal.t('Username'));
  jQuery(".block-user #user-login-form #edit-name").focus(function(){
    if(this.value == Drupal.t('Username')) {
      this.value='';
    }
  }).blur(function(){
    if(this.value == '') {
      this.value = Drupal.t('Username');
    }
  });
  jQuery('.block-user #user-login-form .form-item-pass').after('<div class="form-item form-type-password form-item-pass-tmp"><input type="text" class="form-text" maxlength="60" size="15" name="pass-tmp" id="edit-pass-tmp" value="' + Drupal.t('Password') + '"></div>');
  jQuery('.block-user #user-login-form .form-item-pass').hide();
  jQuery('#edit-pass-tmp').focus(function(){
	jQuery('.block-user #user-login-form .form-item-pass-tmp').hide();
    jQuery('.block-user #user-login-form .form-item-pass').show();
    jQuery('.block-user #user-login-form #edit-pass').focus();
  });
  jQuery('.block-user #user-login-form #edit-pass').blur(function(){
    if(this.value == '') {
      jQuery('.block-user #user-login-form .form-item-pass').hide();
	  jQuery('.block-user #user-login-form .form-item-pass-tmp').show();
    }
  });
  jQuery('ul.openid-links li.openid-link').click(function() {
    jQuery('.block-user #user-login-form .form-item-pass-tmp').hide();
  });
  jQuery('ul.openid-links li.user-link').click(function() {
    jQuery('.block-user #user-login-form .form-item-pass').hide();
    jQuery('.block-user #user-login-form .form-item-pass-tmp').show();
  });
  jQuery(".block input.form-text").focus(function(){
	if(jQuery(this).attr('id') != "edit-pass-tmp") {
	  jQuery(this).addClass('active');
	}
  }).blur(function(){
	jQuery(this).removeClass('active');
  });

  window.setTimeout(tb_make_movable_sidebar, 100);
}); 

function nucleus_set_equal_height() {
//	jQuery('key1, key2, key3, ...').equalHeightColumns();
	jQuery('#panel-first-wrapper .panel-column .block-inner').matchHeights();
	jQuery('.mass-bottom .views-col .grid-inner').matchHeights();
	jQuery('#panel-third-wrapper .panel-column > .grid-inner').matchHeights();
}

function tb_make_movable_sidebar() {
  tb_neris_sidebar = jQuery('#sidebar-first-wrapper > .grid-inner'); 
  tb_neris_main_content = jQuery('#main-wrapper > .container > .container-inner');
  tb_neris_toolbar = jQuery('.region-page-top > #toolbar');
  tb_scroll_page();
  jQuery(window).scroll(function() {
    window.setTimeout(tb_scroll_page, 500)
  });
  jQuery(window).resize(function() {
    window.setTimeout(tb_scroll_page, 500)
  });
}

function tb_scroll_page() {
  if(!tb_neris_sidebar_scrolling) {
    tb_neris_sidebar_scrolling = true;
    var sidebar_top = tb_neris_sidebar.offset().top;
    var sidebar_height = tb_neris_sidebar.height();
    var content_height = tb_neris_main_content.height();
    var content_top = tb_neris_main_content.offset().top;
    var current_top = jQuery(document).scrollTop();
    if(tb_neris_toolbar) {
      current_top += tb_neris_toolbar.height();
    }
    if(current_top > content_top) {
      if(current_top - content_top < content_height - sidebar_height ) {
        var margin = current_top - content_top; 
        tb_neris_sidebar.stop(true, false).animate({"margin-top": margin + "px"}, 500, 'jswing', function(){tb_neris_sidebar_scrolling = false;});      
      }
      else {
        var margin = content_height - sidebar_height;
        tb_neris_sidebar.stop(true, false).animate({"margin-top": margin + "px"}, 500, 'jswing', function(){tb_neris_sidebar_scrolling = false;});      
      }
    }
    else {
    	tb_neris_sidebar.stop(true, false).animate({"margin-top": "0px"}, 500, 'jswing', function(){tb_neris_sidebar_scrolling = false;});  
    }
  }
}

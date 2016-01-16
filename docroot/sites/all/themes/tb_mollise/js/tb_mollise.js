jQuery(window).load(function(){
  window.setTimeout(nucleus_set_equal_height, 100);
  var slideshow = jQuery('#slideshow-wrapper .view-slideshow .views-slideshow-cycle-main-frame');
  if(slideshow) {
    nucleus_customize_slideshow();
    jQuery(window).resize(nucleus_customize_slideshow);
  }
  jQuery('#search-block-form .button input.form-submit').val(" ");
  jQuery('.block-user #user-login-form input[name=name]').val(Drupal.t('Username'));
  jQuery(".block-user #user-login-form input[name=name]").focus(function(){
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
	if(jQuery(this).attr('id') != "edit-pass-tmp" && jQuery(this).attr('name') != 'search_block_form') {
	  jQuery(this).addClass('active');
	}
	if(jQuery(this).attr('name') == 'search_block_form') {
	  console.log(jQuery('#search_block_form'));
	  jQuery('#search-block-form').addClass('active');
	}
  }).blur(function(){
	jQuery(this).removeClass('active');
	if(jQuery(this).attr('name') == 'search_block_form') {
	  jQuery('#search-block-form').removeClass('active');
	}
  });

  var header_top = jQuery('#header-top-wrapper');
  var header_top_button = jQuery('#header-wrapper a.accordion').addClass('active');
  var header_top_height = header_top.height();
  header_top_button.click(function() {
    if(header_top_button.hasClass('active')) {
      header_top_button.removeClass('active');
      header_top.css({'overflow': 'hidden'});
      header_top.stop(true, false).animate({"height": "0px"}, 500, function(){
      });      
    }
    else {
      header_top.stop(true, false).animate({"height": header_top_height + "px"}, 500, function(){
        header_top_button.addClass('active');
        header_top.css({'overflow': 'visible'});
      });
    }
  });
}); 

function nucleus_loop_until_change_height(counter, sidebar_second_height){
  if(counter == 10) {
    return;
  }
  var sidebar_second_height_cur = jQuery('#sidebar-second-wrapper > .grid-inner').height();
  if(sidebar_second_height_cur != sidebar_second_height) {
    nucleus_sidebar_equal_height();
    return;
  }
  window.setTimeout(function(){
    nucleus_loop_until_change_height(counter + 1, sidebar_second_height);
  }, 500);
}


function nucleus_set_equal_height() {
	jQuery('#panel-fourth-wrapper .panel-column > .grid-inner').matchHeights();
	jQuery('#panel-seventh-wrapper .panel-column > .grid-inner').matchHeights();
	jQuery('#sidebar-home-second-wrapper .grid-inner, #sidebar-home-third-wrapper .grid-inner').matchHeights();
	jQuery('#sidebar-home-first-wrapper .grid-inner, .container-home-inner .container-inner').matchHeights();	
	jQuery('#sidebar-first-wrapper > .grid-inner, #sidebar-second-wrapper > .grid-inner, #main-content').matchHeights();
	jQuery('#panel-social-wrapper .grid-inner').matchHeights();
	jQuery('.quicktabs-wrapper ul.quicktabs-tabs li a').click(function(){
      window.setTimeout(nucleus_sidebar_equal_height, 500);
	});
	jQuery('.quicktabs-wrapper ul.quicktabs-tabs li a.ajax-processed').click(function(){
	  var sidebar_second_height = jQuery('#sidebar-second-wrapper > .grid-inner').height();
	  nucleus_loop_until_change_height(0, sidebar_second_height);
	});
	jQuery('.quick-accordion .ui-accordion-header').click(function(){
      window.setTimeout(nucleus_sidebar_equal_height, 500);
	});
	jQuery('.quicktabs-ui-wrapper .ui-state-default').click(function(){
      window.setTimeout(nucleus_sidebar_equal_height, 500);
	});
}

function nucleus_sidebar_equal_height() {
  jQuery('#sidebar-first-wrapper > .grid-inner, #sidebar-second-wrapper > .grid-inner, #main-content > .grid-inner').matchHeights();
}

function nucleus_customize_slideshow() {
	var screen_width = jQuery(window).width();
	var container_width = jQuery('div.container').width();
	var slideshow = jQuery('#slideshow-wrapper .view-slideshow .views-slideshow-cycle-main-frame');
	if(slideshow && screen_width > container_width) {
		slideshow.css({'width': screen_width + "px", 'overflow': 'hidden'});
	}
}


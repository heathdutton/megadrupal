jQuery(document).ready(function() {
    jQuery('#drupalchat-colorpicker1').farbtastic('#edit-drupalchat-chat-topbar-color');
	jQuery('#drupalchat-colorpicker2').farbtastic('#edit-drupalchat-chat-topbar-text-color');
	jQuery('#drupalchat-colorpicker3').farbtastic('#edit-drupalchat-font-color');
	jQuery('#edit-drupalchat-css').hide();
	jQuery("input[name=drupalchat_polling_method]").change(function() {
	    jQuery('#edit-drupalchat-external-api-key').attr('disabled', 'disabled');
	    jQuery('.form-item-drupalchat-external-api-key').hide();
		jQuery('#edit-drupalchat-css').hide();
		jQuery('.form-item-drupalchat-stop-word-list').hide();
		jQuery('.form-item-drupalchat-use-stop-word-list').hide();
		jQuery('.form-item-drupalchat-stop-links').hide();
		jQuery('.form-item-drupalchat-allow-anon-links').hide();
    jQuery('.form-item-drupalchat-allow-render-images').hide();
		jQuery('.form-item-drupalchat-show-admin-list').hide();
		//jQuery('.form-item-drupalchat-user-picture').hide();
		jQuery('.form-item-drupalchat-allow-single-message-delete').hide();
		jQuery('.form-item-drupalchat-allow-clear-room-history').hide();
		jQuery('.form-item-drupalchat-load-chat-async').hide();
		jQuery('.form-item-drupalchat-minimize-chat-user-list').hide();
    jQuery('.form-item-drupalchat-enable-search-bar').hide();
		jQuery('.form-item-drupalchat-allow-user-font-color').hide();
    jQuery('.form-item-drupalchat-anon-change-name').hide();
		jQuery('.form-item-drupalchat-user-latency').show();
		jQuery('.form-item-drupalchat-refresh-rate').show();
		jQuery('.form-item-drupalchat-rel').show();
		jQuery('.form-item-drupalchat-ur-name').show();
		if (jQuery("input[name=drupalchat_polling_method]:checked").val() == '0') {
	    	jQuery('#edit-drupalchat-refresh-rate').removeAttr('disabled');
	    	jQuery('#edit-drupalchat-refresh-rate-wrapper').fadeIn();	    	
	    }
	    else if (jQuery("input[name=drupalchat_polling_method]:checked").val() == '1') {
	    	jQuery('#edit-drupalchat-refresh-rate').attr('disabled', 'disabled');
	    	jQuery('#edit-drupalchat-refresh-rate-wrapper').hide();
	    }
		else if(jQuery("input[name=drupalchat_polling_method]:checked").val() == '3') {
		  jQuery('#edit-drupalchat-external-api-key').removeAttr('disabled');
	      jQuery('.form-item-drupalchat-external-api-key').fadeIn();
		  jQuery('#edit-drupalchat-css').fadeIn();
		  jQuery('.form-item-drupalchat-user-latency').hide();
		  jQuery('.form-item-drupalchat-refresh-rate').hide();
		  jQuery('.form-item-drupalchat-stop-word-list').show();
		  jQuery('.form-item-drupalchat-use-stop-word-list').show();
		  jQuery('.form-item-drupalchat-stop-links').show();
		  jQuery('.form-item-drupalchat-allow-anon-links').show();
      jQuery('.form-item-drupalchat-allow-render-images').show();
		  jQuery('.form-item-drupalchat-allow-single-message-delete').show();
		  jQuery('.form-item-drupalchat-allow-clear-room-history').show();
		  jQuery('.form-item-drupalchat-show-admin-list').show();
		  //jQuery('.form-item-drupalchat-user-picture').show();
		  jQuery('.form-item-drupalchat-load-chat-async').show();
		  jQuery('.form-item-drupalchat-minimize-chat-user-list').show();
      jQuery('.form-item-drupalchat-enable-search-bar').show();
		  jQuery('.form-item-drupalchat-allow-user-font-color').show();
      jQuery('.form-item-drupalchat-anon-change-name').show();
		  //jQuery('.form-item-drupalchat-rel').hide();
		  //jQuery('.form-item-drupalchat-ur-name').hide();
		}
	});

  jQuery("input[name=drupalchat_rel]").change(function() {
      if (jQuery("input[name=drupalchat_rel]:checked").val() == '1') {
        jQuery('#edit-drupalchat-ur-name').removeAttr('disabled');
		jQuery('#edit-drupalchat-ur-name').attr('required', 'true');
        jQuery('#edit-drupalchat-ur-name-wrapper').fadeIn();     
      }
      else {
        jQuery('#edit-drupalchat-ur-name').attr('disabled', 'disabled');
        jQuery('#edit-drupalchat-ur-name').removeAttr('required');
		jQuery('#edit-drupalchat-ur-name-wrapper').hide();
      }
  });

    jQuery("#edit-drupalchat-show-admin-list").change(function() {
      if (jQuery("#edit-drupalchat-show-admin-list").val() == '1') {
        jQuery('#edit-drupalchat-support').fadeIn();     
      }
      else {
        jQuery('#edit-drupalchat-support').hide();
      }
    });  
	
	jQuery("input[name=drupalchat_polling_method]").change();
	jQuery("input[name=drupalchat_rel]").change();
	jQuery("#edit-drupalchat-show-admin-list").change();
});


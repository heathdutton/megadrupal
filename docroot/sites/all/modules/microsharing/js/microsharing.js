(function ($) {
  Drupal.behaviors.microsharingWidget = {
    attach: function (context) {
      var icon_color = '#2ba6cb';
      if (jQuery ("#edit-microsharing-services-code-color-icon").val () != '') {
        icon_color = "#" + jQuery ("#edit-microsharing-services-code-color-icon").val ();
      }
      jQuery('#icon-color div').css('backgroundColor', icon_color);
      jQuery('#icon-color').ColorPicker({
      	color: icon_color,
      	onShow: function (colpkr) {
      		jQuery(colpkr).fadeIn(500);
      		return false;
      	},
      	onHide: function (colpkr) {
      		jQuery(colpkr).fadeOut(500);
      		refresh_widget();
      		return false;
      	},
      	onChange: function (hsb, hex, rgb) {
      		jQuery('#icon-color div').css('backgroundColor', '#' + hex);
      	}
      });
    
      var highlight_color = '#b2f4dd';
      if (jQuery ("#edit-microsharing-services-code-bg-icon").val () != '') {
        highlight_color = "#" + jQuery ("#edit-microsharing-services-code-bg-icon").val ();
      }
      jQuery('#highlight-color div').css('backgroundColor', highlight_color);
      jQuery('#highlight-color').ColorPicker({
      	color: highlight_color,
      	onShow: function (colpkr) {
      		jQuery(colpkr).fadeIn(500);
      		return false;
      	},
      	onHide: function (colpkr) {
      		jQuery(colpkr).fadeOut(500);
      		refresh_widget();
      		return false;
      	},
      	onChange: function (hsb, hex, rgb) {
      		jQuery('#highlight-color div').css('backgroundColor', '#' + hex);
      	}
      });
    
      var text_color = '#000000';
      if (jQuery ("#edit-microsharing-services-code-color-text").val () != '') {
        text_color = "#" + jQuery ("#edit-microsharing-services-code-color-text").val ();
      }
      jQuery('#text-color div').css('backgroundColor', text_color);
      jQuery('#text-color').ColorPicker({
      	color: text_color,
      	onShow: function (colpkr) {
      		jQuery(colpkr).fadeIn(500);
      		return false;
      	},
      	onHide: function (colpkr) {
      		jQuery(colpkr).fadeOut(500);
      		refresh_widget();
      		return false;
      	},
      	onChange: function (hsb, hex, rgb) {
      		jQuery('#text-color div').css('backgroundColor', '#' + hex);
      	}
      });		   
    
      var serviceBackground_color = '#ffffff';
      if (jQuery ("#edit-microsharing-services-code-background-text").val () != '') {
        serviceBackground_color = "#" + jQuery ("#edit-microsharing-services-code-background-text").val ();
      }
      jQuery('#serviceBackground-color div').css('backgroundColor', serviceBackground_color);
      jQuery('#serviceBackground-color').ColorPicker({
      	color: serviceBackground_color,
      	onShow: function (colpkr) {
      		jQuery(colpkr).fadeIn(500);
      		return false;
      	},
      	onHide: function (colpkr) {
      		jQuery(colpkr).fadeOut(500);
      		refresh_widget();
      		return false;
      	},
      	onChange: function (hsb, hex, rgb) {
      		jQuery('#serviceBackground-color div').css('backgroundColor', '#' + hex);
      	}
      });
      var page_sharing = jQuery ("#edit-microsharing-services-code-sharing-page").val ();
      if (page_sharing == 'true') {
        jQuery ("#page-sharing-on").attr ("checked","checked");
        jQuery ("#page-sharing-off").attr ("checked","");
      }else {
        jQuery ("#page-sharing-on").attr ("checked","");
        jQuery ("#page-sharing-off").attr ("checked","checked");
      }
      jQuery("#page-sharing-customize-panel input").change (function (){
        refresh_widget ();
      });
      var text_sharing = jQuery ("#edit-microsharing-services-code-sharing-quote").val ();
      if (text_sharing == 'true') {
        jQuery ("#text-sharing-on").attr ("checked","checked");
        jQuery ("#text-sharing-off").attr ("checked","");
      }else {
        jQuery ("#text-sharing-on").attr ("checked","");
        jQuery ("#text-sharing-off").attr ("checked","checked");
      }
      jQuery("#text-sharing-customize-panel input").change (function (){
        refresh_widget ();
      });
      var img_sharing = jQuery ("#edit-microsharing-services-code-sharing-images").val ();
      if (img_sharing == 'true') {
        jQuery ("#img-sharing-on").attr ("checked","checked");
        jQuery ("#img-sharing-off").attr ("checked","");
      }else {
        jQuery ("#img-sharing-on").attr ("checked","");
        jQuery ("#img-sharing-off").attr ("checked","checked");
      }
      jQuery("#img-sharing-customize-panel input").change (function (){
        refresh_widget ();
      });
      var icons = jQuery ("#edit-microsharing-services-code-social-services").val ();
      var icons_array = icons.split(',');
      for (var i in icons_array) {
        jQuery(".sharing-options[data-value='" + icons_array[i] +"']").addClass ('active');
      }
      jQuery("#sharing-icon-wrap span a").each (function (){
        jQuery(this).unbind("click").bind("click", function (){
          jQuery(this).parent().toggleClass('active');
    	    refresh_widget();
    	    return false;
        });
      });
      
      function refresh_widget () {
    	  if (jQuery("#page-sharing-on").is(":checked")) {
          my_settings.sidebar_sharing = true;
          jQuery("#edit-microsharing-services-code-sharing-page").val ("true");
        }else {
          my_settings.sidebar_sharing = false;
          jQuery("#edit-microsharing-services-code-sharing-page").val ("false");
        }
    	  if (jQuery("#text-sharing-on").is(":checked")) {
          my_settings.text_sharing = true;
          jQuery("#edit-microsharing-services-code-sharing-quote").val ("true");
        }else {
          my_settings.text_sharing = false;
          jQuery("#edit-microsharing-services-code-sharing-quote").val ("false");
        }
    	  if (jQuery("#img-sharing-on").is(":checked")) {
          my_settings.image_sharing = true;
          jQuery("#edit-microsharing-services-code-sharing-images").val ("true");
        }else {
          my_settings.image_sharing = false;
          jQuery("#edit-microsharing-services-code-sharing-images").val ("false");
        }
    	  my_settings.service_color = new String (rgb2hex(jQuery('#icon-color div').css('backgroundColor'))).substring(1);
    	  jQuery("#edit-microsharing-services-code-color-icon").val (my_settings.service_color);
    	  my_settings.highlight_color = new String (rgb2hex(jQuery('#highlight-color div').css('backgroundColor'))).substring(1);
    	  jQuery("#edit-microsharing-services-code-bg-icon").val (my_settings.highlight_color);
    	  my_settings.service_background = new String (rgb2hex(jQuery('#serviceBackground-color div').css('backgroundColor'))).substring(1);
    	  jQuery("#edit-microsharing-services-code-background-text").val (my_settings.service_background);
    	  my_settings.text_color = new String (rgb2hex(jQuery('#text-color div').css('backgroundColor'))).substring(1);
    	  jQuery("#edit-microsharing-services-code-color-text").val (my_settings.text_color);
    	  var services = [];
    	  jQuery("#sharing-icon-wrap span.active").each (function (){
    	    services.push (jQuery(this).attr('data-value'));
    	    my_settings.services = services.join(",");
    	    my_settings.image_services = services.join(",");
    	    jQuery("#edit-microsharing-services-code-social-services").val (services.join(","));
    	  });
    	  
    	  //update textarea
    	  var _my_settings = '<script src="//cdn.4qu.co/w.js"><\/script>\n'+
    			'<script type="text/javascript">\n'+
    			'var my_settings = {\n'+
      		    '	"services": "{{services}}",\n'+
      		    '	"image_services": "{{image_services}}",\n'+
      		    '	"highlight_color": "{{highlight_color}}",\n'+
      		    '	"service_color": "{{service_color}}",\n'+
      		    '	"service_background": "{{service_background}}",\n'+
      		    '	"hover_color": "{{hover_color}}",\n'+
      		    '	"text_color": "{{text_color}}",\n'+
      		    '	"text_sharing": {{text_sharing}},\n'+
      		    '	"image_sharing": {{image_sharing}},\n'+
      		    '	"sidebar_sharing" : {{sidebar_sharing}},\n'+
      		    '	"image_sharing_position": "ne"\n'+
      		'};\n'+
    			'<\/script>\n';
    			_my_settings = _my_settings.replace (new RegExp("{{services}}",'g'), my_settings.services);
    			_my_settings = _my_settings.replace (new RegExp("{{image_services}}",'g'), my_settings.image_services);
    			_my_settings = _my_settings.replace (new RegExp("{{highlight_color}}",'g'), my_settings.highlight_color);
    			_my_settings = _my_settings.replace (new RegExp("{{service_color}}",'g'), my_settings.service_color);
    			_my_settings = _my_settings.replace (new RegExp("{{service_background}}",'g'), my_settings.service_background);
    			_my_settings = _my_settings.replace (new RegExp("{{hover_color}}",'g'), my_settings.hover_color);
    			_my_settings = _my_settings.replace (new RegExp("{{text_color}}",'g'), my_settings.text_color);
    			_my_settings = _my_settings.replace (new RegExp("{{text_sharing}}",'g'), my_settings.text_sharing);
    			_my_settings = _my_settings.replace (new RegExp("{{image_sharing}}",'g'), my_settings.image_sharing);
    			_my_settings = _my_settings.replace (new RegExp("{{sidebar_sharing}}",'g'), my_settings.sidebar_sharing);
    	  jQuery("#edit-microsharing-services-code").val(_my_settings);

    	  jQuery(".core-asset").remove(); 
    	  Core.init();
      };
      
      var hexDigits = new Array ("0","1","2","3","4","5","6","7","8","9","a","b","c","d","e","f"); 
    
    	//Function to convert hex format to a rgb color
    	function rgb2hex(rgb) {
    		rgb = rgb.match(/^rgb\((\d+),\s*(\d+),\s*(\d+)\)$/);
    		return "#" + hex(rgb[1]) + hex(rgb[2]) + hex(rgb[3]);
    	};
    	
    	function hex(x) {
    		return isNaN(x) ? "00" : hexDigits[(x - x % 16) / 16] + hexDigits[x % 16];
    	};
    }
  };
})(jQuery);
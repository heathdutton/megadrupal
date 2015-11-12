(function ($) {
  jQuery.fn.exists = function(){return this.length>0;}	
  Drupal.behaviors.topBarMsg = {
    attach: function(context, settings) {
		$(document).ready(function() {
	      var text = '<div id="topbarmsg-wrapper">';
	          text += '<div id="topbarmsg-container">';
	          text += '<div id="topbarmsg-content">'+ settings.TopBarMsg.message;
	          text += '<a href="'+ settings.TopBarMsg.link +'" title="'+ settings.TopBarMsg.link_text +'" target="'+ settings.TopBarMsg.link_target +'" id="topbarmsg-logo">'+ settings.TopBarMsg.link_text +'</a></div>';
			  text += '<a href="#close" id="topbarmsg-close">Close</a>';
			  text += '<div id="topbarmsg-shadow"></div></div>';
			  text += '</div><a href="#close" id="topbarmsg-open" style="">Open</a>';
		      $('body').prepend(text);
	          $('body').css({"padding-top" : "30px"});
              $('div#topbarmsg-container').css({"background-color" : settings.TopBarMsg.color_bg});
              $('a#topbarmsg-open').css({"background-color" : settings.TopBarMsg.color_bg});
              $('div#topbarmsg-container').css({"color" : settings.TopBarMsg.color_text});
              $('div#topbarmsg-container a#topbarmsg-logo').css({"color" : settings.TopBarMsg.color_link});
		   if (settings.TopBarMsg.autohide) {
			  var seconds = !isNaN(settings.TopBarMsg.autohide_seconds) ? settings.TopBarMsg.autohide_seconds*1000 : 5000;
	          setting_timeout(seconds);
	       }
		   $("#topbarmsg-open").click(function() {
	         open_topbar();
		   });
		   $("#topbarmsg-close").click(function() {
	         close_topbar();
		   });
		});
		function setting_timeout(seconds) {
		  return setTimeout(function() {
	       close_topbar();
	      }, seconds);
		}
		function close_topbar() {
		  if (!$('#toolbar').exists()) {	
		    $('body').animate({ 'padding-top' : '0' }, 'slow');
		  }
	      $("#topbarmsg-wrapper").slideUp();
	      $("#topbarmsg-open").slideDown();	
		}
		function open_topbar() {
	      if (!$('#toolbar').exists()) {	
		    $('body').animate({ 'padding-top' : '30' }, 'slow');	
		  }
		  $("#topbarmsg-wrapper").slideDown();
		  $("#topbarmsg-open").slideUp();	
		}
    }
  }
})(jQuery);
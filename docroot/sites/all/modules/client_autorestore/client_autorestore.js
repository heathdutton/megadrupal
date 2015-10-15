/**
 * @file
 * Used to increase the accuracy of the countdown.
 */
(function ($) {
  Drupal.behaviors.client_autorestore = {
    attach:function(context) {
		  var _time = Drupal.settings.client_autorestore.next_time;
		  var reset =  Math.round((_time - $.now()/1000)/60);
		  if(reset>2) {
			  $('div.messages .restore_time').once().text(reset);
		  } else {
			  $('div.messages').once().removeClass('messages--warning').addClass('messages--error').text('Warning: this website is about to be reset, please wait');
		  }
    }
  };
}(jQuery)); 
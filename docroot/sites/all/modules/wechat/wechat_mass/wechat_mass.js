/**
 * @file
 * A JavaScript file for the theme.
 *
 * In order for this JavaScript to be loaded on pages, see the instructions in
 * the README.txt next to this file.
 */

// JavaScript should be made compatible with libraries other than jQuery by
// wrapping it with an "anonymous closure". See:
// - http://drupal.org/node/1446420
// - http://www.adequatelygood.com/2010/3/JavaScript-Module-Pattern-In-Depth

jQuery.noConflict();  //将变量$的控制权让渡给prototype.js
(function($) { 
$(function() {
//alert('123123');
  $('#wechat-mass-message-node-form .field-name-field-message-type select').change(function () {
     var optionSelected = $(this).find("option:selected");
     var valueSelected  = optionSelected.val();
     if(valueSelected == 'image'){
	   $('#wechat-mass-message-node-form .field-name-field-image').css('display', 'none');
	   $('#wechat-mass-message-node-form .field-name-field-voice').css('display', 'none');
	   $('#wechat-mass-message-node-form .field-name-field-video').css('display', 'none');
	   $('#wechat-mass-message-node-form .field-name-body').css('display', 'none');
	   $('#wechat-mass-message-node-form .field-name-field-article').css('display', 'none');
	   $('#wechat-mass-message-node-form .field-name-field-image').css('display', 'block');
	 }
     if(valueSelected == 'text'){
	   $('#wechat-mass-message-node-form .field-name-field-image').css('display', 'none');
	   $('#wechat-mass-message-node-form .field-name-field-voice').css('display', 'none');
	   $('#wechat-mass-message-node-form .field-name-field-video').css('display', 'none');
	   $('#wechat-mass-message-node-form .field-name-body').css('display', 'none');
	   $('#wechat-mass-message-node-form .field-name-field-article').css('display', 'none');
	   $('#wechat-mass-message-node-form .field-name-body').css('display', 'block');
	 }	
     if(valueSelected == 'voice'){
	   $('#wechat-mass-message-node-form .field-name-field-image').css('display', 'none');
	   $('#wechat-mass-message-node-form .field-name-field-voice').css('display', 'none');
	   $('#wechat-mass-message-node-form .field-name-field-video').css('display', 'none');
	   $('#wechat-mass-message-node-form .field-name-body').css('display', 'none');
	   $('#wechat-mass-message-node-form .field-name-field-article').css('display', 'none');
	   $('#wechat-mass-message-node-form .field-name-field-voice').css('display', 'block');
	 }
     if(valueSelected == 'video'){
	   $('#wechat-mass-message-node-form .field-name-field-image').css('display', 'none');
	   $('#wechat-mass-message-node-form .field-name-field-voice').css('display', 'none');
	   $('#wechat-mass-message-node-form .field-name-field-video').css('display', 'none');
	   $('#wechat-mass-message-node-form .field-name-body').css('display', 'none');
	   $('#wechat-mass-message-node-form .field-name-field-article').css('display', 'none');
	   $('#wechat-mass-message-node-form .field-name-field-video').css('display', 'block');
	   $('#wechat-mass-message-node-form .field-name-body').css('display', 'block');
	 }
     if(valueSelected == 'news'){
	   $('#wechat-mass-message-node-form .field-name-field-image').css('display', 'none');
	   $('#wechat-mass-message-node-form .field-name-field-voice').css('display', 'none');
	   $('#wechat-mass-message-node-form .field-name-field-video').css('display', 'none');
	   $('#wechat-mass-message-node-form .field-name-body').css('display', 'none');
	   $('#wechat-mass-message-node-form .field-name-field-article').css('display', 'block');
	 }	 
  });
	

});
})(jQuery);


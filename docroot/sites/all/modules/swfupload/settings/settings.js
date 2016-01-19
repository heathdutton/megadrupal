$(function() {
  window.swfupload_label_heights = {};
  window.swfupload_wrapper_getting_smaller = {};
  $('#swfupload-settings-form label').each(function() {
    window.swfupload_label_heights[$(this).text()] = $(this).parent().find('.swf-nodetype-wrapper').height();
    $(this).parent().find('.swf-nodetype-wrapper').height(0);
    window.swfupload_wrapper_getting_smaller[$(this).text()] = false;
  });

	$('#swfupload-settings-form label, .swf-harmonica-arrow').css({'cursor':'pointer'}).click(function() {
		var content = $(this).parent().find('.swf-nodetype-wrapper').stop();
		var label = content.parent().find('label').text();
    var height = window.swfupload_label_heights[label];
		var arrow = $(this).parent().find('.swf-harmonica-arrow');
    var current_height = $(this).parent().find('.swf-nodetype-wrapper').height();

    if (content.css("display") == 'none' || window.swfupload_wrapper_getting_smaller[label]) {
      window.swfupload_wrapper_getting_smaller[label] = false;
			arrow.addClass('south');
  		content.css({'height':(current_height == height ? 0 : current_height) + 'px', 'display':'block'}).animate({'height':height + 'px'}, 1000);
    } else {
			arrow.removeClass('south');
      window.swfupload_wrapper_getting_smaller[label] = true;
  		content.animate({'height':'0px'}, 1000, function() {
  		  $(this).css({'display':'none', 'height':height + 'px'});
  		});
    };
	});
});
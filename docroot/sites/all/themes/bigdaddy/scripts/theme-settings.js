(function ($) {
$(document).ready(function() {

  //Display Viewport theme setting
  if ($('body').hasClass('display-viewport')) {
	  
	//create the HTML structure  
    $('body').append('<div id="viewport-values"><span id="width">WIDTH</span> x <span id="height">HEIGHT</span><br /><span id="ems"> EMs </span></div>');
 
    $(window).resize(function() {
	  var viewportwidth;
	  var viewportheight;
	  var widthEM;
	  if (typeof window.innerWidth != 'undefined') {
	    viewportwidth = window.innerWidth,
	    viewportheight = window.innerHeight,
	    widthEM = viewportwidth / 16
	  }
      $('#width').text(viewportwidth);
	  $('#height').text(viewportheight+' px');
	  $('#ems').text(widthEM+' em');
    }); 
  
  }

});
})(jQuery);




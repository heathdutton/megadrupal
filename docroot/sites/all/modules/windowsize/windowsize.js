(function($) {

$(document).ready(function(){
  var height = $(window).height();
  var width = $(window).width();

  //Display the current window width.
  $("body").prepend('<div class="size">Size: '+ width +'w x '+ height +'h</div>');
});


$(window).resize(function() {
  var height = $(window).height();
  var width = $(window).width();
  //Add the current window width on resizing the window.
  $("body .size").html('reSize: '+ width +'w x '+ height +'h');
});

})(jQuery);

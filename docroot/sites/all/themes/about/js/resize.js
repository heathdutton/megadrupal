;
(function($) {

  // refresh and rescale the background image on window resize 
  var resize_id = false;
  
  $(window).resize(function() {
    if (resize_id != false) {
      clearTimeout(resize_id);
    }
    resize_id = setTimeout(doResize, 700);
  });
  
  function doResize() {
    var h = $(window).height();
    var w = $(window).width();

    var cssbg = $('body').css('backgroundImage');
    cssbg = cssbg.replace(/\&w\=\d+/, '&w=' + w);
    cssbg = cssbg.replace(/\&h\=\d+/, '&h=' + h);
    //var i = new Image();
    //i.src = cssbg.slice(4, -1);
    $('body').css('backgroundImage', cssbg);
  }

})(jQuery);
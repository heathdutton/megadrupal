(function($) {
  var ua = $.browser;
  var heightAttr = (ua.msie && parseInt(ua.version.slice(0,1)) < 9) ? 'height' : 'min-height';
  
  $.fn.toEm = function(settings){
    settings = jQuery.extend({
      scope: 'body'
    }, settings);
    var that = parseInt(this[0],10);
    var teststr = '&nbsp;';
    for (var i=1; i<=99; i++) {
      teststr += '<br/>&nbsp;';
    }
    scopeTest = jQuery('<div style="display: none; font-size: 1em; margin: 0; padding:0; height: auto; line-height: 1; border:0;">' + teststr + '</div>').appendTo(settings.scope),
    scopeVal = scopeTest.height() / 100.0;
    scopeTest.remove();
    return (that / scopeVal).toFixed(8) + 'em';
  };

  $.fn.equalHeights = function(container) {
    tallest = 0;
    this.each(function() {
      if($(this).height() > tallest) {
        tallest = $(this).height();
      }
    });
    
    tallest = $(tallest).toEm({'scope' : container}); 

    return this.each(function() {
      $(this).css(heightAttr, tallest);
      if (heightAttr == 'height') {
        $(this).css('overflow', 'visible');
      }
    });
  }
  
  function setHeights() {
    $('.twocol, .threecol, .fourcol, .sixcol').each(function() {
      $('.col', this).equalHeights(this);
    });
  }
  $(document).ready(function() {
    setHeights();
  });
  $(window).load(function() {
    setHeights();
  });
  if(!($.browser.msie && parseInt($.browser.version, 10) == 7)) {
    $(window).resize(function() {
      setHeights();
    });
  }
})(jQuery);

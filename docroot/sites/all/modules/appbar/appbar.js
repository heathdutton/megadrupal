(function ($) {

Drupal.behaviors.appbar = {
  attach: function (context) {
    if (context == document) {
      $('body').addClass('with-appbar');
    }
    $('#appbar-container', context).show();
    if ($.fn.hoverIntent) {
      $('.appbar-block-hover', context).hoverIntent(appbarHoverOn, appbarHoverOff);
    }
    else {  
      $('.appbar-block-hover', context).hover(appbarHoverOn, appbarHoverOff);
    }
    $('.appbar-block-popup .appbar-block-title', context).click(function(e) {
      e.preventDefault();
      var content = $(this).prev('.appbar-block-content');
      var visible = content.slideToggle('fast').attr('display');
      if (visible != 'none') {
        $('.appbar-block-content:visible').not(content).hide();
      }
    });
    $('.appbar-block-popup .appbar-minimize', context).click(function(e) {
      e.preventDefault();
      $(this).parent().parent().slideToggle('fast');
    });
    $('.appbar-block-popup,.appbar-block-hover', context).each(function(index) {
      // The -1 is a cheap hack to make this look better when the border is 1px.
      var leftPos = $(this).offset().left - 1;
      $(this).find('.appbar-block-content').css('left', leftPos);
    });
    $('.appbar-block:first', context).addClass('first');
    $('.appbar-block:last', context).addClass('last');
    $('.appbar-block-controls', context).hover(
      function() { $('.appbar-block-configure').show(); },
      function() { $('.appbar-block-configure').hide(); }
    );
  }
}

function appbarHoverOn(event) {
  var content = $(event.currentTarget).find('.appbar-block-content');
  content.slideToggle('fast');
  // Hide other open blocks.
  $('.appbar-block-content:visible').not(content).slideUp('fast');
}

function appbarHoverOff(event) {
  $(event.currentTarget).find('.appbar-block-content').slideToggle('fast');
}

})(jQuery);

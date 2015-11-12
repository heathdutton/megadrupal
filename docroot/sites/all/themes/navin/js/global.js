// Global Navin JavaScript
;var Navin = Navin || {};

(function($, $$){
  $.extend($$, {}, {
    content: $(),
    footer: $(),
    init: function() {
      $$.content = $('#section-content');
      $$.content.height = $$.content.outerHeight(true);
      $$.footer = $('#zone-footer');
      // Search functions
      $$.initSearch();
      // Resize functions
      $(window).resize(function(){
        $$.resize();
      });
      $$.resize();
    },
    initSearch: function() {
      var form = $('form', $('#block-search-form'));
      if (form.length) {
        var text = form.find('.form-text');
        var submit = form.find('.form-submit');
        var search_text = Drupal.settings.navin.search_text ? Drupal.settings.navin.search_text : 'Search this site...';
        submit.attr('title', search_text);
        if (text.val() == '') {
          text.val(search_text);
        }
        text.focus(function(){
          if (text.val() == search_text) {
            text.val('');
          }
          text.addClass('focus');
          submit.addClass('focus');
        }).blur(function(){
          if (text.val() == '') {
            text.val(search_text);
          }
          text.removeClass('focus');
          submit.removeClass('focus').blur();
        });
        form.submit(function(){
          if (text.val() == search_text) {
            text.val('');
          }
        });
      }
    },
    attach: function(context) {
      // Drupal attachent behaviors
    },
    contentResize: function() {
      // @TODO: Finish out sticky footer for shorter content pages.
      // // Remove content stylings for mobile versions (as the content is wrapped).
      // if (Drupal.omega.getCurrentLayout() == 'mobile') {
      //   $$.content.removeAttr('style');
      // }
      // else {
      //   // Enlarge content height for small content.
      //   if ($$.content.length) {
      //     var height = $$.content.height;
      //     // Get body and document height gap, without the current footer's height
      //     var heightGap = Math.floor($(window).height() - ($('body').outerHeight(true) - $$.content.outerHeight(true)));
      //     // Resize footer to fill gap
      //     if (heightGap > 0) {
      //       height = heightGap;
      //     }
      //     // Ensure content is not hidden
      //     if (height < $$.content.height) {
      //       height = $$.content.height;
      //     }
      //     $$.content.css('height', height + 'px');
      //   }
      // }
    },
    rotatingBannerResize: function() {
      var banner = $('.views-slideshow-cycle-main-frame');
      var slide = $('.views-slideshow-cycle-main-frame-row', banner).filter(':visible');
      var image = $('img', slide);
      banner.height(image.height());
    },
    resize: function() {
      // Resize short content for anchoring footer at bottom.
      $$.contentResize();
      // Resize rotating banner height
      $$.rotatingBannerResize();
    }
  });
  $(document).ready(function(){
    $$.init();
  });
  Drupal.behaviors.navin = {
    attach: function(context){
      $$.attach(context);
    }
  };
})(jQuery, Navin);
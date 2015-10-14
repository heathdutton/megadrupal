/**
 * @file ajax_timeline.js
 *
 * Version: 1.x
 * Drupal Core: 7.x
 *
 */
(function($) {

  /**
   * Custom AJAX callback, triggered from ajax_command_invoke() calls.
   */
  $.fn.ajaxTimelineCallback = function(year, events) {

    if($('#year-' + year).hasClass('loaded')){
      // Remove current year events.
      $('#ajax-timeline .event-current-' + year).each(function(i){
        $(this).slideToggle(300);
      });
      
      $('#year-' + year).toggleClass('current-year');
      
      // Append 'See' to closed.
      if(!$('#year-' + year).hasClass('current-year')){
        $('#year-' + year).find('a').prepend(Drupal.t('SEE') + ' ');
      }
      else{
        $('#year-' + year).find('a').html(year);
      }
    }
    else{
      // Append new list.
      $('#year-' + year).addClass('current-year loaded').after(events).find('a').html(year);
    }
    
    // Scroll back up if list was long.
    if (!Drupal.settings.ajaxTimeline.infiniteScroll) {
      if($('#year-' + year).hasClass('current-year')){
        $('html, body').animate({ scrollTop: $('#year-' + year).offset().top }, 200);
      }
    }
  };
  
  /**
   *  Custom behaviors 
   */
  Drupal.behaviors.ajaxTimeline = {
    attach: function(context, settings) {
      
      // Load years on scroll
      if (Drupal.settings.ajaxTimeline.infiniteScroll) {
        $(window).scroll(function(e){
          checkScroll();
        });
      }
    }
  };
  
  /**
   * Check if we should be opening other
   * years based on scroll location
   */
  function checkScroll() {
    var scrollBottom = $(window).scrollTop() + $(window).height();
    $("#ajax-timeline .year:not(.loaded)").each(function(index, value){
      if (scrollBottom > $(this).offset().top + 10) {
        // Only load one at a time
        if (!$("#ajax-timeline .progress-disabled").length) {
          $(this).children('.use-ajax').click();
          return;
        }
      }
    });
  }

  // Check scroll on page load as well
  $(document).ready(function(e) {
    if (Drupal.settings.ajaxTimeline.infiniteScroll) {
      checkScroll();
    }  
  });
  
})(jQuery);

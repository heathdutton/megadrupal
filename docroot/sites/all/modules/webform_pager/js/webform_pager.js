/**
 * @file
 * Custom jQuery functions for webform_pager
 */

(function ($) {
  Drupal.behaviors.WebformPagerTooltip = {
    attach: function() {
      $("a.webform-pager-show-tooltip-link").click(function(){
        return false;
      });
      $("a.webform-pager-show-tooltip-link").each(function(i){
        $("body").append('<div class="webform-pager-show-tooltip" id="webform-pager-show-tooltip' + i + '"><p>' + $(this).attr('title') + '</p></div>');
        var my_tooltip = $("#webform-pager-show-tooltip" + i).hide();

          if ($(this).attr("title")) {
            $(this).removeAttr("title").mouseover(function(){
              my_tooltip.css({opacity:0.8, display:"none"}).show().fadeIn(400);
            }).mousemove(function(kmouse){
              my_tooltip.css({left:kmouse.pageX + 15, top:kmouse.pageY + 15});
            }).mouseout(function(){
              my_tooltip.fadeOut(400);
            });
          }
      });
    }
  };
})(jQuery);
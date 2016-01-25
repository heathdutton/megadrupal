// vim:et:sw=2
(function($) {

Drupal.tribune_block_popup = {};

Drupal.behaviors.tribune_block_popup = {
  attach: function(context) {
    $('.tribune-block-popup', context).each(function() {
      var close = $('<span class="tribune-block-popup-close">▼</span>');
      close.click(function() {
        $(this).parent().removeClass('tribune-block-popup-open');
        close.fadeOut();
        return false;
      });
      $(this).prepend(close);
      close.fadeOut();
      $(this).click(function() {
        $(this).addClass('tribune-block-popup-open');
        $(this).find('.tribune-block-popup-close').fadeIn();
      });
    });
  }
};
})(jQuery);

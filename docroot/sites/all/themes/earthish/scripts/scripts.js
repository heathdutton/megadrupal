// Add custom JS below. Note code this will be added to all pages on the site.

(function ($) {

  Drupal.behaviors.earthish = {
    attach: function (context, settings) {
      // We hide all but the first block content
      $('#footer-primary .content:gt(0)').hide();
      $('#footer-secondary .content:gt(0)').hide();
      // The the h3 serves as a toggle to view and hide
      $('#footer-primary h3').click(blockToggle).css('cursor', 'pointer');
      $('#footer-secondary h3').click(blockToggle).css('cursor', 'pointer');
      
      function blockToggle() {
        $(this).siblings('div.content').slideToggle();
      }
    }
  }

})(jQuery);


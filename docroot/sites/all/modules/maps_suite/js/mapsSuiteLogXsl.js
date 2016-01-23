(function ($) {

/**
 * Provide some user friendly functionnalities to navigate through
 * MaPS Suite Log contexts and messages.
 */

Drupal.behaviors.mapsSuiteLogXsl = {
  attach: function(context, settings) {
    $('.children-container').hide();

    $('.context-title, .message-title').bind('click', function() {
      var container = $(this).parent();

      container.children('.children-container').toggle();

      if (container.hasClass('wrapper-opened')) {
        container.addClass('wrapper-closed').removeClass('wrapper-opened');
      }
      else {
        container.addClass('wrapper-opened').removeClass('wrapper-closed');
      }
    });

    $('#fold-tree').bind('click', function(e, f) {
      $('.children-container').hide();
      $('.children-container').parent().addClass('wrapper-closed').removeClass('wrapper-opened');
    });

    $('#unfold-tree').bind('click', function() {
      $('.children-container').show();
      $('.children-container').parent().addClass('wrapper-opened').removeClass('wrapper-closed');
    });
  }
};

})(jQuery);

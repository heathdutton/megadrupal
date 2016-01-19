/**
 * @file
 * Dropkick init js.
 */

(function($) {
  $.fn.dropkick = function () {
    var args = Array.prototype.slice.call(arguments);
    return $(this).each(function() {
      if (!args[0] || typeof args[0] === 'object') {
        new Dropkick(this, args[0] || {});
      } else if (typeof args[0] === 'string') {
        Dropkick.prototype[args[0]].apply(new Dropkick(this), args.slice(1));
      }
    });
  };

  Drupal.behaviors.dropkick = {
    attach: function(context) {
      $(Drupal.settings.dropkick.selector, context)
         // Disable dropkick on field ui.
        .not('#field-ui-field-overview-form select, #field-ui-display-overview-form select')
        .each(function() {
          var $element = $(this);
          $element.dropkick({
            mobile: Drupal.settings.dropkick.mobile_support,
            change: function() {
              $element.change();
            },
          });
      });

      // Add clearfix to parent .form-item to fix floats.
      $('.dk_container').parents('.form-item').addClass('clearfix');
    }
  }
})(jQuery);

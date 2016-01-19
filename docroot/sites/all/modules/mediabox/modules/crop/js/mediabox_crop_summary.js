(function ($) {

"use strict";

Drupal.behaviors.takealookFieldsetSummeries = {
  attach:function (context) {
    $('fieldset', context).drupalSetSummary(function (context) {
      var state = $('input', context).serialize();

      setInterval(function () {
        if (state !== $('input', context).serialize()) {
          state = $('input', context).serialize();

          if ($('input.save:checked', context).length) {
            $(context).closest('fieldset').drupalSetSummary(function (context) {
              return '<span class="green">' + Drupal.t('Changed. Marked for save.') + '</span>'
            });
          } else {
            $(context).closest('fieldset').drupalSetSummary(function (context) {
              return '<span class="red">' + Drupal.t('Changed. Not marked for save.') + '</span>'
            });
          }
        }
      }, 500);

      return '<span>' + Drupal.t('Original state') + '</span>';
    });
  }
};

})(jQuery);

;(function ($) {

  "use strict";

  Drupal.behaviors.restoreUI = {
    attached: false,
    attach: function (context) {
      if (!this.attached) {
        this.attached = true;

        $(document).delegate('#operations-tab-content tr.selection-header', 'click', function () {
          var grp = $(this).attr('data-op-group'),
              item = $('#operations-tab-content tr.selection-row[data-op-group="' + grp + '"]');

          item.toggle();
        });
      }

      $('#operations-tab-content tr.selection-row').each(function (index, element) {
        if (context == document || (
          typeof context.hasClass === 'function' &&
          (context.hasClass('operation-selection-items') || context.attr('id') == 'operations-tab-content')
        )) { 
          if (typeof context.hasClass === 'function' &&
            context.parents('#operations-tab-content tr.selection-row')[0] == this
          ) {
            $(element).show();
          } else {
            $(element).hide();
          }
        }
      });
    }
  };

})(jQuery);

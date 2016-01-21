(function ($) {
  Drupal.behaviors.jipi = {
    attach: function (context) {
      $('.form-type-imagepicker select', context).once('init-imagepicker', function () {
        var imagepickerParams = [];
        var imagepickerAvailableParams = [
          'hide_select',
          'show_label'
        ];

        // imagepicker params are extracted from data attributes
        for (var key in imagepickerAvailableParams) {
          var paramKey = imagepickerAvailableParams[key];
          if (typeof $(this).attr('data-' + paramKey) != 'undefined') {
            imagepickerParams[paramKey] = $(this).attr('data-' + paramKey);
          }
        }

        $(this).imagepicker(imagepickerParams);
      });
    }
  };
}(jQuery));

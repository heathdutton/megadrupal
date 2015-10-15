/**
 * @file
 * JavaScript code to implement Nice ImageField Widget.
 */

(function ($) {
  Drupal.behaviors.NiceImageFieldWidget = {
    attach: function (context, settings) {

      $("div.nice-imagefield-sortable").each(function() {
        nice_imagefield_widget_sort(this);
        $(this).sortable({
          items: '> div',
          opacity: 0.7,
          placeholder: "ui-sortable-placeholder",
          revert: 100,
          stop: function() {
            nice_imagefield_widget_sort(this);
          }
        });

        //$(this).disableSelection();
      });

      function nice_imagefield_widget_sort(wrapper) {
        var i = 0;
        $('div.nice-imagefield-weight input', wrapper).each(function () {
          $(this).val(i);
          i++;
        });
      }

      $(".nice-imagefield-card:not(.processed)").each(function() {
        $(this).flip({
          axis: 'x',
          trigger: 'manual'
        });

        var element = $(this);
        element.find('input.flip-back').mousedown(function() {
          $(".nice-imagefield-card").each(function() {
            $(this).flip(false);
          });
          element.flip(true);
        });

        element.find('input.flip-front').mousedown(function() {
          element.flip(false);
        });

        element.find('input[id*="remove-button"]').mousedown(function() {
          element.find('.nice-imagefield-image').addClass('removing');
        });
      }).addClass('processed');

    }
  };
})(jQuery);

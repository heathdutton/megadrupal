(function ($) {

  Drupal.behaviors.checked_input = {
    attach:function (context, settings) {
      $('input[type="radio"], input[type="checkbox"]', context).once('checked-input', function() {
        var element = $(this);

        if (element.is(':checked')) {
          element.closest('.form-item').addClass('checked');
        }

        element.bind('click change', function() {
          if (element.attr('type') === 'checkbox') {
            element.closest('.form-item').toggleClass('checked', element.is(':checked'));
          }
          else {
            var form = element.closest('form');
            var name = element.attr('name');
            form.find('input[name="' + name + '"]').each(function() {
              var radio = $(this);
              radio.closest('.form-item').toggleClass('checked', radio.is(':checked'));
            });
          }
        });

      });
    }
  };

})(jQuery);

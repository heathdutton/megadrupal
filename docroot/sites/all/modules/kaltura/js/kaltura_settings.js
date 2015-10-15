(function ($) {
    Drupal.behaviors.kalturaSettingsCollaps = {
      attach: function (context, settings) {
        $('.advanced div').not('.form-type-item').hide();
        $('.advanced  div.form-type-item label:first-child').click( function () {
          $(this).parent().next().slideToggle();
        });
        }
    };
})(jQuery);


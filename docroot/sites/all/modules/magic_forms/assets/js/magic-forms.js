(function ($) {
  $(document).ready(function () {
    console.info(Drupal);

    function placeholderIsSupported() {
      var element = document.createElement('input');
      return ('placeholder' in element);
    }

    $('form.magic-form input[name="form_build_id"]').each(function (index, element) {
      var $element = $(element),
          form = $element.parents('form')[0],
          settings = Drupal.settings.magic_forms[$element.val()] || {};

      console.info($element.val(), form, settings);
      if (settings['description-placeholder'] && !placeholderIsSupported()) {
        console.error('PLACEHOLDER NOT SUPPORTED!');

        // @todo
        $('form#' + form.id + ' input[placeholder]')
          .load(function () {
              var input = $(this);
              if (input.val() == '')
              {
                  input.addClass('placeholder');
                  input.val(input.attr('placeholder'));
              }
          })
          .focus(function () {
              var input = $(this);
              if (input.val() == input.attr('placeholder')) {
                  input.val('');
                  input.removeClass('placeholder');
              }
          })
          .blur(function () {
              var input = $(this);
              if (input.val() == '' || input.val() == input.attr('placeholder')) {
                  input.addClass('placeholder');
                  input.val(input.attr('placeholder'));
              }
          })
          .blur().parents('form').submit(function () {
              $(this).find('form#' + form.id + ' [placeholder]').each(function () {
                  var input = $(this);
                  if (input.val() == input.attr('placeholder')) {
                      input.val('');
                  }
              })
          });
      }
    });
  });
})(jQuery);

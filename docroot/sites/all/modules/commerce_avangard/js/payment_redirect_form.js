(function ($) {

  "use strict";
  Drupal.ajax.prototype.commands.avangard_form_post = function (ajax, response) {
    $("form[id^='commerce-avangard'] input[name='ticket']").val(response.ticket);
    var redirect_form = $("form[id^='commerce-avangard']");
    redirect_form.attr('action', response.external_action);
    redirect_form.submit();
  };

})(jQuery);

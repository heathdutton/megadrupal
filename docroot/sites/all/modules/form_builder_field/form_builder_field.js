
(function($) {

  Drupal.behaviors.form_builder_field = {
    attach: function() {
      $('a.form-builder-field-open-form')
        .button()
        .click(function() {
          $(".form-builder-field-progress").addClass("throbber");
          document.body.style.cursor = 'wait';

          var form_id = $(this).data('formid');
          $.getJSON(Drupal.settings.form_builder_field.base_url + '/form-builder-field/' + form_id, function(data) {
            var html = data.data;
            $(".form-builder-field-form").html(html).dialog("open");
            Drupal.attachBehaviors();
            $(".form-builder-field-progress").removeClass("throbber");
            document.body.style.cursor = 'default';
            return false;
          });

          return false;
        });

      $('.form-builder-field-form')
        .dialog({
          autoOpen: false,
          modal: true,
          height: 500,
          width: 800
        });
    }
  }

})(jQuery);

/**
 * @file
 * Javascript to support front-end block of document_library module.
 */

(function ($) {
  Drupal.behaviors.documentlibrary = {
    attach: function(context, settings) {
      settings = settings.document_library;

      // Bind focus event for the search field.
      $("#edit-query", context).focus(
        function() {
          if ($("#edit-query").val().toLowerCase() == settings.search_label.toLowerCase()) {
            $("#edit-query").val("");
          }
        }
      );

      // Bind blur event for the search field.
      $("#edit-query", context).blur(
        function() {
          if ($("#edit-query").val() == "" || $("#edit-query").val().toLowerCase() == settings.search_label.toLowerCase()) {
            $("#edit-query").val(settings.search_label);
          }
        }
      );

      // Auto-submit the sort form, if settings dictate.
      if (settings.search_auto_submit == 1) {
        $("#edit-sort", context).change(
            function() {
              $("#edit-sort-submit").click();
            }
        );
      }

      // Handle the show/hide feature of the filter form.
      if (settings.show_filter_form == 1) {
        $("#document-library-filter-form-toggle", context).click(
          function(e) {
            e.preventDefault();

            if (settings.filter_form_status == 1) {
              settings.filter_form_status = 0;
              $("#document-library-filter-form-toggle").text(settings.show_advanced_search);
            } else {
              settings.filter_form_status = 1;
              $("#document-library-filter-form-toggle").text(settings.hide_advanced_search);
            }

            $("#document-library-filter-form-container").toggle();
          }
        );
      }
    }
  };
})(jQuery);

(function ($) {

/**
 * Shows sugarcrm filed on module change and set related module array.
 */
Drupal.behaviors.drupaltosugar = {
  attach: function (context) {
    // Add class to drupaltosugar menu tabs.
    $.each($('.page-admin-drupaltosugar #branding ul li a'), function(e,v) {
      var tab_text = $(this).text().toLowerCase();
      var text_to_add = tab_text.replace(" ", "-");
      // Removing '(active tab)' text from tab that is active.
      var final_class = text_to_add.split('\(');
      $(this).parent().addClass(final_class[0]);
    });
    $('.page-admin-drupaltosugar #branding').addClass('branding-drupaltosugar');
    $('.page-admin-drupaltosugar #page').addClass('page-drupaltosugar');

    var drupaltosugar = window.location.pathname;
    $('#edit-webform').change(function() {
      var webform_type = $('#edit-webform').val();
      if (webform_type == ' ') {
        var url = Drupal.settings.url + '/admin/drupaltosugar/module_configuration/';
        document.location = url;
      }
      else {
        var url = Drupal.settings.url + '/admin/drupaltosugar/module_configuration/' + webform_type;
        document.location = url;
      }
    });

    $('#edit-sugarmodule').change(function() {
      var add_related_url = Drupal.settings.url + '/admin/add_related_module';
      var sugarmodule_name = $('#edit-sugarmodule').val();
      // Setting SESSION variable to store related Module array in single hit.
      $.ajax({
        url: add_related_url,
        type: 'POST',
        dataType: "html",
        data: {'module' : sugarmodule_name },
        success:function(result) {
          var webform_type = $('#edit-webform').val();
          if (sugarmodule_name == ' ') {
            var url = Drupal.settings.url + '/admin/drupaltosugar/module_configuration/' + webform_type + '/';
            document.location = url;
          } else {
            var url = Drupal.settings.url + '/admin/drupaltosugar/module_configuration/' + webform_type + '/' + sugarmodule_name;
            document.location = url;
          }
        },
      });
    });
  },
};
})(jQuery);

(function ($) {

  // enable/disable codeblocks using AJAX call
  Drupal.behaviors.smarttribuneSnippet = {
    attach: function (context, settings) {
      $('.smarttribune-snippet-disable-link', context).click(function () {
        anchor = $(this);
        delta = anchor.attr('rel');
        loading = $('<div class="smarttribune-snippet-loading"></div>');

        anchor.after(loading);
        ajaxResponse = $.getJSON(Drupal.settings.basePath + 'admin/structure/smarttribune_snippet/' + delta + '/disable', function(response) {
          if (response.status) {
            anchor.parents('tr').removeClass('smarttribune-snippet-disabled');
          } else {
            anchor.parents('tr').addClass('smarttribune-snippet-disabled');
          }
          anchor.html(response.label);
          loading.remove();
        });

        return false;
      });
    }
  };

}(jQuery));

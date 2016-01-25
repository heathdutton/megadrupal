(function ($) {
Drupal.behaviors.powertagging = {
  attach: function (context) {

    // check if the individual sOnr configuration is available
    $("div.pp-led").each(function() {
      var $this = $(this);
      var url = Drupal.settings.basePath + "admin/config/services/powertagging/" + $this.attr('data-id') + "/available";
      $.get(url, function (data) {
        if (data == 1) {
          var led = 'led-green';
          var title = 'Service available';
        }
        else {
          var led = 'led-red';
          var title = 'Service NOT available';
        }
        $this.addClass(led);
        $this.attr('title', title);
      });
    });

    if ($("fieldset#edit-batch-jobs").length > 0) {
      $('input#edit-batch-jobs-index').click(function(e) {
        e.preventDefault();
        
        $(this).siblings('table').children('tbody').find('input:checkbox:checked').each(function() {
          $(this).attr('checked', false);
          $(this).closest('table').children('thead').find('input:checkbox:checked').attr('checked', false);
          $(this).closest('tr').removeClass('selected');
          // Index here.
          
          // Check if indexing or synchronization of this project already runs. (also error if project doesn't exist)
          $.getJSON(Drupal.settings.basePath + 'powertagging/indexProject/' + $(this).val(), function(result_data) {
            if (result_data.success) {
              console.log(result_data.message);
            }
            else {
              console.log(result_data.message);
            }
          });
        });
      });
    }
  }
};
})(jQuery);

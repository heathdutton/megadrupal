(function ($) {

  "use strict";
  
  Drupal.behaviors.mean = {
    attach: function (context) {
      // mean json type selected at /admin/build/mean-packets/add
      // update description
      $('#add_new_mean_packet_box select').change(function(){
        var val = $(this).val();
        $('#add_new_mean_packet_box .description').text(Drupal.settings.meanTypes[val].description);
        // clear fields
        $('#add_new_mean_packet_box .form-text').val('');
      });
      // submit new mean_packet line
      $('#add_new_mean_packet_box .form-submit').click(function(){
        var name = $('#add_new_mean_packet_box .name').val();
        if (name) {
          var line = $('#add_new_mean_packet_box select').val() + '/' + name
          if ($('#add_new_mean_packet_box .arg1').val()) {
            line += '/' + $('#add_new_mean_packet_box .arg1').val();
          }
          if ($('#add_new_mean_packet_box .arg2').val()) {
            line += '/' + $('#add_new_mean_packet_box .arg2').val();
          }
          if ($('#add_new_mean_packet_box .arg3').val()) {
            line += '/' + $('#add_new_mean_packet_box .arg3').val();
          }
          var newLine = $('#edit-mean-packets').text() ? "\n" : '';
          $('#edit-mean-packets').text($('#edit-mean-packets').text() + newLine + line);
        }
      });
    }
  }
}(jQuery));
(function ($) {

  Drupal.behaviors.ppCrop = {
    attach: function(context) {
      $('input[name=thumbnails]').once('ppCrop').click(function(){
        var $radios_value = $('input[name=thumbnails]:checked').val();
        if ($radios_value == $(this).val()) {
          $('#edit-format').val($radios_value);
        }
      });
      var $select_value = $('#edit-format').val();
      $('input[name=thumbnails]').each(function(){
        if ($(this).val() == $select_value) {
          $(this).click();
        }
      });
      $('.form-item-format').hide();
    }
  };

  // Reload formatter selection iframe after cropping dialog
  // has been closed so new thumbnails got rendered.
  Drupal.EPSACropOld = jQuery.extend(true, {}, Drupal.EPSACrop);
  Drupal.EPSACrop.dialog = function(type_name, field_name, bundle, delta, img, trueSize) {
    Drupal.EPSACropOld.dialog(type_name, field_name, bundle, delta, img, trueSize);
    $('#EPSACropDialog').bind('dialogclose', function(){
      $(document).ajaxComplete(function() {
        document.location.reload();
      });
      $('img.pp-crop-thumbnail').each(function(){
        var src = $(this).attr('src');
        var rand = Math.floor((Math.random()*1000)+1);
        if (src.indexOf('?')) {
          src = src + '&rand=' + rand;
        }
        else {
          src = src + '?rand=' + rand;
        }
        $(this).attr('src', src);
      });
    });
  };

})(jQuery);

(function ($) {
  Drupal.behaviors.initSimplePinMap = {
    attach: function (context) {
      var simplepinUrl = $('#simplepinmap-url').attr('value');
      var simplepinMarker = $('#simplepinmap-marker-id').attr('value');
      function closeSimpleSlideItems(pos){
        $('.simplepinmap-items').each(function(index){
          if (index != pos){
            $('#simplepinmap-item-content-' + index).slideUp();
          }
        });
        $('.simplepinmap-pins').each(function(index){
          if (index != pos){
            $(this).removeClass('pinactive');
            $(this).find('img:first').attr('src',simplepinUrl + '/markers/marker' + simplepinMarker + '.png');
          }
        });
      }
      $('#simplepinmap-pins-container li').click(function(){
        var pos = $('#simplepinmap-pins-container li').index(this);
        closeSimpleSlideItems(pos);
        if ($(this).hasClass('pinactive')){
          $(this).removeClass('pinactive');
          $(this).find('img:first').attr('src',simplepinUrl + '/markers/marker' + simplepinMarker + '.png');
        }
        else {
          $(this).addClass('pinactive');
          $(this).find('img:first').attr('src',simplepinUrl + '/markers/marker-active' + simplepinMarker + '.png');
        }
        $('#simplepinmap-items-container #simplepinmap-item-content-' + pos).slideToggle();
      });
      $('#simplepinmap-items-container li h3').click(function(){
        var pos = $('#simplepinmap-items-container li').index($(this).parent());
        closeSimpleSlideItems(pos);
        if ($('#simplepinmap-pin-' + pos).hasClass('pinactive')){
          $('#simplepinmap-pin-' + pos).removeClass('pinactive');
          $('#simplepinmap-pin-' + pos).find('img:first').attr('src',simplepinUrl + '/markers/marker' + simplepinMarker + '.png');
        }
        else {
          $('#simplepinmap-pin-' + pos).addClass('pinactive');

          $('#simplepinmap-pin-' + pos).find('img:first').attr('src',simplepinUrl + '/markers/marker-active' + simplepinMarker + '.png');
        }
        $('#simplepinmap-items-container #simplepinmap-item-content-' + pos).slideToggle();
      });
    }
  }
})(jQuery);

(function ($) {
  Drupal.behaviors.initSimplePinMap = {
    attach: function (context){

      $('#simplepinmap-marker').animate({
        opacity: 1.0,
        left: $('#edit-simplepinitem-x input').attr('value'),
        top: $('#edit-simplepinitem-y input').attr('value')
      });
      var mapWidth = $('#simplepin-edit-map').width();
      $('#simplepinmap-edit-layer').attr('style','width:' + mapWidth + 'px');
//Offset mouse Position
      $('#simplepinmap-edit-layer').click(function(e) {
        var xPr = $('#simplepinmap-marker').width() / 2;
        var yPr = $('#simplepinmap-marker').height();
        var posX = $(this).offset().left + xPr;
        var  posY = $(this).offset().top + yPr;

        $('#edit-simplepinitem-x input').attr('value',Math.round(e.pageX - posX,2).toFixed(2));
        $('#edit-simplepinitem-y input').attr('value',Math.round(e.pageY - posY,2).toFixed(2));
        $('#simplepinmap-marker').animate({
          opacity: 1.0,
          left: Math.round(e.pageX - posX,2).toFixed(2),
          top: Math.round(e.pageY - posY,2).toFixed(2)
        });
      });
    }
  }
})(jQuery);

(function ($) {

  Drupal.behaviors.Piecemaker = {
    attach: function (context, settings) {
      $.each(settings.Piecemaker, function($id, $piecemaker) {
        Drupal.Piecemaker.embed($id, $piecemaker, settings);
      });
    }
  }
  Drupal.Piecemaker = {
    embed: function($id, $piecemaker, settings) {
      $attributes = {
        id: $id + '-flash',
        styleclass: 'piecemaker ' + $id + '-style',
        name: $id,
      };
      $url = settings.Piecemaker_URI + '/piecemaker.swf';
      swfobject.embedSWF($url , $id, $piecemaker.width, $piecemaker.height, '10', false, $piecemaker.flashvars,
      $piecemaker.params, $attributes);
    },
  }
  
})(jQuery);
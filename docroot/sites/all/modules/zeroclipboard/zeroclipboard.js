(function ($) {
  Drupal.behaviors.zeroClipboard = {
    attach: function(context) {
      //ZeroClipboard.setMoviePath(Drupal.settings.basePath + Drupal.settings.zeroClipboard.moviePath);
      ZeroClipboard.config({'swfPath': Drupal.settings.zeroClipboard.moviePath,
                            'moviePath': Drupal.settings.zeroClipboard.moviePath});

      for (var i in Drupal.settings.zeroClipboard.selectorsToProcess) {
        var selectorData = Drupal.settings.zeroClipboard.selectorsToProcess[i];
        Drupal.zeroClipboard.process(selectorData.selector, selectorData.value, {});
      }
    }
  };

  Drupal.zeroClipboard = {};
  Drupal.zeroClipboard.clips = {};
  Drupal.zeroClipboard.currentIndex = 0;

  Drupal.zeroClipboard.process = function(selector, clipCallback) {
    $(selector).not('.zeroclipboard-processed').each(function() {
      var elementID = $(this).attr('id');
    
      // If the element to be processed doesn't already have an ID, generate one for it
      if (!elementID) {
        elementID = 'zeroclipboard-dynamic-id-' + Drupal.zeroClipboard.currentIndex;
        $(this).attr('id', elementID);
        Drupal.zeroClipboard.currentIndex++;
      }
    
      Drupal.zeroClipboard.clips[elementID] = new ZeroClipboard($('#' + elementID));
      Drupal.zeroClipboard.clips[elementID].on('ready', function(event) {
        Drupal.zeroClipboard.clips[elementID].on('beforecopy', function(event) {
          var newClipText = '';

          if (typeof(clipCallback) == 'function') {
            // On mouseDown (which is triggered exactly before mouseClick)
            // Set the clipText in the zeroclipboard flash using the clipCallback
            newClipText = clipCallback(elementID);
          }
          else if (typeof(clipCallback) == 'string') {
            newClipText = clipCallback;
          }
          Drupal.zeroClipboard.clips[elementID].setText(newClipText);
        });
      });

      // Mark this element as processed to prevent re-processing
      $(this).addClass('zeroclipboard-processed');
    });
  };
})(jQuery);

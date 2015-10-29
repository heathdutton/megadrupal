(function ($) {
/**
 * Attaches the imageflow javascript to our images.
 */
Drupal.behaviors.imageflow = {
  attach: function(context, settings) {
    $('[id^="imageflow-"]', context).once('imageflow', function() {
      var id = $(this).attr('id');
      var ops = settings.imageflow[id];
      // Eval the opacity array.
      if (ops['opacityArray']) {
        var flowOpacityArray = ops['opacityArray'];
        eval("ops['opacityArray'] = " + flowOpacityArray);
      }
      // Eval the onClick function.
      if (ops['onClick']) {
        var flowClick = ops['onClick'];
        eval("ops['onClick'] = " + flowClick);
      }
      var imageflow = new ImageFlow();
      imageflow.init(ops);
    });
  }
};

}(jQuery));

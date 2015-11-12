splashBlockSplash = (function(){
  var
  method = {},
  $overlay,
  $modal,
  $content,
  $close;

  // Generate the HTML and add it to the document.
  $overlay = jQuery('<div id="splash-block-overlay"></div>');
  $modal = jQuery('<div id="splash-block-modal"></div>');
  $content = jQuery('<div id="splash-block-splash"></div>');
  $close = jQuery('<a id="splash-block-close" href="#">close</a>');

  // Center the modal in the viewport.
  method.center = function () {
    var top, left;

    top = Math.max(jQuery(window).height() - $modal.outerHeight(), 0) / 2;
    left = Math.max(jQuery(window).width() - $modal.outerWidth(), 0) / 2;

    $modal.css({
      top:top + jQuery(window).scrollTop(),
      left:left + jQuery(window).scrollLeft()
    });
  };

  // Open the modal.
  method.open = function (settings) {
    jQuery('body').append($overlay, $modal);
    $modal.append($content, $close);

    $content.empty().append(settings.content).html();

    $modal.css({
      width: settings.width || 'auto',
      height: settings.height || 'auto'
    });

    method.center();
    jQuery(window).bind('resize.modal', method.center);
    $modal.show();
    $overlay.show();
  };

  // Close the modal.
  method.close = function () {
    $modal.hide();
    $overlay.hide();
    $content.empty();
    jQuery(window).unbind('resize.modal');
  };

  $close.click(function(e){
    e.preventDefault();
    method.close();
  });

  $overlay.click(function(){
    method.close();
  });

  return method;
}());

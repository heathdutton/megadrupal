(function ($) {

/**
* @file
* Javascript support files.
*
*/

Drupal.behaviors.blueimp = {
  attach: function (context, settings) {
    // Read URL paramters
    var urlParams = blueimp_readURLparams();
    // Create modal
    blueimp_create_modal('blueimp-gallery', Drupal.settings.blueimp);

    var trigger = Drupal.settings.blueimp.trigger;
    var module_path = Drupal.settings.blueimp.module_path;
    
    $( trigger ).find( "a" ).attr( "data-gallery", "#blueimp-gallery" );
  }
}

function blueimp_readURLparams() {
  (window.onpopstate = function () {
    var match,
      pl     = /\+/g,  // Regex for replacing addition symbol with a space
      search = /([^&=]+)=?([^&]*)/g,
      decode = function (s) { return decodeURIComponent(s.replace(pl, " ")); },
      query  = window.location.search.substring(1);

    urlParams = {};
    while (match = search.exec(query))
      urlParams[decode(match[1])] = decode(match[2]);
  })();
  return urlParams;
}

function blueimp_create_modal(id, p) {
  $('body').append('<div id="blueimp-gallery" class="blueimp-gallery">');
  if ( p.startControls ) { $('#blueimp-gallery').addClass( "blueimp-gallery-controls" ) };
  if ( p.carousel ) { $('#blueimp-gallery').attr( "data-carousel", "true" ) };
  if ( !p.continuous ) { $('#blueimp-gallery').attr( "data-continuous", "false" ) };
  if ( !p.hidePageScrollbars ) { $('#blueimp-gallery').attr( "data-hidePageScrollbars", "false" ) };
  if ( !p.startSlideshow ) { $('#blueimp-gallery').attr( "data-startSlideshow", "false" ) };
  $('#blueimp-gallery').attr( "data-slideshowInterval", p.slideshowInterval );
  $('#blueimp-gallery').attr( "data-transitionSpeed", p.transitionSpeed );
  $('#blueimp-gallery').append('<div class="slides" />');
  $('#blueimp-gallery').append('<h3 class="title" />');
  $('#blueimp-gallery').append('<a class="prev">‹</a>');
  $('#blueimp-gallery').append('<a class="next">›</a>');
  $('#blueimp-gallery').append('<a class="close">x</a>');
  $('#blueimp-gallery').append('<a class="play-pause">›</a>');
  $('#blueimp-gallery').append('<ol class="indicator"></ol>');
}

}(jQuery));


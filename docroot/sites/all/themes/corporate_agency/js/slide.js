jQuery(document).ready(function() {

  var progress = jQuery('#progress'),
      slideshow = jQuery( '.cycle-slideshow' );

  slideshow.on( 'cycle-initialized cycle-before', function( e, opts ) {
      progress.stop(true).css( 'width', 0 );
  });

  slideshow.on( 'cycle-initialized cycle-after', function( e, opts ) {
      if ( ! slideshow.is('.cycle-paused') )
          progress.animate({ width: '100%' }, opts.timeout, 'linear' );
  });

  slideshow.on( 'cycle-paused', function( e, opts ) {
     progress.stop(); 
  });

  slideshow.on( 'cycle-resumed', function( e, opts, timeoutRemaining ) {
      progress.animate({ width: '100%' }, timeoutRemaining, 'linear' );
  });

});

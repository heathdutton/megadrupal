(function ($) { // drupal 7 jquery wrapper

  var abs = abs || {};
  abs.processed = {};

  $(document).ready( function() {
    $('.abs-verses-versions-trigger').click(function() {
        var id = $(this).attr('verse_id');
        var versions = $(this).attr('versions');
        abs_get_verses(id, versions);
      }).mouseover(function() {
        $(this).css('cursor', 'pointer');
      }).mouseout(function(){
        $(this).css('cursor', 'arrow');
      }); 

    $('a#searchtips').fancybox({
        'padding'			: '0',
        'titlePosition'		: 'inside',
        'transitionIn'		: 'none',
        'transitionOut'		: 'none',
      });
        
    $('a#browsethebible').fancybox({
      'padding'		: '0',
      'transitionIn'	: 'elastic',
      'transitionOut'	: 'elastic',
    });
    
    $('input#edit-keys').each( function() {
      if ( $(this).val() == '' ) {
        $(this).val('Search by keyword, verse, or phrase');
      }
    });
    
    $('input#edit-keys').focus( function() {
      if ( $(this).val() == 'Search by keyword, verse, or phrase' ) {
        $(this).val('');
      }
    });
    
    $('input#edit-keys').blur( function() {
      if ( $(this).val() == '' ) {
        $(this).val('Search by keyword, verse, or phrase');
      }
    });

    $('#searchallversions').click( function() {
      $('#selectionmarker').removeClass('specific').addClass('all');
      $('#versions input').each( function() {
        $(this).attr( 'checked', true );
      });
    });
    
    $('#searchsomeversions').click( function() {
      $('#selectionmarker').removeClass('all').addClass('specific');
      $('#versions input').each( function() {
        $(this).attr( 'checked', false );
      });
      $('#edit-versions-GNT').attr( 'checked', true );
    });
    
    $('#limit').change( function() {
      $('#resultsperpage').submit();
    });
    
    if ( window.location.hash ) {
      var hash = window.location.hash.replace('#', '');
      $('a[name='+hash+']').next('span').addClass('highlighted');
    }
  });
  
  function abs_get_verses(id, versions) {
    /* In the id, remove any dots and colons. */
    var el_id = id.replace(/\./g,'').replace(/:/,'');
    
    /* abs.processed is a simple cache, to track whether we've already retrieved this section.  
     * If so, just open up and show them. */
    if (abs.processed[el_id]) {
      $('#trigger'+el_id + ' .expandtext').hide();
      $('#trigger'+el_id + ' .collapsetext').show();
      
      var el = $('#'+el_id);
      if (el.css('display') == 'block') {
        el.slideUp('slow');
        $('#trigger'+el_id + ' .expandtext').show();
        $('#trigger'+el_id + ' .collapsetext').hide();
      }
      else {
        el.slideDown('slow');
      }
    }
    
    /* If not found in the processed cache then retrieve via an ajax call */
    else {
        $('#throbber'+el_id).addClass('abs-throbber').html('&nbsp;&nbsp;&nbsp;&nbsp;');
        $('#throbber'+el_id).attr('style', 'background-image: url(' + Drupal.settings.basePath + 'misc/throbber.gif); background-repeat: no-repeat; background-position: 100% -20px;').html('&nbsp;&nbsp;&nbsp;&nbsp;');
        $('#trigger'+el_id + ' .expandtext').hide();
    $('#trigger'+el_id + ' .collapsetext').show();
    
        $.get('/abs_biblesearch/ajax/verse/'+id+'/'+versions, function(data) {
            $('#throbber'+el_id).removeAttr('style').addClass('nodisplay');
            $('#trigger'+el_id + ' .expandtext').hide();
      $('#trigger'+el_id + ' .collapsetext').show();
      
      // Insert the results...
      $( '#'+el_id ).html( data	).slideDown( 'slow' );
      
            abs.processed[el_id] = 1;
        });
    }
  }
  
})(jQuery);
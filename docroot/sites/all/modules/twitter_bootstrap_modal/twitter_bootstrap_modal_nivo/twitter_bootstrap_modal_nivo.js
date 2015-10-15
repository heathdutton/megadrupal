(function ($) {

/**
* @file
* Javascript support files.
*
*/

Drupal.behaviors.twitter_bootstrap_modal_nivo = {
  attach: function (context, settings) {
    // Read URL paramters
    var urlParams = TBN_readURLparams();
    // Create modal
    twitter_bootstrap_modal_nivo_create_modal('tb_modal_nivo');

    var TBNtrigger = Drupal.settings.twitter_bootstrap_modal_nivo.trigger;
    var module_path = Drupal.settings.twitter_bootstrap_modal_nivo.module_path;
    
    $(TBNtrigger).each(function(idx) {
      $(this).click(function(evt) {
        evt.preventDefault();
        twitter_bootstrap_modal_nivo_create_nivo(idx, TBNtrigger);
        $('#tb_modal_nivo').modal();
      });
    });
    // Read URL parameters, take "image" parameter and check if integer
    var URLnumber = urlParams["imagen"];
    if( Math.floor(URLnumber) == URLnumber && $.isNumeric(URLnumber)) {
      twitter_bootstrap_modal_nivo_create_nivo(URLnumber, TBNtrigger);
      $('#tb_modal_nivo').modal();
    } 
    else {
      twitter_bootstrap_modal_nivo_create_nivo(0, TBNtrigger);
    }
  }
}

function TBN_readURLparams() {
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

function twitter_bootstrap_modal_nivo_create_modal(id) {
  $('body').append('<div id="' + id + '" class="modal fade" tabindex="-1" role="dialog" aria-labelledby="' + id + '" aria-hidden="true">');
  $('#' + id).append('<div class="modal-dialog" />');
  $('#' + id + ' .modal-dialog').append('<div class="modal-content" />');
  $('#' + id + ' .modal-content').append('<div class="modal-header"><html xmlns:fb="http://ogp.me/ns/fb#"><span id="tbm_header_social"></span><button type="button" class="close" data-dismiss="modal" aria-hidden="true">×</button><h3 class="modal_title"></h3></div>');
  $('#' + id + ' .modal-content').append('<div class="modal-body"></div>');
  $('#' + id + ' .modal-content').append('<div class="modal-footer"><button class="btn" data-dismiss="modal" aria-hidden="true">' + Drupal.t('Close') + '</button></div>');

}

function twitter_bootstrap_modal_nivo_create_nivo(idx, trigger) {
  var caption = Drupal.settings.twitter_bootstrap_modal_nivo.caption;
  var effect = Drupal.settings.twitter_bootstrap_modal_nivo.effect;
  var slices = Drupal.settings.twitter_bootstrap_modal_nivo.slices;
  var box_columns = Drupal.settings.twitter_bootstrap_modal_nivo.box_columns;
  var box_rows = Drupal.settings.twitter_bootstrap_modal_nivo.box_rows;
  var speed = Drupal.settings.twitter_bootstrap_modal_nivo.speed;
  var pause_on_hover = Drupal.settings.twitter_bootstrap_modal_nivo.pause_on_hover;
  var directional_navigation = Drupal.settings.twitter_bootstrap_modal_nivo.directional_navigation;
  var image_navigation = Drupal.settings.twitter_bootstrap_modal_nivo.image_navigation;
  var next_text = Drupal.settings.twitter_bootstrap_modal_nivo.next_text;
  var previous_text = Drupal.settings.twitter_bootstrap_modal_nivo.previous_text;
  var control_navigation = Drupal.settings.twitter_bootstrap_modal_nivo.control_navigation;
  var control_nav_thumbs = Drupal.settings.twitter_bootstrap_modal_nivo.control_nav_thumbs;
  var manual_advance = Drupal.settings.twitter_bootstrap_modal_nivo.manual_advance;
  var pause_time = Drupal.settings.twitter_bootstrap_modal_nivo.pause_time;
  $('#tb_modal_nivo .modal-body').html('<div id="slider" />');
    // Load each image on Nivo format
  var title = '';
  $(trigger).each(function() {
    var html_string = $(this).attr( 'href' );
    if (caption) { title = jQuery("img", this).attr( 'alt' ) };
    $('#tb_modal_nivo #slider').append('<img src="' + html_string + '" alt="" data-thumb="' + html_string + '" title="' + title + '" />');
  });
  $('#slider').nivoSlider({
    effect: effect,         // Specify sets like: 'fold,fade,sliceDown'
    slices: slices,                     // For slice animations
    boxCols: box_columns,                     // For box animations
    boxRows: box_rows,                     // For box animations
    animSpeed: speed,                 // Slide transition speed
    pauseTime: pause_time,                // How long each slide will show
    startSlide: idx,                // Set starting Slide (0 index)
    controlNav: control_navigation,               // 1,2,3... navigation
    pauseOnHover: pause_on_hover,             // Stop animation while hovering
    directionNav: directional_navigation,             // Next & Prev navigation
    controlNavThumbs: control_nav_thumbs,         // Use thumbnails for Control Nav
    manualAdvance: manual_advance,            // Force manual transitions
    prevText: previous_text,           // Prev directionNav text
    nextText: next_text,          // Next directionNav text
    afterChange: function(){TBN_setsocial($('#slider').data('nivo:vars').currentSlide);},      // Triggers after a slide transition
  });
  TBN_setsocial(idx);
  $('#tb_modal_nivo .nivo-controlNav').wrap('<div class="nivo-controlNav-wrap" />');
}

$.slideTo = function(idx) {
  $('#slider').data('nivo:vars').currentSlide = idx - 1;
  $("#slider a.nivo-nextNav").trigger('click');
  setsocial(idx - 1);
}

function TBN_setsocial(idx) {
  var numberof = parseInt(idx) + 1;
  var numbertotal = $('.field-name-field-image a').length;
  
  var page_title = jQuery('.node h5').text();
  var url_title = page_title.split(' ').join('+');
  var url_img = $('.nivoSlider img:visible').attr('src');
  
  var url_doc = location.protocol + '//' + location.host + location.pathname + '?imagen=' + idx;
  var enlace1 = '<a class="facebook-modal" href="http://www.facebook.com/sharer.php?s=100&p[title]=' + url_title + '&p[url]=' + url_doc + '&p[images[0]=' + url_img + '" target="_blank"></a>';
  var enlace2 = '<a class="twitter-modal" href="https://twitter.com/share?url=' + url_doc +'&tw_p=tweetbutton&text=' + url_title + '" target="_blank" data-lang="es">Twittear</a>';
    
  $('#tbm_header_social').html('<span class="header_slide_no"><b>' + numberof + '</b> de <b>' + numbertotal + '</b></span>' + enlace1 + enlace2);

  var ancho = 95;
  var pos_left = -ancho * (numberof - 5);
  if (numberof > 4) { 
    $('.nivo-controlNav').css('left', pos_left); 
  }
}

}(jQuery));


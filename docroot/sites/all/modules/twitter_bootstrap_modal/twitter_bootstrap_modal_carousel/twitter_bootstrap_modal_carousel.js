(function ($) {

/**
* @file
* Javascript support files.
*
*/

Drupal.behaviors.twitter_bootstrap_modal_carousel = {
  attach: function (context, settings) {
    // Read URL paramters
    var urlParams = TBS_readURLparams();
    // Create modal
    twitter_bootstrap_modal_carousel_create_modal('tb_modal_carousel');

    var TBStrigger = Drupal.settings.twitter_bootstrap_modal_carousel.trigger;
    var module_path = Drupal.settings.twitter_bootstrap_modal_carousel.module_path;
    
    $(TBStrigger).once(function(idx) {
      $(this).click(function(evt) {
        evt.preventDefault();
        twitter_bootstrap_modal_carousel_create_carousel(idx, TBStrigger);
        $('#tb_modal_carousel').modal();
      });
    });
    // Read URL parameters, take "image" parameter and check if integer
    var URLnumber = urlParams["image"];
    if( Math.floor(URLnumber) == URLnumber && $.isNumeric(URLnumber)) {
      twitter_bootstrap_modal_carousel_create_carousel(URLnumber, TBStrigger);
      $('#tb_modal_carousel').modal();
    } 
    else {
      twitter_bootstrap_modal_carousel_create_carousel(0, TBStrigger);
    }
  }
}

function TBS_readURLparams() {
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

function twitter_bootstrap_modal_carousel_create_modal(id) {
  $('body').append('<div id="' + id + '" class="modal fade" tabindex="-1" role="dialog" aria-labelledby="' + id + '" aria-hidden="true">');
  $('#' + id).append('<div class="modal-dialog" />');
  $('#' + id + ' .modal-dialog').append('<div class="modal-content" />');
  $('#' + id + ' .modal-content').append('<div class="modal-header"><button type="button" class="close" data-dismiss="modal" aria-hidden="true">×</button><h3 class="modal_title"></h3></div>');
  $('#' + id + ' .modal-content').append('<div class="modal-body"></div>');
  $('#' + id + ' .modal-content').append('<div class="modal-footer"><span class="modal-footer-number" /><button class="btn" data-dismiss="modal" aria-hidden="true">' + Drupal.t('Close') + '</button></div>');

}

function twitter_bootstrap_modal_carousel_create_carousel(idx, trigger) {
  var caption = Drupal.settings.twitter_bootstrap_modal_carousel.caption;
  var pause_on_hover = Drupal.settings.twitter_bootstrap_modal_carousel.pause_on_hover;
  var directional_navigation = Drupal.settings.twitter_bootstrap_modal_carousel.directional_navigation;
  var image_navigation = Drupal.settings.twitter_bootstrap_modal_carousel.image_navigation;
  var next_text = Drupal.settings.twitter_bootstrap_modal_carousel.next_text;
  var previous_text = Drupal.settings.twitter_bootstrap_modal_carousel.previous_text;
  var wrap = Drupal.settings.twitter_bootstrap_modal_carousel.wrap;
  var control_navigation = Drupal.settings.twitter_bootstrap_modal_carousel.control_navigation;
  var pause_time = Drupal.settings.twitter_bootstrap_modal_carousel.pause_time;
  var next_html = '<span class="glyphicon glyphicon-chevron-right"></span>';
  var prev_html = '<span class="glyphicon glyphicon-chevron-left"></span>';
  var directional_html = '';
  if ( directional_navigation ) {
    if ( !image_navigation ) {
      // Have to check why
      prev_html = '<span class="modal-nav-text-left">' + next_text + '</span>';
      next_html = '<span class="modal-nav-text-right">' + previous_text + '</span>';
    }
    directional_html = '<a class="left carousel-control" href="#carousel-tbs" data-slide="prev">' + prev_html + '</a><a class="right carousel-control" href="#carousel-tbs" data-slide="next">' + next_html + '</a>';
  }
  $('#tb_modal_carousel .modal-body').html('<div id="carousel-tbs" class="carousel slide" data-ride="carousel"><ol class="carousel-indicators"></ol><div class="carousel-inner"></div>' + directional_html + '</div>');
    // Load each image on carousel format
  var caption_html = '';
  $(trigger).each(function(index) {
    var html_string = $(this).attr( 'href' );
    var title = jQuery("img", this).attr( 'alt' );
    $('#carousel-tbs .carousel-inner').append('<div class="item" id="item' + index + '" />');
    if (index == 0) { $('#carousel-tbs #item' + index).addClass('active') };
    var image_html = '<img src="' + html_string + '" alt="' + title + '" />';
    $('#carousel-tbs #item' + index).append(image_html);
    if (caption) 
    { 
      caption_html = '<div class="carousel-caption"><h3>' + title  + '</h3></div>'; 
      $('#carousel-tbs #item' + index).append(caption_html);
    }
    if (control_navigation) 
    { 
      $('#carousel-tbs .carousel-indicators').append('<li data-target="#carousel-tbs" data-slide-to="' + index + '" />');
    }
  });
  $('#carousel-tbs').carousel({
    interval: pause_time,  // How long each carousel will show
    pause: pause_on_hover, // Stop animation while hovering
    wrap: wrap,            // Plays continously
  });
  $('#carousel-tbs').carousel(idx);
  $('#carousel-tbs').on('slid.bs.carousel', function(){TBS_setsocial($('#carousel-tbs .active').index('#carousel-tbs .item'), trigger);});
  TBS_setsocial(idx, trigger);
}

function TBS_setsocial(idx, trigger) {
  var numberof = parseInt(idx) + 1;
  var numbertotal = $(trigger).length;
  
  $('#tb_modal_carousel .modal-footer-number').html('<b>' + numberof + '</b> ' + Drupal.t('of') + ' <b>' + numbertotal + '</b>');
}

}(jQuery));


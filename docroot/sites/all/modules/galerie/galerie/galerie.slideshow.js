
(function($) {


Drupal.behaviors.galerie = {
  attach: function (context) {
    $('.galerie-slideshow-wrapper').each(function() {
      var nid = $(this).attr('id').replace(/^node-/, '');
      Drupal.galerie.settings(nid).element = $(this);
      Drupal.galerie.settings(nid).offset = 1;

      Drupal.galerie.fixThumbnailLinks(nid);

      $(this).find('.galerie-slideshow-wrapper img').live('click', function() {
        var index = $('.galerie-slideshow-wrapper img').index($(this));
        Drupal.galerie.settings(nid).offset = index;
        clearTimeout(Drupal.galerie.settings(nid).timer);
        Drupal.galerie.showNextImage(nid);
        $('#controls #play').hide();
        $('#controls #pause').show();

        return false;
      });

      $('#controls #pause').click(function() {
        clearTimeout(Drupal.galerie.settings(nid).timer);
        $(this).hide();
        $('#controls #play').show();
      });
      $('#controls #play').click(function() {
        Drupal.galerie.settings(nid).timer = setTimeout("Drupal.galerie.showNextImage(" + nid + ");", 5000);
        $(this).hide();
        $('#controls #pause').show();
      });
      $('#controls #back').click(function() {
          history.go(-1);
      });

      var first_id = $(this).find('.galerie-slideshow-thumbnails img:first').attr('id').replace(/^galerie-/, '');
      Drupal.galerie.showImageAjax(nid, first_id);
      Drupal.galerie.settings(nid).timer = setTimeout("Drupal.galerie.showNextImage(" + nid + ");", 5000);
    });

    $('.galerie-slideshow-thumbnails-wrapper').hover(
      function() {$(this).find('.galerie-slideshow-thumbnails').slideDown(); Drupal.galerie.takeCurrentThumbnailIntoView();},
      function() {$(this).find('.galerie-slideshow-thumbnails').slideUp();}
    );
  }
};

Drupal.galerie = Drupal.galerie || {
};

Drupal.galerie.takeCurrentThumbnailIntoView = function() {
}

Drupal.galerie.fixThumbnailLinks = function(nid) {
  Drupal.galerie.settings(nid).element.find('.galerie-slideshow-thumbnails a').each(function() {
    var id = $(this).find('img').attr('id').replace(/^galerie-/, '');
    var link = Drupal.galerie.settings(nid).baseUrl + '/slideshow#' + id;

    $(this).attr('href', link);
  });
}

Drupal.galerie.appendThumb = function(nid, offset) {
  $('body').addClass('galerie-ajax-waiting');

  $.ajax({
    url: Drupal.galerie.settings(nid).imageListUrl + '/' + offset + '/1',
    success: function(json) {
      Drupal.galerie.settings(nid).element.find('.galerie-slideshow-thumbnails').append(json.markup);
      Drupal.galerie.fixThumbnailLinks(nid);
    },
    dataType: 'json',
    complete: function() {
    },
  });
};


Drupal.galerie.showNextImage = function(nid) {
  $.ajax({
    url: Drupal.galerie.settings(nid).imageListUrl + '/' + Drupal.galerie.settings(nid).offset + '/1',
    success: function(json) {
      if ($(json.markup).find('img').size() == 0) {
        Drupal.galerie.settings(nid).offset = 0;
        Drupal.galerie.showNextImage(nid, 0);
      } else {
        var next_id = $(json.markup).find('img').attr('id').replace(/^galerie-/, '');
        Drupal.galerie.showImageAjax(nid, next_id);
        if (Drupal.galerie.settings(nid).offset >= $('.galerie-slideshow-thumbnails img').size()) {
          Drupal.galerie.appendThumb(nid, Drupal.galerie.settings(nid).offset);
        }
        Drupal.galerie.settings(nid).offset++;
        Drupal.galerie.settings(nid).timer = setTimeout("Drupal.galerie.showNextImage(" + nid + ", " + Drupal.galerie.settings(nid).offset + ");", 5000);
      }
    },
    dataType: 'json',
    complete: function() {
    },
  });
}

Drupal.galerie.settings = function(nid) {
  return Drupal.settings.galerie['galerie-' + nid];
}

Drupal.galerie.showImageAjax = function(nid, image_id) {
  $('body').addClass('galerie-ajax-waiting');

  $.ajax({
    url: Drupal.galerie.settings(nid).imageInfoUrl + '/' + encodeURIComponent(image_id),
    success: function(json) {
      Drupal.galerie.showImage(nid, json, function() {
        $('.galerie-slideshow-thumbnails img').removeClass('current');
        $('.galerie-slideshow-thumbnails img[id=galerie-' + image_id + ']').addClass('current');
        Drupal.galerie.takeCurrentThumbnailIntoView();
      });
    },
    dataType: 'json',
    complete: function() {
      $('body').removeClass('galerie-ajax-waiting');
    },
  });
};

Drupal.galerie.showImage = function(nid, json, callback) {
  var image = $(json.markup).hide();
  image.find('img').load(
    (function(image) {
      return function() {
        image.siblings().fadeOut(400, function() {$(this).remove()});
        image.fadeIn(400, function() {
            if ($('.galerie-image-info').text().trim().length > 0) {
            $('.galerie-image-info').animate({opacity: 1}, 400, 'swing', function() {$(this).animate({opacity: 0}, 1400)});
            $('.galerie-image-info').hover(
              function() {$(this).stop().animate({opacity: 1}, 200);},
              function() {$(this).animate({opacity: 0}, 1400);}
            );
          }
        });
        if (callback) callback();
      }
    })(image)
  );

  Drupal.galerie.settings(nid).element.find('.galerie-slideshow-viewer').prepend(image);
};

})(jQuery);

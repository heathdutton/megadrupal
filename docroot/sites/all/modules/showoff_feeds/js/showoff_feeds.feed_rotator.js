(function ($) {
  var videore = /<a href=\"([^\"]+?)\" type=\"video\/mp4; length=[0-9]+?\">/;
  var imagere = /<img src=\"(.+?)\" (width=\"[0-9]+\" )?(height=\"[0-9]+\" )?(alt=\"\")? \/>/;
  var durationre = /<div class=\"field-label\">Duration:&nbsp;<\/div><div class=\"field-items\"><div class=\"field-item even\">([0-9]+?)<\/div>/;

  $.fn.fix = function(str) {
  	str = str.replace("&lt;", "<");
  	str = str.replace("&gt;", ">");
  	str = str.replace("&quot;", "");
  	str = str.replace("&nbsp;", " ");
  	return str;
  }

  $.fn.rotateFeed = function(feedDiv, feedUrl) {
    $.fn.loadFeed(feedDiv, feedUrl);
    setTimeout('jQuery.fn.rotateFeed("' + feedDiv + '", "' + feedUrl + '")', 1000 * 60 * 30);
  };

  $.fn.loadFeed = function(feedDiv, feedUrl) {
    $.ajax({
      url: feedUrl,
      success: function(data) {
        $('#' + feedDiv).data('xmlFeed', data);
        $.fn.parseFeed(feedDiv);
      }
    });
  }

  $.fn.parseFeed = function(feedDiv) {
    var xmlFeed = $('#' + feedDiv).data('xmlFeed');
    var items = xmlFeed.getElementsByTagName('item');
    var feedItems = new Array();
    var index = 0;
    for (var i=0; i<items.length; i++) {
      var desc = $.fn.fix(items[i].getElementsByTagName('description')[0].childNodes[0].nodeValue);
      var video = videore.exec(desc);
      var image = imagere.exec(desc);
      var duration = durationre.exec(desc);
      if (duration == null) {
        duration = 15000;
      }
      else {
        duration = parseInt(duration[1]) * 1000;
      }
      if (video != null) {
        feedItems[index] = new Array(video[1], duration, 'video')
        index++;
      }
      else if (image != null) {
        feedItems[index] = new Array(image[1], duration, 'image')
        index++
      }
    }
    $('#' + feedDiv).data('xmlFeed', feedItems);
    $('#' + feedDiv).data('index', items.length);
    $.fn.startFeed(feedDiv);
  }
  
  $.fn.startFeed = function(feedDiv) {
    var items = $('#' + feedDiv).data('xmlFeed');
    var newItem = document.createElement("div");
    newItem.className = feedDiv + "-0 showoff_item";
    newItem.style.opacity = 0.0;
    if (items[0][2] == 'image') {
      newItem.innerHTML = "<img src='" + items[0][0] + "' height='100%' width='100%'/>";
    }
    else {
      newItem.innerHTML = "<video src=\"" + items[0][0] + "\" type=\"video/mp4\" loop=\"FALSE\"></video>";
    }
    $('#' + feedDiv).append(newItem);
    $('div.' + feedDiv + '-0').animate({opacity: 1.0}, 1000, "linear", function() {
      $('#' + feedDiv).data('timer', setTimeout('jQuery.fn.nextItem("' + feedDiv + '")', items[0][1]));
      if ($('div.' + feedDiv + '-0' + ' video').length > 0) {
        $('div.' + feedDiv + '-0' + ' video')[0].play();
      }
    });
    $('#' + feedDiv).data('index', 0);
  }

  $.fn.nextItem = function(feedDiv) {
    clearTimeout($('#' + feedDiv).data('timer'));
    var items = $('#' + feedDiv).data('xmlFeed');
    var current = $('#' + feedDiv).data('index');
    var next = current;
    if (++next >= items.length) {
      next = 0;
    }
    var newItem = document.createElement("div");
    newItem.className = feedDiv + "-" + next + " showoff_item";
    newItem.style.opacity = 0.0;
    if (items[next][2] == 'image') {
      newItem.innerHTML = "<img src='" + items[next][0] + "' height='100%' width='100%'/>";
    }
    else {
      newItem.innerHTML = "<video src=\"" + items[next][0] + "\" type=\"video/mp4\" loop=\"FALSE\"></video>";
    }
    $('div.' + feedDiv + '-' + current).animate({opacity: 0.0}, 1000, "linear", function() {
      $('div.' + feedDiv + '-' + current).replaceWith(newItem);
      $('div.' + feedDiv + '-' + next).animate({opacity: 1.0}, 1000, "linear", function() {
        $('#' + feedDiv).data('timer', setTimeout('jQuery.fn.nextItem("' + feedDiv + '")', items[next][1]));
        if ($('div.' + feedDiv + '-' + next + ' video').length > 0) {
          $('div.' + feedDiv + '-' + next + ' video')[0].play();
        }
      });
    });
    $('#' + feedDiv).data('index', next);
  }
})(jQuery);
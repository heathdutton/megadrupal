/**
 * @author DigiSin
 * @author Nicola Tomassoni <nicola@digisin.it>
 * @link http://www.digisin.it digisin homesite
 * @copyright Copyright (c) 2013, Digisin soc. coop
 *
 **/

/**
 * @param {type} $
 * @returns {undefined}
 */
(function($) {
  Drupal.behaviors.dsYtChannel = {
    attach: function(context, settings) {
      $.getJSON('http://gdata.youtube.com/feeds/users/' + settings.dsYtChannel.channelName + '/uploads?alt=json&format=5&max-results=' + settings.dsYtChannel.maxItems + '&callback=?', null, function(data) {
        var feed = data.feed;
        $.each(feed.entry, function(i, entry) {
          var video = {
            title: entry.title.$t,
            description: entry.media$group.media$description.$t,
            uri: entry.link[0].href,
            id: entry.id.$t.match('[^/]*$'),
            thumb: entry.media$group.media$thumbnail[1].url,
            submitted: {
              name: entry.author[0].name.$t,
              uri: feed.link[1].href,
              date: entry.published.$t,
              hits: typeof entry.yt$statistics !== 'undefined' ? entry.yt$statistics.viewCount : 0
            }
          };
          if (i === 0)
            $(".ytchannel").html('');
          $(".ytchannel").append(Drupal.theme('ytChannelRow', video));
        });
      });
    }
  };

  Drupal.theme.prototype.ytChannelRow = function(row) {
    var $out = '<div class="ytc-wrapper">';
    $out += '<div class="ytc-img">';
    $out += '<a href="' + row.uri + '" target="_blank"><img src="' + row.thumb + '"/></a>';
    $out += '</div>';
    $out += '<div class="ytc-info">';
    $out += '<h4><a href="' + row.uri + '" target="_blank">' + row.title + '</a></h4>';
    $out += '<div class="ytc-author">' + Drupal.theme('ytChannelAuthor', row.submitted) + '</div>';
    $out += '<div class="ytc-desc"><p>' + row.description + '</p></div>';
    $out += '</div>';
    $out += '</div>';

    return $out;
  };

  Drupal.theme.prototype.ytChannelAuthor = function(submitted) {
    var date = new Date(submitted.date);
    var $out = 'By <a href="' + submitted.uri + '" target="_blank">' + submitted.name + '</a>';
    $out += ' - <span class="yt-date">' + date.getDay() + '/' + date.getMonth() + '/' + date.getFullYear() + '</span>';
    if (submitted.hits > 0)
      $out += '<span class="yt-hits">' + submitted.hits + ' hits</span>';

    return $out;
  };
}(jQuery));
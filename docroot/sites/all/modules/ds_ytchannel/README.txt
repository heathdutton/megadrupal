Customizing the output.

Write own Javascript Theme function like:

(function($) {
/**
 * Available properties:
 * row.uri
 * row.thumb
 * row.title
 * row.submitted see [[Drupal.theme.prototype.ytChannelAuthor()]]
 * row.description
 */
  Drupal.theme.ytChannelRow = function(row) {
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

  /**
   * Available properties:
   * submitted.uri
   * submitted.name
   * submitted.date
   * submitted.hits
   */
  Drupal.theme.ytChannelAuthor = function(submitted) {
    var date = new Date(submitted.date);
    var $out = 'di <a href="' + submitted.uri + '" target="_blank">' + submitted.name + '</a>';
    $out += ' - <span class="yt-date">' + date.getDay() + '/' + date.getMonth() + '/' + date.getFullYear() + '</span>';
    if (submitted.hits > 0)
      $out += '<span class="yt-hits">' + submitted.hits + ' hits</span>';

    return $out;
  };
})(jQuery);
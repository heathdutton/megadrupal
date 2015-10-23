Drupal.theme.prototype.mkdruResult = function(hit, num, detailLink) {
  var html = "";
  html += '<li class="mkdru-result" id="rec_' + hit.recid + '" >'
          + '<span>' + num + '. </span>'
          + '<a href="' + detailLink + '" class="mkdru-result-title">'
          + hit["md-title"] + '</a> ';
  if (hit["md-title-remainder"] !== undefined) {
    html += '<span class="mkdru-result-title-remainder">'
            + hit["md-title-remainder"] + ' </span>';
  }
  if (hit["md-title-responsibility"] !== undefined) {
    html += '<span class="mkdru-result-author"><i>'
            + hit["md-title-responsibility"]
            + '</i></span>';
  }
  html += '</li>';
  return html;
};

Drupal.theme.prototype.mkdruDetail = function(data, linkBack) {
  var html = '<table id="det_' + data.recid +'">';
  if (data["md-title"] != undefined) {
    html += '<tr><th>' + Drupal.t("Title") + '</th><td>'
            + data["md-title"];
    if (data["md-title-remainder"] !== undefined) {
      html += ' : <span>' + data["md-title-remainder"] + ' </span>';
    }
    if (data["md-title-responsibility"] !== undefined) {
      html += ' <span><i>'+ data["md-title-responsibility"] +'</i></span>';
    }
    html += '</td></tr>';
  }
  if (data["md-date"] != undefined)
    html += '<tr><th>' + Drupal.t("Date") + '</th><td>' + data["md-date"] + '</td></tr>';
  if (data["md-author"] != undefined)
    html += '<tr><th>' + Drupal.t("Author") + '</th><td>' + data["md-author"] + '</td></tr>';
  if (data["md-electronic-url"] != undefined)
    html += '<tr><th>URL</th><td><a href="'
            + data["md-electronic-url"] + '" target="_blank">'
            + data["md-electronic-url"] + '</a>' + '</td></tr>';
  // test if there is an array of per-location data; process it
  if (Object.prototype.toString.call(data['location']) === '[object Array]') {
    var subject = false;
    var locations = "";
    for (var i = 0; i < data['location'].length; i++) {
      var locRec = data['location'][i];
      // grab the first value we find for subject
      if (!subject && locRec['md-subject']) subject = locRec['md-subject'];
      // list named locations and link to the record there if url is available
      var link = locRec["md-electronic-url"];
      var name = locRec["@name"]
      if (link && name) locations += '<a href="' + link + '">' + name + '</a><br>';
      else if (name) locations += name + '<br>';
    }
    if (subject)
      html += '<tr><th>' + Drupal.t("Subject") + '</th><td>' + subject + '</td></tr>';
    if (locations)
      html += '<tr><th>' + Drupal.t("Location") + '</th><td>' + locations + '</td></tr>';
  }
  html += '</table>';
  html += '<a href="' + linkBack + '">' + Drupal.t('Return to result list...') + '</a>';
  return html;
};

/**
 * Pager theme
 *
 * @param pages
 *   Array of hrefs for page links.
 * @param start
 *   Number of first page.
 * @param current
 *   Number of current page.
 * @param total
 *   Total number of pages.
 * @param prev
 *   Href for previous page.
 * @param next
 *   Href for next page.
 */
Drupal.theme.prototype.mkdruPager = function (pages, start, current, total, prev, next) {
  var html = "";
  if (prev)
    html += '<a href="' + prev + '" class="mkdru-pager-prev">&#60;&#60; '
            + Drupal.t("Prev") + '</a> | ';
  else
    html += '<span class="mkdru-pager-prev">&#60;&#60; ' + Drupal.t("Prev")
            + '</span> | ';

  if (start > 1)
    html += '...';

  for (var i = 0; i < pages.length; i++) {
    if (i + start == current)
      html += ' <span class="mkdru-pager-current">' + (i + start) + '</span>';
    else
      html += ' <a href="' + pages[i] + '">' + (i + start) + '</a>';
  }

  if (total > i)
    html += ' ...';

  if (next)
    html += ' | <a href="' + next + '" class="mkdru-pager-next">'
      + Drupal.t("Next") + ' &#62;&#62;</a>';
  else
    html += ' | <span class="mkdru-pager-next">' + Drupal.t("Next")
            + ' &#62;&#62;</span>';

  return html;
};

Drupal.theme.prototype.mkdruCounts = function(first, last, available, total) {
  if (last > 0) {
    args = {};
    args['@first'] = first;
    args['@last'] = last;
    args['@available'] = available;
    args['@total'] = total;
    return Drupal.t('@first to @last of @available available (@total found)', args);
  }
  else {
    return Drupal.t('No results');
  }
};

Drupal.theme.prototype.mkdruStatus = function(activeClients, clients) {
  return Drupal.t('Waiting on ') + activeClients + Drupal.t(' out of ')
         + clients + Drupal.t(' targets');
};

Drupal.theme.prototype.mkdruFacet = function (terms, facet, max, selections) {
  var html = '';
  for (var i = 0; terms && i < terms.length && i < max; i++ ) {
    var term = terms[i];
    html += '<a href="' + term.toggleLink + '"';
    if (term.selected) html += ' class="mkdru-selected-term"><strong';
    html += '>' + Drupal.checkPlain(terms[i].name);
    if (term.selected) html += "</strong>";
    html += '</a><span>';
    if (typeof(term.freq) !== 'undefined') html += '<span> (' + term.freq + ')</span>';
    html += '<br/>';
  }
  return html;
};

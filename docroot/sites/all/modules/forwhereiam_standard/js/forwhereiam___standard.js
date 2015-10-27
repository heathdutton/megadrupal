/**
 * @file
 * Plugin for taking user requests and displaying forWhereiAm results.
 */

(function ($) {
  Drupal.behaviors.forwhereiam___standard = {
    attach: function (context, settings) {

      var refresh_interval = (typeof (settings.forwhereiam___standard) != "undefined") ? settings.forwhereiam___standard.refresh_interval : 60;
      var geolocate = (typeof (settings.forwhereiam___standard) != "undefined") ? settings.forwhereiam___standard.geolocate : true;
      var signup = (typeof (settings.forwhereiam___standard) != "undefined") ? settings.forwhereiam___standard.signup : true;
      var show_map = (typeof (settings.forwhereiam___standard) != "undefined") ? settings.forwhereiam___standard.show_map : false;
      var show_sharing_buttons = (typeof (settings.forwhereiam___standard) != "undefined") ? settings.forwhereiam___standard.show_sharing_buttons : false;
      var show_ratings = (typeof (settings.forwhereiam___standard) != "undefined") ? settings.forwhereiam___standard.show_ratings : true;
      var block_id = (typeof (settings.forwhereiam___standard) != "undefined") ? settings.forwhereiam___standard.block_id : null;

      // Enure only attach the below code once.
      $('#' + block_id, context).once('fwiastandardbehaviour', function () {

        // Countdown timer. Pass in number of seconds to count down from.
        // The plugin maintains an internal count of what time is left.
        $.fn.fwia_countdown = function(options) {

          function _fwia_displayTimes(expirySpan, currentTimeLeft) {

            var wholeSeconds = parseInt(currentTimeLeft / 1000);
            var wholeMinutes = parseInt(currentTimeLeft / (60 * 1000));
            var wholeHours = parseInt(currentTimeLeft / (60 * 60 * 1000));

            var seconds = parseInt(wholeSeconds % 60);
            var minutes = parseInt(wholeMinutes % 60);
            var days = parseInt(wholeHours / 24);
            var hours = parseInt(wholeHours % 24);

            if (days > 0) {
              expirySpan.find('.daysLeft').empty().append(days);
              expirySpan.find('.dayLabel').empty().append("d ");
            } else {
              expirySpan.find('.daysLeft').empty();
              expirySpan.find('.dayLabel').empty();
            }

            if (hours < 10) {
              expirySpan.find('.hoursLeft').empty().append("0" + hours);
            }
            else {
              expirySpan.find('.hoursLeft').empty().append(hours);
            }

            if (minutes < 10) {
              expirySpan.find('.minutesLeft').empty().append("0" + minutes);
            }
            else {
              expirySpan.find('.minutesLeft').empty().append(minutes);
            }

            if (seconds < 10) {
              expirySpan.find('.secondsLeft').empty().append("0" + seconds);
            }
            else {
              expirySpan.find('.secondsLeft').empty().append(seconds);
            }

            if (days === 0 && hours === 0) {
              expirySpan.addClass('highlight');
            }
          }

          function _fwia_setTimer(expirySpan, currentTimeLeft, timerTimeLeft, secondsForTimer) {

            currentTimeLeft = currentTimeLeft - secondsForTimer;

            if (currentTimeLeft <= 0) {
              clearInterval(timerTimeLeft);
              expirySpan.empty().append("Expired").removeClass('highlight').unbind();
            }
            else {
              _fwia_displayTimes(expirySpan, currentTimeLeft);
            }

            return currentTimeLeft;
          }

          function _fwia_setInitialTimer(expirySpan, currentTimeLeft, timerTimeLeft) {

            if (currentTimeLeft <= 0) {
              clearInterval(timerTimeLeft);
              expirySpan.empty().unbind();
            }
            else {
              _fwia_displayTimes(expirySpan, currentTimeLeft);
            }

            return currentTimeLeft;
          }

          // Create defaults only once.
          //
          // currentTimeLeft [int] value holding number of seconds left before announcement due to expire. 0 means expired or not applicable.
          // secondsForTimer [int] set interval (milliseconds).
          var defaults = {
            currentTimeLeft: null,
            secondsForTimer: 1000
          };

          // Use extend to create settings from passed options and the defaults.
          var settings = $.extend({}, defaults, options);

          // Iterate over the current set of matched elements.
          return this.each(function() {
            // Holds the expiry time container span.
            var $this = $(this);

            if (settings.currentTimeLeft === null) {
              return $this;
            }

            var timerTimeLeft = 0;

            settings.currentTimeLeft = _fwia_setInitialTimer($this, settings.currentTimeLeft, timerTimeLeft);
            timerTimeLeft = setInterval (function() {
                     settings.currentTimeLeft = _fwia_setTimer($this,
                                              settings.currentTimeLeft,
                                              timerTimeLeft,
                                              settings.secondsForTimer);
                     }, settings.secondsForTimer);
          });
        };

        // Display of when an announcement was made in terms of 'x minutes/hours ago'.
        $.fn.fwia_relative_time = function(options) {

          function _relative_time(issuedSpan, timeAgo, secondsForTimer, initial) {

            if (!initial) {
              timeAgo = timeAgo + secondsForTimer;
            }

            var r = '';
            if (timeAgo < 60000) {
              r = (parseInt(timeAgo / 1000, 10)).toString() + ' ' + Drupal.t('seconds ago');
            }
            else if (timeAgo < 120000) {
              r = '1 minute ago';
            }
            else if (timeAgo < (60 * 60000)) {
              r = (parseInt(timeAgo / 60000, 10)).toString() + ' ' + Drupal.t('minutes ago');
            }
            // 1 hr ~ 1.5 hr classed as about an hour ago.
            else if (timeAgo < ((60 + 30) * 60000)) {
              r = 'about 1 hour ago';
            }
            // > 1.5 hr ~ 24 hrs.
            else if (timeAgo < (24 * 60 * 60000)) {
              r = 'about ' + (parseInt(timeAgo / 3600000, 10)).toString() + ' ' + Drupal.t('hours ago');
            }
            else if (timeAgo < (48 * 60 * 60000)) {
              r = 'yesterday';
            }
            else {
              r = (parseInt(timeAgo / 86400000, 10)).toString() + ' ' + Drupal.t('days ago');
            }

            var an = Drupal.t('announced');
            issuedSpan.empty().append(an + ' ' + r);

            return timeAgo;
          }

          // Create defaults only once.
          //
          // issued [int] the number of milliseconds ago when announcement was issued.
          // secondsForTimer [int] re-evaluate every 10 seconds.
          var defaults = {
            issued: null,
            secondsForTimer: 10000
          };

          // Use extend to create settings from passed options and the defaults.
          var settings = $.extend({}, defaults, options);

          // Iterate over the current set of matched elements.
          return this.each(function() {
            // $this holds the expiry time container span.
            var $this = $(this);

            if (settings.issued === null) {
              return $this;
            }

            var timerTimeAgo = 0;

            settings.issued = _relative_time($this, settings.issued, settings.secondsForTimer, true);

            timerTimeAgo = setInterval (function() {
                                settings.issued = _relative_time($this, settings.issued, settings.secondsForTimer, false);
                              }, settings.secondsForTimer);
            });

          };

          // Needed to store the current users location settings in a cookie
          // so that we don't need to ask this information again on a page refresh.
          // Below are functions to create, read and delete a cookie.
          function _fwia_createCookie(name,value,days) {
            var expires = "";

            if (days) {
              var date = new Date();
              date.setTime(date.getTime() + (days * 24 * 60 * 60 * 1000));
              expires = "; expires=" + date.toGMTString();
            }
            document.cookie = name + "=" + value + expires + "; path=/";
          }

          function _fwia_readCookie(name) {
            var nameEQ = name + "=";
            var ca = document.cookie.split(';');

            for (var i = 0; i < ca.length; i++) {
              var c = ca[i];

              while (c.charAt(0) == ' ') {
                c = c.substring(1, c.length);
              }

              if (c.indexOf(nameEQ) === 0) {
                return c.substring(nameEQ.length, c.length);
              }
            }

            return "";
          }

          function _fwia_eraseCookie(name) {
            _fwia_createCookie(name,"",-1);
          }

          // Use this function to display the results of a search in a list form.
          //
          // 'results' contains the json formatted search results.
          //
          // 'after' holds the timestamp returned by the API showing what time the
          // last search took place. In our next API request, we can
          // pass this value back to the API to ensure we only get newer results sent
          // back from this timestamp onwards.
          //
          // 'prepend' is a flag we maintain to decide whether we need to display
          // the 'results' just arriving as a new set of results or whether we need
          // to prepend them to the top of an existing results list (e.g. when we have
          // just received the latest results following on from a given timestamp).
          function _fwia_list_announcements(results, after, prepend, container) {
            var results_html = [];

            var results_div = container.find('.fwia---standard-results');
            var fwia_widget_data_store = $('#' + block_id)[0];

            if (!prepend) {
              // Always remove any previous data held before adding new - if we're not prepending.
              // But make sure to only remove for the corresponding container.
              $.removeData(fwia_widget_data_store, "after");

              results_div.find(".fwia---standard-bDesc, .fwia---standard-score_this, .fwia---standard-score a, .fwia---standard-close").unbind().remove();
              results_div.find("ul, .fwia---standard-bExpires").remove();

              results_html.push('<ul>');
            }

            // Remove the 'new' class from all existing results (if there are any) with timers and announcement issued times.
            // This is needed so that we don't try to bind plugins to the previously inserted elements again.
            results_div.find('.fwia---standard-timer, .fwia---standard-bIssued').removeClass('new');

            // Construct announcement rows.
            // Only link the business profile name if the business has a corresponding 'live' profile.
            //
            // Offset f added to b, if we are prepending new results. It is used to decide whether the new results
            // being appended to an existing results list should start as even or odd row (for styling purposes).
            var size = results.length;
            var firstItem = results_div.find('li.row').first();
            var f = (prepend === true) ? (((size % 2 === 0 && firstItem.hasClass('even')) || (size % 2 == 1 && firstItem.hasClass('odd'))) ? 0 : 1) : 0;

            for (var b = 0; b < size; b++) {
              results_html.push('<li class="row ');
              results_html.push((b + f) % 2 === 0 ? 'even' : 'odd');
              results_html.push('"><span class="fwia---standard-clientLogo">');
              results_html.push((results[b].imgUrl) ? '<img src="' + results[b].imgUrl + '" alt="' + results[b].issuer + '" width="' + results[b].imgDimensions[0] + '" height="' + results[b].imgDimensions[1] + '" />' : '');
              results_html.push('</span>');

              results_html.push('<span class="fwia---standard-bDesc"><a href="#');
              results_html.push(results[b].id);
              results_html.push('" title="');
              results_html.push(results[b].title);
              results_html.push('">');
              results_html.push(results[b].title);
              results_html.push('</a>');
              results_html.push('</span>');

              results_html.push('<br><span class="fwia---standard-bExpires');

              if (results[b].label && results[b].label.length > 0) {
                results_html.push(' label-format">' + results[b].label);
              }
              else if (results[b].timer) {
                results_html.push(' fwia---standard-timer new" data-expires="');
                results_html.push(results[b].visibleUntil);
                results_html.push('">expires in <span class="daysLeft"></span><span class="dayLabel"></span><span class="hoursLeft"></span>:<span class="minutesLeft"></span>:<span class="secondsLeft"></span>');
              }
              else {
                results_html.push('">');
              }

              results_html.push('</span>');

              // Display who issued the announcement.
              results_html.push('<br>');

              results_html.push('<span class="fwia---standard-bIssuer">');
              results_html.push(results[b].issuer);
              results_html.push('</span>');

              results_html.push('<span class="fwia---standard-bIssued new" data-issued="');
              results_html.push(results[b].visibleFrom);
              results_html.push('"></span>');

              results_html.push('<div class="fwia---standard-clear">&nbsp;</div></li>');
            }

            $.data (fwia_widget_data_store, 'after', after);

            if (!prepend) {
              results_html.push('</ul>');
            }

            fwia_widget_data_store = null;
            results = null;

            if (!prepend) {
              results_div.empty().html(results_html.join(''));
            }
            else {
              $(results_html.join(''))
                 .prependTo(results_div.find('ul'))
                 .hide()
                 .css('opacity',0)
                 .slideDown(500,
                     function() {
                       // Perform the fade in (using the animate method)
                       // when the slide completes.
                       $(this).animate({opacity:1},1000);
                     }
                 );
            }

            results_html = null;

            _fwia_bindCountdown(container);
            _fwia_bindRelativeTime(container);
          }

          // Binds a countdown timer to the .fwia---standard-timer elements (only to the newly inserted ones)
          // holding number of seconds left before announcement expiry.
          function _fwia_bindCountdown(container) {
            $.each(container.find(".fwia---standard-timer.new"), function() {
              var until = parseInt($(this).attr('data-expires'));
              $(this).fwia_countdown({
                currentTimeLeft: until
              });
            });
          }

          // Binds a 'date prettifier' to the .fwia---standard-bIssued elements (only to the newly inserted ones)
          // which holds the number of seconds ago when an announcement was issued.
          function _fwia_bindRelativeTime(container) {
            $.each(container.find(".fwia---standard-bIssued.new"), function() {
              var issued = parseInt($(this).attr('data-issued'));
              $(this).fwia_relative_time({
                issued: issued
              });
            });
          }

          // Find the user with gelocation.
          function _findMe() {
            // If browser doesn't support it, do nothing.
            if (!window.navigator.geolocation) {
              return;
            }

            var geolocation = $('.fwia---standard-geolocation');
            geolocation.attr('disabled', 'disabled');

            navigator.geolocation.getCurrentPosition(success, error, {
              enableHighAccuracy: true 	/* Want highest quality location fix - so if GPS available, use it */,
              maximumAge: 0 		/* Oldest allowable cached position (ms) */,
              timeout: 10000 		/* Maximum allowable time spent determining location (ms) */
            });

            // Take the position in GPS coordinates and turn into a postcode.
            // NOTE: the forWhereiAm search API can work directly with GPS coordinates - not
            // just postcodes. However converting to postcode is recommended to help the
            // end user better understand what is happening.
            function success(position) {
              var lat = position.coords.latitude;
              var lon = position.coords.longitude;

              $.ajax({
                url: "/forwhereiam/findme",
                data: {lon: lon, lat: lat},
                complete: function(xhr) {
                  var json = $.parseJSON(xhr.responseText);

                  if (json && json.data && json.data.error) {
                    alert(Drupal.t("Your postcode could not be determined automatically. Please enter it manually."));
                  }
                  else if (json.data && json.data.postcode) {
                    $('#' + block_id + ' .fwia---standard-location').val(json.data.postcode);
                    $('#' + block_id + ' form').submit();
                  }
                }
              });

              geolocation.removeAttr('disabled');
            }

            function error(err) {

              if (err.code !== 1 /*Not cancelled by user, instead position unavailable or timeout occurred*/) {
                alert(Drupal.t("Your postcode could not be determined automatically. Please enter it manually."));
              }
              geolocation.removeAttr('disabled');
            }
          }

          // Bind announcement clicks.
          //
          // Fetch the details of the announcement clicked upon via making
          // an ajax request to forWhereiAm 'details' API endpoint.
          $(".row", context).live('click', function (e) {
            e.preventDefault();

            var a_id = $(this).find('.fwia---standard-bDesc a').attr('href').split('#')[1];

            $.ajax({
              url: "/forwhereiam/details",
              data: {id: a_id},
              complete: function(xhr) {
                var json = $.parseJSON(xhr.responseText);

                if (json && json.data && json.data.error) {
                  $("#" + block_id + " .fwia---standard-content").addClass("hide");
                  $("#" + block_id + " .fwia---standard-details").removeClass("hide").html('<a href="" class="fwia---standard-close"><b>x</b> close</a><span class="fwia---standard-title fwia---standard-message">' + Drupal.t("This announcement has expired or is no longer available.") + '</span><div class="fwia---standard-clear">&nbsp;</div>');
                }
                else {
                  // Display the announcement details in the .fwia-details div.
                  var details = [];
                  details.push('<a href="#" class="fwia---standard-close"><b>x</b> close</a>');
                  details.push('<div class="fwia---standard-details-container"><span class="fwia---standard-title">');
                  details.push(json.data.announcements.title);
                  details.push('</span>');

                  // Display who issued the announcement.
                  if (json.data.announcements.profileUrl) {
                    details.push('<div class="fwia---standard-clear">&nbsp;</div>');
                    details.push('<div class="fwia---standard-bIssuer"><b>' + Drupal.t('Announcement by') + ':</b> ');
                    details.push('<a href="');
                    details.push(json.data.announcements.profileUrl);
                    details.push('" target="_blank">');
                    details.push(json.data.announcements.issuer);
                    details.push(' </a></div>');
                  }
                  else {
                    details.push('<div class="fwia---standard-clear">&nbsp;</div>');
                    details.push('<div class="fwia---standard-bIssuer"><b>' + Drupal.t('Announcement by') + ':</b> ');
                    details.push(json.data.announcements.issuer);
                    details.push('</div>');
                  }

                  details.push('<br>');
  
                  details.push('<span class="fwia---standard-clientLogo">');
                  details.push((json.data.announcements.imgUrl) ? '<img src="' + json.data.announcements.imgUrl + '" alt="' + json.data.announcements.issuer + '" width="' + json.data.announcements.imgDimensions[0] + '" height="' + json.data.announcements.imgDimensions[1] + '" />' : '');
                  details.push('</span>');

                  details.push(json.data.announcements.description.replace(/\r\n/g,'<br />'));

                  // Display any announcement url.
                  if (json.data.announcements.url) {
                    details.push('<div class="fwia---standard-clear">&nbsp;</div>');
                    details.push('<div><br><b>URL:</b> <a href="');
                    details.push(json.data.announcements.url);
                    details.push('" target="_blank">');
                    details.push(json.data.announcements.url);
                    details.push('</a></div>');
                  }

                  // Show the rate setter and total votes so far,
                  // only if rating is permitted for this announcement.
                  if (show_ratings && json.data.announcements.rate) {
  
                    var temp = [];

                    // Construct the rate setter.
                    temp.push("<div class='fwia---standard-score_this'>(<a data-score='#' data-id='#' href='#' onclick='return false;' >" + Drupal.t("rate this") + "</a>)</div>");
                    temp.push("<div class='fwia---standard-score'>");
                    temp.push("	<a href='#' onclick='return false;' class='fwia---standard-score1' data-id='");
                    temp.push(json.data.announcements.id);
                    temp.push("' data-score=1>1</a>");
                    temp.push("	<a href='#' onclick='return false;' class='fwia---standard-score2' data-id='");
                    temp.push(json.data.announcements.id);
                    temp.push("' data-score=2>2</a>");
                    temp.push("	<a href='#' onclick='return false;' class='fwia---standard-score3' data-id='");
                    temp.push(json.data.announcements.id);
                    temp.push("' data-score=3>3</a>");
                    temp.push("	<a href='#' onclick='return false;' class='fwia---standard-score4' data-id='");
                    temp.push(json.data.announcements.id);
                    temp.push("' data-score=4>4</a>");
                    temp.push("	<a href='#' onclick='return false;' class='fwia---standard-score5' data-id='");
                    temp.push(json.data.announcements.id);
                    temp.push("' data-score=5>5</a>");
                    temp.push("</div>");

                    details.push('<div class="fwia---standard-clear">&nbsp;</div>');

                    // Add the rate setter to the div showing total current votes.
                    details.push('<div class="fwia---standard-rating_holder_div"><div class="fwia---standard-sp_rating">');
                    details.push('<div class="fwia---standard-rating"></div>');
                    details.push('<div class="fwia---standard-base"><div class="fwia---standard-average" style="width:');
                    details.push(json.data.announcements.averageRating + '%">');
                    details.push(json.data.announcements.averageRating + '</div></div>');
                    details.push(' <div class="fwia---standard-votes">' + json.data.announcements.totalVotes + ' votes</div>');
                    details.push('<div class="fwia---standard-sp_status">' + temp.join('') + '</div></div></div>');
                    temp = null;
                  }

                  // Display social sharing buttons if this announcement is allowed to be shared.
                  // Currently the shared link is set to show the announcement on forwhereiam's site.
                  // You may alternatively point the shared link to your own custom page which can
                  // take an announcement ID, pass it to the 'details' endpoint of forWhereiAm's API
                  // and display the results to the user.
                  if (show_sharing_buttons && json.data.announcements.share) {
                    details.push('<div class="fwia---standard-clear">&nbsp;</div>');
                    details.push('<div id="');
                    details.push(block_id);
                    details.push('-fwia-share" class="fwia---standard-share addthis_toolbox addthis_default_style at_small_toolbox" ');
                    details.push('addthis:url="https://forwhereiam.com/results?a=');
                    details.push(json.data.announcements.id);
                    details.push('" addthis:title="' + json.data.announcements.title + '"');
                    details.push(' addthis:description="Announcement by ' + json.data.announcements.issuer + '" >');
                    details.push('<a class="addthis_button_twitter"></a>');
                    details.push('<a class="addthis_button_facebook"></a>');
                    details.push('<a class="addthis_button_linkedin"></a>');
                    details.push('<a class="addthis_button_email"></a>');
                    details.push('<a class="addthis_button_compact"></a>');
                    details.push('</div>');
                  }

                  details.push('<div class="fwia---standard-clear">&nbsp;</div>');

                  details.push(json.data.announcements.terms ? '<span class="fwia---standard-title">' + Drupal.t('Terms and conditions') + '</span><br>' + json.data.announcements.terms : "");

                  // If a map location is associated with the announcement, give a link to reveal a map
                  // only if 'show_map' setting is enabled.
                  if (show_map && json.data.announcements.coordinates) {
                    details.push('<div class="fwia---standard-clear">&nbsp;</div>');
                    details.push('<div class="fwia---standard-map" id="' + block_id + '-fwia-map"></div>');
                  }

                  details.push('<div class="fwia---standard-clear">&nbsp;</div>');
                  details.push('</div>');

                  // Hide the div showing search results and show the div with these details.
                  $(".fwia---standard-content").addClass("hide");
                  $("#" + block_id + " .fwia---standard-details").removeClass("hide").empty().html(details.join(''));

                  // Bind the social sharing buttons toolbox to placeholder.
                  if (show_sharing_buttons && json.data.announcements.share) {

                    if (window.addthis) {
                      window.addthis.init();
                      window.addthis.toolbox('#' + block_id + '-fwia-share');
                    }

                    // The addthis social sharing buttons don't work for facebook/linkedin using
                    // the data which we pass in. These buttons rely on using special meta tags
                    // present in the <head></head> section. So when viewing details of an
                    // announcement we can add these meta tags in, and remove them on closure
                    // of the details view.
                    $('head').find('meta[property="og:title"], meta[property="og:url"], meta[property="og:description"]').remove();

                    var og_meta = [];
                    og_meta.push('<meta property="og:title" content="');
                    og_meta.push(json.data.announcements.title);
                    og_meta.push('" ><meta property="og:url" content="https://forwhereiam.com/results?a=');
                    og_meta.push(json.data.announcements.id);
                    og_meta.push('"><meta property="og:description" content="Announcement by ');
                    og_meta.push(json.data.announcements.issuer);
                    og_meta.push('" >');
                    og_meta.push('<meta property="og:site_name" content="forWhereiAm" >');
                    og_meta.push('<meta property="og:type" content="website" >');

                    $('head').append(og_meta.join(''));

                    og_meta = null;
                  }

                  // Bind event handler to "show map" link.
                  if (show_map && json.data.announcements.coordinates) {
                    $('#' + block_id + '-fwia-map').show();

                    var latlng = new google.maps.LatLng(json.data.announcements.coordinates[0], json.data.announcements.coordinates[1]);
                    var mapProp = {center: latlng, zoom: 15, mapTypeId:google.maps.MapTypeId.ROADMAP};
                    var map = new google.maps.Map(document.getElementById(block_id + '-fwia-map'), mapProp);

                    // To add the marker to the map, call setMap().
                    var marker = new google.maps.Marker({position: latlng});
                    marker.setMap(map);
                  }

                  details = null;
                  a_id = null;
                }
              }
            });

          });

          // Code for handling vote setter.
          $('.fwia---standard-score_this', context).live('click', function(){
            $(this).slideUp();
            return false;
          });
          $('.fwia---standard-score a', context).live('click', function(e) {
            e.preventDefault();
            $(this).parent().parent().parent().addClass('fwia---standard-scored');

            $.ajax({
              url: "/forwhereiam/rate",
              data: {id: $(this).attr("data-id"), vote: $(this).attr("data-score"), ip: $("#" + block_id + " input[name='fwia---standard-ip']").val()},
              complete: function(xhr) {
                var json = $.parseJSON(xhr.responseText);

                if (json && json.data && json.data.error) {
                  $('.fwia---standard-scored').fadeOut("normal", function() {
                    $(this).html('(Please try later)').fadeIn().removeClass('fwia---standard-scored');
                  });
                }
                else {
                  $('.fwia---standard-scored').fadeOut("normal", function() {

                    var results_html = [];

                    if (show_ratings && json.data.totalVotes) {
                      results_html.push("<div class='fwia---standard-rating_wrapper'>");
                      results_html.push('<div class="fwia---standard-sp_rating">');
                      results_html.push('<div class="fwia---standard-rating"></div>');
                      results_html.push('<div class="fwia---standard-base"><div class="fwia---standard-average" style="width:');
                      results_html.push(json.data.averageRating);
                      results_html.push('%">' + json.data.averageRating + '</div></div>');
                      results_html.push('<div class="fwia---standard-votes">');
                      results_html.push(json.data.totalVotes + ' votes</div>');
                      results_html.push('<div class="fwia---standard-sp_status">');

                      if (json.data.state) {
                        results_html.push("(");
                        results_html.push(json.data.state);
                        results_html.push(")");
                      }
                      else {
                        results_html.push("<div class='fwia---standard-score'>");
                        for (var b = 1; b < 6; b++) {
                          results_html.push("<a class='fwia---standard-score" + b);
                          results_html.push("' vote='?announcement=");
                          results_html.push(json.data.id);
                          results_html.push("&score=" + b);
                          results_html.push(">" + b + "</a>");
                        }
                        results_html.push("</div>");
                      }

                      results_html.push('</div></div>');
                      results_html.push("</div>");
                    }
                    else {
                      results_html.push("(");
                      results_html.push(json.data.state);
                      results_html.push(")");
                    }

                    $(this).html(results_html.join('')).fadeIn().removeClass('fwia---standard-scored');
                    results_html = null;
                  });
                }
              }
            });

          });

        // Bind 'close details' link.
        $(".fwia---standard-close", context).live('click', function (e) {
          e.preventDefault();

          // Cause any announcement view window to close and show the search listings view.
          $("#" + block_id + " .fwia---standard-details").empty().addClass("hide");

          // Show the results div again.
          $('#' + block_id + ' .fwia---standard-search').removeClass("hide");

          // Remove the special meta data added to the head section for addthis buttons to work.
          if (show_sharing_buttons) {
            $('head').find('meta[property="og:title"], meta[property="og:url"], meta[property="og:description"]').remove();
          }

          $('#' + block_id).trigger("fwia:load");
        });

        // Bind the location input field.
        $('#' + block_id + ' form', context).live('submit', function(e) {
          e.preventDefault();

          _fwia_createCookie("fwia---standard-location", $('#' + block_id + ' .fwia---standard-location').val(), 1);

          // Cause any announcement view window to close and show the search listings view.
          $("#" + block_id + " .fwia---standard-details").empty().addClass("hide");

          // Show the results div again.
          $("#" + block_id + " .fwia---standard-search").removeClass("hide");

          var fwia_widget_data_store = $('#' + block_id)[0];
          $.removeData(fwia_widget_data_store);

          // Clear both search results as have new location - which means both sets of
          // 'info' and 'offers' results will change.
          $("#" + block_id + " .fwia---standard-results").empty();

          $('#' + block_id).trigger("fwia:load");
        });

        // Support geolocation.
        // Code to control the 'guess my location' link.
        var geolocation = $('#' + block_id + ' .fwia---standard-geolocation');
        if (geolocate && window.navigator.geolocation) {
          geolocation.css('display', 'inline');
          $('.fwia---standard-geolocation', context).live('click', function() { _findMe(); return false; });
        }
        else {
          geolocation.hide();
        }

        // Will hold the time interval after which to automatically
        // run the "fwia-load" event again.
        var timer;

        // This code binds the fwia:load event to the widget.
        // This code handles the ajax search requests the widget makes to the forWhereiAm server.
        $('#' + block_id, context).live("fwia:load", function() {

          var after = null;

          // If viewing an announcement details, then don't refresh widget
          // (i.e. try fetching new results from the forWhereiAm servers) as it
          // causes it to loose its close button, etc.
          if ($('#' + block_id + ' .fwia---standard-search').hasClass('hide')) {
            return;
          }

          var fwia_widget_data_store = $('#' + block_id)[0];
          var tab_container = $('#' + block_id + ' .fwia---standard-search');

          var loc = _fwia_readCookie("fwia---standard-location");
          if (!loc || loc === "") {
            tab_container.find('.fwia---standard-results').empty().html('<p class="fwia---standard-message">' + Drupal.t('Please enter your postcode to see relevant announcements.') + '</p>');
            _fwia_eraseCookie("fwia---standard-location");
            $.removeData(fwia_widget_data_store);

            // Hide the 'sign up for alerts' link, if shown. As need valid location for it too.
            $('#' + block_id + ' .fwia---standard-signup').hide();
            return;
          }


          // After holds a timestamp representing what time the last set of results were fetched until.
          //
          // It is a timestamp returned by the forWhereiAm API after you make a search request.
          // For the very first search request, you will have no value for this. In that case the forWhereiAm
          // API will return all results if finds up to the time the request was executed, and return a timestamp
          // with the results. On any subsequent requests you make (e.g. via auto-refresh of the widget)
          // if you also pass the 'after' timestamp to the forWhereiAm API, it will only return any newer results
          // if finds after this time.
          after = $.data(fwia_widget_data_store, 'after');

          var prepend = true;
          if (typeof after == 'undefined') {
            prepend = false;
          }

          // If don't have any previous search results showing on the widget, then show the 'Loading' message.
          if (!prepend) {
            tab_container.find('.fwia---standard-results').empty().append('<span class="fwia---standard-loading">' + Drupal.t('Loading...') + '</span>');
          }

          $.ajax({
            url: "/forwhereiam/search",
            data: $('#' + block_id + ' form').serialize() + '&after=' + after,
            complete: function(xhr) {
              var json = $.parseJSON(xhr.responseText);

              if (json && json.data && json.data.error) {
                if (json.data && json.data.error_description) {
                  tab_container.find('.fwia---standard-results').empty().html('<p class="fwia---standard-message">' + json.data.error_description + '</p>');
                  _fwia_eraseCookie('fwia---standard-location');

                  // Hide the 'sign up for alerts' link, if shown. As need valid location for it too.
                  $('#' + block_id + ' .fwia---standard-signup').hide();
                }
                else {
                  tab_container.find('.fwia---standard-results').empty().html('<p class="fwia---standard-message">' + Drupal.t('Please check back later.') + '</p>');
                }

                $.removeData(fwia_widget_data_store);
              }
              else {
                if (!json.data.announcements || json.data.announcements.length === 0) {
                  if (!prepend) {
                    tab_container.find('.fwia---standard-results').empty().html('<p class="fwia---standard-message">' + Drupal.t('There are currently no relevant announcements for you. Please check again later.') + '</p>');
                  }
                }
                else {
                  _fwia_list_announcements(json.data.announcements, json.data._metadata.lastCheckAt, prepend, tab_container);
                }

                // Allow alert signup as assume we have a location. Only if signup setting enabled.
                if (signup) {
                  $('#' + block_id + ' .fwia---standard-signup').css('display', 'inline');
                }

                // Retrigger fetch for new results after 60 seconds.
                // Record timer so we can clear any previously set timer before applying a new one.
                window.clearTimeout(timer);
                var interval = refresh_interval;
                if (interval < 60) {
                  interval = 60;
                }
                interval = interval * 1000;
                timer = window.setTimeout(function() { $('#' + block_id).trigger("fwia:load"); }, interval);
              }
            }
          });

        });

        // On page load/refresh, check if have any location already stored in cookie. If yes,
        // then we can use it immediately to show relevant results automatically.
        var location = _fwia_readCookie("fwia---standard-location");
        if (location) {
          $('#' + block_id + ' .fwia---standard-location').val(location);
          var fwia_widget_data_store = $('#' + block_id)[0];
          // Remove any data stored with the element (could be the 'after' variable).
          $.removeData(fwia_widget_data_store);
          $('#' + block_id).removeData().trigger("fwia:load");
        }
        else {
          // Hide the 'sign up for alerts' link, if shown. As need valid location for it too.
          $('#' + block_id + ' .fwia---standard-signup').hide();
        }
        location = null;

      }); // end of $.once

    } // end of attach

  };

})(jQuery);

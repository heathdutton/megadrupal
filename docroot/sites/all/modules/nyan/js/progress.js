(function ($) {

/**
 * A progressbar object. Initialized with the given id. Must be inserted into
 * the DOM afterwards through progressBar.element.
 *
 * method is the function which will perform the HTTP request to get the
 * progress bar state. Either "GET" or "POST".
 *
 * e.g. pb = new progressBar('myProgressBar');
 *      some_element.appendChild(pb.element);
 */
Drupal.progressBar = function (id, updateCallback, method, errorCallback) {
  var pb = this;
  this.id = id;
  this.method = method || 'GET';
  this.updateCallback = updateCallback;
  this.errorCallback = errorCallback;

  // The WAI-ARIA setting aria-live="polite" will announce changes after users
  // have completed their current activity and not interrupt the screen reader.
  this.element = $('<div class="progress" aria-live="polite"></div>').attr('id', id);
  this.element.html('<div id="nyanContainer">' +
                      '<div id="nyanCat"><div id="wholeHead"><div class="skin ear"></div><div class="skin ear rightEar"></div><div id="mainHead" class="skin"><div class="eye"></div><div class="eye rightEye"></div><div class="nose"></div><div class="chick"></div><div class="chick rightChick"></div><div class="mouth">E</div></div></div><div id="toastBody"><div>&nbsp;  &nbsp; &nbsp;.&nbsp;&nbsp;.&nbsp; &nbsp; &nbsp;..&nbsp;  &nbsp; &nbsp;.&nbsp;.&nbsp; &nbsp; &nbsp;&nbsp&nbsp&nbsp;..&nbsp;  &nbsp; &nbsp;.&nbsp;&nbsp;.</div></div><div id="wholeTail"><div class="tail skin"></div><div class="tail middleTail skin"></div><div class="tail backTail skin"></div></div><div id="allYourLegAreBelongToUs"><div class="skin leg back leftBack"></div><div class="skin leg back"></div><div class="skin leg front leftFront"></div><div class="skin leg front"></div></div><div class="rainbow"></div><div class="rainbow r2"></div><div class="rainbow r3"></div><div class="rainbow r4"></div><div class="rainbow r5"></div><div class="rainbow r6"></div><div class="rainbow r7"></div><div class="rainbow r8"></div><div class="rainbow r9"></div><div class="rainbow r10"></div></div></div>' +
                      '<div class="star starMovement1"><div number="1"></div><div number="2"></div><div number="3"></div><div number="4"></div><div number="5"></div><div number="6"></div><div number="7"></div><div number="8"></div></div><div class="star starMovement2 backwards"><div number="1"></div><div number="2"></div><div number="3"></div><div number="4"></div><div number="5"></div><div number="6"></div><div number="7"></div><div number="8"></div></div><div class="star starMovement3"><div number="1"></div><div number="2"></div><div number="3"></div><div number="4"></div><div number="5"></div><div number="6"></div><div number="7"></div><div number="8"></div></div> <div class="star starMovement4"><div number="1"></div><div number="2"></div><div number="3"></div><div number="4"></div><div number="5"></div><div number="6"></div><div number="7"></div><div number="8"></div></div><div class="star starMovement5"><div number="1"></div><div number="2"></div><div number="3"></div><div number="4"></div><div number="5"></div><div number="6"></div><div number="7"></div><div number="8"></div></div>' +
                    '</div>' +
                    '<div class="percentage"></div>' +
                    '<div class="message">&nbsp;</div>');

  // Add an ID to the body for styling purposes.
  $('body').addClass('nyan');

  // Add in the audio if it is enabled.
  if (Drupal.settings.nyan.audio.enabled == 1) {
    $('body').append('<audio loop="loop" autoplay="true" id="nyanaudio">' +
                       '<source src="' + Drupal.settings.nyan.audio.mp3 + '" type="audio/mpeg">' +
                       '<source src="' + Drupal.settings.nyan.audio.ogg + '" type="audio/ogg">' +
                     '</audio>');

    // Add controls if needed.
    if (Drupal.settings.nyan.audio.show_controls == 1) {
      document.getElementById('nyanaudio').controls = 'true';
    }

    // Change the volume, as it sounds a bit messy at full volume.
    document.getElementById('nyanaudio').volume = Drupal.settings.nyan.audio.initial_volume;
  }
};

/**
 * Set the percentage and status message for the progressbar.
 */
Drupal.progressBar.prototype.setProgress = function (percentage, message) {
  if (percentage >= 0 && percentage <= 100) {
    $('div.percentage', this.element).html(percentage + '%');
    $('#nyanContainer', this.element).css('left', percentage + '%');
  }
  $('div.message', this.element).html(message);
  if (this.updateCallback) {
    this.updateCallback(percentage, message, this);
  }
};

/**
 * Start monitoring progress via Ajax.
 */
Drupal.progressBar.prototype.startMonitoring = function (uri, delay) {
  this.delay = delay;
  this.uri = uri;
  this.sendPing();
};

/**
 * Stop monitoring progress via Ajax.
 */
Drupal.progressBar.prototype.stopMonitoring = function () {
  clearTimeout(this.timer);
  // This allows monitoring to be stopped from within the callback.
  this.uri = null;
};

/**
 * Request progress data from server.
 */
Drupal.progressBar.prototype.sendPing = function () {
  if (this.timer) {
    clearTimeout(this.timer);
  }
  if (this.uri) {
    var pb = this;
    // When doing a post request, you need non-null data. Otherwise a
    // HTTP 411 or HTTP 406 (with Apache mod_security) error may result.
    $.ajax({
      type: this.method,
      url: this.uri,
      data: '',
      dataType: 'json',
      success: function (progress) {
        // Display errors.
        if (progress.status == 0) {
          pb.displayError(progress.data);
          return;
        }
        // Update display.
        pb.setProgress(progress.percentage, progress.message);
        // Schedule next timer.
        pb.timer = setTimeout(function () { pb.sendPing(); }, pb.delay);
      },
      error: function (xmlhttp) {
        pb.displayError(Drupal.ajaxError(xmlhttp, pb.uri));
      }
    });
  }
};

/**
 * Display errors on the page.
 */
Drupal.progressBar.prototype.displayError = function (string) {
  var error = $('<div class="messages error"></div>').html(string);
  $(this.element).before(error).hide();

  if (this.errorCallback) {
    this.errorCallback(this);
  }
};

})(jQuery);

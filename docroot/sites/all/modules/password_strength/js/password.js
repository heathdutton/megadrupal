(function ($) {
  "use strict";
  /**
   * Attach handlers to evaluate the strength of any password fields.
   */
  Drupal.behaviors.PasswordStrengthCheck = {
    attach: function (context, settings) {
      $('input.password-field', context).once('password-strength-check', function () {

        var passwordCheck, position,
            request_data, required_score = queryString['password_score'];

        // Create password check dom elements and apply them around the password field.
        var $self = $(this),
            $container = $('<div class="password-strength-check"></div>'),
            $strength_bar = $('<div class="password-strength-strength-bar"><div class="bar"><div class="value"></div></div></div>'),
            $message_strength = $('<div class="password-strength-message-strength clearfix"></div>'),
            $message_requirements = $('<div class="password-strength-message-requirements"></div>'),
            $message_flaws = $('<div class="password-strength-message-flaws"></div>');
        $self.wrap($container);
        $self.after($message_flaws).after($message_requirements).after($message_strength).after($strength_bar);

        // Hide the message elements.
        $message_strength.hide();
        $message_flaws.hide();

        var passwordCheck = function (e, isCallback) {
          if (typeof isCallback != 'undefined') {
            return;
          }

          e.stopImmediatePropagation();

          request_data = { password: encodeURIComponent($self.val()), token: Drupal.settings.passwordStrength.token, uid: Drupal.settings.passwordStrength.uid };
          if (required_score) {
            request_data['password_score'] = required_score;
          }

          // Check password strength.
          $.post(
            Drupal.settings.passwordStrength.secure_base_url + 'system/password-strength-check',
            request_data,
            function(data) {

              // Password field is empty.
              var is_empty = (!$self.val());

              // Password meets the strength requirements.
              var is_strong = (data.score >= data.score_required && data.score_required != 0);

              // Set strength bar width and color.
              $strength_bar.find('.value')
                .removeClass('bg-score-0 bg-score-1 bg-score-2 bg-score-3 bg-score-4')
                .addClass('bg-score-' + data.score)
                .css('width', data.percent + '%');

              // Set message content.
              $message_strength.html(data.message_strength);
              $message_requirements.html(data.message_requirements);
              $message_flaws.html(data.message_flaws);

              // Show the strength message.
              if ($message_strength.is(':hidden')) {
                $message_strength.slideDown();
              }

              // Show the requirements message.
              if ($message_requirements.is(':hidden')) {
                $message_requirements.slideDown();
              }

              // Hide the flaws if password is strong enough.
              if (is_strong && $message_flaws.is(':visible')) {
                $message_flaws.slideUp();
              }
              // Hide the flaws is password is empty.
              else if (is_empty && $message_flaws.is(':visible')) {
                $message_flaws.slideUp();
              }
              // Show the flaws if password is not empty.
              else if (!is_empty && !is_strong && $message_flaws.is(':hidden')) {
                $message_flaws.slideDown();
              }

            }
          );

        };

        var position = function () {
          // Position the strength meter inside of the password field and adjust
          var width = $self.outerWidth(),
              height = $self.outerHeight(),
              bar_height = $strength_bar.outerHeight();
          $strength_bar.css({ width: width - 6, left: 3, top: height - bar_height - 3 });
          $message_strength.css({ width: width });
        };

        // Reposition the element after transitioning.
        $(window).bind('resize transitionend', function(){
          position();
        });

        // Prevent evaluating password right away on each keystroke, instead wait
        // for a bit and send the updated password in less frequent batches.
        $self.bindWithDelay('keyup focusin', passwordCheck, 500, true);

        // Trigger the passwordCheck right away when js initializes if value is not empty.
        if ($self.val()) {
          $self.trigger('focusin');
        }

        // Position the elements on the page.
        position();

      });

      $('input.password-confirm', context).once('password-strength-check-match', function () {
        // Create password check match dom elements and apply them.
        var $self = $(this),
          $container = $('<div class="password-strength-check-match"></div>'),
          $message = $('<div class="password-strength-check-match-message"></div>');

        $self.wrap($container);
        $self.after($message);

        var passwordCheckMatch = function (e, isCallback) {
          if (typeof isCallback != 'undefined') {
            return;
          }

          e.stopImmediatePropagation();

          if ($self.val()) {
            if ($self.val() === $('input.password-field').val()) {
              $message.html(Drupal.t('Passwords match.'));
              $message.slideDown();
            }
            else {
              $message.html(Drupal.t('Passwords do not match.'));
              $message.slideDown();
            }
          }
          else {
            $message.slideUp();
          }
        }

        // Prevent evaluating password right away on each keystroke.
        $self.bindWithDelay('keyup focusin', passwordCheckMatch, 100, true);

      });

    }
  };

  /**
   * Define an alternative to bind function that will delay execution.
   */
  $.fn.bindWithDelay = function(type, data, fn, timeout, throttle) {

    if ( $.isFunction( data ) ) {
      throttle = timeout;
      timeout = fn;
      fn = data;
      data = undefined;
    }

    // Allow delayed function to be removed with fn in unbind function
    fn.guid = fn.guid || ($.guid && $.guid++);

    // Bind each separately so that each element has its own delay
    return this.each(function() {
      var wait = null;

      function cb() {
        var e = $.extend(true, { }, arguments[0]);
        var ctx = this;
        var throttler = function() {
          wait = null;
          fn.apply(ctx, [e]);
        };

        if (!throttle) { clearTimeout(wait); wait = null; }
        if (!wait) { wait = setTimeout(throttler, timeout); }
      }

      cb.guid = fn.guid;

      $(this).bind(type, data, cb);
    });

  };

  // Parse query string from URL.
  var queryString = (function(a) {
    if (a == "") return {};
    var b = {};
    for (var i = 0; i < a.length; ++i) {
      var p=a[i].split('=');
      if (p.length != 2) continue;
      b[p[0]] = decodeURIComponent(p[1].replace(/\+/g, " "));
    }
    return b;
  })(window.location.search.substr(1).split('&'))

})(jQuery);
